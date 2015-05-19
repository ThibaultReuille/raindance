#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Buffer.hh>

class Rectangle
{
public:

	Rectangle(const glm::vec3& position = glm::vec3(0, 0, 0), const glm::vec2& dimension = glm::vec2(1.0, 1.0))
	{
		m_Shader = ResourceManager::getInstance().loadShader("shaders/primitives/rectangle",
			Assets_Shaders_Primitives_rectangle_vert, sizeof(Assets_Shaders_Primitives_rectangle_vert),
			Assets_Shaders_Primitives_rectangle_frag, sizeof(Assets_Shaders_Primitives_rectangle_frag),
			Assets_Shaders_Primitives_rectangle_geom, sizeof(Assets_Shaders_Primitives_rectangle_geom));
		m_Shader->dump();

		m_VertexBuffer << glm::vec3(0.0, 0.0, 0.0);
		m_VertexBuffer.describe("a_Zero", 3, GL_FLOAT, sizeof(glm::vec3), 0);
		m_VertexBuffer.generate(Buffer::STATIC);

		setPosition(position);
		setDimension(dimension);
		setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
	}

	void draw(Context* context, Camera& camera, Transformation& transformation)
	{
        m_Shader->use();

        m_Shader->uniform("u_ModelViewMatrix").set(camera.getViewMatrix() * transformation.state());
        m_Shader->uniform("u_ProjectionMatrix").set(camera.getProjectionMatrix());
        m_Shader->uniform("u_Texture").set(*m_Texture);

        context->geometry().bind(m_VertexBuffer, *m_Shader);
        context->geometry().drawArrays(GL_POINTS, 0, m_VertexBuffer.size() / sizeof(glm::vec3));        
        context->geometry().unbind(m_VertexBuffer);
	}

	void setPosition(const glm::vec3& position)
	{
		m_Position = position;
		m_Shader->use();
		m_Shader->uniform("u_Position").set(m_Position);
	}

	void setDimension(const glm::vec2& dimension)
	{
		m_Dimension = dimension;
		m_Shader->use();
		m_Shader->uniform("u_Dimension").set(m_Dimension);
	}

	void setColor(const glm::vec4& color)
	{
		m_Color = color;
		m_Shader->use();
		m_Shader->uniform("u_Color").set(m_Color);
	}

	void setTexture(Texture* texture)
	{
		m_Texture = texture;
		m_Shader->use();
		m_Shader->uniform("u_Texture").set(*m_Texture);
	}

private:
	glm::vec3 m_Position;
	glm::vec2 m_Dimension;
	glm::vec4 m_Color;
	Texture* m_Texture;

	Buffer m_VertexBuffer;
	Shader::Program* m_Shader;
};

