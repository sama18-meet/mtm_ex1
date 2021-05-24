#include <map.h>
#include <stdio.h>

#define POINTS_PER_WIN 2
#define POINTS_PER_LOSS 0
#define POINTS_PER_DDRAW 1
#define MAX_INT_DIGITS 10
#define NUM_TOUR_STATISTICS_FIELDS = 6


struct chess_system_t {
    Map tours; // by id
    Map players; //by id data is obj ptr
}

struct Tour {
    int id;
    const char* location;
    Winner winner;
    int max_games_per_player;
    bool active;
    Map games;
    Map players; // key is player id, data is playerInTour, only active players are included
    num_players; // including deleted players
} *Tour;

struct Game {
    char* id;
    int player1_id;
    int player2_id;
    Winner winner;
    int time;
} *Game;


struct PlayerInTour {
    int id;
    int num_losses = 0;
    int num_wins = 0;
    int num_draws = 0;
    int points;
}

struct Player {
    int id;
    int num_losses;
    int num_wins;
    int num_draws;
    int playtime;
    int level;
} *Player

//// INTERNAL FUNCTIONS ////
int calc_level(Player player);
int calc_points(Player player);
void destroy(ChessSystem chess_system);
void set_gmae_id(Game game);
Player set_tour_winner(Tour tour, Player player) // ?
void update_points_per_player(PlayerInTour player);
void update_level_per_player(Player player);





////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// SAMA'S BLOCK //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


//////////////////////////////// ChessAddGame //////////////////////////////////

ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player,
                         int second_player, Winner winner, int play_time) {
    if (chess == NULL) { return CHESS_NULL_ARGUMENT; }
    if (!valid_player_id(first_player) || !valid_player_id(second_player)
        || !valid_tour_id(tournament_id) || first_player == second_player) {
        return CHESS_INVALID_ID;
    }
    Tour tour = mapGet(chess, tournament_id);
    if (tour == NULL) { return CHESS_TOURNAMENT_NOT_EXIST; }
    if (!tour->avtive) { return CHESS_TOURNAMENT_ENDED; }
    char* game_id = generate_game_id(first_player, second_player);
    if (mapContains(tour, game_id)) { return CHESS_GAME_ALREADY_EXIST; }
    if (!valid_playtime(play_time)) { return CHESS_INVALID_PLAY_TIME; }
    if (player_exceeded_games(tour, first_player || 
        player_exceeded_games(tour, second_player))) { return CHESS_EXCEEDED_GAMES; }
    PlayerInTour player1_in_tour = getPlayerInTour(tour, first_player);
    PlayerInTour player2_in_tour = getPlayerInTour(tour, second_player);
    if (player1_in_tour == NULL || player2_in_tour == NULL) { return CHESS_OUT_OF_MEMORY; }
    Game game = createGame(game_id, first_player, second_player, winner, time);
    if (game == NULL) { return CHESS_OUT_OF_MEMORY; }
    MapResult res = mapPut(tour.games, game_id, game);
    assert(res != MAP_NULL_ARGUMENT);
    if (res == MAP_OUT_OF_MEMORY) { return NULL; }
    update_players_info(chess, tour, first_player, second_player, winner, play_time);
    return CHESS_SUCCESS;

}
////////////////////////////////////////////////////////////////////////////////////////////////

bool valid_player_id(int id) {
    return id>0;
}
bool valid_tour_id(int id) {
    return id>0;
}
bool valid_playtime(int playtime) {
    return playtime >= 0;
}


