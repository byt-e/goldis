#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "../src/resp.h"

#define PASS(name) printf("[PASS] %s\n", name)

void test_parse_simple_string() {
    const char *input = "+OK\r\n";
    RespValue result;

    int status = parse_resp(input, &result);

    assert(status == 0);
    assert(result.type == RESP_SIMPLE_STRING);
    assert(strcmp(result.str, "OK") == 0);

    free_resp(&result);

    PASS("test_parse_simple_string");
}

void test_parse_error_string() {
    const char *input = "-Error message\r\n";
    RespValue result;

    int status = parse_resp(input, &result);

    assert(status == 0);
    assert(result.type == RESP_ERROR);
    assert(strcmp(result.str, "Error message") == 0);

    free_resp(&result);

    PASS("test_parse_error_string");
}

void test_parse_bulk_string() {
    const char *input = "$4\r\nPING\r\n";
    RespValue result;

    int status = parse_resp(input, &result);

    assert(status == 0);
    assert(result.type == RESP_BULK_STRING);
    assert(strcmp(result.str, "PING") == 0);

    free_resp(&result);

    PASS("test_parse_bulk_string");
}

void test_parse_array() {
    const char *input = "*2\r\n$4\r\nECHO\r\n$5\r\nhello\r\n";
    RespValue result;

    int status = parse_resp(input, &result);

    assert(status == 0);
    assert(result.type == RESP_ARRAY);
    assert(result.array.length == 2);
    assert(result.array.elements[0].type == RESP_BULK_STRING);
    assert(strcmp(result.array.elements[0].str, "ECHO") == 0);
    assert(strcmp(result.array.elements[1].str, "hello") == 0);

    free_resp(&result);

    PASS("test_parse_array");
}

int main(void) {
    test_parse_simple_string();
    test_parse_error_string();
    test_parse_bulk_string();
    test_parse_array();
    return 0;
}
