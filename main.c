#include "ngpc.h"		// required
#include "carthdr.h"		// required
#include "library.h"		// NGPC routines
#include "tiles.c" //graphics
#include "map.h" //maps
#include "main.h" //structs
#include <stdlib.h>		// std C routines
#include <stdio.h>

u8 i,j,mapNum;
SPRITE player;
void drawMap(u8 mapNum);

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
	player.xPos=10;
	player.yPos=40;
	player.palette=0;
	player.direction=0;
	SetPalette(SPRITE_PLANE, 0, 0, RGB(0,0,0), RGB(12,12,2), RGB(15,15,0));
	SetSprite(player.spriteID,player.tileNum,0,player.xPos,player.yPos,player.palette);
	SetSprite(player.spriteID+1,player.tileNum+1,1,0,8,0); //second sprite is linked to the first one
	
	mapNum=0;

	while (1) {
		if (!JOYPAD)
			player.direction=2;
		if (JOYPAD & J_LEFT) {
			player.xPos--;
			player.direction=0;
		}
		if (JOYPAD & J_RIGHT) {
			player.xPos++;
			player.direction=1;
		}
		if (JOYPAD & J_UP) {
			player.yPos--;
			player.direction=1;
		}
		if (JOYPAD & J_DOWN) { 
			player.yPos++;
			player.direction=0;
		}
		if (JOYPAD & J_A)
			mapNum++;
		if (mapNum > 12)
			mapNum=0;
		drawMap(mapNum);
		
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
		SetSprite(player.spriteID,player.tileNum,0,player.xPos,player.yPos,player.palette);
		SetSprite(player.spriteID+1,player.tileNum+1,1,0,8,0);
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

