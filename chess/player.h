#ifndef _PLAYER_H
#define _PLAYER_H
#include "map.h"

#include <stdio.h>
#include "chessSystem.h"


typedef struct Player_t *Player;

void set_level(Player player);
Player copyPlayer(Player player);
void freePlayer(Player player);
Player* get_sorted_players(Map players);
void insert_player(Player* sorted_players, Player player, int size);
int player_compare_levels(Player player1, Player player2);



#endif
