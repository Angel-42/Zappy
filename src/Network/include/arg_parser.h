/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** arg_parser.h
*/

#ifndef ARG_PARSER_H_
    #define ARG_PARSER_H_

    #include "data.h"

/**
 * @brief Parses command line arguments for the server configuration.
 *
 * This function processes the command line arguments provided to the server,
 * extracting configuration options such as port number, width, height, and
 * team names. It populates the `server_data_t`
 * structure with the parsed values.
 *
 * @param argc The number of command line arguments.
 * @param argv The array of command line argument strings.
 * @param data Pointer to the server_data_t
 * structure to populate with parsed data.
 * @return 0 on success, or -1 on failure (e.g., invalid arguments).
 */
int parse_arguments(int argc, char **argv, server_data_t *data);

#endif /* !ARG_PARSER_H_ */
