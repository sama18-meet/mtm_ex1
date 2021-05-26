#include "map.h"
#include "chessSystem.h"
#include "tournament.h"
#include "intKey.h"
#include "internalFunctions.h"
#include "player.h"
#include "game.h"
#include "gameId.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_INT_DIGITS 10
#define NUM_TOUR_STATISTICS_FIELDS 6


struct chess_system_t {
    Map tours; // by id
    Map players; //by id data is obj ptr
};

//user's functions
ChessSystem chessCreate()
{ 
    ChessSystem chess = malloc(sizeof(*chess));
    if (chess==NULL) 
    { 
        return NULL; 
    }
    chess->tours=mapCreate(tourCopyToVoid, 
                            copyKeyInt,
                            freeTourVoid,
                            freeInt,
                            compareInts);
    if(chess->tours==NULL)
    { 
        free(chess);
        return NULL;
    }
    chess->players=mapCreate(copyPlayerToVoid,
                            copyKeyInt,
                            freePlayerVoid,
                            freeInt,
                            compareInts);
    if(chess->players==NULL)
    { 
        mapDestroy(chess->tours);
        free(chess);
        return NULL;
    }
    return chess; 
}


void chessDestroy(ChessSystem chess)
{
    if(chess == NULL) 
    {
        return;
    }         
    mapDestroy(chess->tours);
    mapDestroy(chess->players);    
    free(chess);
}

