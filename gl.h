#pragma once

#include <glad/glad.h>
#include <cstdint>
#include "chip8.h"

namespace chip8dev
{

    void GLAPIENTRY
        MessageCallback(GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar* message,
            const void* userParam);

#ifndef NDEBUG

#define CHIP8DEV_ASSERT(x) if(!(x)) __debugbreak();

#endif // DEBUG

#ifdef NDEBUG
#define CHIP8DEV_ASSERT(x) x;
#endif // NDEBUG

class Texture
{
public:
    ~Texture();
    static auto from_framebuffer(const Framebuffer& fb) -> Texture;

    GLuint id;
    int w;
    int h;
private:
    Texture() = default;
};

}


