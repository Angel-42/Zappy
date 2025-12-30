# How to Use the ClientAPI

This guide explains how to use the Zappy ClientAPI to connect to a Zappy server, interact with the game, and handle server events. The API is designed for GUI clients and tools that need to communicate with the Zappy server using the official protocol.

---

## 1. Getting Started

### Include the API

Make sure to include the main API header in your project:

```cpp
#include "ClientAPIWrapper.hpp"
```

---

## 2. Connecting to the Server

Use the singleton `ClientAPIWrapper` to connect to the server:

```cpp
#include "ClientAPIWrapper.hpp"

int main() {
    auto& api = clientapi::ClientAPIWrapper::getInstance();
    if (!api.connect("localhost", 4242)) {
        std::cerr << "Failed to connect." << std::endl;
        return 1;
    }
    // ...
}
```

---

## 3. Identifying as a GUI Client

After connecting, send the GRAPHIC command to identify as a GUI client:

```cpp
api.sendGraphicCommand(); // Sends "GRAPHIC\n" to the server
```

---

## 4. Registering Event Callbacks

The API uses callbacks to notify your application of server events. Register your handlers before or after connecting:

```cpp
api.setMapSizeCallback([](int width, int height) {
    std::cout << "Map size: " << width << "x" << height << std::endl;
});

api.setTileContentCallback([](int x, int y, const std::vector<int>& resources) {
    std::cout << "Tile (" << x << "," << y << ") resources:";
    for (int r : resources) std::cout << " " << r;
    std::cout << std::endl;
});

api.setTeamNamesCallback([](const std::vector<std::string>& teams) {
    std::cout << "Teams:";
    for (const auto& t : teams) std::cout << " " << t;
    std::cout << std::endl;
});
```

A full list of available callbacks can be found in `ClientAPIWrapper.hpp` (see the section below for a detailed list).

---

## 5. Command Reference: Every API Command

Below is a detailed reference for every command you can send with the API, including the protocol message, what it does, and how to use it.

### `sendMapSizeRequest()`
- **Protocol:** `msz\n`
- **Description:** Requests the map size from the server.
- **Callback:** `setMapSizeCallback`
- **Example:**
  ```cpp
  api.sendMapSizeRequest();
  // Triggers setMapSizeCallback(int width, int height)
  ```

### `sendTileContentRequest(int x, int y)`
- **Protocol:** `bct X Y\n`
- **Description:** Requests the content of the tile at (x, y).
- **Callback:** `setTileContentCallback`
- **Example:**
  ```cpp
  api.sendTileContentRequest(3, 5);
  // Triggers setTileContentCallback(int x, int y, const std::vector<int>& resources)
  ```

### `sendMapContentRequest()`
- **Protocol:** `mct\n`
- **Description:** Requests the content of all tiles on the map.
- **Callback:** `setTileContentCallback` (called for each tile)
- **Example:**
  ```cpp
  api.sendMapContentRequest();
  // Triggers setTileContentCallback for every tile
  ```

### `sendTeamNamesRequest()`
- **Protocol:** `tna\n`
- **Description:** Requests the list of all team names.
- **Callback:** `setTeamNamesCallback`
- **Example:**
  ```cpp
  api.sendTeamNamesRequest();
  // Triggers setTeamNamesCallback(const std::vector<std::string>& teams)
  ```

### `sendPlayerPositionRequest(int playerId)`
- **Protocol:** `ppo #id\n`
- **Description:** Requests the position of a specific player.
- **Callback:** `setPlayerPositionCallback`
- **Example:**
  ```cpp
  api.sendPlayerPositionRequest(42);
  // Triggers setPlayerPositionCallback(int id, int x, int y, int orientation)
  ```

### `sendPlayerLevelRequest(int playerId)`
- **Protocol:** `plv #id\n`
- **Description:** Requests the level of a specific player.
- **Callback:** `setPlayerLevelCallback`
- **Example:**
  ```cpp
  api.sendPlayerLevelRequest(42);
  // Triggers setPlayerLevelCallback(int id, int level)
  ```

### `sendPlayerInventoryRequest(int playerId)`
- **Protocol:** `pin #id\n`
- **Description:** Requests the inventory of a specific player.
- **Callback:** `setPlayerInventoryCallback`
- **Example:**
  ```cpp
  api.sendPlayerInventoryRequest(42);
  // Triggers setPlayerInventoryCallback(int id, int x, int y, const std::array<int, ResourceType::COUNT>& inventory)
  ```

### `sendTimeUnitRequest()`
- **Protocol:** `sgt\n`
- **Description:** Requests the current time unit (game tick duration).
- **Callback:** `setTimeUnitCallback`
- **Example:**
  ```cpp
  api.sendTimeUnitRequest();
  // Triggers setTimeUnitCallback(int timeUnit)
  ```

