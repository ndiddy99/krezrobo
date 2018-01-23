// Exported by NeoTile

const unsigned short tiles[48][8] = 
{
	{0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000}, // blank
	{0x5500, 0x5500, 0x5500, 0x5500, 0x5500, 0x5500, 0x5500, 0x5500}, // leftWall
	{0x0055, 0x0055, 0x0055, 0x0055, 0x0055, 0x0055, 0x0055, 0x0055}, // rightWall
	{0x5555, 0x5555, 0x5555, 0x5555, 0x0000, 0x0000, 0x0000, 0x0000}, // topWall
	{0x0000, 0x0000, 0x0000, 0x0000, 0x5555, 0x5555, 0x5555, 0x5555}, // bottomWall
	{0x5555, 0x5555, 0x5555, 0x5555, 0x5500, 0x5500, 0x5500, 0x5500}, // topLeftWall
	{0x5555, 0x5555, 0x5555, 0x5555, 0x0055, 0x0055, 0x0055, 0x0055}, // topRightWall
	{0x5500, 0x5500, 0x5500, 0x5500, 0x5555, 0x5555, 0x5555, 0x5555}, // bottomLeftWall
	{0x0055, 0x0055, 0x0055, 0x0055, 0x5555, 0x5555, 0x5555, 0x5555}, // bottomRightWall
	{0x0550, 0x0550, 0x0550, 0x0550, 0x0550, 0x0550, 0x0550, 0x0550}, // verticalWall
	{0x0000, 0x0000, 0x5555, 0x5555, 0x5555, 0x5555, 0x0000, 0x0000}, // horizontalWall
	{0x0000, 0x0000, 0x0000, 0x0140, 0x0140, 0x0000, 0x0550, 0x1144}, // playerStandingL
	{0x1144, 0x0140, 0x0140, 0x0140, 0x0140, 0x0150, 0x0000, 0x0000},
	{0x0000, 0x0000, 0x0000, 0x0140, 0x0140, 0x0000, 0x0540, 0x1140}, // playerWalkingL
	{0x0550, 0x0140, 0x0140, 0x0040, 0x1410, 0x1010, 0x0000, 0x0000},
	{0x0000, 0x0000, 0x0000, 0x0140, 0x0140, 0x0000, 0x0540, 0x0140}, // playerWalkingL2
	{0x0550, 0x0140, 0x0140, 0x0140, 0x1510, 0x1014, 0x0000, 0x0000},
	{0x0000, 0x0000, 0x0000, 0x0140, 0x0140, 0x0000, 0x0550, 0x1144}, // playerStandingR
	{0x1144, 0x0140, 0x0140, 0x0140, 0x0140, 0x0540, 0x0000, 0x0000},
	{0x0000, 0x0000, 0x0000, 0x0140, 0x0140, 0x0000, 0x0150, 0x0144}, // playerWalkingR
	{0x0550, 0x0140, 0x0140, 0x0100, 0x0414, 0x0404, 0x0000, 0x0000},
	{0x0000, 0x0000, 0x0000, 0x0140, 0x0140, 0x0000, 0x0150, 0x0140}, // playerWalkingR2
	{0x0550, 0x0140, 0x0140, 0x0140, 0x0454, 0x1404, 0x0000, 0x0000},
	{0x0000, 0x0000, 0x0550, 0x0410, 0x0410, 0x0550, 0x5005, 0x4001}, // playerDeadL
	{0x4411, 0x5415, 0x0410, 0x0410, 0x0450, 0x0540, 0x0000, 0x0000},
	{0x0000, 0x0000, 0x0550, 0x0410, 0x0410, 0x0550, 0x5005, 0x4001}, // playerDeadR
	{0x4411, 0x5415, 0x0410, 0x0410, 0x0510, 0x0150, 0x0000, 0x0000},
	{0x0000, 0x0000, 0x0000, 0x0aa0, 0x0aa0, 0x0000, 0x0000, 0x0000}, // horizontalShot
	{0x0000, 0x0000, 0x0280, 0x0280, 0x0280, 0x0280, 0x0000, 0x0000}, // verticalShot
	{0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
	{0x0550, 0x1554, 0x5555, 0x4551, 0x4551, 0x0410, 0x0410, 0x0410}, // robotStanding1
	{0x1414, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
	{0x0550, 0x0154, 0x5555, 0x4551, 0x4551, 0x0410, 0x0410, 0x0410}, // robotStanding2
	{0x1414, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
	{0x0550, 0x1054, 0x5555, 0x4551, 0x4551, 0x0410, 0x0410, 0x0410}, // robotStanding3
	{0x1414, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
	{0x0550, 0x1414, 0x5555, 0x4551, 0x4551, 0x0410, 0x0410, 0x0410}, // robotStanding4
	{0x1414, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
	{0x0550, 0x1504, 0x5555, 0x4551, 0x4551, 0x0410, 0x0410, 0x0410}, // robotStanding5
	{0x1414, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
	{0x0550, 0x1540, 0x5555, 0x4551, 0x4551, 0x0410, 0x0410, 0x0410}, // robotStanding6
	{0x1414, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
	{0x0550, 0x1554, 0x5555, 0x4551, 0x4551, 0x0410, 0x0410, 0x1410}, // robotWalking1
	{0x0014, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
	{0x0550, 0x1554, 0x5555, 0x4551, 0x4551, 0x0410, 0x0410, 0x0414}, // robotWalking2
	{0x1400, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
	{0x1414, 0x4001, 0x4411, 0x0000, 0x0140, 0x0140, 0x4141, 0x4001}, // robotDead
	{0x1414, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000}
};

enum
{
	blank = 0,
	leftWall = 1,
	rightWall = 2,
	topWall = 3,
	bottomWall = 4,
	topLeftWall = 5,
	topRightWall = 6,
	bottomLeftWall = 7,
	bottomRightWall = 8,
	verticalWall = 9,
	horizontalWall = 10,
	playerStandingL = 11,
	playerWalkingL = 13,
	playerWalkingL2 = 15,
	playerStandingR = 17,
	playerWalkingR = 19,
	playerWalkingR2 = 21,
	playerDeadL = 23,
	playerDeadR = 25,
	horizontalShot = 27,
	verticalShot = 28,
	robotStanding1 = 30,
	robotStanding2 = 32,
	robotStanding3 = 34,
	robotStanding4 = 36,
	robotStanding5 = 38,
	robotStanding6 = 40,
	robotWalking1 = 42,
	robotWalking2 = 44,
	robotDead = 46
};
