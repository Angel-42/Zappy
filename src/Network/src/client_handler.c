/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Client handling implementation
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "client_handler.h"
#include "game_state.h"
#include "commands.h"
#include "data.h"

void cleanup_client_handler(client_handler_context_t *context)
{
    if (!context)
        return;
    if (context->game_state) {
        destroy_game_state(context->game_state);
        context->game_state = NULL;
    }
}

static char *prepare_command(const char *command)
{
    char *cmd_copy;
    char *newline;

    cmd_copy = strdup(command);
    if (!cmd_copy)
        return NULL;
    newline = strchr(cmd_copy, '\n');
    if (newline)
        *newline = '\0';
    return cmd_copy;
}

static bool send_response_to_client(int client_socket, char *response,
    int player_id)
{
    ssize_t sent;

    sent = send(client_socket, response, strlen(response), 0);
    if (sent < 0) {
        printf("Failed to send response to player %d\n", player_id);
        return false;
    }
    return true;
}

static bool process_and_send_response(game_state_t *game_state,
    player_t *player, char *cmd_copy, int client_socket)
{
    char *response;

    response = process_command(game_state, player, cmd_copy);
    if (!response) {
        printf("Failed to process command for player %d\n", player->id);
        return false;
    }
    if (!send_response_to_client(client_socket, response, player->id)) {
        free(response);
        return false;
    }
    free(response);
    return true;
}

static player_t *validate_and_log_command(game_state_t *game_state,
    int client_socket, const char *command)
{
    player_t *player;

    player = get_player_by_socket(game_state, client_socket);
    if (!player) {
        printf("No player found for socket %d\n", client_socket);
        return NULL;
    }
    printf("Processing command for player %d (socket %d): %s", player->id,
        client_socket, command);
    return player;
}

bool handle_ai_command(game_state_t *game_state, int client_socket,
    const char *command)
{
    player_t *player;
    char *cmd_copy;

    if (!game_state || !command)
        return false;
    player = validate_and_log_command(game_state, client_socket, command);
    if (!player)
        return false;
    cmd_copy = prepare_command(command);
    if (!cmd_copy)
        return false;
    if (!process_and_send_response(game_state, player, cmd_copy,
        client_socket)) {
        free(cmd_copy);
        return false;
    }
    free(cmd_copy);
    printf("Successfully processed command for player %d: %s", player->id,
        command);
    return true;
}

static void shift_players_array(game_state_t *game_state, int index)
{
    for (int j = index; j < game_state->player_count - 1; j++) {
        game_state->players[j] = game_state->players[j + 1];
    }
    game_state->player_count--;
}

bool handle_client_disconnect(client_handler_context_t *context,
    int client_socket)
{
    player_t *player;

    if (!context || !context->game_state)
        return false;
    player = get_player_by_socket(context->game_state, client_socket);
    if (!player)
        return false;
    printf("Client disconnected: player %d (team: %s)\n", player->id,
        player->team_name);
    remove_player(context->game_state, player->id);
    return true;
}

bool update_game_tick(client_handler_context_t *context)
{
    if (!context || !context->game_state)
        return false;
    return update_game_state(context->game_state);
}

game_state_t *get_game_state(client_handler_context_t *context)
{
    return context ? context->game_state : NULL;
}
