#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <strings.h>

#include "handler.h"
#include "resp.h"

#define BUF_SIZE 1024

void handle_command(int client_fd, const char *input) {
    RespValue req;
    int status = parse_resp(input, &req);
    if (status != 0) {
        const char *err = "-ERR Invalid RESP message\r\n";
        send(client_fd, err, strlen(err), 0);
        return;
    }

    if (req.type == RESP_ARRAY && req.array.length > 0) {
        RespValue *first = &req.array.elements[0];

        if (first->type == RESP_BULK_STRING) {
            if (strcasecmp(first->str, "PING") == 0) {
                const char *pong = "+PONG\r\n";
                send(client_fd, pong, strlen(pong), 0);
            } else if (strcasecmp(first->str, "ECHO") == 0 && req.array.length >= 2) {
                RespValue *echo_arg = &req.array.elements[1];
                if (echo_arg->type == RESP_BULK_STRING) {
                    char buf[1024];
                    int n = snprintf(buf, sizeof(buf), "$%lu\r\n%s\r\n", strlen(echo_arg->str), echo_arg->str);
                    send(client_fd, buf, n, 0);
                } else {
                    const char *err = "-ERR ECHO expects a bulk string argument\r\n";
                    send(client_fd, err, strlen(err), 0);
                }
            } else {
                const char *err = "-ERR unknown command\r\n";
                send(client_fd, err, strlen(err), 0);
            }
        }
    } else if (req.type == RESP_BULK_STRING) {
        if (strcasecmp(req.str, "PING") == 0) {
            const char *pong = "+PONG\r\n";
            send(client_fd, pong, strlen(pong), 0);
        } else {
            const char *err = "-ERR unknown bulk command\r\n";
            send(client_fd, err, strlen(err), 0);
        }
    } else {
        const char *err = "-ERR Unsupported RESP type\r\n";
        send(client_fd, err, strlen(err), 0);
    }

    free_resp(&req);
}

