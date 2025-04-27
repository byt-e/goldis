#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

#define DEFAULT_PORT 8888
#define BUF_SIZE 1024

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(DEFAULT_PORT);

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        return EXIT_FAILURE;
    }

    if (listen(sockfd, SOMAXCONN) < 0) {
        perror("listen");
        return EXIT_FAILURE;
    }

    fprintf(stdout, "Listening on port=%d\n", DEFAULT_PORT);

    fd_set master_set, read_fds;
    int fdmax = sockfd;

    FD_ZERO(&master_set);
    FD_SET(sockfd, &master_set);

    while (1) {
        read_fds = master_set;
        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            return EXIT_FAILURE;
        }

        for (int i = 0; i <= fdmax; ++i) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == sockfd) {
                    // New Connection.
                    struct sockaddr_in client_addr;
                    socklen_t addrlen = sizeof(client_addr);
                    int newfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);

                    if (newfd == -1) {
                        perror("accept");
                    } else {
                        FD_SET(newfd, &master_set);
                        if (newfd > fdmax) fdmax = newfd;

                        char ip[INET_ADDRSTRLEN];
                        inet_ntop(AF_INET, &(client_addr.sin_addr), ip, INET_ADDRSTRLEN);
                        printf("New connection from %s:%d\n", ip, ntohs(client_addr.sin_port));
                    }
                } else {
                    // Data from client
                    char buf[BUF_SIZE];
                    int nbytes = recv(i, buf, sizeof(buf), 0);

                    if (nbytes == 0) {
                        printf("Socket %d disconnected\n", i);
                        close(i);
                        FD_CLR(i, &master_set);
                    } else if (nbytes < 0) {
                        perror("recv");
                        close(i);
                        FD_CLR(i, &master_set);
                    } else {
                        buf[nbytes] = '\0';
                        printf("Received: %s", buf);
                        send(i, "PONG\n", 5, 0);
                    }
                }
            }
        }

    }

    return EXIT_SUCCESS;
}
