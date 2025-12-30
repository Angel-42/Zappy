/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Core game state management header
*/

#ifndef CORE_H_
    #define CORE_H_

    #include "game_state.h"

game_state_t *create_game_state(int width, int height, int tick_rate);
void destroy_game_state(game_state_t *game_state);
bool start_game(game_state_t *game_state);
bool pause_game(game_state_t *game_state);
bool reset_game(game_state_t *game_state);
bool update_game_state(game_state_t *game_state);
bool process_game_tick(game_state_t *game_state);

#endif /* CORE_H_ */
