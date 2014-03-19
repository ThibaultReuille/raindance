#pragma once

#include "Core/Buffer.hh"

class Mesh
{
public:
	struct Vertex
	{
		typedef unsigned int ID;

		Vertex(const glm::vec3& position)
			: Position(position), Normal(glm::vec3(0, 0, 0)), Texcoord(glm::vec2(0, 0))
		{
		}

		Vertex(const glm::vec3& position, const glm::vec3& normal)
			: Position(position), Normal(normal), Texcoord(glm::vec2(0, 0))
		{
		}

		Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& texcoord)
			: Position(position), Normal(normal), Texcoord(texcoord)
		{
		}

		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 Texcoord;
	};

	struct Line
	{
		typedef unsigned int ID;

		Line(Vertex::ID a, Vertex::ID b)
			: A(a), B(b)
		{
		}

		Vertex::ID A;
		Vertex::ID B;
	};

	struct Triangle
	{
		typedef unsigned int ID;

		Triangle(Vertex::ID a, Vertex::ID b, Vertex::ID c)
			: A(a), B(b), C(c)
		{
		}

		Vertex::ID A;
		Vertex::ID B;
		Vertex::ID C;
	};

	Mesh(const char* name)
	{
		LOG("[MESH] Creating mesh '%s' ...\n", name);
		m_Name = std::string(name);
		m_FirstUpdate = true;
	}

	~Mesh()
	{
		LOG("[MESH] Destroying mesh '%s' ...\n", m_Name.c_str());
	}

	Vertex::ID addVertex(const Vertex& vertex)
	{
		m_VertexBuffer.push(&vertex, sizeof(Vertex));
		return (m_VertexBuffer.size() / sizeof(Vertex)) - 1;
	}

	Line::ID addLine(const Line& line)
	{
		m_LineBuffer.push(&line, sizeof(Line));
		return (m_LineBuffer.size() / sizeof(Line)) - 1;
	}

	Triangle::ID addTriangle(const Triangle& triangle)
	{
		m_TriangleBuffer.push(&triangle, sizeof(Triangle));
		return (m_TriangleBuffer.size() / sizeof(Triangle)) - 1;
	}

	void update()
	{
		if (!m_FirstUpdate)
			m_VertexBuffer.update();

		m_VertexBuffer.describe("a_Position", 3, GL_FLOAT, sizeof(Vertex), 0);
		m_VertexBuffer.describe("a_Normal",   3, GL_FLOAT, sizeof(Vertex), 3 * sizeof(GLfloat));
		// m_VertexBuffer.describe("a_Texcoord", 2, GL_FLOAT, sizeof(Vertex), 6 * sizeof(GLfloat));

		if (m_FirstUpdate)
			m_VertexBuffer.generate(Buffer::DYNAMIC);

		m_FirstUpdate = false;
	}

	inline Buffer& getVertexBuffer() { return m_VertexBuffer; }
	inline Buffer& getLineBuffer() { return m_LineBuffer; }
	inline Buffer& getTriangleBuffer() { return m_TriangleBuffer; }

private:
	std::string m_Name;

	Buffer m_VertexBuffer;
	Buffer m_LineBuffer;
	Buffer m_TriangleBuffer;

	bool m_FirstUpdate;
};

