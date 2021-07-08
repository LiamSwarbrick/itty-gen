@echo off

gcc -std=c99 *.c -o ./itty-gen.exe -Iraylib -Lraylib -lraylib

@echo on
