/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Player broadcast system implementation
*/

#include "broadcast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "game_state.h"
#include "player.h"

static void format_inventory_message(char *message, size_t size,
    player_t *player)
{
    snprintf(message, size, "pin %d %d %d %d %d %d %d %d %d %d\n",
        player->id, player->x, player->y,
        player->inventory[FOOD], player->inventory[LINEMATE],
        player->inventory[DERAUMERE], player->inventory[SIBUR],
        player->inventory[MENDIANE], player->inventory[PHIRAS],
        player->inventory[THYSTAME]);
}

bool broadcast_player_inventory(broadcast_system_t *broadcast_system,
    game_state_t *game_state, player_t *player)
{
    char message[128];

    if (!broadcast_system || !game_state || !player)
        return false;
    format_inventory_message(message, sizeof(message), player);
    return broadcast_to_gui(broadcast_system, game_state, message);
}

bool broadcast_player_death(broadcast_system_t *broadcast_system,
    game_state_t *game_state, player_t *player)
{
    char message[32];

    if (!broadcast_system || !game_state || !player)
        return false;
    snprintf(message, sizeof(message), "pdi %d\n", player->id);
    return broadcast_to_gui(broadcast_system, game_state, message);
}

bool broadcast_player_expulsion(broadcast_system_t *broadcast_system,
    game_state_t *game_state, player_t *player)
{
    char message[32];

    if (!broadcast_system || !game_state || !player)
        return false;
    snprintf(message, sizeof(message), "pex %d\n", player->id);
    return broadcast_to_gui(broadcast_system, game_state, message);
}

bool broadcast_player_broadcast(broadcast_system_t *broadcast_system,
    game_state_t *game_state, player_t *player, const char *message)
{
    char broadcast_msg[512];

    if (!broadcast_system || !game_state || !player || !message)
        return false;
    snprintf(broadcast_msg, sizeof(broadcast_msg), "pbc %d %s\n",
        player->id, message);
    return broadcast_to_gui(broadcast_system, game_state, broadcast_msg);
}
