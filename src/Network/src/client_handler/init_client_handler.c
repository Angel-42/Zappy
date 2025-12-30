/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Client handler implementation
*/

#include "client_handler.h"
#include "game_state.h"
#include "commands.h"
#include "data.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>

static bool allocate_map_tiles(game_state_t *game_state, int width, int height)
{
    game_state->map->width = width;
    game_state->map->height = height;
    game_state->map->tiles = calloc(width * height, sizeof(tile_t));
    if (!game_state->map->tiles) {
        fprintf(stderr, "Failed to allocate map tiles\n");
        return false;
    }
    return true;
}

static void initialize_tile(tile_t *tile, int index, int width)
{
    tile->x = index % width;
    tile->y = index / width;
    tile->player_count = 0;
    tile->players_on_tile = NULL;
    tile->egg_count = 0;
    tile->eggs_on_tile = NULL;
    memset(tile->resources, 0, sizeof(int) * RESOURCE_COUNT);
}

static void initialize_all_tiles(game_state_t *game_state, int width,
    int height)
{
    int i;

    for (i = 0; i < width * height; i++) {
        initialize_tile(&game_state->map->tiles[i], i, width);
    }
}

static void finalize_game_initialization(game_state_t *game_state, int width,
    int height, int frequency)
{
    initialize_all_tiles(game_state, width, height);
    spawn_resources(game_state);
    start_game(game_state);
    printf("Client handler initialized with map %dx%d, frequency %d\n",
        width, height, frequency);
}

bool init_client_handler(client_handler_context_t *context, server_data_t *dta)
{
    int frequency = dta->frequency;
    int width = dta->width;
    int height = dta->height;

    if (!context)
        return false;
    context->game_state = create_game_state(width, height, frequency);
    if (!context->game_state) {
        fprintf(stderr, "Failed to create game state\n");
        return false;
    }
    if (!allocate_map_tiles(context->game_state, width, height)) {
        destroy_game_state(context->game_state);
        context->game_state = NULL;
        return false;
    }
    finalize_game_initialization(context->game_state, width, height,
        frequency);
    return true;
}
