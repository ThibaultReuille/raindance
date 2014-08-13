#pragma once

#include <raindance/Core/Headers.hh>

#define LOGGING

#ifdef LOGGING
# define LOG(fmt, ...) do { printf(fmt, ##__VA_ARGS__); } while(false)
#else
# define LOG(fmt, ...) do { } while(false)
#endif

