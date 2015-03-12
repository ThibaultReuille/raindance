#pragma once

#include <raindance/Core/Headers.hh>

static void checkGLErrors(bool mute = false)
{
	static int failures = 0;
	if (failures >= 3)
		return;

	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		if (mute)
			continue;

		std::cerr << "OpenGL error (" << err << ") : ";
		failures++;

		switch(err)
		{
		case  GL_INVALID_ENUM:
			std::cerr << "GL_INVALID_ENUM" << std::endl;
			break;
		case  GL_INVALID_VALUE:
			std::cerr << "GL_INVALID_VALUE" << std::endl;
			break;
		case  GL_INVALID_OPERATION:
			std::cerr << "GL_INVALID_OPERATION" << std::endl;
			break;
		case  GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;
			break;
		case  GL_OUT_OF_MEMORY:
			std::cerr << "GL_OUT_OF_MEMORY" << std::endl;
			break;
		case  GL_STACK_UNDERFLOW:
			std::cerr << "GL_STACK_UNDERFLOW" << std::endl;
			break;
		case  GL_STACK_OVERFLOW:
			std::cerr << "GL_STACK_OVERFLOW" << std::endl;
			break;
		default:
			std::cerr << "Unknown Error !" << std::endl;
		}
	}
}

