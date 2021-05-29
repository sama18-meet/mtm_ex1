#include "player.h"
#include "chessSystem.h"

#include <stdlib.h>
#define NUM_WINS_WEIGHT 6
#define NUM_LOSSES_WEIGHT -10
#define NUM_DRAWS_WEIGHT 2


struct Player_t {
    int id;
    int num_losses;
    int num_wins;
    int num_draws;
    int playtime;
    double level;
};


//GET FUNCS
int playerGetNumLosses(Player player) {
    return player->num_losses;
}
int playerGetNumWins(Player player) {
    return player->num_wins;
}
int playerGetNumDraws(Player player) {
    return player->num_draws;
}
int playerGetNumGames(Player player) {
    return (player->num_wins)+(player->num_losses)+(player->num_draws);
}
int playerGetPlaytime(Player player) {
    return player->playtime;
}
double playerGetLevel(Player player) {
    return player->level;
}
int playerGetId(Player player) {
    return player->id;
}

//SET FUNCS
void playerSetNumLosses(Player player, int num) {
    player->num_losses+=num;
}
void playerSetNumWins(Player player, int num) {
    player->num_wins+=num;
}
void playerSetNumDraws(Player player, int num) {
    player->num_draws+=num;
}
void playerSetPlaytime(Player player, int num) {
    player->playtime+=num;
}

void set_level(Player player)
{
    int num_games= playerGetNumGames(player);
    if (num_games == 0) {
        player->level = 0;
        return;
    }
    player->level=(double)((NUM_WINS_WEIGHT*(player->num_wins))+(NUM_LOSSES_WEIGHT*(player->num_losses))+(NUM_DRAWS_WEIGHT*(player->num_draws)))/(num_games);  
}


Player createPlayer(int player_id) {
    Player player=malloc(sizeof(*player));
    if(player==NULL) {
        return NULL;
    }
    player->id=player_id;
    player->num_losses=0;
    player->num_wins=0;
    player->num_draws=0;
    player->playtime=0;
    player->level=0;
    return player;
}


void* createPlayerVoid(int player_id) {
    return (void*)createPlayer(player_id);
}


Player copyPlayer(Player player)
{
    Player CopyOfplayer=malloc(sizeof(*CopyOfplayer));
    if(CopyOfplayer==NULL) return NULL;
    CopyOfplayer->id=player->id;
    CopyOfplayer->num_losses=player->num_losses;
    CopyOfplayer->num_wins=player->num_wins;
    CopyOfplayer->num_draws=player->num_draws;
    CopyOfplayer->playtime=player->playtime;
    CopyOfplayer->level=player->level;
    return CopyOfplayer;
}

void* copyPlayerToVoid(void* player) {
    return (void*) copyPlayer((Player) player);
}

void freePlayer(Player player)
{
    free(player);
}

void freePlayerVoid(void* player) {
    freePlayer((Player)player);
}




Player* get_sorted_players(Map players)
{
    Player* sorted_players = malloc(sizeof(Player)*mapGetSize(players));
    if (sorted_players == NULL) {
        return NULL;
    }
    int size = 0;
    MAP_FOREACH(int*, player_key, players)
    {
        insert_player(sorted_players, mapGet(players, player_key), size);
        size++;
    }
    return sorted_players;
}


void insert_player(Player* sorted_players, Player player, int size) {
    for (int i=0; i<size; ++i) {
        if (player_compare_levels(sorted_players[i], player) > 0) {
            for (int j=size; j>i; j--) {
                sorted_players[j] = sorted_players[j-1];
            }
            sorted_players[i] = player;
            return;
        }
    }
    sorted_players[size] = player;
}

int player_compare_levels(Player player1, Player player2) {
    double level1 = player1->level;
    double level2 = player2->level;
    if (level1 != level2)
    {
        return level1-level2;
    }
    return player2->id - player1->id;
}
