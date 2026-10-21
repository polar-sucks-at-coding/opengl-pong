#include "../headers/shader-program.h"

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

    //layout (location = 0) assigns this input to attribute location 0
    //a vertex attribute is a piece of per-vertex data, such as position or color
    // "in" means data comes into this shader
    // vec2 means a two-component vector, x and y here
    // void main() is the shader's entry point
    // gl_Position is a built-in output variable required by a vertex shader
    // vec4(aPosition, 0.0, 1.0) turns the two-component position into four components: x, y, z, and w and here z is 0.0 and w is 1.0

    const char* fragmentSource = R"(
        #version 330 core
        out vec4 outputColor;
        void main(){
            outputColor = vec4(1.0, 0.4, 0.1, 1);
        }
    )";

    //a fragment is a candidate piece of the image produced while OpenGL turns a shape into screen-sized pieces.
    //out vec4 outputColor declares the color that this shader will use


    // param 1 takes what type of shader to create
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
