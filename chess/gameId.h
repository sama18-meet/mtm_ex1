
typedef struct game_id_t *GameId;
GameId createGameId(int id1, int id2);
GameId copyGameId(GameId gameId);
GameId freeGameId(GameId gameId);
int cmpGameId(GameId gmid1, GameId gmid2);
void changeGameId(GameId gmid, int id1, int id2);
