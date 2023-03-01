#include "FramebufferWindow.h"
#include "imgui/imgui.h"
#include <vector>

namespace gui
{
	FramebufferWindow::FramebufferWindow(const emu::Chip8& chip8)
		: chip8_(chip8), tex_id_(), tex_zoom_(8), tex_w_(64), tex_h_(32)
	{
		glGenTextures(1, &tex_id_);
		glBindTexture(GL_TEXTURE_2D, tex_id_);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	FramebufferWindow::~FramebufferWindow()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &tex_id_);
	}
	

	auto FramebufferWindow::update() -> void
	{
		glBindTexture(GL_TEXTURE_2D, tex_id_);

		/* create rgb buffer array from framebuffer */
		std::vector<uint8_t> rgb;
		rgb.reserve((size_t)tex_w_ * tex_h_ * 3);

		for (auto& pixel : chip8_.framebuffer_)
		{
			if (pixel == 1) //WHITE
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

		/* copy rgb buffer array to texture data */
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_w_, tex_h_,
			0, GL_RGB, GL_UNSIGNED_BYTE, rgb.data());

		glBindTexture(GL_TEXTURE_2D, 0);

	}

	auto FramebufferWindow::render() -> void
	{
		update();
		ImGui::Begin("Framebuffer");
		{
			ImVec2 img_dim = {(float)tex_w_ * tex_zoom_, (float)tex_h_ * tex_zoom_ };
			ImGui::Image((void*)(intptr_t)tex_id_, img_dim);
		}
		ImGui::End();
	}
}
