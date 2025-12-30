#include "ClientAPIWrapper.hpp"
#include <iostream>
#include <string>
#include <csignal>
#include <atomic>
#include <thread>
#include <chrono>
#include <vector>

std::atomic<bool> running(true);
std::atomic<bool> authenticated(false);
std::atomic<bool> graphic_sent(false);

void signal_handler(int) {
    running = false;
}

int main(int argc, char* argv[]) {
    std::string host = "127.0.0.1";
    int port = 4242;
    std::string team = "team1";

    // Simple argument parsing
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-p" && i + 1 < argc) {
            port = std::stoi(argv[++i]);
        } else if (arg == "-h" && i + 1 < argc) {
            host = argv[++i];
        } else if (arg == "-n" && i + 1 < argc) {
            team = argv[++i];
        }
    }

    std::cout << "Connecting to " << host << ":" << port << "..." << std::endl;
    auto& api = clientapi::ClientAPIWrapper::getInstance();
    if (!api.connect(host, port)) {
        std::cerr << "Failed to connect." << std::endl;
        return 1;
    }
    std::cout << "Connected successfully!" << std::endl;

    // Set up info callbacks
    api.setMapSizeCallback([](int width, int height) {
        std::cout << "[CALLBACK] Map size: " << width << "x" << height << std::endl;
    });
    api.setTileContentCallback([](int x, int y, const std::vector<int>& resources) {
        std::cout << "[CALLBACK] Tile (" << x << "," << y << "):";
        for (int r : resources) std::cout << " " << r;
        std::cout << std::endl;
    });
    api.setTeamNamesCallback([](const std::vector<std::string>& teams) {
        std::cout << "[CALLBACK] Teams:";
        for (const auto& t : teams) std::cout << " " << t;
        std::cout << std::endl;
    });

    // Set up a raw message callback for authentication and WELCOME
    api.setRawMessageCallback([&](const std::string& msg) {
        std::cout << "[RAW] Received: '" << msg << "'" << std::endl;
        
        if (msg == "WELCOME" && !authenticated) {
            std::cout << "[AUTH] Got WELCOME, sending GRAPHIC..." << std::endl;
            authenticated = true;
            api.sendGraphicCommand();
            graphic_sent = true;
        } else if (graphic_sent && (msg == "ok" || msg.find("ok") == 0)) {
            std::cout << "[AUTH] Got response to GRAPHIC, sending info requests..." << std::endl;
            graphic_sent = false; // Reset flag
            api.sendMapSizeRequest();
            api.sendMapContentRequest();
            api.sendTeamNamesRequest();
        }
    });

    std::signal(SIGINT, signal_handler); // Handle Ctrl+C

    std::cout << "Press Ctrl+C to exit..." << std::endl;
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    api.disconnect();
    std::cout << "Disconnected. Exiting." << std::endl;
    return 0;
} 