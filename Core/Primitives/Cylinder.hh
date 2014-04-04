#pragma once

class Cylinder
{
public:
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
	};

	inline glm::vec3 spherical(float r, float theta, float phi)
	{
	    return glm::vec3(r * sin(theta) * cos(phi), r * sin(theta) * sin(phi), r * cos(theta));
	}

	Cylinder(float radius, float halfLength, int slices, int stacks, glm::vec3 step = glm::vec3(0, 0, 0))
	{
	    float d = 1.0f / sqrt(2.0);
	    float length;

		for(int i = 0; i < slices; i++)
		{
		    float theta1 = ((float) i) * (2.0 * M_PI) / (float) slices;
		    float theta2 = ((float) i + 1) * (2.0 * M_PI) / (float) slices;

		    // Vertex at middle of end

		    m_VertexBuffer << glm::vec3(0.0, halfLength, 0.0);
		    m_VertexBuffer << glm::vec3(0, 1, 0);

		    // Vertices at edges of circle

		    m_VertexBuffer << glm::vec3(radius * cos(theta2), halfLength, radius * sin(theta2)) + step;
		    m_VertexBuffer << d * glm::vec3(cos(theta2), 1.0, sin(theta2));

		    m_VertexBuffer << glm::vec3(radius * cos(theta1), halfLength, radius * sin(theta1)) + step;
            m_VertexBuffer << d * glm::vec3(cos(theta1), 1.0, sin(theta1));

            // Vertices in the different stacks
            for (int j = 1; j < stacks - 1; j++)
            {
                length = halfLength - (float) j * (2.0 * halfLength) / (float) stacks;

                m_VertexBuffer << glm::vec3(radius * cos(theta2), length, radius * sin(theta2)) + step;
                m_VertexBuffer << glm::vec3(cos(theta2), 0, sin(theta2));

                m_VertexBuffer << glm::vec3(radius * cos(theta1), length, radius * sin(theta1)) + step;
                m_VertexBuffer << glm::vec3(cos(theta1), 0, sin(theta1));
            }

		    // The same vertices at the bottom of the cylinder

            m_VertexBuffer << glm::vec3(radius * cos(theta2), -halfLength, radius * sin(theta2)) + step;
            m_VertexBuffer << d * glm::vec3(cos(theta2), -1.0, sin(theta2));

		    m_VertexBuffer << glm::vec3(radius * cos(theta1), -halfLength, radius * sin(theta1)) + step;
		    m_VertexBuffer << d * glm::vec3(cos(theta1), -1.0, sin(theta1));

		    m_VertexBuffer << glm::vec3(0.0, -halfLength, 0.0) + step;
		    m_VertexBuffer << glm::vec3(0, -1, 0);
		}

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

private:
	Buffer m_VertexBuffer;
	bool m_FirstUpdate;
};

