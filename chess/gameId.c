
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
    if (gameId == NULL) {return NULL; }
    GameId gameIdCpy = malloc(sizeof(*gameIdCpy));
    if (gameIdCpy == NULL) { return NULL; }
    gameIdCpy->id1 = gameId->id1;
    gameIdCpy->id2 = gameId->id2;
    return gameIdCpy;
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

void changeGameId(GameId gmid, int id1, int id2) {
    gmid->id1 = id1;
    gmid->id2 = id2;
}
