/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Map system header
*/

#ifndef MAP_H_
    #define MAP_H_

    #include "game_state.h"

bool init_map(game_state_t *game_state);
bool set_map_dimensions(game_state_t *game_state, int width, int height);
bool update_map(game_state_t *game_state);
bool spawn_resources(game_state_t *game_state);
map_t *get_map(game_state_t *game_state);
tile_t *get_tile(map_t *map, int x, int y);
bool is_valid_position(map_t *map, int x, int y);
void destroy_map(map_t *map);

void spawn_all_resources(game_state_t *game_state, int *positions,
    int map_area, resource_additions_t *additions);

/* Tile management */
void add_player_to_tile(tile_t *tile, player_t *player);
void remove_player_from_tile(tile_t *tile, player_t *player);
void add_egg_to_tile(tile_t *tile, egg_t *egg);
void remove_egg_from_tile(tile_t *tile, egg_t *egg);

#endif /* MAP_H_ */
