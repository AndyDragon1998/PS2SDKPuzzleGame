/*****************************************************************************/
/*  Author        : Дракончик												 */
/*  Machine       : Sony PlayStation 2										 */
/*  OS		  	  : OSDSYS													 */
/*  Language      : GNU C                                                    */
/*                                                                           */
/*  File Contents : GAME STATE FOR HANDLING ALL INGAME STATES LIKE BATTLE    */
/*  File Attribute: SOURCE                                                   */
/*  File Name     : gameState.c                                              */
/*                                                                           */
/*****************************************************************************/

#include "include/gameState.h"
#include "include/menuState.h"
#include "include/overState.h"
#include "include/pad.h"
#include "include/musicManager.h"

extern StateMachine GameMachineState;

/****** User External Files ********/
#include "extern/gameState.ext"

/****** User Table Files ***********/


/****** User Define Files ***********/
#include "define/gameState.def"

// Graphics Synthesizer Texture structs
GSTEXTURE BackgroundImage;
GSTEXTURE ForegroundImage;
GSTEXTURE PieceSetImage;

BGM BackgroundMusic;

Crystal CrystalGrid[GRIDWIDTH][GRIDHEIGHT];

int GlobalClock;
int Step;

int PieceXbottomPos;
int PieceYbottomPos;

bool GameOver;

void GameStart(GSGLOBAL* gsGlobal)
{
	Step = STEP;
	GlobalClock = 0;
	GameOver = false;
	gsKit_texture_png(gsGlobal, &BackgroundImage, BACKGROUND2);
	gsKit_texture_png(gsGlobal, &PieceSetImage, PIECESET1);
	
	for(int i = 0; i < GRIDWIDTH; i++)
	{
		for(int j = 0; j < GRIDHEIGHT; j++)
		{
			if(i == 0 || i == GRIDWIDTH -1)
			{
				CrystalGrid[i][j].type = 9; // Unbreakable
				CrystalGrid[i][j].isAlive = true;
			}
			else if(j == GRIDHEIGHT -1)
			{
				CrystalGrid[i][j].type = 9; // Unbreakable
				CrystalGrid[i][j].isAlive = true;
			}
			else
			{
				CrystalGrid[i][j].type = 7;
				CrystalGrid[i][j].isAlive = true;
			}
		}
	}
	GeneratePiece();
}

void GameUpdate(GSGLOBAL* gsGlobal)
{
	GlobalClock++;
		
	if(PlaystationGamePad.UP_KEY_TAP)
	{
		
	}
	
	if(PlaystationGamePad.DOWN_KEY_TAP)
	{
		MovePiece(0);
	}
	
	if(PlaystationGamePad.LEFT_KEY_TAP)
	{
		MovePiece(1);
	}	
	
	if(PlaystationGamePad.RIGHT_KEY_TAP)
	{
		MovePiece(2);
	}
	
	if(PlaystationGamePad.BUTTON_X_KEY_TAP)
	{
		ShuflePiece(0);
	}
	
	if(PlaystationGamePad.BUTTON_O_KEY_TAP)
	{
		ShuflePiece(1);
	}
	
	
	if(GlobalClock > Step)
	{
		if(CrystalGrid[PieceXbottomPos][PieceYbottomPos +1].type != 7)
		{
			ScanGrid();
			GeneratePiece();
		}	
		else
		{
			MovePiece(0);
		}
			
		
		
	}	
}

void GameDraw(GSGLOBAL* gsGlobal, u64 colour)
{

	gsKit_prim_sprite_texture(gsGlobal, &BackgroundImage,0,  // X1
										0,  // Y1
										0,  // U1
										0.0f,  // V1
										BackgroundImage.Width  * 2, // X2
										BackgroundImage.Height * 2, // Y2
										BackgroundImage.Width  * 2, // U2
										BackgroundImage.Height * 2, // V2
										2,
										colour);	

	for(int i = 0; i < GRIDWIDTH; i++)
	{
		for(int j = 0; j < GRIDHEIGHT; j++)
		{
			if(CrystalGrid[i][j].type < 8)
			{
				gsKit_prim_sprite_texture(gsGlobal, &PieceSetImage,TILEWIDTH * i + GRIDOFFSETX,  // X1
										TILEHEIGHT * j + GRIDOFFSETY,  // Y1
										TILEWIDTH/2 * CrystalGrid[i][j].type,  // U1
										0.0f,  // V1
										TILEWIDTH * i + GRIDOFFSETX + TILEWIDTH, // X2
										TILEHEIGHT * j + GRIDOFFSETY + TILEHEIGHT, // Y2
										TILEWIDTH/2 * CrystalGrid[i][j].type + TILEWIDTH/2, // U2
										TILEHEIGHT/2, // V2
										2,
										colour);		
			}
				
		}
	}

}

