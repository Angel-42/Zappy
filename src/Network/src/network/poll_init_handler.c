/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Poll event handling implementation
*/

#include "poll_handler.h"
#include "client_handler.h"
#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

static bool init_context_variables(poll_handler_context_t *context,
    int max_clients_param, int frequency)
{
    context->max_clients = max_clients_param;
    context->tick_rate = frequency;
    context->server_socket = -1;
    context->poll_fds = NULL;
    context->poll_count = 0;
    context->current_tick = 0;
    return true;
}

static bool setup_client_handler(poll_handler_context_t *context,
    server_data_t *data)
{
    if (!init_client_handler(&context->client_context, data)) {
        fprintf(stderr, "Failed to initialize client handler\n");
        return false;
    }
    return true;
}

static bool create_server_socket(poll_handler_context_t *context)
{
    int opt = 1;

    context->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (context->server_socket < 0) {
        fprintf(stderr, "Failed to create server socket\n");
        cleanup_client_handler(&context->client_context);
        return false;
    }
    if (setsockopt(context->server_socket, SOL_SOCKET, SO_REUSEADDR, &opt,
        sizeof(opt)) < 0) {
        fprintf(stderr, "Failed to set socket options\n");
        close(context->server_socket);
        cleanup_client_handler(&context->client_context);
        return false;
    }
    return true;
}

static bool bind_and_listen(poll_handler_context_t *context, int port,
    int max_clients_param)
{
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    if (bind(context->server_socket, (struct sockaddr*)&server_addr,
        sizeof(server_addr)) < 0) {
        fprintf(stderr, "Failed to bind server socket\n");
        close(context->server_socket);
        cleanup_client_handler(&context->client_context);
        return false;
    }
    if (listen(context->server_socket, max_clients_param) < 0) {
        fprintf(stderr, "Failed to listen on server socket\n");
        close(context->server_socket);
        cleanup_client_handler(&context->client_context);
        return false;
    }
    return true;
}

static bool allocate_poll_array(poll_handler_context_t *context,
    int max_clients_param, int port)
{
    context->poll_fds = malloc((max_clients_param + 1) *
        sizeof(struct pollfd));
    if (!context->poll_fds) {
        fprintf(stderr, "Failed to allocate poll array\n");
        close(context->server_socket);
        cleanup_client_handler(&context->client_context);
        return false;
    }
    context->poll_fds[0].fd = context->server_socket;
    context->poll_fds[0].events = POLLIN;
    context->poll_count = 1;
    printf("Poll handler initialized on port %d\n", port);
    return true;
}

bool init_poll_handler(poll_handler_context_t *context, server_data_t *data)
{
    if (!context)
        return false;
    if (!init_context_variables(context, data->max_clients, data->frequency))
        return false;
    if (!setup_client_handler(context, data))
        return false;
    if (!create_server_socket(context))
        return false;
    if (!bind_and_listen(context, data->port, data->max_clients))
        return false;
    if (!allocate_poll_array(context, data->max_clients, data->port))
        return false;
    return true;
}
