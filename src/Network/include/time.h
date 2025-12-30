/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Time management system header
*/

#ifndef GAME_STATE_TIME_H_
    #define GAME_STATE_TIME_H_

    #include "game_state.h"

int get_action_time(const char *action, int frequency);
bool is_action_ready(const game_state_t *game_state, player_t *player);
bool is_game_running(const game_state_t *game_state);
bool is_game_paused(const game_state_t *game_state);
int get_current_tick(const game_state_t *game_state);
double get_frequency(const game_state_t *game_state);

#endif // GAME_STATE_TIME_H_
