#include <map.h>
#include <stdio.h>

#define MAX_INT_DIGITS 10
#define NUM_TOUR_STATISTICS_FIELDS = 6


struct chess_system_t {
    Map tours; // by id
    Map players; //by id data is obj ptr
}


ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player,
                         int second_player, Winner winner, int play_time) {
    if (chess == NULL) { return CHESS_NULL_ARGUMENT; }
    if (!valid_player_id(first_player) || !valid_player_id(second_player)
        || !valid_id(tournament_id) || first_player == second_player) {
        return CHESS_INVALID_ID;
    }
    Tour tour = mapGet(chess, tournament_id);
    if (tour == NULL) { return CHESS_TOURNAMENT_NOT_EXIST; }
    if (!tour->avtive) { return CHESS_TOURNAMENT_ENDED; }
    // check if game already exists
    GameId game_id = createGameId(first_player, second_player);
    if (game_id == NULL) { return CHESS_OUT_OF_MEMORY; }
    if (mapContains(tour, game_id)) {
        free(game_id); 
        return CHESS_GAME_ALREADY_EXIST;
    }
    // check playtime validity
    if (!valid_playtime(play_time)) { return CHESS_INVALID_PLAY_TIME; }
    // check if player exceeded games
    if (player_exceeded_games(tour, first_player || 
        player_exceeded_games(tour, second_player))) { return CHESS_EXCEEDED_GAMES; }
    // all is good, create game 
    PlayerInTour player1_in_tour = getPlayerInTour(tour, first_player);
    PlayerInTour player2_in_tour = getPlayerInTour(tour, second_player);
    if (player1_in_tour == NULL || player2_in_tour == NULL) { return CHESS_OUT_OF_MEMORY; }
    Game game = createGame(game_id, first_player, second_player, winner, time);
    if (game == NULL) { return CHESS_OUT_OF_MEMORY; }
    MapResult res = mapPut(tour.games, game_id, game);
    freeGameId(game_id);
    freeGame(game);
    assert(res != MAP_NULL_ARGUMENT);
    if (res == MAP_OUT_OF_MEMORY) { return CHESS_OUT_OF_MEMORY; }
    update_players_info(chess, tour, first_player, second_player, winner, play_time);
    return CHESS_SUCCESS;

}
////////////////////////////////////////////////////////////////////////////////////////////////

