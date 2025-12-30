/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GUI command wrapper functions (no args)
*/

#include "gui_commands.h"

char *handle_msz_with_args(game_state_t *game_state, char *args)
{
    (void)args;
    return handle_msz_command(game_state);
}

char *handle_mct_with_args(game_state_t *game_state, char *args)
{
    (void)args;
    return handle_mct_command(game_state);
}

char *handle_tna_with_args(game_state_t *game_state, char *args)
{
    (void)args;
    return handle_tna_command(game_state);
}

char *handle_sgt_with_args(game_state_t *game_state, char *args)
{
    (void)args;
    return handle_sgt_command(game_state);
}
