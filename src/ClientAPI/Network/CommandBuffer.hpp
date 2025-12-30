/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** CommandBuffer - Manages timed command execution for AI clients
*/

#ifndef COMMANDBUFFER_HPP_
#define COMMANDBUFFER_HPP_

#include <queue>
#include <string>
#include <memory>
#include "../Model/Player.hpp"

namespace clientapi {
/**
 * @struct PendingCommand
 * @brief Represents a command waiting for execution with timing information
 * 
 * Each AI client command must wait for a specific amount of time before execution.
 * This struct encapsulates all information needed to track and execute a command
 * when its time limit has elapsed.
 */
struct PendingCommand {
    std::string command;        ///< The full command string (e.g., "Broadcast hello")
    clientapi::Player* player;             ///< The player who issued this command
    int timeRemaining;          ///< Time units remaining before execution
    
    /**
     * @brief Constructs a new pending command
     * @param cmd The command string
     * @param p Pointer to the player issuing the command
     * @param time Initial time remaining in time units
     */
    PendingCommand(const std::string& cmd, clientapi::Player* p, int time)
        : command(cmd), player(p), timeRemaining(time) {}
};

/**
 * @class CommandBuffer
 * @brief Manages command buffering and timed execution for individual AI clients
 * 
 * The CommandBuffer implements the Zappy protocol requirement that:
 * - Clients can send up to 10 commands without waiting for responses
 * - Commands are executed in order after their time limit expires
 * - Commands have different execution times (e.g., Forward=7, Fork=42, Inventory=1)
 * 
 * Key Features:
 * - FIFO command queue with timing
 * - Automatic command execution when time expires
 * - Protocol-compliant response formatting
 * - Command validation and error handling
 * - Buffer overflow protection (max 10 commands)
 * 
 * Time Management:
 * Commands are executed based on time units, not real time. The actual execution
 * time depends on the server frequency: actual_seconds = time_units / frequency
 * 
 * @example Usage flow:
 * @code
 * CommandBuffer buffer;
 * 
 * // Client sends commands
 * buffer.addCommand("Forward", player);       // 7 time units
 * buffer.addCommand("Look", player);          // 7 time units  
 * buffer.addCommand("Inventory", player);     // 1 time unit
 * 
 * // Game loop processes time
 * buffer.processCommands(5);  // Advance 5 time units
 * buffer.processCommands(3);  // Execute first command (7 total)
 * @endcode
 */
class CommandBuffer {
    public:
        /**
         * @brief Constructs an empty command buffer
         */
        CommandBuffer();
        
        /**
         * @brief Default destructor - cleans up any pending commands
         */
        ~CommandBuffer() = default;

        // Command Management
        
        /**
         * @brief Adds a new command to the buffer for timed execution
         * @param command The command string to execute (e.g., "Forward", "Broadcast hello")
         * @param player The player issuing this command
         * @return true if command was added successfully, false if buffer is full or command invalid
         * 
         * Validation performed:
         * - Checks if buffer has space (max 10 commands)
         * - Validates command syntax using Protocol::isValidCommand()
         * - Determines execution time using Protocol::getCommandTimeLimit()
         * 
         * If validation fails, an immediate "ko" response should be sent to the client.
         * If buffer is full, additional commands are dropped as per Zappy specification.
         */
        bool addCommand(const std::string& command, clientapi::Player* player);
        
        /**
         * @brief Advances time and processes commands ready for execution
         * @param timeUnits Number of time units to advance
         * 
         * This method:
         * 1. Reduces timeRemaining for all pending commands
         * 2. Executes commands whose timeRemaining reaches 0 or below
         * 3. Maintains command order (FIFO execution)
         * 4. Sends appropriate responses to clients
         * 
         * Called by the game's main time management system each tick.
         */
        void processCommands(int timeUnits);
        
        /**
         * @brief Immediately executes all commands that are ready (timeRemaining <= 0)
         * 
         * Alternative to processCommands() for processing without advancing time.
         * Useful for handling commands that should execute immediately.
         */
        void executeReadyCommands();
        
        // Buffer State
        
        /**
         * @brief Gets the current number of commands in the buffer
         * @return Number of pending commands
         */
        size_t getBufferSize() const { return _commandQueue.size(); }
        
        /**
         * @brief Checks if the buffer is at maximum capacity
         * @return true if buffer contains 10 commands (maximum), false otherwise
         * 
         * When buffer is full, new commands should be dropped as per Zappy protocol:
         * "Over 10, the server will no longer take them into account."
         */
        bool isFull() const;
        
        /**
         * @brief Removes all pending commands from the buffer
         * 
         * Used when a player disconnects or dies to clean up their pending commands.
         */
        void clear();

    private:
        /**
         * @brief Queue of commands waiting for execution, ordered by arrival time
         */
        std::queue<PendingCommand> _commandQueue;
        
        /**
         * @brief Executes a single command and sends the appropriate response
         * @param cmd The command to execute
         * 
         * This method:
         * 1. Parses the command name and arguments
         * 2. Executes the game action (movement, inventory check, etc.)
         * 3. Formats the response according to Zappy protocol
         * 4. Sends the response back to the client
         * 
         * Response formats:
         * - "ok" for successful simple commands
         * - "ko" for failed commands
         * - "[item1 n, item2 n, ...]" for inventory
         * - "[tile1, tile2, ...]" for look commands
         * - Custom messages for special commands
         */
        void executeCommand(const PendingCommand& cmd);
        
        /**
         * @brief Formats an inventory response for the Inventory command
         * @param player The player whose inventory to format
         * @return Formatted inventory string like "[food 5, linemate 2, sibur 0, ...]"
         * 
         * Follows exact Zappy protocol format for inventory responses.
         */
        std::string formatInventoryResponse(clientapi::Player* player);
        
        /**
         * @brief Formats a vision response for the Look command
         * @param player The player performing the look action
         * @return Formatted vision string like "[player, food, , linemate, ...]"
         * 
         * Response format depends on player's level (vision range) and map state.
         * Each tile is comma-separated, with multiple objects space-separated within tiles.
         */
        std::string formatLookResponse(clientapi::Player* player);
};

} // namespace clientapi

#endif /* !COMMANDBUFFER_HPP_ */ 