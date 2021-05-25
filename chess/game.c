struct Game {
    GameId id;
    int player1_id;
    int player2_id;
    Winner winner;
    int time;
} *Game;

Game copyGame(Game game) {
    Game gameCpy = createGame(game->player1, game->player2, game->winner, game->time);
    if (gameCpy == NULL) {
        return NULL;
    }
    return gameCpy;
}

Game createGame(int player1, int player2, Winner winner, int time) {
    Game game = malloc(sizeof(*game));
    if (game == NULL) { return NULL; }
    GameId gmid = createGameId(player1, palyer2);
    if (gmid == NULL) {
        free(game);
        return NULL;
    }
    game->id = gmid;
    game->player1_id = player1;
    game->player2_id = player2_id;
    game->winner = winner;
    game->time = time;
    return game;
}


void freeGame(Game game) {
    if (game == NULL) { return; }
    freeGameId(game->id);
    free(game);
}
