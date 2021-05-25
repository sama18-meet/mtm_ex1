#ifndef _INTERNALFUNCTIONS_H
#define _INTERNALFUNCTIONS_H

#include <stdio.h>
#include <chessSystem.h>

bool valid_playtime(int playtime);
void update_players_info(ChessSystem chess, Tour tour, int first_player, int second_player, Winner winner, int playtime);
typedef char* (*str_returning_func)(Tour);
ChessResult put_to_file(*str_returning_func func, FILE* file, Tour tour);
char* int_to_str(int n);
char* double_to_str(double d, int num_digits);
bool valid_id(int id);
bool location_valid(const char* tournament_location);
bool attempt_put(FILE* file, char* str, char* id, char* level);

#endif