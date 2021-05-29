#include "player.h"
#include "chessSystem.h"

#include "intKey.h"
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

void setLevel(Player player)
{
    int num_games= playerGetNumGames(player);
    if (num_games == 0) {
        player->level = 0;
        return;
    }
    player->level=(double)((NUM_WINS_WEIGHT*(player->num_wins))+(NUM_LOSSES_WEIGHT*(player->num_losses))+(NUM_DRAWS_WEIGHT*(player->num_draws)))/(num_games);  
}


Player playerCreate(int player_id) {
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


void* playerCreateVoid(int player_id) {
    return (void*)playerCreate(player_id);
}


Player playerCopy(Player player)
{
    Player copy_of_player=malloc(sizeof(*copy_of_player));
    if(copy_of_player==NULL) return NULL;
    copy_of_player->id=player->id;
    copy_of_player->num_losses=player->num_losses;
    copy_of_player->num_wins=player->num_wins;
    copy_of_player->num_draws=player->num_draws;
    copy_of_player->playtime=player->playtime;
    copy_of_player->level=player->level;
    return copy_of_player;
}

void* playerCopyToVoid(void* player) {
    return (void*) playerCopy((Player)player);
}

void playerFree(Player player)
{
    free(player);
}

void playerFreeVoid(void* player) {
    playerFree((Player)player);
}


Player* getSortedPlayers(Map players)
{
    Player* sorted_players = malloc(sizeof(Player)*mapGetSize(players));
    if (sorted_players == NULL) {
        return NULL;
    }
    int size = 0;
    MAP_FOREACH(int*, player_key, players)
    {
        insertPlayer(sorted_players, mapGet(players, player_key), size);
        size++;
        freeInt(player_key);
    }
    return sorted_players;
}


void insertPlayer(Player* sorted_players, Player player, int size) {
    for (int i=0; i<size; ++i) {
        if (playerCompareLevels(sorted_players[i], player) > 0) {
            for (int j=size; j>i; j--) {
                sorted_players[j] = sorted_players[j-1];
            }
            sorted_players[i] = player;
            return;
        }
    }
    sorted_players[size] = player;
}

int playerCompareLevels(Player player1, Player player2) {
    double level1 = player1->level;
    double level2 = player2->level;
    if (level1 != level2)
    {
        return level1-level2;
    }
    return player2->id - player1->id;
}
