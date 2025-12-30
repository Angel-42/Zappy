/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Map resource spawning system
*/

#include "map.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "game_state.h"

static int calculate_max_resources(int map_area, float density)
{
    int max = (int)(map_area * density);

    return (max < 1) ? 1 : max;
}

static void calculate_resource_limits(game_state_t *game_state,
    resource_limits_t *limits)
{
    int map_area = game_state->map->width * game_state->map->height;

    limits->max_food = calculate_max_resources(map_area, FOOD_DENSITY);
    limits->max_linemate = calculate_max_resources(map_area, LINEMATE_DENSITY);
    limits->max_deraumere = calculate_max_resources(map_area,
        DERAUMERE_DENSITY);
    limits->max_sibur = calculate_max_resources(map_area, SIBUR_DENSITY);
    limits->max_mendiane = calculate_max_resources(map_area, MENDIANE_DENSITY);
    limits->max_phiras = calculate_max_resources(map_area, PHIRAS_DENSITY);
    limits->max_thystame = calculate_max_resources(map_area, THYSTAME_DENSITY);
}

static void initialize_resource_counts(resource_counts_t *counts)
{
    counts->food = 0;
    counts->linemate = 0;
    counts->deraumere = 0;
    counts->sibur = 0;
    counts->mendiane = 0;
    counts->phiras = 0;
    counts->thystame = 0;
}

static void add_tile_resources_to_counts(tile_t *tile,
    resource_counts_t *counts)
{
    counts->food += tile->resources[FOOD];
    counts->linemate += tile->resources[LINEMATE];
    counts->deraumere += tile->resources[DERAUMERE];
    counts->sibur += tile->resources[SIBUR];
    counts->mendiane += tile->resources[MENDIANE];
    counts->phiras += tile->resources[PHIRAS];
    counts->thystame += tile->resources[THYSTAME];
}

static void count_resources_in_row(game_state_t *game_state, int y,
    resource_counts_t *counts)
{
    tile_t *tile;

    for (int x = 0; x < game_state->map->width; x++) {
        tile = get_tile(game_state->map, x, y);
        if (tile) {
            add_tile_resources_to_counts(tile, counts);
        }
    }
}

static void count_current_resources(game_state_t *game_state,
    resource_counts_t *counts)
{
    initialize_resource_counts(counts);
    for (int y = 0; y < game_state->map->height; y++) {
        count_resources_in_row(game_state, y, counts);
    }
}

static void calculate_resources_to_add(resource_limits_t *limits,
    resource_counts_t *counts, resource_additions_t *additions)
{
    additions->food = (limits->max_food - counts->food > 0) ?
        (limits->max_food - counts->food) : 0;
    additions->linemate = (limits->max_linemate - counts->linemate > 0) ?
        (limits->max_linemate - counts->linemate) : 0;
    additions->deraumere = (limits->max_deraumere - counts->deraumere > 0) ?
        (limits->max_deraumere - counts->deraumere) : 0;
    additions->sibur = (limits->max_sibur - counts->sibur > 0) ?
        (limits->max_sibur - counts->sibur) : 0;
    additions->mendiane = (limits->max_mendiane - counts->mendiane > 0) ?
        (limits->max_mendiane - counts->mendiane) : 0;
    additions->phiras = (limits->max_phiras - counts->phiras > 0) ?
        (limits->max_phiras - counts->phiras) : 0;
    additions->thystame = (limits->max_thystame - counts->thystame > 0) ?
        (limits->max_thystame - counts->thystame) : 0;
}

static int *generate_random_positions(int map_area)
{
    int *positions;
    int j;
    int temp;

    positions = malloc(map_area * sizeof(int));
    if (!positions)
        return NULL;
    for (int i = 0; i < map_area; i++) {
        positions[i] = i;
    }
    for (int i = map_area - 1; i > 0; i--) {
        j = rand() % (i + 1);
        temp = positions[i];
        positions[i] = positions[j];
        positions[j] = temp;
    }
    return positions;
}

bool spawn_resources(game_state_t *game_state)
{
    resource_limits_t limits;
    resource_counts_t counts;
    resource_additions_t additions;
    int map_area;
    int *positions;

    if (!game_state || !game_state->map)
        return false;
    map_area = game_state->map->width * game_state->map->height;
    calculate_resource_limits(game_state, &limits);
    count_current_resources(game_state, &counts);
    calculate_resources_to_add(&limits, &counts, &additions);
    positions = generate_random_positions(map_area);
    if (!positions)
        return false;
    spawn_all_resources(game_state, positions, map_area, &additions);
    free(positions);
    return true;
}
