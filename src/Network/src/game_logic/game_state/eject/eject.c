/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Player ejection system implementation
*/

#include "eject.h"
#include <stdlib.h>
#include <string.h>
#include "game_state.h"
#include "map.h"
#include "player.h"

static void calculate_ejection_position(game_state_t *game_state,
    player_t *ejector, int *new_x, int *new_y)
{
    *new_x = ejector->x;
    *new_y = ejector->y;
    switch (ejector->direction) {
        case NORTH:
            *new_y = (*new_y - 1 + game_state->map->height) %
                game_state->map->height;
            break;
        case EAST:
            *new_x = (*new_x + 1) % game_state->map->width;
            break;
        case SOUTH:
            *new_y = (*new_y + 1) % game_state->map->height;
            break;
        case WEST:
            *new_x = (*new_x - 1 + game_state->map->width) %
                game_state->map->width;
            break;
    }
}

static bool eject_single_player(game_state_t *game_state, player_t *ejector,
    player_t *target, tile_t *current_tile)
{
    int new_x;
    int new_y;
    tile_t *new_tile;

    calculate_ejection_position(game_state, ejector, &new_x, &new_y);
    remove_player_from_tile(current_tile, target);
    target->x = new_x;
    target->y = new_y;
    new_tile = get_tile(game_state->map, new_x, new_y);
    if (new_tile) {
        add_player_to_tile(new_tile, target);
    }
    return true;
}

bool eject_players(game_state_t *game_state, player_t *ejector)
{
    tile_t *tile;
    bool ejected;

    if (!game_state || !ejector)
        return false;
    tile = get_tile(game_state->map, ejector->x, ejector->y);
    if (!tile)
        return false;
    ejected = false;
    for (int i = 0; i < tile->player_count; i++) {
        if (tile->players_on_tile[i] && tile->players_on_tile[i] != ejector) {
            eject_single_player(game_state, ejector, tile->players_on_tile[i],
                tile);
            ejected = true;
        }
    }
    return ejected;
}
