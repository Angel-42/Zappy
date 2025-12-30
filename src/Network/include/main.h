/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Main server header - Server context and main program structure
*/

/*
** This header defines the main server context structure that coordinates
** all server components. It provides the high-level interface for
** server initialization, execution, and shutdown.
*/
#ifndef MAIN_H_
    #define MAIN_H_

    #include <stdbool.h>

/**
 * @brief Main server context structure
 *
 * This structure serves as the top-level coordinator for all server
 * components. It contains the running state and the poll handler
 * context that manages all network operations.
 */
typedef struct server_context_s {
    bool running;
    poll_handler_context_t poll_context;
} server_context_t;

#endif /* !MAIN_H_ */
