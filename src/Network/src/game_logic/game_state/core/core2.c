/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Core game state management implementation
*/

#include <stdlib.h>
#include <string.h>
#include "core.h"
#include "game_state.h"
#include "map.h"
#include "player.h"
#include "team.h"
#include "egg.h"
#include "incantation.h"
#include "broadcast.h"

static game_state_t *initialize_state_fields(game_state_t *state,
    int tick_rate)
{
    if (!state)
        return NULL;
    state->map = NULL;
    state->teams = NULL;
    state->team_count = 0;
    state->players = NULL;
    state->player_count = 0;
    state->eggs = NULL;
    state->egg_count = 0;
    state->incantations = NULL;
    state->incantation_count = 0;
    state->is_running = false;
    state->is_paused = false;
    state->tick_rate = tick_rate;
    state->frequency = tick_rate;
    state->current_tick = 0;
    state->next_player_id = 1;
    state->next_egg_id = 1;
    state->next_incantation_id = 1;
    return state;
}

game_state_t *set_state(int tick_rate)
{
    game_state_t *state = malloc(sizeof(game_state_t));

    if (!state)
        return NULL;
    return initialize_state_fields(state, tick_rate);
}

static bool setup_game_components(game_state_t *state, int width, int height)
{
    if (!init_map(state)) {
        free(state);
        return false;
    }
    if (!set_map_dimensions(state, width, height)) {
        destroy_map(state->map);
        free(state);
        return false;
    }
    if (!init_broadcast_system(&state->broadcast_system, state)) {
        destroy_map(state->map);
        free(state);
        return false;
    }
    return true;
}

game_state_t *create_game_state(int width, int height, int tick_rate)
{
    game_state_t *state = set_state(tick_rate);

    if (state == NULL)
        return NULL;
    if (!setup_game_components(state, width, height))
        return NULL;
    return state;
}
