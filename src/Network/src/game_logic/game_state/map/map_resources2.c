/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Map resource spawning system
*/

#include "map.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "game_state.h"

static void add_resource_at_position(game_state_t *game_state, int pos,
    resource_type_t type)
{
    int x = pos % game_state->map->width;
    int y = pos / game_state->map->width;
    resource_with_broadcast_t op;

    op.game_state = game_state;
    op.x = x;
    op.y = y;
    op.type = type;
    op.amount = 1;
    add_resource_with_broadcast(&op);
}

static void spawn_resource_type(resource_spawn_context_t *context)
{
    int pos;

    for (int i = 0; i < context->count; i++) {
        pos = context->positions[(i + context->offset) % context->map_area];
        add_resource_at_position(context->game_state, pos, context->type);
    }
}

static void setup_spawn_context(resource_spawn_context_t *context,
    game_state_t *game_state, int *positions, int map_area)
{
    context->game_state = game_state;
    context->positions = positions;
    context->map_area = map_area;
}

static void spawn_single_resource_type(resource_spawn_context_t *context,
    int count, int offset, resource_type_t type)
{
    context->count = count;
    context->offset = offset;
    context->type = type;
    spawn_resource_type(context);
}

static void spawn_food_resources(resource_spawn_context_t *context,
    resource_additions_t *additions, int *offset)
{
    spawn_single_resource_type(context, additions->food, *offset, FOOD);
    *offset += additions->food;
}

static void spawn_mineral_resources(resource_spawn_context_t *context,
    resource_additions_t *additions, int *offset)
{
    spawn_single_resource_type(context, additions->linemate, *offset,
        LINEMATE);
    *offset += additions->linemate;
    spawn_single_resource_type(context, additions->deraumere, *offset,
        DERAUMERE);
    *offset += additions->deraumere;
    spawn_single_resource_type(context, additions->sibur, *offset, SIBUR);
    *offset += additions->sibur;
    spawn_single_resource_type(context, additions->mendiane, *offset,
        MENDIANE);
    *offset += additions->mendiane;
    spawn_single_resource_type(context, additions->phiras, *offset,
        PHIRAS);
    *offset += additions->phiras;
    spawn_single_resource_type(context, additions->thystame, *offset,
        THYSTAME);
    *offset += additions->thystame;
}

void spawn_all_resources(game_state_t *game_state, int *positions,
    int map_area, resource_additions_t *additions)
{
    resource_spawn_context_t context;
    int offset = 0;

    setup_spawn_context(&context, game_state, positions, map_area);
    spawn_food_resources(&context, additions, &offset);
    spawn_mineral_resources(&context, additions, &offset);
}
