/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Action command handlers header
*/

#ifndef ACTIONS_H_
    #define ACTIONS_H_

    #include "game_state.h"

// Action command handlers
char *handle_broadcast(game_state_t *game_state, player_t *player,
    const char *message);
char *handle_fork(game_state_t *game_state, player_t *player);
char *handle_eject(game_state_t *game_state, player_t *player);
char *handle_incantation(game_state_t *game_state, player_t *player);

#endif /* !ACTIONS_H_ */
