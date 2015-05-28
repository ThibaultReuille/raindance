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

		std::vector<Vertex> vertices;

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

				//m_VertexBuffer.push(&vertex, sizeof(Vertex));
				vertices.push_back(vertex);
			}

		for(unsigned int r = 0; r < rings - 1; r++)
			for(unsigned int s = 0; s < sectors - 1; s++)
			{
				unsigned int a = r * sectors + s;
				unsigned int b = r * sectors + (s + 1);
				unsigned int c = (r + 1) * sectors + (s + 1);
				unsigned int d = (r + 1) * sectors + s;
				//m_IndexBuffer << a << b << c;
				//m_IndexBuffer << a << c << d;

				m_VertexBuffer.push(&vertices[a], sizeof(Vertex));
				m_VertexBuffer.push(&vertices[b], sizeof(Vertex));
				m_VertexBuffer.push(&vertices[c], sizeof(Vertex));

				m_VertexBuffer.push(&vertices[a], sizeof(Vertex));
				m_VertexBuffer.push(&vertices[c], sizeof(Vertex));
				m_VertexBuffer.push(&vertices[d], sizeof(Vertex));
			}

		m_VertexBuffer.describe("a_Position", 3, GL_FLOAT, sizeof(Vertex), 0);
		m_VertexBuffer.describe("a_Normal",   3, GL_FLOAT, sizeof(Vertex), sizeof(glm::vec3));
		m_VertexBuffer.describe("a_Texcoord", 2, GL_FLOAT, sizeof(Vertex), sizeof(glm::vec3) * 2);
		m_VertexBuffer.generate(Buffer::STATIC);
	}
	/*

	Vertex computeVertex(float radius, unsigned int ring, unsigned int sector, unsigned int total_rings, unsigned int total_sectors)
	{
		float R = 1.0f / (float)(rings - 1);
		float S = 1.0f / (float)(sectors - 1);

		Vertex vertex;

		vertex.Normal = glm::vec3
		(
			cos(2 * M_PI * sector * S) * sin(M_PI * ring * R),
			sin(-M_PI_2 + M_PI * ring * R),
			sin(2 * M_PI * sector * S) * sin(M_PI * ring * R)
		);
		vertex.Position = radius * vertex.Normal;
		vertex.Texcoord = glm::vec2(s * S, ring * R);

		return vertex;
	}
	*/


	inline Buffer& getVertexBuffer() { return m_VertexBuffer; }
	//inline Buffer& getIndexBuffer() { return m_IndexBuffer; }

private:
	Buffer m_VertexBuffer;
	//Buffer m_IndexBuffer;
};

