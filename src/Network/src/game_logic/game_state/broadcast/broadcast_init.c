/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Broadcast initialization system implementation
*/

#include "broadcast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "game_state.h"
#include "map.h"

static bool send_map_size_to_client(broadcast_system_t *broadcast_system,
    game_state_t *game_state, int socket)
{
    char message[64];

    if (!broadcast_system || !game_state || !game_state->map || socket < 0)
        return false;
    snprintf(message, sizeof(message), "msz %d %d\n",
        game_state->map->width, game_state->map->height);
    return send_to_gui_client(broadcast_system, game_state, socket, message);
}

static bool send_tile_to_client(broadcast_context_t *context, int x, int y)
{
    tile_t *tile;
    char message[256];

    if (!context || !context->game_state || !context->game_state->map ||
        context->socket < 0)
        return false;
    tile = get_tile(context->game_state->map, x, y);
    if (!tile)
        return false;
    snprintf(message, sizeof(message), "bct %d %d %d %d %d %d %d %d %d\n",
        tile->x, tile->y, tile->resources[FOOD], tile->resources[LINEMATE],
        tile->resources[DERAUMERE], tile->resources[SIBUR],
        tile->resources[MENDIANE], tile->resources[PHIRAS],
        tile->resources[THYSTAME]);
    return send_to_gui_client(context->broadcast_system, context->game_state,
        context->socket, message);
}

static bool send_tiles_row_to_client(broadcast_context_t *context, int y)
{
    for (int x = 0; x < context->game_state->map->width; x++) {
        if (!send_tile_to_client(context, x, y))
            return false;
    }
    return true;
}

static bool send_all_tiles_to_client(broadcast_system_t *broadcast_system,
    game_state_t *game_state, int socket)
{
    broadcast_context_t context = {broadcast_system, game_state, socket};

    if (!broadcast_system || !game_state || !game_state->map || socket < 0)
        return false;
    for (int y = 0; y < game_state->map->height; y++) {
        if (!send_tiles_row_to_client(&context, y))
            return false;
    }
    return true;
}

static bool send_team_names_to_client(broadcast_system_t *broadcast_system,
    game_state_t *game_state, int socket)
{
    char message[512];
    int pos = 0;

    if (!broadcast_system || !game_state || socket < 0)
        return false;
    pos += snprintf(message + pos, sizeof(message) - pos, "tna");
    for (int i = 0; i < game_state->team_count; i++) {
        if (game_state->teams[i]) {
            pos += snprintf(message + pos, sizeof(message) - pos, " %s",
                game_state->teams[i]->name);
        }
    }
    pos += snprintf(message + pos, sizeof(message) - pos, "\n");
    return send_to_gui_client(broadcast_system, game_state, socket, message);
}

static bool send_all_players_to_client_bis(broadcast_system_t
    *broadcast_system, game_state_t *game_state, int socket, int i)
{
    char message[256];

    if (game_state->players[i] && game_state->players[i]->is_alive) {
        snprintf(message, sizeof(message), "pnw %d %d %d %d %d %s\n",
            game_state->players[i]->id, game_state->players[i]->x,
            game_state->players[i]->y, game_state->players[i]->direction,
            game_state->players[i]->level, game_state->players[i]->team_name);
        if (!send_to_gui_client(broadcast_system, game_state, socket, message))
            return false;
    }
    return true;
}

static bool send_all_players_to_client(broadcast_system_t *broadcast_system,
    game_state_t *game_state, int socket)
{
    for (int i = 0; i < game_state->player_count; i++) {
        if (!send_all_players_to_client_bis(broadcast_system,
            game_state, socket, i))
            return false;
    }
    return true;
}

static bool send_time_unit_to_client(broadcast_system_t *broadcast_system,
    game_state_t *game_state, int socket)
{
    char message[64];

    if (!broadcast_system || !game_state || socket < 0)
        return false;
    snprintf(message, sizeof(message), "sgt %d\n", game_state->tick_rate);
    return send_to_gui_client(broadcast_system, game_state, socket, message);
}

bool send_initial_game_state(broadcast_system_t *broadcast_system,
    game_state_t *game_state, int socket)
{
    if (!broadcast_system || !game_state || socket < 0)
        return false;
    if (!send_map_size_to_client(broadcast_system, game_state, socket))
        return false;
    if (!send_all_tiles_to_client(broadcast_system, game_state, socket))
        return false;
    if (!send_team_names_to_client(broadcast_system, game_state, socket))
        return false;
    if (!send_all_players_to_client(broadcast_system, game_state, socket))
        return false;
    if (!send_time_unit_to_client(broadcast_system, game_state, socket))
        return false;
    return true;
}
