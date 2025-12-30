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
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int get_poll_current_tick(poll_handler_context_t *context)
{
    return context ? context->current_tick : 0;
}

bool is_server_running(poll_handler_context_t *context)
{
    return context && context->server_socket >= 0;
}

void cleanup_poll_handler(poll_handler_context_t *context)
{
    if (!context)
        return;
    if (context->poll_fds) {
        free(context->poll_fds);
        context->poll_fds = NULL;
    }
    if (context->server_socket >= 0) {
        close(context->server_socket);
        context->server_socket = -1;
    }
    cleanup_client_handler(&context->client_context);
}

static bool print_poll_success(int poll_result)
{
    if (poll_result < 0) {
        fprintf(stderr, "Poll failed\n");
        return false;
    }
    return true;
}

bool handle_poll_events(poll_handler_context_t *context)
{
    int poll_result;

    if (!context || !context->poll_fds || context->server_socket < 0)
        return false;
    poll_result = poll(context->poll_fds, context->poll_count,
        1000 / context->tick_rate);
    if (!print_poll_success(poll_result))
        return false;
    context->current_tick++;
    update_game_tick(&context->client_context);
    if (context->poll_fds[0].revents & POLLIN)
        handle_new_connection(context);
    for (int i = 1; i < context->poll_count; i++) {
        if (context->poll_fds[i].revents & POLLIN)
            handle_client_data(context, i);
        if (context->poll_fds[i].revents & (POLLHUP | POLLERR))
            handle_poll_client_disconnect(context, i);
    }
    return true;
}

static void register_client_socket(poll_handler_context_t *context,
    int client_socket)
{
    const char *welcome = "WELCOME\n";

    context->poll_fds[context->poll_count].fd = client_socket;
    context->poll_fds[context->poll_count].events = POLLIN;
    context->poll_count++;
    printf("New connection accepted, socket: %d\n", client_socket);
    send(client_socket, welcome, strlen(welcome), 0);
}

void handle_new_connection(poll_handler_context_t *context)
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_socket = -1;

    if (!context)
        return;
    client_socket = accept(context->server_socket,
        (struct sockaddr*)&client_addr, &client_addr_len);
    if (client_socket < 0) {
        fprintf(stderr, "Failed to accept connection\n");
        return;
    }
    if (context->poll_count >= context->max_clients + 1) {
        fprintf(stderr, "Maximum clients reached\n");
        close(client_socket);
        return;
    }
    register_client_socket(context, client_socket);
}
