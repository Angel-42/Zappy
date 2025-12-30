/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Player management system
*/

#include "player.h"
#include <stdlib.h>
#include <string.h>
#include "game_state.h"
#include "map.h"
#include "broadcast.h"
#include <sys/socket.h>
#include <stdio.h>

static void send_death_message_to_player(player_t *player)
{
    const char *death_msg;
    ssize_t sent;

    if (player && player->socket > 0) {
        death_msg = "dead\n";
        sent = send(player->socket, death_msg, strlen(death_msg),
            MSG_NOSIGNAL);
        if (sent > 0) {
            printf("Sent death message to player %d (socket %d)\n", player->id,
                player->socket);
        } else {
            printf("Failed to send death message to player %d (socket %d)\n",
                player->id, player->socket);
        }
    }
}

static void handle_player_food_consumption(player_t *player)
{
    const char *dead_msg;

    if (player->life_units > 252) {
        return;
    }
    if (player->food > 0) {
        player->food--;
        player->life_units += 126;
    } else {
        player->is_alive = false;
        if (player->socket >= 0) {
            dead_msg = "dead\n";
            send(player->socket, dead_msg, strlen(dead_msg), 0);
            printf("Sent 'dead' message to player %d (socket %d)\n",
                player->id, player->socket);
        }
    }
}

static void broadcast_death_if_just_died(game_state_t *game_state,
    player_t *player, bool was_alive)
{
    if (was_alive && !player->is_alive)
        broadcast_player_death(&game_state->broadcast_system, game_state,
            player);
}

bool update_player(game_state_t *game_state, int player_id)
{
    player_t *player;
    bool was_alive;

    if (!game_state)
        return false;
    player = get_player(game_state, player_id);
    if (!player || !player->is_alive)
        return false;
    was_alive = player->is_alive;
    player->life_units--;
    handle_player_food_consumption(player);
    broadcast_death_if_just_died(game_state, player, was_alive);
    return true;
}