### `sendTimeUnitModification(int timeUnit)`
- **Protocol:** `sst T\n`
- **Description:** Requests to change the time unit (admin only).
- **Callback:** `setTimeUnitCallback` (if successful)
- **Example:**
  ```cpp
  api.sendTimeUnitModification(100);
  // Triggers setTimeUnitCallback(int timeUnit) if allowed
  ```

### `sendGraphicCommand()`
- **Protocol:** `GRAPHIC\n`
- **Description:** Identifies the client as a GUI client. Must be sent after connecting.
- **Callback:** Triggers initial state callbacks (map size, tiles, teams, etc.)
- **Example:**
  ```cpp
  api.sendGraphicCommand();
  // Triggers all initial state callbacks
  ```

---

## 6. Callback/Event Reference

Here are all the callbacks you can register, what triggers them, and what data they provide:

- **Map and Tiles**
  - `setMapSizeCallback(void(int width, int height))` — Map size received (`msz`)
  - `setTileContentCallback(void(int x, int y, const std::vector<int>& resources))` — Tile content received (`bct`)
  - `setTeamNamesCallback(void(const std::vector<std::string>& teams))` — Team names received (`tna`)

- **Players**
  - `setPlayerNewCallback(void(int id, int x, int y, int orientation, int level, const std::string& team))` — New player joined (`pnw`)
  - `setPlayerPositionCallback(void(int id, int x, int y, int orientation))` — Player position update (`ppo`)
  - `setPlayerLevelCallback(void(int id, int level))` — Player level update (`plv`)
  - `setPlayerInventoryCallback(void(int id, int x, int y, const std::array<int, ResourceType::COUNT>& inventory))` — Player inventory update (`pin`)
  - `setPlayerExpulsionCallback(void(int id))` — Player expulsion (`pex`)
  - `setPlayerBroadcastCallback(void(int id, const std::string& message))` — Player broadcast message (`pbc`)
  - `setPlayerDeathCallback(void(int id))` — Player death (`pdi`)

- **Incantations**
  - `setIncantationStartCallback(void(int x, int y, int level, const std::vector<int>& players))` — Incantation started (`pic`)
  - `setIncantationEndCallback(void(int x, int y, int result))` — Incantation ended (`pie`)

- **Resources**
  - `setResourceDropCallback(void(int playerId, int resourceId))` — Resource dropped (`pdr`)
  - `setResourceCollectCallback(void(int playerId, int resourceId))` — Resource collected (`pgt`)

- **Eggs**
  - `setEggLaidCallback(void(int eggId, int playerId, int x, int y))` — Egg laid (`enw`)
  - `setEggHatchingCallback(void(int eggId))` — Egg hatching (`ebo`)
  - `setEggDeathCallback(void(int eggId))` — Egg death (`edi`)

- **Game State**
  - `setTimeUnitCallback(void(int timeUnit))` — Time unit update (`sgt`)
  - `setGameEndCallback(void(const std::string& winningTeam))` — Game end (`seg`)
  - `setServerMessageCallback(void(const std::string& message))` — Server message (`smg`)
  - `setRawMessageCallback(void(const std::string& message))` — Any raw message

---

## 7. Accessing Game State

The API maintains a local game state you can query at any time:

```cpp
clientapi::GameState& state = api.getGameState();

// Example: Get a player by ID
if (auto* player = state.getPlayer(playerId)) {
    std::cout << "Player position: " << player->getPosition().x << "," << player->getPosition().y << std::endl;
}

// Example: Get the map
if (auto* map = state.getMap()) {
    if (auto* tile = map->getTile(x, y)) {
        std::cout << "Tile has " << tile->getPlayers().size() << " players." << std::endl;
    }
}
```

---

## 8. Example: Minimal Client

```cpp
#include "ClientAPIWrapper.hpp"
#include <iostream>

int main() {
    auto& api = clientapi::ClientAPIWrapper::getInstance();
    if (!api.connect("127.0.0.1", 4242)) {
        std::cerr << "Failed to connect." << std::endl;
        return 1;
    }
    api.sendGraphicCommand();
    api.setMapSizeCallback([](int w, int h) {
        std::cout << "Map: " << w << "x" << h << std::endl;
    });
    api.sendMapSizeRequest();
    // ... main loop, event handling, etc.
    return 0;
}
```

---

## 9. Advanced: GameManager and Server Integration

If you are developing the server or need to broadcast events to GUI clients, use the `GameManager`:

```cpp
#include "GameManager.hpp"

// Broadcast a custom event to all GUI clients
auto& gm = clientapi::GameManager::getInstance();
gm.broadcastToGUI("custom_event ...");
```

---

## 10. Tips
- Always check connection status with `api.isConnected()`.
- Register all needed callbacks before sending requests.
- Use the local game state for fast access to the latest known data.
- See `test_main.cpp` for a more complete example.

---

## 11. References
- Main API: `ClientAPIWrapper.hpp`
- Game State Models: `Model/`
- Server-side: `GameManager.hpp`, `Network/`

---

Happy coding! 