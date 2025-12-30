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

static void cleanup_teams(game_state_t *state)
{
    if (!state->teams)
        return;
    for (int i = 0; i < state->team_count; i++) {
        destroy_team(state->teams[i]);
    }
    free(state->teams);
}

static void cleanup_players(game_state_t *state)
{
    if (!state->players)
        return;
    for (int i = 0; i < state->player_count; i++) {
        if (state->players[i]) {
            destroy_player(state->players[i]);
        }
    }
    free(state->players);
}

static void cleanup_eggs(game_state_t *state)
{
    if (!state->eggs)
        return;
    for (int i = 0; i < state->egg_count; i++) {
        if (state->eggs[i]) {
            destroy_egg(state->eggs[i]);
        }
    }
    free(state->eggs);
}

static void cleanup_incantations(game_state_t *state)
{
    if (!state->incantations)
        return;
    for (int i = 0; i < state->incantation_count; i++) {
        if (state->incantations[i]) {
            destroy_incantation(state->incantations[i]);
        }
    }
    free(state->incantations);
}

void destroy_game_state(game_state_t *state)
{
    if (!state)
        return;
    cleanup_broadcast_system(&state->broadcast_system, state);
    cleanup_teams(state);
    cleanup_players(state);
    cleanup_eggs(state);
    cleanup_incantations(state);
    if (state->map) {
        destroy_map(state->map);
    }
    free(state);
}
