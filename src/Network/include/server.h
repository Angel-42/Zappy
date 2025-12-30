/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server header - Main server structure and initialization
*/

/*
** This header defines the core server structure and functions for game server
** The server manages network connections, client handling, and game state.
** It supports both AI clients (game players) and GUI clients (spectators).
*/

#ifndef SERVER_H_
    #define SERVER_H_

    #include "client/client.h"
    #include "client/client_list.h"
    #include "command.h"
    #include "data.h"
    #include "game_state.h"

    #include <stdio.h>
    #include <stdlib.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <time.h>

    /**
     * @brief Number of AI commands supported by the server
     *
     * AI commands include: Forward, Right, Left, Look, Inventory, Broadcast,
     * Connect_nbr, Fork, Eject, Take, Set, Incantation
     */
    #define AI_COMMAND_COUNT 12

    /**
     * @brief Number of GUI commands supported by the server
     *
     * GUI commands include: msz, bct, mct, tna, ppo, plv, pin, sgt, sst
     */
    #define GUI_COMMAND_COUNT 9

/**
 * @brief Client handshake states during connection establishment
 *
 * This enum tracks the progress of client authentication and team assignment
 * during the initial connection handshake process.
 */
typedef enum handshake_state_e {
    HANDSHAKE_WELCOME,
    HANDSHAKE_TEAMNAME,
    HANDSHAKE_DONE,
} handshake_state_t;

/**
 * @brief Main server structure containing all server state and resources
 *
 * This structure holds all the necessary data for running the Zappy server,
 * including network sockets, client lists, command handlers, and game state.
 * It serves as the central coordination point for all server operations.
 */
typedef struct server_s {
    command_handler_t ai_commands[AI_COMMAND_COUNT];
    command_handler_t gui_commands[GUI_COMMAND_COUNT];
    int socket;
    struct sockaddr_in addr;
    struct pollfd *pfds;
    nfds_t nfds;
    client_list_t clients;
    game_state_t *game_state;
    time_t last_tick_time;
} server_t;

/**
 * @brief Initializes and starts the Zappy server
 *
 * This function performs the complete server initialization sequence:
 * 1. Creates and configures the server socket
 * 2. Binds the socket to the specified port
 * 3. Starts listening for incoming connections
 * 4. Initializes command handlers for AI and GUI clients
 * 5. Sets up the game state with the provided configuration
 * 6. Enters the main server loop to handle client connections and game logic
 *
 * The server will continue running until it receives a shutdown signal
 * or encounters a fatal error. It handles both AI clients (game players)
 * and GUI clients (spectators) simultaneously.
 *
 * @param server_data Configuration data including port, map dimensions,
 *                    team names, client limits, and game frequency
 * @return 0 on successful server shutdown, 84 on initialization failure
 *
 * @note This function does not return until the server is shut down
 * @note The server handles SIGINT and SIGTERM for graceful shutdown
 * @note All memory allocated during server operation is automatically cleaned
 */
int host_server(const server_data_t *const restrict server_data);

#endif /* SERVER_H_ */
