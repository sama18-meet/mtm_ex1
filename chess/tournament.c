struct Tour {
    int id;
    const char* location;
    Winner winner;
    int max_games_per_player;
    bool active;
    Map games;
    Map playerInTour; // key is player id, data is playerInTour, only active players are included
    int num_players; // including deleted players
} *Tour;

void freeTour(Tour tour)
{
    mapDestroy(tour->games);
    mapDestroy(tour->playerInTour);
    free(tour);
}

Tour copyTour(Tour tour)
{
    Tour CopyOfTour=malloc(sizeof(*CopyOfTour));
    if(CopyOfTour==NULL) return NULL;
    CopyOfTour->id=tour->id;
    CopyOfTour->location=tour->location;
    CopyOfTour->max_games_per_player=tour->max_games_per_player;
    CopyOfTour->active=tour->active;
    CopyOfTour->games=mapCopy(tour->games);
    CopyOfTour->playerInTour=mapCopy(tour->playerInTour);
    CopyOfTour->num_players=tour->num_players;
 return CopyOfTour;
}

Tour CreateTour(int tournament_id, int max_games_per_player, const char* tournament_location)
{
    Tour tour= malloc(sizeof(*tour));
    if(tour==NULL) return NULL; 
    tour->id= tournament_id;
    tour->location= tournament_location;
    tour->winner=0; //if NULL?? 
    tour->max_games_per_player=max_games_per_player;
    tour->active=false; 
    tour->games=mapCreate(copyGame,
                        copyInt,
                        freeGame,
                        freeInt,
                        cmpGameId);
    tour->playerInTour=mapCreate(copyPlayerInTour,
                        copyInt,
                        freePlayerInTour,
                        freeInt,
                        compareInts);
    tour->num_players = 0;
    return tour;
}

PlayerInTour max_player(PlayerInTour player1, PlayerInTour player2) {
    if (player1->points != player2->points) 
        return (player1->points > player2->points) ? player1 : player2;
    if (player1->num_losses != player2->num_losses)
        return (player1->num_losses < player2->num_losses) ? player1 : player2;
    if (player1->num_wins != player2->num_wins)
        return (player1->num_wins > player2->num_wins) ? player1 : player2;
    return (player1->id < player2->id) : player1 : player2;
}

PlayerInTour get_winner(Tour tour) {
    assert(tour != NULL);
    assert(tour->playerInTour != NULL);
    Player winner = mapGetFirst(tour->playerInTour);
    MAP_FOREACH(int, curr_player, tour->playerInTour) {
        update_points(curr_player);
        winner = max_player(winner, curr_player); 
    }
    tour.winner = winner;
    return winner;
}

bool player_exceeded_games(Tour tour, player_id) {
    assert(tour!=NULL);
    if (!mapContains(tour->playerInTour, player_id)
        return false;
    PlayerInTour player = mapGet(tour->playerInTour, player_id);
    assert(player!=NULL);
    if (player->max_games_per_player == tour->max_games_per_player)
        return true;
    return false;
}


void removePlayerFromTour(Tour tour, int player_id) {
    if (tour.active) {
        MAP_FOREACH(Game, g, tour.games) {
            if (g.player1_id == player_id) {
                g.winner = SECOND_PLAYER;
                g.player1_id = 0;
                changeGameId(g->id, 0, g->player2_id);

            }
            else if (g.player2_id == player_id) {
                g.winner = FIRST_PLAYER;
                g.player2_id = 0;
                changeGameId(g->id, g->player1_id, 0);
            }
    }
    mapRemove(tour->playerInTour, player_id);
}

char* get_winner_id(Tour tour) {
    return int_to_str(winner_id);
}
char* get_longest_game_time(Tour tour) {
    int longes_game_time = -1;
    MAP_FOREACH(Game, game, tour->games) {
        if (game->time > longest_game_time)
            longest_game_time = game->time;
    }
    return int_to_str(longest_game_time)
}
char* get_avg_game_time(Tour tour) {
    int times_sum = 0;
    int num_games = 0
    MAP_FOREACH(Game, game, tour->games) {
        times_sum += game->time;
        num_games++;
    }
    assert(mapGetSize(tour->games) == num_games);
    return double_to_str(times_sum/num_games, 2);
}
char* get_location(Tour tour) {
    return tour->location;
}
char* get_num_of_games(Tour tour) {
    return int_to_str(mapGetSize(tour->games);
}
char* get_num_players(Tour tour) {
    return int_to_str(tour->num_players);
}


