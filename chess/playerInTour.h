#ifndef _PLAYERINTOUR_H
#define _PLAYERINTOUR_H


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

void playerInTourSetNumLosses(PlayerInTour player_in_tour, int num);
void playerInTourSetNumWins(PlayerInTour player_in_tour,int num);
void playerInTourSetNumDraws(PlayerInTour player_in_tour, int num);
void playerInTourSetPlaytime(PlayerInTour player_in_tour, int num);
void playerInTourSetPoints(PlayerInTour player_in_tour, int num);



#endif
