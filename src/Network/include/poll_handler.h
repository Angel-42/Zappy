/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Poll handler header - I/O multiplexing for network connections
*/

/*
** This header defines the poll-based I/O multiplexing system for the Zappy
** server. It manages multiple client connections efficiently using the poll()
** system call, handling new connections, data transfer, and disconnections.
*/

#ifndef POLL_HANDLER_H_
    #define POLL_HANDLER_H_

typedef struct server_s server_t;
typedef struct server_data_s server_data_t;

/**
 * @brief Performs one iteration of the poll-based event loop
 *
 * This function handles all pending I/O events for the server and its
 * clients. It processes new connections, client data, and disconnections
 * in a single non-blocking operation.
 *
 * @param server The server structure containing all server state
 * @param data Server configuration data
 * @return 0 on success, non-zero on error
 */
int poll_iteration(server_t *const restrict server,
    const server_data_t *const restrict data);

    #include <stdbool.h>
    #include <poll.h>
    #include "client_handler.h"
    #include "data.h"

/**
 * @brief Poll handler context structure
 *
 * This structure contains all the state needed for poll-based I/O
 * multiplexing. It manages the poll file descriptor array and
 * coordinates between network events and client handling.
 */
typedef struct poll_handler_context_s {
    int server_socket; /**< Main server socket for new connections */
    struct pollfd *poll_fds; /**< Array of poll file descriptors */
    int poll_count; /**< Number of active file descriptors */
    int max_clients; /**< Maximum allowed client connections */
    int current_tick; /**< Current game tick number */
    int tick_rate; /**< Game tick rate (ticks per second) */
    client_handler_context_t client_context; /**< Client handler context */
} poll_handler_context_t;

/**
 * @brief Initializes the poll handler system
 *
 * Sets up the poll handler with the provided server configuration.
 * This includes creating the poll file descriptor array and
 * initializing the client handler context.
 *
 * @param context The poll handler context to initialize
 * @param data Server configuration data
 * @return true on success, false on failure
 */
bool init_poll_handler(poll_handler_context_t *context, server_data_t *data);

/**
 * @brief Cleans up poll handler resources
 *
 * Frees all memory and resources allocated by the poll handler.
 * This includes closing sockets and freeing the poll array.
 *
 * @param context The poll handler context to clean up
 */
void cleanup_poll_handler(poll_handler_context_t *context);

/**
 * @brief Handles all pending poll events
 *
 * Processes all ready file descriptors in the poll array. This includes
 * new connections, client data, and disconnections. The function
 * handles one complete event cycle.
 *
 * @param context The poll handler context
 * @return true if events were handled successfully, false on error
 */
bool handle_poll_events(poll_handler_context_t *context);

/**
 * @brief Handles a new client connection
 *
 * Accepts a new connection on the server socket and adds it to
 * the poll array for monitoring. This function is called when
 * the server socket has pending connections.
 *
 * @param context The poll handler context
 */
void handle_new_connection(poll_handler_context_t *context);

/**
 * @brief Handles data from a client
 *
 * Processes incoming data from a client socket. This includes
 * reading commands, parsing them, and sending responses.
 *
 * @param context The poll handler context
 * @param poll_index Index of the client in the poll array
 */
void handle_client_data(poll_handler_context_t *context, int poll_index);

/**
 * @brief Handles client disconnection
 *
 * Removes a disconnected client from the poll array and cleans up
 * associated resources. This function is called when a client
 * socket is closed or encounters an error.
 *
 * @param context The poll handler context
 * @param poll_index Index of the client in the poll array
 */
void handle_poll_client_disconnect(poll_handler_context_t *context,
    int poll_index);

/**
 * @brief Checks if the server is still running
 *
 * Returns the current running state of the server. This is used
 * by the main server loop to determine when to stop.
 *
 * @param context The poll handler context
 * @return true if server is running, false otherwise
 */
bool is_server_running(poll_handler_context_t *context);

/**
 * @brief Gets the current game tick
 *
 * Returns the current game tick number for external access.
 * This allows other modules to synchronize with the game timing.
 *
 * @param context The poll handler context
 * @return Current game tick number
 */
int get_poll_current_tick(poll_handler_context_t *context);

#endif /* !POLL_HANDLER_H_ */
