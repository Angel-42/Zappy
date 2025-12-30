/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Game state management header
*/

/*
** This header defines the complete game state structure and all related data
** for the Zappy game server. It includes resource management, player tracking,
** team organization, map representation, and game mechanics.
*/

#ifndef GAME_STATE_H_
    #define GAME_STATE_H_

    /**
     * @brief Resource density constants for map generation
     *
     * These values determine the probability of each resource type appearing
     * on map tiles during initial generation and respawning.
     */
    #define FOOD_DENSITY 0.5        /**< 50% chance of food per tile */
    #define LINEMATE_DENSITY 0.3    /**< 30% chance of linemate per tile */
    #define DERAUMERE_DENSITY 0.15  /**< 15% chance of deraumere per tile */
    #define SIBUR_DENSITY 0.1       /**< 10% chance of sibur per tile */
    #define MENDIANE_DENSITY 0.1    /**< 10% chance of mendiane per tile */
    #define PHIRAS_DENSITY 0.08     /**< 8% chance of phiras per tile */
    #define THYSTAME_DENSITY 0.05   /**< 5% chance of thystame per tile */

    #include <stdbool.h>
    #include <stdint.h>

// Forward declarations
typedef struct player_s player_t;
typedef struct team_s team_t;
typedef struct map_s map_t;
typedef struct egg_s egg_t;
typedef struct incantation_s incantation_t;

/**
 * @brief Resource types available in the game
 *
 * Each resource type has different properties and uses in the game.
 * Resources are used for player survival (food) and team elevation (minerals).
 */
typedef enum {
    FOOD = 0,       /**< Food - consumed by players to survive */
    LINEMATE = 1,   /**< Linemate - mineral required for elevation */
    DERAUMERE = 2,  /**< Deraumere - mineral required for elevation */
    SIBUR = 3,      /**< Sibur - mineral required for elevation */
    MENDIANE = 4,   /**< Mendiane - mineral required for elevation */
    PHIRAS = 5,     /**< Phiras - mineral required for elevation */
    THYSTAME = 6,   /**< Thystame - mineral required for elevation */
    RESOURCE_COUNT  /**< Total number of resource types */
} resource_type_t;

/**
 * @brief Player movement directions
 *
 * Players can face one of four cardinal directions on the map.
 * Direction affects player movement, vision, and resource collection.
 */
typedef enum {
    NORTH = 0,  /**< Player facing north (up on map) */
    EAST = 1,   /**< Player facing east (right on map) */
    SOUTH = 2,  /**< Player facing south (down on map) */
    WEST = 3    /**< Player facing west (left on map) */
} direction_t;

/**
 * @brief Maximum resource limits per tile
 *
 * This structure defines the maximum number of each resource type
 * that can be present on a single map tile at any time.
 */
typedef struct {
    int max_food;       /**< Maximum food items per tile */
    int max_linemate;   /**< Maximum linemate items per tile */
    int max_deraumere;  /**< Maximum deraumere items per tile */
    int max_sibur;      /**< Maximum sibur items per tile */
    int max_mendiane;   /**< Maximum mendiane items per tile */
    int max_phiras;     /**< Maximum phiras items per tile */
    int max_thystame;   /**< Maximum thystame items per tile */
} resource_limits_t;

/**
 * @brief Current resource counts on a tile or in inventory
 *
 * This structure tracks the actual number of each resource type
 * present in a specific location (tile, player inventory, etc.).
 */
typedef struct {
    int food;       /**< Number of food items */
    int linemate;   /**< Number of linemate items */
    int deraumere;  /**< Number of deraumere items */
    int sibur;      /**< Number of sibur items */
    int mendiane;   /**< Number of mendiane items */
    int phiras;     /**< Number of phiras items */
    int thystame;   /**< Number of thystame items */
} resource_counts_t;

/**
 * @brief Resource additions for spawning or collection
 *
 * This structure specifies how many resources to add to a location
 * during spawning, collection, or other resource operations.
 */
typedef struct {
    int food;       /**< Food items to add */
    int linemate;   /**< Linemate items to add */
    int deraumere;  /**< Deraumere items to add */
    int sibur;      /**< Sibur items to add */
    int mendiane;   /**< Mendiane items to add */
    int phiras;     /**< Phiras items to add */
    int thystame;   /**< Thystame items to add */
} resource_additions_t;

/**
 * @brief Requirements for player elevation
 *
 * This structure defines the exact resources and player count needed
 * for a team to successfully complete an elevation ritual.
 */
