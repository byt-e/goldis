#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "handler.h"

void handle_command(int client_fd, const char *input) {
    if (strncmp(input, "PING", 4) == 0) {
        const char *resp = "+PONG\r\n";
        send(client_fd, resp, strlen(resp), 0);
    } else {
        const char *resp = "-ERR unknown command\r\n";
        send(client_fd, resp, strlen(resp), 0);
    }
}
