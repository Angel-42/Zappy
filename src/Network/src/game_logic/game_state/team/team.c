/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Team management system
*/

#include "team.h"
#include <stdlib.h>
#include <string.h>
#include "game_state.h"
#include "player.h"

static bool initialize_team_players(team_t *team, int max_players)
{
    team->players = calloc(max_players, sizeof(player_t *));
    if (!team->players) {
        free(team->name);
        free(team);
        return false;
    }
    return true;
}

team_t *create_team(game_state_t *game_state, const char *team_name,
    int max_players)
{
    team_t *team;

    if (!game_state || !team_name)
        return NULL;
    team = malloc(sizeof(team_t));
    if (!team)
        return NULL;
    team->name = strdup(team_name);
    team->player_count = 0;
    team->max_players = max_players;
    team->eggs_count = 0;
    if (!initialize_team_players(team, max_players))
        return NULL;
    return team;
}

static void cleanup_team_players(team_t *team)
{
    for (int j = 0; j < team->player_count; j++) {
        if (team->players[j])
            destroy_player(team->players[j]);
    }
}

static void shift_teams_array(game_state_t *game_state, int start_index)
{
    for (int j = start_index; j < game_state->team_count - 1; j++)
        game_state->teams[j] = game_state->teams[j + 1];
}

void remove_team(game_state_t *game_state, const char *team_name)
{
    if (!game_state || !team_name)
        return;
    for (int i = 0; i < game_state->team_count; i++) {
        if (game_state->teams[i] &&
            strcmp(game_state->teams[i]->name, team_name) == 0) {
            cleanup_team_players(game_state->teams[i]);
            destroy_team(game_state->teams[i]);
            shift_teams_array(game_state, i);
            game_state->team_count--;
            break;
        }
    }
}

team_t *get_team(game_state_t *game_state, const char *team_name)
{
    if (!game_state || !team_name)
        return NULL;
    for (int i = 0; i < game_state->team_count; i++) {
        if (game_state->teams[i] &&
            strcmp(game_state->teams[i]->name, team_name) == 0)
            return game_state->teams[i];
    }
    return NULL;
}

static void shift_players_array(team_t *team, int start_index)
{
    for (int k = start_index; k < team->player_count - 1; k++)
        team->players[k] = team->players[k + 1];
}

bool remove_player_from_team_bis(game_state_t *game_state, player_t *player,
    team_t *team)
{
    for (int j = 0; j < team->player_count; j++) {
        if (team->players[j] == player) {
            shift_players_array(team, j);
            team->player_count--;
            return true;
        }
    }
    return false;
}

bool remove_player_from_team(game_state_t *game_state, player_t *player)
{
    team_t *team;

    if (!game_state || !player)
        return false;
    for (int i = 0; i < game_state->team_count; i++) {
        team = game_state->teams[i];
        if (remove_player_from_team_bis(game_state, player, team))
            return true;
    }
    return false;
}
