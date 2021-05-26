#ifndef _TOURNAMENT_H
#define _TOURNAMENT_H

#include <stdio.h>
#include <stdbool.h>
#include "map.h"
#include "chessSystem.h"
#include "tournament.h"
#include "playerInTour.h"

typedef struct Tour_t *Tour;

int tourGetId(Tour tour);
int tourGetWinnerId(Tour tour);
int tourGetMaxGamesPerPlayer(Tour tour);
int tourGetNumPlayers(Tour tour);
Map tourGetGames(Tour tour);
Map tourGetPlayerInTour(Tour tour);
bool tourGetActive(Tour tour);
const char* tourGetLocation(Tour tour);
void tourSetWinnerId(Tour tour,int num);
void tourSetActive(Tour tour,bool active_status);
void tourSetNumPlayers(Tour tour,int num);
void freeTour(Tour tour);
Tour tourCopy(Tour tour);
void* tourCopyToVoid(void* tour);
Tour CreateTour(int tournament_id, int max_games_per_player, const char* tournament_location);
void set_winner(Tour tour);
bool player_exceeded_games(Tour tour, int player_id);
PlayerInTour getPlayerInTour(Tour tour, int player_id);
void removePlayerFromTour(Tour tour, int player_id);
char* get_winner_id(Tour tour);
char* get_longest_game_time(Tour tour);
char* get_avg_game_time(Tour tour);
char* get_location(Tour tour);
char* get_num_of_games(Tour tour);
char* get_num_players(Tour tour);

#endif
