# ZAPPY

## A TRIBUTE TO ZAPHOD BEEBLEBROX


**ZAPPY**

The winning team is the first one where at least 6 players reach the maximum elevation.
**1. Geography**
**2. Resources**
These stones have six distinct categories, as follows:

**3. Activities**

Trantor’s inhabitants take care of two things:

For various reasons, the players’ field of vision is limited.
The AI client is autonomous.
After it’s launched the user has no further influence on how it functions.
```
**2. Teams**

```
```
start incantation Incantation 300/f Elevation underway
Current level:k

Once it’s layed, the player who has layed it can take care of their business.
The selected egg then hatches and the newly spawned player starts with a random direction.
---
title: Zappy - Project Documentation
---

# Zappy

> This project was developed as part of an Epitech curriculum project by [Angel-42](https://github.com/Angel-42), [Skynoxvr](https://github.com/Skynoxvr), [Mattaiuss](https://github.com/Mattaiuss), [Robin-Toillon](https://github.com/Robin-Toillon), [Enichy](https://github.com/Enichy), and [ThibaultLafont](https://github.com/ThibaultLafont).

## Overview

**Zappy** is a multiplayer real-time strategy (RTS) game where several teams compete on a resource-filled map. The first team to have at least 6 players reach the maximum elevation wins. The project consists of three main binaries:

- `zappy_server` (C): The game server
- `zappy_gui` (C++): The graphical client
- `zappy_ai` (any language): The AI client

The Makefile provides rules to build all three binaries. The C standard library is available for use.

## Game World

### Geography
The world, called **Trantor**, is a flat map with no relief. The board wraps around: exiting one edge brings you to the opposite side.

### Resources
Players can find food and six types of stones:

- linemate
- deraumere
- sibur
- mendiane
- phiras
- thystame

Resources spawn at game start and every 20 time units. At least one of each resource and food must be present on the map. The quantity is determined by `map_width * map_height * density`.

### Activities
Players must eat and collect stones to survive and progress. Elevation (leveling up) is a key activity.

### Individuals
Players are non-violent, occupy a single tile, and can meet others on the same tile. Food is essential for survival (1 food = 126 time units).

### Elevation Ritual
To level up, players must gather specific resources and a certain number of players of the same level on the same tile. The requirements increase with each level. If conditions are not met, the ritual fails. All participants are frozen during the ritual.

### Vision
Vision increases with each elevation. The `look` command returns visible objects. Multiple objects on a tile are space-separated.

### Sound Transmission
Broadcasts are heard by all players, with the direction indicated by a tile number. The shortest path is always chosen.

## Programs

### Binaries
You must provide:
- A server (`zappy_server`, C)
- A graphical client (`zappy_gui`, C++)
- An AI client (`zappy_ai`, any language)

#### Example Usage
```sh
./zappy_server -p port -x width -y height -n name1 name2 ... -c clientsNb -f freq
./zappy_gui -p port -h machine
./zappy_ai -p port -n name -h machine
```

### Teams
Teams start with a number of available slots (eggs). Players cannot communicate outside the game. Each player starts with 10 life units (1260 time units).

### Commands
Players can:
- Move (`Forward`, `Right`, `Left`)
- Look around (`Look`)
- Check inventory (`Inventory`)
- Broadcast messages (`Broadcast`)
- Fork (create an egg)
- Eject others (`Eject`)
- Take/set objects
- Start incantation (`Incantation`)

All commands are newline-terminated. Unknown commands return `ko`.

### Client/Server Communication
Communication is via TCP sockets. The client can send up to 10 requests without waiting for a response. The server executes requests in order.

### Time
Actions take time based on the frequency parameter `f`. For example, `Forward` takes `7/f` seconds.

### Object Management
Objects of the same class are indistinguishable.

### Player Reproduction
The `fork` command creates an egg, allowing a new client to connect. The `connect_nbr` command returns the number of available slots.

### Egg Hatching
When a client connects, a random egg hatches and the player starts with a random direction.

### Inventory
The `Inventory` command shows the player's items and remaining time.

### Broadcast
The `Broadcast` command sends a message to all clients, indicating the direction.

### Ejection
The `Eject` command pushes other players from the tile in the direction the player is facing. Eggs on the tile are destroyed.

### Graphical User Interface
The project must include a graphical visualization (at least 2D, 3D is a plus). The GUI authenticates by sending `GRAPHIC` as the team name. SFML is recommended for 2D rendering.

---
This documentation is a summary. For protocol details, see the protocol documentation in the `docs/` folder.

---
**Project realized as part of the Epitech curriculum.**
In order for a player to recognize their team, the client sends the **look** command. The server will

respond with the character string, as follows.

