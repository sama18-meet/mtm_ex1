#ifndef _PLAYER_H
#define _PLAYER_H
#include "mtm_map/map.h"

#include <stdio.h>
#include "chessSystem.h"


typedef struct Player_t *Player;

void setLevel(Player player);
Player playerCreate(int player_id);
void* playerCreateVoid(int player_id);
Player playerCopy(Player player);
void* playerCopyToVoid(void* player);
void playerFree(Player player);
void playerFreeVoid(void* player);
Player* getSortedPlayers(Map players);
void insertPlayer(Player* sorted_players, Player player, int size);
double playerCompareLevels(Player player1, Player player2);

int playerGetNumLosses(Player player);
int playerGetNumWins(Player player);
int playerGetNumDraws(Player player);
int playerGetNumGames(Player player);
int playerGetPlaytime(Player player);
double playerGetLevel(Player player);
int playerGetId(Player player);

void playerSetNumLosses(Player player, int num);
void playerSetNumWins(Player player, int num);
void playerSetNumDraws(Player player, int num);
void playerSetPlaytime(Player player, int num);

#endif

