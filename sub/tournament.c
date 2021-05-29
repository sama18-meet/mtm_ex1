#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "game.h"
#include "gameId.h"
#include "intKey.h"
#include "internalFunctions.h"

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

void freeTour(Tour tour)
{
    mapDestroy(tour->games);
    mapDestroy(tour->player_in_tour);
    free(tour);
}

void freeTourVoid(void* tour) {
    freeTour((void*)tour);
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
        free(tour_copy);
        return NULL;
    }
    tour_copy->player_in_tour=mapCopy(tour->player_in_tour);
    if(tour_copy->player_in_tour==NULL)
    {
        free(tour_copy);
        mapDestroy(tour_copy->games);
        return NULL;
    }
    return tour_copy;
}

void* tourCopyToVoid(void* tour) {
    return (void*)tourCopy((Tour) tour);
}
Tour CreateTour(int tournament_id, int max_games_per_player, const char* tournament_location)
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
                        gameIdCmpVoid);
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
        free(tour);
        mapDestroy(tour->games);
        return NULL;
    }
    return tour;
}



void set_winner(Tour tour) {
    assert(tour != NULL);
    assert(tour->player_in_tour != NULL);
    PlayerInTour winner = mapGetFirst(tour->player_in_tour);
    MAP_FOREACH(int*, curr_player, tour->player_in_tour) {
        playerInTourUpdatePoints(mapGet(tour->player_in_tour, (void*)curr_player));
        winner = playerInTourMaxPlayer(winner, mapGet(tour->player_in_tour, curr_player)); 
    }
    tour->winner_id = playerInTourGetId(winner);
}

bool player_exceeded_games(Tour tour, int player_id) {
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


void removePlayerFromTour(Tour tour, int player_id)
 {
    assert(tour != NULL);
    MAP_FOREACH(Game, g, tour->games)
    {
        if (gameGetPlayer1Id(g) == player_id) {
            gameSetPlayer1Id(g, 0);
            gameIdChange(gameGetId(g), 0, gameGetPlayer2Id(g));
            if (tour->active) {
                gameSetWinner(g, SECOND_PLAYER);
            }

        }
        else if (gameGetPlayer2Id(g) == player_id) 
        {
            gameSetPlayer2Id(g, 0);
            gameIdChange(gameGetId(g), gameGetPlayer1Id(g), 0);
            if (tour->active) {
                gameSetWinner(g, FIRST_PLAYER);
            }
        }
    }
    mapRemove(tour->player_in_tour, &player_id);
}

char* getWinnerIdStr(Tour tour) {
    return putIntInStr(tour->winner_id);
}
char* getLongestGameTimeStr(Tour tour) {
    int longest_game_time = -1;
    MAP_FOREACH(Game, game_key, tour->games) {
        Game game = mapGet(tour->games, game_key);
        if (gameGetTime(game) > longest_game_time)
            longest_game_time = gameGetTime(game);
    }
    return putIntInStr(longest_game_time);
}
char* getAvgGameTimeStr(Tour tour) {
    int times_sum = 0;
    int num_games = 0;
    MAP_FOREACH(Game, game_key, tour->games) {
        Game game = mapGet(tour->games, game_key);
        times_sum += gameGetTime(game);
        num_games++;
    }
    assert(mapGetSize(tour->games) == num_games);
    return putDoubleInStr(times_sum/num_games);
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
