#ifndef _TOURNAMENT_H
#define _TOURNAMENT_H

#include <stdio.h>
#include <stdbool.h>
#include "chessSystem.h"
#include "tournament.h"
#include "playerInTour.h"

typedef struct tour_t *Tour;

void freeTour(Tour tour);
Tour copyTour(Tour tour);
Tour CreateTour(int tournament_id, int max_games_per_player, const char* tournament_location);
PlayerInTour max_player(PlayerInTour player1, PlayerInTour player2);
void set_winner(Tour tour);
bool player_exceeded_games(Tour tour, int player_id);
PlayerInTour getPlayerInTour(Tour tour, int player_id);
void removePlayerFromTour(Tour tour, int player_id);
char* get_winner_id(Tour tour);

#endif
