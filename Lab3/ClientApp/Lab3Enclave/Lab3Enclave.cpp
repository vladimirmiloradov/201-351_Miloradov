#include "Lab3Enclave_t.h"
#include "sgx_trts.h"
#include <stdio.h>
#include <string.h>

#define BUF_LEN 100

const char table[5][10] = {
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday"
};

void foo(char* buf, size_t len, size_t idx) {
    if (idx < 6) {
        const char* data_ptr = data_ptr = table[idx - 1];
        memcpy(buf, data_ptr, strlen(data_ptr));
    }
    else {
        memset(buf, 0, strlen(table[0]));
    }
    return;
}
