/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Player management system header
*/

#ifndef GAME_STATE_PLAYER_H_
    #define GAME_STATE_PLAYER_H_

    #include "game_state.h"

player_t *create_player(game_state_t *game_state, const char *team_name,
    int socket);
void remove_player(game_state_t *game_state, int player_id);
player_t *get_player(game_state_t *game_state, int player_id);
player_t *get_player_by_socket(game_state_t *game_state, int socket);
bool update_player(game_state_t *game_state, int player_id);
bool process_player_action(game_state_t *game_state, int player_id,
    const char *action);
void destroy_player(player_t *player);

// New functions with broadcasting
bool move_player(game_state_t *game_state, int player_id, int new_x,
    int new_y);
bool set_player_level(game_state_t *game_state, int player_id,
    int new_level);
bool update_player_inventory(game_state_t *game_state, int player_id,
    int resource_type, int amount);

#endif // GAME_STATE_PLAYER_H_
