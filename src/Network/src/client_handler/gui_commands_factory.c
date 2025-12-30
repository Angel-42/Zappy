/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GUI command factory pattern
*/

#include "gui_commands.h"

static const command_entry_t command_table[] = {
    {"msz", handle_msz_with_args, false},
    {"mct", handle_mct_with_args, false},
    {"bct", handle_bct_with_args, true},
    {"tna", handle_tna_with_args, false},
    {"ppo", handle_ppo_with_args, true},
    {"plv", handle_plv_with_args, true},
    {"pin", handle_pin_with_args, true},
    {"sgt", handle_sgt_with_args, false},
    {"sst", handle_sst_with_args, true},
    {NULL, NULL, false}
};

command_handler_t find_command_handler(const char *command, bool *needs_args)
{
    for (int i = 0; command_table[i].command != NULL; i++) {
        if (strcmp(command_table[i].command, command) == 0) {
            *needs_args = command_table[i].needs_args;
            return command_table[i].handler;
        }
    }
    return NULL;
}
