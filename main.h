typedef struct Sprite {
	u8 spriteID;
	u8 tileNum;
	u8 xPos;
	u8 yPos;
	u8 palette;
	u8 direction;
	u8 isMoving;
} SPRITE;

typedef struct Projectile {
	u8 spriteID;
	u8 tileNum;
	u8 xPos;
	u8 yPos;
	u8 palette;
	u8 direction;
	u8 hasBeenFired;
} PROJECTILE;