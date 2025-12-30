/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** NetworkManager - Handles network communication with the server
*/

#ifndef NETWORKMANAGER_HPP_
#define NETWORKMANAGER_HPP_

#include <string>
#include <functional>
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace clientapi {

/**
 * @brief Manages network communication with the Zappy server
 * 
 * This class handles all low-level network operations including:
 * - Establishing and maintaining TCP connections
 * - Sending commands to the server
 * - Receiving and parsing responses
 * - Managing network threads
 * - Buffering incoming messages
 * 
 * The class uses a separate thread for network operations to avoid
 * blocking the main application thread. All network operations are
 * thread-safe and use mutexes to protect shared resources.
 */
class NetworkManager {
public:
    /**
     * @brief Construct a new NetworkManager object
     */
    NetworkManager();

    /**
     * @brief Destroy the NetworkManager object
     * 
     * Ensures proper cleanup of network resources and thread termination
     */
    ~NetworkManager();

    /**
     * @brief Connect to the server
     * @param host The server hostname or IP address
     * @param port The server port number
     * @return true if connection was successful, false otherwise
     */
    bool connect(const std::string& host, int port);

    /**
     * @brief Disconnect from the server
     * 
     * Closes the socket connection and stops the network thread
     */
    void disconnect();

    /**
     * @brief Check if currently connected to the server
     * @return true if connected, false otherwise
     */
    bool isConnected() const;

    /**
     * @brief Send a command to the server
     * @param command The command to send
     * @return true if command was sent successfully
     */
    bool sendCommand(const std::string& command);

    /**
     * @brief Set the callback for handling server responses
     * @param callback The callback function to handle responses
     */
    void setResponseCallback(std::function<void(const std::string&)> callback);

private:
    /**
     * @brief Main network thread function
     * 
     * Handles sending and receiving data in a loop until disconnected
     */
    void networkLoop();

    /**
     * @brief Send data to the server
     * @param data The data to send
     * @return true if data was sent successfully
     */
    bool sendData(const std::string& data);

    /**
     * @brief Receive data from the server
     * @return The received data, or empty string if error
     */
    std::string receiveData();

    /**
     * @brief Process a complete message from the server
     * @param message The message to process
     */
    void processResponse(const std::string& message);

    /**
     * @brief Set up the socket connection
     * @param host The server hostname or IP address
     * @param port The server port number
     * @return true if socket was set up successfully
     */
    bool setupSocket(const std::string& host, int port);

    /**
     * @brief Close the socket connection
     */
    void closeSocket();

    int _socket;                                    ///< The socket file descriptor
    std::atomic<bool> _connected;                   ///< Connection state
    std::atomic<bool> _running;                     ///< Network thread running state
    std::thread _networkThread;                     ///< Network thread
    std::mutex _sendMutex;                          ///< Mutex for send operations
    std::mutex _receiveMutex;                       ///< Mutex for receive operations
    std::queue<std::string> _sendQueue;             ///< Queue of commands to send
    std::string _host;
    int _port;
    std::mutex _callbackMutex;
    using ResponseCallback = std::function<void(const std::string&)>;
    ResponseCallback _responseCallback;
};

} // namespace clientapi

#endif /* !NETWORKMANAGER_HPP_ */ 