#include "internalFunctions.h"

#include "assert.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX_DIGITS 15


bool checkValidPlaytime(int playtime) {
    return playtime >= 0;
}

void PlayersInfoUpdate(ChessSystem chess, Tour tour, int first_player, int second_player, Winner winner, int playtime) {
    assert(chess != NULL);
    assert(tour != NULL);
    PlayerInTour player1_in_tour = mapGet(tour->playerInTour, first_player);
    assert(player1_in_tour != NULL);
    PlayerInTour player2_in_tour = mapGet(tour->playerInTour, second_player);
    assert(player2_in_tour != NULL);
    Player player1_chess = mapGet(chess->players,(void*)first_player);
    assert(player1_chess != NULL);
    Player player2_chess = mapGet(chess->players,(void*)second_player);
    assert(player2_chess != NULL);
    if (winner == FIRST_PLAYER) {
        player1_in_tour->num_wins++;
        player1_chess->num_wins++;
        player2_in_tour->num_losses++;
        player2_chess->num_losses++;
    }
    else if (winner == SECOND_PLAYER) {
        player2_in_tour->num_wins++;
        player2_chess->num_wins++;
        player1_in_tour->num_losses++;
        player1_chess->num_losses++;
    }
    else if (winner == DRAW) {
        player1_in_tour->num_draws++;
        player1_chess->num_draws++;
        player2_in_tour->num_draws++;
        player2_chess->num_draws++;
    }
    player1_in_tour->time += playtime;
    player2_in_tour->time += playtime;
    player1_chess->playtime += playtime;
    player2_chess->playtime += playtime;
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
    if (fputs_out == EOF) { return CHESS_SAVE_FAILURE; }
    return CHESS_SUCCESS;
}

bool checkValidId(int id) {
    return id>0;
}

bool checkValidlocation(const char* tournament_location)
{
    int counter=0,pointer=0;
    for(int i=0; tournament_location[i]!='\0'; i++)
    {
        if(i==0 && tournament_location[i]<'A' ||  tournament_location[i]>'Z') return false;
        if(i>0 &&  tournament_location[i]<'a' ||  tournament_location[i]>'z' || tournament_location[i]==' ') return false ;
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

char* putdoubleInStr(double doub) {
    char* str = malloc(sizeof(*str)* MAX_DIGITS);
    sprintf(str, "%lf", doub);
    return str;
}
