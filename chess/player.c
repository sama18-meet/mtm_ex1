struct Player {
    int id;
    int num_losses;
    int num_wins;
    int num_draws;
    int playtime;
    int level;
} *Player




int calc_level(Player player)
{
   return (6*(player->num_wins))-(10*(player->num_losses))+(2*(player->num_draws))/((player->num_draws)+(player->num_losses)+(player->num_wins));   
}

Player copyPlayer(Player player)
{
    Player CopyOfplayer=malloc(sizeof(*CopyOfplayer));
    if(CopyOfplayer=NULL) return NULL;
    CopyOfplayer->id=playerInTour->id;
    CopyOfplayer->num_losses=player->num_losses;
    CopyOfplayer->num_wins=player->num_wins;
    CopyOfplayer->num_draws=player->num_draws;
    CopyOfplayer->playtime=player->playtime;
    CopyOfplayer->level=player->level;
    return copyPlayer;
}

void freePlayer(Player player)
{
    free(player);
}


Player get_sorted_players(Player players) {
    Player sorted_players = malloc(sizeof(sorted_player)*mapGetSize(chess->players));
    if (sorted_players == NULL) {
        return NULL;
    }
    int size = 0;
    MAP_FOREACH(Player, p, players) {
        insert_player(sorted_players, p, size)
        size++;
    }
    return sorted_players;
}


void insert_player(Player sorted_players, Player player, int size) {
    for (int i=0; i<size; ++i) {
        if (player_compare_levels(sorted_players[i], players) > 0) {
            for (int j=size; j>i; j--) {
                sorted_players[j] = sorted_players[j-1];
            }
            sorted_players[i] = player;
            return;
        }
    }
    sorted_players[size] = player;
}

int player_compare_levels(Player player1, Player player2) {
    int level1 = player1->level;
    int level2 = player2->level;
    if (level1 != level2) {
        return level1-level2;
    return player2->id - player1->id;
}
