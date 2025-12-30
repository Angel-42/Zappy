/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Command processing and handling header
*/

/*
** This header defines the command processing system for the Zappy server.
** It includes command parsing, tokenization, and the command handler interface
** that allows both AI and GUI clients to interact with the server.
*/

#ifndef COMMAND_H_
    #define COMMAND_H_

    #include <dirent.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include "./commands/ai.h"
    #include "./commands/gui.h"

    #ifndef MAX_COMMAND_LENGTH
        /**
         * @brief Maximum length of a command string
         *
         * This is the maximum size of each command in the command stack.
         * Commands longer than this will be truncated or rejected.
         *
         * @note This limit applies to both AI and GUI commands
         * @note Commands are null-terminated strings
         */
        #define MAX_COMMAND_LENGTH 1024
    #endif /* !MAX_COMMAND_LENGTH */

/**
 * @brief Function pointer type for command handlers
 *
 * All command handlers must conform to this signature. They receive
 * a client pointer and an array of command arguments, and return
 * an integer status code indicating success or failure.
 *
 * @param client The client that sent the command
 * @param args Array of command arguments (null-terminated)
 * @return 0 on success, non-zero error code on failure
 */
typedef int (*command_t)(client_t *client, char **args);

/**
 * @brief Command handler structure for hash-based command lookup
 *
 * This structure pairs a command hash with its corresponding handler function.
 * The hash is computed from the command name for efficient lookup in
 * the command handler arrays.
 */
typedef struct command_handler_s {
    unsigned long hash;  /**< Hash value of the command name for fast lookup */
    command_t handler;   /**< Function pointer to the command handler */
} command_handler_t;

/**
 * @brief Tokenizes a command string into an array of arguments
 *
 * This function takes a command string and splits it into an array of
 * arguments, using whitespace as the delimiter. It dynamically allocates
 * memory for the array and each argument, so it is the caller's responsibility
 * to free the allocated memory after use.
 *
 * The function handles:
 * - Multiple consecutive whitespace characters
 * - Leading and trailing whitespace
 * - Empty commands
 * - Memory allocation failures
 *
 * @param args The command string to tokenize (must be null-terminated)
 * @return A dynamically allocated array of strings (arguments),
 *         or NULL on failure.
 *   The last element of the array is NULL to indicate the end of arguments.
 *
 * @note The caller is responsible for freeing both the array and each string
 * @note Returns NULL if args is NULL or if memory allocation fails
 * @note Empty strings result in an array with one NULL element
 *
 * @example
 * ```c
 * char **tokens = arg_tokeniser("Forward 3");
 * // tokens[0] = "Forward", tokens[1] = "3", tokens[2] = NULL
 * // ... use tokens ...
 * // Free memory
 * for (int i = 0; tokens[i] != NULL; i++) {
 *     free(tokens[i]);
 * }
 * free(tokens);
 * ```
 */
char **arg_tokeniser(const char *args);

/**
 * @brief Transfers data between two sockets
 *
 * This function transfers data between two sockets, acting as a bridge
 * between the sender and receiver sockets. It reads data from the sender
 * socket and writes it to the receiver socket, handling any errors that may
 * occur during the transfer process.
 *
 * The function performs a complete transfer of all available data, handling:
 * - Partial reads and writes
 * - Socket errors and disconnections
 * - Memory allocation for buffers
 * - Proper cleanup on errors
 *
 * @param receiver The socket descriptor of the receiver (destination)
 * @param sender The socket descriptor of the sender (source)
 * @param buffer_size The size of the buffer used for data transfer
 *
 * @return Returns 0 on successful completion, or -1 on failure
 *
 * @note Both sockets must be valid and open
 * @note The function will block until all data is transferred or an error
 * @note On failure, the function will close both sockets
 * @note The buffer_size parameter affects performance but not functionality
 */
int transfer_tool(int receiver, int sender, ssize_t buffer_size);

/**
 * @brief Handles validation errors for client commands
 *
 * This function provides a standardized way to handle command validation
 * errors. It sends appropriate error messages to the client and may
 * perform additional error handling such as logging or client disconnection.
 *
 * Common validation errors include:
 * - Invalid command syntax
 * - Missing required arguments
 * - Invalid argument types or ranges
 * - Client not in correct state for command
 *
 * @param client The client that sent the invalid command
 * @param return_code The specific error code indicating the type of failure
 * @return The return_code parameter (for chaining with other error handling)
 *
 * @note This function may modify the client's state or disconnect the client
 * @note Error messages are sent to the client before any other processing
 */
int handle_validation_error(client_t *client, int return_code);

#endif /* !COMMAND_H_ */
