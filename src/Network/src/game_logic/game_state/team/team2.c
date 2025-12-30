/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Team management system 2
*/

#include "team.h"
#include <stdlib.h>
#include <string.h>
#include "game_state.h"
#include "player.h"

static bool expand_players_array(game_state_t *game_state)
{
    player_t **new_players;

    new_players = realloc(game_state->players,
        (game_state->player_count + 1) * sizeof(player_t *));
    if (!new_players)
        return false;
    game_state->players = new_players;
    return true;
}

bool add_player(game_state_t *game_state, const char *team_name)
{
    player_t *player;

    if (!game_state || !team_name)
        return false;
    if (!expand_players_array(game_state))
        return false;
    player = create_player(game_state, team_name, -1);
    if (!player)
        return false;
    game_state->players[game_state->player_count] = player;
    game_state->player_count++;
    return true;
}

void destroy_team(team_t *team)
{
    if (!team)
        return;
    free(team->name);
    free(team->players);
    free(team);
}

static bool expand_teams_array(game_state_t *game_state)
{
    team_t **new_teams;

    new_teams = realloc(game_state->teams,
        (game_state->team_count + 1) * sizeof(team_t *));
    if (!new_teams)
        return false;
    game_state->teams = new_teams;
    return true;
}

bool add_team(game_state_t *game_state, const char *team_name)
{
    team_t *team;

    if (!game_state || !team_name)
        return false;
    if (!expand_teams_array(game_state))
        return false;
    team = create_team(game_state, team_name, 4);
    if (!team)
        return false;
    game_state->teams[game_state->team_count] = team;
    game_state->team_count++;
    return true;
}

int get_available_slots(game_state_t *game_state, const char *team_name)
{
    team_t *team;

    team = get_team(game_state, team_name);
    if (!team)
        return 0;
    return team->max_players - team->player_count + team->eggs_count;
}

bool add_player_to_team(game_state_t *game_state, player_t *player,
    const char *team_name)
{
    team_t *team;

    if (!game_state || !player || !team_name)
        return false;
    team = get_team(game_state, team_name);
    if (!team || team->player_count >= team->max_players)
        return false;
    team->players[team->player_count] = player;
    team->player_count++;
    return true;
}
