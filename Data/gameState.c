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
bool nextPiece;


void GameStart(GSGLOBAL* gsGlobal)
{
	Step = STEP;
	GlobalClock = 0;
	GameOver = false;
	nextPiece = true;
	gsKit_texture_png(gsGlobal, &BackgroundImage, BACKGROUND1);
	gsKit_texture_png(gsGlobal, &PieceSetImage, PIECESET1);
	
	for(int i = 0; i < GRIDWIDTH; i++)
	{
		for(int j = 0; j < GRIDHEIGHT; j++)
		{
			CrystalGrid[i][j].type = 7;
		}
		
	}
}

void GameUpdate(GSGLOBAL* gsGlobal)
{
	GlobalClock++;
	
	if(nextPiece)
	{
		GeneratePiece();
	}
	
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
			MovePiece(0);
	}	
}

void GameDraw(GSGLOBAL* gsGlobal, u64 colour)
{

	for(int i = 0; i < GRIDWIDTH; i++)
	{
		for(int j = 0; j < GRIDHEIGHT; j++)
		{
			gsKit_prim_sprite_texture(gsGlobal, &PieceSetImage,TILEWIDTH * i + GRIDOFFSETX,  // X1
										TILEHEIGHT * j + GRIDOFFSETY,  // Y1
										TILEWIDTH * CrystalGrid[i][j].type,  // U1
										0.0f,  // V1
										TILEWIDTH * i + GRIDOFFSETX + TILEWIDTH + GRIDOFFSETX, // X2
										TILEHEIGHT * j + GRIDOFFSETY + TILEHEIGHT + GRIDOFFSETY, // Y2
										TILEWIDTH * CrystalGrid[i][j].type + TILEWIDTH, // U2
										TILEHEIGHT, // V2
										2,
										colour);			
		}
	}

}

void GameEnd(GSGLOBAL* gsGlobal)
{

}


void GeneratePiece()
{
	int x = 0;
	
	for(int i = 0; i < 3; i++)
	{
		PieceXbottomPos = 3;
		PieceYbottomPos = 3;
		x = rand();
		x = x % 6;
		
		if(CrystalGrid[3][i].type <=5)
		{
			GameOver = true;
		}
		
		CrystalGrid[PieceXbottomPos][i].type == 7;
		CrystalGrid[PieceXbottomPos][i].isAlive = true;
		
	}
	
	nextPiece = false;
}

void MovePiece(int direction)
{
	switch(direction)
	{
		case 0: // Bottom
			if(CrystalGrid[PieceXbottomPos][PieceYbottomPos +1].type == 7)
			{
				CrystalGrid[PieceXbottomPos][PieceYbottomPos +1].type = CrystalGrid[PieceXbottomPos][PieceYbottomPos-0].type;
				CrystalGrid[PieceXbottomPos][PieceYbottomPos +0].type = CrystalGrid[PieceXbottomPos][PieceYbottomPos-1].type;
				CrystalGrid[PieceXbottomPos][PieceYbottomPos -1].type = CrystalGrid[PieceXbottomPos][PieceYbottomPos-2].type;
				CrystalGrid[PieceXbottomPos][PieceYbottomPos -2].type = 7;
				PieceYbottomPos++;
				GlobalClock = 0;
			}
		break;
		
		case 1: // Left
			if(CrystalGrid[PieceXbottomPos -1][PieceYbottomPos].type == 7)
			{
				CrystalGrid[PieceXbottomPos-1][PieceYbottomPos -0].type = CrystalGrid[PieceXbottomPos][PieceYbottomPos-0].type;
				CrystalGrid[PieceXbottomPos-1][PieceYbottomPos -1].type = CrystalGrid[PieceXbottomPos][PieceYbottomPos-1].type;
				CrystalGrid[PieceXbottomPos-1][PieceYbottomPos -2].type = CrystalGrid[PieceXbottomPos][PieceYbottomPos-2].type;
				CrystalGrid[PieceXbottomPos][PieceYbottomPos -0].type = 7;
				CrystalGrid[PieceXbottomPos][PieceYbottomPos -1].type = 7;
				CrystalGrid[PieceXbottomPos][PieceYbottomPos -2].type = 7;
				PieceYbottomPos++;
			}
		break;
		
		case 2: // Right
			if(CrystalGrid[PieceXbottomPos +1][PieceYbottomPos].type == 7)
			{
				CrystalGrid[PieceXbottomPos+1][PieceYbottomPos -0].type = CrystalGrid[PieceXbottomPos][PieceYbottomPos-0].type;
				CrystalGrid[PieceXbottomPos+1][PieceYbottomPos -1].type = CrystalGrid[PieceXbottomPos][PieceYbottomPos-1].type;
				CrystalGrid[PieceXbottomPos+1][PieceYbottomPos -2].type = CrystalGrid[PieceXbottomPos][PieceYbottomPos-2].type;
				CrystalGrid[PieceXbottomPos][PieceYbottomPos -0].type = 7;
				CrystalGrid[PieceXbottomPos][PieceYbottomPos -1].type = 7;
				CrystalGrid[PieceXbottomPos][PieceYbottomPos -2].type = 7;
				PieceYbottomPos++;
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
	
}

StateManager GameState =
{
	GameStart,
	GameUpdate,
	GameDraw,
	GameEnd
};
