/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Core broadcast system implementation - additional functions
*/

#include "broadcast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "game_state.h"


bool broadcast_to_gui(broadcast_system_t *broadcast_system,
    game_state_t *game_state, const char *message)
{
    int sent_count;

    if (!broadcast_system || !game_state || !message)
        return false;
    sent_count = send_to_connected_clients(broadcast_system, message);
    return sent_count > 0;
}

bool send_to_gui_client(broadcast_system_t *broadcast_system,
    game_state_t *game_state, int socket, const char *message)
{
    if (!broadcast_system || !game_state || !message || socket < 0)
        return false;
    return send(socket, message, strlen(message), 0) >= 0;
}
