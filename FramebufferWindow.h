#include "glad/glad.h"
#include "chip8.h"

namespace gui
{
	class FramebufferWindow
	{
	public:
		FramebufferWindow(const emu::Chip8& chip8);
		~FramebufferWindow();
		auto render() -> void;

	private:
		auto update() -> void;

		const emu::Chip8& chip8_;
		GLuint tex_id_;
		int tex_zoom_;
		int tex_w_;
		int tex_h_;
	};
}