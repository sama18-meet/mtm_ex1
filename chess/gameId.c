#include <stdlib.h>
#include <assert.h>
#include "gameId.h"

struct GameId_t {
    int id1;
    int id2;
};

GameId gameIdCreate(int id1, int id2) {
    GameId gameId = malloc(sizeof(*gameId));
    if (gameId == NULL) { return NULL; }
    gameId->id1 = id1;
    gameId->id2 = id2;
    return gameId;
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
    return (void*)gameIdCopy((GameId) game_id);
}

void gameIdFree(GameId gameId) {
    free(gameId);     
}

void gameIdFreeVoid(void* game_id) {
    gameIdFree((GameId)game_id);     
}

int gameIdCmp(GameId gameId1, GameId gameId2) {
    if ((gameId1->id1 == gameId2->id1 && gameId1->id2 == gameId2->id2) ||
        (gameId1->id1 == gameId2->id2 && gameId1->id2 == gameId2->id1))
        return 0;
    return 1;
}

int gameIdCmpVoid(void* gameId1, void* gameId2) {
    return gameIdCmp((GameId) gameId1, (GameId) gameId2);
}

void gameIdChange(GameId gameId, int newId1, int newId2) {
    assert(gameId != NULL);
    gameId->id1 = newId1;
    gameId->id2 = newId2;
}


