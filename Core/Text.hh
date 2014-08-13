#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Geometry.hh>
#include <raindance/Core/Resources/Texture.hh>
#include <raindance/Core/Resources/Shader.hh>

#include <raindance/Lib/stb_fonts/usascii/courier_bold/stb_font_courier_bold_18_usascii.inl>

class Font
{
public:
	Font()
	{
		m_Characters.resize(STB_SOMEFONT_NUM_CHARS);

		static unsigned char pixels[STB_SOMEFONT_BITMAP_HEIGHT_POW2][STB_SOMEFONT_BITMAP_WIDTH];
		STB_SOMEFONT_CREATE(&m_Characters.front(), pixels, STB_SOMEFONT_BITMAP_HEIGHT_POW2);

		m_Texture = ResourceManager::getInstance().loadTexture("font", (unsigned char*) pixels, STB_SOMEFONT_BITMAP_WIDTH, STB_SOMEFONT_BITMAP_HEIGHT_POW2, 1);
		m_Shader = ResourceManager::getInstance().loadShader("text", Resources_Shaders_text_vert, sizeof(Resources_Shaders_text_vert),
		                                                             Resources_Shaders_text_frag, sizeof(Resources_Shaders_text_frag));
		// m_Shader->dump();
	}

	~Font()
	{
		ResourceManager::getInstance().unload(m_Shader);
		ResourceManager::getInstance().unload(m_Texture);
	}

	void build(const char* str, Buffer& buffer)
	{
		buffer.clear();

		float x = 0;
		while (*str != '\0')
		{
			stb_fontchar cd = m_Characters[*str - STB_SOMEFONT_FIRST_CHAR];

			//        Position                         Texcoord
			buffer << glm::vec2(x + cd.x0f, -cd.y0f) << glm::vec2(cd.s0f, cd.t0f); // 0
			buffer << glm::vec2(x + cd.x1f, -cd.y0f) << glm::vec2(cd.s1f, cd.t0f); // 1
			buffer << glm::vec2(x + cd.x1f, -cd.y1f) << glm::vec2(cd.s1f, cd.t1f); // 2

			buffer << glm::vec2(x + cd.x0f, -cd.y0f) << glm::vec2(cd.s0f, cd.t0f); // 0
			buffer << glm::vec2(x + cd.x1f, -cd.y1f) << glm::vec2(cd.s1f, cd.t1f); // 2
			buffer << glm::vec2(x + cd.x0f, -cd.y1f) << glm::vec2(cd.s0f, cd.t1f); // 3

			x += cd.advance;
			str++;
		}

        if (buffer.isGenerated())
        {
            buffer.update();
            buffer.describe("a_Position", 2, GL_FLOAT, 4 * sizeof(GLfloat), 0);
            buffer.describe("a_Texcoord", 2, GL_FLOAT, 4 * sizeof(GLfloat), 2 * sizeof(GLfloat));
        }
        else
        {
            buffer.describe("a_Position", 2, GL_FLOAT, 4 * sizeof(GLfloat), 0);
            buffer.describe("a_Texcoord", 2, GL_FLOAT, 4 * sizeof(GLfloat), 2 * sizeof(GLfloat));
            buffer.generate(Buffer::DYNAMIC);
        }
	}

	inline const Texture& texture()
	{
		return *m_Texture;
	}

	inline Shader::Program& shader()
	{
		return *m_Shader;
	}

private:
	Texture* m_Texture;
	Shader::Program* m_Shader;
	std::vector<stb_fontchar> m_Characters;
};

class Text
{
public:
	Text()
	{
		m_Color = glm::vec4(1.0, 1.0, 1.0, 1.0);
		m_Font = NULL;
	}

	virtual ~Text()
	{
		m_Font = NULL;
	}

	void set(const char* str, Font* font)
	{
		m_Font = font;

		m_Text = std::string(str);
		m_Font->build(str, m_Buffer);
	}

	void draw(Context* context, glm::mat4 mvp)
	{
		if (m_Font == NULL)
		{
			std::cout << "Error : Text font is not set !" << std::endl;
			return;
		}

		m_Font->shader().use();
		m_Font->shader().uniform("u_ModelViewProjection").set(mvp);
		m_Font->shader().uniform("u_Font").set(m_Font->texture());
		m_Font->shader().uniform("u_Color").set(m_Color);

		context->geometry().bind(m_Buffer, m_Font->shader());
		context->geometry().drawArrays(GL_TRIANGLES, 0, m_Buffer.size() / (4 * sizeof(GLfloat)));
		context->geometry().unbind(m_Buffer);
	}

	inline void setColor(const glm::vec4& c) { m_Color = c; }

	inline const std::string& getCaption() { return m_Text; }

private:
	Font* m_Font;
	glm::vec4 m_Color;
	std::string m_Text;
	Buffer m_Buffer;
};

