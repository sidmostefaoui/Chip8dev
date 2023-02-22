#include "gl.h"
#include <iostream>
#include <vector>

namespace chip8dev
{

namespace gl
{
    auto clear_errors() -> void
    {
        while (glGetError() != GL_NO_ERROR);
    }

    auto log_call(const char* func, const char* file, int line) -> bool
    {
        bool success = true;
        while (auto error = glGetError()) {
            std::cout << "[OPENGL] error " << "0x0" << std::hex << error
                << ": line " << line << ": " << func << ": " << file << "\n";
            success = false;
        }
        return success;
    }

    Texture::~Texture()
    {
        CHIP8DEV_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
        CHIP8DEV_GL_CALL(glDeleteTextures(1, &id_));
    }

    auto rgb_from_framebuffer(const uint8_t* fb_data, int fb_w, int fb_h) -> std::vector<uint8_t>
    {
        std::vector<uint8_t> rgb;
        rgb.reserve(static_cast<size_t>(fb_w) * fb_h * 3);

        for (int i = 0; i < fb_w * fb_h; i++)
        {
            if (fb_data[i] == 1) //WHITE
            {
                rgb.emplace_back(255); // RED
                rgb.emplace_back(255); // GREEN
                rgb.emplace_back(255); // BLUE
            }
            else // BLACK
            {
                rgb.emplace_back(0); // RED
                rgb.emplace_back(0); // GREEN
                rgb.emplace_back(0); // BLUE
            }
        }

        return rgb;
    }

    auto Texture::from_framebuffer(const uint8_t* fb_data, int fb_w, int fb_h) -> Texture
    {
        auto tex = Texture();
        tex.w_ = fb_w;
        tex.h_ = fb_h;

        CHIP8DEV_GL_CALL(glGenTextures(1, &tex.id_));
        CHIP8DEV_GL_CALL(glActiveTexture(GL_TEXTURE0));
        CHIP8DEV_GL_CALL(glBindTexture(GL_TEXTURE_2D, tex.id_));

        CHIP8DEV_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        CHIP8DEV_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        CHIP8DEV_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        CHIP8DEV_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        std::vector<uint8_t> rgb = rgb_from_framebuffer(fb_data, fb_w, fb_h);

        CHIP8DEV_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
            fb_w, fb_h, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb.data()));

        return tex;
    }

    auto Texture::update_data(const uint8_t* fb_data, int fb_w, int fb_h) -> void
    {
        CHIP8DEV_GL_CALL(glDeleteTextures(1, &id_));

        w_ = fb_w;
        h_ = fb_h;

        CHIP8DEV_GL_CALL(glGenTextures(1, &id_));
        CHIP8DEV_GL_CALL(glActiveTexture(GL_TEXTURE0));
        CHIP8DEV_GL_CALL(glBindTexture(GL_TEXTURE_2D, id_));

        CHIP8DEV_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        CHIP8DEV_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        CHIP8DEV_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        CHIP8DEV_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        std::vector<uint8_t> rgb = rgb_from_framebuffer(fb_data, fb_w, fb_h);

        CHIP8DEV_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
            fb_w, fb_h, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb.data()));
    }

 
}
}
