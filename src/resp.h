#ifndef RESP_H
#define RESP_H

typedef enum {
    RESP_SIMPLE_STRING,
    RESP_BULK_STRING,
    RESP_ARRAY,
    RESP_ERROR,
} RespType;

typedef struct RespValue {
    RespType type;
    union {
        char *str;
        struct {
            struct RespValue *elements;
            int length;
        } array;
    };
} RespValue;

int parse_resp(const char *input, RespValue *out);

#endif
