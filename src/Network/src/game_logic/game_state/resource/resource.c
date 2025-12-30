/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Resource management system
*/

#include "resource.h"
#include <stdlib.h>
#include <string.h>
#include "game_state.h"
#include "map.h"
#include "player.h"
#include "broadcast.h"

static bool validate_resource_operation(resource_with_broadcast_t *op)
{
    tile_t *tile;

    tile = get_tile(op->game_state->map, op->x, op->y);
    if (!tile || op->type >= RESOURCE_COUNT || op->amount <= 0)
        return false;
    return true;
}

bool add_resource(resource_with_broadcast_t *op)
{
    tile_t *tile;

    if (!validate_resource_operation(op))
        return false;
    tile = get_tile(op->game_state->map, op->x, op->y);
    tile->resources[op->type] += op->amount;
    return true;
}

static bool validate_remove_operation(resource_with_broadcast_t *op)
{
    tile_t *tile;

    if (!validate_resource_operation(op))
        return false;
    tile = get_tile(op->game_state->map, op->x, op->y);
    if (tile->resources[op->type] < op->amount)
        return false;
    return true;
}

bool remove_resource(resource_with_broadcast_t *op)
{
    tile_t *tile;

    if (!validate_remove_operation(op))
        return false;
    tile = get_tile(op->game_state->map, op->x, op->y);
    tile->resources[op->type] -= op->amount;
    return true;
}

int get_resource_count(map_t *map, int x, int y, resource_type_t type)
{
    tile_t *tile;

    tile = get_tile(map, x, y);
    if (!tile || type >= RESOURCE_COUNT)
        return -1;
    return tile->resources[type];
}

static bool validate_broadcast_operation(resource_with_broadcast_t *op)
{
    if (!op->game_state || !op->game_state->map)
        return false;
    return validate_resource_operation(op);
}

bool add_resource_with_broadcast(resource_with_broadcast_t *op)
{
    tile_t *tile;

    if (!validate_broadcast_operation(op))
        return false;
    tile = get_tile(op->game_state->map, op->x, op->y);
    tile->resources[op->type] += op->amount;
    broadcast_tile_content(&op->game_state->broadcast_system, op->game_state,
        op->x, op->y);
    return true;
}

bool remove_resource_with_broadcast(resource_with_broadcast_t *op)
{
    tile_t *tile;

    if (!validate_broadcast_operation(op))
        return false;
    tile = get_tile(op->game_state->map, op->x, op->y);
    if (tile->resources[op->type] < op->amount)
        return false;
    tile->resources[op->type] -= op->amount;
    broadcast_tile_content(&op->game_state->broadcast_system,
        op->game_state, op->x, op->y);
    return true;
}
