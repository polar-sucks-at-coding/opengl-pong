#! /bin/bash
gcc -Iinclude -c src/glad.c -o target/glad.o &&
    g++ -Iinclude src/main.cpp target/glad.o -lglfw -lGL -ldl -pthread -o target/opengl-pong &&
    ./target/opengl-pong
