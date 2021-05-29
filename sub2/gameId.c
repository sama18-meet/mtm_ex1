#include <stdlib.h>
#include <assert.h>
#include "gameId.h"

struct GameId_t {
    int id1;
    int id2;
};

GameId gameIdCreate(int id1, int id2) {
    GameId game_id = malloc(sizeof(*game_id));
    if (game_id == NULL) { return NULL; }
    game_id->id1 = id1;
    game_id->id2 = id2;
    return game_id;
}


GameId gameIdCopy(GameId game_id) {
    if (game_id == NULL) {
        return NULL;
    }
    GameId game_id_copy = gameIdCreate(game_id->id1, game_id->id2);
    if (game_id_copy == NULL) {
        return NULL;
    }
    return game_id_copy;
}

void* gameIdCopyToVoid(void* game_id) {
    return (void*)gameIdCopy((GameId)game_id);
}

void gameIdFree(GameId game_id) {
    free(game_id);     
}

void gameIdFreeVoid(void* game_id) {
    gameIdFree((GameId)game_id);     
}

int gameIdCompare(GameId game_id1, GameId game_id2) {
    if ((game_id1->id1 == game_id2->id1 && game_id1->id2 == game_id2->id2) ||
        (game_id1->id1 == game_id2->id2 && game_id1->id2 == game_id2->id1))
        return 0;
    return 1;
}

int gameIdCompareVoid(void* game_id1, void* game_id2) {
    return gameIdCompare((GameId) game_id1, (GameId) game_id2);
}

void gameIdChange(GameId game_id, int new_id1, int new_id2) {
    assert(game_id != NULL);
    game_id->id1 = new_id1;
    game_id->id2 = new_id2;
}


