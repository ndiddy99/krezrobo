#include "ngpc.h"		// required
#include "carthdr.h"		// required
#include "library.h"		// NGPC routines
#include "tiles.c" //graphics
#include "map.h" //maps
#include "main.h" //structs
#include <stdlib.h>		// std C routines
#include <stdio.h>


#define TILEMAP_OFFSET 128 //offset in tilemap due to system font taking up first half
u8 i,j,mapNum,slowCounter,numRobots,playerAnimCounter,robotStandingAnimCounter,robotWalkingAnimCounter,
isRobotMoving,robotToMove,robotMotionDelay,robotWalkCounter,lives,tempScreenCounter;
u16 currentTile,score;

SPRITE player;
PROJECTILE playerShot;
PROJECTILE robotShot;
#define MAX_NUM_ROBOTS 10
SPRITE robots[MAX_NUM_ROBOTS];

void initGame(void);
void drawMap(u8 mapNum);
u8 checkPlayerBGCollision(void);
u8 checkShotBGCollision(PROJECTILE shot);
void setPlayerStartingPoint(u8 location);
void handlePlayerMovement(void);
void animatePlayerMovement(void);
void handlePlayerShot(void);
void handlePlayerShotCollision(PROJECTILE* shot);
void handlePlayerDeath(void);
void switchScreens(u16 direction);
void initRobots(void);
void spawnRobots(u8 numRobots);
u8 checkRobotBGCollision(SPRITE robot);
void drawRobots(u8 numRobots);
void animateRobots(u8 numRobots);
void handleRobotShotCollision(PROJECTILE* shot);
void despawnRobot(SPRITE* robot);
void setRobotDirection(SPRITE* robot);
void moveRobot(SPRITE* robot,u8 speed);
void handleRobotMovement(u8 speed);
void shootPlayer(SPRITE robotShooting);

void main(void) {
	InitNGPC();
	SysSetSystemFont();
	InstallTileSetAt(tiles,sizeof(tiles)/2,TILEMAP_OFFSET);
	initGame();

	while (1) {
		handlePlayerMovement();
		handlePlayerShot();	
		animatePlayerMovement();	
		
		SetSprite(playerShot.spriteID,TILEMAP_OFFSET+playerShot.tileNum,0,playerShot.xPos,playerShot.yPos,playerShot.palette); //draw player projectile 
		SetSprite(robotShot.spriteID,TILEMAP_OFFSET+robotShot.tileNum,0,robotShot.xPos,robotShot.yPos,robotShot.palette); //draw robot projectile
		SetSprite(player.spriteID,TILEMAP_OFFSET+player.tileNum,0,player.xPos,player.yPos,player.palette); //draw player sprite
		SetSprite(player.spriteID+1,TILEMAP_OFFSET+player.tileNum+1,1,0,8,0); //draw bottom player sprite
		
		handlePlayerShotCollision(&robotShot);
		handleRobotShotCollision(&playerShot);
			
		animateRobots(numRobots);
		drawRobots(numRobots);
		handleRobotMovement(3);
		shootPlayer(robots[robotToMove]);
		
		//movement between screens
		if (player.xPos==0) { //exit to the left
			switchScreens(0);
		}
		else if (player.xPos==152) { //exit to the right
			switchScreens(2);
		}
		else if (player.yPos==250) { //exit to the top
			switchScreens(1);
		}
		else if (player.yPos==110) { //exit to the bottom
			switchScreens(3);
		}
		
		//show lives and score
		PrintNumber(SCR_1_PLANE,0,6,15,lives,1);
		PrintNumber(SCR_1_PLANE,0,6,16,score,GetNumDigits(score));
		
		WaitVsync();
		
	}
}

