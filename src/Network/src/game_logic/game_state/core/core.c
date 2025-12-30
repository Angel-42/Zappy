/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Core game state management implementation
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "core.h"
#include "game_state.h"
#include "map.h"
#include "player.h"
#include "team.h"
#include "egg.h"
#include "incantation.h"
#include "broadcast.h"

bool start_game(game_state_t *game_state)
{
    if (!game_state)
        return false;
    game_state->is_running = true;
    game_state->is_paused = false;
    return true;
}

bool pause_game(game_state_t *game_state)
{
    if (!game_state)
        return false;
    game_state->is_paused = true;
    return true;
}

bool reset_game(game_state_t *game_state)
{
    if (!game_state)
        return false;
    game_state->is_running = false;
    game_state->is_paused = false;
    game_state->current_tick = 0;
    return true;
}

static void update_players(game_state_t *game_state)
{
    printf("=== UPDATING ALL PLAYERS (tick: %d) ===\n",
        game_state->current_tick);
    printf("Total players: %d\n", game_state->player_count);
    for (int i = 0; i < game_state->player_count; i++) {
        if (game_state->players[i]) {
            printf("Updt player %d (team: %s, alive: %s, life_units: %d)\n",
                game_state->players[i]->id,
                game_state->players[i]->team_name,
                game_state->players[i]->is_alive ? "yes" : "no",
                game_state->players[i]->life_units);
            update_player(game_state, game_state->players[i]->id);
        }
    }
    for (int i = game_state->player_count - 1; i >= 0; i--) {
        if (game_state->players[i] && !game_state->players[i]->is_alive) {
            printf("Cleaning up dead player %d\n", game_state->players[i]->id);
            remove_player(game_state, game_state->players[i]->id);
        }
    }
    printf("=== END PLAYER UPDATE ===\n");
}

static void update_eggs(game_state_t *game_state)
{
    for (int i = 0; i < game_state->egg_count; i++) {
        if (!game_state->eggs[i] || game_state->eggs[i]->is_hatched)
            continue;
        if (game_state->current_tick >= game_state->eggs[i]->hatch_time) {
            hatch_egg(game_state, game_state->eggs[i]->id);
        }
    }
}

static void update_incantations(game_state_t *game_state)
{
    int incantation_duration;

    for (int i = 0; i < game_state->incantation_count; i++) {
        if (!game_state->incantations[i] ||
            !game_state->incantations[i]->is_active)
            continue;
        incantation_duration = 300 / game_state->frequency;
        if (game_state->current_tick >=
            game_state->incantations[i]->start_time +
                incantation_duration) {
            end_incantation(game_state, game_state->incantations[i]);
        }
    }
}

bool update_game_state(game_state_t *game_state)
{
    if (!game_state || !game_state->is_running || game_state->is_paused)
        return false;
    game_state->current_tick++;
    update_players(game_state);
    update_eggs(game_state);
    update_incantations(game_state);
    update_map(game_state);
    return true;
}

bool process_game_tick(game_state_t *game_state)
{
    return update_game_state(game_state);
}
