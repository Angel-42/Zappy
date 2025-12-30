/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Incantation system implementation
*/

#include "incantation.h"
#include <stdlib.h>
#include <string.h>
#include "game_state.h"
#include "map.h"
#include "player.h"
#include "broadcast.h"

static const elevation_requirements_t ELEVATION_TABLE[] = {
    {1, 1, 0, 0, 0, 0, 0},   // Level 1->2
    {2, 1, 1, 1, 0, 0, 0},   // Level 2->3
    {2, 2, 0, 1, 0, 2, 0},   // Level 3->4
    {4, 1, 1, 2, 0, 1, 0},   // Level 4->5
    {4, 1, 2, 1, 3, 0, 0},   // Level 5->6
    {6, 1, 2, 3, 0, 1, 0},   // Level 6->7
    {6, 2, 2, 2, 2, 2, 1}    // Level 7->8
};

static incantation_t *initialize_incantation_fields(incantation_t *incantation,
    game_state_t *game_state, int x, int y)
{
    if (!incantation)
        return NULL;
    incantation->id = (game_state->next_incantation_id + 1);
    incantation->x = x;
    incantation->y = y;
    incantation->player_count = 0;
    incantation->players = NULL;
    incantation->start_time = game_state->current_tick;
    incantation->is_active = true;
    return incantation;
}

incantation_t *create_incantation(game_state_t *game_state, int x, int y,
    int level)
{
    incantation_t *incantation = malloc(sizeof(incantation_t));

    if (!incantation)
        return NULL;
    incantation->level = level;
    return initialize_incantation_fields(incantation, game_state, x, y);
}

void destroy_incantation(incantation_t *incantation)
{
    if (!incantation)
        return;
    free(incantation->players);
    free(incantation);
}

static bool allocate_player_memory(incantation_t *incantation)
{
    player_t **new_players = realloc(incantation->players,
        (incantation->player_count + 1) * sizeof(player_t *));

    if (!new_players)
        return false;
    incantation->players = new_players;
    return true;
}

bool add_player_to_incantation(incantation_t *incantation, player_t *player)
{
    if (!incantation || !player)
        return false;
    if (!allocate_player_memory(incantation))
        return false;
    incantation->players[incantation->player_count] = player;
    incantation->player_count++;
    return true;
}

elevation_requirements_t get_elevation_requirements(int level)
{
    if (level < 1 || level > 7) {
        return (elevation_requirements_t){0};
    }
    return ELEVATION_TABLE[level - 1];
}

static bool check_resource_requirements(player_t *player,
    elevation_requirements_t req)
{
    if (player->inventory[LINEMATE] < req.linemate)
        return false;
    if (player->inventory[DERAUMERE] < req.deraumere)
        return false;
    if (player->inventory[SIBUR] < req.sibur)
        return false;
    if (player->inventory[MENDIANE] < req.mendiane)
        return false;
    if (player->inventory[PHIRAS] < req.phiras)
        return false;
    if (player->inventory[THYSTAME] < req.thystame)
        return false;
    return true;
}

static bool check_player_count_requirement(game_state_t *game_state,
    player_t *player, elevation_requirements_t req)
{
    tile_t *tile = get_tile(game_state->map, player->x, player->y);
    int same_level_count = 0;

    if (!tile)
        return false;
    for (int i = 0; i < tile->player_count; i++) {
        if (tile->players_on_tile[i] &&
            tile->players_on_tile[i]->level == player->level) {
            same_level_count++;
        }
    }
    return same_level_count >= req.players;
}

bool can_elevate(game_state_t *game_state, player_t *player)
{
    elevation_requirements_t req;

    if (!game_state || !player)
        return false;
    req = get_elevation_requirements(player->level);
    if (!check_resource_requirements(player, req))
        return false;
    return check_player_count_requirement(game_state, player, req);
}
