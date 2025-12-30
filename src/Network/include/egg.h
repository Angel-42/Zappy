/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Egg management header
*/

#ifndef EGG_H_
    #define EGG_H_

    #include "game_state.h"

egg_t *get_egg(game_state_t *game_state, int egg_id);
bool add_egg(game_state_t *game_state, player_t *parent);
bool remove_egg(game_state_t *game_state, int egg_id);
void destroy_egg(egg_t *egg);

// New function with broadcasting
bool hatch_egg(game_state_t *game_state, int egg_id);

#endif /* EGG_H_ */
