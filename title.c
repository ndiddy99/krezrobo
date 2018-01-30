#include "logo.c"

void showTitleScreen(void);

#define TILEMAP_OFFSET 128 //offset in tilemap due to system font taking up first half
#define LOGO_HEIGHT 3
#define LOGO_WIDTH 14

void showTitleScreen() {
	u8 i,j,scr1XVal,scr2XVal,scrollYVal;
	InstallTileSetAt(logo,sizeof(logo)/2,TILEMAP_OFFSET);
	SetPalette(SCR_1_PLANE, 0, 0, RGB(0,8,10), RGB(15,0,0), RGB(0,0,15));
	SetPalette(SCR_1_PLANE, 1, 0, RGB(15,15,15), RGB(15,15,15), RGB(15,15,15));
	SetPalette(SCR_2_PLANE, 0, 0, RGB(0,8,10), RGB(15,0,0), RGB(0,0,15));
	for (i=0; i < LOGO_HEIGHT; i++) {
		for (j=0; j < LOGO_WIDTH; j++) {
			if (i%2)
				PutTile(SCR_1_PLANE,0,j+3,i,TILEMAP_OFFSET+(i*LOGO_WIDTH+j));
			else
				PutTile(SCR_2_PLANE,0,j+3,i,TILEMAP_OFFSET+(i*LOGO_WIDTH+j));
		}
	}
	scr1XVal=120;
	scr2XVal=135;
	scrollYVal=225;
	
	while(scr1XVal!=0 || scr2XVal!=0) { //horiz scrolling animation
		ShiftScroll(SCR_1_PLANE,scr1XVal,scrollYVal);
		ShiftScroll(SCR_2_PLANE,scr2XVal,scrollYVal);
		if (scr1XVal!=0)
			scr1XVal--;
		if (scr2XVal!=0)
			scr2XVal++;
		WaitVsync();
		
	}
	while (scrollYVal != 240) { //vertical scrolling animation
		ShiftScroll(SCR_1_PLANE,scr1XVal,scrollYVal);
		ShiftScroll(SCR_2_PLANE,scr2XVal,scrollYVal);
		scrollYVal++;
		WaitVsync();	
	}
	PrintString(SCR_1_PLANE,1,3,5,"Press any key");
	PrintString(SCR_1_PLANE,1,4,6,"to start");
	PrintString(SCR_1_PLANE,1,3,8,"A/B: Shoot");
	PrintString(SCR_1_PLANE,1,3,9,"Joystick: Move");
	PrintString(SCR_1_PLANE,1,3,10,"Option: Pause");
	
	PrintString(SCR_1_PLANE,1,0,15,"(c) 2018");
	PrintString(SCR_1_PLANE,1,0,16,"Nathan Misner");
	while (!JOYPAD);
	
}