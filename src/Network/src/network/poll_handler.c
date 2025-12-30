/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Poll event handling implementation
*/

#include "poll_handler.h"
#include "client_handler.h"
#include "game_state.h"
#include "broadcast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>

static void handle_gui_client_registration(game_state_t *game_state,
    int client_socket)
{
    printf("GUI client connected\n");
    if (register_gui_client(&game_state->broadcast_system, game_state,
        client_socket)) {
        printf("GUI client registered for broadcasting\n");
        send_initial_game_state(&game_state->broadcast_system, game_state,
            client_socket);
    } else {
        printf("Failed to register GUI client\n");
    }
}

static void handle_ai_client_registration(poll_handler_context_t *context,
    int client_socket, const char *team_name, int poll_index)
{
    game_state_t *game_state = get_game_state(&context->client_context);
    char response[64];
    const char *error;

    if (handle_new_client(&context->client_context, client_socket,
        team_name)) {
        snprintf(response, sizeof(response), "%d\n%d %d\n",
            get_available_slots(game_state, team_name) - 1,
            game_state->map->width, game_state->map->height);
        send(client_socket, response, strlen(response), 0);
    } else {
        error = "ko\n";
        send(client_socket, error, strlen(error), 0);
        handle_poll_client_disconnect(context, poll_index);
    }
}

static void handle_new_client_registration(poll_handler_context_t *context,
    int client_socket, char *buffer, int poll_index)
{
    game_state_t *game_state = get_game_state(&context->client_context);
    char *team_name = strdup(buffer);
    char *newline = strchr(team_name, '\n');

    if (newline)
        *newline = '\0';
    if (strcmp(team_name, "GRAPHIC") == 0) {
        handle_gui_client_registration(game_state, client_socket);
    } else {
        handle_ai_client_registration(context, client_socket, team_name,
            poll_index);
    }
    free(team_name);
}

static void handle_existing_client_command(game_state_t *game_state,
    int client_socket, char *buffer, player_t *player)
{
    if (strcmp(player->team_name, "GRAPHIC") == 0) {
        handle_gui_command(game_state, client_socket, buffer);
    } else {
        handle_ai_command(game_state, client_socket, buffer);
    }
}

static int process_client_data(poll_handler_context_t *context,
    int poll_index, char *buffer)
{
    int client_socket;
    ssize_t bytes_received;

    client_socket = context->poll_fds[poll_index].fd;
    bytes_received = recv(client_socket, buffer, 1023, 0);
    if (bytes_received <= 0) {
        handle_poll_client_disconnect(context, poll_index);
        return -1;
    }
    buffer[bytes_received] = '\0';
    return client_socket;
}

static bool is_gui_client_socket(game_state_t *game_state, int client_socket)
{
    for (int i = 0; i < game_state->broadcast_system.client_count; i++) {
        if (game_state->broadcast_system.clients[i].socket == client_socket &&
            game_state->broadcast_system.clients[i].is_connected) {
            return true;
        }
    }
    return false;
}

static void route_client_command(poll_handler_context_t *context,
    int client_socket, char *buffer, int poll_index)
{
    game_state_t *game_state;
    player_t *player;

    game_state = get_game_state(&context->client_context);
    if (is_gui_client_socket(game_state, client_socket)) {
        handle_gui_command(game_state, client_socket, buffer);
        return;
    }
    player = get_player_by_socket(game_state, client_socket);
    if (!player) {
        handle_new_client_registration(context, client_socket,
            buffer, poll_index);
        return;
    }
    handle_existing_client_command(game_state, client_socket, buffer, player);
}

void handle_client_data(poll_handler_context_t *context, int poll_index)
{
    char buffer[1024];
    int client_socket;

    if (!context || poll_index < 1 || poll_index >= context->poll_count)
        return;
    client_socket = process_client_data(context, poll_index, buffer);
    if (client_socket < 0)
        return;
    route_client_command(context, client_socket, buffer, poll_index);
}

void handle_poll_client_disconnect(poll_handler_context_t *context,
    int poll_index)
{
    int client_socket = -1;

    if (!context || poll_index < 1 || poll_index >= context->poll_count)
        return;
    client_socket = context->poll_fds[poll_index].fd;
    handle_client_disconnect(&context->client_context, client_socket);
    close(client_socket);
    for (int i = poll_index; i < context->poll_count - 1; i++) {
        context->poll_fds[i] = context->poll_fds[i + 1];
    }
    context->poll_count--;
    printf("Client disconnected, socket: %d\n", client_socket);
}
