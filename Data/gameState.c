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

bool GameOver;
bool nextPiece;


void GameStart(GSGLOBAL* gsGlobal)
{
	Step = STEP;
	GlobalClock = 0;
	GameOver = false;
	nextPiece = true;
	gsKit_texture_png(gsGlobal, BackgroundImage, BACKGROUND1);
	gsKit_texture_png(gsGlobal, PieceSetImage, PIECESET1);
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
		
	}
	
	if(PlaystationGamePad.LEFT_KEY_TAP)
	{

	}	
	
	if(PlaystationGamePad.RIGHT_KEY_TAP)
	{

	}
	
	if(PlaystationGamePad.BUTTON_X_KEY_TAP)
	{

	}
	
	if(PlaystationGamePad.BUTTON_O_KEY_TAP)
	{

	}	
}

void GameDraw(GSGLOBAL* gsGlobal, u64 colour)
{

}

void GameEnd(GSGLOBAL* gsGlobal)
{

}


void GeneratePiece()
{
	int x = 0;
	for(int i = 0; i < 3; i++)
	{
		x = rand();
		x = x % 6;
		if(CrystalGrid[3][i].type <=5)
		{
			GameOver = true;
		}
		CrystalGrid[3][i].type = x;
		CrystalGrid[3][i].isAlive = true;
		CrystalGrid[3][i].isControlled = true;
	}
	
	nextPiece = false;
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
