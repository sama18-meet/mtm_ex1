struct Game {
    GameId id;
    int player1_id;
    int player2_id;
    Winner winner;
    int time;
} *Game;

Game copyGame(Game game) {
    Game gameCpy = malloc(sizeof(*gameCpy));
    if (gameCpy == NULL) { return NULL; }
    GameId gmidCpy = copyGameId(game->gameId);
    if (gmidCpy == NULL) {
        free(gameCpy);
        return NULL;
    }
    gameCpy->id = gmidCpy;
    gameCpy->player1_id = game->player1_id;
    gameCpy->player2_id = game->player2_id;
    gameCpy->winner = game->winner;
    gameCpy->time = game->time;
    return gameCpy;
}


void freeGame(Game game) {
    if (game == NULL) { return; }
    freeGameId(game->id);
    free(game);
}

Game createGame(GameId id, int player1, int player2, Winner winner, int time) {
    Game game = malloc(sizeof(*game));
    if (game == NULL) { return NULL; }
    game->id = id;
    game->player1_id = player1;
    game->player2_id = player2;
    game->winner = winner;
    game->time = time;
    return game;
}


