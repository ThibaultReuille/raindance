#pragma once

#include "Core/Scene.hh"

class Line
{
public:
	struct Vertex
	{
		Vertex()
			: Position(glm::vec3(0.0, 0.0, 0.0)),
			  Color(glm::vec4(1.0, 1.0, 1.0, 1.0))
		{
		}

		Vertex(const glm::vec3& position, const glm::vec4& color)
			: Position(position),
			  Color(color)
		{
		}
		Vertex(const glm::vec3& position)
			: Position(position),
			  Color(glm::vec4(1.0, 1.0, 1.0, 1.0))
		{
		}

		glm::vec3 Position;
		glm::vec4 Color;
	};

	Line()
	{
		m_FirstUpdate = true;

		Vertex v;
		m_VertexBuffer.push(&v, sizeof(Vertex));
		m_VertexBuffer.push(&v, sizeof(Vertex));

		update();
	}

	void set(const Vertex& v1, const Vertex& v2)
	{
		m_VertexBuffer.clear();
		m_VertexBuffer.push(&v1, sizeof(Vertex));
		m_VertexBuffer.push(&v2, sizeof(Vertex));

		update();
	}

	void get(Vertex* v1, Vertex* v2)
	{
	    if (v1 != NULL)
	        m_VertexBuffer.get(0, v1, sizeof(Vertex));
	    if (v2 != NULL)
	        m_VertexBuffer.get(1, v2, sizeof(Vertex));
	}

	void setPositions(const glm::vec3& pos1, const glm::vec3& pos2)
	{
		Vertex v1;
		Vertex v2;

		m_VertexBuffer.get(0, &v1, sizeof(Vertex));
		m_VertexBuffer.get(1, &v2, sizeof(Vertex));
		v1.Position = pos1;
		v2.Position = pos2;

		m_VertexBuffer.clear();
		m_VertexBuffer.push(&v1, sizeof(Vertex));
		m_VertexBuffer.push(&v2, sizeof(Vertex));

		update();
	}

	void setColor(unsigned int vertex, const glm::vec4& color)
	{
		Vertex v;
		m_VertexBuffer.get(vertex, &v, sizeof(Vertex));
		v.Color = color;
		m_VertexBuffer.set(vertex, &v, sizeof(Vertex));

		update();
	}

	glm::vec4 getColor(unsigned int vertex)
	{
	    Vertex v;
	    m_VertexBuffer.get(vertex, &v, sizeof(Vertex));
	    return v.Color;
	}

	void update()
	{
		if (m_FirstUpdate)
		{
			m_VertexBuffer.describe("a_Position", 3, GL_FLOAT, sizeof(Vertex), 0);
			m_VertexBuffer.describe("a_Color",    4, GL_FLOAT, sizeof(Vertex), sizeof(glm::vec3));
			m_VertexBuffer.generate(Buffer::DYNAMIC);
			m_FirstUpdate = false;
		}
		else
		{
			m_VertexBuffer.update();
			m_VertexBuffer.describe("a_Position", 3, GL_FLOAT, sizeof(Vertex), 0);
			m_VertexBuffer.describe("a_Color", 4, GL_FLOAT, sizeof(Vertex), sizeof(glm::vec3));
		}
	}

	inline Buffer& getVertexBuffer() { return m_VertexBuffer; }

private:
	Buffer m_VertexBuffer;
	bool m_FirstUpdate;
};

