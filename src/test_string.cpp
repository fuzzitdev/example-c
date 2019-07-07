
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

const char * FLAG = "FUZZING IS IMPORTANT";

int test_string(const char *src, size_t len)
{
    if (len < strlen(FLAG)) {
        return 0;
    } else {
        if (strncmp(src, FLAG, strlen(FLAG)) == 0) {
            // TRIGGER HEAP OVERFLOW READ
            if (src[len-1] == 0) {
                return -2;
            }
            return -1;
        } else {
            return 0;
        }
    }

}