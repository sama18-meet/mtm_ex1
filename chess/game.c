#include "game.h"
struct Game_t {
    GameId id;
    int player1_id;
    int player2_id;
    Winner winner;
    int time;
};

Game gameCreate(int player1_id, int player2_id, Winner winner, int time) {
    Game game = malloc(sizeof(*game));
    if (game == NULL) {
        return NULL;
    }
    GameId game_id = gameIdCreate(player1_id, palyer2_id);
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
    Game game_copy = gameCreate(game->player1, game->player2, game->winner, game->time);
    if (game_copy == NULL) {
        return NULL;
    }
    return game_copy;
}


void gameFree(Game game) {
    if (game == NULL) {
        return;
    }
    gameIdFree(game->id);
    free(game);
}
