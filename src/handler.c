#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>

#include "handler.h"
#include "resp.h"

#define BUF_SIZE 1024

int handle_client(int client_fd) {
    char buf[BUF_SIZE];
    ssize_t bytes_read = recv(client_fd, buf, sizeof(buf)-1, 0);
    if (bytes_read <= 0) {
        perror("recv");
        return -1; 
    }

    buf[bytes_read] = '\0'; 

    RespValue val;
    if (parse_resp(buf, &val) != 0) {
        const char *msg = "-ERR invalid command\r\n";
        send(client_fd, msg, strlen(msg), 0);
        return 0;
    }

    if (val.type == RESP_SIMPLE_STRING && strcmp(val.str, "PING") == 0) {
        const char *pong = "+PONG\r\n";
        send(client_fd, pong, strlen(pong), 0);
    } else {
        const char *msg = "-ERR unknown command\r\n";
        send(client_fd, msg, strlen(msg), 0);
    }

    free(val.str);

    return 0;
}
