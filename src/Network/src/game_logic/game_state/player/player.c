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

bool process_player_action(game_state_t *game_state, int player_id,
    const char *action)
{
    player_t *player;
    int action_time;

    if (!game_state || !action)
        return false;
    player = get_player(game_state, player_id);
    if (!player || !player->is_alive) {
        printf("Player %d not found or not alive\n", player_id);
        return false;
    }
    if (!is_action_ready(game_state, player)) {
        printf("Action not ready for player %d\n", player_id);
        return false;
    }
    action_time = get_action_time(action, game_state->frequency);
    player->action_end_time = game_state->current_tick + action_time;
    printf("Action queued for player %d, will be ready at tick %d\n",
        player_id, player->action_end_time);
    return true;
}

static void remove_player_from_old_tile(game_state_t *game_state,
    player_t *player)
{
    tile_t *old_tile;

    old_tile = get_tile(game_state->map, player->x, player->y);
    if (old_tile)
        remove_player_from_tile(old_tile, player);
}

static void add_player_to_new_tile(game_state_t *game_state,
    player_t *player)
{
    tile_t *new_tile;

    new_tile = get_tile(game_state->map, player->x, player->y);
    if (new_tile)
        add_player_to_tile(new_tile, player);
}

bool move_player(game_state_t *game_state, int player_id, int new_x,
    int new_y)
{
    player_t *player;

    if (!game_state)
        return false;
    player = get_player(game_state, player_id);
    if (!player || !player->is_alive)
        return false;
    remove_player_from_old_tile(game_state, player);
    player->x = new_x;
    player->y = new_y;
    add_player_to_new_tile(game_state, player);
    broadcast_player_position(&game_state->broadcast_system, game_state,
        player);
    return true;
}

bool set_player_level(game_state_t *game_state, int player_id,
    int new_level)
{
    player_t *player;
    int old_level;

    if (!game_state)
        return false;
    player = get_player(game_state, player_id);
    if (!player || !player->is_alive)
        return false;
    old_level = player->level;
    player->level = new_level;
    if (old_level != new_level)
        broadcast_player_level(&game_state->broadcast_system, game_state,
            player);
    return true;
}

static void validate_inventory_amount(player_t *player, int resource_type)
{
    if (player->inventory[resource_type] < 0)
        player->inventory[resource_type] = 0;
}

bool update_player_inventory(game_state_t *game_state, int player_id,
    int resource_type, int amount)
{
    player_t *player;

    if (!game_state)
        return false;
    player = get_player(game_state, player_id);
    if (!player || !player->is_alive)
        return false;
    if (resource_type < 0 || resource_type >= RESOURCE_COUNT)
        return false;
    player->inventory[resource_type] += amount;
    validate_inventory_amount(player, resource_type);
    broadcast_player_inventory(&game_state->broadcast_system, game_state,
        player);
    return true;
}

void destroy_player(player_t *player)
{
    if (!player)
        return;
    free(player->team_name);
    free(player);
}
