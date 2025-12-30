/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Incantation system header
*/

#ifndef INCANTATION_H_
    #define INCANTATION_H_

    #include "game_state.h"

incantation_t *create_incantation(game_state_t *game_state,
    int x, int y, int level);
void destroy_incantation(incantation_t *incantation);
bool add_player_to_incantation(incantation_t *incantation,
    player_t *player);
bool remove_player_from_incantation(incantation_t *incantation,
    player_t *player);
bool start_incantation(game_state_t *game_state, player_t *player);
bool end_incantation(game_state_t *game_state, incantation_t *incantation);
bool can_elevate(game_state_t *game_state, player_t *player);
elevation_requirements_t get_elevation_requirements(int level);
void setup_broadcast_data(broadcast_incantation_t *broadcast_data,
    game_state_t *game_state, incantation_t *incantation, int *player_ids);

#endif /* INCANTATION_H_ */
