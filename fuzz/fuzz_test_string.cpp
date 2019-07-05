//
// Created by fuzzit.dev inc.
//

#include "test_string.h"

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t * data, size_t size) {
    test_string((const char *)data, size);
    return 0;
}