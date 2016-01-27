#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Camera/Camera.hh>

class Canvas
{
public:
	typedef unsigned int ID;

	Canvas(unsigned int width, unsigned int height)
	{
		create(width, height);

		m_Camera2D.setOrthographicProjection(0.0f, (float)m_Width, 0.0f, (float)m_Height, 0.001f, 100.f);
		m_Camera2D.lookAt(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		m_Shader = ResourceManager::getInstance().loadShader("canvas", 
			Assets_Shaders_canvas_vert, sizeof(Assets_Shaders_canvas_vert),
			Assets_Shaders_canvas_frag, sizeof(Assets_Shaders_canvas_frag));
		m_Shader->dump();

		m_Buffer << glm::vec3(0, 0, 0) << glm::vec2(0, 0);
		m_Buffer << glm::vec3(1, 0, 0) << glm::vec2(1, 0);
		m_Buffer << glm::vec3(0, 1, 0) << glm::vec2(0, 1);
		m_Buffer << glm::vec3(1, 1, 0) << glm::vec2(1, 1);

		m_Buffer.describe("a_Position", 3, GL_FLOAT, 5 * sizeof(GLfloat), 0);
		m_Buffer.describe("a_Texcoord", 2, GL_FLOAT, 5 * sizeof(GLfloat), 3 * sizeof(GLfloat));

		m_Buffer.generate(Buffer::STATIC);
	}

	virtual ~Canvas()
	{
		ResourceManager::getInstance().unload(m_Shader);

		unbind();
		destroy();
	}

	void resize(unsigned int width, unsigned int height)
	{
		destroy();
		create(width, height);

		m_Camera2D.resize(width, height);
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
		unbind();
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
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture->id(), 0);
	}

	void unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void draw(Context* context)
	{
		glViewport(0, 0, m_Width, m_Height);
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, m_Texture->id());

		m_Shader->use();
		m_Shader->uniform("u_ModelViewProjection").set(m_Camera2D.getViewProjectionMatrix() * glm::scale(glm::mat4(), glm::vec3(m_Width, m_Height, 1)));
		m_Shader->uniform("u_Texture").set(*m_Texture);
		
		context->geometry().bind(m_Buffer, *m_Shader);
		context->geometry().drawArrays(GL_TRIANGLE_STRIP, 0, m_Buffer.size() / (sizeof(glm::vec3) + sizeof(glm::vec2)));
		context->geometry().unbind(m_Buffer);
	}

	void dump(const char* filename)
	{
		m_Texture->dump(filename);
	}

	unsigned int width() const { return m_Width; }
	unsigned int height() const { return m_Height; }

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
};
