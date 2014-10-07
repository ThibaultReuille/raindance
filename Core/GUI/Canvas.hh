#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Camera/Camera.hh>

class Canvas
{
public:
	typedef unsigned int ID;

	enum ConvolutionEffect
	{
		NORMAL = 0,
		BLUR,
		LAPLACIAN, // Edge Detection
		EMBOSS,
		SHARPNESS,
		DOUBLE,
		LAST
	};

	Canvas(unsigned int width, unsigned int height)
	{
		create(width, height);

		m_Camera2D.setOrthographicProjection(0.0f, (float)m_Width, 0.0f, (float)m_Height, 0.001f, 100.f);
		m_Camera2D.lookAt(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		m_Shader = ResourceManager::getInstance().loadShader("convolution", Resources_Shaders_convolution_vert, sizeof(Resources_Shaders_convolution_vert),
																			Resources_Shaders_convolution_frag, sizeof(Resources_Shaders_convolution_frag));
		//m_Shader->dump();

		m_Buffer << glm::vec3(0, 1, 0) << glm::vec2(0, 1);
		m_Buffer << glm::vec3(1, 1, 0) << glm::vec2(1, 1);
		m_Buffer << glm::vec3(1, 0, 0) << glm::vec2(1, 0);
		m_Buffer << glm::vec3(0, 0, 0) << glm::vec2(0, 0);

		m_Buffer.describe("a_Position", 3, GL_FLOAT, 5 * sizeof(GLfloat), 0);
		m_Buffer.describe("a_Texcoord", 2, GL_FLOAT, 5 * sizeof(GLfloat), 3 * sizeof(GLfloat));

		m_Buffer.generate(Buffer::STATIC);

		setConvolutionEffect(NORMAL);
	}

	~Canvas()
	{
		ResourceManager::getInstance().unload(m_Shader);

		unbind();
		destroy();
	}

	void reshape(unsigned int width, unsigned int height)
	{
		destroy();
		create(width, height);

		m_Camera2D.reshape(width, height);
		m_Camera2D.setOrthographicProjection(0.0f, (float)m_Width, 0.0f, (float)m_Height, 0.001f, 100.f);
		m_Camera2D.lookAt(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	}

	void create(unsigned int width, unsigned int height)
	{
		m_Width = width;
		m_Height = height;
		m_Components = 3;

		LOG("[CANVAS] Creating FrameBuffer Object %ux%u ...\n", m_Width, m_Height);

		// Create a texture object
		m_Texture = new Texture("FrameBuffer", m_Width, m_Height, 4);

		// Create a renderbuffer object to store depth info
#ifndef EMSCRIPTEN
		// NOTE : Not supported by WebGL
		glGenRenderbuffers(1, &m_RenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
#endif

		// Create a framebuffer object
		glGenFramebuffers(1, &m_FrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

		// Attach the texture to FBO color attachment point
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture->id(), 0);

#ifndef EMSCRIPTEN
		// Attach the renderbuffer to depth attachment point
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer);
#endif

		// Check FBO status
		m_Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (m_Status != GL_FRAMEBUFFER_COMPLETE)
			LOG("[CANVAS] FBO initialization failed !\n");

		// Switch back to window-system-provided framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void destroy()
	{
		LOG("[CANVAS] Destroying FrameBuffer Object %u (%ux%u) ...\n", m_FrameBuffer, m_Width, m_Height);
#ifndef EMSCRIPTEN
		glDeleteRenderbuffers(1, &m_RenderBuffer); // NOTE : Not supported by WebGL
#endif
		glDeleteFramebuffers(1, &m_FrameBuffer);

		SAFE_DELETE(m_Texture);
	}

	void bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
		glViewport(0, 0, m_Width, m_Height);
	}

	void unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void bindColorTexture(Texture* texture)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->id(), 0);
	}

