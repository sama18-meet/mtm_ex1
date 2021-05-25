#ifndef _GAME_H
#define _GAME_H

#include <stdio.h>
#include <chessSystem.h>



typedef struct game_t *Game;

Game copyGame(Game game);
Game createGame(int player1, int player2, Winner winner, int time);
void freeGame(Game game);

#endif