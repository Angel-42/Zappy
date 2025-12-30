/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** CommandQueue.cpp
*/

#include "CommandQueue.hpp"
#include <iostream>

void CommandQueue::setFrequency(int f)
{
    freq_ = f > 0 ? f : 100;
}

bool CommandQueue::push(const std::string &cmd)
{
    if (queue_.size() >= 10 || !cooldownOk(cmd)) {
        if (!cooldownOk(cmd)) {
            std::cout << "[DEBUG] CommandQueue: Command rejected due to cooldown: " << cmd << std::endl;
        } else {
            std::cout << "[DEBUG] CommandQueue: Command rejected due to full queue: " << cmd << std::endl;
        }
        return false;
    }
    queue_.push_back(cmd);
    lastType_[token(cmd)] = now();
    std::cout << "[DEBUG] CommandQueue: Pushed command: " << cmd << " (queue size: " << queue_.size() << ")" << std::endl;
    return true;
}

const std::string *CommandQueue::popReady()
{
    if (queue_.empty()) {
        return nullptr;
    }
    if (now() - lastAny_ < costSec(token(queue_.front()))) {
        std::cout << "[DEBUG] CommandQueue: Command not ready yet due to cooldown: " << queue_.front() << std::endl;
        return nullptr;
    }
    ready_ = queue_.front();
    queue_.pop_front();
    lastAny_ = now();
    std::cout << "[DEBUG] CommandQueue: Popped ready command: " << ready_ << " (remaining: " << queue_.size() << ")" << std::endl;
    return &ready_;
}

void CommandQueue::ack()
{
    // server 'ok' already popped via popReady()
    std::cout << "[DEBUG] CommandQueue: Command acknowledged by server" << std::endl;
}

void CommandQueue::clear()
{
    std::cout << "[DEBUG] CommandQueue: Clearing entire queue (" << queue_.size() << " commands)" << std::endl;
    queue_.clear();
    lastType_.clear();
    lastAny_ = 0;
    ready_.clear();
}

size_t CommandQueue::size() const
{
    return queue_.size();
}

bool CommandQueue::empty() const
{
    return queue_.empty();
}

double CommandQueue::now()
{
    using namespace std::chrono;
    return duration<double>(steady_clock::now().time_since_epoch()).count();
}

std::string CommandQueue::token(const std::string &cmd)
{
    return cmd.substr(0, cmd.find(' '));
}

double CommandQueue::costSec(const std::string &t) const
{
    static const std::unordered_map<std::string, int> cost{
        {"Forward", 7}, {"Left", 7}, {"Right", 7}, {"Look", 7}, {"Inventory", 1}, {"Take", 7}, {"Set", 7}, {"Broadcast", 7}, {"Incantation", 300}, {"Fork", 42}, {"Eject", 7}, {"Connect_nbr", 0}, {"sgt", 0}};
    auto it = cost.find(t);
    return (it == cost.end() ? 0 : it->second) / double(freq_);
}

bool CommandQueue::cooldownOk(const std::string &cmd)
{
    auto t = token(cmd);
    if (t == "Take" || t == "Inventory")
        return true;
    auto it = lastType_.find(t);
    return it == lastType_.end() || now() - it->second >= costSec(t);
}

void CommandQueue::replaceAt(int index, const std::string &cmd)
{
    if (index < 0 || index >= static_cast<int>(queue_.size()))
        return;
    queue_[index] = cmd;
    lastType_[token(cmd)] = now();
}

void CommandQueue::pushFront(const std::string &cmd)
{
    if (queue_.size() >= 10 || !cooldownOk(cmd)) {
        std::cout << "[DEBUG] CommandQueue: Cannot pushFront command: " << cmd << " (queue full or cooldown)" << std::endl;
        return;
    }
    queue_.push_front(cmd);
    lastType_[token(cmd)] = now();
    std::cout << "[DEBUG] CommandQueue: Pushed command to front: " << cmd << " (queue size: " << queue_.size() << ")" << std::endl;
}

const std::string &CommandQueue::peekAt(int index) const
{
    if (index < 0 || index >= static_cast<int>(queue_.size()))
        throw std::out_of_range("Index out of range in CommandQueue::peekAt");
    return queue_[index];
}
