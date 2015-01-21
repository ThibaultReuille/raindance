#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Buffer.hh>
#include <raindance/Core/Resources/Shader.hh>

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
            GeometryObjects = 0;
            Frames = 0;
            DrawArrays = 0;
            DrawArraysInstanced = 0;
            DrawElements = 0;
        }

        void dump() const
        {
            LOG("[METRICS] Geometry Objects : %u\n", GeometryObjects);
            LOG("[METRICS] Frames : %u\n", Frames);
            LOG("[METRICS] DrawArrays : %u\n", DrawArrays);
            LOG("[METRICS] DrawArraysInstanced : %u\n", DrawArraysInstanced);
            LOG("[METRICS] DrawElements : %u\n", DrawElements);
            LOG("[METRICS] Average # of drawcalls per frame : %u\n", (DrawArrays + DrawArraysInstanced + DrawElements) / Frames);
            LOG("[METRICS] ---\n");
        }

        unsigned int GeometryObjects;
        unsigned int Frames;
        unsigned int DrawArrays;
        unsigned int DrawArraysInstanced;
        unsigned int DrawElements;
    };

    Geometry()
    {
        getMetrics().GeometryObjects++;
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
            if (it->Mute)
                continue;
            if (it->Location == -1)
                it->Location = shader.attribute(it->Attribute.c_str()).location();
 
            glEnableVertexAttribArray(it->Location);
			glVertexAttribPointer(it->Location, it->Size, it->Type, GL_FALSE, it->Stride, reinterpret_cast<void*>(it->Offset));
		}
	}

	void unbind(Buffer& buffer)
	{
		std::vector<Buffer::Description>::iterator it;

		for (it = buffer.descriptions_begin(); it != buffer.descriptions_end(); ++it)
		{
		    if (it->Mute)
		        continue;
			glDisableVertexAttribArray(it->Location);
		}
	}

	static inline void beginFrame()
	{
	    getMetrics().Frames++;
	}

	static inline void endFrame()
	{
	}

    inline void drawArrays(GLenum mode, GLint first, GLsizei count)
    {
        glDrawArrays(mode, first, count);
        getMetrics().DrawArrays++;
    }

    inline void drawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei primcount)
    {
        // TODO : Add proper ifdef protection for non GL 3.3 machines
        glDrawArraysInstancedARB(mode, first, count, primcount);
        getMetrics().DrawArraysInstanced++;
    }

	inline void drawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices)
	{
	    glDrawElements(mode, count, type, indices);
	    getMetrics().DrawElements++;
	}

    static inline Metrics& getMetrics()
    {
        static Metrics instance;
        return instance;
    }
};

