/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Core broadcast system implementation
*/

#include "broadcast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "game_state.h"

static bool allocate_client_array(broadcast_system_t *broadcast_system)
{
    broadcast_system->clients = calloc(broadcast_system->max_clients,
        sizeof(gui_client_t));
    return broadcast_system->clients != NULL;
}

bool init_broadcast_system(broadcast_system_t *broadcast_system,
    game_state_t *game_state)
{
    if (!broadcast_system || !game_state)
        return false;
    broadcast_system->max_clients = 10;
    broadcast_system->client_count = 0;
    return allocate_client_array(broadcast_system);
}

void cleanup_broadcast_system(broadcast_system_t *broadcast_system,
    game_state_t *game_state)
{
    if (!broadcast_system)
        return;
    if (broadcast_system->clients) {
        free(broadcast_system->clients);
        broadcast_system->clients = NULL;
    }
    broadcast_system->client_count = 0;
    broadcast_system->max_clients = 0;
}

static bool is_client_already_registered(broadcast_system_t *broadcast_system,
    int socket)
{
    for (int i = 0; i < broadcast_system->client_count; i++) {
        if (broadcast_system->clients[i].socket == socket) {
            broadcast_system->clients[i].is_connected = true;
            return true;
        }
    }
    return false;
}

static bool add_new_client(broadcast_system_t *broadcast_system, int socket)
{
    if (broadcast_system->client_count >= broadcast_system->max_clients)
        return false;
    broadcast_system->clients[broadcast_system->client_count].socket = socket;
    broadcast_system->clients[broadcast_system->client_count].is_connected =
        true;
    broadcast_system->client_count++;
    return true;
}

bool register_gui_client(broadcast_system_t *broadcast_system,
    game_state_t *game_state, int socket)
{
    if (!broadcast_system || !game_state || socket < 0)
        return false;
    if (is_client_already_registered(broadcast_system, socket))
        return true;
    if (!add_new_client(broadcast_system, socket))
        return false;
    send_initial_game_state(broadcast_system, game_state, socket);
    return true;
}

void unregister_gui_client(broadcast_system_t *broadcast_system,
    game_state_t *game_state, int socket)
{
    if (!broadcast_system || !game_state || socket < 0)
        return;
    for (int i = 0; i < broadcast_system->client_count; i++) {
        if (broadcast_system->clients[i].socket == socket) {
            broadcast_system->clients[i].is_connected = false;
            break;
        }
    }
}

int send_to_connected_clients(broadcast_system_t *broadcast_system,
    const char *message)
{
    int sent_count = 0;

    for (int i = 0; i < broadcast_system->client_count; i++) {
        if (!broadcast_system->clients[i].is_connected)
            continue;
        printf("[DEBUG] sending to client %d (socket %d): %s",
            i, broadcast_system->clients[i].socket, message);
        if (send(broadcast_system->clients[i].socket, message,
            strlen(message), 0) < 0) {
            printf("[DEBUG] send failed for client %d (socket %d)\n",
                i, broadcast_system->clients[i].socket);
            broadcast_system->clients[i].is_connected = false;
            continue;
        }
        sent_count++;
    }
    return sent_count;
}
