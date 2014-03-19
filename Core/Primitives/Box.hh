#pragma once

#include "Core/Headers.hh"

class Box
{
public:
	Box()
	{
		m_Shader = ResourceManager::getInstance().loadShader("box", Resources_Shaders_box_vert, sizeof(Resources_Shaders_box_vert),
		                                                            Resources_Shaders_box_frag, sizeof(Resources_Shaders_box_frag));
		// m_Shader->dump();

		m_VertexBuffer.disable(Buffer::GPU_SYNC);

		update();
	}

	~Box()
	{
		ResourceManager::getInstance().unload(m_Shader);
	}

	void draw(Context* context, glm::mat4 mvp)
	{
		unsigned short int lines_indices[] =
		{
			0, 1, 1, 2, 2, 3, 3, 0,
			4, 5, 5, 6, 6, 7, 7, 4,
			0, 4, 1, 5, 2, 6, 3, 7
		};

		m_Shader->use();
		m_Shader->uniform("u_ModelViewProjection").set(mvp);
		context->geometry().bind(m_VertexBuffer, *m_Shader);
		context->geometry().drawElements(GL_LINES, sizeof(lines_indices) / sizeof(short int), GL_UNSIGNED_SHORT, lines_indices);
		context->geometry().unbind(m_VertexBuffer);
	}

	void reset()
	{
		m_Max = glm::vec3(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
		m_Min = glm::vec3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	}

	void adjust(const glm::vec3& point)
	{
		if (point.x < m_Min.x) m_Min.x = point.x;
		if (point.y < m_Min.y) m_Min.y = point.y;
		if (point.z < m_Min.z) m_Min.z = point.z;

		if (point.x > m_Max.x) m_Max.x = point.x;
		if (point.y > m_Max.y) m_Max.y = point.y;
		if (point.z > m_Max.z) m_Max.z = point.z;
	}

	void update()
	{
		m_VertexBuffer.clear();

		m_VertexBuffer << glm::vec3(m_Min.x, m_Min.y, m_Max.z);
		m_VertexBuffer << glm::vec3(m_Max.x, m_Min.y, m_Max.z);
		m_VertexBuffer << glm::vec3(m_Max.x, m_Max.y, m_Max.z);
		m_VertexBuffer << glm::vec3(m_Min.x, m_Max.y, m_Max.z);

		m_VertexBuffer << glm::vec3(m_Min.x, m_Min.y, m_Min.z);
		m_VertexBuffer << glm::vec3(m_Max.x, m_Min.y, m_Min.z);
		m_VertexBuffer << glm::vec3(m_Max.x, m_Max.y, m_Min.z);
		m_VertexBuffer << glm::vec3(m_Min.x, m_Max.y, m_Min.z);

		static unsigned int updateCount = 0;
		if (updateCount == 0)
		{
			m_VertexBuffer.enable(Buffer::GPU_SYNC);
			m_VertexBuffer.describe("a_Position", 3, GL_FLOAT, sizeof(glm::vec3), 0);
			m_VertexBuffer.generate(Buffer::DYNAMIC);
			m_VertexBuffer.disable(Buffer::GPU_SYNC);
		}
		else
		{
			m_VertexBuffer.enable(Buffer::GPU_SYNC);
			m_VertexBuffer.update();
			m_VertexBuffer.describe("a_Position", 3, GL_FLOAT, sizeof(glm::vec3), 0);
			m_VertexBuffer.disable(Buffer::GPU_SYNC);
		}
		updateCount++;
	}

	inline const glm::vec3& min() const { return m_Min; }
	inline const glm::vec3& max() const { return m_Max; }
	inline glm::vec3 center() const { return (m_Max + m_Min) / 2.0f; }

private:
	glm::vec3 m_Min;
	glm::vec3 m_Max;

	Buffer m_VertexBuffer;
	Shader::Program* m_Shader;
};
