#pragma once

#ifndef NDEBUG

#define CHIP8DEV_ASSERT(x) if(!(x)) __debugbreak();

#endif // DEBUG

#ifdef NDEBUG
#define CHIP8DEV_ASSERT(x) (void)0;
#endif // NDEBUG
