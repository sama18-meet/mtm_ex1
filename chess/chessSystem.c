#include <map.h>

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
    Map player; // key is player id, data is playerInTour
    int num_players = 0;
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
    int num_games = 0;
    int points;
}

struct Player {
    int id;
    int num_losses;
    int num_wins;
    int num_draws;
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


/////////////// DECLERATIONS OF IMPLEMENTED INTERNAL FUNCTIONS /////////////////
void removePlayerFromTour(Tour t_obj, int player_id);
bool invalid_player_id(int id);
void set_game_id(Game g);
////////////////////////////////////////////////////////////////////////////////
bool invalid_player_id(int id) { return id>0; }

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
    if (invalid_player_id(player_id)) { return CHESS_INVALID_ID; }
    if (!mapContains(chess.players, player_id)) { return CHESS_PLAYER_NOT_EXIST; }
    for (MapKeyElement t_id = mapGetFirst(chess.tours); t_id != NULL; t_id = mapGetNext(chess.tour)) {
        MapDataElement t_obj = mapGet(chess.tours, t_id);
        removePlayerFromTour(t_obj, player_id);
    mapRemove(chess.players, player_id);  
    return CHESS_SUCCESS;
}





////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// RAGHAD'S BLOCK //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


