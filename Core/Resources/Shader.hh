#pragma once

#include "Core/Headers.hh"
#include "Core/Resources/Texture.hh"
#include "Core/Resources/Resource.hh"

class Shader
{
public:
	class Stream
	{
	protected:
		Stream()
		{
			m_Name = std::string("");;
			m_Type = GL_NONE;
			m_Size = 0;
			m_Location = -1;
		}

		Stream(const char* name, GLenum type, GLint size, GLint location)
		{
			m_Name = std::string(name);
			m_Type = type;
			m_Size = size;
			m_Location = location;
		}

	public:
		const char* name() const { return m_Name.c_str(); }
		GLint location() const { return m_Location; }
		inline GLenum type() const { return m_Type; }

	protected:
		std::string m_Name;
		GLenum m_Type;
		GLint m_Size;
		GLint m_Location;
	};

	class Attribute : public Stream
	{
	public:
		Attribute()
		: Stream()
		{
		}

		Attribute(const char* name, GLenum type, GLint size, GLint location)
		: Stream(name, type, size, location)
		{
		}
	};

	class Uniform : public Stream
	{
	public:
		Uniform()
		: Stream()
		{
		}

		Uniform(const char* name, GLenum type, GLint size, GLint location)
		: Stream(name, type, size, location)
		{
		}

