/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GUI map command handling
*/

#include "gui_commands.h"

char *handle_msz_command(game_state_t *game_state)
{
    char msz_response[64];

    snprintf(msz_response, sizeof(msz_response), "msz %d %d\n",
            game_state->map->width, game_state->map->height);
    return strdup(msz_response);
}

void append_tile_info(char *response, tile_t *tile, int x, int y)
{
    char tile_info[256];

    snprintf(tile_info, sizeof(tile_info),
            "bct %d %d %d %d %d %d %d %d %d\n",
            x, y,
            tile->resources[FOOD],
            tile->resources[LINEMATE],
            tile->resources[DERAUMERE],
            tile->resources[SIBUR],
            tile->resources[MENDIANE],
            tile->resources[PHIRAS],
            tile->resources[THYSTAME]);
    strcat(response, tile_info);
}

static void process_row_tiles(char *response, map_t *map, int y)
{
    tile_t *tile = NULL;

    for (int x = 0; x < map->width; x++) {
        tile = get_tile(map, x, y);
        if (tile) {
            append_tile_info(response, tile, x, y);
        }
    }
}

char *handle_mct_command(game_state_t *game_state)
{
    char *response = malloc(4096);

    if (!response)
        return NULL;
    strcpy(response, "");
    for (int y = 0; y < game_state->map->height; y++) {
        process_row_tiles(response, game_state->map, y);
    }
    return response;
}

char *handle_bct_command(game_state_t *game_state,
    char *x_str, char *y_str)
{
    int x = atoi(x_str);
    int y = atoi(y_str);
    tile_t *tile = get_tile(game_state->map, x, y);
    char bct_response[256];

    if (!tile)
        return NULL;
    snprintf(bct_response, sizeof(bct_response),
            "bct %d %d %d %d %d %d %d %d %d\n",
            x, y,
            tile->resources[FOOD],
            tile->resources[LINEMATE],
            tile->resources[DERAUMERE],
            tile->resources[SIBUR],
            tile->resources[MENDIANE],
            tile->resources[PHIRAS],
            tile->resources[THYSTAME]);
    return strdup(bct_response);
}
