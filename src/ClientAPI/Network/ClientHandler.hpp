/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ClientHandler - Manages individual client connections and protocol handling
*/

#ifndef CLIENTHANDLER_HPP_
#define CLIENTHANDLER_HPP_

#include <string>
#include <queue>
#include <memory>
#include <vector>
#include "CommandBuffer.hpp"
#include "../Model/Player.hpp"

namespace clientapi {

class ConnectionManager;

/**
 * @enum ClientType
 * @brief Represents the type of client connection
 * 
 * Zappy supports two types of clients:
 * - AI clients: Control game entities, join teams, play the game
 * - GRAPHIC clients: Observe the game, receive all game events for display
 */
enum class ClientType {
    UNKNOWN,    ///< Client not yet authenticated
    AI,         ///< AI client controlling a player (team-based)
    GRAPHIC     ///< GUI client observing the game (team "GRAPHIC")
};

/**
 * @class ClientHandler
 * @brief Manages a single client connection from authentication through gameplay
 * 
 * The ClientHandler is responsible for:
 * - Managing the network connection for one client
 * - Handling the authentication flow (WELCOME -> team name -> game info)
 * - Processing incoming commands from AI clients
 * - Sending responses and game updates
 * - Managing the client's Player instance (for AI clients)
 * - Buffering commands according to Zappy protocol (max 10 commands)
 * 
 * Protocol Flow:
 * 1. Client connects, ClientHandler sends "WELCOME"
 * 2. Client sends team name (e.g., "team1" or "GRAPHIC")
 * 3. ClientHandler authenticates and sends game info
 * 4. For AI clients: Commands are processed through CommandBuffer
 * 5. For GUI clients: Game events are sent as they occur
 * 
 * @note Each ClientHandler is owned by ConnectionManager and represents one TCP connection
 * 
 * @example Authentication flow:
 * @code
 * // Server -> Client: "WELCOME\n"
 * // Client -> Server: "team1\n"
 * // Server -> Client: "20 20\n5\n"  (map dimensions + available slots)
 * @endcode
 */
class ClientHandler {
    public:
        /**
         * @brief Constructs a ClientHandler for a new client connection
         * @param socket The TCP socket file descriptor for this client
         * @param manager Pointer to the ConnectionManager that owns this handler
         * 
         * Initializes the handler in UNKNOWN state and creates a CommandBuffer for AI clients.
         */
        ClientHandler(int socket, ConnectionManager* manager);
        
        /**
         * @brief Destructor - closes socket and cleans up resources
         */
        ~ClientHandler();

        // Network I/O Management
        
        /**
         * @brief Handles incoming data from the client socket
         * @return true if connection is still valid, false if client disconnected
         * 
         * This method:
         * - Reads available data from the socket (non-blocking)
         * - Buffers incomplete messages until newline is received
         * - Processes complete messages based on authentication state
         * - Handles authentication for new clients
         * - Forwards commands to CommandBuffer for authenticated AI clients
         */
        bool handleIncomingData();
        
        /**
         * @brief Sends a response message to the client
         * @param response The message to send (newline will be added automatically)
         * 
         * Used for sending command responses, game updates, and protocol messages.
         */
        void sendResponse(const std::string& response);
        
        /**
         * @brief Sends the initial WELCOME message to a new client
         * 
         * Called immediately when a new connection is accepted.
         * Starts the authentication flow according to Zappy protocol.
         */
        void sendWelcome();
        
        // Authentication and Client Management
        
        /**
         * @brief Attempts to authenticate a client with the given team name
         * @param teamName The team name sent by the client
         * @return true if authentication successful, false otherwise
         * 
         * Authentication rules:
         * - "GRAPHIC" always authenticates as GUI client
         * - Valid team names authenticate as AI clients if slots available
         * - Invalid team names or full teams are rejected
         * 
         * On successful authentication:
         * - Sets client type and team name
         * - Creates Player instance for AI clients
         * - Sends game information to client
         */
        bool authenticate(const std::string& teamName);
        
