#include "ngpc.h"		// required
#include "carthdr.h"		// required
#include "library.h"		// NGPC routines
#include "tiles.c" //graphics
#include "map.h" //maps
#include "main.h" //structs
#include <stdlib.h>		// std C routines
#include <stdio.h>

u8 i,j,mapNum;
u16 currentTile,currentTileTemp;
SPRITE player;
void drawMap(u8 mapNum);
u8 checkBGCollision(void);
void setPlayerStartingPoint(u8 location);
void switchScreens(u8 direction);

void main(void) {
	InitNGPC();
	SysSetSystemFont();
	
	InstallTileSet(tiles,sizeof(tiles)/2);
	ClearScreen(SCR_1_PLANE);
	ClearScreen(SCR_2_PLANE);
	SetBackgroundColour(RGB(15, 15, 15));
	SetPalette(SCR_2_PLANE, 0, 0, RGB(0,0,12), RGB(0,0,0), RGB(0,0,0));
	player.spriteID=0;
	player.tileNum=playerStandingL;
	setPlayerStartingPoint(0);
	player.palette=0;
	player.direction=0;
	SetPalette(SPRITE_PLANE, 0, 0, RGB(0,0,0), RGB(12,12,2), RGB(15,15,0));
	SetSprite(player.spriteID,player.tileNum,0,player.xPos,player.yPos,player.palette);
	SetSprite(player.spriteID+1,player.tileNum+1,1,0,8,0); //second sprite is linked to the first one
	drawMap(0);
	currentTile=0;


	while (1) {
		if (!JOYPAD)
			player.direction=2;
		if (JOYPAD & J_LEFT) {
			player.xPos--;
			if (checkBGCollision())
				player.xPos++;
			player.direction=0;
		}
		if (JOYPAD & J_RIGHT) {
			player.xPos++;
			if (checkBGCollision())
				player.xPos--;
			player.direction=1;
		}
		if (JOYPAD & J_UP) {
			player.yPos--;
			if (checkBGCollision())
				player.yPos++;
			player.direction=1;
		}
		if (JOYPAD & J_DOWN) { 
			player.yPos++;
			if (checkBGCollision())
				player.yPos--;
			player.direction=1;
		}
	/*	if (JOYPAD & J_A)
			mapNum++;
		if (mapNum > 12)
			mapNum=0;
		drawMap(mapNum);
	*/	
		switch (player.direction) {
			case 0:
			if (player.tileNum < playerStandingR || player.tileNum >= playerWalkingR2)
				player.tileNum=playerStandingR;
			else 
				player.tileNum+=2;	
			break;
			case 1:
			if (player.tileNum < playerStandingL || player.tileNum >= playerWalkingL2)
				player.tileNum=playerStandingL;
			else 
				player.tileNum+=2;
			break;
		}
		PrintNumber(SCR_1_PLANE,0,0,15,player.xPos,4);
		PrintNumber(SCR_1_PLANE,0,0,16,player.yPos,4);
		SetSprite(player.spriteID,player.tileNum,0,player.xPos,player.yPos,player.palette);
		SetSprite(player.spriteID+1,player.tileNum+1,1,0,8,0);
		
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
			PutTile(SCR_2_PLANE,0,i,j,maps[mapNum][j][i]);
		}
	}	
}
u8 checkBGCollision() {
	GetTile(SCR_2_PLANE,NULL,(player.xPos+4)>>3,(player.yPos+8)>>3,&currentTile); //collision detection
	return currentTile!=blank; //1 if collision, 0 if not
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
		setPlayerStartingPoint(0);
		break;
		case 1:
		drawMap(topEnterMaps[GetRandom(3)]);
		setPlayerStartingPoint(1);
		break;
		case 2:
		drawMap(sideEnterMaps[GetRandom(3)]);
		setPlayerStartingPoint(2);
		break;
		case 3:
		drawMap(bottomEnterMaps[GetRandom(5)]);
		setPlayerStartingPoint(3);
		break;
	}
}