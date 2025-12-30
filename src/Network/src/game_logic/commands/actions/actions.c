/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Action command handlers
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "actions.h"
#include "game_state.h"

// Action command handlers
char *handle_broadcast(game_state_t *game_state, player_t *player,
    const char *message)
{
    if (!player || !game_state || !message)
        return strdup("ko\n");
    broadcast_message(&game_state->broadcast_system,
        game_state, player, message);
    return strdup("ok\n");
}

char *handle_fork(game_state_t *game_state, player_t *player)
{
    if (!player || !game_state)
        return strdup("ko\n");
    if (!add_egg(game_state, player)) {
        return strdup("ko\n");
    }
    return strdup("ok\n");
}

char *handle_eject(game_state_t *game_state, player_t *player)
{
    if (!player || !game_state)
        return strdup("ko\n");
    if (eject_players(game_state, player)) {
        return strdup("ok\n");
    } else {
        return strdup("ko\n");
    }
}

char *handle_incantation(game_state_t *game_state, player_t *player)
{
    char response[64];

    if (!player || !game_state)
        return strdup("ko\n");
    if (start_incantation(game_state, player)) {
        snprintf(response, sizeof(response),
            "Elevation underway\nCurrent level: %d\n", player->level);
        return strdup(response);
    } else {
        return strdup("ko\n");
    }
}

// Static helper functions
static bool is_valid_action_command(const char *command)
{
    const char *action_commands[] = {"Broadcast", "Fork",
        "Eject", "Incantation"};

    if (!command)
        return false;
    for (int i = 0; i < 4; i++) {
        if (strcmp(command, action_commands[i]) == 0) {
            return true;
        }
    }
    return false;
}
