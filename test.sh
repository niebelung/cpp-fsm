#! /bin/sh

echo -e '\nBuild with g++'
command -v g++ && make gcc; ./test_gcc

echo -e '\nBuild with clang++'
command -v clang++ && make clang; ./test_clang

echo -e '\nBuild with arm-none-eabi-g++'
command -v arm-none-eabi-g++ && command -v qemu-arm && make arm; qemu-arm ./test_arm
