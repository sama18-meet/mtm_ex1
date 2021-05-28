#ifndef _GAMEID_H
#define _GAMEID_H

#include <stdio.h>

typedef struct GameId_t *GameId;

GameId gameIdCreate(int id1, int id2);
GameId gameIdCopy(GameId game_id);
void* gameIdCopyToVoid(void* game_id);
void gameIdFree(GameId game_id);
void gameIdFreeVoid(void* game_id);
int gameIdCmp(GameId gameId1, GameId gameId2);
int gameIdCmpVoid(void* gameId1, void* gameId2);
void gameIdChange(GameId gameId, int newId1, int newId2);

#endif