void initGame() {
	ClearScreen(SCR_1_PLANE);
	ClearScreen(SCR_2_PLANE);
	SetBackgroundColour(RGB(0, 0, 0));
	SetPalette(SCR_2_PLANE, 0, 0, RGB(3,2,9), RGB(0,0,0), RGB(0,0,0));
	SetPalette(SCR_1_PLANE, 0, 0, RGB(15,15,15), RGB(15,15,15), RGB(15,15,15));
	SetPalette(SPRITE_PLANE, 0, 0, RGB(15,15,15), RGB(15,0,2), RGB(15,15,0)); //player palette
	SetPalette(SPRITE_PLANE,1,0,RGB(0,15,0),RGB(15,15,0),RGB(15,15,15)); //robot palette
	SetSprite(player.spriteID,TILEMAP_OFFSET+player.tileNum,0,player.xPos,player.yPos,player.palette);
	SetSprite(player.spriteID+1,TILEMAP_OFFSET+player.tileNum+1,1,0,8,0); //second sprite is linked to the first one
	SetSprite(playerShot.spriteID,TILEMAP_OFFSET+playerShot.tileNum,0,playerShot.xPos,playerShot.yPos,playerShot.palette);
	
	player.spriteID=0;
	player.tileNum=playerStandingL;
	setPlayerStartingPoint(0);
	player.palette=0;
	player.direction=4;
	lives=3;
	score=0;
	
	playerShot.spriteID=2;
	playerShot.tileNum=horizontalShot;
	playerShot.xPos=200;
	playerShot.yPos=0;
	playerShot.palette=0;
	playerShot.hasBeenFired=0;
	
	numRobots=5;
	robotStandingAnimCounter=0;
	robotWalkingAnimCounter=0;
	isRobotMoving=0;
	robotToMove=0;
	robotWalkCounter=0;
	initRobots();
	
	robotShot.spriteID=robots[MAX_NUM_ROBOTS-1].spriteID+2;
	robotShot.tileNum=horizontalShot;
	robotShot.xPos=200;
	robotShot.yPos=0;
	robotShot.palette=1;
	robotShot.hasBeenFired=0;
	
	SeedRandom();
	switchScreens(GetRandom(3));
	spawnRobots(numRobots);
	currentTile=0;
	slowCounter=0;
	PrintString(SCR_1_PLANE,0,0,15,"Lives:");
	PrintString(SCR_1_PLANE,0,0,16,"Score:");
}

void drawMap(u8 mapNum) {
	for (i=0; i < MAP_SIZE_X;++i) {
		for (j=0; j < MAP_SIZE_Y;++j) {
			PutTile(SCR_2_PLANE,0,i,j,TILEMAP_OFFSET+maps[mapNum][j][i]);
		}
	}	
}
u8 checkPlayerBGCollision() {
	GetTile(SCR_2_PLANE,NULL,(player.xPos+4)>>3,(player.yPos+8)>>3,&currentTile); //collision detection
	return (currentTile-TILEMAP_OFFSET)!=blank; //1 if collision, 0 if not
}

u8 checkShotBGCollision(PROJECTILE shot) {
	GetTile(SCR_2_PLANE,NULL,(shot.xPos+4)>>3,(shot.yPos+4)>>3,&currentTile); //collision detection
	return (currentTile-TILEMAP_OFFSET)!=blank; //1 if collision, 0 if not	
}

void setPlayerStartingPoint(u8 location) {
	//location: 0=left,1=top,2=right,3=bottom
	switch(location) {
		case 0:
		player.xPos=10;
		player.yPos=45;
		break;
		case 1:
		player.xPos=75;
		player.yPos=5;
		break;
		case 2:
		player.xPos=140;
		player.yPos=45;
		break;
		case 3:
		player.xPos=75;
		player.yPos=90;
		break;
	}
	
}

void handlePlayerMovement() {
/* player direction:
			 1 2 3
			  \|/
			0 - - 4
			  /|\
			 7 6 5
*/			
	if (slowCounter) {	
		if (!JOYPAD)
			player.isMoving=0;
		else
			player.isMoving=1;
		if (JOYPAD & J_LEFT) {
			if (JOYPAD & J_UP) {	//up-left
				--player.xPos;
				--player.yPos;
				player.direction=1;
			}
			else if (JOYPAD & J_DOWN) { //down-left
				--player.xPos;
				++player.yPos;
				player.direction=7;
			}				
			else { //left
				--player.xPos;
				player.direction=0;
			}
		}
		else if (JOYPAD & J_RIGHT) {
			if (JOYPAD & J_UP) { //up-right
				++player.xPos;
				player.yPos--;
				player.direction=3;
			}
			else if (JOYPAD & J_DOWN) { //down-right
				++player.xPos;
				++player.yPos;
				player.direction=5;
			}				
			else { //right
				++player.xPos;
				player.direction=4;
			}
		}
		else if (JOYPAD & J_UP) { //up
			player.yPos--;
			player.direction=2;
		}
		else if (JOYPAD & J_DOWN) { //down
			++player.yPos;
			player.direction=6;
		}
		if (checkPlayerBGCollision())
			handlePlayerDeath();
	slowCounter=0;
	}
	else
		slowCounter=1;
}

