#pragma once

#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES

#ifdef __APPLE__
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <Glut/glut.h>
# include <OpenCL/opencl.h>
#else
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glut.h>
# include <CL/cl.h>
#endif

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
#include <iomanip>
#include <unordered_map>
#include <set>
#include <fstream>
#include <sstream>

#include "Lib/stb_image.c"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Lib/stb_image_write.h"

#include "Core/Log.hh"
#include "Core/Resources/ResourceManager.hh"

const glm::vec3 NEON_PINK =       glm::vec3(0.96, 0.20, 0.66);
const glm::vec3 SKY_BLUE =        glm::vec3(0.50, 0.79, 1.00);
const glm::vec3 LOVE_RED =        glm::vec3(0.89, 0.11, 0.09);
const glm::vec3 DRAGON_GREEN =    glm::vec3(0.42, 0.98, 0.57);
const glm::vec3 PUMPKIN_ORANGE =  glm::vec3(0.97, 0.44, 0.09);
const glm::vec3 GUNMETAL =        glm::vec3(0.17, 0.20, 0.22);
const glm::vec3 INDIGO =          glm::vec3(0.29, 0.00, 0.50);
const glm::vec3 COFFEE_BROWN =    glm::vec3(0.43, 0.30, 0.21);

#define SAFE_DELETE(ptr) 	\
	if ((ptr) != NULL) 		\
	{						\
		delete (ptr);		\
		ptr = NULL;			\
	}						\

// --- Packed resources ---

// Shaders

#include "Pack/Shaders/ray.vert"
#include "Pack/Shaders/ray.frag"

#include "Pack/Shaders/convolution.vert"
#include "Pack/Shaders/convolution.frag"

#include "Pack/Shaders/wallpaper.vert"
#include "Pack/Shaders/wallpaper.frag"

#include "Pack/Shaders/text.vert"
#include "Pack/Shaders/text.frag"

#include "Pack/Shaders/icon.vert"
#include "Pack/Shaders/icon.frag"

#include "Pack/Shaders/environment.vert"
#include "Pack/Shaders/environment.frag"

#include "Pack/Shaders/widget.vert"
#include "Pack/Shaders/widget.frag"

#include "Pack/Shaders/slider.vert"
#include "Pack/Shaders/slider.frag"

#include "Pack/Shaders/checkbox.vert"
#include "Pack/Shaders/checkbox.frag"

#include "Pack/Shaders/box.vert"
#include "Pack/Shaders/box.frag"

#include "Pack/Shaders/pointcloud.vert"
#include "Pack/Shaders/pointcloud.frag"

#include "Pack/Shaders/isovolume.vert"
#include "Pack/Shaders/isovolume.frag"

// Textures

#include "Pack/Textures/umbrella-logo.png"
#include "Pack/Textures/script.png"
#include "Pack/Textures/mark.png"

