/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Data structures header - Server configuration and team management
*/

/*
** This header defines the core data structures used for server configuration
** and team management. These structures hold the parameters needed to
** initialize and run the Zappy server.
*/

#ifndef DATA_H_
    #define DATA_H_

    #include <stdint.h>

/**
 * @brief Team configuration structure
 *
 * This structure holds information about the teams participating in the game.
 * It contains an array of team names and the total number of teams.
 * Teams are the primary organizational unit for players in the game.
 */
typedef struct teams_s {
    char **names;    /**< Array of team name strings */
    uint64_t count;  /**< Number of teams in the array */
} teams_t;

/**
 * @brief Server configuration structure
 *
 * This structure contains all the configuration parameters needed to
 * initialize and run the Zappy server. It includes map dimensions,
 * team information, client limits, and network settings.
 */
typedef struct server_data_s {
    uint64_t width;      /**< Map width in tiles */
    uint64_t height;     /**< Map height in tiles */
    teams_t teams;       /**< Team configuration */
    uint64_t max_clients; /**< Maximum clients per team */
    uint64_t frequency;  /**< Game frequency (ticks per second) */
    uint16_t port;       /**< Server port number */
} server_data_t;

#endif /* !DATA_H_ */
