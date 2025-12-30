/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Time management system
*/

#include "time.h"
#include <stdlib.h>
#include <string.h>
#include "game_state.h"
#include "player.h"

static bool is_basic_action(const char *action)
{
    return (strcmp(action, "Forward") == 0 || strcmp(action, "Right") == 0 ||
            strcmp(action, "Left") == 0 || strcmp(action, "Look") == 0 ||
            strcmp(action, "Inventory") == 0 ||
            strcmp(action, "Broadcast") == 0 ||
            strcmp(action, "Eject") == 0 || strcmp(action, "Take") == 0 ||
            strcmp(action, "Set") == 0);
}

int get_action_time(const char *action, int frequency)
{
    if (!action)
        return 0;
    if (is_basic_action(action))
        return 7 / frequency;
    if (strcmp(action, "Connect_nbr") == 0)
        return 0;
    if (strcmp(action, "Fork") == 0)
        return 42 / frequency;
    if (strcmp(action, "Incantation") == 0)
        return 300 / frequency;
    return 0;
}

bool is_action_ready(const game_state_t *game_state, player_t *player)
{
    if (!player || !game_state)
        return false;
    return player->action_end_time <= game_state->current_tick;
}

bool is_game_running(const game_state_t *game_state)
{
    return game_state && game_state->is_running;
}
