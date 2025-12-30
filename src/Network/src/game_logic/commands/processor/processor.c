/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Main command processor
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "processor.h"
#include "movement.h"
#include "info.h"
#include "resources.h"
#include "actions.h"

static char *handle_movement_commands(game_state_t *game_state,
    player_t *player, const char *token)
{
    if (strcmp(token, "Forward") == 0)
        return handle_forward(game_state, player);
    if (strcmp(token, "Right") == 0)
        return handle_right(game_state, player);
    if (strcmp(token, "Left") == 0)
        return handle_left(game_state, player);
    return NULL;
}

static char *handle_info_commands(game_state_t *game_state, player_t *player,
    const char *token)
{
    if (strcmp(token, "Look") == 0)
        return handle_look(game_state, player);
    if (strcmp(token, "Inventory") == 0)
        return handle_inventory(game_state, player);
    if (strcmp(token, "Connect_nbr") == 0)
        return handle_connect_nbr(game_state, player);
    return NULL;
}

static char *handle_basic_actions(game_state_t *game_state, player_t *player,
    const char *token)
{
    if (strcmp(token, "Fork") == 0)
        return handle_fork(game_state, player);
    if (strcmp(token, "Eject") == 0)
        return handle_eject(game_state, player);
    if (strcmp(token, "Incantation") == 0)
        return handle_incantation(game_state, player);
    return NULL;
}

static char *handle_complex_actions(game_state_t *game_state, player_t *player,
    const char *token)
{
    char *message = NULL;
    char *resource = NULL;

    if (strcmp(token, "Broadcast") == 0) {
        message = strtok(NULL, "");
        return handle_broadcast(game_state, player, message ? message : "");
    }
    if (strcmp(token, "Take") == 0) {
        resource = strtok(NULL, " ");
        return handle_take(game_state, player, resource);
    }
    if (strcmp(token, "Set") == 0) {
        resource = strtok(NULL, " ");
        return handle_set(game_state, player, resource);
    }
    return NULL;
}

static char *handle_action_commands(game_state_t *game_state, player_t *player,
    const char *token)
{
    char *response;

    response = handle_basic_actions(game_state, player, token);
    if (response)
        return response;
    return handle_complex_actions(game_state, player, token);
}

static char *route_command(game_state_t *game_state, player_t *player,
    const char *token, char *cmd_copy)
{
    char *response;

    response = handle_movement_commands(game_state, player, token);
    if (response) {
        free(cmd_copy);
        return response;
    }
    response = handle_info_commands(game_state, player, token);
    if (response) {
        free(cmd_copy);
        return response;
    }
    response = handle_action_commands(game_state, player, token);
    if (response) {
        free(cmd_copy);
        return response;
    }
    return NULL;
}

char *process_command(game_state_t *game_state, player_t *player,
    const char *command)
{
    char *cmd_copy;
    char *token;
    char *response;

    if (!player || !command || !game_state)
        return strdup("ko\n");
    cmd_copy = prepare_command_copy(command);
    if (!cmd_copy)
        return strdup("ko\n");
    token = strtok(cmd_copy, " ");
    if (!token) {
        free(cmd_copy);
        return strdup("ko\n");
    }
    response = route_command(game_state, player, token, cmd_copy);
    if (response)
        return response;
    free(cmd_copy);
    return strdup("ko\n");
}

int get_command_time(const char *command, int frequency)
{
    char *cmd_copy;
    char *token;
    int time;

    if (!command)
        return 0;
    cmd_copy = prepare_command_copy(command);
    if (!cmd_copy)
        return 0;
    token = strtok(cmd_copy, " ");
    if (!token) {
        free(cmd_copy);
        return 0;
    }
    time = get_action_time(token, frequency);
    free(cmd_copy);
    return time;
}
