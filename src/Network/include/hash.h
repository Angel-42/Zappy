/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** hash.h
*/

#ifndef HASH_H_
    #define HASH_H_

    #include <stdlib.h>
    #include <sha256.h>

/**
 * @brief Computes the hash value of a given string using the djb2 algorithm.
 *
 * This function implements the djb2 algorithm.
 * It processes each character of the null-terminated string to produce a hash
 * value, ensuring a good distribution over large sets of input strings.
 *
 * @param string A pointer to the null-terminated string to hash.
 * @return The computed hash value as a long integer.
 */
unsigned long djb2(const char *string);

#endif /* !HASH_H_ */
