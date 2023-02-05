#ifndef GAME_STATE
#define GAME_STATE

#include "stateManager.h"
#include <boolean.h>

typedef struct
{
	int type;
	bool isAlive;
	bool isControlled;
}Crystal

void GameStart(GSGLOBAL* gsGlobal);
void GameUpdate(GSGLOBAL* gsGlobal);
void GameEnd(GSGLOBAL* gsGlobal);

void ScanGrid();

extern StateManager GameState;

#endif
