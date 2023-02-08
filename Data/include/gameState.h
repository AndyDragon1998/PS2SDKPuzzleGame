#ifndef GAME_STATE
#define GAME_STATE

#include "stateManager.h"
#include <boolean.h>

typedef struct
{
	int type;
	bool isAlive;
	bool isControlled;
}Crystal;

void GameStart(GSGLOBAL* gsGlobal);
void GameUpdate(GSGLOBAL* gsGlobal);
void GameEnd(GSGLOBAL* gsGlobal);

void GeneratePiece();
void ScanGrid();
void MovePiece(int direction); // 0 bottom 1 left 2 right
void ShuflePiece();

extern StateManager GameState;

#endif
