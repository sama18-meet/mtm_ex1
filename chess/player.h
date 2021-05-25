#ifndef _PLAYER_H
#define _PLAYER_H

#include <stdio.h>
#include <chessSystem.h>


typedef struct player_t *Player;
void set_level(Player player);
Player copyPlayer(Player player);
void freePlayer(Player player);
Player get_sorted_players(Player players);
void insert_player(Player sorted_players, Player player, int size);
int player_compare_levels(Player player1, Player player2);



#endif