void GameEnd(GSGLOBAL* gsGlobal)
{

}


void GeneratePiece()
{
	int x = 0;
	PieceXbottomPos = 3;
	PieceYbottomPos = 3;
	for(int i = 1; i < 4; i++)
	{
		
		x = rand();
		x = x % 6;
		printf("x rand: %d",x);
		if(CrystalGrid[4][i].type <=5)
		{
			GameOver = true;
		}
		
		CrystalGrid[PieceXbottomPos][i].type = x;
		CrystalGrid[PieceXbottomPos][i].isAlive = true;
		
	}
	
	GlobalClock = 0;
}

void MovePiece(int direction)
{

	int tempMem[3];
	
		tempMem[0] = CrystalGrid[PieceXbottomPos][PieceYbottomPos-0].type;
		tempMem[1] = CrystalGrid[PieceXbottomPos][PieceYbottomPos-1].type;
		tempMem[2] = CrystalGrid[PieceXbottomPos][PieceYbottomPos-2].type;
	switch(direction)
	{
		case 0: // Bottom
			if(CrystalGrid[PieceXbottomPos][PieceYbottomPos +1].type == 7)
			{
				CrystalGrid[PieceXbottomPos][PieceYbottomPos -0].type = 7;
				CrystalGrid[PieceXbottomPos][PieceYbottomPos -1].type = 7;
				CrystalGrid[PieceXbottomPos][PieceYbottomPos -2].type = 7;
				PieceYbottomPos++;
				CrystalGrid[PieceXbottomPos][PieceYbottomPos -0].type = tempMem[0];
				CrystalGrid[PieceXbottomPos][PieceYbottomPos -1].type = tempMem[1];
				CrystalGrid[PieceXbottomPos][PieceYbottomPos -2].type = tempMem[2];
				GlobalClock = 0;
			}
		break;
		
		case 1: // Left
			if(CrystalGrid[PieceXbottomPos -1][PieceYbottomPos].type == 7)
			{
				CrystalGrid[PieceXbottomPos-1][PieceYbottomPos -0].type = tempMem[0];
				CrystalGrid[PieceXbottomPos-1][PieceYbottomPos -1].type = tempMem[1];
				CrystalGrid[PieceXbottomPos-1][PieceYbottomPos -2].type = tempMem[2];
				CrystalGrid[PieceXbottomPos][PieceYbottomPos -0].type = 7;
				CrystalGrid[PieceXbottomPos][PieceYbottomPos -1].type = 7;
				CrystalGrid[PieceXbottomPos][PieceYbottomPos -2].type = 7;
				PieceXbottomPos--;
			}
		break;
		
		case 2: // Right
			if(CrystalGrid[PieceXbottomPos +1][PieceYbottomPos].type == 7)
			{
				CrystalGrid[PieceXbottomPos+1][PieceYbottomPos -0].type = tempMem[0];
				CrystalGrid[PieceXbottomPos+1][PieceYbottomPos -1].type = tempMem[1];
				CrystalGrid[PieceXbottomPos+1][PieceYbottomPos -2].type = tempMem[2];
				CrystalGrid[PieceXbottomPos][PieceYbottomPos -0].type = 7;
				CrystalGrid[PieceXbottomPos][PieceYbottomPos -1].type = 7;
				CrystalGrid[PieceXbottomPos][PieceYbottomPos -2].type = 7;
				PieceXbottomPos++;
			}
		break;
	}
}

