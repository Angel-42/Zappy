/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** arg_parser.c
*/

#include "data.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct arg_parser_s {
    int argc;
    char **argv;
    int index;
};

static int next_arg(struct arg_parser_s *args)
{
    if (args->index + 1 >= args->argc)
        return -1;
    args->index++;
    return 0;
}

static int parse_numeric_arg(struct arg_parser_s *args, unsigned long *value)
{
    if (next_arg(args) < 0)
        return -1;
    *value = strtoul(args->argv[args->index], NULL, 10);
    return 0;
}

static int parse_port(struct arg_parser_s *args, unsigned short *port)
{
    unsigned long value = 0;

    if (parse_numeric_arg(args, &value) < 0)
        return -1;
    if (value > USHRT_MAX)
        return -1;
    *port = (unsigned short)value;
    if (*port == 0)
        return -1;
    return 0;
}

static int parse_teams(struct arg_parser_s *args, teams_t *teams)
{
    if (next_arg(args) < 0)
        return -1;
    teams->names = &args->argv[args->index];
    teams->count = 0;
    while (args->index < args->argc && args->argv[args->index][0] != '-') {
        if (strcmp(args->argv[args->index], "GRAPHIC") == 0) {
            perror("The team name 'GRAPHIC' is reserved and cannot be used.");
            return -1;
        }
        teams->count++;
        args->index++;
    }
    args->index--;
    return 0;
}

static bool is_already_set_flag(char *parsed_args, char *ptr, char flag)
{
    if (strchr(parsed_args, flag) != NULL) {
        fprintf(stderr, "Flag -%c is already set\n", flag);
        return true;
    }
    *ptr = flag;
    return false;
}

static int process_argument(struct arg_parser_s *args, server_data_t *data,
    char *parsed_args, char *ptr)
{
    if (is_already_set_flag(parsed_args, ptr, args->argv[args->index][1]))
        return -1;
    switch (args->argv[args->index][1]) {
        case 'p':
            return parse_port(args, &data->port);
        case 'x':
            return parse_numeric_arg(args, &data->width);
        case 'y':
            return parse_numeric_arg(args, &data->height);
        case 'n':
            return parse_teams(args, &data->teams);
        case 'c':
            return parse_numeric_arg(args, &data->max_clients);
        case 'f':
            return parse_numeric_arg(args, &data->frequency);
        default:
            return -1;
    }
}

static int has_duplicate_team_name(const char *team_name, const teams_t *teams,
    unsigned long from_index)
{
    for (unsigned long i = from_index; i < teams->count; i++) {
        if (strcmp(team_name, teams->names[i]) == 0)
            return 1;
    }
    return 0;
}

static int validate_unique_team_names(const teams_t *teams)
{
    if (teams->count < 2) {
        perror("At least two teams are required");
        return -1;
    }
    for (unsigned long i = 0; i < teams->count; i++) {
        if (has_duplicate_team_name(teams->names[i], teams, i + 1)) {
            perror("Duplicate team name found");
            return -1;
        }
    }
    return 0;
}

static int verify_flags(const char *parsed_args, const char *expected_flags)
{
    for (const char *flag = expected_flags; *flag != '\0'; ++flag) {
        if (strchr(parsed_args, *flag) == NULL) {
            fprintf(stderr, "Missing required flag: -%c\n", *flag);
            return -1;
        }
    }
    return 0;
}

int parse_arguments(int argc, char **argv, server_data_t *data)
{
    struct arg_parser_s args = {argc, argv, 1};
    char parsed_args[7] = {0};
    char *ptr = parsed_args;
    constexpr size_t MAX_SIZE = sizeof(parsed_args) / sizeof(char) - 1;

    while (args.index < args.argc) {
        if (args.argv[args.index][0] != '-' ||
            args.argv[args.index][1] == '\0' ||
            args.argv[args.index][2] != '\0')
            return -1;
        if (strlen(parsed_args) >= MAX_SIZE ||
            process_argument(&args, data, parsed_args, ptr) < 0)
            return -1;
        ptr++;
        args.index++;
    }
    if (verify_flags(parsed_args, "pxync") < 0)
        return -1;
    return validate_unique_team_names(&data->teams);
}
