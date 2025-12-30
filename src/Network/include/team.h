/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Team management system header
*/

#ifndef GAME_STATE_TEAM_H_
    #define GAME_STATE_TEAM_H_

    #include "game_state.h"

team_t *create_team(game_state_t *game_state, const char *team_name,
    int max_players);
void remove_team(game_state_t *game_state, const char *team_name);
team_t *get_team(game_state_t *game_state, const char *team_name);
bool add_player_to_team(game_state_t *game_state, player_t *player,
    const char *team_name);
bool remove_player_from_team(game_state_t *game_state, player_t *player);
int get_available_slots(game_state_t *game_state, const char *team_name);
bool add_team(game_state_t *game_state, const char *team_name);
bool add_player(game_state_t *game_state, const char *team_name);
void destroy_team(team_t *team);

#endif // GAME_STATE_TEAM_H_
