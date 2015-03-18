#pragma once

// ----- External Libraries ---

#ifdef __APPLE__
# define GL_GLEXT_PROTOTYPES
# define EGL_EGLEXT_PROTOTYPES
# include <GL/glew.h>
# include <GLFW/glfw3.h>
# include <OpenGL/OpenGL.h>
# include <OpenCL/opencl.h>
# include <OpenCL/cl_gl.h>
#endif

#ifdef __linux__
# define GL_GLEXT_PROTOTYPES
# define EGL_EGLEXT_PROTOTYPES
# include <GL/glew.h>

# include <GLFW/glfw3.h>
# define GLFW_EXPOSE_NATIVE_X11
# define GLFW_EXPOSE_NATIVE_GLX
# include <GLFW/glfw3native.h>

# include <CL/cl.h>
# include <CL/cl_gl.h>
#endif

#ifdef EMSCRIPTEN
# define GL_GLEXT_PROTOTYPES
# define EGL_EGLEXT_PROTOTYPES
# include <GL/glew.h>
# include <GLFW/glfw3.h>
#endif

#if defined(_MSC_VER)

# define M_E        2.71828182845904523536   // e
# define M_LOG2E    1.44269504088896340736   // log2(e)
# define M_LOG10E   0.434294481903251827651  // log10(e)
# define M_LN2      0.693147180559945309417  // ln(2)
# define M_LN10     2.30258509299404568402   // ln(10)
# define M_PI       3.14159265358979323846   // pi
# define M_PI_2     1.57079632679489661923   // pi / 2
# define M_PI_4     0.785398163397448309616  // pi / 4
# define M_1_PI     0.318309886183790671538  // 1 / pi
# define M_2_PI     0.636619772367581343076  // 2 / pi
# define M_2_SQRTPI 1.12837916709551257390   // 2 / sqrt(pi)
# define M_SQRT2    1.41421356237309504880   // sqrt(2)
# define M_SQRT1_2  0.707106781186547524401  // 1 / sqrt(2)

# define GL_GLEXT_PROTOTYPES
# include <windows.h>
# include <GL/glew.h>
# include <GLFW/glfw3.h>
# include <CL/cl.h>
# include <CL/cl_gl.h>
#endif

#define GLM_SWIZZLE
#define GLM_FORCE_RADIANS

#include <raindance/Lib/glm-0.9.5.4/glm/glm.hpp>
#include <raindance/Lib/glm-0.9.5.4/glm/gtc/matrix_transform.hpp>
#include <raindance/Lib/glm-0.9.5.4/glm/gtc/type_ptr.hpp>
#include <raindance/Lib/glm-0.9.5.4/glm/gtx/quaternion.hpp>
#include <raindance/Lib/glm-0.9.5.4/glm/gtx/norm.hpp>

#include <string>
#include <limits>
#include <vector>
#include <stack>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <strstream>
#include <iomanip>
#include <unordered_map>
#include <set>
#include <fstream>
#include <sstream>
#include <exception>

#ifndef _WIN32
# include <sys/time.h>
#else
# include <sys/timeb.h>
#endif

#include <raindance/Lib/stb_image.c>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <raindance/Lib/stb_image_write.h>

#include <raindance/Lib/picojson.h>

// ----- Macros / Constants Definitions ----

#define SAFE_DELETE(ptr)  \
    if ((ptr) != NULL)    \
    {                     \
        delete (ptr);     \
		ptr = NULL;           \
    }                     \

#define RANDOM_FLOAT(A, B)                                          \
    ((((B) - (A)) * (static_cast<float>(rand()) / RAND_MAX)) + (A)) \

#define RANDOM_INT(A, B)                                            \
    ((A) + rand() % ((B) - (A) + 1))							                	\


#define HEX_COLOR(C)                                                \
    glm::vec3(static_cast<float>(((C) & 0xFF0000) >> 16) / 255.0f,  \
              static_cast<float>(((C) & 0x00FF00) >> 8) / 255.0f,   \
              static_cast<float>(((C) & 0x0000FF) >> 0) / 255.0f)   \

const glm::vec3 BLACK =           HEX_COLOR(0x000000);
const glm::vec3 WHITE =           HEX_COLOR(0xFFFFFF);
const glm::vec3 RED =             HEX_COLOR(0xFF0000);
const glm::vec3 GREEN =           HEX_COLOR(0x00FF00);
const glm::vec3 BLUE =            HEX_COLOR(0x0000FF);

const glm::vec3 AQUAMARINE =      HEX_COLOR(0x4E78A0);
const glm::vec3 NEON_PINK =       glm::vec3(0.96, 0.20, 0.66);
const glm::vec3 SKY_BLUE =        glm::vec3(0.50, 0.79, 1.00);
const glm::vec3 LOVE_RED =        glm::vec3(0.89, 0.11, 0.09);
const glm::vec3 DRAGON_GREEN =    glm::vec3(0.42, 0.98, 0.57);
const glm::vec3 PUMPKIN_ORANGE =  glm::vec3(0.97, 0.44, 0.09);
const glm::vec3 GUNMETAL =        glm::vec3(0.17, 0.20, 0.22);
const glm::vec3 INDIGO =          glm::vec3(0.29, 0.00, 0.50);
const glm::vec3 COFFEE_BROWN =    glm::vec3(0.43, 0.30, 0.21);

// ----- Packed Resources -----

#include <raindance/Pack.hh>
#include <raindance/Assets/Fonts/Fonts.hh>

// ----- Others -----

#include <raindance/Core/Log.hh>
#include <raindance/Core/Resources/ResourceManager.hh>
