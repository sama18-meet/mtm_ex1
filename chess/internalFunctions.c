#include "internalFunctions.h"

#define MAX_DIGITS 15


bool valid_playtime(int playtime) {
    return playtime >= 0;
}

void update_players_info(ChessSystem chess, Tour tour, int first_player, int second_player, Winner winner, int playtime) {
    assert(chess != NULL);
    assert(tour != NULL);
    PlayerInTour player1_in_tour = mapGet(tour->playerInTour, first_player);
    assert(player1_in_tour != NULL);
    PlayerInTour player2_in_tour = mapGet(tour->playerInTour, second_player);
    assert(player2_in_tour != NULL);
    Player player1_chess = mapGet(chess->players, first_player);
    assert(player1_chess != NULL);
    Player player2_chess = mapGet(chess->players, second_player);
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

ChessResult put_to_file(*str_returning_func func, FILE* file, Tour tour) {
    assert(tour!=NULL);
    char* out = func(tour);
    if (out == NULL)
    {
        chessDestroy(chess);
        return CHESS_OUT_OF_MEMORY;
    }
    int fputs_out = fputs(file, out);
    free(out);
    if (fputs_out == EOF) { return CHESS_SAVE_FAILURE; }
    return CHESS_SUCCESS;
}


char* int_to_str(int n);                       ///////////// NEEDS IMPLEMENTATION
char* double_to_str(double d, int num_digits); ///////////// NEEDS IMPLEMENTATION

bool valid_id(int id) {
    return id>0;
}

bool location_valid(const char* tournament_location)
{
    int counter=0,pointer=0;
    for(int i=0; location[i]!='\0'; i++)
    {
        if(i==0 && location[i]<'A' ||  location[i]>'Z') return false;
        if(i>0 &&  location[i]<'a' ||  location[i]>'z' || location[i]=' ') return false ;
    }
 return true;
}

bool attempt_put(FILE* file, char* str, char* id, char* level) {
    if (fputs(file, str) == EOF) {
        free(id);
        free(level);
        fclose(file);
        return false;
    }
    return true;
}

char* int_to_str(int num) {
    char* str = malloc(sizeof(*str)* MAX_DIGITS);
    sprintf(str, "%d", n);
    return str;
}

char* double_to_str(double doub) {
    char* str = malloc(sizeof(*str)* MAX_DIGITS);
    sprintf(str, "%lf", n);
    return str;
}
