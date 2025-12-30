/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Movement command handlers
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "movement.h"
#include "game_state.h"
#include "player.h"
#include "broadcast.h"

// Movement command handlers
void get_new_position(game_state_t *game_state, player_t *player,
    int *new_x, int *new_y)
{
    *new_x = player->x;
    *new_y = player->y;
    switch (player->direction) {
        case NORTH:
            *new_y = (*new_y - 1 + game_state->map->height) %
                game_state->map->height;
            break;
        case EAST:
            *new_x = (*new_x + 1) % game_state->map->width;
            break;
        case SOUTH:
            *new_y = (*new_y + 1) % game_state->map->height;
            break;
        case WEST:
            *new_x = (*new_x - 1 + game_state->map->width) %
                game_state->map->width;
            break;
    }
}

char *handle_forward(game_state_t *game_state, player_t *player)
{
    int new_x = 0;
    int new_y = 0;

    if (!player || !game_state)
        return strdup("ko\n");
    get_new_position(game_state, player, &new_x, &new_y);
    if (move_player(game_state, player->id, new_x, new_y))
        return strdup("ok\n");
    return strdup("ko\n");
}

char *handle_right(game_state_t *game_state, player_t *player)
{
    direction_t old_direction;

    if (!player || !game_state)
        return strdup("ko\n");
    old_direction = player->direction;
    player->direction = (player->direction + 1) % 4;
    if (old_direction != player->direction) {
        broadcast_player_position(&game_state->broadcast_system,
            game_state, player);
    }
    return strdup("ok\n");
}

char *handle_left(game_state_t *game_state, player_t *player)
{
    direction_t old_direction;

    if (!player || !game_state)
        return strdup("ko\n");
    old_direction = player->direction;
    player->direction = (player->direction - 1 + 4) % 4;
    if (old_direction != player->direction) {
        broadcast_player_position(&game_state->broadcast_system,
            game_state, player);
    }
    return strdup("ok\n");
}

// Static helper functions
static bool is_valid_movement_command(const char *command)
{
    const char *movement_commands[] = {"Forward", "Right", "Left"};

    if (!command)
        return false;
    for (int i = 0; i < 3; i++) {
        if (strcmp(command, movement_commands[i]) == 0) {
            return true;
        }
    }
    return false;
}

static int get_movement_time(const char *command, int frequency)
{
    if (!command)
        return 0;
    if (strcmp(command, "Forward") == 0 ||
        strcmp(command, "Right") == 0 ||
        strcmp(command, "Left") == 0) {
        return 7 / frequency;
    }
    return 0;
}
