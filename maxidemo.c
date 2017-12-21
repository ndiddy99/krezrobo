/* MaxiDemo

	Lots of little demos combined into one bigger one.
*/

#if 1			// header files

#include "ngpc.h"		// required
#include "carthdr.h"		// required
#include "library.h"		// NGPC routines
#include <stdlib.h>		// std C routines
#include <stdio.h>

#include "colors.h"		// used for bitmapdemo2

// data files
#include "colors.inc"		// used for colordemo
#include "sample.inc"		// used for neodemo
#include "sprites.inc"		// used for sprdemo
#include "fr_tiles.inc"		// used for DrawBox

#endif

#if 1 			// global variables

u8	rstat;		// gets restart status before init routines
u8	data[256];	// for flashdemo

#endif

#if 1			// function prototypes for this file

u8   menu(void);
void DrawBox(u8 Plane, u8 Palette, u8 XL, u8 YT, u8 XR, u8 YB);
void harddemo(void);
void PrintTime(void);
u16  SpeedTest(void);

void sprdemo(void);
void CopySpriteAnim(u8 AnimNumber);

void snddemo(void);
void rnddemo(void);
void neodemo(void);

void colordemo(void);

void powerdemo(void);
void wakedemo(u8 type);
void standbydemo(void);

void bitmaps(void);
void bmpdemo(void);
void bmp2demo(void);
void bmp2demo2(void);

void WaitAny(void);
void WaitOption(void);

#endif

/* Program structure 
1. show list of demos
2. use joypad to select one
   each line of text uses different palette and
   changes color when selected
   press A to select
*/

void WaitAny(void) {

	do {
		WaitVsync();
	} while (SYS_LEVER == 0);

};

void main(void) {	// init NGPC and execute demo routines

u8 	m_i;

    rstat = USR_BOOT;
    InitNGPC();
    SysSetSystemFont();

    while (1) {		// never fall out of main!!!
// print blanks on screen - really clear it
	FillScreen(SCR_1_PLANE, (u16)' ', 0);
	FillScreen(SCR_2_PLANE, (u16)' ', 0);

	m_i = menu();
	FillScreen(SCR_1_PLANE, (u16)' ', 0);
	switch (m_i) {
	    case 1:
		harddemo();
		break;
	    case 2:
		sprdemo();
		break;
	    case 3:
		bitmaps();
		break;
	    case 4:
		snddemo();
		break;
	    case 5:
		neodemo();
		break;
	    case 6:
		rnddemo();
		break;
	    case 7:
		colordemo();
		break;
	    case 8:
		powerdemo();
		break;
	   case 9:
		break;
	}
    }
}		// end of main routine


u8 menu(void) {		// display menu of demos and get selection

#define SELECTIONS 8
    u8 choice = 1;

// black text on white screen is more visible
    SetBackgroundColour(RGB(15, 15, 15));

// not the most efficient way, but it's only a menu
    SetColors(SCR_1_PLANE, 1, 0, 0, RGB(15, 0, 0));
    SetColors(SCR_1_PLANE, 2, 0, 0, RGB(0, 0, 0));
    SetColors(SCR_1_PLANE, 3, 0, 0, RGB(0, 0, 0));
    SetColors(SCR_1_PLANE, 4, 0, 0, RGB(0, 0, 0));
    SetColors(SCR_1_PLANE, 5, 0, 0, RGB(0, 0, 0));
    SetColors(SCR_1_PLANE, 6, 0, 0, RGB(0, 0, 0));
    SetColors(SCR_1_PLANE, 7, 0, 0, RGB(0, 0, 0));
    SetColors(SCR_1_PLANE, 8, 0, 0, RGB(0, 0, 0));
    SetColors(SCR_1_PLANE, 9, 0, 0, RGB(0, 0, 0));

    PrintString(SCR_1_PLANE, 1, 1, 1,  "Hardware demo");
    PrintString(SCR_1_PLANE, 2, 1, 3,  "Sprite");
    PrintString(SCR_1_PLANE, 3, 1, 5,  "Bitmaps");
    PrintString(SCR_1_PLANE, 4, 1, 7,  "Sound Effects");
    PrintString(SCR_1_PLANE, 5, 1, 9,  "Neotracker");
    PrintString(SCR_1_PLANE, 6, 1, 11, "Random numbers");
    PrintString(SCR_1_PLANE, 7, 1, 13, "146 color demo");
    PrintString(SCR_1_PLANE, 8, 1, 15, "Power");
    PrintString(SCR_1_PLANE, 9, 1, 17, "Reserved");

do {
	WaitAny();
	if (SYS_LEVER == J_UP) {
	    SetColors(SCR_1_PLANE, choice, 0, 0, RGB(0, 0, 0));
	    choice-- ;
	    if (choice == 0)
		choice = SELECTIONS;
	    SetColors(SCR_1_PLANE, choice, 0, 0, RGB(15, 0, 0));

	    do {
		WaitVsync();
	    } while (SYS_LEVER != 0);
	}

	if (SYS_LEVER == J_DOWN) {
	    SetColors(SCR_1_PLANE, choice, 0, 0, RGB(0, 0, 0));
	    choice++ ;
	    if (choice == SELECTIONS+1)
		choice = 1;
	    SetColors(SCR_1_PLANE, choice, 0, 0, RGB(15, 0, 0));

	    do {
		WaitVsync();
	    } while (SYS_LEVER != 0);
	}

    } while (SYS_LEVER != J_A);

    do {
	WaitVsync();
    } while (SYS_LEVER != 0);

    return choice;
}

