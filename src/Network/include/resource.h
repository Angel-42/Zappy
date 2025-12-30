/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Resource management system header
*/

#ifndef GAME_STATE_RESOURCE_H_
    #define GAME_STATE_RESOURCE_H_

    #include "game_state.h"

bool take_resource(game_state_t *game_state, player_t *player,
    resource_type_t type);
bool drop_resource(game_state_t *game_state, player_t *player,
    resource_type_t type);
bool add_resource(resource_with_broadcast_t *op);
bool remove_resource(resource_with_broadcast_t *op);
int get_resource_count(map_t *map, int x, int y, resource_type_t type);

// New functions with broadcasting
bool add_resource_with_broadcast(resource_with_broadcast_t *op);
bool remove_resource_with_broadcast(resource_with_broadcast_t *op);

#endif // GAME_STATE_RESOURCE_H_