        /**
         * @brief Checks if client has completed authentication
         * @return true if client type is not UNKNOWN
         */
        bool isAuthenticated() const { return _clientType != ClientType::UNKNOWN; }
        
        // Client Information Access
        
        /**
         * @brief Gets the socket file descriptor for this client
         * @return Socket file descriptor
         */
        int getSocket() const { return _socket; }
        
        /**
         * @brief Gets the team name for authenticated clients
         * @return Team name string, or empty string if not authenticated
         */
        const std::string& getTeamName() const { return _teamName; }
        
        /**
         * @brief Gets the client type (UNKNOWN, AI, or GRAPHIC)
         * @return ClientType enum value
         */
        ClientType getClientType() const { return _clientType; }
        
        /**
         * @brief Gets the Player instance for AI clients
         * @return Pointer to Player object, or nullptr for non-AI clients
         */
        clientapi::Player* getPlayer() const { return _player.get(); }

        // Command Processing
        
        /**
         * @brief Processes a command from an authenticated AI client
         * @param command The command string to process
         * 
         * For AI clients:
         * - Validates command format
         * - Adds command to buffer if space available (max 10)
         * - Sends "ko" for invalid commands or full buffer
         * 
         * For GUI clients:
         * - Commands are handled differently (GUI protocol)
         */
        void processCommand(const std::string& command);

        /**
         * @brief Processes a command from a GUI client
         * @param command The GUI command string to process
         * 
         * GUI commands include:
         * - msz: Request map size
         * - bct X Y: Request tile content
         * - mct: Request all tiles content
         * - tna: Request team names
         * - ppo #n: Request player position
         * - plv #n: Request player level
         * - pin #n: Request player inventory
         * - sgt: Request time unit
         * - sst T: Modify time unit
         */
        void processGUICommand(const std::string& command);

        /**
         * @brief Send a map size message to GUI client
         * @param width Map width
         * @param height Map height
         */
        void sendMapSize(int width, int height);

        /**
         * @brief Send a tile content message to GUI client
         * @param x Tile X coordinate
         * @param y Tile Y coordinate
         * @param resources Vector of resource quantities
         */
        void sendTileContent(int x, int y, const std::vector<int>& resources);

        /**
         * @brief Send all tile contents to GUI client
         * @param allTiles Vector of all tile contents
         */
        void sendAllTileContents(const std::vector<std::vector<int>>& allTiles);

        /**
         * @brief Send team names to GUI client
         * @param teams Vector of team names
         */
        void sendTeamNames(const std::vector<std::string>& teams);

        /**
         * @brief Send player position to GUI client
         * @param playerId Player ID
         * @param x Player X coordinate
         * @param y Player Y coordinate
         * @param orientation Player orientation
         */
        void sendPlayerPosition(int playerId, int x, int y, int orientation);

        /**
         * @brief Send player level to GUI client
         * @param playerId Player ID
         * @param level Player level
         */
        void sendPlayerLevel(int playerId, int level);

        /**
         * @brief Send player inventory to GUI client
         * @param playerId Player ID
         * @param x Player X coordinate
         * @param y Player Y coordinate
         * @param inventory Vector of inventory quantities
         */
        void sendPlayerInventory(int playerId, int x, int y, const std::vector<int>& inventory);

        /**
         * @brief Send time unit to GUI client
         * @param timeUnit Current time unit
         */
        void sendTimeUnit(int timeUnit);

        /**
         * @brief Send new player event to GUI client
         * @param playerId Player ID
         * @param x Player X coordinate
         * @param y Player Y coordinate
         * @param orientation Player orientation
         * @param level Player level
         * @param team Player team name
         */
        void sendPlayerNew(int playerId, int x, int y, int orientation, int level, const std::string& team);

        /**
         * @brief Send player expulsion event to GUI client
         * @param playerId Player ID
         */
        void sendPlayerExpulsion(int playerId);

