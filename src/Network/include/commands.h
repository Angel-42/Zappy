/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Command processing header
*/

#ifndef COMMANDS_H_
    #define COMMANDS_H_

    #include <stdbool.h>
    #include "game_state.h"

    // Include all command module headers
    #include "movement.h"
    #include "info.h"
    #include "resources.h"
    #include "actions.h"
    #include "processor.h"

// Legacy function declarations for backward compatibility
// These are now handled by the processor module
bool init_commands(void);
void cleanup_commands(void);
bool process_commands(void);

// Individual command handlers
char *handle_forward(game_state_t *game_state, player_t *player);
char *handle_right(game_state_t *game_state, player_t *player);
char *handle_left(game_state_t *game_state, player_t *player);
char *handle_look(game_state_t *game_state, player_t *player);
char *handle_inventory(game_state_t *game_state, player_t *player);
char *handle_broadcast(game_state_t *game_state, player_t *player,
    const char *message);
char *handle_connect_nbr(game_state_t *game_state, player_t *player);
char *handle_fork(game_state_t *game_state, player_t *player);
char *handle_eject(game_state_t *game_state, player_t *player);
char *handle_take(game_state_t *game_state, player_t *player,
    const char *resource_name);
char *handle_set(game_state_t *game_state, player_t *player,
    const char *resource_name);
char *handle_incantation(game_state_t *game_state, player_t *player);

// Main command processor
char *process_command(game_state_t *game_state, player_t *player,
    const char *command);

// Command validation and utilities
bool is_valid_command(const char *command);
int get_command_time(const char *command, int frequency);

#endif /* !COMMANDS_H_ */