void DrawBox(u8 Plane, u8 Palette, u8 XL, u8 YT, u8 XR, u8 YB) {
// sample routine to draw a double-lined box (like pop-up menu)

#define FRBASE 128
    u8 cx;
    u8 cy;
// around border - lines - inside
    SetColors(Plane, Palette, RGB(10, 0, 0), RGB(4, 4, 15), RGB(14, 0, 0));
    InstallTileSetAt(&frame_tiles, sizeof(frame_tiles)/2, FRBASE);

    PutTile(Plane,Palette,XR,YT,FRBASE+UpperRight);
    PutTile(Plane,Palette,XR,YB,FRBASE+BottomRight);
    PutTile(Plane,Palette,XL,YT,FRBASE+UpperLeft);
    PutTile(Plane,Palette,XL,YB,FRBASE+BottomLeft);
	for (cx=XL+1;cx<XR;cx++) {
		PutTile(Plane,Palette,cx,YT,FRBASE+TopHorizontal);
		PutTile(Plane,Palette,cx,YB,FRBASE+BottomHorizontal);
	};
	for (cy=YT+1;cy<YB;cy++) {
		PutTile(Plane,Palette,XR,cy,FRBASE+RightVertical);
		PutTile(Plane,Palette,XL,cy,FRBASE+LeftVertical);
	};

	for (cx=XL+1;cx<XR;cx++)
		for (cy=YT+1;cy<YB;cy++)
		    PutTile(Plane,Palette,cx,cy,FRBASE+Inside);
};

u16  SpeedTest(void) {	// sample routine to test CPU speed change

// counts number of increments possible in 1/60 sec

	register u16 wcnt;

	VBCounter = 0;

	while (VBCounter < 1) {
		wcnt ++ ;
	};
	wcnt = 0;
	while (VBCounter < 2) {
		wcnt ++ ;
   	}
	return wcnt;
};

void PrintTime(void) {	// sample routine to print day/date/time

TIME		ttime;
const u8	t_y = 8;

	GetTime(&ttime);
// date
	if (ttime.Year > 0x90)
		PrintString(SCR_1_PLANE, 0, 0, t_y, "19");
	else
		PrintString(SCR_1_PLANE, 0, 0, t_y, "20");
	PrintHex(SCR_1_PLANE, 0, 2, t_y, ttime.Year, 2);

	PrintString(SCR_1_PLANE, 0, 4, t_y, "/");
	PrintHex(SCR_1_PLANE, 0, 5, t_y, ttime.Month, 2);
	PrintString(SCR_1_PLANE, 0, 7, t_y, "/");
	PrintHex(SCR_1_PLANE, 0, 8, t_y, ttime.Day, 2);

// time
	PrintHex(SCR_1_PLANE, 0, 11, t_y, ttime.Hour, 2);
	PrintString(SCR_1_PLANE, 0, 13, t_y, ":");
	PrintHex(SCR_1_PLANE, 0, 14, t_y, ttime.Minute, 2);
	PrintString(SCR_1_PLANE, 0, 16, t_y, ":");
	PrintHex(SCR_1_PLANE, 0, 17, t_y, ttime.Second, 2);

// day of week
	PrintString(SCR_1_PLANE, 0, 0, t_y-1, DayNames[ttime.Weekday]);

}

