#ifndef _PLAYER_H
#define _PLAYER_H
#include "mtm_map/map.h"

#include <stdio.h>
#include "chessSystem.h"


typedef struct Player_t *Player;

void set_level(Player player);
Player createPlayer(int player_id);
void* createPlayerVoid(int player_id);
Player copyPlayer(Player player);
void* copyPlayerToVoid(void* player);
void freePlayer(Player player);
void freePlayerVoid(void* player);
Player* get_sorted_players(Map players);
void insert_player(Player* sorted_players, Player player, int size);
int player_compare_levels(Player player1, Player player2);

int playerGetNumLosses(Player player);
int playerGetNumWins(Player player);
int playerGetNumDraws(Player player);
int playerGetPlaytime(Player player);
int playerGetLevel(Player player);
int playerGetId(Player player);

void playerSetNumLosses(Player player, int num);
void playerSetNumWins(Player player, int num);
void playerSetNumDraws(Player player, int num);
void playerSetPlaytime(Player player, int num);

#endif

