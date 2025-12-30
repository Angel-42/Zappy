/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GUI command wrapper functions (with args)
*/

#include "gui_commands.h"

char *handle_bct_with_args(game_state_t *game_state, char *args)
{
    char *x_str = strtok(args, " ");
    char *y_str = strtok(NULL, " ");

    if (!x_str || !y_str)
        return NULL;
    return handle_bct_command(game_state, x_str, y_str);
}

char *handle_ppo_with_args(game_state_t *game_state, char *args)
{
    char *player_id_str = strtok(args, " ");

    if (!player_id_str)
        return NULL;
    return handle_ppo_command(game_state, player_id_str);
}

char *handle_plv_with_args(game_state_t *game_state, char *args)
{
    char *player_id_str = strtok(args, " ");

    if (!player_id_str)
        return NULL;
    return handle_plv_command(game_state, player_id_str);
}

char *handle_pin_with_args(game_state_t *game_state, char *args)
{
    char *player_id_str = strtok(args, " ");

    if (!player_id_str)
        return NULL;
    return handle_pin_command(game_state, player_id_str);
}

char *handle_sst_with_args(game_state_t *game_state, char *args)
{
    char *freq_str = strtok(args, " ");

    if (!freq_str)
        return NULL;
    return handle_sst_command(game_state, freq_str);
}
