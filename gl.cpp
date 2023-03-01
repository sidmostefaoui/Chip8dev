#include "gl.h"
#include <iostream>
#include <vector>

namespace gl
{
    void GLAPIENTRY
        MessageCallback(GLenum,
            GLenum type,
            GLuint,
            GLenum severity,
            GLsizei,
            const GLchar* message,
            const void*)
    {
        printf("GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
       
        __debugbreak();
    }
}
