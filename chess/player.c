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


