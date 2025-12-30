/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Time management system
*/

#include "time.h"
#include <stdlib.h>
#include <string.h>
#include "game_state.h"
#include "player.h"

bool is_game_paused(const game_state_t *game_state)
{
    return game_state && game_state->is_paused;
}

int get_current_tick(const game_state_t *game_state)
{
    return game_state ? game_state->current_tick : 0;
}

double get_frequency(const game_state_t *game_state)
{
    return game_state ? (double) game_state->tick_rate : 1.0;
}
