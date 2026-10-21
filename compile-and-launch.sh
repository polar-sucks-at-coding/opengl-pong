#! /bin/bash
gcc -Iinclude -c src/glad.c -o target/glad.o &&
    g++ -Iinclude src/*.cpp target/glad.o -lglfw -lGL -ldl -pthread -o target/opengl-pong &&
    ./target/opengl-pong
