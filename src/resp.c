#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "resp.h"

static int parse_resp_line(const char *input, RespValue *out, RespType type) {
    size_t len = strlen(input);

    // Check the line is formatted correctly (delimited with \r\n)
    if (len < 3 || input[len - 2] != '\r' || input[len - 1] != '\n') {
        return -1;
    }

    size_t body_len = len - 3 + 1; // Minux prefix (-/+, and \r\n), Add 1 for null terminator.
    out->str = malloc(body_len);
    if (!out->str) return -1;

    strncpy(out->str, input + 1, body_len - 1);
    out->str[body_len -1] = '\0';

    out->type = type;
    return 0;
}

int parse_resp(const char *input, RespValue *out) {
    if (!input || !out) return -1;

    switch (input[0]) {
        case '+':
            return parse_resp_line(input, out, RESP_SIMPLE_STRING);
        case '-':
            return parse_resp_line(input, out, RESP_ERROR);
        default:
            return -1; // Unsupported RESP type.
    }
}

void free_resp(RespValue *val) {
    if (val && val->str) {
        free(val->str);
        val->str = NULL;
    }
}
