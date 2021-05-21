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
    Map players; // key is player id, data is game_counter
} *Tour;



struct Game {
    char* id;
    int player1_id;
    int player2_id;
    Winner winner;
    int time;
} *Game;


struct Player {
    int id;
    int num_losses;
    int num_wins;
    int num_draws;
    Map games; /// ??
} *Player



//// INTERNAL FUNCTIONS ////

int calc_level(Player player);
int calc_points(Player player);
void destroy(ChessSystem chess_system);
void set_gmae_id(Game game);
Player set_tour_winner(Tour tour, Player player) // ?
