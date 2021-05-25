#ifndef _GAME_H
#define _GAME_H

#include <stdio.h>
#include <chessSystem.h>


typedef struct Game_t *Game;

Game gameCreate(int player1_id, int player2_id, Winner winner, int time);
Game gameCopy(Game game);
void gameFree(Game game);

#endif
