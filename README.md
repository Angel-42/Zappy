# Zappy

> Developed as part of an Epitech project by [Angel-42](https://github.com/Angel-42), [Skynoxvr](https://github.com/Skynoxvr), [Mattaiuss](https://github.com/Mattaiuss), [Robin-Toillon](https://github.com/Robin-Toillon), [Enichy](https://github.com/Enichy), and [ThibaultLafont](https://github.com/ThibaultLafont).

## Overview

**Zappy** is a multiplayer real-time strategy (RTS) game where teams compete to dominate a dynamic world. Players must gather resources, cooperate, and outsmart opponents to achieve victory. The project features a custom server, a graphical client, and an AI client.


## Project Videos
> The embedded videos below use compressed versions for faster playback. Full-quality originals are available for download: [Zappy 2D](./screenshots/zappy2D_ori.mp4), [Zappy 3D](./screenshots/zappy3D_ori.mp4).

[![Zappy 2D demo](./screenshots/zappy2D_thumb.png)](./screenshots/zappy2D_small.mp4)

[![Zappy 3D demo](./screenshots/zappy3D_thumb.png)](./screenshots/zappy3D_small.mp4)


## Features

- Multiplayer RTS gameplay
- Custom server in C
- Graphical client in C++ (2D & 3D)
- Autonomous AI client (any language)
- Resource management and team strategy
- Real-time communication protocol
- 2D/3D visualization (SFML recommended for 2D)

## Getting Started

### Prerequisites
- C compiler (for server)
- C++ compiler (for GUI)
- Make
- SFML (for GUI)

### Build

```sh
make
```

### Run

```sh
./zappy_server -p <port> -x <width> -y <height> -n <team1> <team2> ... -c <clientsNb> -f <freq>
./zappy_gui -p <port> -h <server_host>
./zappy_ai -p <port> -n <team> -h <server_host>
```

## Documentation

See [docs/Zappy.md](docs/Zappy.md) for the full project documentation and protocol details.

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

---

**This project was realized as part of the Epitech curriculum.**

---

### Authors

- [Angel-42](https://github.com/Angel-42)
- [Skynoxvr](https://github.com/Skynoxvr)
- [Mattaiuss](https://github.com/Mattaiuss)
- [Robin-Toillon](https://github.com/Robin-Toillon)
- [Enichy](https://github.com/Enichy)
- [ThibaultLafont](https://github.com/ThibaultLafont)
