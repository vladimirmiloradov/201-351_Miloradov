#include <iostream>
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include "sgx_urts.h"
#include "Lab3Enclave_u.h"
#define ENCLAVE_FILE _T("Lab3Enclave.signed.dll")
#define BUF_LEN 100

//const char table[5][10] = {
//    "Monday",
//    "Tuesday",
//    "Wednesday",
//    "Thursday",
//    "Friday"
//};
//
//void foo(char * buf, size_t len, size_t idx) {
//    if (idx < 6) {
//        const char* data_ptr = data_ptr = table[idx-1];
//        memcpy(buf, data_ptr, strlen(data_ptr));
//    }
//    else {
//        memset(buf, 0, strlen(table[0]));
//    }
//    return;
//}

int main() {
    sgx_enclave_id_t eid;
    sgx_status_t ret = SGX_SUCCESS;
    sgx_launch_token_t token = { 0 };
    int updated = 0;
    char buffer[BUF_LEN] = { 0 };
    // Create the Enclave with above launch token.
    ret = sgx_create_enclave(ENCLAVE_FILE, SGX_DEBUG_FLAG, &token, &updated, &eid, NULL);
    if (ret != SGX_SUCCESS) {
        printf("App: error %#x, failed to create enclave.\n", ret);
        return -1;
    }
    while (true) {
        std::cout << "Enter id form 1 to 5 to see elem or -1 to exit: ";
        int idx = -1;
        std::cin >> idx;
        if (idx < 0) {
            return 0;
        }
        foo(eid, buffer, BUF_LEN, idx);
        printf_s("%s\n", buffer);
    }
    if (SGX_SUCCESS != sgx_destroy_enclave(eid))
        return -1;
    return 0;
}