/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Event broadcast system implementation
*/

#include "broadcast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "game_state.h"

static void format_player_ids(char *message, size_t size, int *player_ids,
    int player_count)
{
    int pos = 0;

    pos += snprintf(message + pos, size - pos, "pic %d %d %d",
        player_ids[0], player_ids[1], player_ids[2]);
    for (int i = 3; i < player_count; i++) {
        pos += snprintf(message + pos, size - pos, " %d", player_ids[i]);
    }
    pos += snprintf(message + pos, size - pos, "\n");
}

bool broadcast_incantation_start(broadcast_incantation_t *incantation)
{
    char message[256];

    if (!incantation || !incantation->broadcast_system ||
        !incantation->game_state || !incantation->player_ids ||
        incantation->player_count < 1)
        return false;
    format_player_ids(message, sizeof(message), incantation->player_ids,
        incantation->player_count);
    return broadcast_to_gui(incantation->broadcast_system,
        incantation->game_state, message);
}

bool broadcast_incantation_end(broadcast_incantation_t *incantation)
{
    char message[64];

    if (!incantation || !incantation->broadcast_system ||
        !incantation->game_state)
        return false;
    snprintf(message, sizeof(message), "pie %d %d %d\n", incantation->x,
        incantation->y, incantation->result);
    return broadcast_to_gui(incantation->broadcast_system,
        incantation->game_state, message);
}

static const char *get_resource_name(int resource_id)
{
    switch (resource_id) {
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

bool broadcast_resource_drop(broadcast_system_t *broadcast_system,
    game_state_t *game_state, player_t *player, int resource_id)
{
    char message[128];

    if (!broadcast_system || !game_state || !player)
        return false;
    snprintf(message, sizeof(message), "pdr %d %d\n",
        player->id, resource_id);
    return broadcast_to_gui(broadcast_system, game_state, message);
}

bool broadcast_resource_collect(broadcast_system_t *broadcast_system,
    game_state_t *game_state, player_t *player, int resource_id)
{
    char message[128];

    if (!broadcast_system || !game_state || !player)
        return false;
    snprintf(message, sizeof(message), "pgt %d %d\n",
        player->id, resource_id);
    return broadcast_to_gui(broadcast_system, game_state, message);
}

bool broadcast_egg_laid(broadcast_system_t *broadcast_system,
    game_state_t *game_state, egg_t *egg)
{
    char message[64];

    if (!broadcast_system || !game_state || !egg)
        return false;
    snprintf(message, sizeof(message), "enw %d %d %d %d\n",
        egg->id, egg->player_id, egg->x, egg->y);
    return broadcast_to_gui(broadcast_system, game_state, message);
}
