#pragma once

#include <glad/glad.h>
#include <cstdint>

namespace chip8dev::gl
{


auto clear_errors() -> void;
auto log_call(const char* func, const char* file, int line) -> bool;

#ifndef NDEBUG

#define CHIP8DEV_ASSERT(x) if(!(x)) __debugbreak();
#define CHIP8DEV_GL_CALL(x) clear_errors();\
    x;\
    CHIP8DEV_ASSERT(log_call(#x, __FILE__, __LINE__))

#endif // DEBUG

#ifdef NDEBUG
#define CHIP8DEV_GL_CALL(x) x 
#endif // NDEBUG

class Texture
{
public:
    ~Texture();

    static auto from_framebuffer(const uint8_t* fb_data, int fb_w, int fb_h) -> Texture;
    auto update_data(const uint8_t* fb_data, int fb_w, int fb_h) -> void;
    auto id() -> GLuint { return id_; }
    auto w() -> int { return w_; }
    auto h() -> int { return h_; }

private:
    Texture() = default;

    GLuint id_;
    int w_;
    int h_;
};

}


