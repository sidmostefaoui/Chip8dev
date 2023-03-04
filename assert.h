#pragma once

#ifndef NDEBUG

#define ASSERT(x) if(!(x)) __debugbreak();

#endif // DEBUG

#ifdef NDEBUG
#define ASSERT(x) (void)0;
#endif // NDEBUG
