#pragma once

#include <raindance/Core/Headers.hh>

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
        m_Buffer << glm::vec3(dimension.x * (-0.5 + step.x), dimension.y * ( 0.5 + step.y), 0) << glm::vec2(0, 0);
		m_Buffer << glm::vec3(dimension.x * ( 0.5 + step.x), dimension.y * ( 0.5 + step.y), 0) << glm::vec2(1, 0);

		m_Buffer.describe("a_Position", 3, GL_FLOAT, 5 * sizeof(GLfloat), 0);
		m_Buffer.describe("a_Texcoord", 2, GL_FLOAT, 5 * sizeof(GLfloat), 3 * sizeof(GLfloat));

		m_Buffer.generate(Buffer::STATIC);
	}

	virtual ~Icon()
	{
		ResourceManager::getInstance().unload(m_Shader);

		std::vector<Texture*>::iterator it;
		for (it = m_Textures.begin(); it != m_Textures.end(); ++it)
			ResourceManager::getInstance().unload(*it);
	}

	unsigned long load(const char* name, unsigned char* image, unsigned int size)
	{
	    std::string sname(name);
	    std::unordered_map<std::string, unsigned long>::const_iterator search = m_TextureMap.find(sname);

	    Texture* texture = ResourceManager::getInstance().loadTexture(name, image, size);

	    if (search != m_TextureMap.end())
	    {
	        LOG("[ICON] Texture '%s' already defined! Overwriting.\n", search->first.c_str());
	        ResourceManager::getInstance().unload(m_Textures[search->second]);
	        m_Textures[search->second] = texture;
	        return search->second;
	    }
	    else
	    {
	        m_Textures.push_back(texture);
	        m_TextureMap[sname] = m_Textures.size() - 1;
	        return m_Textures.size() - 1;
	    }
	}

	void draw(Context* context, glm::mat4 mvp, glm::vec4 color, unsigned int mode)
	{
		m_Shader->use();
		m_MvpUniform->set(mvp);
		m_ColorUniform->set(color);
		m_TextureUniform->set(*m_Textures[mode]);

		context->geometry().bind(m_Buffer, *m_Shader);
		context->geometry().drawArrays(GL_TRIANGLE_STRIP, 0, m_Buffer.size() / (3 * sizeof(GLfloat) + 2 * sizeof(GLfloat)));
		context->geometry().unbind(m_Buffer);
	}

	inline unsigned long countTextures() const { return m_Textures.size(); }

	bool getTexture(const char* name, unsigned long* id)
	{
	    std::string sname(name);
	    std::unordered_map<std::string, unsigned long>::const_iterator search = m_TextureMap.find(sname);

	    if (search == m_TextureMap.end())
	        return false;

	    *id = search->second;
	    return true;
	}

	inline const Texture* getTexture(unsigned int id) const { return m_Textures[id]; }

private:
	Buffer m_Buffer;
	std::vector<Texture*> m_Textures;
	std::unordered_map<std::string, unsigned long> m_TextureMap;

	Shader::Program* m_Shader;
	const Shader::Uniform* m_MvpUniform;
	const Shader::Uniform* m_ColorUniform;
	const Shader::Uniform* m_TextureUniform;
};