ChessResult chessAddTournament (ChessSystem chess, int tournament_id, int max_games_per_player, const char* tournament_location)
{
    if(chess==NULL || tournament_location==NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if(!checkValidId(tournament_id))
    {
        return CHESS_INVALID_ID; 
    }
    if(mapContains(chess->tours, &tournament_id))
    {
        return CHESS_TOURNAMENT_ALREADY_EXISTS; 
    }
    if(!checkValidLocation(tournament_location))
    { 
        return CHESS_INVALID_LOCATION;
    }
    if(!checkValidId(max_games_per_player))
    {
        return CHESS_INVALID_MAX_GAMES;
    }

    Tour tour=CreateTour(tournament_id, max_games_per_player, tournament_location);

    if(tour==NULL) 
    {
        chessDestroy(chess);
        return CHESS_OUT_OF_MEMORY;
    }
    MapResult mapRes=mapPut(chess->tours, &tournament_id, tour);
    assert(mapRes!=MAP_NULL_ARGUMENT);
    if(mapRes==MAP_OUT_OF_MEMORY)
    {
        freeTour(tour);
        chessDestroy(chess);
        return CHESS_OUT_OF_MEMORY;
    }
    return CHESS_SUCCESS;
}

ChessResult chessRemoveTournament (ChessSystem chess, int tournament_id)
{
    if(chess==NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if(!checkValidId(tournament_id))
    {
        return CHESS_INVALID_ID; 
    }
    if(!mapContains(chess->tours, &tournament_id))
    { 
        return CHESS_TOURNAMENT_NOT_EXIST; 
    }
   
    Tour tour_obj=mapGet(chess->tours, &tournament_id); 
    assert(tour_obj!=NULL);
    MAP_FOREACH(PlayerInTour, player_in_tour, tourGetPlayerInTour(tour_obj))
    {
        int player_in_tour_id = playerInTourGetId(player_in_tour);
        Player player=mapGet(chess->players,&player_in_tour_id);
        assert(player!=NULL);
        playerSetNumWins(player, -playerInTourGetNumWins(player_in_tour));
        playerSetNumLosses(player, -playerInTourGetNumLosses(player_in_tour));
        playerSetNumDraws(player, -playerInTourGetNumDraws(player_in_tour));
        playerSetPlaytime(player, -playerInTourGetPlaytime(player_in_tour));
        set_level(player);
    }
    mapDestroy(tourGetGames(tour_obj));
    mapDestroy(tourGetPlayerInTour(tour_obj));
    freeTour(tour_obj);
    mapRemove(chess->tours, &tournament_id);
}


ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player,
                         int second_player, Winner winner, int play_time) {
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if (!checkValidId(first_player) || !checkValidId(second_player)
        || !checkValidId(tournament_id) || first_player == second_player)
    {
        return CHESS_INVALID_ID;
    }
    Tour tour = mapGet(chess->tours, &tournament_id);
    if (tour == NULL)
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    if (!tourGetActive(tour))
    {
        return CHESS_TOURNAMENT_ENDED;
    }
    // check if game already exists
    GameId game_id = gameIdCreate(first_player, second_player);
    if (game_id == NULL)
    {
        chessDestroy(chess);
        return CHESS_OUT_OF_MEMORY;
    }
    if (mapContains(tourGetGames(tour), game_id)) {
        gameIdFree(game_id);
        return CHESS_GAME_ALREADY_EXISTS;
    }
    gameIdFree(game_id);
    // check playtime validity
    if (!checkValidPlaytime(play_time))
    {
        return CHESS_INVALID_PLAY_TIME;
    }
    // check if player exceeded games
    if (player_exceeded_games(tour, first_player) || 
        player_exceeded_games(tour, second_player))
    {
        return CHESS_EXCEEDED_GAMES;
    }
    // all is good, create game 
    PlayerInTour player1_in_tour = getPlayerInTour(tour, first_player);
    PlayerInTour player2_in_tour = getPlayerInTour(tour, second_player);
    if (player1_in_tour == NULL || player2_in_tour == NULL) {
        chessDestroy(chess);
        return CHESS_OUT_OF_MEMORY;
    }
    Game game = gameCreate(first_player, second_player, winner, play_time);
    if (game == NULL)
    {
        chessDestroy(chess);
        return CHESS_OUT_OF_MEMORY;
    }
    MapResult res = mapPut(tourGetGames(tour), gameGetId(game), game);
    gameFree(game);
    assert(res != MAP_NULL_ARGUMENT);
    if (res == MAP_OUT_OF_MEMORY)
    {
        chessDestroy(chess);
        return CHESS_OUT_OF_MEMORY;
    }
    PlayersInfoUpdate(chess->players, tourGetPlayerInTour(tour), first_player, second_player, winner, play_time);
    return CHESS_SUCCESS;

}

ChessResult chessRemovePlayer(ChessSystem chess, int player_id) {
    if (chess == NULL) { return CHESS_NULL_ARGUMENT; }
    assert(chess->players != NULL);
    if (!checkValidId(player_id)) 
    {
        return CHESS_INVALID_ID;
    }
    if (!mapContains(chess->players, &player_id))
    {
        return CHESS_PLAYER_NOT_EXIST; 
    }
    MAP_FOREACH(Tour, t_id, chess->tours)
    {
        Tour t_obj = mapGet(chess->tours, t_id);
        assert(t_obj != NULL);
        removePlayerFromTour(t_obj, player_id);
    }
    mapRemove(chess->players, &player_id);
    return CHESS_SUCCESS;
}

ChessResult chessEndTournament (ChessSystem chess, int tournament_id) {
    if (chess == NULL)
    { 
        return CHESS_NULL_ARGUMENT;
    }
    if (!checkValidId(tournament_id))
    { 
        return CHESS_INVALID_ID;
    }
    Tour tour = mapGet(chess->tours, &tournament_id);
    if (tour == NULL)
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    if (!tourGetActive(tour))
    { 
        return CHESS_TOURNAMENT_ENDED;
    }
    if (mapGetSize(tourGetGames(tour)) == 0)
    {
        return CHESS_NO_GAMES;
    }
    set_winner(tour);
    tourSetActive(tour, false);
    return CHESS_SUCCESS;
}


double chessCalculateAveragePlayTime (ChessSystem chess, int player_id, ChessResult* chess_result) {
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if (!checkValidId(player_id))
    {
        return CHESS_INVALID_ID;
    }
    Player player = mapGet(chess->players, &player_id);
    if (player == NULL)
    {
        return CHESS_PLAYER_NOT_EXIST;
    }
    int num_games=playerGetNumWins(player) + playerGetNumLosses(player) + playerGetNumDraws(player);
    return playerGetPlaytime(player)/num_games;
}


ChessResult chessSaveTournamentStatistics (ChessSystem chess, char* path_file) {
    if (chess == NULL || path_file==NULL)
    { 
        return CHESS_NULL_ARGUMENT;
    }
    bool found_ended_tour = false;
    MAP_FOREACH(Tour, tour, chess->tours) {
        if (tourGetActive(tour))
        {
            continue;
        }
        found_ended_tour = true;
        FILE* stat_file = fopen(path_file, "a"); // shold this be "a" or "w" ?
        if (stat_file == NULL)
        { 
            return CHESS_SAVE_FAILURE;
        }
        // start printing to file
        str_returning_func funcs[NUM_TOUR_STATISTICS_FIELDS] = 
                                { getWinnerIdStr,
                                  getLongestGameTimeStr,
                                  getAvgGameTimeStr,
                                  getLocationStr,
                                  getNumOfGamesStr,
                                  getNumPlayersStr };
        for (int i=0; i<NUM_TOUR_STATISTICS_FIELDS; ++i) {
            ChessResult put_res = putToFile(chess, funcs[i], stat_file, tour);
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

    
ChessResult chessSavePlayersLevels (ChessSystem chess, FILE* file)
{
    if (chess == NULL || file == NULL) {
        return CHESS_NULL_ARGUMENT;
    }
    assert(chess->players != NULL);
    int num_players = mapGetSize(chess->players);
    Player* sorted_players = get_sorted_players(chess->players);
    if (sorted_players == NULL) {
        chessDestroy(chess);
        return CHESS_OUT_OF_MEMORY;
    }
    for (; num_players>0; num_players--) {
        char* id = putIntInStr(playerGetId(sorted_players[num_players])); 
        if (id == NULL) {
            chessDestroy(chess);
            return CHESS_OUT_OF_MEMORY;
        }
        char* level = putIntInStr(playerGetLevel(sorted_players[num_players])); 
        if (level == NULL) {
            free(id);
            chessDestroy(chess);
            return CHESS_OUT_OF_MEMORY;
        }
        if (!attemptPut(file, id, id, level)) {
            return CHESS_SAVE_FAILURE;
        }
         if (!attemptPut(file, " ", id, level)) {
            return CHESS_SAVE_FAILURE;
        }
         if (!attemptPut(file, level, id, level)) {
            return CHESS_SAVE_FAILURE;
        }
        if (!attemptPut(file, "\n", id, level)) {
            return CHESS_SAVE_FAILURE;
        }
        free(id);
        free(level);

    }
    fclose(file);
    free(sorted_players);
}
