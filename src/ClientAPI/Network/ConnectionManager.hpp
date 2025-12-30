/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ConnectionManager - Manages TCP network connections and client lifecycle
*/

#ifndef CONNECTIONMANAGER_HPP_
#define CONNECTIONMANAGER_HPP_

#include <vector>
#include <memory>
#include <map>
#include "ClientHandler.hpp"

namespace clientapi {

/**
 * @class ConnectionManager
 * @brief Manages TCP server socket and all client connections for the Zappy server
 * 
 * The ConnectionManager is responsible for:
 * - Setting up and managing the server socket
 * - Accepting new client connections (AI clients and GUI clients)
 * - Managing the lifecycle of all connected clients
 * - Enforcing team-based connection limits
 * - Using select() for non-blocking I/O multiplexing as required by the project
 * 
 * @note This class implements the networking requirements specified in the Zappy protocol:
 * - Supports both AI clients (team-based authentication) and GRAPHIC clients
 * - Enforces maximum clients per team limits
 * - Uses select() for socket multiplexing (not poll as initially mentioned)
 * 
 * @example Basic usage:
 * @code
 * std::vector<std::string> teams = {"team1", "team2"};
 * ConnectionManager manager(4242, teams, 5);
 * 
 * if (manager.initialize()) {
 *     manager.run(); // Blocks and handles all connections
 * }
 * @endcode
 */
class ConnectionManager {
    public:
        /**
         * @brief Constructs a ConnectionManager with server configuration
         * @param port Server port to listen on
         * @param teamNames List of valid team names for AI clients
         * @param clientsPerTeam Maximum number of clients allowed per team
         * 
         * @note The team name "GRAPHIC" is reserved for GUI clients and handled specially
         */
        ConnectionManager(int port, const std::vector<std::string>& teamNames, int clientsPerTeam);
        
        /**
         * @brief Destructor - automatically shuts down server and cleans up connections
         */
        ~ConnectionManager();

        /**
         * @brief Initializes the server socket and prepares for connections
         * @return true if initialization successful, false otherwise
         * 
         * Sets up:
         * - TCP server socket with SO_REUSEADDR
         * - Non-blocking socket mode
         * - Binds to specified port and starts listening
         */
        bool initialize();
        
        /**
         * @brief Main event loop - handles all network I/O using select()
         * 
         * This method blocks and continuously:
         * - Monitors all sockets for activity using select()
         * - Accepts new connections
         * - Processes incoming data from existing clients
         * - Removes disconnected clients
         * 
         * @note This runs until shutdown() is called or an error occurs
         */
        void run();
        
        /**
         * @brief Gracefully shuts down the server and disconnects all clients
         */
        void shutdown();

        // Client Management
        
        /**
         * @brief Accepts a new client connection and creates a ClientHandler
         * 
         * Called automatically when select() detects activity on the server socket.
         * Creates a new ClientHandler and sends the initial WELCOME message.
         */
        void acceptNewConnection();
        
        /**
         * @brief Removes and disconnects a client by socket descriptor
         * @param socket The socket descriptor of the client to remove
         * 
         * Updates team counts and properly closes the socket connection.
         */
        void removeClient(int socket);
        
        /**
         * @brief Retrieves a client handler by socket descriptor
         * @param socket The socket descriptor to look up
         * @return Pointer to ClientHandler if found, nullptr otherwise
         */
        ClientHandler* getClient(int socket);

        // Team Management
        
        /**
         * @brief Gets the number of available connection slots for a team
         * @param teamName The team to check
         * @return Number of remaining slots (0 if team is full or invalid)
         * 
         * @note Used during client authentication to enforce team limits
         */
        int getAvailableSlots(const std::string& teamName) const;
        
        /**
         * @brief Checks if a team name is valid for AI client connections
         * @param teamName The team name to validate
         * @return true if team exists and accepts connections, false otherwise
         * 
         * @note "GRAPHIC" is always valid but handled separately for GUI clients
         */
        bool isValidTeam(const std::string& teamName) const;

    private:
        // Network configuration
        int _serverSocket;                                      ///< Server socket file descriptor
        int _port;                                              ///< Port number to listen on
        
        // Game configuration
        std::vector<std::string> _teamNames;                    ///< Valid team names for AI clients
        int _clientsPerTeam;                                    ///< Maximum clients allowed per team
        
        // Connection state
        std::map<int, std::unique_ptr<ClientHandler>> _clients; ///< Active client connections (socket -> handler)
        std::map<std::string, int> _teamCounts;                 ///< Current number of clients per team
        bool _running;                                          ///< Server running state

        /**
         * @brief Sets up the server socket with proper options and binding
         * @throws std::runtime_error if socket setup fails
         */
        void setupSocket();
        
        /**
         * @brief Handles incoming data from a specific client socket
         * @param socket The client socket that has data ready
         */
        void handleClientData(int socket);
        
        /**
         * @brief Processes the results of a select() call
         * @param readfds The file descriptor set from select()
         * @param maxfd The highest file descriptor number
         * 
         * Iterates through ready sockets and handles new connections or client data.
         */
        void processSelectResult(fd_set& readfds, int maxfd);
};

} // namespace clientapi

#endif /* !CONNECTIONMANAGER_HPP_ */ 