#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Buffer.hh>

class DrawCall
{
public:
	DrawCall() {}
	virtual ~DrawCall() {}

	inline Shader::Program* shader() { return m_Shader; }
	inline void shader(Shader::Program* s) { m_Shader = s; }

	unsigned int add(Buffer* buffer)
	{
		m_Buffers.push_back(buffer);
		return m_Buffers.size() - 1;
	}

	inline Buffer* buffer(unsigned int index = 0) { return m_Buffers[index]; }

	virtual void execute(Context* context) = 0;

protected:
	Shader::Program* m_Shader;
	std::vector<Buffer*> m_Buffers;
};

class DrawArrays : public DrawCall
{
public:

	void execute(Context* context) override
	{
		m_Shader->use();

		for (auto buffer : m_Buffers)
		{
			context->geometry().bind(*buffer, *m_Shader);
			context->geometry().drawArrays(GL_POINTS, 0, buffer->size() / buffer->blocksize());
			context->geometry().unbind(*buffer);
		}
	}
};

class Batch
{
public:

	Batch()
	{
	}

	virtual ~Batch()
	{
	}

	virtual void execute(Context* context)
	{
		for (auto drawcall : m_DrawCalls)
			drawcall->execute(context);
	}

	void clear() { m_DrawCalls.clear(); }

	void add(DrawCall* drawcall) { m_DrawCalls.push_back(drawcall); }

private:
	std::vector<DrawCall*> m_DrawCalls;
};