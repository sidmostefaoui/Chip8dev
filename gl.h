#pragma once

#include <glad/glad.h>
#include <cstdint>
#include "chip8.h"

namespace gl
{

    void GLAPIENTRY
        MessageCallback(GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar* message,
            const void* userParam);

}


