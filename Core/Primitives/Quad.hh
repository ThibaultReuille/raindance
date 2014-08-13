#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Buffer.hh>

class Quad
{
public:
	struct Vertex
	{
		Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& texcoord)
			: Position(position), Normal(normal), Texcoord(texcoord)
		{
		}

		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 Texcoord;
	};

	Quad()
	{
		//                Position                       Normal             Texcoord
		m_VertexBuffer << glm::vec3(0.0, 0.0, 0.0) << glm::vec3(0, 0, 1) << glm::vec2(0, 1);
		m_VertexBuffer << glm::vec3(1.0, 0.0, 0.0) << glm::vec3(0, 0, 1) << glm::vec2(1, 1);
		m_VertexBuffer << glm::vec3(1.0, 1.0, 0.0) << glm::vec3(0, 0, 1) << glm::vec2(1, 0);
		m_VertexBuffer << glm::vec3(0.0, 1.0, 0.0) << glm::vec3(0, 0, 1) << glm::vec2(0, 0);

		unsigned char lines_indices[] = { 0, 1, 1, 2, 2, 3, 3, 0 };
		m_LineBuffer.push(lines_indices, sizeof(lines_indices));

		unsigned char triangles_indices[] = { 0, 1, 2, 0, 2, 3 };
		m_TriangleBuffer.push(triangles_indices, sizeof(triangles_indices));

		m_FirstUpdate = true;
		update();
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
	inline Buffer& getLineBuffer() { return m_LineBuffer; }
	inline Buffer& getTriangleBuffer() { return m_TriangleBuffer; }

private:
	Buffer m_VertexBuffer;
	Buffer m_LineBuffer;
	Buffer m_TriangleBuffer;
	bool m_FirstUpdate;
};

