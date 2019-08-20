#!/bin/bash
set -xe

## Build fuzzing targets
export CC=`which clang`
export CXX=`which clang++`
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=AddressSanitizer
make

if [ "$TRAVIS_PULL_REQUEST" = "false" ]; then
    export FUZZING_TYPE="fuzzing"
else
    export FUZZING_TYPE="local-regression"
fi

## Install fuzzit specific version for production or latest version for development :
# https://github.com/fuzzitdev/fuzzit/releases/latest/download/fuzzit_Linux_x86_64
wget -q -O fuzzit https://github.com/fuzzitdev/fuzzit/releases/download/v2.4.29/fuzzit_Linux_x86_64
chmod a+x fuzzit

## upload fuzz target for long fuzz testing on fuzzit.dev server or run locally for regression
./fuzzit create job --type ${FUZZING_TYPE} fuzzitdev/fuzz-test-string ./fuzz/fuzz_test_string
