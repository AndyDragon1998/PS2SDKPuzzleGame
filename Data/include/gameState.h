#ifndef GAME_STATE
#define GAME_STATE

#include "stateManager.h"
#include <stdbool.h>

typedef struct
{
	int type; // 0 - 6
	bool isAlive;
}Crystal;

void GameStart(GSGLOBAL* gsGlobal);
void GameUpdate(GSGLOBAL* gsGlobal);
void GameEnd(GSGLOBAL* gsGlobal);

void GeneratePiece();
void ScanGrid();
void ClearGrid();
void MovePiece(int direction); // 0 bottom 1 left 2 right
void ShuflePiece(int direction);

extern StateManager GameState;

#endif
