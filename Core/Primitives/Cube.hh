#pragma once

#include <raindance/Core/Headers.hh>

class Cube
{
public:
	struct Vertex
	{
		Vertex(const glm::vec3& position, const glm::vec3& normal)
		: Position(position), Normal(normal)
		{
		}

		glm::vec3 Position;
		glm::vec3 Normal;
	};

    enum DrawMode
    {
        TRIANGLES,
        LINES
    };

	Cube(glm::vec3 step = glm::vec3(0, 0, 0))
	{
		Vertex vertices[8] =
		{
			Vertex(glm::vec3(-0.5,  0.5, -0.5) + step, glm::normalize(glm::vec3(-1,  1,  1))),
			Vertex(glm::vec3( 0.5,  0.5, -0.5) + step, glm::normalize(glm::vec3( 1,  1, -1))),
			Vertex(glm::vec3( 0.5,  0.5,  0.5) + step, glm::normalize(glm::vec3( 1,  1,  1))),
			Vertex(glm::vec3(-0.5,  0.5,  0.5) + step, glm::normalize(glm::vec3(-1,  1,  1))),

			Vertex(glm::vec3(-0.5, -0.5, -0.5) + step, glm::normalize(glm::vec3(-1, -1, -1))),
			Vertex(glm::vec3( 0.5, -0.5, -0.5) + step, glm::normalize(glm::vec3( 1, -1, -1))),
			Vertex(glm::vec3( 0.5, -0.5,  0.5) + step, glm::normalize(glm::vec3( 1,  1, -1))),
			Vertex(glm::vec3(-0.5, -0.5,  0.5) + step, glm::normalize(glm::vec3(-1,  1, -1)))
		};

		unsigned short int triangles_indices[] =
		{
			0, 1, 2, 0, 2, 3,
			1, 5, 6, 1, 6, 2,
			5, 4, 7, 5, 7, 6,
			4, 0, 3, 4, 3, 7,
			4, 5, 1, 4, 1, 0,
			6, 7, 3, 6, 3, 2
		};
		for (auto i : triangles_indices)
			m_TriangleVertexBuffer.push(&vertices[i], sizeof(Vertex));

		
		unsigned short int lines_indices[] =
		{
			0, 1, 1, 2, 2, 3, 3, 0,
			4, 5, 5, 6, 6, 7, 7, 4,
			0, 4, 1, 5, 2, 6, 3, 7
		};
		for (auto i : lines_indices)
			m_LineVertexBuffer.push(&vertices[i], sizeof(Vertex));
		
		m_FirstUpdate = true;
		update();
	}

	void update()
	{
		if (!m_FirstUpdate)
			return;

		m_TriangleVertexBuffer.describe("a_Position", 3, GL_FLOAT, sizeof(Vertex), 0);
		m_TriangleVertexBuffer.describe("a_Normal",   3, GL_FLOAT, sizeof(Vertex), 3 * sizeof(GLfloat));
		m_TriangleVertexBuffer.generate(Buffer::STATIC);

		m_LineVertexBuffer.describe("a_Position", 3, GL_FLOAT, sizeof(Vertex), 0);
		m_LineVertexBuffer.describe("a_Normal",   3, GL_FLOAT, sizeof(Vertex), 3 * sizeof(GLfloat));
		m_LineVertexBuffer.generate(Buffer::STATIC);

		m_FirstUpdate = false;
	}
    
	// TODO : Visitor
    void draw(Context* context, Camera& camera, Transformation& transformation, Shader::Program* shader, DrawMode drawmode)
    {
        shader->use();
        shader->uniform("u_ModelViewProjectionMatrix").set(camera.getViewProjectionMatrix() * transformation.state());

        switch(drawmode)
        {
            case TRIANGLES:
                context->geometry().bind(m_TriangleVertexBuffer, *shader);        
                context->geometry().drawArrays(GL_TRIANGLES, 0, m_TriangleVertexBuffer.size() / sizeof(Cube::Vertex));
                context->geometry().unbind(m_TriangleVertexBuffer);
                break;
            case LINES:
                context->geometry().bind(m_LineVertexBuffer, *shader);        
                context->geometry().drawArrays(GL_LINES, 0, m_LineVertexBuffer.size() / sizeof(Cube::Vertex));
                context->geometry().unbind(m_LineVertexBuffer);
            default:
                break;
        }
    }

    inline Buffer& getTriangleVertexBuffer() { return m_TriangleVertexBuffer; }
    inline Buffer& getLineVertexBuffer() { return m_LineVertexBuffer; }

private:
    Buffer m_TriangleVertexBuffer;
    Buffer m_LineVertexBuffer;
    bool m_FirstUpdate;
};

