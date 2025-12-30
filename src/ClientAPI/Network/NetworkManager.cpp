/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** NetworkManager - Handles network communication with the server
*/

#include "NetworkManager.hpp"
#include "Protocol.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>

namespace clientapi {

/**
 * @brief Construct a new NetworkManager object
 * 
 * Initializes the socket to an invalid state and sets up the connection flags
 */
NetworkManager::NetworkManager()
    : _socket(-1), _port(0), _connected(false), _running(false)
{
}

/**
 * @brief Destroy the NetworkManager object
 * 
 * Ensures proper cleanup by disconnecting from the server
 */
NetworkManager::~NetworkManager() {
    disconnect();
}

/**
 * @brief Connect to the server
 * 
 * Establishes a TCP connection to the specified server and starts
 * the network thread for handling communication.
 * 
 * @param host The server hostname or IP address
 * @param port The server port number
 * @return true if connection was successful, false otherwise
 */
bool NetworkManager::connect(const std::string& host, int port) {
    if (_connected) {
        return true;
    }

    _host = host;
    _port = port;

    if (!setupSocket(host, port)) {
        return false;
    }

    _connected = true;
    _running = true;
    std::cout << "NetworkManager::connect: Connected to " << host << ":" << port << std::endl;
    _networkThread = std::thread(&NetworkManager::networkLoop, this);

    return true;
}

/**
 * @brief Disconnect from the server
 * 
 * Stops the network thread and closes the socket connection
 */
void NetworkManager::disconnect() {
    if (!_connected) {
        return;
    }

    _running = false;
    if (_networkThread.joinable()) {
        _networkThread.join();
    }

    closeSocket();
    _connected = false;
}

/**
 * @brief Check if currently connected to the server
 * @return true if connected, false otherwise
 */
bool NetworkManager::isConnected() const {
    return _connected;
}

/**
 * @brief Send a command to the server
 * 
 * Adds the command to the send queue. The network thread will
 * handle the actual sending of the data.
 * 
 * @param command The command to send
 * @return true if command was queued successfully
 */
bool NetworkManager::sendCommand(const std::string& command) {
    if (!_connected) {
        return false;
    }

    std::lock_guard<std::mutex> lock(_sendMutex);
    _sendQueue.push(command);
    return true;
}

/**
 * @brief Set the callback for handling server responses
 * @param callback The callback function to handle responses
 */
void NetworkManager::setResponseCallback(ResponseCallback callback) {
    std::lock_guard<std::mutex> lock(_callbackMutex);
    _responseCallback = callback;
}

/**
 * @brief Main network thread function
 * 
 * Continuously processes the send queue and receives data from the server
 * until disconnected. This function runs in a separate thread to avoid
 * blocking the main application thread.
 */
void NetworkManager::networkLoop() {
    std::string buffer;
    while (_running) {
        // Send queued commands
        {
            std::lock_guard<std::mutex> lock(_sendMutex);
            while (!_sendQueue.empty()) {
                const std::string& cmd = _sendQueue.front();
                if (!sendData(cmd)) {
                    _running = false;
                    break;
                }
                _sendQueue.pop();
            }
        }
        // Receive data
        buffer += receiveData();
        // Process complete messages
        size_t pos = 0;
        while ((pos = buffer.find('\n')) != std::string::npos) {
            std::string message = buffer.substr(0, pos);
            buffer.erase(0, pos + 1);
            processResponse(message);
        }
        // Small sleep to prevent busy waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

/**
 * @brief Send data to the server
 * 
 * Sends the specified data through the socket connection.
 * 
 * @param data The data to send
 * @return true if data was sent successfully
 */
bool NetworkManager::sendData(const std::string& data) {
    std::cout << "[DEBUG] Sending to server: " << data;
    ssize_t sent = send(_socket, data.c_str(), data.length(), 0);
    return sent == static_cast<ssize_t>(data.length());
}

/**
 * @brief Receive data from the server
 * 
 * Receives data from the socket connection and returns it as a string.
 * 
 * @return The received data, or empty string if error
 */
std::string NetworkManager::receiveData() {
    char recvBuffer[4096];
    ssize_t received = recv(_socket, recvBuffer, sizeof(recvBuffer) - 1, 0);
    if (received <= 0) {
        return "";
    }
    recvBuffer[received] = '\0';
    std::string receivedStr(recvBuffer);
    std::cout << "[DEBUG] Received from server: " << receivedStr;
    return receivedStr;
}

/**
 * @brief Process a complete message from the server
 * 
 * This method is called by the network thread when a complete
 * message is received from the server. It forwards the message
 * to the registered callback function.
 * 
 * @param message The message to process
 */
void NetworkManager::processResponse(const std::string& response) {
    std::lock_guard<std::mutex> lock(_callbackMutex);
    if (_responseCallback) {
        _responseCallback(response);
    }
}

/**
 * @brief Set up the socket connection
 * 
 * Creates and configures a TCP socket for connecting to the server.
 * 
 * @param host The server hostname or IP address
 * @param port The server port number
 * @return true if socket was set up successfully
 */
bool NetworkManager::setupSocket(const std::string& host, int port) {
    std::cout << "[DEBUG] Creating socket..." << std::endl;
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0) {
        std::cerr << "Failed to create socket: " << strerror(errno) << std::endl;
        return false;
    }

    // Set non-blocking mode
    int flags = fcntl(_socket, F_GETFL, 0);
    if (flags < 0) {
        std::cerr << "Failed to get socket flags: " << strerror(errno) << std::endl;
        closeSocket();
        return false;
    }
    if (fcntl(_socket, F_SETFL, flags | O_NONBLOCK) < 0) {
        std::cerr << "Failed to set socket to non-blocking mode: " << strerror(errno) << std::endl;
        closeSocket();
        return false;
    }

    std::cout << "[DEBUG] Preparing sockaddr_in for " << host << ":" << port << std::endl;
    struct sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid address: " << host << " (" << strerror(errno) << ")" << std::endl;
        closeSocket();
        return false;
    }

    std::cout << "[DEBUG] Attempting to connect..." << std::endl;
    if (::connect(_socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        if (errno == EINPROGRESS) {
            std::cout << "[DEBUG] Non-blocking connect in progress (EINPROGRESS), treating as success." << std::endl;
            return true;
        }
        std::cerr << "Connection failed: " << strerror(errno) << std::endl;
        closeSocket();
        return false;
    }

    std::cout << "[DEBUG] Socket connected successfully!" << std::endl;
    return true;
}

/**
 * @brief Close the socket connection
 * 
 * Closes the socket file descriptor and resets it to an invalid state
 */
void NetworkManager::closeSocket() {
    if (_socket >= 0) {
        close(_socket);
        _socket = -1;
    }
}

} // namespace clientapi 