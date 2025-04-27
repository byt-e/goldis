#ifndef RESP_H
#define RESP_H

typedef enum {
    RESP_SIMPLE_STRING,
    RESP_ERROR,
} RespType;

typedef struct {
    RespType type;
    char *str;
} RespValue;

int parse_resp(const char *input, RespValue *out);

#endif
