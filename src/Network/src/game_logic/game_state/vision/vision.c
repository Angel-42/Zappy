/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Vision system for players
*/

#include "vision.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "game_state.h"
#include "map.h"

static char *get_resource_name(resource_type_t type)
{
    switch (type) {
        case FOOD:
            return "food";
        case LINEMATE:
            return "linemate";
        case DERAUMERE:
            return "deraumere";
        case SIBUR:
            return "sibur";
        case MENDIANE:
            return "mendiane";
        case PHIRAS:
            return "phiras";
        case THYSTAME:
            return "thystame";
        default:
            return "unknown";
    }
}

static void append_tile_content(char *vision, tile_t *tile)
{
    char temp[64];

    if (!tile)
        return;
    strcat(vision, "player");
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        if (tile->resources[i] > 0) {
            snprintf(temp, sizeof(temp), " %s", get_resource_name(i));
            strcat(vision, temp);
        }
    }
}

char *get_vision_string(game_state_t *game_state, player_t *player)
{
    tile_t *current_tile;
    char *vision;

    if (!game_state || !player)
        return NULL;
    vision = malloc(4096);
    if (!vision)
        return NULL;
    strcpy(vision, "[");
    current_tile = get_tile(game_state->map, player->x, player->y);
    append_tile_content(vision, current_tile);
    strcat(vision, "]");
    return vision;
}
