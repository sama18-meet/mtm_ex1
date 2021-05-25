#ifndef _PLAYERINTOUR_H
#define _PLAYERINTOUR_H

#include "tournament.h"


typedef struct PlayerInTour_t *PlayerInTour;

PlayerInTour createPlayerInTour(int player_id);
void playerInTourFree(PlayerInTour player_in_tour);
PlayerInTour playerInTourCopy(PlayerInTour player_in_tour);
void playerInTourUpdatePoints(PlayerInTour player_in_tour);


#endif
