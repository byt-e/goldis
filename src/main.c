#include "server.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DEFAULT_PORT 8888

struct flags{
    int port;
};

struct flags parse_flags(int argc, char *argv[]);
int parse_port(const char *arg);

int main(int argc, char *argv[]) {
    struct flags flags = parse_flags(argc, argv);

    run_server(flags.port);

    return EXIT_SUCCESS;
}

struct flags parse_flags(int argc, char *argv[]) {
    int port;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printf("Usage: %s [--port|-p <port>]\n", argv[0]);
            printf("  --port|-p  Run Goldis on the provided port\n");
            exit(EXIT_SUCCESS);
        }

        if (strcmp(argv[i], "--port") == 0 || strcmp(argv[i], "-p") == 0) {
            port = parse_port(argv[++i]);
            if (port < 0) {
                fprintf(stderr, "Invalid value for --port|-p: %s\n", argv[i]);
                exit(EXIT_FAILURE);
            }
        }
    }

    if (port <= 0) {
        port = DEFAULT_PORT;
    }

    struct flags flags = {
        .port = port,
    };

    return flags;
}

int parse_port(const char *arg) {
    char *endptr;
    long val = strtol(arg, &endptr, 10);

    if (*endptr != '\0' || val < 1) {
        return -1;
    }

    return (int) val;
}
