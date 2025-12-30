/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GUI team command handling
*/

#include "gui_commands.h"

char *handle_tna_command(game_state_t *game_state)
{
    char *response = malloc(1024);
    char team_info[256];

    if (!response)
        return NULL;
    strcpy(response, "");
    for (int i = 0; i < game_state->team_count; i++) {
        snprintf(team_info, sizeof(team_info), "tna %s\n",
                game_state->teams[i]->name);
        strcat(response, team_info);
    }
    return response;
}
