#include <string.h>
#include <stdlib.h>
#include <stdio.h>

bool parse_complex(const char *src, size_t len) {
    if (len == 5) {
        if (src[0] == 'F' &&
            src[1] == 'U' &&
            src[2] == 'Z' &&
            src[3] == 'Z' &&
            src[4] == 'I' &&
            src[5] == 'T') {
            return true;
        }
    }
    return false;
}