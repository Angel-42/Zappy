/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** CommandQueue.hpp
*/

#pragma once
#include <deque>
#include <unordered_map>
#include <string>
#include <chrono>
#include <iostream>

/** 10-pending FIFO with per-type cooldowns derived from frequency. */
class CommandQueue
{
public:
    void setFrequency(int f);

    bool push(const std::string &cmd);

    /** Returns next cmd ready to send or nullptr. */
    const std::string *popReady();

    void ack();

    void clear();

    size_t size() const;
    bool empty() const;

    const std::string &peekAt(int index) const;
    void replaceAt(int index, const std::string &cmd);
    void pushFront(const std::string &cmd);

private:
    static double now();
    static std::string token(const std::string &cmd);
    double costSec(const std::string &t) const;
    bool cooldownOk(const std::string &cmd);

    int freq_ = 100;
    std::deque<std::string> queue_;
    std::unordered_map<std::string, double> lastType_;
    double lastAny_ = 0;
    std::string ready_;
};
