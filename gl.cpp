#include "gl.h"
#include <iostream>
#include <vector>

namespace chip8dev
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

    Texture::~Texture()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &id);
    }

    auto rgb_from_framebuffer(const Framebuffer& fb) -> std::vector<uint8_t>
    {
        std::vector<uint8_t> rgb;
        size_t w = fb[0].size();
        size_t h = fb.size();
        int n_channels = 3;
        size_t rgb_sz = w * h * n_channels;
        rgb.reserve(rgb_sz);

        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < w; j++)
            {
                if (fb[i][j] == 1) //WHITE
                {
                    rgb.push_back(255); // RED
                    rgb.push_back(255); // GREEN
                    rgb.push_back(255); // BLUE
                }
                else // BLACK
                {
                    rgb.push_back(0); // RED
                    rgb.push_back(0); // GREEN
                    rgb.push_back(0); // BLUE
                }
            }
        }

        return rgb;
    }

    auto Texture::from_framebuffer(const Framebuffer& fb) -> Texture
    {
        Texture tex = {};
        tex.w = static_cast<int>(fb[0].size());
        tex.h = static_cast<int>(fb.size());

        glGenTextures(1, &tex.id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex.id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        std::vector<uint8_t> rgb = rgb_from_framebuffer(fb);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, tex.w, tex.h, 0,
            GL_RED, GL_UNSIGNED_BYTE, rgb.data());

        return tex;
    }
 
}
