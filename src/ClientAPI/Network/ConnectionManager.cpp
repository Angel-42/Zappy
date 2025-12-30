/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ConnectionManager
*/

#include "ConnectionManager.hpp"
#include "Protocol.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <algorithm>

namespace clientapi {

ConnectionManager::ConnectionManager(int port, const std::vector<std::string>& teamNames, int clientsPerTeam)
    : _serverSocket(-1), _port(port), _teamNames(teamNames), _clientsPerTeam(clientsPerTeam), _running(false)
{
    for (const auto& team : _teamNames) {
        _teamCounts[team] = 0;
    }
}

ConnectionManager::~ConnectionManager() {
    shutdown();
}

bool ConnectionManager::initialize() {
    try {
        setupSocket();
        _running = true;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Failed to initialize connection manager: " << e.what() << std::endl;
        return false;
    }
}

void ConnectionManager::setupSocket() {
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket < 0) {
        throw std::runtime_error("Failed to create socket");
    }

    int opt = 1;
    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        close(_serverSocket);
        throw std::runtime_error("Failed to set socket options");
    }

    int flags = fcntl(_serverSocket, F_GETFL, 0);
    fcntl(_serverSocket, F_SETFL, flags | O_NONBLOCK);

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(_port);

    if (bind(_serverSocket, (struct sockaddr*)&address, sizeof(address)) < 0) {
        close(_serverSocket);
        throw std::runtime_error("Failed to bind socket");
    }

    if (listen(_serverSocket, 10) < 0) {
        close(_serverSocket);
        throw std::runtime_error("Failed to listen on socket");
    }
}

void ConnectionManager::run() {
    fd_set readfds;
    int maxfd;

    while (_running) {
        FD_ZERO(&readfds);
        FD_SET(_serverSocket, &readfds);
        maxfd = _serverSocket;

        for (const auto& [socket, client] : _clients) {
            FD_SET(socket, &readfds);
            maxfd = std::max(maxfd, socket);
        }

        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int activity = select(maxfd + 1, &readfds, nullptr, nullptr, &timeout);
        
        if (activity > 0) {
            processSelectResult(readfds, maxfd);
        }
    }
}

void ConnectionManager::processSelectResult(fd_set& readfds, int maxfd) {
    if (FD_ISSET(_serverSocket, &readfds)) {
        acceptNewConnection();
    }

    std::vector<int> toRemove;
    for (const auto& [socket, client] : _clients) {
        if (FD_ISSET(socket, &readfds)) {
            if (!client->handleIncomingData()) {
                toRemove.push_back(socket);
            }
        }
    }

    for (int socket : toRemove) {
        removeClient(socket);
    }
}

void ConnectionManager::acceptNewConnection() {
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    int newSocket = accept(_serverSocket, (struct sockaddr*)&address, &addrlen);
    
    if (newSocket >= 0) {
        int flags = fcntl(newSocket, F_GETFL, 0);
        fcntl(newSocket, F_SETFL, flags | O_NONBLOCK);

        auto client = std::make_unique<ClientHandler>(newSocket, this);
        client->sendWelcome();
        _clients[newSocket] = std::move(client);
    }
}

void ConnectionManager::removeClient(int socket) {
    auto it = _clients.find(socket);
    if (it != _clients.end()) {
        const std::string& team = it->second->getTeamName();
        if (!team.empty() && _teamCounts.find(team) != _teamCounts.end()) {
            _teamCounts[team]--;
        }
        
        close(socket);
        _clients.erase(it);
    }
}

ClientHandler* ConnectionManager::getClient(int socket) {
    auto it = _clients.find(socket);
    return (it != _clients.end()) ? it->second.get() : nullptr;
}

int ConnectionManager::getAvailableSlots(const std::string& teamName) const {
    auto it = _teamCounts.find(teamName);
    if (it == _teamCounts.end()) {
        return 0;
    }
    return _clientsPerTeam - it->second;
}

bool ConnectionManager::isValidTeam(const std::string& teamName) const {
    return std::find(_teamNames.begin(), _teamNames.end(), teamName) != _teamNames.end();
}

void ConnectionManager::shutdown() {
    _running = false;
    if (_serverSocket >= 0) {
        close(_serverSocket);
        _serverSocket = -1;
    }
    _clients.clear();
}

} // namespace clientapi 