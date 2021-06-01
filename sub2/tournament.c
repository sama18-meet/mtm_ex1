#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "game.h"
#include "gameId.h"
#include "intKey.h"
#include "internalFunctions.h"

#define DELETED_PLAYER 0

struct Tour_t {
    int id;
    const char* location;
    int winner_id;
    int max_games_per_player;
    bool active;
    Map games;
    Map player_in_tour; // key is player id, data is playerInTour, only active players are included
    int num_players; // including deleted players
};

//get funcs 
int tourGetId(Tour tour){
    return tour->id;
}
int tourGetWinnerId(Tour tour){
    return tour->winner_id;
}
int tourGetMaxGamesPerPlayer(Tour tour){
    return tour->max_games_per_player;
}
int tourGetNumPlayers(Tour tour){
    return tour->num_players;
}
Map tourGetGames(Tour tour){
    return tour->games;
}
Map tourGetPlayerInTour(Tour tour){
    return tour->player_in_tour;
}
bool tourGetActive(Tour tour){
    return tour->active;
}
const char* tourGetLocation(Tour tour){
    return tour->location;
}

//set funcs 
void tourSetWinnerId(Tour tour,int num){
    tour->winner_id+=num;
}
void tourSetActive(Tour tour, bool active_status){
    tour->active=active_status;
}
void tourSetNumPlayers(Tour tour,int num){
    tour->num_players+=num;
}

void tourFree(Tour tour)
{
    mapDestroy(tour->games);
    mapDestroy(tour->player_in_tour);
    free(tour);
}

void tourFreeVoid(void* tour) {
    tourFree((void*)tour);
}

Tour tourCopy(Tour tour)
{
    if(tour==NULL) {
        return NULL;
    }
    Tour tour_copy= malloc(sizeof(*tour_copy));
    if(tour_copy==NULL) return NULL; 
    tour_copy->id=tour->id;
    tour_copy->location=tour->location;
    tour_copy->winner_id=tour->winner_id; 
    tour_copy->max_games_per_player=tour->max_games_per_player;
    tour_copy->active=tour->active; 
    tour_copy->num_players=tour->num_players;
    tour_copy->games=mapCopy(tour->games);
    if(tour_copy->games==NULL) {
        tourFree(tour_copy);
        return NULL;
    }
    tour_copy->player_in_tour=mapCopy(tour->player_in_tour);
    if(tour_copy->player_in_tour==NULL)
    {
        tourFree(tour_copy);
        mapDestroy(tour_copy->games);
        return NULL;
    }
    return tour_copy;
}

void* tourCopyToVoid(void* tour) {
    return (void*)tourCopy((Tour) tour);
}

Tour tourCreate(int tournament_id, int max_games_per_player, const char* tournament_location)
{
    assert(tournament_location!=NULL);
    Tour tour= malloc(sizeof(*tour));
    if(tour==NULL) return NULL; 
    tour->id= tournament_id;
    tour->location= tournament_location;
    tour->winner_id=-1;
    tour->max_games_per_player=max_games_per_player;
    tour->active=true; 
    tour->num_players = 0;
    tour->games=mapCreate(gameCopyToVoid,
                        gameIdCopyToVoid,
                        gameFreeVoid,
                        gameIdFreeVoid,
                        gameIdCompareVoid);
    if(tour->games==NULL)
    {
        free(tour);
        return NULL;
    }
    tour->player_in_tour=mapCreate(playerInTourCopyToVoid,
                        copyKeyInt,
                        playerInTourFreeVoid,
                        freeInt,
                        compareInts);
     if(tour->player_in_tour==NULL)
    {
        tourFree(tour);
        mapDestroy(tour->games);
        return NULL;
    }
    return tour;
}



void setWinner(Tour tour) {
    assert(tour != NULL);
    assert(tour->player_in_tour != NULL);
    MapKeyElement first_key = mapGetFirst(tour->player_in_tour);
    PlayerInTour winner = mapGet(tour->player_in_tour, first_key);
    freeInt(first_key);
    MAP_FOREACH(int*, curr_player_key, tour->player_in_tour) {
        PlayerInTour curr_player = mapGet(tour->player_in_tour, (void*)curr_player_key);
        playerInTourUpdatePoints(curr_player);
        winner = playerInTourMaxPlayer(winner, curr_player); 
        freeInt(curr_player_key);
    }
    tour->winner_id = playerInTourGetId(winner);
}