        /**
         * @brief Send player broadcast event to GUI client
         * @param playerId Player ID
         * @param message Broadcast message
         */
        void sendPlayerBroadcast(int playerId, const std::string& message);

        /**
         * @brief Send incantation start event to GUI client
         * @param x Incantation X coordinate
         * @param y Incantation Y coordinate
         * @param level Incantation level
         * @param players Vector of participating player IDs
         */
        void sendIncantationStart(int x, int y, int level, const std::vector<int>& players);

        /**
         * @brief Send incantation end event to GUI client
         * @param x Incantation X coordinate
         * @param y Incantation Y coordinate
         * @param result Incantation result (0 for failure, 1 for success)
         */
        void sendIncantationEnd(int x, int y, int result);

        /**
         * @brief Send resource drop event to GUI client
         * @param playerId Player ID
         * @param resourceId Resource ID
         */
        void sendResourceDrop(int playerId, int resourceId);

        /**
         * @brief Send resource collect event to GUI client
         * @param playerId Player ID
         * @param resourceId Resource ID
         */
        void sendResourceCollect(int playerId, int resourceId);

        /**
         * @brief Send player death event to GUI client
         * @param playerId Player ID
         */
        void sendPlayerDeath(int playerId);

        /**
         * @brief Send egg laid event to GUI client
         * @param eggId Egg ID
         * @param playerId Player ID who laid the egg
         * @param x Egg X coordinate
         * @param y Egg Y coordinate
         */
        void sendEggLaid(int eggId, int playerId, int x, int y);

        /**
         * @brief Send egg hatching event to GUI client
         * @param eggId Egg ID
         */
        void sendEggHatching(int eggId);

        /**
         * @brief Send egg death event to GUI client
         * @param eggId Egg ID
         */
        void sendEggDeath(int eggId);

        /**
         * @brief Send game end event to GUI client
         * @param winningTeam Name of the winning team
         */
        void sendGameEnd(const std::string& winningTeam);

        /**
         * @brief Send server message to GUI client
         * @param message Server message
         */
        void sendServerMessage(const std::string& message);

    private:
        // Network connection
        int _socket;                                ///< TCP socket file descriptor
        ConnectionManager* _connectionManager;      ///< Parent connection manager
        
        // Client state
        ClientType _clientType;                     ///< Type of client (UNKNOWN/AI/GRAPHIC)
        std::string _teamName;                      ///< Team name after authentication
        
        // Game objects (AI clients only)
        std::unique_ptr<clientapi::Player> _player;            ///< Player instance for AI clients
        std::unique_ptr<clientapi::CommandBuffer> _commandBuffer; ///< Command buffer for AI clients
        
        // Network buffering
        std::string _inputBuffer;                   ///< Incomplete incoming data buffer

        /**
         * @brief Handles authentication response from client
         * @param input The team name or identifier sent by client
         * 
         * Called when client is in UNKNOWN state and sends their team name.
         * Attempts authentication and handles success/failure.
         */
        void handleAuthenticationResponse(const std::string& input);
        
        /**
         * @brief Sends game information to newly authenticated client
         * 
         * Message format depends on client type:
         * - AI clients: "X Y\n" (map dimensions) followed by "N\n" (available slots)
         * - GUI clients: Complete game state information
         */
        void sendGameInfo();
        
        /**
         * @brief Reads available data from socket into input buffer
         * @return true if data read successfully or no data available, false on error/disconnect
         * 
         * Uses non-blocking recv() to read data. Returns false if client disconnected.
         */
        bool readFromSocket();
        
        /**
         * @brief Processes complete messages from the input buffer
         * 
         * Splits input buffer on newlines and processes each complete message.
         * Handles both authentication and command processing based on client state.
         */
        void processBufferedInput();
        
        /**
         * @brief Splits input string into individual commands on newlines
         * @param input Raw input string potentially containing multiple commands
         * @return Vector of individual command strings
         */
        std::vector<std::string> splitCommands(const std::string& input);
};

} // namespace clientapi

#endif /* !CLIENTHANDLER_HPP_ */ 