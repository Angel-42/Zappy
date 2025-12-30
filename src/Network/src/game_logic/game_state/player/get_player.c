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

player_t *get_player(game_state_t *game_state, int player_id)
{
    if (!game_state || !game_state->players)
        return NULL;
    for (int i = 0; i < game_state->player_count; i++) {
        if (game_state->players[i] &&
            game_state->players[i]->id == player_id)
            return game_state->players[i];
    }
    return NULL;
}

player_t *get_player_by_socket(game_state_t *game_state, int socket)
{
    if (!game_state || !game_state->players)
        return NULL;
    for (int i = 0; i < game_state->player_count; i++) {
        if (game_state->players[i] &&
            game_state->players[i]->socket == socket)
            return game_state->players[i];
    }
    return NULL;
}
