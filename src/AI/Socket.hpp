/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Socket.hpp
*/

#pragma once
#include <string>
#include <vector>

/** Tiny RAII TCP client with non-blocking reads split into lines. */
class Socket
{
public:
    Socket(const std::string &host, const std::string &port);
    ~Socket();

    /** Blocking send of one line (adds '\n'). */
    void sendLine(const std::string &s) const;

    /** Poll, read all available, return complete lines. */
    std::vector<std::string> recvLines(int timeout_ms = 0);

private:
    int fd_;
    std::string buffer_;
};
