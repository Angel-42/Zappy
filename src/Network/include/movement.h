/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Movement command handlers header
*/

#ifndef MOVEMENT_H_
    #define MOVEMENT_H_

    #include "game_state.h"

// Movement command handlers
char *handle_forward(game_state_t *game_state, player_t *player);
char *handle_right(game_state_t *game_state, player_t *player);
char *handle_left(game_state_t *game_state, player_t *player);

#endif /* !MOVEMENT_H_ */
