/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Map broadcast system implementation
*/

#include "broadcast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "game_state.h"
#include "map.h"

bool broadcast_map_size(broadcast_system_t *broadcast_system,
    game_state_t *game_state)
{
    char message[64];

    if (!broadcast_system || !game_state || !game_state->map)
        return false;
    snprintf(message, sizeof(message), "msz %d %d\n",
        game_state->map->width, game_state->map->height);
    return broadcast_to_gui(broadcast_system, game_state, message);
}

static void format_tile_resources(char *message, size_t size, tile_t *tile)
{
    snprintf(message, size, "bct %d %d %d %d %d %d %d %d %d\n",
        tile->x, tile->y, tile->resources[FOOD], tile->resources[LINEMATE],
        tile->resources[DERAUMERE], tile->resources[SIBUR],
        tile->resources[MENDIANE], tile->resources[PHIRAS],
        tile->resources[THYSTAME]);
}

bool broadcast_tile_content(broadcast_system_t *broadcast_system,
    game_state_t *game_state, int x, int y)
{
    tile_t *tile;
    char message[256];

    if (!broadcast_system || !game_state || !game_state->map)
        return false;
    tile = get_tile(game_state->map, x, y);
    if (!tile)
        return false;
    format_tile_resources(message, sizeof(message), tile);
    return broadcast_to_gui(broadcast_system, game_state, message);
}

static bool broadcast_all_tiles_bis(broadcast_system_t *broadcast_system,
    game_state_t *game_state, int y)
{
    for (int x = 0; x < game_state->map->width; x++) {
        if (!broadcast_tile_content(broadcast_system, game_state, x, y))
            return false;
    }
    return true;
}

bool broadcast_all_tiles(broadcast_system_t *broadcast_system,
    game_state_t *game_state)
{
    if (!broadcast_system || !game_state || !game_state->map)
        return false;
    for (int y = 0; y < game_state->map->height; y++) {
        if (!broadcast_all_tiles_bis(broadcast_system, game_state, y))
            return false;
    }
    return true;
}

static void format_team_names_message(char *message, size_t size,
    game_state_t *game_state)
{
    int pos = 0;

    pos += snprintf(message + pos, size - pos, "tna");
    for (int i = 0; i < game_state->team_count; i++) {
        if (game_state->teams[i]) {
            pos += snprintf(message + pos, size - pos, " %s",
                game_state->teams[i]->name);
        }
    }
    pos += snprintf(message + pos, size - pos, "\n");
}

bool broadcast_team_names(broadcast_system_t *broadcast_system,
    game_state_t *game_state)
{
    char message[512];

    if (!broadcast_system || !game_state)
        return false;
    format_team_names_message(message, sizeof(message), game_state);
    return broadcast_to_gui(broadcast_system, game_state, message);
}