typedef struct {
    int players;    /**< Number of players required for elevation */
    int linemate;   /**< Linemate items required */
    int deraumere;  /**< Deraumere items required */
    int sibur;      /**< Sibur items required */
    int mendiane;   /**< Mendiane items required */
    int phiras;     /**< Phiras items required */
    int thystame;   /**< Thystame items required */
} elevation_requirements_t;

/**
 * @brief Incantation ritual structure
 *
 * Represents an active elevation ritual being performed by a team.
 * Incantations require multiple players and specific resources to succeed.
 */
typedef struct incantation_s {
    int id;
    int x;
    int y;
    int level;
    int player_count;
    player_t **players;
    int start_time;
    bool is_active;
} incantation_t;

/**
 * @brief Player structure representing a game participant
 *
 * Contains all information about a single player including position,
 * inventory, status, and game statistics.
 */
typedef struct player_s {
    int id;
    char *team_name;
    int x;
    int y;
    direction_t direction;
    int level;
    int food;
    int life_units;
    int socket;
    bool is_alive;
    bool is_incantating;
    int action_end_time;
    int inventory[RESOURCE_COUNT];
    int last_food_time;
} player_t;

/**
 * @brief Team structure representing a group of players
 *
 * Teams compete against each other in the game. Each team has a name,
 * maximum player limit, and tracks all its members and eggs.
 */
typedef struct team_s {
    char *name;
    int player_count;
    int max_players;
    player_t **players;
    int eggs_count;
} team_t;

/**
 * @brief Map tile structure representing a single location
 *
 * Each tile contains resources, players, and eggs present at that location.
 * Tiles are the fundamental units of the game map.
 */
typedef struct {
    int x;
    int y;
    int resources[RESOURCE_COUNT];
    player_t **players_on_tile;
    int player_count;
    egg_t **eggs_on_tile;
    int egg_count;
} tile_t;

/**
 * @brief Map structure representing the game world
 *
 * The map is a 2D grid of tiles where all game activity occurs.
 * It contains all resources, players, and eggs in the game world.
 */
typedef struct map_s {
    int width;
    int height;
    tile_t *tiles;
} map_t;

/**
 * @brief Egg structure representing a player spawn point
 *
 * Eggs are laid by players and hatch into new players after a time delay.
 * They provide a way for teams to increase their player count.
 */
typedef struct egg_s {
    int id;
    int player_id;
    int x;
    int y;
    bool is_hatched;
    char *team_name;
    int hatch_time;
} egg_t;

/**
 * @brief GUI client structure for spectator connections
 *
 * GUI clients can observe the game but cannot participate directly.
 * They receive broadcast updates about game events.
 */
typedef struct gui_client_s {
    int socket;
    bool is_connected;
} gui_client_t;

/**
 * @brief Broadcast system for GUI client communication
 *
 * Manages the list of GUI clients that receive game event broadcasts.
 * Handles client connections, disconnections, and message distribution.
 */
typedef struct broadcast_system_s {
    gui_client_t *clients;
    int client_count;
    int max_clients;
} broadcast_system_t;

// Include broadcast system before game_state_t definition
    #include "broadcast.h"

/**
 * @brief Main game state structure containing all game data
 *
 * This is the central structure that holds all game information including
 * the map, teams, players, eggs, incantations, and game settings.
 * It serves as the single source of truth for the entire game state.
 */
typedef struct game_state_s {
    map_t *map;
    team_t **teams;
    int team_count;
    player_t **players;
    int player_count;
    egg_t **eggs;
    int egg_count;
    incantation_t **incantations;
    int incantation_count;
    bool is_running;
    bool is_paused;
    int tick_rate;
    int current_tick;
    int next_player_id;
    int next_egg_id;
    int next_incantation_id;
    int frequency;
    broadcast_system_t broadcast_system;
} game_state_t;


// Resource spawn context structure
typedef struct {
    game_state_t *game_state;
    int *positions;
    int map_area;
    int count;
    int offset;
    resource_type_t type;
} resource_spawn_context_t;

typedef struct resource_with_broadcast_s {
    struct game_state_s *game_state;
    int x;
    int y;
    resource_type_t type;
    int amount;
} resource_with_broadcast_t;

// Incantation broadcast structure
typedef struct broadcast_incantation_s {
    broadcast_system_t *broadcast_system;
    game_state_t *game_state;
    int x;
    int y;
    int level;
    int *player_ids;
    int player_count;
    int result;
} broadcast_incantation_t;


// Include all other game state module headers
    #include "core.h"
    #include "map.h"
    #include "player.h"
    #include "team.h"
    #include "egg.h"
    #include "incantation.h"
    #include "vision.h"
    #include "eject.h"
    #include "resource.h"
    #include "time.h"

#endif /* !GAME_STATE_H_ */
