/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Resource management system TAKE
*/

#include "resource.h"
#include <stdlib.h>
#include <string.h>
#include "game_state.h"
#include "map.h"
#include "player.h"
#include "broadcast.h"

static bool validate_take_operation(game_state_t *game_state, player_t *player,
    resource_type_t type)
{
    tile_t *tile;

    if (!game_state || !player)
        return false;
    tile = get_tile(game_state->map, player->x, player->y);
    if (!tile || tile->resources[type] <= 0)
        return false;
    return true;
}

static void perform_take_operation(game_state_t *game_state, player_t *player,
    resource_type_t type)
{
    tile_t *tile;

    tile = get_tile(game_state->map, player->x, player->y);
    tile->resources[type]--;
    player->inventory[type]++;
    broadcast_resource_collect(&game_state->broadcast_system, game_state,
        player, type);
    broadcast_tile_content(&game_state->broadcast_system, game_state,
        player->x, player->y);
    broadcast_player_inventory(&game_state->broadcast_system, game_state,
        player);
}

bool take_resource(game_state_t *game_state, player_t *player,
    resource_type_t type)
{
    if (!validate_take_operation(game_state, player, type))
        return false;
    perform_take_operation(game_state, player, type);
    return true;
}
