/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Resource command handlers header
*/

#ifndef RESOURCES_H_
    #define RESOURCES_H_

    #include <stdbool.h>
    #include "game_state.h"

// Resource command handlers
char *handle_take(game_state_t *game_state, player_t *player,
    const char *resource_name);
char *handle_set(game_state_t *game_state, player_t *player,
    const char *resource_name);

// Resource command validation and utilities
bool is_valid_resource_command(const char *command);
int get_resource_time(const char *command, int frequency);

#endif /* !RESOURCES_H_ */