void ShuflePiece(int direction)
{
	int tempMem[3];
	
	tempMem[0] = CrystalGrid[PieceXbottomPos][PieceYbottomPos-0].type;
	tempMem[1] = CrystalGrid[PieceXbottomPos][PieceYbottomPos-1].type;
	tempMem[2] = CrystalGrid[PieceXbottomPos][PieceYbottomPos-2].type;
	
	if(direction == 1)
	{
		CrystalGrid[PieceXbottomPos][PieceYbottomPos-0].type =tempMem[2];
		CrystalGrid[PieceXbottomPos][PieceYbottomPos-1].type =tempMem[0];
		CrystalGrid[PieceXbottomPos][PieceYbottomPos-2].type =tempMem[1];
	}
	else
	{
		CrystalGrid[PieceXbottomPos][PieceYbottomPos-0].type =tempMem[1];
		CrystalGrid[PieceXbottomPos][PieceYbottomPos-1].type =tempMem[2];
		CrystalGrid[PieceXbottomPos][PieceYbottomPos-2].type =tempMem[0];	
	}
}

void ScanGrid()
{
	bool hasActivity = false;

		for(int i = 2; i < GRIDWIDTH; i++)
		{
			for(int j = 2; j < GRIDHEIGHT; j++)
			{
				if((CrystalGrid[i][j].type == CrystalGrid[i][j-1].type && CrystalGrid[i][j].type == CrystalGrid[i][j-2].type) && CrystalGrid[i][j].type <= 6)
				{
					CrystalGrid[i][j].isAlive = false;
					CrystalGrid[i][j-1].isAlive = false;
					CrystalGrid[i][j-2].isAlive = false;
					hasActivity = true;
				}
				
				if((CrystalGrid[i][j].type == CrystalGrid[i-1][j].type && CrystalGrid[i][j].type == CrystalGrid[i-2][j].type) && CrystalGrid[i][j].type <= 6)
				{
					CrystalGrid[i][j].isAlive = false;
					CrystalGrid[i-1][j].isAlive = false;
					CrystalGrid[i-2][j].isAlive = false;
					hasActivity = true;
				}
				
				if((CrystalGrid[i][j].type == CrystalGrid[i-1][j-1].type && CrystalGrid[i][j].type == CrystalGrid[i-2][j-2].type) && CrystalGrid[i][j].type <= 6)
				{
					CrystalGrid[i][j].isAlive = false;
					CrystalGrid[i-1][j-1].isAlive = false;
					CrystalGrid[i-2][j-2].isAlive = false;
					hasActivity = true;
				}
			}
		}
		
		for(int i = GRIDWIDTH-1; i > 2; i--)
		{
			for(int j = GRIDHEIGHT-1; j > 2; j--)
			{
				if((CrystalGrid[i][j].type == CrystalGrid[i+1][j-1].type && CrystalGrid[i][j].type == CrystalGrid[i+2][j-2].type) && CrystalGrid[i][j].type < 6)
				{
					CrystalGrid[i][j].isAlive = false;
					CrystalGrid[i+1][j-1].isAlive = false;
					CrystalGrid[i+2][j-2].isAlive = false;
					hasActivity = true;
				}
			}
		}
		
		if(hasActivity)
		{
			ClearGrid();
			ScanGrid();
		}
		
		
}


void ClearGrid()
{
	for(int i = 0; i < GRIDWIDTH; i++)
	{
		for(int j = 0; j < GRIDHEIGHT; j++)
		{
			if(CrystalGrid[i][j].isAlive == false)
			{
				CrystalGrid[i][j].type = 7;
				CrystalGrid[i][j].isAlive = true;
			}
		}
	}
	
	
	for(int z = 0; z < 20; z++)
	{
		for(int i = 0; i < GRIDWIDTH; i++)
		{
			for(int j = GRIDHEIGHT -1; j > 0; j--)
			{
				if(CrystalGrid[i][j+1].type == 7 && CrystalGrid[i][j].type < 6)
				{
					CrystalGrid[i][j+1].type = CrystalGrid[i][j].type;
					CrystalGrid[i][j].type = 7;
				}
			}
		}
	}

	/*int tempType = 0;
	int bottom = 0;
	for(int i = GRIDWIDTH - 1; i > 0; i--)
	{
		for(int j = GRIDHEIGHT - 1; j > 0; j--)
		{
			tempType =CrystalGrid[i][j].type;
			bottom = j;
			
			while(CrystalGrid[i][bottom+1].type == 7)
			{				
				CrystalGrid[i][bottom].type = 7;
				CrystalGrid[i][bottom+1].type = tempType;
				bottom++;
			}

		}
	}*/
}

StateManager GameState =
{
	GameStart,
	GameUpdate,
	GameDraw,
	GameEnd
};
