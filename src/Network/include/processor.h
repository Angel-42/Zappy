/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Main command processor header
*/

#ifndef PROCESSOR_H_
    #define PROCESSOR_H_

    #include <stdbool.h>
    #include "game_state.h"

// Main command processor
char *process_command(game_state_t *game_state, player_t *player,
    const char *command);

char *prepare_command_copy(const char *command);

// Command validation and utilities
bool is_valid_command(const char *command);
int get_command_time(const char *command, int frequency);

#endif /* !PROCESSOR_H_ */