void harddemo(void) {	// demo
u8	k;
u16	dchk;
u8	jinp;
u16     batper;

// ALARM	talrm;
TIME	ttime;
u16 	count1, count2;

// black text on white screen is more visible
    SetBackgroundColour(RGB(15, 15, 15));
    SetColors(SCR_1_PLANE, 0, 0, 0, RGB(0, 0, 0));
    SetColors(SCR_1_PLANE, 1, 0, 0, RGB(15, 0, 0));

// show instructions
    PrintString(SCR_1_PLANE, 1, 4, 0, "Harry's Demo");
    PrintString(SCR_1_PLANE, 0, 0, 18, "PRESS OPTION TO EXIT");

// get boot status
    PrintString(SCR_1_PLANE, 1, 0, 3, "Boot:");
    PrintNumber(SCR_1_PLANE, 0, 5, 3, rstat, 3);
    if (rstat == 32)
	PrintString(SCR_1_PLANE, 0, 9, 3, "Resume");
    if (rstat == 64)
	PrintString(SCR_1_PLANE, 0, 9, 3, "Power on");
    if (rstat == 128)
	PrintString(SCR_1_PLANE, 0, 9, 3, "Alarm");

// test CPU slowdown
	CpuSpeed(4);
	count1 = SpeedTest();
// test CPU at full speed
	CpuSpeed(0);
	count2 = SpeedTest();
// Display result
	PrintString(SCR_1_PLANE, 1, 2, 10, "Speed Test");
	PrintString(SCR_1_PLANE, 0, 2, 11, "Slow:");
	PrintString(SCR_1_PLANE, 0, 2, 12, "Fast:");
	PrintHex(SCR_1_PLANE, 0, 8, 11, count1, 4);
	PrintHex(SCR_1_PLANE, 0, 8, 12, count2, 4);

    do {

// wait a bit
	Sleep(5);
	PrintTime();

	PrintString(SCR_1_PLANE, 1, 0, 5, "Battery:");
	PrintHex(SCR_1_PLANE, 0, 9, 5, BATTERY, 4);
	batper = BATTERY;
	batper = ((batper + 5) * 25) >> 8; // same as BATTERY/0x03ff * 100% + 0.5
	PrintNumber(SCR_1_PLANE, 0, 15, 5, batper, 3);
	PrintString(SCR_1_PLANE, 0, 18, 5, "%");

	jinp = SYS_LEVER;
	PrintString(SCR_1_PLANE, 1, 2, 15, "Joypad:");
	PrintHex(SCR_1_PLANE, 0, 11, 15, jinp, 2);

    } while (jinp != J_OPTION);
}

