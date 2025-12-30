/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Core map operations
*/

#include "map.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "game_state.h"

map_t *get_map(game_state_t *game_state)
{
    return game_state ? game_state->map : NULL;
}

tile_t *get_tile(map_t *map, int x, int y)
{
    if (!map || !is_valid_position(map, x, y))
        return NULL;
    return &map->tiles[y * map->width + x];
}

bool is_valid_position(map_t *map, int x, int y)
{
    if (!map)
        return false;
    return x >= 0 && x < map->width && y >= 0 && y < map->height;
}
