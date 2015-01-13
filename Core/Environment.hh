#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Primitives/Sphere.hh>

class EnvironmentSphere
{
public:
	EnvironmentSphere()
	{
		m_Texture = NULL;
		m_Sphere = new SphereMesh(1000.0f, 20, 20);
		m_Shader = ResourceManager::getInstance().loadShader("environment", Assets_Shaders_environment_vert, sizeof(Assets_Shaders_environment_vert),
		                                                                    Assets_Shaders_environment_frag, sizeof(Assets_Shaders_environment_frag));
	}

	~EnvironmentSphere()
	{
		ResourceManager::getInstance().unload(m_Shader);
		delete m_Sphere;
	}

	void draw(Context* context, Camera& camera) // TODO : Should be "const Camera&" when possible
	{
		if (m_Texture == NULL)
			LOG("[ENVIRONMENT] Environment sphere needs to be assigned a texture !\n");

		Buffer& vertexBuffer = m_Sphere->getVertexBuffer();
		Buffer& indexBuffer = m_Sphere->getIndexBuffer();

		m_Shader->use();
		m_Shader->uniform("u_ModelViewProjection").set(camera.getProjectionMatrix() * camera.getViewMatrix() * glm::translate(glm::mat4(), camera.getPosition()));
		m_Shader->uniform("u_Texture").set(*m_Texture);
		context->geometry().bind(vertexBuffer, *m_Shader);
		context->geometry().drawElements(GL_TRIANGLES, indexBuffer.size() / sizeof(unsigned short int), GL_UNSIGNED_SHORT, indexBuffer.ptr());
		context->geometry().unbind(vertexBuffer);
	}

	inline void setTexture(Texture* texture) { m_Texture = texture; }

private:
	SphereMesh* m_Sphere;
	Texture* m_Texture;
	Shader::Program* m_Shader;
};

