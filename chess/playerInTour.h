#ifndef _PLAYERINTOUR_H
#define _PLAYERINTOUR_H

typedef struct PlayerInTour_t *PlayerInTour;

void freePlayerInTour(PlayerInTour player);

PlayerInTour copyPlayerInTour(PalyerInTour playerInTour)

// if player doesn't already exist in tour, it adds it
PlayerInTour getPlayerInTour(Tour tour, int player_id) {    
    PlayerInTour player_in_tour = mapGet(tour->playerInTour, player_id);
    if (player_in_tour == NULL) {
        player_in_tour = createPlayerInTour(player_id);
        if (player_in_tour == NULL) { return NULL; }
        MapResult res = mapPut(tour->playerInTour, player_id, player_in_tour);
        assert(res != MAP_NULL_ARGUMENT);
        if (res == MAP_OUT_OF_MEMORY) { return NULL; }
        tour->num_players++;
    }
    return player_in_tour;
}

PlayerInTour createPlayerInTour(int player_id) {
    PlayerInTour player = malloc(sizeof(*player));
    if (player == NULL) { return NULL; }
    player->id = player_id;
    player->num_losses = 0;
    player->num_wins = 0;
    player->num_draws = 0;
    player->points = 0;
    return player;
}


void update_points(PlayerInTour player) {
    assert(player != NULL);
    int sum = player->num_wins*POINTS_PER_WIN
              + player->num_losses*POINTS_PER_LOSS
              + player->num_draws*POINTS_PER_DDRAW;
    player->points = sum;
}


