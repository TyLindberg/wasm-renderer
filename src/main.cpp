#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES2/gl2.h>

int main() {
    EmscriptenWebGLContextAttributes attributes = EmscriptenWebGLContextAttributes();
    attributes.majorVersion = 2;
    auto context = emscripten_webgl_create_context(0, &attributes);
    if (emscripten_webgl_make_context_current(context) != EMSCRIPTEN_RESULT_SUCCESS) {
        emscripten_log(EM_LOG_CONSOLE, "Error making WebGL context active");
    } else {
        emscripten_log(EM_LOG_CONSOLE, "Context created successfully!");
    }

    glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}