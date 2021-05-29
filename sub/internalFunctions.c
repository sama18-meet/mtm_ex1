#include "internalFunctions.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_DIGITS 15

bool checkValidPlaytime(int playtime) {
    return playtime >= 0;
}


bool playerAddIfNew(Map map, int player_id, createPalyerFunc create_func, freePlayerFunc free_func, bool*player_is_new) {
    MapDataElement player = mapGet(map, &player_id);
    *player_is_new=false;
    if (player == NULL) {
        *player_is_new=true;
        MapDataElement player = create_func(player_id);
        if (player == NULL) {
            return false;
        }
        MapResult res = mapPut(map, &player_id, player);
        assert(res != MAP_NULL_ARGUMENT);
        if (res == MAP_OUT_OF_MEMORY)
        {
            free_func(player);
            return false;
        }
        free_func(player);
    }
    return true;
}


void PlayersInfoUpdate(Map players, Map playerInTour, int first_player, int second_player, Winner winner, int playtime) {
    assert(players != NULL);
    assert(playerInTour != NULL);
    PlayerInTour player1_in_tour = mapGet(playerInTour, &first_player);
    assert(player1_in_tour != NULL);
    PlayerInTour player2_in_tour = mapGet(playerInTour, &second_player);
    assert(player2_in_tour != NULL);
    Player player1_chess = mapGet(players, &first_player);
    assert(player1_chess != NULL);
    Player player2_chess = mapGet(players, &second_player);
    assert(player2_chess != NULL);
    if (winner == FIRST_PLAYER) {
        playerInTourSetNumWins(player1_in_tour, 1);
        playerSetNumWins(player1_chess, 1);
        playerInTourSetNumLosses(player2_in_tour, 1);
        playerSetNumLosses(player2_chess, 1);
    }
    else if (winner == SECOND_PLAYER) {
        playerInTourSetNumWins(player2_in_tour, 1);
        playerSetNumWins(player2_chess, 1);
        playerInTourSetNumLosses(player1_in_tour, 1);
        playerSetNumLosses(player1_chess, 1);
    }
    else if (winner == DRAW) {
        playerInTourSetNumDraws(player1_in_tour, 1);
        playerInTourSetNumDraws(player2_in_tour, 1);
        playerSetNumDraws(player1_chess, 1);
        playerSetNumDraws(player2_chess, 1);

    }

    playerSetPlaytime(player1_chess, playtime);
    playerSetPlaytime(player2_chess, playtime);
    playerInTourSetPlaytime(player1_in_tour, playtime);
    playerInTourSetPlaytime(player1_in_tour, playtime);
}

ChessResult putToFile(ChessSystem chess,str_returning_func func, FILE* file, Tour tour) {
    assert(tour!=NULL);
    char* out = func(tour);
    if (out == NULL)
    {
        chessDestroy(chess);
        return CHESS_OUT_OF_MEMORY;
    }
    int fputs_out = fputs(out,file);
    free(out);
    if (fputs_out == EOF) {
        return CHESS_SAVE_FAILURE;
    }
    if (fputs("\n", file) == EOF) {
        return CHESS_SAVE_FAILURE;
    }
    return CHESS_SUCCESS;
}

bool checkValidId(int id) {
    return id>0;
}

bool checkValidLocation(const char* tournament_location)
{
    if (!('A' <= *tournament_location && *tournament_location <='Z')) {
        return false;
    }
    for(int i=1; tournament_location[i] != '\0'; i++) {
        if (!(('a'<=tournament_location[i] && tournament_location[i]<='z') || (tournament_location[i]==' '))) {
            return false;
        }
    }
    return true;
}

bool attemptPut(FILE* file, char* str, char* id, char* level) {
    if (fputs(str, file) == EOF) {
        free(id);
        free(level);
        fclose(file);
        return false;
    }
    return true;
}

char* putIntInStr(int num) {
    char* str = malloc(sizeof(*str)* MAX_DIGITS);
    sprintf(str, "%d", num);
    return str;
}

char* putDoubleInStr(double doub) {
    char* str = malloc(sizeof(*str)* MAX_DIGITS);
    sprintf(str, "%.2f", doub);
    return str;
}
