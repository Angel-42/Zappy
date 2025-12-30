/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Main server implementation
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "arg_parser.h"
#include "poll_handler.h"
#include "main.h"

int usage(int exit_code)
{
    printf("Usage: ./zappy_server -p <port> -x <width> -y <height> "
        "-n <team1> <team2> ... -c <max_clients> [-f <frequency>]\n");
    printf("Options:\n");
    printf("  -p <port>        : Port number for the server (1-65535)\n");
    printf("  -x <width>       : Width of the map (positive integer)\n");
    printf("  -y <height>      : Height of the map (positive integer)\n");
    printf("  -n <team1> ...   : Names of teams ");
    printf("(at least two, no duplicates)\n");
    printf("  -c <max_clients> : Maximum clients per team ");
    printf("(positive integer)\n");
    printf("  -f <frequency>   : Frequency of server updates");
    printf(" (positive integer, optional)\n");
    return exit_code;
}

void signal_handler(int sig)
{
    (void)sig;
    printf("\nShutting down server...\n");
    exit(0);
}

static void print_server_info(server_data_t *args)
{
    printf("Starting Zappy server...\n");
    printf("Port: %d\n", args->port);
    printf("Map size: %lu x %lu\n", args->width, args->height);
    printf("Max clients per team: %lu\n", args->max_clients);
    printf("Frequency: %lu\n", args->frequency);
    printf("Teams: ");
    for (uint64_t i = 0; i < args->teams.count; i++) {
        printf("%s ", args->teams.names[i]);
    }
    printf("\n");
}

static int init_server(server_context_t *server_ctx, server_data_t *args)
{
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    server_ctx->running = true;
    print_server_info(args);
    if (!init_poll_handler(&server_ctx->poll_context, args)) {
        fprintf(stderr, "Failed to initialize poll handler\n");
        return 84;
    }
    printf("Server started successfully. Waiting for connections...\n");
    return 0;
}

static void run_server_loop(server_context_t *server_ctx)
{
    while (server_ctx->running &&
        is_server_running(&server_ctx->poll_context)) {
        if (!handle_poll_events(&server_ctx->poll_context)) {
            fprintf(stderr, "Error handling poll events\n");
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    server_data_t args = {.frequency = 100};
    server_context_t server_ctx = {0};

    if (argc == 2 && strcmp(argv[1], "-h") == 0)
        return usage(0);
    if (parse_arguments(argc, argv, &args) < 0) {
        fprintf(stderr, "Invalid arguments. Use -h for help.\n");
        return 84;
    }
    if (init_server(&server_ctx, &args) != 0) {
        return 84;
    }
    run_server_loop(&server_ctx);
    cleanup_poll_handler(&server_ctx.poll_context);
    printf("Server shutdown complete.\n");
    return 0;
}
