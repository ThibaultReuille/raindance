#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Resources/Shader.hh>

struct Buffer
{
public:
	struct Description
	{
		GLsizei Size;
		GLenum Type;
		GLsizei Stride;
		GLuint Offset;
		std::string Attribute;
		GLint Location;
		bool Mute;
	};

	enum BufferFlag
	{
		GPU_SYNC
	};

	enum BufferUsage
	{
		STATIC,
		DYNAMIC,
		STREAM
	};

	Buffer()
	{
		m_Generated = false;
		m_Synchronized = true;
		m_Usage = GL_NONE;
	}

	virtual ~Buffer()
	{
	    if (m_Generated)
	        glDeleteBuffers(1, &m_VBO);
	}

	inline void push(const void* data, std::size_t size)
	{
		std::copy (static_cast<const unsigned char*>(data), static_cast<const unsigned char*>(data) + size, std::back_inserter(m_Data));
	}

	void generate(BufferUsage usage)
	{
		switch(usage)
		{
		case DYNAMIC:
			m_Usage = GL_DYNAMIC_DRAW;
			break;
		case STATIC:
			m_Usage = GL_STATIC_DRAW;
			break;
		case STREAM:
			m_Usage = GL_STREAM_DRAW;
			break;
		default:
			LOG("Buffer usage not supported !");
			return;
		}

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_Data.size(), &m_Data.front(), m_Usage);
		m_Generated = true;
	}

	void describe(const char* attribute, GLsizei size, GLenum type, GLsizei stride, GLuint offset)
	{
		Description description;

		description.Size = size;
		description.Type = type;
		description.Stride = stride;
		description.Offset = offset;
		description.Attribute = std::string(attribute);
		description.Location = -1;
		description.Mute = false;

		m_Descriptions.push_back(description);
	}

	void mute(const char* attribute, bool flag)
	{
	    std::string a(attribute);
	    for (auto& d : m_Descriptions)
	    {
	        if (d.Attribute == a)
	        {
	            d.Mute = flag;
	            return;
	        }
	    }
	}

	void clear()
	{
		m_Data.clear();
		m_Descriptions.clear();
	}

	void dump(unsigned int size)
	{
		std::vector<unsigned char>::iterator it;

		int count = 1;
		for (it = m_Data.begin(); it != m_Data.end(); ++it)
		{
			std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)*it;

			if (count % size != 0)
				std::cout << " ";
			else
				std::cout << std::endl;

			count++;
		}
	}

	inline void get(unsigned long offset, void* data, unsigned int size) const
	{
		memcpy(data, &m_Data[offset * size], size);
	}

	inline void set(unsigned long offset, const void* data, unsigned int size)
	{
		memcpy(&m_Data[offset * size], data, size);

		if (m_Generated && m_Synchronized)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferSubData(GL_ARRAY_BUFFER, offset  * size, size, data);
		}
	}

	inline void update()
	{
		if (m_Generated && m_Synchronized)
		{
			if (m_Usage == GL_STATIC_DRAW)
			{
				LOG("Unable to update a static buffer !");
				return;
			}

            GLint bufferSize;

            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);

            if (m_Data.size() > static_cast<unsigned int>(bufferSize))
                glBufferData(GL_ARRAY_BUFFER, m_Data.size(), &m_Data.front(), m_Usage);
            else
                glBufferSubData(GL_ARRAY_BUFFER, 0, m_Data.size(), &m_Data.front());
		}
	}

	void enable(BufferFlag flag)
	{
		switch(flag)
		{
		case GPU_SYNC:
			m_Synchronized = true;
			break;
		default:
			break;
		}
	}

	void disable(BufferFlag flag)
	{
		switch(flag)
		{
		case GPU_SYNC:
			m_Synchronized = false;
			break;
		default:
			break;
		}
	}

	inline const std::vector<Description>::iterator descriptions_begin()
	{
		return m_Descriptions.begin();
	}

	inline const std::vector<Description>::iterator descriptions_end()
	{
		return m_Descriptions.end();
	}

	inline GLuint vbo() const { return m_VBO; }

	inline GLuint size() const { return static_cast<GLuint>(m_Data.size()); }

	inline unsigned char* ptr() { return m_Data.data(); }

    inline bool isGenerated() { return m_Generated; }
    inline bool isSynchronized() { return m_Synchronized; }

	Buffer& operator<<(GLchar c)
	{
		push(&c, sizeof(GLchar));
		return *this;
	}
	Buffer& operator<<(GLushort s)
	{
		push(&s, sizeof(GLushort));
		return *this;
	}
	Buffer& operator<<(GLuint i)
	{
		push(&i, sizeof(GLuint));
		return *this;
	}
	Buffer& operator<<(GLfloat f)
	{
		push(&f, sizeof(GLfloat));
		return *this;
	}
	Buffer& operator<<(glm::vec2 v)
	{
		push(glm::value_ptr(v), sizeof(glm::vec2));
		return *this;
	}
	Buffer& operator<<(glm::vec3 v)
	{
		push(glm::value_ptr(v), sizeof(glm::vec3));
		return *this;
	}
	Buffer& operator<<(glm::vec4 v)
	{
		push(glm::value_ptr(v), sizeof(glm::vec4));
		return *this;
	}
	Buffer& operator<<(glm::mat4 m)
	{
		push(glm::value_ptr(m), sizeof(glm::mat4));
		return *this;
	}

private:
	std::vector<unsigned char> m_Data;
	std::vector<Description> m_Descriptions;
	GLuint m_VBO;
	GLenum m_Usage;
	bool m_Generated;
	bool m_Synchronized;
};
