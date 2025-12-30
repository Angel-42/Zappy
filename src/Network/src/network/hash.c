/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** hash.c
*/

unsigned long djb2(const char *string)
{
    unsigned long hash = 5381;
    int c;

    while (*string != '\0') {
        c = *string;
        hash = ((hash << 5) + hash) + c;
        string = string + 1;
    }
    return hash;
}
