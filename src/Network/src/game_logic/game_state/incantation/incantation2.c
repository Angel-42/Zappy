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

static bool check_all_players_can_elevate(game_state_t *game_state,
    incantation_t *incantation)
{
    for (int i = 0; i < incantation->player_count; i++) {
        if (!can_elevate(game_state, incantation->players[i])) {
            return false;
        }
    }
    return true;
}

static void consume_resources(player_t *player, elevation_requirements_t req)
{
    player->inventory[LINEMATE] -= req.linemate;
    player->inventory[DERAUMERE] -= req.deraumere;
    player->inventory[SIBUR] -= req.sibur;
    player->inventory[MENDIANE] -= req.mendiane;
    player->inventory[PHIRAS] -= req.phiras;
    player->inventory[THYSTAME] -= req.thystame;
}

static void elevate_players(game_state_t *game_state,
    incantation_t *incantation)
{
    elevation_requirements_t req;
    player_t *player;

    for (int i = 0; i < incantation->player_count; i++) {
        player = incantation->players[i];
        player->level++;
        req = get_elevation_requirements(player->level - 1);
        consume_resources(player, req);
        player->is_incantating = false;
        broadcast_player_level(&game_state->broadcast_system, game_state,
            player);
        broadcast_player_inventory(&game_state->broadcast_system, game_state,
            player);
    }
}

static void reset_players_incantation_state(incantation_t *incantation)
{
    for (int i = 0; i < incantation->player_count; i++) {
        incantation->players[i]->is_incantating = false;
    }
}

static void broadcast_incantation_end_event(game_state_t *game_state,
    incantation_t *incantation, bool success)
{
    broadcast_incantation_t broadcast_data;

    setup_broadcast_data(&broadcast_data, game_state, incantation, NULL);
    broadcast_data.result = success ? 1 : 0;
    broadcast_incantation_end(&broadcast_data);
}

bool end_incantation(game_state_t *game_state, incantation_t *incantation)
{
    bool success;

    if (!game_state || !incantation)
        return false;
    success = check_all_players_can_elevate(game_state, incantation);
    if (success) {
        elevate_players(game_state, incantation);
    } else {
        reset_players_incantation_state(incantation);
    }
    incantation->is_active = false;
    broadcast_incantation_end_event(game_state, incantation, success);
    return success;
}
