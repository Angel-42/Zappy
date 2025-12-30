/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Socket.cpp
*/

#include "Socket.hpp"
#include <stdexcept>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <poll.h>
#include <iostream>

Socket::Socket(const std::string &host, const std::string &port)
{
    addrinfo hints{}, *res;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (::getaddrinfo(host.c_str(), port.c_str(), &hints, &res) != 0)
        throw std::runtime_error("getaddrinfo: " + std::string(std::strerror(errno)));

    fd_ = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (fd_ < 0)
        throw std::runtime_error("socket() failed");

    if (::connect(fd_, res->ai_addr, res->ai_addrlen) < 0)
        throw std::runtime_error("connect() failed");

    ::freeaddrinfo(res);

    // make non-blocking
    int flags = fcntl(fd_, F_GETFL, 0);
    fcntl(fd_, F_SETFL, flags | O_NONBLOCK);
    
    std::cout << "[DEBUG] Socket: Connected to " << host << ":" << port << std::endl;
}

Socket::~Socket()
{
    if (fd_ >= 0)
        ::close(fd_);
}

void Socket::sendLine(const std::string &s) const
{
    std::string line = s + '\n';
    std::cout << "[DEBUG] Socket: SENDING -> " << s << std::endl;
    if (::send(fd_, line.data(), line.size(), MSG_NOSIGNAL) < 0)
        throw std::runtime_error("send failed");
}

std::vector<std::string> Socket::recvLines(int timeout_ms)
{
    std::vector<std::string> out;
    pollfd pfd{fd_, POLLIN, 0};
    int r = ::poll(&pfd, 1, timeout_ms);
    if (r < 0)
        throw std::runtime_error("poll failed");

    if (pfd.revents & POLLIN)
    {
        char buf[4096];
        ssize_t n = ::recv(fd_, buf, sizeof(buf), 0);
        if (n == 0)
            throw std::runtime_error("server closed");
        if (n < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
            throw std::runtime_error("recv error");
        if (n > 0)
        {
            buffer_.append(buf, n);
            std::cout << "[DEBUG] Socket: RECEIVED raw data (" << n << " bytes): ";
            for (int i = 0; i < n; i++) {
                if (buf[i] >= 32 && buf[i] <= 126) {
                    std::cout << buf[i];
                } else if (buf[i] == '\n') {
                    std::cout << "\\n";
                } else if (buf[i] == '\r') {
                    std::cout << "\\r";
                } else {
                    std::cout << "\\x" << std::hex << (int)(unsigned char)buf[i] << std::dec;
                }
            }
            std::cout << std::endl;
        }
    }

    size_t pos;
    while ((pos = buffer_.find('\n')) != std::string::npos)
    {
        std::string line = buffer_.substr(0, pos);
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (!line.empty()) {
            std::cout << "[DEBUG] Socket: PARSED LINE <- " << line << std::endl;
        }
        out.push_back(std::move(line));
        buffer_.erase(0, pos + 1);
    }
    return out;
}
