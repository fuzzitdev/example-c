//
// Created by fuzzit.dev inc.
//

#include "parse_complex.h"

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t * data, size_t size) {
    parse_complex((const char *)data, size);
    return 0;
}