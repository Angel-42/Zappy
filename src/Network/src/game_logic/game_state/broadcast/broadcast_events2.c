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

bool broadcast_egg_death(broadcast_system_t *broadcast_system,
    game_state_t *game_state, egg_t *egg)
{
    char message[32];

    if (!broadcast_system || !game_state || !egg)
        return false;
    snprintf(message, sizeof(message), "edi %d\n", egg->id);
    return broadcast_to_gui(broadcast_system, game_state, message);
}

bool broadcast_time_unit(broadcast_system_t *broadcast_system,
    game_state_t *game_state)
{
    char message[32];

    if (!broadcast_system || !game_state)
        return false;
    snprintf(message, sizeof(message), "sgt %d\n", game_state->tick_rate);
    return broadcast_to_gui(broadcast_system, game_state, message);
}

bool broadcast_game_end(broadcast_system_t *broadcast_system,
    game_state_t *game_state, const char *winning_team)
{
    char message[128];

    if (!broadcast_system || !game_state || !winning_team)
        return false;
    snprintf(message, sizeof(message), "seg %s\n", winning_team);
    return broadcast_to_gui(broadcast_system, game_state, message);
}

bool broadcast_server_message(broadcast_system_t *broadcast_system,
    game_state_t *game_state, const char *message)
{
    char broadcast_msg[512];

    if (!broadcast_system || !game_state || !message)
        return false;
    snprintf(broadcast_msg, sizeof(broadcast_msg), "smg %s\n", message);
    return broadcast_to_gui(broadcast_system, game_state, broadcast_msg);
}

bool broadcast_egg_hatching(broadcast_system_t *broadcast_system,
    game_state_t *game_state, egg_t *egg)
{
    char message[32];

    if (!broadcast_system || !game_state || !egg)
        return false;
    snprintf(message, sizeof(message), "eht %d\n", egg->id);
    return broadcast_to_gui(broadcast_system, game_state, message);
}
