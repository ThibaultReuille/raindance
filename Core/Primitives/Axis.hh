#pragma once

#include <raindance/Core/Headers.hh>

class Axis
{
public:
	struct Vertex
	{
		Vertex(const glm::vec3& position, const glm::vec4& color)
		: Position(position), Color(color)
		{}

		glm::vec3 Position;
		glm::vec4 Color;
	};

	Axis()
	{
		m_Shader = ResourceManager::getInstance().loadShader("Primitives/axis", Assets_Shaders_Primitives_axis_vert, sizeof(Assets_Shaders_Primitives_axis_vert),
		                                                             		 	Assets_Shaders_Primitives_axis_frag, sizeof(Assets_Shaders_Primitives_axis_frag));
		m_Shader->dump();

		Vertex o(glm::vec3(0.0, 0.0, 0.0), glm::vec4(1.0, 1.0, 1.0, 1.0));
		Vertex x(glm::vec3(1.0, 0.0, 0.0), glm::vec4(1.0, 0.0, 0.0, 1.0));
		Vertex y(glm::vec3(0.0, 1.0, 0.0), glm::vec4(0.0, 1.0, 0.0, 1.0));
		Vertex z(glm::vec3(0.0, 0.0, 1.0), glm::vec4(0.0, 0.0, 1.0, 1.0));
		
		m_Buffer.push(&o, sizeof(Vertex));
		m_Buffer.push(&x, sizeof(Vertex));

		m_Buffer.push(&o, sizeof(Vertex));
		m_Buffer.push(&y, sizeof(Vertex));

		m_Buffer.push(&o, sizeof(Vertex));
		m_Buffer.push(&z, sizeof(Vertex));
		
		m_Buffer.describe("a_Position", 3, GL_FLOAT, sizeof(Vertex), 0);
		m_Buffer.describe("a_Color",    4, GL_FLOAT, sizeof(Vertex), sizeof(glm::vec3));
		m_Buffer.generate(Buffer::STATIC);
	}

	virtual ~Axis()
	{
		ResourceManager::getInstance().unload(m_Shader);
	}

	void draw(Context* context, const Camera& camera, Transformation transformation)
	{
		m_Shader->use();
		m_Shader->uniform("u_ModelViewProjectionMatrix").set(camera.getViewProjectionMatrix() * transformation.state());

		context->geometry().bind(m_Buffer, *m_Shader);
		context->geometry().drawArrays(GL_LINES, 0, m_Buffer.size() / sizeof(Vertex));
		context->geometry().unbind(m_Buffer);
	}

	Buffer m_Buffer;
	Shader::Program* m_Shader;
};
