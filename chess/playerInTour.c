#include <stdio.h>
#include <stdlib.h>
#include "playerInTour.h"
#define POINTS_PER_WIN 2
#define POINTS_PER_LOSS 0
#define POINTS_PER_DDRAW 1

struct PlayerInTour_t {
    int id;
    int num_losses;
    int num_wins;
    int num_draws;
    int time;
    int points;
};

PlayerInTour createPlayerInTour(int player_id) {
    PlayerInTour player_in_tour = malloc(sizeof(*player_in_tour));
    if (player_in_tour == NULL) {
        return NULL;
    }
    player_in_tour->id = player_id;
    player_in_tour->num_losses = 0;
    player_in_tour->num_wins = 0;
    player_in_tour->num_draws = 0;
    player_in_tour->time=0;
    player_in_tour->points = 0;
    return player_in_tour;
}

void playerInTourFree(PlayerInTour player_in_tour) {
    free(player_in_tour);
}

PlayerInTour playerInTourCopy(PlayerInTour player_in_tour) {
    PlayerInTour player_in_tour_copy=malloc(sizeof(*player_in_tour_copy));
    if(player_in_tour_copy==NULL) {
        return NULL;
    }
    player_in_tour_copy->id=player_in_tour->id;
    player_in_tour_copy->num_losses=player_in_tour->num_losses;
    player_in_tour_copy->num_wins=player_in_tour->num_wins;
    player_in_tour_copy->num_draws=player_in_tour->num_draws;
    player_in_tour_copy->time=player_in_tour->time;
    player_in_tour_copy->points=player_in_tour->points;
    return player_in_tour_copy;
}

void playerInTourUpdatePoints(PlayerInTour player_in_tour) {
    assert(player_in_tour != NULL);
    int sum = player_in_tour->num_wins*POINTS_PER_WIN
              + player_in_tour->num_losses*POINTS_PER_LOSS
              + player_in_tour->num_draws*POINTS_PER_DDRAW;
    player_in_tour->points = sum;
}

