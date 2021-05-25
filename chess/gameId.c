
struct game_id_t {
    int id1;
    int id2;
}

GameId createGameId(int id1, int id2) {
    GameId gameId = malloc(sizeof(*gameId));
    if (gameId == NULL) { return NULL; }
    gameId->id1 = id1;
    gameId->id2 = id2;
    return gameId;
}


GameId copyGameId(GameId gameId) {
    if (gameId == NULL) {
        return NULL;
    }
    GameId gameIdCopy = createGameId(gameId->id1, gameId->id2);
    if (gameIdCopy == NULL) {
        return NULL;
    }
    return gameIdCopy;
}

GameId freeGameId(GameId gameId) {
    free(gameId);     
}

int cmpGameId(GameId gmid1, GameId gmid2) {
    if ((gmid1->id1 == gmid2->id1 && gmid1->id2 == gmid2->id2) ||
        (gmid1->id1 == gmid2->id2 && gmid1->id2 == gmid2->id1))
        return 0;
    return 1;
}

void changeGameId(GameId gameId, int newId1, int newId2) {
    assert(gameId != NULL);
    gameId->id1 = newId1;
    gameId->id2 = newId2;
}
