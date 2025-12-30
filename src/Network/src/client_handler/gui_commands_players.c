/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GUI player command handling
*/

#include "gui_commands.h"

char *handle_ppo_command(game_state_t *game_state,
    char *player_id_str)
{
    int player_id = atoi(player_id_str);
    player_t *player = get_player(game_state, player_id);
    char ppo_response[128];

    if (!player)
        return NULL;
    snprintf(ppo_response, sizeof(ppo_response),
        "ppo %d %d %d %d\n", player_id, player->x, player->y,
        player->direction);
    return strdup(ppo_response);
}

char *handle_plv_command(game_state_t *game_state,
    char *player_id_str)
{
    int player_id = atoi(player_id_str);
    player_t *player = get_player(game_state, player_id);
    char plv_response[64];

    if (!player)
        return NULL;
    snprintf(plv_response, sizeof(plv_response), "plv %d %d\n",
            player_id, player->level);
    return strdup(plv_response);
}

char *handle_pin_command(game_state_t *game_state, char *player_id_str)
{
    int player_id = atoi(player_id_str);
    player_t *player = get_player(game_state, player_id);
    char pin_response[256];

    if (!player)
        return NULL;
    snprintf(pin_response, sizeof(pin_response),
            "pin %d %d %d %d %d %d %d %d %d %d\n",
            player_id, player->x, player->y,
            player->inventory[FOOD],
            player->inventory[LINEMATE],
            player->inventory[DERAUMERE],
            player->inventory[SIBUR],
            player->inventory[MENDIANE],
            player->inventory[PHIRAS],
            player->inventory[THYSTAME]);
    return strdup(pin_response);
}
