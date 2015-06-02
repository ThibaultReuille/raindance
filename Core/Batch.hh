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

/*
int main()
{
	auto buffer0 = new Buffer();
	buffer0 << glm::vec2(0, 0);
	buffer0 << glm::vec2(1, 0);
	buffer0 << glm::vec2(0, 1);
	buffer0 << glm::vec2(1, 1);
	buffer0->describe("a_Position")

	auto buffer1 = new Buffer();
	for (int i = 0; i < 128; i++)
		buffer1 << glm::vec3(i / 16, i % 16, 0);
	buffer1->generate(Buffer::STATIC);

    FS::TextFile vert("Assets/TimeSeries/timevector.vert");
    FS::TextFile geom("Assets/TimeSeries/timevector.geom");
    FS::TextFile frag("Assets/TimeSeries/timevector.frag");
    auto shader1 = ResourceManager::getInstance().loadShader("test/test", vert.content(), frag.content(), geom.content());
	
	auto drawcall = new Drawcall();	
	drawcall->shader(shader1);
	drawcall->uniform()
	drawcall->texture()
	drawcall->buffer(buffer1)

	auto batch = new Batch();
	batch.add(drawcall1);

	batch->execute();
}
*/