void bmpdemo(void) {	// demo

#define BitmapTileBase 256

	u16 TestBitmap[65][8];
	u8 iSineBase;
	u8 iSineLoop;

	SetPalette(SCR_1_PLANE, 0, 0, RGB(15,0,0), RGB(0,0,15), RGB(0,15,0));
	PrintString(SCR_1_PLANE, 0, 0, 18, "PRESS OPTION TO EXIT");

	//Quick Demonstration of the bitmap and trig functions.
	//Create a 64x64 bitmap
	CreateBitmap((u16 *)TestBitmap, 64, 64);

	//Draw a border
	DrawLine((u16 *)TestBitmap, 0, 0, 0, 63, 3);
	DrawLine((u16 *)TestBitmap, 0, 0, 63, 0, 3);
	DrawLine((u16 *)TestBitmap, 63, 63, 0, 63, 3);
	DrawLine((u16 *)TestBitmap, 63, 63, 63, 0, 3);

	//Draw the bitmap
	DrawBitmap((u16 *)TestBitmap, 2, 2, BitmapTileBase);

	iSineBase=0;

	while (SYS_LEVER != J_OPTION) {

		//Need to create (clear) the bitmap within every iteration
		CreateBitmap((u16 *)TestBitmap, 64, 64);

		//However, we don't need to redraw it as the tiles containing the
		//bitmap are already on the screen.

		//Draw a border
		DrawLine((u16 *)TestBitmap, 0, 0, 0, 63, 3);
		DrawLine((u16 *)TestBitmap, 0, 0, 63, 0, 3);
		DrawLine((u16 *)TestBitmap, 63, 63, 0, 63, 3);
		DrawLine((u16 *)TestBitmap, 63, 63, 63, 0, 3);


		//Draw a sine wave
		for (iSineLoop=1;iSineLoop!=63;iSineLoop++) {
			//As the sin() and cos() functions return a signed value between -127 & 127
			//we need to munge it into an unsigned value to pass to the SetPixel function

			//Also, to demonstrate the power of shifting instead of multiplying...
			//Compare the following two sets of, functionally identical, code

			//Slow...
			//SetPixel((u16 *)TestBitmap, iSineLoop, (Sin(iSineBase+iSineLoop*4)/8)+32,1);
			//SetPixel((u16 *)TestBitmap, iSineLoop, (Cos(iSineBase-iSineLoop*4)/8)+32,2);

			//Quick...
			SetPixel((u16 *)TestBitmap, iSineLoop, (Sin(iSineBase+iSineLoop<<2)>>3)+32,1);
			SetPixel((u16 *)TestBitmap, iSineLoop, (Cos(iSineBase-iSineLoop<<2)>>3)+32,2);
		}
		//Copy the bitmap into tile memory
		CopyBitmap((u16 *)TestBitmap, BitmapTileBase);
		iSineBase++;
	}
};

void sprdemo(void) {	// demo

#define TILE_SPRITE (129)	// first tile used for Sprite
#define SPR_SPRITE (0)		// sprite number used for Sprite (also 1,2 and 3)

   u8 SpriteX;
   u8 SpriteY;
   u8 SpriteAnim;
   u8 SpriteBaseAnim;
   u8 xx;

   InstallTileSetAt(TileSet, sizeof(TileSet)/2, TILE_SPRITE);
   SetPalette(SPRITE_PLANE, 0, 0, RGB(15,0,0), RGB(14,12,12), RGB(0,0,0));
   PrintString(SCR_1_PLANE, 0, 0, 18, "PRESS OPTION TO EXIT");

   // initialise Sprite variables
   SpriteX = 64;
   SpriteY = 64;
   SpriteAnim = 0;
   SpriteBaseAnim = 0;
   
   // set the initial sprite up
   // Note that all 4 sprites are chained together so we only have to
   // move the first one and the rest follow :-)
   SetSprite(SPR_SPRITE, TILE_SPRITE, 0, SpriteX, SpriteY, 0);
   SetSprite(SPR_SPRITE+1, TILE_SPRITE+1, 1, 8, 0, 0);
   SetSprite(SPR_SPRITE+2, TILE_SPRITE+2, 1, -8, 8, 0);
   SetSprite(SPR_SPRITE+3, TILE_SPRITE+3, 1, 8, 0, 0);
   CopySpriteAnim(0);

// put the sprites between planes
	for (xx=SPR_SPRITE;xx<SPR_SPRITE+4;xx++)
		SpriteControl(xx, SPR_MIDDLE, 0);

// put up things to walk around
   PrintString(SCR_1_PLANE, 0, 2, 13, "======FRONT======");
   PrintString(SCR_2_PLANE, 0, 2, 10, "=======REAR======");

   while (SYS_LEVER != J_OPTION)
   {
      if ((SYS_LEVER & J_LEFT) && (SpriteX > 0))
      {
         CopySpriteAnim(16+(SpriteAnim * 4));
         SpriteX -= 2;
         SetSpritePosition(SPR_SPRITE, SpriteX, SpriteY);
         SpriteAnim ^= 1; // toggles anim 0->1->0
      }
      else if ((SYS_LEVER & J_RIGHT) && (SpriteX < SCRN_W-16))
      {
         CopySpriteAnim(24+(SpriteAnim * 4));
         SpriteX += 2;
         SetSpritePosition(SPR_SPRITE, SpriteX, SpriteY);
         SpriteAnim ^= 1;
      }
      else if ((SYS_LEVER & J_UP) && (SpriteY > 0))
      {
         CopySpriteAnim(8+(SpriteAnim * 4));
         SpriteY -= 2;
         SetSpritePosition(SPR_SPRITE, SpriteX, SpriteY);
         SpriteAnim ^= 1;
      }
      else if ((SYS_LEVER & J_DOWN) && (SpriteY < SCRN_H-16))
      {
         CopySpriteAnim(0+(SpriteAnim * 4));
         SpriteY += 2;
         SetSpritePosition(SPR_SPRITE, SpriteX, SpriteY);
         SpriteAnim ^= 1;
      }

      Sleep(3);
   }
   for (xx=0; xx<4; xx++)
	UnsetSprite(SPR_SPRITE+xx);

}

