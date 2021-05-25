#ifndef _INTERNALFUNCTIONS_H
#define _INTERNALFUNCTIONS_H

#include "chessSystem.h"
#include "tournament.h"
#include "playerInTour.h"
#include "player.h"
#include "map.h"

typedef char* (*str_returning_func)(Tour);

bool checkValidPlaytime(int playtime);
void PlayersInfoUpdate(ChessSystem chess, Tour tour, int first_player, int second_player, Winner winner, int playtime);
ChessResult putToFile(ChessSystem,str_returning_func func, FILE* file, Tour tour);
bool checkValidId(int id);
bool checkValidlocation(const char* tournament_location);
bool attemptPut(FILE* file, char* str, char* id, char* level);
char* putIntInStr(int num);
char* putdoubleInStr(double doub);


#endif
