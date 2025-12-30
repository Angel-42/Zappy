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

bool init_map(game_state_t *game_state)
{
    if (!game_state)
        return false;
    game_state->map = malloc(sizeof(map_t));
    if (!game_state->map)
        return false;
    game_state->map->width = 10;
    game_state->map->height = 10;
    game_state->map->tiles = NULL;
    return true;
}

static void initialize_tile(tile_t *tile, int x, int y)
{
    tile->x = x;
    tile->y = y;
    tile->player_count = 0;
    tile->egg_count = 0;
    tile->players_on_tile = NULL;
    tile->eggs_on_tile = NULL;
    memset(tile->resources, 0, sizeof(int) * RESOURCE_COUNT);
}

bool set_map_dimensions(game_state_t *game_state, int width, int height)
{
    if (!game_state || !game_state->map || width <= 0 || height <= 0)
        return false;
    game_state->map->width = width;
    game_state->map->height = height;
    game_state->map->tiles = calloc(width * height, sizeof(tile_t));
    if (!game_state->map->tiles)
        return false;
    for (int i = 0; i < width * height; i++) {
        initialize_tile(&game_state->map->tiles[i], i % width, i / width);
    }
    spawn_resources(game_state);
    return true;
}

bool update_map(game_state_t *game_state)
{
    if (!game_state || !game_state->map)
        return false;
    if (game_state->current_tick % 20 == 0) {
        spawn_resources(game_state);
    }
    return true;
}

static void cleanup_tile_arrays(tile_t *tile)
{
    if (tile->players_on_tile) {
        free(tile->players_on_tile);
    }
    if (tile->eggs_on_tile) {
        free(tile->eggs_on_tile);
    }
}

void destroy_map(map_t *map)
{
    if (!map)
        return;
    if (map->tiles) {
        for (int i = 0; i < map->width * map->height; i++) {
            cleanup_tile_arrays(&map->tiles[i]);
        }
        free(map->tiles);
    }
    free(map);
}
