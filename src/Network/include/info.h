/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Information command handlers header
*/

#ifndef INFO_H_
    #define INFO_H_

    #include "game_state.h"

// Information command handlers
char *handle_look(game_state_t *game_state, player_t *player);
char *handle_inventory(game_state_t *game_state, player_t *player);
char *handle_connect_nbr(game_state_t *game_state, player_t *player);

#endif /* !INFO_H_ */
