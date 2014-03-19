#pragma once

#include "Core/Headers.hh"
#include "Core/Buffer.hh"
#include "Core/Resources/Shader.hh"

class Geometry
{
public:

    struct Metrics
    {
        Metrics()
        {
            reset();
        }

        void reset()
        {
            Frames = 0;
            DrawArrays = 0;
            DrawElements = 0;
        }

        void dump() const
        {
            LOG("[METRICS] Frames : %u\n", Frames);
            LOG("[METRICS] DrawArrays : %u\n", DrawArrays);
            LOG("[METRICS] DrawElements : %u\n", DrawElements);
            LOG("[METRICS] Average # of drawcalls per frame : %u\n", (DrawArrays + DrawElements) / Frames);
            LOG("[METRICS] ---\n");
        }

        unsigned int Frames;
        unsigned int DrawArrays;
        unsigned int DrawElements;
    };

    Geometry()
    {
    }

    static glm::mat4 billboard(glm::mat4 mv)
    {
        return glm::mat4
        (
            1.0f,     0.0f,     0.0f,     mv[0][3],
            0.0f,     1.0f,     0.0f,     mv[1][3],
            0.0f,     0.0f,     1.0f,     mv[2][3],
            mv[3][0], mv[3][1], mv[3][2], mv[3][3]
        );
    }

	void bind(Buffer& buffer, Shader::Program& shader)
	{
		std::vector<Buffer::Description>::iterator it;

		glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo());

		for (it = buffer.descriptions_begin(); it != buffer.descriptions_end(); ++it)
		{
			if (it->Location == -1)
			{
				it->Location = shader.attribute(it->Attribute.c_str()).location();
			}
			glVertexAttribPointer(it->Location, it->Size, it->Type, GL_FALSE, it->Stride, reinterpret_cast<void*>(it->Offset));
		}

		for (it = buffer.descriptions_begin(); it != buffer.descriptions_end(); ++it)
		{
			glEnableVertexAttribArray(it->Location);
		}
	}

	void unbind(Buffer& buffer)
	{
		std::vector<Buffer::Description>::iterator it;

		for (it = buffer.descriptions_begin(); it != buffer.descriptions_end(); ++it)
		{
			glDisableVertexAttribArray(it->Location);
		}
	}

	inline void beginFrame()
	{
	    m_Metrics.Frames++;
	}

	inline void endFrame()
	{
	}

	inline void drawArrays(GLenum mode, GLint first, GLsizei count)
	{
	    glDrawArrays(mode, first, count);
        m_Metrics.DrawArrays++;
	}

	inline void drawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices)
	{
	    glDrawElements(mode, count, type, indices);
	    m_Metrics.DrawElements++;
	}

	inline Metrics& getMetrics() { return m_Metrics; }

private:
	Metrics m_Metrics;
};

