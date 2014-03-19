#pragma once

#if 0
class Monkey
{
public:
	Monkey()
	{
		m_Shader = ResourceManager::getInstance().loadShader
		(
			"monkey",
			BlenderMonkey::Resources_monkey_vert, sizeof(BlenderMonkey::Resources_monkey_vert),
			BlenderMonkey::Resources_monkey_frag, sizeof(BlenderMonkey::Resources_monkey_frag)
		);
		m_Shader->dump();

		m_Buffer.push(BlenderMonkey::vertices, sizeof(BlenderMonkey::vertices));
		m_Buffer.describe("a_Position", 3, GL_FLOAT, sizeof(struct BlenderMonkey::vertex_struct), 0);
		m_Buffer.describe("a_Normal",   3, GL_FLOAT, sizeof(struct BlenderMonkey::vertex_struct), 3 * sizeof(GLfloat));
		// m_Buffer.describe("a_Texcoord", 2, GL_FLOAT, sizeof(struct Monkey::vertex_struct), 6 * sizeof(GLfloat));
		m_Buffer.generate();
	}

	void draw(Context* context, glm::mat4 projection, glm::mat4 view, glm::mat4 model)
	{
		m_Shader->use();
		m_Shader->uniform("u_Projection").set(projection);
		m_Shader->uniform("u_View").set(view);
		m_Shader->uniform("u_Model").set(model);
		m_Shader->uniform("u_Light.position").set(10, 10, 0);

		context->geometry().bind(m_Buffer, *m_Shader);
		context->geometry().drawElements(GL_TRIANGLES, sizeof(BlenderMonkey::indexes) / sizeof(short int), GL_UNSIGNED_SHORT, BlenderMonkey::indexes);
		context->geometry().unbind(m_Buffer, *m_Shader);
	}

private:
	Buffer m_Buffer;
	Shader::Program* m_Shader;
};
#endif