void animatePlayerMovement() {
	if (playerAnimCounter) {
		if (player.isMoving) {
			switch (player.direction) {
				case 0:
				case 1:
				case 7:
				if (player.tileNum < playerStandingR || player.tileNum >= playerWalkingR2)
					player.tileNum=playerStandingR;
				else
					player.tileNum+=2;	
				break;
				default:
				if (player.tileNum < playerStandingL || player.tileNum >= playerWalkingL2)
					player.tileNum=playerStandingL;
				else 
					player.tileNum+=2;
				break;
			}
		}
		else {
			switch (player.direction) {
				case 0:
				case 1:
				case 7:		
					player.tileNum=playerStandingR;
				break;
				default:
					player.tileNum=playerStandingL;
				break;
			}
		}
		playerAnimCounter=0;
	}
	else
		playerAnimCounter++;
}

void handlePlayerShot() {
	if ((JOYPAD & J_A) && !playerShot.hasBeenFired) { //determines shot starting location
		playerShot.xPos=player.xPos;
		playerShot.yPos=player.yPos+4;
		playerShot.direction=player.direction;
		if (playerShot.direction==2 || playerShot.direction==6)
			playerShot.tileNum=verticalShot;
		else
			playerShot.tileNum=horizontalShot;
		playerShot.hasBeenFired=1;
	}

	if (playerShot.hasBeenFired) {
		switch(playerShot.direction) {
			case 0:
			playerShot.xPos-=2;
			break;
			case 1:
			playerShot.xPos-=2;
			playerShot.yPos-=2;
			break;
			case 2:
			playerShot.yPos-=2;
			break;
			case 3:
			playerShot.xPos+=2;
			playerShot.yPos-=2;
			break;
			case 4:
			playerShot.xPos+=2;
			break;
			case 5:
			playerShot.xPos+=2;
			playerShot.yPos+=2;
			break;
			case 6:
			playerShot.yPos+=2;
			break;
			case 7:
			playerShot.xPos-=2;
			playerShot.yPos+=2;
			break;
		}
	}
	
	else { //if done firing, move shot offscreen
		playerShot.xPos=200;
		playerShot.yPos=0;
	}
	if (playerShot.xPos > 160 || playerShot.yPos > 110 || checkShotBGCollision(playerShot))
		playerShot.hasBeenFired=0;
}

void switchScreens(u16 direction) {
	//direction: 0: left exit, 1: top exit, 2: right exit, 3: bottom exit
	SeedRandom(); //seed rng
	switch(direction) {
		case 0:
		drawMap(sideEnterMaps[GetRandom(3)]);
		setPlayerStartingPoint(2);
		break;
		case 1:
		drawMap(bottomEnterMaps[GetRandom(5)]);
		setPlayerStartingPoint(3);
		break;
		case 2:
		drawMap(sideEnterMaps[GetRandom(3)]);
		setPlayerStartingPoint(0);
		break;
		case 3:
		drawMap(topEnterMaps[GetRandom(3)]);
		setPlayerStartingPoint(1);
		break;
	}
	spawnRobots(numRobots);
}

void handlePlayerShotCollision(PROJECTILE* shot) {
	if ((shot->xPos+4) > player.xPos && (shot->xPos+5) < player.xPos+9) {
		if ((shot->yPos+4) > player.yPos && (shot->yPos+5) < player.yPos+10) {
			handlePlayerDeath();
			shot->hasBeenFired=0;
		}
	}
}

void handlePlayerDeath() { //todo: add death animation
	if (lives!=0) {
		--lives;
		switchScreens(GetRandom(3));
	}
	else {
		tempScreenCounter=100;
		PrintString(SCR_1_PLANE,0,6,6,"Game over");
		while (tempScreenCounter!=0) {
			tempScreenCounter--;
			WaitVsync();
		} //todo: initgame should show title screen eventually
		initGame();
	}
}

