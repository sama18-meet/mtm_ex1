#ifndef _PLAYERINTOUR_H
#define _PLAYERINTOUR_H

#include "tournament.h"


typedef struct PlayerInTour_t *PlayerInTour;

PlayerInTour createPlayerInTour(int player_id);
void playerInTourFree(PlayerInTour player_in_tour);
PlayerInTour playerInTourCopy(PlayerInTour player_in_tour);
void* playerInTourCopyToVoid(void* player_in_tour);
void playerInTourFreeVoid(void* player_in_tour);
void playerInTourUpdatePoints(PlayerInTour player_in_tour);
int playerInTourGetId(PlayerInTour player_in_tour);
int playerInTourGetNumLosses(PlayerInTour player_in_tour);
int playerInTourGetNumWins(PlayerInTour player_in_tour);
int playerInTourGetNumDraws(PlayerInTour player_in_tour);
int playerInTourGetPlaytime(PlayerInTour player_in_tour);
int playerInTourGetPoints(PlayerInTour player_in_tour);
PlayerInTour playerInTourMaxPlayer(PlayerInTour player1, PlayerInTour player2);
#endif
