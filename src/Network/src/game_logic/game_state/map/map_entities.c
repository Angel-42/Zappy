/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Map entity management (players and eggs)
*/

#include "map.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "game_state.h"

static bool allocate_player_memory(tile_t *tile)
{
    player_t **new_players = realloc(tile->players_on_tile,
        (tile->player_count + 1) * sizeof(player_t *));

    if (!new_players)
        return false;
    tile->players_on_tile = new_players;
    return true;
}

void add_player_to_tile(tile_t *tile, player_t *player)
{
    if (!tile || !player)
        return;
    if (!allocate_player_memory(tile))
        return;
    tile->players_on_tile[tile->player_count] = player;
    tile->player_count++;
}

static void shift_players_array(tile_t *tile, int removed_index)
{
    for (int j = removed_index; j < tile->player_count - 1; j++) {
        tile->players_on_tile[j] = tile->players_on_tile[j + 1];
    }
    tile->player_count--;
}

void remove_player_from_tile(tile_t *tile, player_t *player)
{
    if (!tile || !player)
        return;
    for (int i = 0; i < tile->player_count; i++) {
        if (tile->players_on_tile[i] == player) {
            shift_players_array(tile, i);
            break;
        }
    }
}

static bool allocate_egg_memory(tile_t *tile)
{
    egg_t **new_eggs = realloc(tile->eggs_on_tile,
        (tile->egg_count + 1) * sizeof(egg_t *));

    if (!new_eggs)
        return false;
    tile->eggs_on_tile = new_eggs;
    return true;
}

void add_egg_to_tile(tile_t *tile, egg_t *egg)
{
    if (!tile || !egg)
        return;
    if (!allocate_egg_memory(tile))
        return;
    tile->eggs_on_tile[tile->egg_count] = egg;
    tile->egg_count++;
}

static void shift_eggs_array(tile_t *tile, int removed_index)
{
    for (int j = removed_index; j < tile->egg_count - 1; j++) {
        tile->eggs_on_tile[j] = tile->eggs_on_tile[j + 1];
    }
    tile->egg_count--;
}

void remove_egg_from_tile(tile_t *tile, egg_t *egg)
{
    if (!tile || !egg)
        return;
    for (int i = 0; i < tile->egg_count; i++) {
        if (tile->eggs_on_tile[i] == egg) {
            shift_eggs_array(tile, i);
            break;
        }
    }
}
