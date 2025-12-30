/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Broadcast system header for GUI clients
*/

#ifndef BROADCAST_H_
    #define BROADCAST_H_

    #include <stdbool.h>
    #include "game_state.h"

// Forward declarations
typedef struct game_state_s game_state_t;
typedef struct player_s player_t;
typedef struct egg_s egg_t;
typedef struct broadcast_incantation_s broadcast_incantation_t;

typedef struct broadcast_context_s {
    broadcast_system_t *broadcast_system;
    game_state_t *game_state;
    int socket;
} broadcast_context_t;

// Core broadcast system functions
bool init_broadcast_system(broadcast_system_t *broadcast_system,
    game_state_t *game_state);
void cleanup_broadcast_system(broadcast_system_t *broadcast_system,
    game_state_t *game_state);
bool register_gui_client(broadcast_system_t *broadcast_system,
    game_state_t *game_state, int socket);
void unregister_gui_client(broadcast_system_t *broadcast_system,
    game_state_t *game_state, int socket);
bool broadcast_to_gui(broadcast_system_t *broadcast_system,
    game_state_t *game_state, const char *message);
bool send_to_gui_client(broadcast_system_t *broadcast_system,
    game_state_t *game_state, int socket, const char *message);

// Player-to-player broadcast
void broadcast_message(broadcast_system_t *broadcast_system,
    game_state_t *game_state, player_t *sender, const char *message);
int calculate_broadcast_direction(game_state_t *game_state,
    player_t *sender, player_t *receiver);

// Map broadcast functions
bool broadcast_map_size(broadcast_system_t *broadcast_system,
    game_state_t *game_state);
bool broadcast_tile_content(broadcast_system_t *broadcast_system,
    game_state_t *game_state, int x, int y);
bool broadcast_all_tiles(broadcast_system_t *broadcast_system,
    game_state_t *game_state);
bool broadcast_team_names(broadcast_system_t *broadcast_system,
    game_state_t *game_state);

// Player broadcast functions
bool broadcast_player_new(broadcast_system_t *broadcast_system,
    game_state_t *game_state, player_t *player);
bool broadcast_player_position(broadcast_system_t *broadcast_system,
    game_state_t *game_state, player_t *player);
bool broadcast_player_level(broadcast_system_t *broadcast_system,
    game_state_t *game_state, player_t *player);
bool broadcast_player_inventory(broadcast_system_t *broadcast_system,
    game_state_t *game_state, player_t *player);
bool broadcast_player_death(broadcast_system_t *broadcast_system,
    game_state_t *game_state, player_t *player);
bool broadcast_player_expulsion(broadcast_system_t *broadcast_system,
    game_state_t *game_state, player_t *player);
bool broadcast_player_broadcast(broadcast_system_t *broadcast_system,
    game_state_t *game_state, player_t *player, const char *message);

// Event broadcast functions
bool broadcast_incantation_start(broadcast_incantation_t *incantation);
bool broadcast_incantation_end(broadcast_incantation_t *incantation);
bool broadcast_resource_drop(broadcast_system_t *broadcast_system,
    game_state_t *game_state, player_t *player, int resource_id);
bool broadcast_resource_collect(broadcast_system_t *broadcast_system,
    game_state_t *game_state, player_t *player, int resource_id);
bool broadcast_egg_laid(broadcast_system_t *broadcast_system,
    game_state_t *game_state, egg_t *egg);
bool broadcast_egg_hatching(broadcast_system_t *broadcast_system,
    game_state_t *game_state, egg_t *egg);
bool broadcast_egg_death(broadcast_system_t *broadcast_system,
    game_state_t *game_state, egg_t *egg);
bool broadcast_time_unit(broadcast_system_t *broadcast_system,
    game_state_t *game_state);
bool broadcast_game_end(broadcast_system_t *broadcast_system,
    game_state_t *game_state, const char *winning_team);
bool broadcast_server_message(broadcast_system_t *broadcast_system,
    game_state_t *game_state, const char *message);

// Initialization functions
bool send_initial_game_state(broadcast_system_t *broadcast_system,
    game_state_t *game_state, int socket);


int send_to_connected_clients(broadcast_system_t *broadcast_system,
    const char *message);

#endif /* BROADCAST_H_ */
