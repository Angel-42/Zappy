/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Resource command handlers
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "resources.h"
#include "game_state.h"

static resource_type_t parse_food_resources(const char *resource_name)
{
    if (strcmp(resource_name, "food") == 0)
        return FOOD;
    if (strcmp(resource_name, "linemate") == 0)
        return LINEMATE;
    if (strcmp(resource_name, "deraumere") == 0)
        return DERAUMERE;
    return FOOD;
}

static resource_type_t parse_mineral_resources(const char *resource_name)
{
    if (strcmp(resource_name, "sibur") == 0)
        return SIBUR;
    if (strcmp(resource_name, "mendiane") == 0)
        return MENDIANE;
    if (strcmp(resource_name, "phiras") == 0)
        return PHIRAS;
    if (strcmp(resource_name, "thystame") == 0)
        return THYSTAME;
    return FOOD;
}

static resource_type_t parse_resource_type(const char *resource_name)
{
    resource_type_t type;

    if (!resource_name)
        return FOOD;
    type = parse_food_resources(resource_name);
    if (type != FOOD)
        return type;
    return parse_mineral_resources(resource_name);
}

static char *execute_take_action(game_state_t *game_state, player_t *player,
    resource_type_t resource_type)
{
    if (take_resource(game_state, player, resource_type))
        return strdup("ok\n");
    return strdup("ko\n");
}

char *handle_take(game_state_t *game_state, player_t *player,
    const char *resource_name)
{
    resource_type_t resource_type;

    if (!player || !game_state || !resource_name)
        return strdup("ko\n");
    resource_type = parse_resource_type(resource_name);
    if (resource_type == FOOD && strcmp(resource_name, "food") != 0)
        return strdup("ko\n");
    return execute_take_action(game_state, player, resource_type);
}

static char *execute_set_action(game_state_t *game_state, player_t *player,
    resource_type_t resource_type)
{
    if (drop_resource(game_state, player, resource_type))
        return strdup("ok\n");
    return strdup("ko\n");
}

char *handle_set(game_state_t *game_state, player_t *player,
    const char *resource_name)
{
    resource_type_t resource_type;

    if (!player || !game_state || !resource_name)
        return strdup("ko\n");
    resource_type = parse_resource_type(resource_name);
    if (resource_type == FOOD && strcmp(resource_name, "food") != 0)
        return strdup("ko\n");
    return execute_set_action(game_state, player, resource_type);
}

static bool check_resource_command(const char *command, const char *expected)
{
    return strcmp(command, expected) == 0;
}

bool is_valid_resource_command(const char *command)
{
    if (!command)
        return false;
    if (check_resource_command(command, "Take"))
        return true;
    return check_resource_command(command, "Set");
}

int get_resource_time(const char *command, int frequency)
{
    if (!command)
        return 0;
    if (check_resource_command(command, "Take") ||
        check_resource_command(command, "Set"))
        return 7 / frequency;
    return 0;
}
