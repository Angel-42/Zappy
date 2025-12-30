/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Player management system
*/

#include "player.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "game_state.h"
#include "map.h"
#include "broadcast.h"

static void initialize_player_stats(player_t *player, game_state_t *game_state)
{
    player->id = game_state->next_player_id;
    game_state->next_player_id++;
    player->x = rand() % game_state->map->width;
    player->y = rand() % game_state->map->height;
    player->direction = rand() % 4;
    player->level = 1;
    player->food = 10;
    player->life_units = 1260;
    player->is_alive = true;
    player->is_incantating = false;
    player->action_end_time = 0;
    player->last_food_time = game_state->current_tick;
    memset(player->inventory, 0, sizeof(int) * RESOURCE_COUNT);
}

static void place_player_on_map(game_state_t *game_state, player_t *player)
{
    tile_t *tile;

    tile = get_tile(game_state->map, player->x, player->y);
    if (tile)
        add_player_to_tile(tile, player);
}

player_t *create_player(game_state_t *game_state, const char *team_name,
    int socket)
{
    player_t *player;

    printf("=== CREATING PLAYER ===\n");
    printf("Team name: [%s], Socket: %d\n", team_name, socket);
    if (!game_state || !team_name)
        return NULL;
    player = malloc(sizeof(player_t));
    if (!player)
        return NULL;
    player->team_name = strdup(team_name);
    player->socket = socket;
    initialize_player_stats(player, game_state);
    printf("Player created: ID=%d, Team=[%s], Socket=%d, Pos=(%d,%d)\n",
        player->id, player->team_name, player->socket, player->x, player->y);
    printf("=== END CREATING PLAYER ===\n");
    place_player_on_map(game_state, player);
    broadcast_player_new(&game_state->broadcast_system, game_state, player);
    return player;
}
