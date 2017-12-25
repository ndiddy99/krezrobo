#include "ngpc.h"		// required
#include "carthdr.h"		// required
#include "library.h"		// NGPC routines
#include "tiles.c" //graphics
#include "map.h" //maps
#include "main.h" //structs
#include <stdlib.h>		// std C routines
#include <stdio.h>


#define TILEMAP_OFFSET 128 //offset in tilemap due to system font taking up first half
u8 i,j,mapNum,slowCounter,numRobots,robotAnimCounter;
u16 currentTile,currentTileTemp;

SPRITE player;
PROJECTILE playerShot;
SPRITE robots[10];

void drawMap(u8 mapNum);
u8 checkPlayerBGCollision(void);
u8 checkShotBGCollision(PROJECTILE shot);
void setPlayerStartingPoint(u8 location);
void switchScreens(u8 direction);
void initRobots(void);
void spawnRobots(u8 numRobots, u8 playerX, u8 playerY);
void drawRobots(u8 numRobots);
void animateRobots(u8 numRobots);

void main(void) {
	InitNGPC();
	SysSetSystemFont();
	InstallTileSetAt(tiles,sizeof(tiles)/2,TILEMAP_OFFSET);
	ClearScreen(SCR_1_PLANE);
	ClearScreen(SCR_2_PLANE);
	SetBackgroundColour(RGB(0, 0, 0));
	SetPalette(SCR_2_PLANE, 0, 0, RGB(3,2,9), RGB(0,0,0), RGB(0,0,0));
	SetPalette(SCR_1_PLANE, 0, 0, RGB(15,15,15), RGB(15,15,15), RGB(15,15,15));
	SetPalette(SPRITE_PLANE, 0, 0, RGB(15,15,15), RGB(15,0,2), RGB(15,15,0));
	SetPalette(SPRITE_PLANE,1,0,RGB(0,15,0),RGB(15,15,15),RGB(15,15,15));

	player.spriteID=0;
	player.tileNum=playerStandingL;
	setPlayerStartingPoint(0);
	player.palette=0;
	player.direction=4;
	
	playerShot.spriteID=2;
	playerShot.tileNum=horizontalShot;
	playerShot.xPos=50;
	playerShot.yPos=20;
	playerShot.palette=0;
	playerShot.hasBeenFired=0;
	
	numRobots=5;
	robotAnimCounter=0;
	initRobots();
	
	SetSprite(player.spriteID,TILEMAP_OFFSET+player.tileNum,0,player.xPos,player.yPos,player.palette);
	SetSprite(player.spriteID+1,TILEMAP_OFFSET+player.tileNum+1,1,0,8,0); //second sprite is linked to the first one
	SetSprite(playerShot.spriteID,TILEMAP_OFFSET+playerShot.tileNum,0,playerShot.xPos,playerShot.yPos,playerShot.palette);
	//SetSprite(robots[0].spriteID,TILEMAP_OFFSET+robots[0].tileNum,0,robots[0].xPos,robots[0].yPos,robots[0].palette);
	spawnRobots(numRobots,player.xPos,player.yPos);
	drawMap(0);
	currentTile=0;
	slowCounter=0;


	while (1) {
		
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
					player.xPos--;
					if (checkPlayerBGCollision())
						player.xPos++;
					player.yPos--;
					if (checkPlayerBGCollision())
						player.yPos++;
					player.direction=1;
				}
				else if (JOYPAD & J_DOWN) { //down-left
					player.xPos--;
					if (checkPlayerBGCollision())
						player.xPos++;
					player.yPos++;
					if (checkPlayerBGCollision())
						player.yPos--;
					player.direction=7;
				}				
				else { //left
					player.xPos--;
					if (checkPlayerBGCollision())
						player.xPos++;
					player.direction=0;
				}
			}
			else if (JOYPAD & J_RIGHT) {
				if (JOYPAD & J_UP) { //up-right
					player.xPos++;
					if (checkPlayerBGCollision())
						player.xPos--;
					player.yPos--;
					if (checkPlayerBGCollision())
						player.yPos++;
					player.direction=3;
				}
				else if (JOYPAD & J_DOWN) { //down-right
					player.xPos++;
					if (checkPlayerBGCollision())
						player.xPos--;
					player.yPos++;
					if (checkPlayerBGCollision())
						player.yPos--;
					player.direction=5;
				}				
				else { //right
					player.xPos++;
					if (checkPlayerBGCollision())
						player.xPos--;
					player.direction=4;
				}
			}
			else if (JOYPAD & J_UP) { //up
				player.yPos--;
				if (checkPlayerBGCollision())
					player.yPos++;
				player.direction=2;
			}
			else if (JOYPAD & J_DOWN) { //down
				player.yPos++;
				if (checkPlayerBGCollision())
					player.yPos--;
				player.direction=6;
			}
		slowCounter=0;
		}
		else
			slowCounter=1;
		if ((JOYPAD & J_A) && !playerShot.hasBeenFired) { //determines shot starting location
			switch(player.direction) {
				case 0:
				case 1: //facing left
				case 7:
				playerShot.xPos=player.xPos-6;
				playerShot.yPos=player.yPos+4;
				break;
				
				case 2: //facing up
				playerShot.xPos=player.xPos;
				playerShot.yPos=player.yPos-4;
				break;
				
				case 3:
				case 4: //facing right
				case 5: 
				playerShot.xPos=player.xPos+6;
				playerShot.yPos=player.yPos+4;
				break;
				case 6: //facing down
				playerShot.xPos=player.xPos;
				playerShot.yPos=player.yPos+12;
				break;
			}
			
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
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
					player.tileNum=playerStandingL;
				break;
			}
		}
		PrintNumber(SCR_1_PLANE,0,0,15,playerShot.xPos,3);
		PrintNumber(SCR_1_PLANE,0,0,16,playerShot.yPos,4);
		PrintNumber(SCR_1_PLANE,0,0,17,player.direction,4);
		PrintNumber(SCR_1_PLANE,0,0,18,playerShot.direction,4);
		
		SetSprite(playerShot.spriteID,TILEMAP_OFFSET+playerShot.tileNum,0,playerShot.xPos,playerShot.yPos,playerShot.palette);
		
		SetSprite(player.spriteID,TILEMAP_OFFSET+player.tileNum,0,player.xPos,player.yPos,player.palette);
		SetSprite(player.spriteID+1,TILEMAP_OFFSET+player.tileNum+1,1,0,8,0);
		
		animateRobots(numRobots);
		drawRobots(numRobots);
		

		
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
		
		WaitVsync();
		
	}
}
void drawMap(u8 mapNum) {
	for (i=0; i < MAP_SIZE_X;i++) {
		for (j=0; j < MAP_SIZE_Y;j++) {
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

void switchScreens(u8 direction) {
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
	spawnRobots(numRobots,player.xPos,player.yPos);
}

void initRobots() {
	for (i=0; i < sizeof(robots)/sizeof(robots[0]); i++) {
		robots[i].spriteID=3+i*2;
		robots[i].tileNum=robotStanding1;
		robots[i].xPos=0;
		robots[i].yPos=0;
		robots[i].palette=1;
		robots[i].isMoving=0;
	}	
}

void spawnRobots(u8 numRobots, u8 playerX, u8 playerY) {
	SeedRandom();
	for (i=0; i < numRobots; i++) {
		do {  //30x30 area around player shouldnt have robots (nondeterministic af but yolo)
			robots[i].xPos=GetRandom(130)+10; //between 10 and 140
		} while (!(robots[i].xPos > playerX-30) && !(robots[i].xPos < playerX+30));
		
		do { 
			robots[i].yPos=GetRandom(80)+10; //between 10 and 90
		} while (!(robots[i].yPos > playerY-30) && !(robots[i].yPos < playerY+30));
	}
}

void drawRobots(u8 numRobots) {
	for (i=0; i < numRobots; i++) {
		SetSprite(robots[i].spriteID,TILEMAP_OFFSET+robots[i].tileNum,0,robots[i].xPos,robots[i].yPos,robots[i].palette);
		SetSprite(robots[i].spriteID+1,TILEMAP_OFFSET+robots[i].tileNum+1,1,0,8,robots[i].palette);
	}
}

void animateRobots(u8 numRobots) {
#define ANIMATION_DELAY 5
	if (robotAnimCounter==ANIMATION_DELAY) {  
		for (i=0; i < numRobots; i++) {
			if (robots[i].tileNum < robotStanding1 || robots[i].tileNum >= robotStanding6)
				robots[i].tileNum=robotStanding1;
			else
				robots[i].tileNum+=2;
		}
		robotAnimCounter=0;
	}
	else
		robotAnimCounter++;
}