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

char *prepare_command_copy(const char *command)
{
    char *cmd_copy;
    char *newline;

    cmd_copy = strdup(command);
    if (!cmd_copy)
        return NULL;
    newline = strchr(cmd_copy, '\n');
    if (newline)
        *newline = '\0';
    return cmd_copy;
}

static bool check_command_validity(const char *token,
    const char **valid_commands)
{
    int i;

    for (i = 0; i < 12; i++) {
        if (strcmp(token, valid_commands[i]) == 0)
            return true;
    }
    return false;
}

bool is_valid_command(const char *command)
{
    const char *valid_commands[] = {
        "Forward", "Right", "Left", "Look", "Inventory", "Broadcast",
        "Connect_nbr", "Fork", "Eject", "Take", "Set", "Incantation"
    };
    char *cmd_copy;
    char *token;

    if (!command)
        return false;
    cmd_copy = prepare_command_copy(command);
    if (!cmd_copy)
        return false;
    token = strtok(cmd_copy, " ");
    if (!token) {
        free(cmd_copy);
        return false;
    }
    free(cmd_copy);
    return check_command_validity(token, valid_commands);
}