void CopySpriteAnim(u8 AnimNumber) {	// subroutine for above
// copies the new animation onto the Sprites tiles.
   u8 i;
   u16 * TileRam = TILE_RAM;
   u16 * theSpriteAnim = (u16 *) Sprite_Tiles;

   TileRam += (TILE_SPRITE * 8);
   theSpriteAnim += (AnimNumber * 8);
   
   for (i = 0; i < 32; i ++)
   {
      TileRam[i] = *theSpriteAnim++;
   }
}

void snddemo(void) {	// demo

// give our sounds a name - useful when we want to play them
enum
{
   SND_SIREN = 1,
   SND_CHIRP,
   NUM_SOUNDS = SND_CHIRP
};

/* A quicker way of defining our sounds - this is the same as:
 * Sounds[SND_SIREN].Channel =  2;
 * Sounds[SND_SIREN].Length = 0x60;
 * etc.
 * See Library.h for a definition of the sound structure
 */

const SOUNDEFFECT Sounds[NUM_SOUNDS] = {
   { 2, 0x60, 0, 0x60, 8, 1, 2, 0x40, 0x80, 0xf, 0,0,0,0,0 },
   { 1, 0x40, 0, 0x80, 0x20, 1, 1, 0x44, 0x82, 0xf, 0,0,0,0,0 }
};

   SetColors(SCR_1_PLANE, 0, 0, 0, RGB(0, 0, 0));
   // write the on-screen instructions
   PrintString(SCR_1_PLANE, 0, 0, 2, "PRESS 'A' FOR SIREN");
   PrintString(SCR_1_PLANE, 0, 0, 4, "PRESS 'B' FOR CHIRP");
   PrintString(SCR_1_PLANE, 0, 0, 18, "PRESS OPTION TO EXIT");

   // install the sounds driver and our sound effects
   InstallSoundDriver();
   InstallSounds(Sounds, NUM_SOUNDS);

   while(JOYPAD != J_OPTION) {

	  if (JOYPAD & J_A) // if A is pressed play the siren sound
		 PlaySound(SND_SIREN);

	  if (JOYPAD & J_B) // if B is pressed play the chirp sound
		 PlaySound(SND_CHIRP);

	  while (JOYPAD); // wait for button release
   }
}

void rnddemo(void) {	// demo

u8 i;

   SetColors(SCR_1_PLANE, 0, 0, 0, RGB(0, 0, 0));
   PrintString(SCR_1_PLANE, 0, 2, 14, "PRESS A TO SEED");

   // wait for A to be pressed
   while((JOYPAD & J_A) != J_A) ;

   // seeded after user input to ensure randomness
   SeedRandom(); 

   DrawBox(SCR_2_PLANE,1,6,1,12,12);

   // display 10 pseudo random numbers in range 0 to 20
   for (i = 0; i < 10; i ++)
   {
      u16 RandNum = GetRandom(200);
      PrintNumber(SCR_1_PLANE, 0, 8, 2 + i, RandNum, 3);
   }
   WaitOption();   
}

