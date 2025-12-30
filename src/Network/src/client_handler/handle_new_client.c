/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Handle new client connection
*/

#include "client_handler.h"
#include "game_state.h"
#include "team.h"
#include "player.h"
#include "broadcast.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static bool setup_team_for_client(game_state_t *game_state,
    const char *team_name)
{
    team_t *team;

    team = get_team(game_state, team_name);
    if (!team) {
        if (!add_team(game_state, team_name))
            return false;
        team = get_team(game_state, team_name);
        if (!team)
            return false;
    }
    return get_available_slots(game_state, team_name) > 0;
}

static bool add_player_to_game(client_handler_context_t *context,
    player_t *player)
{
    player_t **new_players;

    new_players = realloc(context->game_state->players,
        (context->game_state->player_count + 1) * sizeof(player_t *));
    if (!new_players) {
        destroy_player(player);
        return false;
    }
    context->game_state->players = new_players;
    context->game_state->players[context->game_state->player_count] = player;
    context->game_state->player_count++;
    return true;
}

static void log_new_client_connection(int client_socket, const char *team_name)
{
    printf("=== NEW CLIENT CONNECTION ===\n");
    printf("Socket: %d, Team: %s\n", client_socket, team_name);
}

static void log_client_success(const char *team_name, player_t *player,
    game_state_t *game_state)
{
    printf("New client connected: team=%s, id=%d, pos=(%d,%d), socket=%d\n",
        team_name, player->id, player->x, player->y, player->socket);
    printf("Total players now: %d\n", game_state->player_count);
    printf("=== END NEW CLIENT ===\n");
}

static player_t *create_and_add_player(client_handler_context_t *context,
    int client_socket, const char *team_name)
{
    player_t *player;

    player = create_player(context->game_state, team_name, client_socket);
    if (!player) {
        printf("Failed to create player\n");
        return NULL;
    }
    if (!add_player_to_game(context, player)) {
        printf("Failed to add player to game\n");
        return NULL;
    }
    add_player_to_team(context->game_state, player, team_name);
    return player;
}

bool handle_new_client(client_handler_context_t *context, int client_socket,
    const char *team_name)
{
    player_t *player;

    log_new_client_connection(client_socket, team_name);
    if (!context || !context->game_state || !team_name) {
        printf("Invalid parameters for new client\n");
        return false;
    }
    if (!setup_team_for_client(context->game_state, team_name)) {
        printf("Failed to setup team for client\n");
        return false;
    }
    player = create_and_add_player(context, client_socket, team_name);
    if (!player)
        return false;
    log_client_success(team_name, player, context->game_state);
    return true;
}
