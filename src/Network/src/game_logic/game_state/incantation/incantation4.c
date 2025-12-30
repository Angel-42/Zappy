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

static void shift_players_array(incantation_t *incantation, int removed_index)
{
    for (int j = removed_index; j < incantation->player_count - 1; j++) {
        incantation->players[j] = incantation->players[j + 1];
    }
    incantation->player_count--;
}

bool remove_player_from_incantation(incantation_t *incantation,
    player_t *player)
{
    if (!incantation || !player)
        return false;
    for (int i = 0; i < incantation->player_count; i++) {
        if (incantation->players[i] == player) {
            shift_players_array(incantation, i);
            return true;
        }
    }
    return false;
}
