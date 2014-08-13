#pragma once

#include <raindance/Core/Headers.hh>

class Axis
{
public:
	Axis()
	{
		m_Shader = ResourceManager::getInstance().loadShader("axis", Resources_Shaders_Widgets_axis_vert, sizeof(Resources_Shaders_Widgets_axis_vert),
		                                                             Resources_Shaders_Widgets_axis_frag, sizeof(Resources_Shaders_Widgets_axis_frag));
		m_Shader->dump();

		//          Position              Color
		m_Buffer << glm::vec3(0, 0, 0) << glm::vec4(1, 1, 1, 1);
		m_Buffer << glm::vec3(1, 0, 0) << glm::vec4(1, 0, 0, 1);
		m_Buffer << glm::vec3(0, 1, 0) << glm::vec4(0, 1, 0, 1);
		m_Buffer << glm::vec3(0, 0, 1) << glm::vec4(0, 0, 1, 1);

		m_Buffer.describe("a_Position", 3, GL_FLOAT, 7 * sizeof(GLfloat), 0);
		m_Buffer.describe("a_Color",    4, GL_FLOAT, 7 * sizeof(GLfloat), 3 * sizeof(GLfloat));

		m_Buffer.generate(Buffer::STATIC);
	}

	~Axis()
	{
		ResourceManager::getInstance().unload(m_Shader);
	}

	void draw(Context* context, glm::mat4 mvp)
	{
		static unsigned short int indices[] = {0, 1, 0, 2, 0, 3};

		m_Shader->use();
		m_Shader->uniform("u_ModelViewProjection").set(mvp);

		context->geometry().bind(m_Buffer, *m_Shader);
		context->geometry().drawElements(GL_LINES, sizeof(indices) / sizeof(short int), GL_UNSIGNED_SHORT, indices);
		context->geometry().unbind(m_Buffer);
	}

	Buffer m_Buffer;
	Shader::Program* m_Shader;
};
