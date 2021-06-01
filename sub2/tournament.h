#ifndef _TOURNAMENT_H
#define _TOURNAMENT_H

#include <stdio.h>
#include <stdbool.h>
#include "mtm_map/map.h"
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
void tourFree(Tour tour);
void tourFreeVoid(void* tour);
Tour tourCopy(Tour tour);
void* tourCopyToVoid(void* tour);
Tour tourCreate(int tournament_id, int max_games_per_player, const char* tournament_location);
void setWinner(Tour tour);
bool playerExceededGames(Tour tour, int player_id);
PlayerInTour getPlayerInTour(Tour tour, int player_id);
bool removePlayerFromTour(Tour tour, Map players, int player_id);
char* getWinnerIdStr(Tour tour);
char* getLongestGameTimeStr(Tour tour);
char* getAvgGameTimeStr(Tour tour);
char* getLocationStr(Tour tour);
char* getNumOfGamesStr(Tour tour);
char* getNumPlayersStr(Tour tour);
#endif
