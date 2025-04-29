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

static int parse_bulk_string(const char *input, RespValue *out) {
    const char *len_start = input + 1;
    char *new_line = strstr(len_start, "\r\n");
    if (!new_line) return -1;

    int len = atoi(len_start);
    if (len < 0) {
        out->type = RESP_BULK_STRING;
        out->str = NULL;
        return 0;
    }

    const char *data_start = new_line + 2;
    const char *data_end = data_start + len;

    if (strncmp(data_end, "\r\n", 2) != 0) return -1;

    out->str = malloc(len + 1);
    if (!out->str) return -1;

    memcpy(out->str, data_start, len);
    out->str[len] = '\0';

    out->type = RESP_BULK_STRING;
    return 0;
}

static int parse_array(const char *input, RespValue *out) {
    if (!input || input[0] != '*') return -1;

    int count;
    const char *newline = strstr(input, "\r\n");
    if (!newline || sscanf(input + 1, "%d", &count) != 1 || count < 0) return -1;

    RespValue *elements = calloc(count, sizeof(RespValue));
    if (!elements) return -1;

    const char *cursor = newline + 2;
    for (int i = 0; i < count; ++i) {
        int status = parse_resp(cursor, &elements[i]);
        if (status != 0) {
            for (int j = 0; j < i; j++) {
                free_resp(&elements[j]);
            }
            free(elements);
            return -1;
        }

        if (elements[i].type == RESP_BULK_STRING) {
            int bulk_len = strlen(elements[i].str);
            cursor = strstr(cursor, "\r\n") + 2 + bulk_len + 2;
        } else if (elements[i].type == RESP_SIMPLE_STRING || elements[i].type == RESP_ERROR) {
            cursor = strstr(cursor, "\r\n") + 2;
        } else {
            free_resp(&elements[i]);
            free(elements);
            return -1;
        }
    }

    out->type = RESP_ARRAY;
    out->array.length = count;
    out->array.elements = elements;

    return 0;
}

int parse_resp(const char *input, RespValue *out) {
    if (!input || !out) return -1;

    switch (input[0]) {
        case '+':
            return parse_resp_line(input, out, RESP_SIMPLE_STRING);
        case '-':
            return parse_resp_line(input, out, RESP_ERROR);
        case '$':
            return parse_bulk_string(input, out);
        case '*':
            return parse_array(input, out);
        default:
            return -1; // Unsupported RESP type.
    }
}

void free_resp(RespValue *val) {
    if (!val) return;

    if (val->type == RESP_SIMPLE_STRING 
        || val->type == RESP_ERROR 
        || val->type == RESP_BULK_STRING
    ) {
        if (val->str) {
            free(val->str);
            val->str = NULL;
        }
    } else if (val->type == RESP_ARRAY) {
        if (val->array.elements) {
            for (int i = 0; i < val->array.length; ++i) {
                free_resp(&val->array.elements[i]);
            }
            free(val->array.elements);
            val->array.elements = NULL;
            val->array.length = 0;
        }
    }
}
