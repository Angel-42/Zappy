/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Player broadcast system implementation
*/

#include "broadcast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "game_state.h"
#include "player.h"

static void send_broadcast_to_teammate(player_t *sender, player_t *receiver,
    int direction, const char *message)
{
    char broadcast_msg[512];

    snprintf(broadcast_msg, sizeof(broadcast_msg), "message %d,%s\n",
        direction, message);
    send(receiver->socket, broadcast_msg, strlen(broadcast_msg), 0);
}

static bool is_valid_teammate(player_t *sender, player_t *player)
{
    return (player && player != sender && player->is_alive &&
        strcmp(player->team_name, sender->team_name) == 0);
}

void broadcast_message(broadcast_system_t *broadcast_system,
    game_state_t *game_state, player_t *sender, const char *message)
{
    player_t *player;
    int direction;

    if (!broadcast_system || !game_state || !sender || !message)
        return;
    for (int i = 0; i < game_state->player_count; i++) {
        player = game_state->players[i];
        if (is_valid_teammate(sender, player)) {
            direction = calculate_broadcast_direction(game_state,
                sender, player);
            send_broadcast_to_teammate(sender, player, direction, message);
        }
    }
    broadcast_player_broadcast(broadcast_system, game_state, sender, message);
}

static int handle_map_wrapping(int value, int map_size)
{
    if (value > map_size / 2)
        value -= map_size;
    if (value < (-map_size) / 2)
        value += map_size;
    return value;
}

static int calculate_base_direction(int dx, int dy)
{
    if (dx == 0 && dy == 0)
        return 0;
    if (dx > 0 && dy == 0)
        return 1;
    if (dx > 0 && dy > 0)
        return 2;
    if (dx == 0 && dy > 0)
        return 3;
    if (dx < 0 && dy > 0)
        return 4;
    if (dx < 0 && dy == 0)
        return 5;
    if (dx < 0 && dy < 0)
        return 6;
    if (dx == 0 && dy < 0)
        return 7;
    if (dx > 0 && dy < 0)
        return 8;
    return 0;
}

int calculate_broadcast_direction(game_state_t *game_state,
    player_t *sender, player_t *receiver)
{
    int dx;
    int dy;
    int base_direction;
    int relative_direction;

    if (!game_state || !sender || !receiver)
        return 0;
    dx = receiver->x - sender->x;
    dy = receiver->y - sender->y;
    dx = handle_map_wrapping(dx, game_state->map->width);
    dy = handle_map_wrapping(dy, game_state->map->height);
    base_direction = calculate_base_direction(dx, dy);
    relative_direction = (base_direction - sender->direction * 2) % 8;
    if (relative_direction <= 0)
        relative_direction += 8;
    return relative_direction;
}

bool broadcast_player_new(broadcast_system_t *broadcast_system,
    game_state_t *game_state, player_t *player)
{
    char message[128];

    if (!broadcast_system || !game_state || !player)
        return false;
    snprintf(message, sizeof(message), "pnw %d %d %d %d %d %s\n",
        player->id, player->x, player->y, player->direction,
        player->level, player->team_name);
    return broadcast_to_gui(broadcast_system, game_state, message);
}

bool broadcast_player_position(broadcast_system_t *broadcast_system,
    game_state_t *game_state, player_t *player)
{
    char message[64];

    if (!broadcast_system || !game_state || !player)
        return false;
    snprintf(message, sizeof(message), "ppo %d %d %d %d\n",
        player->id, player->x, player->y, player->direction);
    printf("[DEBUG] sending to GUI: %s", message);
    return broadcast_to_gui(broadcast_system, game_state, message);
}

bool broadcast_player_level(broadcast_system_t *broadcast_system,
    game_state_t *game_state, player_t *player)
{
    char message[32];

    if (!broadcast_system || !game_state || !player)
        return false;
    snprintf(message, sizeof(message), "plv %d %d\n",
        player->id, player->level);
    return broadcast_to_gui(broadcast_system, game_state, message);
}
