# GUI Protocol Implementation

This document describes the complete GUI protocol implementation for the Zappy project.

## Overview

The GUI protocol implementation provides a complete system for:
- GUI client authentication and registration
- Real-time game state broadcasting
- GUI command processing
- Event-driven game updates

## Architecture

### Core Components

1. **GameManager**: Central game state manager that coordinates all GUI communications
2. **ClientHandler**: Manages individual client connections and protocol handling
3. **Protocol**: Provides constants and utilities for GUI protocol messages
4. **Event Broadcasting System**: Automatically broadcasts game events to all GUI clients

### Class Relationships

```
GameManager
├── Manages GUI client registration
├── Broadcasts game events
└── Provides game state access

ClientHandler
├── Handles GUI authentication
├── Processes GUI commands
└── Sends protocol messages

Protocol
├── Message constants
├── Parsing utilities
└── Formatting utilities
```

## GUI Authentication Flow

1. **Client Connection**: GUI client connects to server
2. **WELCOME Message**: Server sends "WELCOME\n"
3. **Team Authentication**: Client sends "GRAPHIC\n"
4. **Registration**: Server registers client as GUI type
5. **Initial State**: Server sends complete game state
6. **Event Broadcasting**: Server begins broadcasting game events

## Supported GUI Commands

### Information Requests
- `msz` - Request map size
- `bct X Y` - Request tile content
- `mct` - Request all tiles content
- `tna` - Request team names
- `ppo #n` - Request player position
- `plv #n` - Request player level
- `pin #n` - Request player inventory
- `sgt` - Request time unit

### Control Commands
- `sst T` - Modify time unit

## Game Events Broadcasted

### Player Events
- `pnw` - New player connected
- `ppo` - Player position update
- `plv` - Player level update
- `pin` - Player inventory update
- `pex` - Player expulsion
- `pbc` - Player broadcast
- `pdi` - Player death

### Game Events
- `pic` - Incantation start
- `pie` - Incantation end
- `pdr` - Resource drop
- `pgt` - Resource collect
- `enw` - Egg laid
- `eht` - Egg hatching
- `edi` - Egg death
- `seg` - Game end
- `smg` - Server message

## Usage Examples

### Registering a GUI Client

```cpp
GameManager gameManager;

// When a GUI client authenticates
ClientHandler* guiClient = new ClientHandler(socket, connectionManager);
if (guiClient->authenticate("GRAPHIC")) {
    gameManager.registerGUIClient(guiClient);
}
```

### Broadcasting Game Events

```cpp
// When a player moves
gameManager.broadcastPlayerPosition(player);

// When a resource is collected
gameManager.broadcastResourceCollect(player, resourceId);

// When an incantation starts
gameManager.broadcastIncantationStart(x, y, level, playerIds);
```

### Processing GUI Commands

```cpp
// In ClientHandler::processGUICommand
if (cmd == "msz") {
    auto [width, height] = gameManager.getMapSize();
    sendMapSize(width, height);
} else if (cmd == "bct" && parts.size() >= 3) {
    int x = std::stoi(parts[1]);
    int y = std::stoi(parts[2]);
    auto tileContent = gameManager.getTileContent(x, y);
    sendTileContent(x, y, tileContent);
}
```

## Implementation Details

### Message Formatting

All GUI protocol messages follow the format specified in the Zappy documentation:

- Map size: `msz X Y\n`
- Tile content: `bct X Y q0 q1 q2 q3 q4 q5 q6\n`
- Player position: `ppo #n X Y O\n`
- Time unit: `sgt T\n`

### Error Handling

- Invalid commands return `suc\n` (unknown command)
- Bad parameters return `sbp\n` (bad parameter)
- Network errors are handled gracefully

### Thread Safety

- GUI client registration is thread-safe
- Event broadcasting uses proper synchronization
- Command processing is isolated per client

## Integration Points

### With Game Logic

The GUI protocol system integrates with game logic through:

1. **Player Management**: Player events trigger GUI broadcasts
2. **Resource Management**: Resource changes trigger GUI updates
3. **Map Management**: Map changes trigger tile content updates
4. **Team Management**: Team events trigger GUI notifications

### With Network Layer

The system integrates with the network layer through:

1. **ClientHandler**: Manages individual connections
2. **ConnectionManager**: Manages all client connections
3. **NetworkManager**: Handles low-level network operations

## Testing

### Unit Tests

Run the GUI protocol tests:

```bash
make test_gui_protocol
./test_gui_protocol
```

### Integration Tests

Test the complete GUI protocol flow:

```bash
make gui_protocol_example
./gui_protocol_example
```

## Future Enhancements

1. **Performance Optimization**: Implement message batching for multiple events
2. **Compression**: Add message compression for large game states
3. **Caching**: Implement client-side caching for static game data
4. **Metrics**: Add performance metrics and monitoring
5. **Security**: Add authentication and authorization for GUI clients

## Troubleshooting

### Common Issues

1. **GUI client not receiving updates**: Check if client is properly registered
2. **Commands not processed**: Verify command format matches protocol spec
3. **Memory leaks**: Ensure proper cleanup of GUI client registrations
4. **Network timeouts**: Check network configuration and firewall settings

### Debug Mode

Enable debug logging by setting the `DEBUG_GUI_PROTOCOL` environment variable:

```bash
export DEBUG_GUI_PROTOCOL=1
./zappy_server
```

This will log all GUI protocol messages for debugging purposes. 