bool valid_playtime(int playtime) {
    return playtime >= 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void update_players_info(ChessSystem chess, Tour tour, int first_player, int second_player, Winner winner, int playtime) {
    assert(chess != NULL);
    assert(tour != NULL);
    PlayerInTour player1_in_tour = mapGet(tour->playerInTour, first_player);
    assert(player1_in_tour != NULL);
    PlayerInTour player2_in_tour = mapGet(tour->playerInTour, second_player);
    assert(player2_in_tour != NULL);
    Player player1_chess = mapGet(chess.players, first_player);
    assert(player1_chess != NULL);
    Player player2_chess = mapGet(chess.players, second_player);
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
    player1_in_tour->playtime += playtime;
    player2_in_tour->playtime += playtime;
}


///////////////////////////// ChessRemovePlayer ////////////////////////////////
ChessResult chessRemovePlayer(ChessSystem chess, int player_id) {
    if (chess == NULL) { return CHESS_NULL_ARGUMENT; }
    assert(chess->players != NULL);
    if (!valid_player_id(player_id)) { return CHESS_INVALID_ID; }
    if (!mapContains(chess.players, player_id)) { return CHESS_PLAYER_NOT_EXIST; }
    MAP_FOREACH(Tour, t_id, chess.tours) {
        MapDataElement t_obj = mapGet(chess.tours, t_id);
        assert(t_obj != NULL);
        removePlayerFromTour(t_obj, player_id);
    mapRemove(chess->players, player_id);  
    return CHESS_SUCCESS;
}

////////////////////////////// chessEndTournament //////////////////////////////
ChessResult chessEndTournament (ChessSystem chess, int tournament_id) {
    if (chess == NULL) { return CHESS_NULL_ARGUMENT; }
    if (!valid_id(tournament_id)) { return CHESS_INVALID_ID; }
    Tour tour = mapGet(chess.tours, tournament_id);
    if (tour == NULL) { return CHESS_TOURNAMENT_NOT_EXIST; }
    if (!tour.active) { return CHESS_TOURNAMENT_ENDED; }
    if (mapGetSize(tour.games) == 0) { return CHESS_N0_GAMES; }
    PlayerInTour winner = get_winner(tour);
    if (winner == NULL) { return CHESS_OUT_OF_MEMORY; }
    tour->winner = winner;
    tour->active = false;
    return CHESS_SUCCESS;
}

/////////////////////////// chessCalcAvgPlayTime /////////////////////////////////


double chessCalculateAveragePlayTime (ChessSystem chess, int player_id, ChessResult* chess_result) {
    if (chess == NULL) { return CHESS_NULL_ARGUMENT; }
    if (!valid_player_id(player_id)) { return CHESS_INVALID_ID; }
    Player player = mapGet(chess.players, player_id);
    if (player == NULL) { return CHESS_PLAYER_NOT_EXIST; }
    return player->playtime/(player->num_wins + player->num_losses + player->num_draws);
}

/////////////////////////// chessSaveTourStats /////////////////////////////////

ChessResult chessSaveTournamentStatistics (ChessSystem chess, char* path_file) {
    if (chess == NULL) { return CHESS_NULL_ARGUMENT; }
    bool found_ended_tour = false;
    MAP_FOREACH(Tour, tour, chess.tours) {
        if (tour->active) { continue; }
        found_ended_tour = true;
        FILE* stat_file = fopen(path_file, "a"); // shold this be "a" or "w" ?
        if (stat_file == NULL) { return CHESS_SAVE_FAILURE; }
        // start printing to file
        str_returning_func funcs[NUM_TOUR_STATISTICS_FIELDS] = 
                                { get_winner_id,
                                  get_longest_game_time,
                                  get_avg_game_time,
                                  get_location,
                                  get_num_of_games,
                                  get_num_players };
        for (int i=0; i<NUM_TOUR_STATISTICS_FIELDS; ++i) {
            ChessResult put_res = put_to_file(funcs[i], stat_file, tour);
            if (put_res != CHESS_SUCCESS) {
                fclose(stat_file);
                return put_res;
            }
        }
        fclose(stat_file);
    }
    if (!found_ended_tour) { return CHESS_NO_TOURNAMENTS_ENDED; }
    return CHESS_SUCCESS;
}

typedef char* (*str_returning_func)(Tour);
ChessResult put_to_file(*str_returning_func func, FILE* file, Tour tour) {
    char* out = func(tour);
    if (out == NULL) { return CHESS_OUT_OF_MEMORY; }
    int fputs_out = fputs(file, out);
    free(out);
    if (fputs_out == EOF) { return CHESS_SAVE_FAILURE; }
    return CHESS_SUCCESS;
}


char* int_to_str(int n);                       ///////////// NEEDS IMPLEMENTATION
char* double_to_str(double d, int num_digits); ///////////// NEEDS IMPLEMENTATION


////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// RAGHAD'S BLOCK //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
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


ChessSystem chessCreate(){ 
    ChessSystem chess = malloc(sizeof(*chess));
    if (chess==NULL) { return NULL; }

     chess->tours=mapCreate(copyTour, 
                        copyInt,
                        freeTour,
                        freeKey,
                        compareInts);
     if(chess->tours==NULL) return NULL;
    
     chess->players=mapCreate(copyPlayer,
                        copyInt,
                        freePlayer,
                        freeKey,
                        compareInts;
    if(chess->players==NULL)
    { 
        mapDestroy(chess->tours);
        return NULL;
    }
   return chess; 
}


void chessDestroy(ChessSystem chess)
{
    if(chess != NULL) 
    {
    for (MapKeyElement tours_id = mapGetFirst(chess->tours); tours_id != NULL; tours_id = mapGetNext(chess->tour))
        {
            //FOREACH (casting to int here )
            chessRemoveTournament(chess, tours_id); 
        }
    mapDestroy(chess.tours);

    for (MapKeyElement players_id = mapGetFirst(chess.players); players_id != NULL; players_id = mapGetNext(chess.players))
        {
           MapDataElement players_obj = mapGet(chess.players, players_id);
           mapDestroy(chess.players); 
        }
    mapDestroy(chess.players); 
    free(chess);
    }
}


ChessResult chessAddTournament (ChessSystem chess, int tournament_id, int max_games_per_player, const char* tournament_location)
{
    if(!valid_id(tournament_id)) return CHESS_INVALID_ID; 
    if(mapContains(chess->tours, tournament_id)) { return CHESS_TOURNAMENT_ALREADY_EXISTS; }
    if(!location_valid(tournament_location)) { retrun CHESS_INVALID_LOCATION; }
    if(!valid_id(max_games_per_player)) {return CHESS_INVALID_MAX_GAMES;}

    CreateTour(tour, tournament_id, max_games_per_player, tournament_location);
    mapPut(chess->tours, tournament_id, tour);
    freeTour(tour);
}


ChessResult chessRemoveTournament (ChessSystem chess, int tournament_id)
{
    if(!valid_id(tournament_id)) return CHESS_INVALID_ID; 
    if(!mapContains(chess->tours, tournament_id)) { return CHESS_TOURNAMENT_NOT_EXISTS; }
   
   
    MapDataElement tour_obj=mapGet(chess->tours, tournament_id);
    if(tour_obj==NULL) return NULL;
    freeTour(tour_obj;
    free(tour_obj);

    // make sure of the *t_obj
    for (MapKeyElement player_id = mapGetFirst(*tour_obj.playerInTour); player_id != NULL; player_id = mapGetNext(*tour_obj.player))
     {
         MapDataElement currentPlayerData=mapGet(chess.tours.*tour_obj.playerInTour, player_id);
         MapDataElement Tot_Player_data=mapGet(chess.players, player_id);
        *(*int)(Total_player_data.num_wins)-=*(*int)(currentPlayerData.num_wins);
        *(*int)(Total_player_data.num_losses)-=*(*int)(currentPlayerData.num_losses);
        *(*int)(Total_player_data.num_draws)-=*(*int)(currentPlayerData.num_draws);
        *(*int)(Total_player_data.level)=calc_level(Tot_Player_data);
    }
    mapDestroy(chess.tours.*tour_obj.games);
    mapDestroy(chess.tours.*tour_obj.playerInTour);
    free(tour);
    mapRemove(chess.tours, tournament_id);
}


ChessResult chessSavePlayersLevels (ChessSystem chess, FILE* file)
{
    if (chess == NULL) {
        return CHESS_NULL_ARGUMENT;
    }
    assert(chess->players != NULL);
    int num_players = mapGetSize(chess->players);
    assert(num_payers != -1);
    Player sorted_players = malloc(sizeof(sorted_player)*mapGetSize(chess->players));
    get_sorted_players(sorted_players, chess->players);

    free(sorted_players);
    // no printing emplemnted
}
