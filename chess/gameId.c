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


GameId gameIdCopy(GameId gameId) {
    if (gameId == NULL) {
        return NULL;
    }
    GameId gameIdCopy = gameIdCreate(gameId->id1, gameId->id2);
    if (gameIdCopy == NULL) {
        return NULL;
    }
    return gameIdCopy;
}

GameId gameIdFree(GameId gameId) {
    free(gameId);     
}

int gameIdCmp(GameId gameId1, GameId gameId2) {
    if ((gameId1->id1 == gameId2->id1 && gameId1->id2 == gameId2->id2) ||
        (gameId1->id1 == gameId2->id2 && gameId1->id2 == gameId2->id1))
        return 0;
    return 1;
}

void gameIdChange(GameId gameId, int newId1, int newId2) {
    assert(gameId != NULL);
    gameId->id1 = newId1;
    gameId->id2 = newId2;
}