void neodemo(void) {	// demo


   NeoTracker_InstallDriver();
   NeoTracker_SendGroup(sample_Data, sample_Len);

   SetColors(SCR_1_PLANE, 0, 0, 0, RGB(0, 0, 0));
   PrintString(SCR_1_PLANE, 0, 5, 7, "A = MUSIC");
   PrintString(SCR_1_PLANE, 0, 5, 9, "B = SFX");
   PrintString(SCR_1_PLANE, 0, 3, 11, "Opt = Mute");

   do {
	  if (JOYPAD & J_A)
	  {
		 while(JOYPAD & J_A); // wait for button release
		 NeoTracker_PlayMusic(BGM_ARKANOID);
	  }

	  if (JOYPAD & J_B)
	  {
		 while(JOYPAD & J_B); // wait for button release
		 NeoTracker_PlaySFX(SFX_WARP);
	  }

	  if (JOYPAD & J_OPTION)
	  {
		 while(JOYPAD & J_OPTION); // wait for button release
		 NeoTracker_StopAll();
	  }
	  
   } while (JOYPAD != J_OPTION);
}

void colordemo(void) {	// demo

	u8	xx, yy;
#define BLK_BASE 145
	SetScreen(8, 8, 144, 136);
	SetBackgroundColour(0x0000);
	SetWindowColor(0x0fff);
	SetMoreColors(SCR_1_PLANE, 0, 16, backpal);
	SetMoreColors(SCR_2_PLANE, 0, 16, forepal);
	SetMoreColors(SPRITE_PLANE, 0, 16, spripal);
	InstallTileSetAt(&blocks, sizeof(blocks)/2, BLK_BASE);

	for (xx=0; xx<16; xx++)
		for (yy=0; yy<3; yy++) {
			PutTile(SCR_1_PLANE, xx, xx+2, yy*2+1, BLK_BASE+yy);
			PutTile(SCR_2_PLANE, xx, xx+2, yy*2+7, BLK_BASE+yy);
			SetSprite(xx*3+yy, BLK_BASE+yy, 0, xx*8+16, yy*16+104, xx);
		};
	WaitOption();

// clear sprites
	for (xx=0; xx<48; xx++)
		UnsetSprite(xx);
// set back to full screen
	SetScreen(0, 0, 160, 152);
}

void powerdemo(void) {

#define PSELECT 3
    u8 choice = 1;

// not the most efficient way, but it's only a menu
    SetColors(SCR_1_PLANE, 1, 0, 0, RGB(15, 0, 0));
    SetColors(SCR_1_PLANE, 2, 0, 0, RGB(0, 0, 0));
    SetColors(SCR_1_PLANE, 3, 0, 0, RGB(0, 0, 0));

    PrintString(SCR_1_PLANE, 1, 1, 1,  "Wakeup demo");
    PrintString(SCR_1_PLANE, 2, 1, 3,  "Standby demo");
    PrintString(SCR_1_PLANE, 3, 1, 5,  "Exit demo");

do {

	WaitAny();
	if (SYS_LEVER == J_UP) {
	    SetColors(SCR_1_PLANE, choice, 0, 0, RGB(0, 0, 0));
	    choice-- ;
	    if (choice == 0)
		choice = PSELECT;
	    SetColors(SCR_1_PLANE, choice, 0, 0, RGB(15, 0, 0));

	    do {
		WaitVsync();
	    } while (SYS_LEVER != 0);
	}

	if (SYS_LEVER == J_DOWN) {
	    SetColors(SCR_1_PLANE, choice, 0, 0, RGB(0, 0, 0));
	    choice++ ;
	    if (choice == PSELECT+1)
		choice = 1;
	    SetColors(SCR_1_PLANE, choice, 0, 0, RGB(15, 0, 0));

	    do {
		WaitVsync();
	    } while (SYS_LEVER != 0);
	}

    } while (SYS_LEVER != J_A);

    do {
	WaitVsync();
    } while (SYS_LEVER != 0);

	switch (choice) {
	    case 1:
		wakedemo(1);
		break;
	    case 2:
		standbydemo();
		break;
	    case 3:
		break;
	};
};

