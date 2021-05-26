#ifndef _GAME_H
#define _GAME_H

#include <stdio.h>
#include "chessSystem.h"
#include "gameId.h"


typedef struct Game_t *Game;

Game gameCreate(int player1_id, int player2_id, Winner winner, int time);
Game gameCopy(Game game);
void* gameCopyToVoid(void* game);
void gameFree(Game game);
void gameFreeVoid(void* game);

GameId gameGetId(Game game);
int gameGetPlayer1Id(Game game);
int gameGetPlayer2Id(Game game);
Winner gameGetWinner(Game game);
int gameGetTime(Game game);
void gameSetPlayer1Id(Game game, int id);
void gameSetPlayer2Id(Game game, int id);
void gameSetWinner(Game game, Winner winner);

#endif

