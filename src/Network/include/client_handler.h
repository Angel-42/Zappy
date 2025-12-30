/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Client handling header
*/

/*
** This header defines the client handling system for the Zappy server.
** It manages both AI and GUI client connections, command processing,
** and game state updates. The client handler provides a unified interface
** for all client interactions.
*/

#ifndef CLIENT_HANDLER_H_
    #define CLIENT_HANDLER_H_

    #include <stdbool.h>
    #include "game_state.h"
    #include "data.h"

/**
 * @brief Client handler context structure
 *
 * This structure contains the context needed for client handling operations.
 * It replaces global variables with a more modular and testable approach.
 * The context holds references to the game state and other necessary data.
 */
typedef struct client_handler_context_s {
    game_state_t *game_state;  /**< Pointer to the main game state */
} client_handler_context_t;

/**
 * @brief Initializes the client handler system
 *
 * Sets up the client handler with the provided server configuration.
 * This includes initializing the game state and preparing the system
 * for client connections.
 *
 * @param context The client handler context to initialize
 * @param data Server configuration data
 * @return true on success, false on failure
 */
bool init_client_handler(client_handler_context_t *context,
    server_data_t *data);

/**
 * @brief Cleans up client handler resources
 *
 * Frees all memory and resources allocated by the client handler.
 * This should be called when shutting down the server to prevent
 * memory leaks.
 *
 * @param context The client handler context to clean up
 */
void cleanup_client_handler(client_handler_context_t *context);

/**
 * @brief Handles a new client connection
 *
 * Processes a new client connection, validates the team name,
 * and adds the client to the appropriate team. This function
 * handles the initial handshake and client registration.
 *
 * @param context The client handler context
 * @param client_socket The socket descriptor for the new client
 * @param team_name The team name provided by the client
 * @return true if client was successfully added, false otherwise
 */
bool handle_new_client(client_handler_context_t *context, int client_socket,
    const char *team_name);

/**
 * @brief Processes AI client commands
 *
 * Parses and executes commands from AI clients. AI commands include
 * movement, resource collection, team communication, and elevation
 * rituals. This function validates commands and updates the game state.
 *
 * @param game_state The current game state
 * @param client_socket The socket of the AI client
 * @param command The command string to process
 * @return true if command was processed successfully, false otherwise
 */
bool handle_ai_command(game_state_t *game_state, int client_socket,
    const char *command);

/**
 * @brief Handles client disconnection
 *
 * Properly removes a client from the game when they disconnect.
 * This includes cleaning up player data, removing from teams,
 * and freeing associated resources.
 *
 * @param context The client handler context
 * @param client_socket The socket of the disconnecting client
 * @return true if client was successfully removed, false otherwise
 */
bool handle_client_disconnect(client_handler_context_t *context,
    int client_socket);

/**
 * @brief Updates the game state for the current tick
 *
 * Performs all necessary game state updates for the current tick,
 * including player actions, resource spawning, egg hatching,
 * and incantation processing.
 *
 * @param context The client handler context
 * @return true if tick was processed successfully, false otherwise
 */
bool update_game_tick(client_handler_context_t *context);

/**
 * @brief Gets the current game state
 *
 * Returns a pointer to the current game state for external access.
 * This allows other modules to read game state without direct
 * access to the context structure.
 *
 * @param context The client handler context
 * @return Pointer to the current game state
 */
game_state_t *get_game_state(client_handler_context_t *context);

/**
 * @brief Processes GUI client commands
 *
 * Handles commands from GUI clients (spectators). GUI commands
 * are typically information requests about the game state,
 * such as map data, player positions, and team information.
 *
 * @param game_state The current game state
 * @param client_socket The socket of the GUI client
 * @param command The command string to process
 * @return true if command was processed successfully, false otherwise
 */
bool handle_gui_command(game_state_t *game_state, int client_socket,
    const char *command);

#endif /* !CLIENT_HANDLER_H_ */