void wakedemo(u8 type) {

TIME ttime;
ALARM twake;


// print instruction
   SetColors(SCR_1_PLANE, 2, 0, 0, RGB(15, 0, 0));
   PrintString(SCR_1_PLANE, 2, 2, 7, "Press B to Sleep");
   PrintString(SCR_1_PLANE, 2, 2, 9, "System will turn");
   PrintString(SCR_1_PLANE, 2, 2, 10, "back on in less");
   PrintString(SCR_1_PLANE, 2, 2, 11, "than a minute");
// wait for button
	WaitAny();
// set sleep
	if (SYS_LEVER == J_B) {
	GetTime(&ttime);
	twake.Day = 0xFF;
	if (ttime.Minute != 0x59) {
		twake.Hour = 0xFF;
		if ((ttime.Minute & 15) == 9)
			twake.Min = ttime.Minute +7;
		else
			twake.Min = ttime.Minute +1;
	}
	else {
		if ((twake.Hour & 15) == 9)
			twake.Hour = ttime.Hour +7;
		else
			twake.Hour = ttime.Hour +1;
	}
	SetWake(&twake);
// sleep
	if (type = 1)
		SysShutdown();
	else
		ResumeOff();
	};
};

void standbydemo(void) {

// print instruction
   SetColors(SCR_1_PLANE, 2, 0, 0, RGB(15, 0, 0));
   PrintString(SCR_1_PLANE, 2, 0, 8, "  Press B to put");
   PrintString(SCR_1_PLANE, 2, 0, 9, "System into standby");
// wait for button
	do {
		WaitVsync();
	} while (SYS_LEVER == 0);
// set to standby
	if (SYS_LEVER == J_B) {
		ResumeOff();
	};
};

void WaitOption(void) {

	do {
		WaitVsync();
	} while (SYS_LEVER != J_OPTION);

};


void LowerFont(void) {
// moves font down lower in tile RAM
	u16 k;
	u16 * TileRam = TILE_RAM;

	for (k=0;k<64*8;k++)
		TileRam[k] = TileRam[k+32*8];
};

void PrintString_2(u8 Plane, u8 Palette, u8 XPos, u8 YPos, const char * theString) {

   u16 * Screen;

   switch (Plane)
   {
	  case SCR_1_PLANE:
		 Screen = SCROLL_PLANE_1;
		 break;

	  case SCR_2_PLANE:
		 Screen = SCROLL_PLANE_2;
		 break;

	  default:
		 return;
   }

   while (*theString)
   {
	  u16 Offset = ((u16)YPos * 32) + XPos;
	  u16 Value = *theString + ((u16)Palette << 9) - 32;
	  Screen[Offset] = Value;

	  theString++;
	  XPos++;
   }
}

void bitmaps(void) {

#define BSELECT 4
    u8 choice = 1;

// not the most efficient way, but it's only a menu
    SetColors(SCR_1_PLANE, 1, 0, 0, RGB(15, 0, 0));
    SetColors(SCR_1_PLANE, 2, 0, 0, RGB(0, 0, 0));
    SetColors(SCR_1_PLANE, 3, 0, 0, RGB(0, 0, 0));
    SetColors(SCR_1_PLANE, 4, 0, 0, RGB(0, 0, 0));

    PrintString(SCR_1_PLANE, 1, 1, 1,  "3 color - sines");
    PrintString(SCR_1_PLANE, 2, 1, 3,  "7 color - random");
    PrintString(SCR_1_PLANE, 3, 1, 5,  "7 color - sines");
    PrintString(SCR_1_PLANE, 4, 1, 7,  "Exit demo");

do {

	WaitAny();
	if (SYS_LEVER == J_UP) {
	    SetColors(SCR_1_PLANE, choice, 0, 0, RGB(0, 0, 0));
	    choice-- ;
	    if (choice == 0)
		choice = BSELECT;
	    SetColors(SCR_1_PLANE, choice, 0, 0, RGB(15, 0, 0));

	    do {
		WaitVsync();
	    } while (SYS_LEVER != 0);
	}

	if (SYS_LEVER == J_DOWN) {
	    SetColors(SCR_1_PLANE, choice, 0, 0, RGB(0, 0, 0));
	    choice++ ;
	    if (choice == BSELECT+1)
		choice = 1;
	    SetColors(SCR_1_PLANE, choice, 0, 0, RGB(15, 0, 0));

	    do {
		WaitVsync();
	    } while (SYS_LEVER != 0);
	}

    } while (SYS_LEVER != J_A);

    do {
	WaitVsync();
    } while (SYS_LEVER != 0);

	switch (choice) {
	    case 1:
		FillScreen(SCR_1_PLANE, (u16)' ', 0);
		bmpdemo();
		break;
	    case 2:
		bmp2demo();
		break;
	    case 3:
		bmp2demo2();
		break;
	    case 4:
		break;
	};
};

