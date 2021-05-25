struct Tour {
    int id;
    const char* location;
    int winner_id;
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
    if(tour==NULL) 
    {
        return NULL;
    }

    Tour tourCpy= malloc(sizeof(*tour));
    if(tourCpy==NULL) return NULL; 
    tourCpy->id= tour->id;
    tourCpy->location= tour->location;
    tourCpy->winner_id=tour->winner_id; 
    tourCpy->max_games_per_player=tour->max_games_per_player;
    tourCpy->active=tour->active; 
    tourCpy->num_players = tour->num_players;
    tourCpy->games=mapCopy(tour->games);
    if(tourCpy->games==NULL)
    {
        free(tourCpy);
        return NULL;
    }
    tourCpy->playerInTour=mapCopy(tour->PlayerInTour);
    if(tourCpy->playerInTour==NULL)
    {
        free(tourCpy);
        mapDestroy(tourCpy->games);
        return NULL;
    }
    return tourCpy;
}

Tour CreateTour(int tournament_id, int max_games_per_player, const char* tournament_location)
{
    assert(tournament_location!=NULL);
    Tour tour= malloc(sizeof(*tour));
    if(tour==NULL) return NULL; 
    tour->id= tournament_id;
    tour->location= tournament_location;
    tour->winner_id=-1;
    tour->max_games_per_player=max_games_per_player;
    tour->active=false; 
    tour->num_players = 0;
    tour->games=mapCreate(copyGame,
                        copyGameId,
                        freeGame,
                        freeGameId,
                        cmpGameId);
    if(tour->games==NULL)
    {
        free(tour);
        return NULL;
    }
    tour->playerInTour=mapCreate(copyPlayerInTour,
                        copyInt,
                        freePlayerInTour,
                        freeInt,
                        compareInts);
     if(tour->PlayerInTour==NULL)
    {
        free(tour);
        mapDestroy(tour->games);
        return NULL;
    }
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

void set_winner(Tour tour) {
    assert(tour != NULL);
    assert(tour->playerInTour != NULL);
    PlayerInTour winner = mapGetFirst(tour->playerInTour);
    MAP_FOREACH(int, curr_player, tour->playerInTour) {
        update_points(curr_player);
        winner = max_player(winner, curr_player); 
    }
    tour->winner_id = winner->id;
}

bool player_exceeded_games(Tour tour, int player_id) {
    assert(tour!=NULL);
    if (!mapContains(tour->playerInTour, player_id)
    {
        return false;
    }
    PlayerInTour player = mapGet(tour->playerInTour, player_id);
    assert(player!=NULL);
    int num_games = player->num_wins + player->num_losses + player->num_draws;
    assert(num_games <= tour->max_games_per_player);
    if (num_games == tour->max_games_per_player) {
        return true;
    }
    return false;
}


void removePlayerFromTour(Tour tour, int player_id)
 {
    assert(tour != NULL);
    MAP_FOREACH(Game, g, tour->games)
    {
        if (g->player1_id == player_id) {
            g->player1_id = 0;
            changeGameId(g->id, 0, g->player2_id);
            if (tour->active) {
                g->winner = SECOND_PLAYER;
            }

        }
        else if (g->player2_id == player_id) 
        {
            g->player2_id = 0;
            changeGameId(g->id, g->player1_id, 0);
            if (tour->active) {
                g->winner = FIRST_PLAYER;
            }
        }
    }
    mapRemove(tour->playerInTour, player_id);
}

char* get_winner_id(Tour tour) {
    return int_to_str(tour->winner_id);
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




