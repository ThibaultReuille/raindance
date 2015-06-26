#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Transformation.hh>

class Polyline
{
public:
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};

	enum Mode
	{
		LINE_STRIP,
		WIDE_LINES
	};

	Polyline()
	{
		 m_LineStripShader = ResourceManager::getInstance().loadShader("shaders/primitives/polyline",
            Assets_Shaders_Primitives_polyline_vert, sizeof(Assets_Shaders_Primitives_polyline_vert),
            Assets_Shaders_Primitives_polyline_frag, sizeof(Assets_Shaders_Primitives_polyline_frag));
        m_LineStripShader->dump();

		m_WideLineShader = ResourceManager::getInstance().loadShader("shaders/primitives/wideline",
            Assets_Shaders_Primitives_wideline_vert, sizeof(Assets_Shaders_Primitives_wideline_vert),
            Assets_Shaders_Primitives_wideline_frag, sizeof(Assets_Shaders_Primitives_wideline_frag));
        // m_WideLineShader->dump();

		m_Mode = WIDE_LINES;

		m_Color = glm::vec4(1.0, 1.0, 1.0, 1.0);
		
		m_WideLine.setColor(0, m_Color);
        m_WideLine.setColor(1, m_Color);
        m_WideLine.update();
        m_Width = 1.0;
	}

	virtual ~Polyline()
	{
		ResourceManager::getInstance().unload(m_WideLineShader);
		ResourceManager::getInstance().unload(m_LineStripShader);
	}

	inline void draw(Context& context, Camera& camera, Transformation transformation, const std::vector<Vertex>& vertices, size_t begin, size_t end)
	{
		switch(m_Mode)
		{
			case LINE_STRIP:
				drawLineStrip(context, camera, transformation, vertices, begin, end);
				break;
			case WIDE_LINES:
				drawWideLines(context, camera, transformation, vertices, begin, end);
				break;
			default:
				break;
		}
	}

	// TODO : Use/Implement a true circular buffer with appropriate iterators
	void drawLineStrip(Context& context, Camera& camera, Transformation transformation, const std::vector<Vertex>& vertices, size_t begin, size_t end)
	{
		(void) context;
		(void) vertices;
		
		m_LineStripShader->use();
		m_LineStripShader->uniform("u_ModelViewProjection").set(camera.getViewProjectionMatrix() * transformation.state());

		if (begin < end)
		{
			// TODO : draw from begin to end
		}
		else
		{
			// TODO : draw from begin to vertices.size, then from 0 to end
		}
	}

	// TODO : Use/Implement a true circular buffer with appropriate iterators
	void drawWideLines(Context& context, Camera& camera, Transformation transformation, const std::vector<Vertex>& vertices, size_t begin, size_t end)
	{
		m_WideLineShader->use();
        
        m_WideLineShader->uniform("u_ModelViewProjection").set(camera.getViewProjectionMatrix() * transformation.state());
        // m_WideLineShader->uniform("u_ExtrudeDirection").set(glm::vec3(0.0, 1.0, 0.0));
        // m_WideLineShader->uniform("u_Texture").set(Icon->getTexture(m_TextureID));

        size_t index = begin;
        
        glm::vec3 lastPos;
        glm::vec3 currPos = vertices[index].Position;
        glm::vec3 shift = glm::vec3(currPos.x, 0.0, 0.0);

        do
        {
            index = (index + 1) % vertices.size();
            if (index == end)
                break;

			lastPos = currPos;
            currPos = vertices[index].Position;

            if (false)
            {
            	glLineWidth(m_Width);

                m_WideLineShader->uniform("u_Mode").set(0.0f);
                m_WideLineShader->uniform("u_StartPosition").set(lastPos - shift);
                m_WideLineShader->uniform("u_EndPosition").set(currPos - shift);
                m_WideLineShader->uniform("u_Tint").set(m_Color);

                context.geometry().bind(m_WideLine.getVertexBuffer(), *m_WideLineShader);
                context.geometry().drawArrays(GL_LINES, 0, 2 * sizeof(WideLine::Vertex));
                context.geometry().unbind(m_WideLine.getVertexBuffer());

                glLineWidth(1.0);
            }
            else
            {
            	glm::vec3 start = lastPos - shift;
            	glm::vec3 stop = currPos - shift;
            	glm::vec3 diff = stop - start;

                glm::vec3 extrusion = glm::normalize(glm::vec3(-diff.y, diff.x, 0.0));

                m_WideLineShader->uniform("u_Mode").set(3.0f);
                m_WideLineShader->uniform("u_StartPosition").set(start);
                m_WideLineShader->uniform("u_EndPosition").set(stop);
                m_WideLineShader->uniform("u_ExtrudeDirection").set(m_Width * extrusion);
                m_WideLineShader->uniform("u_Tint").set(m_Color);

                context.geometry().bind(m_WideLine.getVertexBuffer(), *m_WideLineShader);
                context.geometry().drawArrays(GL_TRIANGLE_STRIP, 0, m_WideLine.getVertexBuffer().size() / sizeof(WideLine::Vertex));
                context.geometry().unbind(m_WideLine.getVertexBuffer());
            }
        }
        while(true);
	}

	inline void setMode(Mode mode) { m_Mode = mode; }
	inline Mode getMode() { return m_Mode; }

	inline void setColor(const glm::vec4& color) { m_Color = color; }
	inline const glm::vec4& getColor() const { return m_Color; }

	inline void setWidth(float width) { m_Width = width; }
	inline float getWidth() const { return m_Width; }

private:
	Shader::Program* m_LineStripShader;
	Shader::Program* m_WideLineShader;
	Mode m_Mode;
	glm::vec4 m_Color;
	float m_Width;
    WideLine m_WideLine;
};