void initRobots() {
#define NUM_PLAYER_SPRITES 3 //2 for player character, 1 for player projectile
	for (i=0; i < MAX_NUM_ROBOTS; ++i) {
		robots[i].spriteID=NUM_PLAYER_SPRITES+i*2; //i*2 because there's another sprite being used by the bottom of the robot
		robots[i].tileNum=robotStanding1;
		robots[i].xPos=0;
		robots[i].yPos=0;
		robots[i].palette=1;
		robots[i].isMoving=0;
		robots[i].isAlive=1;
	}	
}

void spawnRobots(u8 numRobots) {
	SeedRandom();
	for (i=0; i < numRobots; ++i) {
		do {  //robots shouldn't spawn inside walls
			robots[i].xPos=GetRandom(130)+10; //between 10 and 140
			robots[i].yPos=GetRandom(80)+10; //between 10 and 90
		} while (checkRobotBGCollision(robots[i]));
		robots[i].isAlive=1;
	}
}

u8 checkRobotBGCollision(SPRITE robot) {
	GetTile(SCR_2_PLANE,NULL,(robot.xPos+4)>>3,(robot.yPos+4)>>3,&currentTile); //collision detection
	return (currentTile-TILEMAP_OFFSET)!=blank; //1 if collision, 0 if not
}

void drawRobots(u8 numRobots) {
	for (i=0; i < numRobots; ++i) {
		SetSprite(robots[i].spriteID,TILEMAP_OFFSET+robots[i].tileNum,0,robots[i].xPos,robots[i].yPos,robots[i].palette);
		SetSprite(robots[i].spriteID+1,TILEMAP_OFFSET+robots[i].tileNum+1,1,0,8,robots[i].palette);
	}
}

void animateRobots(u8 numRobots) {
#define ROBOT_STANDING_ANIMATION_DELAY 5
	if (robotStandingAnimCounter==ROBOT_STANDING_ANIMATION_DELAY) {  
		for (i=0; i < numRobots; ++i) {
			if (!robots[i].isMoving) {
				if (robots[i].tileNum < robotStanding1 || robots[i].tileNum >= robotStanding6)
					robots[i].tileNum=robotStanding1;
				else
					robots[i].tileNum+=2;
			}
		}
		robotStandingAnimCounter=0;
	}
	else
		robotStandingAnimCounter++;
}

void handleRobotShotCollision(PROJECTILE* shot) {
	for (i=0; i < numRobots; ++i) {
		if (robots[i].xPos !=0) { //if robot is on the playfield
			if ((shot->xPos+4) > robots[i].xPos && (shot->xPos+5) < robots[i].xPos+9) {
				if ((shot->yPos+4) > robots[i].yPos && (shot->yPos+5) < robots[i].yPos+10) {
					despawnRobot(&robots[i]);
					shot->hasBeenFired=0;
				}
			}
		}
	}
}

void despawnRobot(SPRITE* robot) {
	robot->xPos=180; //move robot offscreen (todo: add robot death animation)
	robot->isMoving=0;
	robot->isAlive=0;
	score+=50; //increase score
}

void setRobotDirection(SPRITE* robot) { //speed is a value from 0 to 5
	if (robot->xPos >= player.xPos && robot->yPos >=player.yPos) { //robot is to the bottom-right of player
		if (robot->xPos-player.xPos > robot->yPos-player.yPos) //if robot is closer to right
			robot->direction=0; //go left
		else //if robot's closer to the bottom, go up
			robot->direction=2; 
	}
	else if (robot->xPos < player.xPos && robot->yPos >=player.yPos) { //robot is to the bottom left of player
		if (player.xPos-robot->xPos > robot->yPos-player.yPos) //if robot is closer to left
			robot->direction=4; //go right
		else //if robot's closer to the bottom, go up
			robot->direction=2;
	}
	else if (robot->xPos >= player.xPos && robot->yPos < player.yPos) { //robot is to the top-right of player
		if (robot->xPos-player.xPos > player.yPos-robot->yPos) //if robot is closer to the right
			robot->direction=0; //go left
		else //if robot's closer to the top, go down
			robot->direction=6;	
	}
	else if (robot->xPos < player.xPos && robot->yPos < player.yPos) { //robot is to the top-left of player
		if (player.xPos-robot->xPos > player.yPos-robot->yPos) //if robot is closer to the right
			robot->direction=4; //go right
		else //if robot's closer to the top, go down
			robot->direction=6;	
	}
//	PrintNumber(SCR_1_PLANE,0,0,15,robot->direction,3);
}

