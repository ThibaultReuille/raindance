#pragma once

#include "Core/Headers.hh"
#include "Core/Resources/Resource.hh"

class Texture : public IResource
{
public:
	enum Type
	{
		RAW,
		STB
	};

	Texture(const char* name, unsigned char* data, unsigned int size)
	{
		int w, h, c;

		m_Name = std::string(name);
		m_Type = STB;
		m_Data = stbi_load_from_memory(data, size, &w, &h, &c, 0);

		load(m_Data, w, h, c);
	}

	Texture(const char* name, unsigned char* data, int width, int height, int components)
	{
		m_Name = std::string(name);
		m_Type = RAW;
		m_Data = new unsigned char[width * height * components];

		memcpy(m_Data, data, width * height * components);

		load(m_Data, width, height, components);
	}

	Texture(const char* name, int width, int height, int components)
	{
		m_Name = std::string(name);
		m_Type = RAW;
		m_Data = new unsigned char[width * height * components];

		memset(m_Data, 0, width * height * components);

		load(m_Data, width, height, components);
	}

	~Texture()
	{
		glDeleteTextures(1, &m_ID);

		switch(m_Type)
		{
		case STB:
			stbi_image_free(m_Data);
			break;
		case RAW:
			delete m_Data;
		}
	}

	void load(unsigned char* data, unsigned int width, unsigned int height, unsigned int components)
	{
		m_Width = width;
		m_Height = height;
		m_Components = components;

		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#if EMSCRIPTEN
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
#endif
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		switch(components)
		{
		case 1:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
			break;
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;
		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;
		default:
			LOG("[TEXTURE] Format not supported !\n");
		}

#ifndef EMSCRIPTEN
		glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
		glGenerateMipmap(GL_TEXTURE_2D);
#endif
	}

	void update()
	{
		glBindTexture(GL_TEXTURE_2D, m_ID);

		switch(m_Components)
		{
		case 1:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, m_Width, m_Height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, m_Data);
			break;
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_Data);
			break;
		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Data);
			break;
		default:
			LOG("[TEXTURE] Format not supported !\n");
		}

#ifndef EMSCRIPTEN
		glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
		glGenerateMipmap(GL_TEXTURE_2D);
#endif
	}

	void setPixel(unsigned int x, unsigned int y, unsigned char* color, unsigned int size)
	{
		unsigned char* ptr = NULL;

		if (size != m_Components)
		{
			LOG("[TEXTURE] Pixels have %u components !", m_Components);
			return;
		}

		ptr = m_Data + (x + m_Width * y) * m_Components;
		memcpy(ptr, color, m_Components);
	}

	void getPixel(unsigned int x, unsigned int y, unsigned int size, unsigned char* color)
	{
		unsigned char* ptr = NULL;

		if (size != m_Components)
		{
			LOG("[TEXTURE] Pixels have %u components !", m_Components);
			return;
		}

		ptr = m_Data + (x + m_Width * y) * m_Components;
		memcpy(color, ptr, m_Components);
	}

	void dump(const char* filename)
	{
		LOG("[TEXTURE] Output : <%s>\n", filename);

		unsigned int components = 3;
		char* pixels = new char[m_Width * m_Height * components]; // NOTE : Forcing 3 RGB components

		glBindTexture(GL_TEXTURE_2D, m_ID);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

		// NOTE : Flip Y
		for (unsigned int h = 0; h < m_Height / 2; h++)
			for (unsigned int w = 0; w < m_Width; w++)
				for (unsigned int c = 0; c < components; c++)
				{
					unsigned int offset1 = (w + m_Width * h) * components + c;
					unsigned int offset2 = (w + m_Width * (m_Height - h - 1)) * components  + c;
					char t = pixels[offset1];
					pixels[offset1] = pixels[offset2];
					pixels[offset2] = t;
				}

		stbi_write_tga(filename, m_Width, m_Height, components, pixels);

		delete[] pixels;
	}

	const std::string& name() const { return m_Name; }
	unsigned long memory() const { return sizeof(Texture) + m_Width * m_Height * m_Components; }

	inline GLuint id() const { return m_ID; }
	inline GLuint width() const { return m_Width; }
	inline GLuint height() const { return m_Height; }
	inline GLuint components() const { return m_Components; }

private:
	std::string m_Name;

	unsigned int m_Width;
	unsigned int m_Height;
	unsigned int m_Components;
	unsigned char* m_Data;

	GLuint m_ID;

	Type m_Type;
};

class TextureVector
{
public:
	TextureVector()
	{
	}

	~TextureVector()
	{
		m_Textures.erase(m_Textures.begin(), m_Textures.end());
	}

	unsigned long add(Texture* texture)
	{
		m_Textures.push_back(texture);
		return m_Textures.size() - 1;
	}

	void reshape(unsigned int width, unsigned int height)
	{
		std::vector<Texture*>::iterator it;
		for (it = m_Textures.begin(); it != m_Textures.end(); ++it)
		{
			Texture* tex = new Texture((*it)->name().c_str(), width, height, (*it)->components());
			// TODO : Potential need of texture interpolation here
			delete *it;
			*it = tex;
		}
	}

	inline Texture* operator[](unsigned id) { return m_Textures[id]; }

private:
	std::vector<Texture*> m_Textures;
};