bool player_exceeded_games(Tour tour, player_id) {
    assert(tour!=NULL);
    if (!mapContains(tour->players, player_id)
        return false;
    PlayerInTour player = mapGet(tour->players, player_id);
    assert(player!=NULL);
    if (player->max_games_per_player == tour->max_games_per_player)
        return true;
    return false;
}

// if player doesn't already exist in tour, it adds it
PlayerInTour getPlayerInTour(Tour tour, int player_id) {    
    PlayerInTour player_in_tour = mapGet(tour.players, player_id);
    if (player_in_tour == NULL) {
        player_in_tour = createPlayerInTour(player_id);
        if (player_in_tour == NULL) { return NULL; }
        MapResult res = mapPut(tour.players, player_id, player_in_tour);
        assert(res != MAP_NULL_ARGUMENT);
        if (res == MAP_OUT_OF_MEMORY) { return NULL; }
        tour->num_players++;
    }
    return player_in_tour;
}

PlayerInTour createPlayerInTour(int player_id) {
    PlayerInTour player = malloc(sizeof(*player));
    if (player == NULL) { return NULL; }
    player->id = player_id;
    player->num_losses = 0;
    player->num_wins = 0;
    player->num_draws = 0;
    player->points = 0;
    return player;
}


Game createGame(char* id, int player1, int player2, Winner winner, int time) {
    Game game = malloc(sizeof(*game));
    if (game == NULL) { return NULL; }
    game->id = id;
    game->player1_id = player1;
    game->player2_id = player2;
    game->winner = winner;
    game->time = time;
    return game;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void update_players_info(ChessSystem chess, Tour tour, int first_player, int second_player, Winner winner, int playtime) {
    assert(chess != NULL);
    assert(tour != NULL);
    PlayerInTour player1_in_tour = mapGet(tour.players, first_player);
    assert(player1_in_tour != NULL);
    PlayerInTour player2_in_tour = mapGet(tour.players, second_player);
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

void removePlayerFromTour(Tour tour, int player_id) {
    if (tour.active) {
        MAP_FOREACH(Game, g, tour.games) {
            if (g.player1_id == player_id) {
                g.winner = SECOND_PLAYER;
                g.player1_id = 0;
            }
            else if (g.player2_id == player_id) {
                g.winner = FIRST_PLAYER;
                g.player2_id = 0;
            set_game_id(g);
            }
    }
    mapRemove(tour.players, player_id);
}

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

void update_points(PlayerInTour player) {
    assert(player != NULL);
    int sum = player->num_wins*POINTS_PER_WIN
              + player->num_losses*POINTS_PER_LOSS
              + player->num_draws*POINTS_PER_DDRAW;
    player->points = sum;
}

PlayerInTour max_player(PlayerInTour player1, PlayerInTour player2) {
    if (player1->points != player2->points) 
        return (player1->points > player2->points) ? player1 : player2;
    if (player1->num_losses != player2->num_losses)
        return (player1->num_losses < player2->num_losses) ? player1 : player2;
    if (player1->num_wins != player2->num_wins)
        return (player1->num_wins > player2->num_wins) ? player1 : player2;
    return (player1->id < player2->id) : player1 : player2;
}

PlayerInTour get_winner(Tour tour) {
    assert(tour != NULL);
    assert(tour->players != NULL);
    Player winner = mapGetFirst(players);
    MAP_FOREACH(int, curr_player, players) {
        update_points(player);
        winner = max_player(winner, curr_player); 
    }
    tour.winner = winner;
    return winner;
}

ChessResult chessEndTournament (ChessSystem chess, int tournament_id) {
    if (chess == NULL) { return CHESS_NULL_ARGUMENT; }
    if (!valid_tour_id(tournament_id)) { return CHESS_INVALID_ID; }
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
            if (put_res != CHESS_SUCCESS) { return put_res; }
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


/////////////////////////////////////////////////////////////////////////////
char* get_winner_id(Tour tour) {
    return int_to_str(winner_id);
}
char* get_longest_game_time(Tour tour) {
    int longes_game_time = -1;
    MAP_FOREACH(Game, game, tour->games) {
        if (game->time > longest_game_time)
            longest_game_time = game->time;
    }
    return int_to_str(longest_game_time)
}
char* get_avg_game_time(Tour tour) {
    int times_sum = 0;
    int num_games = 0
    MAP_FOREACH(Game, game, tour->games) {
        times_sum += game->time;
        num_games++;
    }
    assert(mapGetSize(tour->games) == num_games);
    return double_to_str(times_sum/num_games, 2);
}
char* get_location(Tour tour) {
    return tour->location;
}
char* get_num_of_games(Tour tour) {
    return int_to_str(mapGetSize(tour->games);
}
char* get_num_players(Tour tour) {
    return int_to_str(tour->num_players);
}

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

     chess.games=mapCreate(copyDataElement,
                        copyKeyElement,
                        freeDataElement,
                        freeKeyElement,
                        compareKeyElements);
     chess.players=mapCreate(copyDataElement,
                        copyKeyElement,
                        freeDataElement,
                        freeKeyElement,
                        compareKeyElements);
   return chess; 
}


void chessDestroy(ChessSystem chess)
{
    if(chess != NULL) 
    {
    for (MapKeyElement tours_id = mapGetFirst(chess.tours); tours_id != NULL; tours_id = mapGetNext(chess.tour))
        {
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
    if(mapContains(chess.tours, tournament_id)) { return CHESS_TOURNAMENT_ALREADY_EXISTS; }
    if(!location_valid(tournament_location)) { retrun CHESS_INVALID_LOCATION; }
    if(!valid_id(max_games_per_player)) {return CHESS_INVALID_MAX_GAMES;}

    Tour tour= malloc(sizeof(*tour));
    if(tour==NULL) return NULL; //check the return
    SetTour(tour, tournament_id, max_games_per_player, tournament_location);
    mapPut(chess.tours, tournament_id, tour);
}


Tour SetTour(Tour tour, int tournament_id, int max_games_per_player, const char* tournament_location)
{
    tour.id= tournament_id;
    tour.location= tournament_location;
    tour.winner=NULL; //if NULL?? 
    tour.max_games_per_player=max_games_per_player;
    tour.active=false; // ?????
    tour.games=mapCreate(copyDataElement,
                        copyKeyElement,
                        freeDataElement,
                        freeKeyElement,
                        compareKeyElements);
    tour.playerInTour=mapCreate(copyDataElement,
                        copyKeyElement,
                        freeDataElement,
                        freeKeyElement,
                        compareKeyElements);
    tour.num_players = 0;
    return tour;
}


int calc_level(Player player)
{
   return (6*(player.num_wins))-(10*(player.num_losses))+(2*(player.num_draws))/((player.num_draws)+(player.num_losses)+(player.num_wins));   
}

ChessResult chessRemoveTournament (ChessSystem chess, int tournament_id)
{
    if(!valid_id(tournament_id)) return CHESS_INVALID_ID; 
    if(!mapContains(chess.tours, tournament_id)) { return CHESS_TOURNAMENT_NOT_EXISTS; }
    MapDataElement tour_obj=mapGet(chess.tours, tournament_id);
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

    levels_map=mapCreate(copyDataElement,
             copyKeyElement,
             freeDataElement, 
             freeKeyElement,
             compareKeyElements);

    MapKeyElement player_key = mapGetFirst(chess.players);

    for (MapDataElement player_data=mapGet(chess.players, player_key); player_data != NULL; player_data = mapGetNext(chess.player))
    {
        mapPut(levels_map, player_key, chess.players.player_data.level); // *player_data.level /&player_data.level
    }
    // no printing emplemnted
}

