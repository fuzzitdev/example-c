![fuzzit](https://app.fuzzit.dev/badge?org_id=hP8u8bNAda91Cnj0mKPX&branch=master)

# Continuous Fuzzing Example
This is an example of a project with continuous fuzzing integration

## Introduction

In this tutorial will walk through how to setup fuzzing and then continuous fuzzing
for c/c++ project with cmake build system (though it can be used with any other build system as well)

## Prerequisite

This tutorial was tested on Ubuntu 18 though it should work on Unix environment

The required packages are cmake and clang > 6.0

```bash
apt update && apt install -y git clang cmake 
```

## Getting the code

```bash
git clone https://github.com/fuzzitdev/continuous-fuzzing-example
```

## Compiling

```bash
# you might need to export CXX=<path_to_clang++> CC=<path_to_clang>

cd continuous-fuzzing-example
mkdir build
export CC=`which clang`
export CXX=`which clang++`
cmake .. -DCMAKE_BUILD_TYPE=AddressSanitizer
make
```

## Understanding the code
This code consist of library and a command line program.

The code is a dummy code (just for the sake of the example).
that trigger an heap out-of-bound access when it is passed the correct
"secret" flag.

```text
int test_string(const char *src, size_t len)
{
    if (len < strlen(FLAG)) {
        return 0;
    } else {
        if (strncmp(src, FLAG, strlen(FLAG)) == 0) {
            // TRIGGER HEAP OVERFLOW READ
            if (src[len] == 0) {
                return -2;
            }
            return -1;
        } else {
            return 0;
        }
    }

}
```

when the secret flag is passed then the code is accessing an off-by-one in the `src`
array.

## Fuzzing
we will use the libFuzzer fuzzer located under `fuzz/` to find the "secret" 
flag and trigger the bug.

```bash
./fuzz/fuzz_test_string -exact_artifact_path=crash
```

Because it's a very simple code libFuzzer under <1 sec (Though in complicated code libFuzzer sometimes find complicated
 bugs quickly like heartbleed and others...)
 
 The output should look something like this:
 ```text
 INFO: Seed: 432631686
 INFO: Loaded 1 modules   (1 inline 8-bit counters): 1 [0x76bf40, 0x76bf41), 
 INFO: Loaded 1 PC tables (1 PCs): 1 [0x545248,0x545258), 
 INFO: -max_len is not provided; libFuzzer will not generate inputs larger than 4096 bytes
 INFO: A corpus is not provided, starting from an empty corpus
 #2      INITED cov: 1 ft: 2 corp: 1/1b lim: 4 exec/s: 0 rss: 26Mb
 =================================================================
 ==15057==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x603000009da4 at pc 0x0000005299ba bp 0x7fffddf93d80 sp 0x7fffddf93d78
 READ of size 1 at 0x603000009da4 thread T0
     #0 0x5299b9 in test_string(char const*, unsigned long) /home/jeka/fuzzitdev/continuous-fuzzing-example/src/test_string.cpp:15:17
     #1 0x5298f0 in LLVMFuzzerTestOneInput /home/jeka/fuzzitdev/continuous-fuzzing-example/fuzz/fuzz_test_string.cpp:13:5
     #2 0x42ef2a in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/jeka/fuzzitdev/continuous-fuzzing-example/build/fuzz/fuzz_test_string+0x42ef2a)
     #3 0x42e4d5 in fuzzer::Fuzzer::RunOne(unsigned char const*, unsigned long, bool, fuzzer::InputInfo*, bool*) (/home/jeka/fuzzitdev/continuous-fuzzing-example/build/fuzz/fuzz_test_string+0x42e4d5)
     #4 0x430a4e in fuzzer::Fuzzer::MutateAndTestOne() (/home/jeka/fuzzitdev/continuous-fuzzing-example/build/fuzz/fuzz_test_string+0x430a4e)
     #5 0x431595 in fuzzer::Fuzzer::Loop(std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, fuzzer::fuzzer_allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > const&) (/home/jeka/fuzzitdev/continuous-fuzzing-example/build/fuzz/fuzz_test_string+0x431595)
     #6 0x424c53 in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/jeka/fuzzitdev/continuous-fuzzing-example/build/fuzz/fuzz_test_string+0x424c53)
     #7 0x44d472 in main (/home/jeka/fuzzitdev/continuous-fuzzing-example/build/fuzz/fuzz_test_string+0x44d472)
     #8 0x7fb0633e282f in __libc_start_main /build/glibc-LK5gWL/glibc-2.23/csu/../csu/libc-start.c:291
     #9 0x41d9c8 in _start (/home/jeka/fuzzitdev/continuous-fuzzing-example/build/fuzz/fuzz_test_string+0x41d9c8)
 
 0x603000009da4 is located 0 bytes to the right of 20-byte region [0x603000009d90,0x603000009da4)
 allocated by thread T0 here:
     #0 0x526b52 in operator new[](unsigned long) (/home/jeka/fuzzitdev/continuous-fuzzing-example/build/fuzz/fuzz_test_string+0x526b52)
     #1 0x42ee11 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/jeka/fuzzitdev/continuous-fuzzing-example/build/fuzz/fuzz_test_string+0x42ee11)
     #2 0x42e4d5 in fuzzer::Fuzzer::RunOne(unsigned char const*, unsigned long, bool, fuzzer::InputInfo*, bool*) (/home/jeka/fuzzitdev/continuous-fuzzing-example/build/fuzz/fuzz_test_string+0x42e4d5)
     #3 0x430a4e in fuzzer::Fuzzer::MutateAndTestOne() (/home/jeka/fuzzitdev/continuous-fuzzing-example/build/fuzz/fuzz_test_string+0x430a4e)
     #4 0x431595 in fuzzer::Fuzzer::Loop(std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, fuzzer::fuzzer_allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > const&) (/home/jeka/fuzzitdev/continuous-fuzzing-example/build/fuzz/fuzz_test_string+0x431595)
     #5 0x424c53 in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/jeka/fuzzitdev/continuous-fuzzing-example/build/fuzz/fuzz_test_string+0x424c53)
     #6 0x44d472 in main (/home/jeka/fuzzitdev/continuous-fuzzing-example/build/fuzz/fuzz_test_string+0x44d472)
     #7 0x7fb0633e282f in __libc_start_main /build/glibc-LK5gWL/glibc-2.23/csu/../csu/libc-start.c:291
 
 SUMMARY: AddressSanitizer: heap-buffer-overflow /home/jeka/fuzzitdev/continuous-fuzzing-example/src/test_string.cpp:15:17 in test_string(char const*, unsigned long)
 Shadow bytes around the buggy address:
   0x0c067fff9360: fd fa fa fa fd fd fd fa fa fa fd fd fd fa fa fa
   0x0c067fff9370: fd fd fd fa fa fa fd fd fd fa fa fa fd fd fd fa
   0x0c067fff9380: fa fa fd fd fd fa fa fa fd fd fd fa fa fa fd fd
   0x0c067fff9390: fd fa fa fa fd fd fd fa fa fa fd fd fd fa fa fa
   0x0c067fff93a0: fd fd fd fa fa fa fd fd fd fa fa fa fd fd fd fa
 =>0x0c067fff93b0: fa fa 00 00[04]fa fa fa fa fa fa fa fa fa fa fa
   0x0c067fff93c0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
   0x0c067fff93d0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
   0x0c067fff93e0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
   0x0c067fff93f0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
   0x0c067fff9400: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
 Shadow byte legend (one shadow byte represents 8 application bytes):
   Addressable:           00
   Partially addressable: 01 02 03 04 05 06 07 
   Heap left redzone:       fa
   Freed heap region:       fd
   Stack left redzone:      f1
   Stack mid redzone:       f2
   Stack right redzone:     f3
   Stack after return:      f5
   Stack use after scope:   f8
   Global redzone:          f9
   Global init order:       f6
   Poisoned by user:        f7
   Container overflow:      fc
   Array cookie:            ac
   Intra object redzone:    bb
   ASan internal:           fe
   Left alloca redzone:     ca
   Right alloca redzone:    cb
   Shadow gap:              cc
 ==15057==ABORTING
 MS: 2 InsertRepeatedBytes-CMP- DE: "FUZZING IS IMPORTANT"-; base unit: adc83b19e793491b1c6ea0fd8b46cd9f32e592fc
 0x46,0x55,0x5a,0x5a,0x49,0x4e,0x47,0x20,0x49,0x53,0x20,0x49,0x4d,0x50,0x4f,0x52,0x54,0x41,0x4e,0x54,
 FUZZING IS IMPORTANT
 artifact_prefix='./'; Test unit written to crash
 Base64: RlVaWklORyBJUyBJTVBPUlRBTlQ=
 ```
 
 We can see clearly the heap-buffer-overflow READ 1 byte memory bug and the exact line
 that triggeres. we also see the data that triggered the bug which is `FUZZING IS IMPORTANT`.
 
 The data is also saved to `./crash` and we can double test it with the main command line
 ```bash
 cat ./crash
 # FUZZING IS IMPORTANT
 ./src/test_string "FUZZING IS IMPORTANT"
 # you got it:)
 ```
 
 ## Continuous Fuzzing
 
 Fuzzit can be integrated to any CI as it's just involves additional step
 of compiling the fuzzers in your current CI just like you compile the tests and pushing
 the fuzzer binaries to fuzzit via fuzzit CLI.
 
 In this short tutorial we will use travis (See links to other oss integration for more examples)
 
 ## More Examples
 
 You can look in the following open-source examples integrating fuzzit with different CI and different
 build systemd
 
 * systemd - [travis](https://github.com/systemd/systemd/blob/master/.travis.yml), 
             [case-study](https://fuzzit.dev/2019/06/20/continuous-fuzzing-systemd-case-study/).
 * radare2 - [travis](https://github.com/radare/radare2/blob/master/.travis.yml)
 * pdfgen -  [circle](https://github.com/AndreRenaud/PDFGen/blob/master/.circleci/config.yml)