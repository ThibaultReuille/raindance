#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Primitives/Box.hh>

class PointCloud
{
public:

	typedef unsigned long ID;

	struct PointVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};

	struct Filter
	{
		bool Active;
		float Threshold;
		float Value;
	};

	enum DrawMode
	{
		POINTS,
		ICONS
	};

	Filter m_FilterZ;

	PointCloud()
	{
		m_Box = new Box();

		m_Shader = ResourceManager::getInstance().loadShader("pointcloud", Resources_Shaders_pointcloud_vert, sizeof(Resources_Shaders_pointcloud_vert),
		                                                                   Resources_Shaders_pointcloud_frag, sizeof(Resources_Shaders_pointcloud_frag));

		m_LayerTexture = ResourceManager::getInstance().loadTexture("miller-bw", Resources_Earth_miller_2048x1502_bw_jpg, sizeof(Resources_Earth_miller_2048x1502_bw_jpg));
		m_LayerShader = ResourceManager::getInstance().loadShader("world", Resources_world_vert, sizeof(Resources_world_vert), Resources_world_frag, sizeof(Resources_world_frag));

		m_PointIcon = new Icon();
		m_PointIcon->load("disk", Resources_Particle_ball_png, sizeof(Resources_Particle_ball_png));

		m_DrawMode = POINTS;

		update();
	}

	~PointCloud()
	{
		SAFE_DELETE(m_PointIcon);

		ResourceManager::getInstance().unload(m_LayerTexture);
		ResourceManager::getInstance().unload(m_LayerShader);

		ResourceManager::getInstance().unload(m_Shader);
		delete m_Box;
	}

	void draw(Context* context, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model)
	{
		Transformation transformation;
		transformation.set(model);

		m_Box->draw(context, projection * view * model);

		switch(m_DrawMode)
		{
		case POINTS:
			glPointSize(2.0f);

			m_Shader->use();
			m_Shader->uniform("u_ModelViewProjection").set(projection * view * model);
			m_Shader->uniform("u_Box.min").set(m_Box->min());
			m_Shader->uniform("u_Box.max").set(m_Box->max());

			// m_Shader->uniform("u_FilterX.active").set(1.0f);
			// m_Shader->uniform("u_FilterX.value").set(value);
			// m_Shader->uniform("u_FilterX.threshold").set(0.5f);

			// m_Shader->uniform("u_FilterY.active").set(1.0f);
			// m_Shader->uniform("u_FilterY.value").set(0.5f);
			// m_Shader->uniform("u_FilterY.threshold").set(0.9f);

			m_Shader->uniform("u_FilterZ.active").set(m_FilterZ.Active ? 1 : 0);
			m_Shader->uniform("u_FilterZ.value").set(m_FilterZ.Value);
			m_Shader->uniform("u_FilterZ.threshold").set(m_FilterZ.Threshold);

			context->geometry().bind(m_VertexBuffer, *m_Shader);
			context->geometry().drawArrays(GL_POINTS, 0, m_VertexBuffer.size() / sizeof(PointVertex));
			context->geometry().unbind(m_VertexBuffer);
			break;

		case ICONS:
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			float iconSize = 0.5;
			PointVertex point;
			unsigned int size = m_VertexBuffer.size() / sizeof(PointVertex);
			for (unsigned int i = 0; i < size; i++)
			{
				m_VertexBuffer.get(i, &point, sizeof(PointVertex));

				glm::mat4 modelView = view * glm::translate(transformation.state(), point.Position);
				modelView = glm::scale(Geometry::billboard(modelView), glm::vec3(iconSize, iconSize, iconSize));
				m_PointIcon->draw(context, projection * modelView, point.Color, 0);
			}
			glDisable(GL_BLEND);
			break;
		}

		/*
		{
			static unsigned short int indices[] = { 0, 1, 2, 0, 2, 3 };
			m_LayerShader->use();
			m_LayerShader->uniform("u_ModelViewProjection").set(mvp);
			m_LayerShader->uniform("u_Texture").set(*m_LayerTexture);
			context->geometry().bind(m_LayerVertexBuffer, *m_LayerShader);
			context->geometry().drawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned short int), GL_UNSIGNED_SHORT, indices);
			context->geometry().unbind(m_LayerVertexBuffer);
		}
		*/
	}

	unsigned long addPoint(PointVertex& point)
	{
		m_VertexBuffer.push(&point, sizeof(PointVertex));
		return m_VertexBuffer.size() / sizeof(PointVertex);
	}

	void update()
	{
		static unsigned int updateCount = 0;

		m_Box->reset();
		PointVertex point;
		for (unsigned long i = 0; i < m_VertexBuffer.size() / sizeof(PointVertex); i++)
		{
			m_VertexBuffer.get(i, &point, sizeof(PointVertex));
			m_Box->adjust(point.Position);
		}
		m_Box->update();

		if (updateCount == 0)
		{
			m_VertexBuffer.enable(Buffer::GPU_SYNC);
			m_VertexBuffer.describe("a_Position", 3, GL_FLOAT, sizeof(PointVertex), 0);
			m_VertexBuffer.describe("a_Color", 4, GL_FLOAT, sizeof(PointVertex), 3 * sizeof(GLfloat));
			m_VertexBuffer.generate(Buffer::DYNAMIC);
			m_VertexBuffer.disable(Buffer::GPU_SYNC);
		}
		else
		{
			m_VertexBuffer.enable(Buffer::GPU_SYNC);
			m_VertexBuffer.update();
			m_VertexBuffer.describe("a_Position", 3, GL_FLOAT, sizeof(PointVertex), 0);
			m_VertexBuffer.describe("a_Color", 4, GL_FLOAT, sizeof(PointVertex), 3 * sizeof(GLfloat));
			m_VertexBuffer.disable(Buffer::GPU_SYNC);
		}

		m_LayerVertexBuffer.clear();
		glm::vec3 min = m_Box->min();
		glm::vec3 max = m_Box->max();
		m_LayerVertexBuffer << glm::vec3(min.x, min.y, min.z) << glm::vec2(0.0, 1.0);
		m_LayerVertexBuffer << glm::vec3(max.x, min.y, min.z) << glm::vec2(1.0, 1.0);
		m_LayerVertexBuffer << glm::vec3(max.x, max.y, min.z) << glm::vec2(1.0, 0.0);
		m_LayerVertexBuffer << glm::vec3(min.x, max.y, min.z) << glm::vec2(0.0, 0.0);
		if (updateCount == 0)
		{
			m_LayerVertexBuffer.enable(Buffer::GPU_SYNC);
			m_LayerVertexBuffer.describe("a_Position", 3, GL_FLOAT, 5 * sizeof(GLfloat), 0);
			m_LayerVertexBuffer.describe("a_Texcoord", 2, GL_FLOAT, 5 * sizeof(GLfloat), 3 * sizeof(GLfloat));
			m_LayerVertexBuffer.generate(Buffer::DYNAMIC);
			m_LayerVertexBuffer.disable(Buffer::GPU_SYNC);
		}
		else
		{
			m_LayerVertexBuffer.enable(Buffer::GPU_SYNC);
			m_LayerVertexBuffer.update();
			m_LayerVertexBuffer.describe("a_Position", 3, GL_FLOAT, 5 * sizeof(GLfloat), 0);
			m_LayerVertexBuffer.describe("a_Texcoord", 2, GL_FLOAT, 5 * sizeof(GLfloat), 3 * sizeof(GLfloat));
			m_LayerVertexBuffer.disable(Buffer::GPU_SYNC);
		}

		updateCount++;
	}

	inline const Box& box() const { return *m_Box; }

	inline void getPoint(unsigned long i, PointVertex* point) const { m_VertexBuffer.get(i, point, sizeof(PointVertex)); }

	inline void setPoint(unsigned long i, PointVertex* point) { m_VertexBuffer.set(i, point, sizeof(PointVertex)); }

	inline unsigned int countPoints() { return m_VertexBuffer.size() / sizeof(PointVertex); }

	float computeScalar(const glm::vec3& v)
	{
		float value = 0;

		float radius = 0.01;

		PointVertex point;
		unsigned long size = m_VertexBuffer.size() / sizeof(PointVertex);
		for (unsigned int i = 0; i < size; i++)
		{
			m_VertexBuffer.get(i, &point, sizeof(PointVertex));

			float d = glm::length2(v - point.Position);
			value += radius / d;
		}

		return value;
	}

	ID getNearestPoint(const glm::vec3& v)
	{
		float minDistance = std::numeric_limits<float>::max();
		ID result;

		PointVertex point;
		for (unsigned long i = 0; i < m_VertexBuffer.size() / sizeof(PointVertex); i++)
		{
			m_VertexBuffer.get(i, &point, sizeof(PointVertex));

			float d = glm::length2(v - point.Position);
			if (d < minDistance)
			{
				minDistance = d;
				result = i;
			}
		}

		return result;
	}

private:
	Box* m_Box;

	DrawMode m_DrawMode;

	Buffer m_VertexBuffer;
	Shader::Program* m_Shader;

	Buffer m_LayerVertexBuffer;
	Texture* m_LayerTexture;
	Shader::Program* m_LayerShader;

	Icon* m_PointIcon;
};

