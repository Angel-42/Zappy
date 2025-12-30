/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Egg management implementation
*/

#include "egg.h"
#include <stdlib.h>
#include <string.h>
#include "game_state.h"
#include "map.h"
#include "team.h"
#include "broadcast.h"

egg_t *get_egg(game_state_t *game_state, int egg_id)
{
    if (!game_state || !game_state->eggs)
        return NULL;
    for (int i = 0; i < game_state->egg_count; i++) {
        if (game_state->eggs[i] && game_state->eggs[i]->id == egg_id) {
            return game_state->eggs[i];
        }
    }
    return NULL;
}

static bool allocate_egg_memory(game_state_t *game_state)
{
    egg_t **new_eggs = realloc(game_state->eggs,
        (game_state->egg_count + 1) * sizeof(egg_t *));

    if (!new_eggs)
        return false;
    game_state->eggs = new_eggs;
    return true;
}

static egg_t *create_egg(game_state_t *game_state, player_t *parent)
{
    egg_t *egg = malloc(sizeof(egg_t));

    if (!egg)
        return NULL;
    egg->id = (game_state->next_egg_id + 1);
    egg->player_id = parent->id;
    egg->x = parent->x;
    egg->y = parent->y;
    egg->is_hatched = false;
    egg->team_name = strdup(parent->team_name);
    egg->hatch_time = game_state->current_tick + 600 / game_state->frequency;
    return egg;
}

static void setup_egg_in_game(game_state_t *game_state, egg_t *egg,
    player_t *parent)
{
    tile_t *tile;
    team_t *team;

    game_state->eggs[game_state->egg_count] = egg;
    game_state->egg_count++;
    tile = get_tile(game_state->map, egg->x, egg->y);
    if (tile) {
        add_egg_to_tile(tile, egg);
    }
    team = get_team(game_state, parent->team_name);
    if (team) {
        team->eggs_count++;
    }
}

bool add_egg(game_state_t *game_state, player_t *parent)
{
    egg_t *egg;

    if (!game_state || !parent)
        return false;
    if (!allocate_egg_memory(game_state))
        return false;
    egg = create_egg(game_state, parent);
    if (!egg)
        return false;
    setup_egg_in_game(game_state, egg, parent);
    broadcast_egg_laid(&game_state->broadcast_system, game_state, egg);
    return true;
}

static void remove_egg_from_tile_and_team(game_state_t *game_state,
    egg_t *egg)
{
    tile_t *tile;
    team_t *team;

    tile = get_tile(game_state->map, egg->x, egg->y);
    if (tile) {
        remove_egg_from_tile(tile, egg);
    }
    team = get_team(game_state, egg->team_name);
    if (team) {
        team->eggs_count--;
    }
}

static void shift_eggs_array(game_state_t *game_state, int removed_index)
{
    for (int j = removed_index; j < game_state->egg_count - 1; j++) {
        game_state->eggs[j] = game_state->eggs[j + 1];
    }
    game_state->egg_count--;
}

bool remove_egg(game_state_t *game_state, int egg_id)
{
    egg_t *egg;

    if (!game_state || !game_state->eggs)
        return false;
    for (int i = 0; i < game_state->egg_count; i++) {
        if (game_state->eggs[i] && game_state->eggs[i]->id == egg_id) {
            egg = game_state->eggs[i];
            broadcast_egg_death(&game_state->broadcast_system, game_state,
                egg);
            remove_egg_from_tile_and_team(game_state, egg);
            destroy_egg(egg);
            shift_eggs_array(game_state, i);
            return true;
        }
    }
    return false;
}

bool hatch_egg(game_state_t *game_state, int egg_id)
{
    egg_t *egg;

    if (!game_state)
        return false;
    egg = get_egg(game_state, egg_id);
    if (!egg || egg->is_hatched)
        return false;
    egg->is_hatched = true;
    broadcast_egg_hatching(&game_state->broadcast_system, game_state, egg);
    return true;
}

void destroy_egg(egg_t *egg)
{
    if (!egg)
        return;
    free(egg->team_name);
    free(egg);
}
