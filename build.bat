cls
@echo off
echo Building!
gcc -g httpserver.c -o httpserver -lws2_32

echo Running!
httpserver.exe
