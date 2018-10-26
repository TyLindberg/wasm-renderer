#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES3/gl3.h>

const char* vertSource = "\
#version 300 es\n\
precision mediump float;\n\
layout (location = 0) in vec3 aPos;\n\
\n\
void main() {\n\
    gl_Position = vec4(aPos, 1.0);\n\
}\n\
";
const char* fragSource = "\
#version 300 es\n\
precision mediump float;\n\
out vec4 fragColor;\n\
\n\
void main() {\n\
    fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n\
}\n\
";

int main() {
    EmscriptenWebGLContextAttributes attributes = EmscriptenWebGLContextAttributes();
    attributes.majorVersion = 2;
    auto context = emscripten_webgl_create_context(0, &attributes);
    if (emscripten_webgl_make_context_current(context) != EMSCRIPTEN_RESULT_SUCCESS) {
        emscripten_log(EM_LOG_CONSOLE, "Error making WebGL context active");
    } else {
        emscripten_log(EM_LOG_CONSOLE, "Context created successfully!");
    }

    emscripten_set_element_css_size(0, 800, 600);
    emscripten_set_canvas_element_size(0, 800, 600);
    glViewport(0, 0, 800, 600);

    glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Create vertex array object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create geometry
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Link vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Create shaders
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertSource, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        emscripten_log(EM_LOG_CONSOLE, "Vertex shader error: %s", infoLog);
        return 1;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        emscripten_log(EM_LOG_CONSOLE, "Fragment shader error: %s",infoLog);
        return 1;
    }

    // Create shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        emscripten_log(EM_LOG_CONSOLE, infoLog);
        return 1;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}