		void set(GLfloat n1) const
		{
			if (m_Type != GL_FLOAT)
				LOG("[SHADER] Uniform type differs !\n");
			glUniform1f(m_Location, n1);
		}
		void set(GLint n1) const
		{
			if (m_Type != GL_INT)
				LOG("[SHADER] Uniform type differs !\n");
			glUniform1i(m_Location, n1);
		}
		void set(GLuint n1) const
		{
			if (m_Type != GL_UNSIGNED_INT)
				LOG("[SHADER] Uniform type differs !\n");
			glUniform1i(m_Location, n1);
		}
		void set(GLfloat n1, GLfloat n2) const
		{
			if (m_Type != GL_FLOAT_VEC2)
				LOG("[SHADER] Uniform type differs !\n");
			glUniform2f(m_Location, n1, n2);
		}
		void set(GLfloat n1, GLfloat n2, GLfloat n3) const
		{
			if (m_Type != GL_FLOAT_VEC3)
				LOG("[SHADER] Uniform type differs !\n");
			glUniform3f(m_Location, n1, n2, n3);
		}
		void set(GLfloat n1, GLfloat n2, GLfloat n3, GLfloat n4) const
		{
			if (m_Type != GL_FLOAT_VEC4)
				LOG("[SHADER] Uniform type differs !\n");
			glUniform4f(m_Location, n1, n2, n3, n4);
		}
		void set(GLint n1, GLint n2, GLint n3, GLint n4) const
		{
			if (m_Type != GL_INT_VEC4)
				LOG("[SHADER] Uniform type differs !\n");
			glUniform4i(m_Location, n1, n2, n3, n4);
		}
		void set(const GLfloat* v, GLsizei n) const
		{
			// TODO : Type check
			glUniform1fv(m_Location, n, v);
		}
		void set(const glm::vec3& v) const
		{
			if (m_Type != GL_FLOAT_VEC3)
				LOG("[SHADER] Uniform type differs !\n");
			glUniform3fv(m_Location, 1, glm::value_ptr(v));
		}
		void set(const glm::vec4& v) const
		{
			if (m_Type != GL_FLOAT_VEC4)
				LOG("[SHADER] Uniform type differs !\n");
			glUniform4fv(m_Location, 1, glm::value_ptr(v));
		}
		void set(const glm::mat3& m) const
		{
			if (m_Type != GL_FLOAT_MAT3)
				LOG("[SHADER] Uniform type differs !\n");
			glUniformMatrix3fv(m_Location, 1, GL_FALSE, glm::value_ptr(m));
		}
		void set(const glm::mat4& m) const
		{
			if (m_Type != GL_FLOAT_MAT4)
				LOG("[SHADER] Uniform type differs !\n");
			glUniformMatrix4fv(m_Location, 1, GL_FALSE, glm::value_ptr(m));
		}
        void set(const Texture& texture) const
        {
            if (m_Type != GL_SAMPLER_2D)
                LOG("[SHADER] Uniform type differs !\n");
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture.id());
            glUniform1i(m_Location, /* GL_TEXTURE */ 0);
        }
        void set(const Texture* texture) const
        {
            if (texture == NULL)
            {
                LOG("[SHADER] Uniform texture is NULL!\n");
                return;
            }
            set(*texture);
        }
	};

	class Program : public IResource
	{
	public:

		friend class ResourceManager;

		void use()
		{
			glUseProgram(m_Object);
		}

		const Attribute& attribute(const char* name)
		{
			std::vector<Attribute>::iterator it;
			for(it = m_Attributes.begin(); it != m_Attributes.end(); ++it)
			{
				if (strcmp(it->name(), name) == 0)
				{
					return *it;
				}
			}
			LOG("[SHADER] Attribute '%s' not found !", name);
			throw;
		}

		const Uniform& uniform(const char* name)
		{
			std::vector<Uniform>::iterator it;
			for(it = m_Uniforms.begin(); it != m_Uniforms.end(); ++it)
			{
				if (strcmp(it->name(), name) == 0)
				{
					return *it;
				}
			}

			LOG("[SHADER] Uniform '%s' not found !", name);
			throw;
		}

		const char* toString(GLenum e)
		{
			switch(e)
			{
				case GL_FLOAT:
					return "GL_FLOAT";
				case GL_FLOAT_VEC2:
					return "GL_FLOAT_VEC2";
				case GL_FLOAT_VEC3:
					return "GL_FLOAT_VEC3";
				case GL_FLOAT_VEC4:
					return "GL_FLOAT_VEC4";
				case GL_FLOAT_MAT2:
					return "GL_FLOAT_MAT2";
				case GL_FLOAT_MAT3:
					return "GL_FLOAT_MAT3";
				case GL_FLOAT_MAT4:
					return "GL_FLOAT_MAT4";

				case GL_INT:
					return "GL_INT";
				case GL_INT_VEC2:
					return "GL_INT_VEC2";
				case GL_INT_VEC3:
					return "GL_INT_VEC3";
				case GL_INT_VEC4:
					return "GL_INT_VEC4";

				case GL_SAMPLER_2D:
					return "GL_SAMPLER_2D";

				default:
					return "UNKNOWN";
			}
		}

		void dump()
		{
			std::vector<Uniform>::iterator uit;
			std::vector<Attribute>::iterator ait;

			std::cout
				<< "Shader \"" << m_Name << "\"" << std::endl
				<< ". Uniforms (" << m_Uniforms.size() << ")" << std::endl;
			for(uit = m_Uniforms.begin(); uit != m_Uniforms.end(); ++uit)
			{
				std::cout << "    . " << uit->name() << " : " << uit->location() << ", " << toString(uit->type()) << std::endl;
			}
			std::cout
				<< ". Attributes (" << m_Attributes.size() << ")" << std::endl;
			for(ait = m_Attributes.begin(); ait != m_Attributes.end(); ++ait)
			{
				std::cout << "    . " << ait->name() << " : " << ait->location() << ", " << toString(ait->type()) << std::endl;
			}
		}

		const std::string& name() const { return m_Name; }

		unsigned long memory() const
		{
			return sizeof(Shader::Program)
				+ m_Uniforms.size() * sizeof(Uniform)
				+ m_Attributes.size() * sizeof(Attribute);
		}

	private:
		void load(const char* name, const char* vertex, unsigned long vertex_length, const char* fragment, unsigned long fragment_length)
		{
			m_Name = std::string(name);

			m_Object = glCreateProgram();
			if (m_Object == 0)
				LOG("[SHADER] Couldn't create shader program !\n");

			if (vertex != NULL)
				m_VertexShader = loadShader(GL_VERTEX_SHADER, vertex, vertex_length);

			if (fragment != NULL)
				m_FragmentShader = loadShader(GL_FRAGMENT_SHADER, fragment, fragment_length);

			glAttachShader(m_Object, m_VertexShader);
			glAttachShader(m_Object, m_FragmentShader);

			glLinkProgram(m_Object);
			glGetProgramiv(m_Object, GL_LINK_STATUS, &m_Linked);
			if (!m_Linked)
			{
				GLint infoLogLength = 0;
				glGetProgramiv(m_Object, GL_INFO_LOG_LENGTH, &infoLogLength);
				if (infoLogLength > 1)
				{
					char* infoLog = reinterpret_cast<char*>(malloc(sizeof(char) * infoLogLength));
					glGetProgramInfoLog(m_Object, infoLogLength, NULL, infoLog);
					LOG("[SHADER] Linking Error :\n%s\n", infoLog);
					free(infoLog);
				}

				glDeleteProgram(m_Object);
				return;
			}

			GLint number;
			GLint maxlength;
			GLint length;
			GLint size;
			GLenum type;
			GLint location;
			char* varname;

			glGetProgramiv(m_Object, GL_ACTIVE_UNIFORMS, &number);
			// TODO : WebGL seems to return 0 in maxlength, using 256 for now
			// glGetProgramiv(m_Object, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxlength);
			maxlength = 256;
			varname = new char[maxlength];
			for(int i = 0; i < number; i++)
			{
				glGetActiveUniform(m_Object, i, maxlength, &length, &size, &type, varname);
				location = glGetUniformLocation(m_Object, varname);
				Uniform uniform(varname, type, size, location);
				m_Uniforms.push_back(uniform);
			}
			delete[] varname;

			glGetProgramiv(m_Object, GL_ACTIVE_ATTRIBUTES, &number);
			// TODO : WebGL seems to return 0 in maxlength, using 256 for now
			// glGetProgramiv(m_Object, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxlength);
			maxlength = 256;
			varname = new char[maxlength];
			for(int i = 0; i < number; i++)
			{
				glGetActiveAttrib(m_Object, i, maxlength, &length, &size, &type, varname);
				location = glGetAttribLocation(m_Object, varname);
				Attribute attribute(varname, type, size, location);
				m_Attributes.push_back(attribute);
			}
			delete[] varname;

			glValidateProgram(m_Object);
			glUseProgram(m_Object);
		}

		void load(const char* name, const unsigned char* vertex, unsigned long vertexSize, const unsigned char* fragment, unsigned long fragmentSize)
		{
			load(name, (const char*) vertex, vertexSize, (const char*) fragment, fragmentSize);
		}

		GLuint loadShader(GLenum type, const char* source, unsigned long length)
		{
			GLuint shader;
			GLint compiled;

			shader = glCreateShader(type);
			if (shader == 0)
			{
				return 0;
			}
			GLint shaderLength = static_cast<GLint>(length);
			glShaderSource(shader, 1, &source, &shaderLength);
			glCompileShader(shader);

			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
			if (!compiled)
			{
				GLint infoLogLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
				if (infoLogLength > 1)
				{
					char* infoLog = reinterpret_cast<char*>(malloc(sizeof(char) * infoLogLength));
					glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
					LOG("[SHADER] Compilation Error :\n%s\n", infoLog);
					free(infoLog);
				}

				glDeleteShader(shader);
				return 0;
			}

			return shader;
		}

		std::string m_Name;
		GLuint m_Object;
		GLuint m_VertexShader;
		GLuint m_FragmentShader;
		GLint m_Linked;

		std::vector<Uniform> m_Uniforms;
		std::vector<Attribute> m_Attributes;
	};
};

