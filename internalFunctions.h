#ifndef _INTERNALFUNCTIONS_H
#define _INTERNALFUNCTIONS_H

#include "mtm_map/map.h"
#include "chessSystem.h"
#include "tournament.h"
#include "playerInTour.h"
#include "player.h"

typedef void* createPalyerFunc(int);
typedef void freePlayerFunc(void*);
typedef char* (*str_returning_func)(Tour);
bool playerAddIfNew(Map map, int player_id, createPalyerFunc create_func, freePlayerFunc free_func, bool*player_is_new);
bool checkValidPlaytime(int playtime);
void PlayersInfoUpdate(Map players,Map playerInTour, int first_player, int second_player, Winner winner, int playtime);
ChessResult putToFile(ChessSystem,str_returning_func func, FILE* file, Tour tour);
bool checkValidId(int id);
bool checkValidLocation(const char* tournament_location);
bool attemptPut(FILE* file, char* str, char* id, char* level);
char* putIntInStr(int num);
char* putDoubleInStr(double doub);

#endif
