void initRobots(void);
void spawnRobots(u8 numRobots, u8 playerX, u8 playerY);
u8 checkRobotBGCollision(SPRITE robot);
void drawRobots(u8 numRobots);
void animateRobots(u8 numRobots);
void handleRobotShotCollision(PROJECTILE* shot);
void despawnRobot(SPRITE* robot);
void setRobotDirection(SPRITE* robot);
void moveRobot(SPRITE* robot,u8 speed);
void handleRobotMovement(u8 speed);
void shootPlayer(SPRITE robotShooting);

void initRobots() {
#define NUM_PLAYER_SPRITES 3 //2 for player character, one for projectile
	for (i=0; i < sizeof(robots)/sizeof(robots[0]); ++i) {
		robots[i].spriteID=NUM_PLAYER_SPRITES+i*2; //i*2 because there's another sprite being used by the bottom of the robot
		robots[i].tileNum=robotStanding1;
		robots[i].xPos=0;
		robots[i].yPos=0;
		robots[i].palette=1;
		robots[i].isMoving=0;
	}	
}

void spawnRobots(u8 numRobots, u8 playerX, u8 playerY) {
	SeedRandom();
	for (i=0; i < numRobots; ++i) {
		do {  //robots shouldn't spawn inside walls
			robots[i].xPos=GetRandom(130)+10; //between 10 and 140
			robots[i].yPos=GetRandom(80)+10; //between 10 and 90
		} while (checkRobotBGCollision(robots[i]));
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
	robot->xPos=200; //move robot offscreen (todo: add robot death animation)
	robot->isMoving=0;
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
				if (checkRobotBGCollision(*robot)) {
					despawnRobot(robot); //kill robot if it touches a wall
					robot->isMoving=0;
				}
				break;
				case 2: //up
				robot->yPos-=2;
				if (checkRobotBGCollision(*robot)) {
					despawnRobot(robot);
					robot->isMoving=0;
				}
				break;
				case 4: //right
				robot->xPos+=2;
				if (checkRobotBGCollision(*robot)) {
					despawnRobot(robot);
					robot->isMoving=0;
				}
				break;
				case 6: //down
				robot->yPos+=2;
				if (checkRobotBGCollision(*robot)) {
					despawnRobot(robot);
					robot->isMoving=0;
				}
				break;
			}
			if (GetDifference(robot->xPos,player.xPos) <= 20 || GetDifference(robot->yPos,player.yPos) <=20) {
				shootPlayer(*robot);
			}
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
	if (!isRobotMoving) {				//if a robot isn't moving, set a random one moving
		robotToMove=GetRandom(numRobots);
		robots[robotToMove].isMoving=1;
		setRobotDirection(&robots[robotToMove]);
		isRobotMoving=1;
	}
	else {
		 //otherwise move the robot currently moving
		moveRobot(&robots[robotToMove],speed);
		robotWalkCounter++;
	}

#define MAX_ROBOT_TICKS 50
	if (robotWalkCounter==MAX_ROBOT_TICKS) { //if the same robot's walked for max # of "ticks", move another robot
		isRobotMoving=0;
		robotWalkCounter=0;
	}
}

void shootPlayer(SPRITE robotShooting) {
	if (!robotShot.hasBeenFired) {
		robotShot.direction=robotShooting.direction;
		robotShot.xPos=robotShooting.xPos;
		robotShot.yPos=robotShooting.yPos;
		robotShot.hasBeenFired=1;
	}
	else {
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

}
