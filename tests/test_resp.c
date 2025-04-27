#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "../src/resp.h"

void test_parse_simple_string() {
    const char *input = "+OK\r\n";
    RespValue result;

    int status = parse_resp(input, &result);

    assert(status == 0);
    assert(result.type == RESP_SIMPLE_STRING);
    assert(strcmp(result.str, "OK") == 0);

    printf("[PASS] test_parse_simple_string\n");
}

void test_parse_error_string() {
    const char *input = "-Error message\r\n";
    RespValue result;

    int status = parse_resp(input, &result);

    assert(status == 0);
    assert(result.type == RESP_ERROR);
    assert(strcmp(result.str, "Error message") == 0);

    printf("[PASS] test_parse_error_string\n");
}

void test_parse_bulk_string() {
    const char *input = "$4\r\nPING\r\n";
    RespValue result;

    int status = parse_resp(input, &result);

    assert(status == 0);
    assert(result.type == RESP_BULK_STRING);
    assert(strcmp(result.str, "PING") == 0);

    printf("[PASS] test_parse_bulk_string\n");
}

int main(void) {
    test_parse_simple_string();
    test_parse_error_string();
    test_parse_bulk_string();
    return 0;
}
