#pragma once

#include <raindance/Core/Headers.hh>

class Grid
{
public:
    Grid(unsigned long width, unsigned long height)
	{
        m_Width = width;
        m_Height = height;
        m_Color = glm::vec4(1.0, 1.0, 1.0, 1.0);

        float x;
        float y;
        GLushort index = 0;

        y = static_cast<float>(height);
        for (unsigned int i = 0; i <= width; i++)
        {
            x = static_cast<float>(i);
            m_VertexBuffer << glm::vec3(x, 0, 0) << glm::vec3(x, y, 0);
            m_LineBuffer << index++;
            m_LineBuffer << index++;
        }

        x = static_cast<float>(width);
        for (unsigned int j = 0; j <= height; j++)
        {
            y = static_cast<float>(j);
            m_VertexBuffer << glm::vec3(0, y, 0) << glm::vec3(x, y, 0);
            m_LineBuffer << index++;
            m_LineBuffer << index++;
        }

        m_VertexBuffer.describe("a_Position", 3, GL_FLOAT, 3 * sizeof(GLfloat), 0);
        m_VertexBuffer.generate(Buffer::STATIC);

        m_Shader = ResourceManager::getInstance().loadShader("Primitives/grid",
                Resources_Shaders_Primitives_grid_vert, sizeof(Resources_Shaders_Primitives_grid_vert),
                Resources_Shaders_Primitives_grid_frag, sizeof(Resources_Shaders_Primitives_grid_frag));
        // m_Shader->dump();
	}

	virtual ~Grid()
	{
	    ResourceManager::getInstance().unload(m_Shader);
	}

	void draw(Context& context, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
	{
	    m_Shader->use();
        m_Shader->uniform("u_ModelViewProjection").set(projection * view * model);
        m_Shader->uniform("u_Color").set(m_Color);
        context.geometry().bind(m_VertexBuffer, *m_Shader);
        context.geometry().drawElements(GL_LINES, m_LineBuffer.size() / sizeof(unsigned short int), GL_UNSIGNED_SHORT, m_LineBuffer.ptr());
        context.geometry().unbind(m_VertexBuffer);
	}

	inline void setColor(const glm::vec4& color) { m_Color = color; }

	inline Buffer& getVertexBuffer() { return m_VertexBuffer; }
	inline Buffer& getLineBuffer() { return m_LineBuffer; }

	inline unsigned long width() { return m_Width; }
	inline unsigned long height() { return m_Height; }

private:
	unsigned long m_Width;
	unsigned long m_Height;

	glm::vec4 m_Color;

	Buffer m_VertexBuffer;
	Buffer m_LineBuffer;

	Shader::Program* m_Shader;
};
