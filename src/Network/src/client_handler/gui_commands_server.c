/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GUI server command handling
*/

#include "gui_commands.h"

char *handle_sgt_command(game_state_t *game_state)
{
    char sgt_response[32];

    snprintf(sgt_response, sizeof(sgt_response), "sgt %d\n",
            game_state->frequency);
    return strdup(sgt_response);
}

char *handle_sst_command(game_state_t *game_state, char *freq_str)
{
    int new_freq = atoi(freq_str);
    char *response = NULL;
    char freq_response[32];

    if (new_freq <= 0)
        return NULL;
    game_state->frequency = new_freq;
    response = strdup("sgt ");
    snprintf(freq_response, sizeof(freq_response), "%d\n", new_freq);
    strcat(response, freq_response);
    return response;
}
