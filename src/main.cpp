//
// Created by fuzzit.dev inc,
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "test_string.h"

int main(int argc, char* const argv[]) {
    if (argc < 2) {
        printf("USAGE: ./test_string <string>\n");
        return -1;
    }
    int ret = test_string(argv[1], strlen(argv[1]));
    if (ret == 0) {
        printf("wrong flag!\n");
    } else {
        printf("you got it:)\n");
    }
    return 0;
}