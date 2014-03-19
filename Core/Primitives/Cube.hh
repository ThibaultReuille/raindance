#pragma once

class Cube
{
public:
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
	};

	Cube()
	{
		//                Position                       Normal
		m_VertexBuffer << glm::vec3(-0.5,  0.5, -0.5) << glm::normalize(glm::vec3(-1,  1,  1));
		m_VertexBuffer << glm::vec3( 0.5,  0.5, -0.5) << glm::normalize(glm::vec3( 1,  1, -1));
		m_VertexBuffer << glm::vec3( 0.5,  0.5,  0.5) << glm::normalize(glm::vec3( 1,  1,  1));
		m_VertexBuffer << glm::vec3(-0.5,  0.5,  0.5) << glm::normalize(glm::vec3(-1,  1,  1));

		m_VertexBuffer << glm::vec3(-0.5, -0.5, -0.5) << glm::normalize(glm::vec3(-1, -1, -1));
		m_VertexBuffer << glm::vec3( 0.5, -0.5, -0.5) << glm::normalize(glm::vec3( 1, -1, -1));
		m_VertexBuffer << glm::vec3( 0.5, -0.5,  0.5) << glm::normalize(glm::vec3( 1,  1, -1));
		m_VertexBuffer << glm::vec3(-0.5, -0.5,  0.5) << glm::normalize(glm::vec3(-1,  1, -1));

		unsigned short int triangles_indices[] =
		{
			0, 1, 2, 0, 2, 3,
			1, 5, 6, 1, 6, 2,
			5, 4, 7, 5, 7, 6,
			4, 0, 3, 4, 3, 7,
			4, 5, 1, 4, 1, 0,
			6, 7, 3, 6, 3, 2
		};
		m_IndexBuffer.push(triangles_indices, sizeof(triangles_indices));

		/*
		unsigned short int lines_indices[] =
		{
			0, 1, 1, 2, 2, 3, 3, 0,
			4, 5, 5, 6, 6, 7, 7, 4,
			0, 4, 1, 5, 2, 6, 3, 7
		};
		*/

		m_FirstUpdate = true;
		update();
	}

	void update()
	{
		if (!m_FirstUpdate)
			m_VertexBuffer.update();

		m_VertexBuffer.describe("a_Position", 3, GL_FLOAT, sizeof(Vertex), 0);
		m_VertexBuffer.describe("a_Normal",   3, GL_FLOAT, sizeof(Vertex), 3 * sizeof(GLfloat));

		if (m_FirstUpdate)
			m_VertexBuffer.generate(Buffer::STATIC);

		m_FirstUpdate = false;
	}

	inline Buffer& getVertexBuffer() { return m_VertexBuffer; }
	inline Buffer& getIndexBuffer() { return m_IndexBuffer; }

private:
	Buffer m_VertexBuffer;
	Buffer m_IndexBuffer;
	bool m_FirstUpdate;
};

