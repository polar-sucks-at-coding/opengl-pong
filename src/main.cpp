#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

//A shader is a program that runs on the GPU as part of the drawing
GLuint compileShader(GLenum type, const char* source){
    //glCreateShader creates a shader object with the type passed in the argument and returns a handler that I'm storing in "shader"
    GLuint shader =  glCreateShader(type);
    //glShaderSource gives the source code for our shader to our shader variable
    // param 1 is the object to give the source to
    // param 2 is how many strings of source code we're passing
    // param 3 is a reference to the source
    // param 4 is a custom end string but I don't need one so I can just do nullptr
    glShaderSource(shader, 1, &source, nullptr);
    //compiles the source code we passed to be held in the shader variable
    glCompileShader(shader);

    GLint succeeded = 0;
    //asks for a specific property of the shader, in this case its compile status, and writes it
    // param 1 is the shader to check
    // param 2 is what to check for about that shader
    // param 3 is a reference to the variable to write the result to
    glGetShaderiv(shader, GL_COMPILE_STATUS, &succeeded);

    if (!succeeded){
        char log[1024];
        //asking for a diagnostic log about the shader
        // param 1 is the shader to ask about
        // param 2 is the size of the char array for it to write to
        // param 3 is if you want the exact amount of characters written, in this case I don't so I can put nullptr
        // param 4 is the char array for it to write to
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        std::cerr << "Shader compilation failed." << "\n" << log << "\n";
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}
GLuint createProgram(){
    const char* vertexSource = R"(
        #version 330 core
        layout (location = 0) in vec2 aPosition;

        void main(){
            gl_Position = vec4(aPosition, 0.0, 1.0);
        }
    )";

    const char* fragmentSource = R"(
        #version 330 core
        out vec4 outputColor;
        void main(){
            outputColor = vec4(1.0, 0.4, 0.1, 1);
        }
    )";

    //param 1 takes what type of shader to create
    // param 2 takes the source for that shader
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    if (!vertexShader) return 0;

    //same thing
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (!fragmentShader){
        glDeleteShader(vertexShader);
        return 0;
    }

    //creates an empty program object
    GLuint program = glCreateProgram();
    //associates a shader with the program I want
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    //links the attached shaders with the program
    glLinkProgram(program);

    //shaders have been linked to the program, so they can be safely deleted
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint succeeded = 0;
    //another use of this function, this time asking the link status
    glGetProgramiv(program, GL_LINK_STATUS, &succeeded);

    if (!succeeded){
        char log[1024];
        glGetProgramInfoLog(program, sizeof(log), nullptr, log);
        std::cerr << "Shader program linking failed" << "\n" << log << "\n";
        glDeleteProgram(program);
        return 0;
    }

    return program;
}


int main(){
    if (!glfwInit()) return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "opengl-pong", nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    float vertices[] = {
        -0.6f, -0.5f,
        0.6f, -0.5f,
        0.0f, 0.5f,
    };

    GLuint vao;
    GLuint vbo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Describe how the position data is laid out in the VBO.
    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr
    );
    glEnableVertexAttribArray(0);

    GLuint program = createProgram();
    if (!program){
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    while (!glfwWindowShouldClose(window)){
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
