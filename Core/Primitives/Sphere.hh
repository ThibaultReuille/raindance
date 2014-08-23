#pragma once

#include <raindance/Core/Headers.hh>

class SphereMesh
{
public:
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 Texcoord;
	};

	SphereMesh(float radius, unsigned int rings, unsigned int sectors)
	{
		float R = 1.0f / (float)(rings - 1);
		float S = 1.0f / (float)(sectors - 1);

		for(unsigned int r = 0; r < rings; r++)
			for(unsigned int s = 0; s < sectors; s++)
			{
				Vertex vertex;

				vertex.Normal = glm::vec3
				(
					cos(2 * M_PI * s * S) * sin(M_PI * r * R),
					sin(-M_PI_2 + M_PI * r * R),
					sin(2 * M_PI * s * S) * sin(M_PI * r * R)
				);
				vertex.Position = radius * vertex.Normal;
				vertex.Texcoord = glm::vec2(s * S, r * R);

				m_VertexBuffer.push(&vertex, sizeof(Vertex));
			}

		for(unsigned int r = 0; r < rings - 1; r++)
			for(unsigned int s = 0; s < sectors - 1; s++)
			{
				unsigned short int a = r * sectors + s;
				unsigned short int b = r * sectors + (s + 1);
				unsigned short int c = (r + 1) * sectors + (s + 1);
				unsigned short int d = (r + 1) * sectors + s;
				m_IndexBuffer << a << b << c;
				m_IndexBuffer << a << c << d;
			}

		m_FirstUpdate = true;
		update();
	}

	~SphereMesh()
	{
	}

	void update()
	{
		if (!m_FirstUpdate)
			m_VertexBuffer.update();

		m_VertexBuffer.describe("a_Position", 3, GL_FLOAT, sizeof(Vertex), 0);
		m_VertexBuffer.describe("a_Normal",   3, GL_FLOAT, sizeof(Vertex), 3 * sizeof(GLfloat));
		m_VertexBuffer.describe("a_Texcoord", 2, GL_FLOAT, sizeof(Vertex), 6 * sizeof(GLfloat));

		if (m_FirstUpdate)
			m_VertexBuffer.generate(Buffer::STATIC);

		m_FirstUpdate = false;
	}

	inline Buffer& getVertexBuffer() { return m_VertexBuffer; }
	inline Buffer& getIndexBuffer() { return m_IndexBuffer; }

private:
	bool m_FirstUpdate;
	Buffer m_VertexBuffer;
	Buffer m_IndexBuffer;
};

