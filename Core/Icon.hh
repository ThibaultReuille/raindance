#pragma once

#include "Core/Headers.hh"

class Icon
{
public:
	Icon(const glm::vec2& dimension = glm::vec2(1.0, 1.0), const glm::vec2& step = glm::vec2(0.0, 0.0))
	{
		m_Shader = ResourceManager::getInstance().loadShader("icon", Resources_Shaders_icon_vert, sizeof(Resources_Shaders_icon_vert),
		                                                             Resources_Shaders_icon_frag, sizeof(Resources_Shaders_icon_frag));
		// m_Shader->dump();
		m_MvpUniform = &m_Shader->uniform("u_ModelViewProjection");
		m_ColorUniform = &m_Shader->uniform("u_Color");
		m_TextureUniform = &m_Shader->uniform("u_Texture");

		m_Buffer << glm::vec3(dimension.x * (-0.5 + step.x), dimension.y * (-0.5 + step.y), 0) << glm::vec2(0, 1);
		m_Buffer << glm::vec3(dimension.x * ( 0.5 + step.x), dimension.y * (-0.5 + step.y), 0) << glm::vec2(1, 1);
		m_Buffer << glm::vec3(dimension.x * ( 0.5 + step.x), dimension.y * ( 0.5 + step.y), 0) << glm::vec2(1, 0);
		m_Buffer << glm::vec3(dimension.x * (-0.5 + step.x), dimension.y * ( 0.5 + step.y), 0) << glm::vec2(0, 0);

		m_Buffer.describe("a_Position", 3, GL_FLOAT, 5 * sizeof(GLfloat), 0);
		m_Buffer.describe("a_Texcoord", 2, GL_FLOAT, 5 * sizeof(GLfloat), 3 * sizeof(GLfloat));

		m_Buffer.generate(Buffer::STATIC);
	}

	~Icon()
	{
		ResourceManager::getInstance().unload(m_Shader);

		std::vector<Texture*>::iterator it;
		for (it = m_Textures.begin(); it != m_Textures.end(); ++it)
			ResourceManager::getInstance().unload(*it);
	}

	unsigned long load(const char* name, unsigned char* image, unsigned int size)
	{
		m_Textures.push_back(ResourceManager::getInstance().loadTexture(name, image, size));
		return m_Textures.size() - 1;
	}

	void draw(Context* context, glm::mat4 mvp, glm::vec4 color, unsigned int mode)
	{
		unsigned short int indices[] = { 0, 1, 2, 0, 2, 3 };

		m_Shader->use();
		m_MvpUniform->set(mvp);
		m_ColorUniform->set(color);
		m_TextureUniform->set(*m_Textures[mode]);

		context->geometry().bind(m_Buffer, *m_Shader);
		context->geometry().drawElements(GL_TRIANGLES, sizeof(indices) / sizeof(short int), GL_UNSIGNED_SHORT, indices);
		context->geometry().unbind(m_Buffer);
	}

	inline unsigned long countTextures() const { return m_Textures.size(); }

private:
	Buffer m_Buffer;
	std::vector<Texture*> m_Textures;

	Shader::Program* m_Shader;
	const Shader::Uniform* m_MvpUniform;
	const Shader::Uniform* m_ColorUniform;
	const Shader::Uniform* m_TextureUniform;
};

