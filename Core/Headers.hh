#pragma once

// ----- External Libraries ---

#ifdef __APPLE__
# define GL_GLEXT_PROTOTYPES
# define EGL_EGLEXT_PROTOTYPES
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <Glut/glut.h>
# ifndef EMSCRIPTEN
#  include <OpenCL/opencl.h>
# endif
#endif

#ifdef __linux__
# define GL_GLEXT_PROTOTYPES
# define EGL_EGLEXT_PROTOTYPES
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glut.h>
# ifndef EMSCRIPTEN
#  include <CL/cl.h>
# endif
#endif

#if defined(_MSC_VER)

/*
# undef NEAR
# undef near
# undef FAR
# undef far
*/
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
# include <GL/glut.h>
# ifndef EMSCRIPTEN
#  include <CL/cl.h>
# endif
#endif

#define GLM_SWIZZLE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/norm.hpp>

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
	ptr = NULL;       \
    }                     \

#define RANDOM_FLOAT(A, B)                                           \
    ((((B) - (A)) * (static_cast<float>(rand()) / RAND_MAX)) + (A))  \

#define HEX_COLOR(C)                                                \
    glm::vec3(static_cast<float>(((C) & 0xFF0000) >> 16) / 255.0f,  \
              static_cast<float>(((C) & 0x00FF00) >> 8) / 255.0f,   \
              static_cast<float>(((C) & 0x0000FF) >> 0) / 255.0f)   \

const glm::vec3 BLACK =           HEX_COLOR(0x000000);
const glm::vec3 WHITE =           HEX_COLOR(0xFFFFFF);
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
#include <raindance/Resources/Fonts/Fonts.hh>

// ----- Others -----

#include <raindance/Core/Log.hh>
#include <raindance/Core/Resources/ResourceManager.hh>
