/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GUI command handling header
*/

#ifndef GUI_COMMANDS_H
    #define GUI_COMMANDS_H

    #include <stdbool.h>
    #include <stddef.h>
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include "game_state.h"

typedef char *(*command_handler_t)(game_state_t *, char *);

typedef struct {
    const char *command;
    command_handler_t handler;
    bool needs_args;
} command_entry_t;

// Map commands
char *handle_msz_command(game_state_t *game_state);
void append_tile_info(char *response, tile_t *tile, int x, int y);
char *handle_mct_command(game_state_t *game_state);
char *handle_bct_command(game_state_t *game_state, char *x_str, char *y_str);

// Team commands
char *handle_tna_command(game_state_t *game_state);

// Player commands
char *handle_ppo_command(game_state_t *game_state, char *player_id_str);
char *handle_plv_command(game_state_t *game_state, char *player_id_str);
char *handle_pin_command(game_state_t *game_state, char *player_id_str);

// Server commands
char *handle_sgt_command(game_state_t *game_state);
char *handle_sst_command(game_state_t *game_state, char *freq_str);

// Wrapper functions
char *handle_msz_with_args(game_state_t *game_state, char *args);
char *handle_mct_with_args(game_state_t *game_state, char *args);
char *handle_tna_with_args(game_state_t *game_state, char *args);
char *handle_sgt_with_args(game_state_t *game_state, char *args);
char *handle_bct_with_args(game_state_t *game_state, char *args);
char *handle_ppo_with_args(game_state_t *game_state, char *args);
char *handle_plv_with_args(game_state_t *game_state, char *args);
char *handle_pin_with_args(game_state_t *game_state, char *args);
char *handle_sst_with_args(game_state_t *game_state, char *args);

// Factory function
command_handler_t find_command_handler(const char *command, bool *needs_args);

// Main command handling
char *parse_gui_command(game_state_t *game_state, char *cmd_copy);
bool handle_gui_command(game_state_t *game_state, int client_socket,
    const char *command);

#endif // GUI_COMMANDS_H
