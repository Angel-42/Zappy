/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GUI main command handling
*/

#include "gui_commands.h"
#include <unistd.h>
#include <sys/socket.h>

char *parse_gui_command(game_state_t *game_state, char *cmd_copy)
{
    char *newline = NULL;
    char *token = NULL;
    char *args = NULL;
    command_handler_t handler = NULL;
    bool needs_args = false;

    newline = strchr(cmd_copy, '\n');
    if (newline)
        *newline = '\0';
    token = strtok(cmd_copy, " ");
    if (!token)
        return NULL;
    handler = find_command_handler(token, &needs_args);
    if (!handler)
        return NULL;
    if (needs_args) {
        args = strtok(NULL, "");
        return handler(game_state, args);
    }
    return handler(game_state, NULL);
}

bool handle_gui_command(game_state_t *game_state, int client_socket,
    const char *command)
{
    char *cmd_copy = NULL;
    char *response = NULL;
    ssize_t sent = 0;

    if (!game_state || !command)
        return false;
    cmd_copy = strdup(command);
    if (!cmd_copy)
        return false;
    response = parse_gui_command(game_state, cmd_copy);
    free(cmd_copy);
    if (!response)
        response = strdup("suc\n");
    sent = send(client_socket, response, strlen(response), 0);
    free(response);
    return sent >= 0;
}
