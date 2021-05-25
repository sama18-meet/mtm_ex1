#include <stdlib.h>
#include "game.h"
#include "gameId.h"



struct Game_t {
    GameId id;
    int player1_id;
    int player2_id;
    Winner winner;
    int time;
};

GameId gameGetId(Game game) {
    return game->id;
}
int gameGetPlayer1Id(Game game) {
    return game->player1_id;
}
int gameGetPlayer2Id(Game game) {
    return game->player2_id;
}
Winner gameGetWinner(Game game) {
    return game->winner;
}
int gameGetTime(Game game) {
    return game->time;
}

void gameSetPlayer1Id(Game game, int id) {
    game->player1_id = id;
}
void gameSetPlayer2Id(Game game, int id) {
    game->player2_id = id;
}
void gameSetWinner(Game game, Winner winner) {
    game->winner = winner;
}

Game gameCreate(int player1_id, int player2_id, Winner winner, int time) {
    Game game = malloc(sizeof(*game));
    if (game == NULL) {
        return NULL;
    }
    GameId game_id = gameIdCreate(player1_id, player2_id);
    if (game_id == NULL) {
        free(game);
        return NULL;
    }
    game->id = game_id;
    game->player1_id = player1_id;
    game->player2_id = player2_id;
    game->winner = winner;
    game->time = time;
    return game;
}


Game gameCopy(Game game) {
    Game game_copy = gameCreate(game->player1_id, game->player2_id, game->winner, game->time);
    if (game_copy == NULL) {
        return NULL;
    }
    return game_copy;
}

void* gameCopyToVoid(void* game) {
    return (void*) gameCopy((Game)game);
}

void gameFree(Game game) {
    if (game == NULL) {
        return;
    }
    gameIdFree(game->id);
    free(game);
}

void gameFreeVoid(void* game) {
    free((game)game);
}


