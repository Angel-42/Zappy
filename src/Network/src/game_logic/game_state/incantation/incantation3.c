/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Incantation system implementation
*/

#include "incantation.h"
#include <stdlib.h>
#include <string.h>
#include "game_state.h"
#include "map.h"
#include "player.h"
#include "broadcast.h"

static void add_same_level_players_to_incantation(game_state_t *game_state,
    incantation_t *incantation, tile_t *tile, int level)
{
    for (int i = 0; i < tile->player_count; i++) {
        if (tile->players_on_tile[i] &&
            tile->players_on_tile[i]->level == level) {
            add_player_to_incantation(incantation, tile->players_on_tile[i]);
            tile->players_on_tile[i]->is_incantating = true;
        }
    }
}

static bool allocate_incantation_memory(game_state_t *game_state)
{
    incantation_t **new_incantations = realloc(game_state->incantations,
        (game_state->incantation_count + 1) * sizeof(incantation_t *));

    if (!new_incantations)
        return false;
    game_state->incantations = new_incantations;
    return true;
}

void setup_broadcast_data(broadcast_incantation_t *broadcast_data,
    game_state_t *game_state, incantation_t *incantation, int *player_ids)
{
    broadcast_data->broadcast_system = &game_state->broadcast_system;
    broadcast_data->game_state = game_state;
    broadcast_data->x = incantation->x;
    broadcast_data->y = incantation->y;
    broadcast_data->level = incantation->level;
    broadcast_data->player_ids = player_ids;
    broadcast_data->player_count = incantation->player_count;
}

static bool broadcast_incantation_start_event(game_state_t *game_state,
    incantation_t *incantation)
{
    int *player_ids = malloc(incantation->player_count * sizeof(int));
    broadcast_incantation_t broadcast_data;

    if (!player_ids)
        return true;
    for (int i = 0; i < incantation->player_count; i++) {
        player_ids[i] = incantation->players[i]->id;
    }
    setup_broadcast_data(&broadcast_data, game_state, incantation, player_ids);
    broadcast_incantation_start(&broadcast_data);
    free(player_ids);
    return true;
}

bool start_incantation(game_state_t *game_state, player_t *player)
{
    incantation_t *incantation;
    tile_t *tile;

    if (!game_state || !player || !can_elevate(game_state, player))
        return false;
    incantation = create_incantation(game_state, player->x, player->y,
        player->level);
    if (!incantation)
        return false;
    tile = get_tile(game_state->map, player->x, player->y);
    if (tile)
        add_same_level_players_to_incantation(game_state, incantation, tile,
            player->level);
    if (!allocate_incantation_memory(game_state)) {
        destroy_incantation(incantation);
        return false;
    }
    game_state->incantations[game_state->incantation_count] = incantation;
    game_state->incantation_count++;
    return broadcast_incantation_start_event(game_state, incantation);
}
