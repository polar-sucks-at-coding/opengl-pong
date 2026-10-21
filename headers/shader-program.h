#pragma once
#include <iostream>
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>

GLuint compileShader(GLenum type, const char* source);
GLuint createProgram();
