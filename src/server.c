#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

#include "handler.h"

void run_server(int port) {
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(port)
    };

    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(listener, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Listening on port: %d\n", port);

    fd_set master, read_fds;
    FD_ZERO(&master);
    FD_SET(listener, &master);
    int fdmax = listener;

    while (1) {
        read_fds = master;
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == listener) {
                    // New Connection.
                    struct sockaddr_in client;
                    socklen_t addrlen = sizeof(client);
                    int newfd = accept(listener, (struct sockaddr *)&client, &addrlen);
                    if (newfd == -1) {
                        perror("accept");
                    } else {
                        FD_SET(newfd, &master);
                        if (newfd > fdmax) fdmax = newfd;
                        printf("New client connected: fd=%d\n", newfd);
                    }
                } else {
                    // Existing Client, handle message.
                    if (handle_client(i) < 0) {
                        close(i);
                        FD_CLR(i, &master);
                        printf("Client disconnected: fd=%d\n", i);
                    }
                }
            }
        }
    }
}
