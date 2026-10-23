#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "../headers/shader-program.h"
#include "../headers/rectangle.h"
#include <iostream>
#include <vector>

void rectangleTest(){
    Rectangle* rect = new Rectangle(0.5f, 0.5f, 0.0f, 0.0f, 0);

    std::cout << "width: " << rect->w << " " << "height: " << rect->h << " " << "center x: " << rect->center[0] << " " << "center y: " << rect->center[1] << " " << "rotation (degrees): " << rect->rotation << std::endl;
    for (int i = 0; i < 2; i++){
        if (i == 0) std::cout << "x: ";
        else std::cout << "y: ";
        std::cout << "l: " << rect->l[i] << " " << "r: " << rect->r[i] << " " << "t: " << rect->t[i] << " " << "b: " << rect->b[i] << std::endl;
    }

    rect->assignVerticesForDrawing();

    for (int i = 0; i < 12; i++){
        std::cout << rect->vertices[i] << std::endl;
    }
}

int main(){

    Rectangle* rect = new Rectangle(0.5f, 0.5f, 0.0f, 0.0f, 0);
    rect->assignVerticesForDrawing();
    float vertices[12];

    for (int i = 0; i < 12; i++){
        vertices[i] = rect->vertices[i];
    }

    if (!glfwInit()) return 1;

    //tells it the major.minor version we're using
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //tells it which profile we want honestly I have no idea how they're different but I just used core
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //creating a window object with its
    // param 1: width
    // param 2: height
    // param 3: the window's name
    // param 4: is for fullscreen monitors apparently but there's no need to get into all that
    // param 5: is for sharing opengl resources with another window which is also entirely unnecessary so nullptr
    GLFWwindow* window = glfwCreateWindow(800, 600, "opengl-pong", nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        return 1;
    }

    //setting current context to our window object so that future function calls target it
    glfwMakeContextCurrent(window);

    //glfwGetProcAddress asks the system for an opengl function by name
    // gladLoadLoader uses that lookup function to load OpenGL
    // the cast (GLADloadproc) converts the GLFW function pointer that we got from glfwGetProcAddress into the callback type that GLAD expects
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    //vao stands for "vertex array object" and it remembers how vertex data is arranged and which buffers apply it
    GLuint vao;
    //vbo stands for "vertex buffer object" and stores the actual vertex data for OpenGL
    GLuint vbo;

    //generating handles for both our declared variables
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    //a "bind" selects an OpenGL object as the current object for a particular purpose and later commands can use it
    // glBindVertexArray(vao) binds vao, basically "selects" it
    glBindVertexArray(vao);
    //GL_ARRAY_BUFFER is a binding target for a buffer that supplies vertex attributes
    //glBindBuffer selects vbo for that target
    // basically tells openGL that for commands using GL_ARRAY_BUFFER it should target vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //glBufferData operates on the buffer we bound to GL_ARRAY_BUFFER (which is vbo)
    // glBufferData replaces the GL_ARRAY_BUFFER with an array
    // param 1: the buffer
    // param 2: size of vertices array
    // param 3: the vertices array
    // param 4: is a hint telling opengl that we're gonna draw one thing over and over (hence "static")
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Describe how the position data is laid out in the VBO.
    // param 1: uses attribute location 0 [matching layout (location = 0) in the vertex shader]
    // param 2: the amount of components each position has, in this case 2 for x and y
    // param 3: the type of components, float here
    // param 4: to normalize the values or not, here, no
    // param 5: the size of each vertex, here because each vertex just holds 2 floats so we can do the size of 2 floats
    // param 6: offset for the first byte in the VBO, idfk what that is but nullptr works
    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr
    );
    //enables attribute location 0 we defined in the vertex shaders so that opengl will draw it I guess
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
        //gets the size of the window and writes it to the width and height variables
        glfwGetFramebufferSize(window, &width, &height);
        //sets the rectangular area that OpenGL draws into, x starts at 0 and y starts at 0, and its width and height match the drawable area
        // param 1: where x starts
        // param 2: where y starts
        // param 3: the drawable area's width
        // param 4: the drawable area's height
        glViewport(0, 0, width, height);

        //sets GL_COLOR_BUFFER_BIT to these RGBA values
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        //paints the background
        glClear(GL_COLOR_BUFFER_BIT);

        //selects the program for subsequent drawing commands
        glUseProgram(program);
        //selects the vao that remembers how to write vertex data
        glBindVertexArray(vao);
        //tells opengl to draw using the currently selected program and VAO
        // param 1: primitive type
        // param 2: the index for the first vertex
        // param 3: number of vertices to use
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //swaps the buffer window with the current one
        glfwSwapBuffers(window);
        //processes pending events
        glfwPollEvents();
    }

    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
