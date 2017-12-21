#include "ngpc.h"		// required
#include "carthdr.h"		// required
#include "library.h"		// NGPC routines
#include "tiles.c" //graphics
#include "map.h" //maps
#include <stdlib.h>		// std C routines
#include <stdio.h>

u8 rstat,xPos,yPos,i,j,mapNum;

void drawMap(u8 mapNum);

void main(void) {
	xPos=0;
	yPos=0;
	rstat = USR_BOOT;
	InitNGPC();
	SysSetSystemFont();
	
	InstallTileSet(tiles,sizeof(tiles)/2);
	ClearScreen(SCR_1_PLANE);
	ClearScreen(SCR_2_PLANE);
	SetBackgroundColour(RGB(15, 15, 15));
	SetPalette(SCR_2_PLANE, 0, 0, RGB(0,0,12), RGB(0,0,0), RGB(0,0,0));
	//PutTile(SCR_2_PLANE,0,5,5,leftWall);
	//loadMap(maps);
	//drawMap(12);
	SetPalette(SPRITE_PLANE, 0, 0, RGB(0,0,0), RGB(12,12,2), RGB(15,15,0));
	SetSprite(0,moon,0,xPos,yPos,0);
	SetSprite(1,moon+1,1,8,0,0);
	SetSprite(2,moon+2,1,-8,8,0);
	SetSprite(3,moon+3,1,8,0,0);
	
	mapNum=0;

	while (1) {
		if (JOYPAD & J_LEFT)
			xPos--;
		if (JOYPAD & J_RIGHT)
			xPos++;
		if (JOYPAD & J_UP)
			yPos--;
		if (JOYPAD & J_DOWN)
			yPos++;
		if (JOYPAD & J_A)
			mapNum++;
		if (mapNum > 12)
			mapNum=0;
		drawMap(mapNum);
		SetSprite(0,moon,0,xPos,yPos,0);
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