	void dump(const char* filename, Texture* texture)
	{
		LOG("[FRAMEBUFFER] Screenshot\n");

		unsigned int components = 3;
		char* pixels = new char[texture->width() * texture->height() * components]; // NOTE : Forcing component number to 3

		glBindTexture(GL_TEXTURE_2D, texture->id());
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

		// NOTE : Flip Y
		for (unsigned int h = 0; h < texture->height() / 2; h++)
			for (unsigned int w = 0; w < texture->width(); w++)
				for (unsigned int c = 0; c < components; c++)
				{
					unsigned int offset1 = (w + texture->width() * h) * components + c;
					unsigned int offset2 = (w + texture->width() * (texture->height() - h - 1)) * components  + c;
					char t = pixels[offset1];
					pixels[offset1] = pixels[offset2];
					pixels[offset2] = t;
				}

		stbi_write_tga(filename, texture->width(), texture->height(), components, pixels);

		delete[] pixels;
	}

	void dump(const char* filename)
	{
		dump(filename, m_Texture);
	}

	void draw(Context* context)
	{
		unsigned short int indices[] = { 0, 1, 2, 0, 2, 3 };

		m_Shader->use();
		m_Shader->uniform("u_ModelViewProjection").set(m_Camera2D.getViewProjectionMatrix() * glm::scale(glm::mat4(), glm::vec3(m_Width, m_Height, 1)));
		m_Shader->uniform("u_Texture").set(*m_Texture);
		m_Shader->uniform("u_TextureWidth").set((float)m_Width);
		m_Shader->uniform("u_TextureHeight").set((float)m_Height);

		m_Shader->uniform("u_Convolution").set(m_ConvolutionMatrix);

		context->geometry().bind(m_Buffer, *m_Shader);
		context->geometry().drawElements(GL_TRIANGLES, sizeof(indices) / sizeof(short int), GL_UNSIGNED_SHORT, indices);
		context->geometry().unbind(m_Buffer);
	}

	void draw(Context* context, Texture* texture)
	{
		unsigned short int indices[] = { 0, 1, 2, 0, 2, 3 };

		m_Shader->use();
		m_Shader->uniform("u_ModelViewProjection").set(m_Camera2D.getViewProjectionMatrix() * glm::scale(glm::mat4(), glm::vec3(m_Width, m_Height, 1)));
		m_Shader->uniform("u_Texture").set(*texture);
		m_Shader->uniform("u_TextureWidth").set((float)texture->width());
		m_Shader->uniform("u_TextureHeight").set((float)texture->height());

		m_Shader->uniform("u_Convolution").set(m_ConvolutionMatrix);

		context->geometry().bind(m_Buffer, *m_Shader);
		context->geometry().drawElements(GL_TRIANGLES, sizeof(indices) / sizeof(short int), GL_UNSIGNED_SHORT, indices);
		context->geometry().unbind(m_Buffer);
	}

	void setConvolutionEffect(unsigned int id)
	{
		switch(id)
		{
		case BLUR:
			m_ConvolutionMatrix = glm::mat3(1.0, 2.0, 1.0, 2.0, 4.0, 2.0, 1.0, 2.0, 1.0) / 16.0f;
			break;
		case LAPLACIAN:
			m_ConvolutionMatrix = glm::mat3(0.0, 1.0, 0.0, 1.0, -4.0, 1.0, 0.0, 1.0, 0.0);
			break;
		case EMBOSS:
			m_ConvolutionMatrix = glm::mat3(2.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, -1.0);
			break;
		case SHARPNESS:
			m_ConvolutionMatrix = glm::mat3(-1.0, -1.0, -1.0, -1.0, 9.0, -1.0, -1.0, -1.0, -1.0);
			break;
		case DOUBLE:
			m_ConvolutionMatrix = glm::mat3(0.0, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case NORMAL:
		default:
			m_ConvolutionMatrix = glm::mat3(0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0);
			break;
		}
	}

private:
	GLuint m_Width;
	GLuint m_Height;
	GLuint m_Components;
	Texture* m_Texture;
	GLuint m_RenderBuffer;
	GLuint m_FrameBuffer;
	GLenum m_Status;

	Camera m_Camera2D;
	Shader::Program* m_Shader;
	Buffer m_Buffer;
	glm::mat3 m_ConvolutionMatrix;
};
