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
#include <stdio.h>

static void cleanup_player_from_map(game_state_t *game_state, player_t *player)
{
    tile_t *tile;

    tile = get_tile(game_state->map, player->x, player->y);
    if (tile)
        remove_player_from_tile(tile, player);
}

static void broadcast_player_death_if_needed(game_state_t *game_state,
    player_t *player)
{
    if (player->is_alive)
        broadcast_player_death(&game_state->broadcast_system, game_state,
            player);
}

static void shift_players_array(game_state_t *game_state, int start_index)
{
    int j;

    for (j = start_index; j < game_state->player_count - 1; j++) {
        game_state->players[j] = game_state->players[j + 1];
    }
    game_state->player_count--;
}

static void remove_player_from_game_state_array(game_state_t *game_state,
    player_t *player)
{
    int i;

    for (i = 0; i < game_state->player_count; i++) {
        if (game_state->players[i] != player)
            continue;
        shift_players_array(game_state, i);
        printf("Removed player %d from game state array\n", player->id);
        break;
    }
}

void remove_player(game_state_t *game_state, int player_id)
{
    player_t *player;

    if (!game_state)
        return;
    player = get_player(game_state, player_id);
    if (!player)
        return;
    printf("Removing player %d from game\n", player_id);
    broadcast_player_death_if_needed(game_state, player);
    cleanup_player_from_map(game_state, player);
    remove_player_from_game_state_array(game_state, player);
    remove_player_from_team(game_state, player);
    destroy_player(player);
}