bool playerExceededGames(Tour tour, int player_id) {
    assert(tour!=NULL);
    if (!mapContains(tour->player_in_tour, &player_id))
    {
        return false;
    }
    PlayerInTour player = mapGet(tour->player_in_tour, &player_id);
    assert(player!=NULL);
    int num_games = playerInTourGetNumWins(player) + playerInTourGetNumLosses(player) + playerInTourGetNumDraws(player);
    assert(num_games <= tour->max_games_per_player);
    if (num_games == tour->max_games_per_player) {
        return true;
    }
    return false;
}


bool removePlayerFromTour(Tour tour, int player_id) {
    assert(tour != NULL);
    Map games_map_copy = mapCopy(tour->games);
    if (games_map_copy == NULL) {
        return false;
    }
    MAP_FOREACH(GameId, game_key, games_map_copy)
    {
        Game game = mapGet(games_map_copy, game_key);
        bool change_game = false;
        int time = gameGetTime(game);
        Winner winner = gameGetWinner(game);
        GameId old_game_id = gameGetId(game);
        int remaining_player_id;
        if (gameGetPlayer1Id(game) == player_id) {
            change_game = true;
            remaining_player_id = gameGetPlayer2Id(game);
            if (tour->active) {
                winner = SECOND_PLAYER;
            }
        }
        else if (gameGetPlayer2Id(game) == player_id)  {
            change_game = true;
            remaining_player_id = gameGetPlayer1Id(game);
            if (tour->active){
                winner = FIRST_PLAYER;
            }
        }
        if (change_game) {
            MapResult res;
            res = mapRemove(tour->games, old_game_id);
            assert(res == MAP_SUCCESS);
            Game new_game = gameCreate(DELETED_PLAYER, remaining_player_id, winner, time);
            if (new_game == NULL) {
                gameIdFree(game_key);
                mapDestroy(games_map_copy);
                return false;
            }
            res = mapPut(tour->games, gameGetId(new_game), new_game);
            if (res == MAP_OUT_OF_MEMORY) {
                gameIdFree(game_key);
                gameFree(new_game);
                mapDestroy(games_map_copy);
                return false;
            }
            assert (res == MAP_SUCCESS);
            gameFree(new_game);
        }
        gameIdFree(game_key);
    }
    mapRemove(tour->player_in_tour, &player_id);
    mapDestroy(games_map_copy);
    return true;
}

char* getWinnerIdStr(Tour tour) {
    return putIntInStr(tour->winner_id);
}
char* getLongestGameTimeStr(Tour tour) {
    int longest_game_time = -1;
    MAP_FOREACH(GameId, game_key, tour->games) {
        Game game = mapGet(tour->games, game_key);
        if (gameGetTime(game) > longest_game_time)
            longest_game_time = gameGetTime(game);
        gameIdFree(game_key);
    }
    return putIntInStr(longest_game_time);
}
char* getAvgGameTimeStr(Tour tour) {
    int times_sum = 0;
    int num_games = 0;
    MAP_FOREACH(GameId, game_key, tour->games) {
        Game game = mapGet(tour->games, game_key);
        times_sum += gameGetTime(game);
        num_games++;
        gameIdFree(game_key);
    }
    assert(mapGetSize(tour->games) == num_games);
    return putDoubleInStr(((double)times_sum)/((double)num_games));
}
char* getLocationStr(Tour tour) {
    char* location_copy = malloc(sizeof(char)*(strlen(tour->location)+1));
    strcpy(location_copy, tour->location);
    return location_copy;
}
char* getNumOfGamesStr(Tour tour) {
    return putIntInStr(mapGetSize(tour->games));
}
char* getNumPlayersStr(Tour tour) {
    return putIntInStr(tour->num_players);
}