void bmp2demo(void) {

	u8   i, j;
	u8   m = 0;
	u8   r;

// setup screen to be in center only
	SetScreen(0, 32, 160, 96);			// 20x11 widescreen w/title
	ShiftScroll(SCR_1_PLANE, 0, (u8)(256-32));	// orient tiles to 0,0
	ShiftScroll(SCR_2_PLANE, 0, (u8)(256-32));

// setup all colors
	SetWindowColor(WHITE);
	SetBackgroundColour(BLACK);
	SetColors(SCR_1_PLANE, 0, RED, GREEN, BLUE);
	SetColors(SCR_2_PLANE, 0, CYAN, YELLOW, MAGENTA);
// Set font so it's still usable
	LowerFont();
// setup bitmap mode
	CreateBitmap_2();
	ClearBitmap_2();

// do something interesting now it is setup
	SeedRandom();
// display title
	PrintString_2(SCR_1_PLANE, 0, 4, 11, "BITMAP DEMO");

// fill screen with random colors - leave 1 pixel border
	for (i=1;i<87;i++)
		for (j=1;j<159;j++) {
			if (m == 0) {
				r = GetRandom(7);
				m = GetRandom(3)+1;
			}
			else
				m --;
			SetPixel_2(j, i, r);
		};

	do {
		WaitVsync();
		if (SYS_LEVER == J_A) {
// split the 2 planes slowly
			SetScreen(0, 0, 160, 152);
			for (i=0;i<48;i++) {
				ShiftScroll(SCR_1_PLANE, 0, (u8)(256-32-i));
				ShiftScroll(SCR_2_PLANE, 0, (u8)(256-32+i));
				Sleep(8);
			};
		};
	} while (SYS_LEVER != J_OPTION);

// all done - put things back to normal	
	SetScreen(0, 0, 160, 152);
	ShiftScroll(SCR_1_PLANE, 0, 0);
	ShiftScroll(SCR_2_PLANE, 0, 0);
	SysSetSystemFont();
};

void bmp2demo2(void) {

	u8 iSineBase = 0;
	u8 iSineLoop;

// setup all colors
	SetWindowColor(BLACK);
	SetBackgroundColour(WHITE);
	SetColors(SCR_1_PLANE, 0, RED, GREEN, BLUE);
	SetColors(SCR_2_PLANE, 0, CYAN, YELLOW, MAGENTA);
	LowerFont();

// setup bitmap mode
	CreateBitmap_2();
// Clear the bitmap
	ClearBitmap_2();
// Put some text on screen
	PrintString_2(SCR_1_PLANE, 0, 2, 12, "3RD BITMAP DEMO");
	PrintString_2(SCR_2_PLANE, 0, 0, 18, "PRESS OPTION TO EXIT");

	while (SYS_LEVER != J_OPTION) {
		WaitVsync();

		//Draw sine waves
		for (iSineLoop=1;iSineLoop<160;iSineLoop++) {
			SetPixel_2(iSineLoop, (Sin(   iSineBase+iSineLoop*2)>>2)+44,0);
			SetPixel_2(iSineLoop, (Sin(10+iSineBase+iSineLoop*3)>>2)+44,1);
			SetPixel_2(iSineLoop, (Sin(30+iSineBase+iSineLoop*3)>>2)+44,2);
			SetPixel_2(iSineLoop, (Sin(10+iSineBase+iSineLoop*4)>>2)+44,3);
			SetPixel_2(iSineLoop, (Sin(30+iSineBase+iSineLoop*4)>>2)+44,4);
			SetPixel_2(iSineLoop, (Sin(10+iSineBase+iSineLoop*5)>>2)+44,5);
			SetPixel_2(iSineLoop, (Sin(30+iSineBase+iSineLoop*5)>>2)+44,6);
		}
		iSineBase++;
	}
	SysSetSystemFont();
};
// EOF
