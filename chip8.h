#pragma once

#include<array>
#include<string>
#include "asm.h"

namespace gui
{
	class RegistersWindow;
	class FramebufferWindow;
	class StackWindow;
	class SettingsWindow;
}

namespace emu
{

using Framebuffer = std::array<uint8_t, 32 * 64>;
using Keyboard = std::array<bool, 16>;

class Chip8
{
public:
	Chip8(const std::string& rom);
	void emulate_cycle(const float delta_time);
	void update_keyboard(const Keyboard& keys);

	friend class gui::RegistersWindow;
	friend class gui::FramebufferWindow;
	friend class gui::StackWindow;
	friend class gui::SettingsWindow;

private:
	/* mapping binary opcode code to instructions */
	void execute();

	/* instruction set */
	void cls(); 
	void ret();
	void jp();
	void call_nnn();
	void se_vx_kk();
	void sne_vx_kk();
	void se_vx_vy();
	void ld_vx_kk();
	void add_vx_kk();
	void sub_vx_vy();
	void shr_vx();
	void subn_vx_vy();
	void shl_vx();
	void sne_vx_vy();
	void ld_i_nnn();
	void jp_v0_nnn();
	void rnd_vx_kk();
	void drw_vx_vy();
	void skp_vx();
	void sknp_vx();
	void ld_vx_dt();
	void add_i_vx();
	void ld_f_vx();
	void ld_b_vx();
	void ld_i_vx();
	void ld_vx_i();
	void ld_vx_k();
	void ld_dt_vx();
	void ld_st_vx();
	void ld_vx_vy();
	void or_vx_vy();
	void and_vx_vy();
	void xor_vx_vy();
	void add_vx_vy();

	/* virtual machine internal state */
	std::array<uint8_t, 4096> memory;
	std::array<uint8_t, 16> V; // general purpose registers
	uint16_t I; // register used to store memory adresses
	uint16_t pc; // program counter
	uint8_t sp; // stack pointer (start at 0x4E right after fontset)
	uint8_t st; // sound timer register
	uint8_t dt; // delay timer register
	Keyboard keyboard;
	Framebuffer framebuffer_;
	Asm::Opcode opcode_;
	Asm::Instruction inst_;
};

}