void moveRobot(SPRITE* robot, u8 speed) { //valid speed values are 1-5
#define ROBOT_WALKING_ANIMATION_DELAY 10

	if (robotMotionDelay > 5-speed) {
		if (robot->isMoving && robot->xPos > 0 && robot->xPos < 152 && robot->yPos > 0 && robot->yPos < 110) {
			switch (robot->direction) {
				case 0: //left
				robot->xPos-=2;
				if (checkRobotBGCollision(*robot))
					despawnRobot(robot); //kill robot if it touches a wall
				break;
				case 2: //up
				robot->yPos-=2;
				if (checkRobotBGCollision(*robot))
					despawnRobot(robot);
				break;
				case 4: //right
				robot->xPos+=2;
				if (checkRobotBGCollision(*robot))
					despawnRobot(robot);
				break;
				case 6: //down
				robot->yPos+=2;
				if (checkRobotBGCollision(*robot))
					despawnRobot(robot);
				break;
			}
			shootPlayer(*robot);
		}
		else
			isRobotMoving=0;
		robotMotionDelay=0;
	}
	else
		robotMotionDelay++;
	if (robotWalkingAnimCounter>ROBOT_WALKING_ANIMATION_DELAY) {
		if (robot->tileNum==robotWalking1)
			robot->tileNum=robotWalking2;
		else
			robot->tileNum=robotWalking1;
		robotWalkingAnimCounter=0;
	}
	else
		robotWalkingAnimCounter++;
}
void handleRobotMovement(u8 speed) {
	if (!isRobotMoving) {				//if a robot isn't moving, set the closest one to the player moving
	
		u8 closestRobot=0;
		for (i=0; i < numRobots; ++i) {
			if (min(GetDifference(robots[i].xPos,player.xPos),GetDifference(robots[i].yPos,player.yPos)) <
				min(GetDifference(robots[closestRobot].xPos,player.xPos),GetDifference(robots[closestRobot].yPos,player.yPos)) && robots[i].isAlive) {
					closestRobot=i;
			}
		}	
		PrintNumber(SCR_1_PLANE,0,0,18,closestRobot,3);
		robotToMove=closestRobot;
		
		robots[robotToMove].isMoving=1;
		setRobotDirection(&robots[robotToMove]);
		isRobotMoving=1;
	}
	else {
		 //otherwise move the robot currently moving
		moveRobot(&robots[robotToMove],speed);
		robotWalkCounter++;
	}

#define MAX_ROBOT_TICKS 20
	if (robotWalkCounter==MAX_ROBOT_TICKS) { //if the same robot's walked for max # of "ticks", move another robot
		isRobotMoving=0;
		robotWalkCounter=0;
		robots[robotToMove].isMoving=0;
	}
}

void shootPlayer(SPRITE robotShooting) {
	if (!robotShot.hasBeenFired) {
		robotShot.direction=robotShooting.direction;
		robotShot.xPos=robotShooting.xPos;
		robotShot.yPos=robotShooting.yPos;
		robotShot.hasBeenFired=1;
	}
	else if (robotShooting.isMoving) { //only fire if robot is moving
		switch (robotShot.direction) {
			case 0:
			robotShot.xPos-=2;
			robotShot.tileNum=horizontalShot;
			break;
			case 2:
			robotShot.yPos-=2;
			robotShot.tileNum=verticalShot;
			break;
			case 4:
			robotShot.xPos+=2;
			robotShot.tileNum=horizontalShot;
			break;
			case 6:
			robotShot.yPos+=2;
			robotShot.tileNum=verticalShot;
			break;
		}
		if (robotShot.xPos > 160 || robotShot.yPos > 110 || checkShotBGCollision(robotShot))
			robotShot.hasBeenFired=0;
	}
	else
		robotShot.hasBeenFired=0;
//	PrintNumber(SCR_1_PLANE,0,0,17,robotShot.xPos,3);
//	PrintNumber(SCR_1_PLANE,0,0,18,robotShot.yPos,3);

}


