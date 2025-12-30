/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Information command handlers
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "info.h"
#include "game_state.h"

// Information command handlers
char *handle_look(game_state_t *game_state, player_t *player)
{
    char *vision = NULL;
    char *result = NULL;

    if (!player || !game_state)
        return strdup("ko\n");
    vision = get_vision_string(game_state, player);
    if (!vision)
        return strdup("ko\n");
    result = malloc(strlen(vision) + 2);
    if (!result) {
        free(vision);
        return strdup("ko\n");
    }
    strcpy(result, vision);
    strcat(result, "\n");
    free(vision);
    return result;
}

//NOTE: POSSIBLE BUG HERE DUE TO DOUBLE STRING IN SNPRINTF
char *handle_inventory(game_state_t *game_state, player_t *player)
{
    char inventory[1024];

    if (!player)
        return strdup("ko\n");
    snprintf(inventory, sizeof(inventory),
        "[food %d, linemate %d, deraumere %d, sibur %d, "
        "mendiane %d, phiras %d, thystame %d]\n",
        player->inventory[FOOD],
        player->inventory[LINEMATE],
        player->inventory[DERAUMERE],
        player->inventory[SIBUR],
        player->inventory[MENDIANE],
        player->inventory[PHIRAS],
        player->inventory[THYSTAME]);
    return strdup(inventory);
}

char *handle_connect_nbr(game_state_t *game_state, player_t *player)
{
    int available_slots = 0;
    char response[64];

    if (!player || !game_state)
        return strdup("ko\n");
    available_slots = get_available_slots(game_state, player->team_name);
    snprintf(response, sizeof(response), "%d\n", available_slots);
    return strdup(response);
}

// Static helper functions
static bool is_valid_info_command(const char *command)
{
    const char *info_commands[] = {"Look", "Inventory", "Connect_nbr"};

    if (!command)
        return false;
    for (int i = 0; i < 3; i++) {
        if (strcmp(command, info_commands[i]) == 0) {
            return true;
        }
    }
    return false;
}

static int get_info_time(const char *command, int frequency)
{
    if (!command)
        return 0;
    if (strcmp(command, "Look") == 0 || strcmp(command,
        "Inventory") == 0) {
        return 7 / frequency;
    } else if (strcmp(command, "Connect_nbr") == 0) {
        return 0;
    }
    return 0;
}
