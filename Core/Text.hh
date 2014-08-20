#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Geometry.hh>
#include <raindance/Core/Font.hh>

class Text
{
public:
	Text()
	{
		m_Color = glm::vec4(1.0, 1.0, 1.0, 1.0);
		m_Font = NULL;
		m_TextWidth = 0;
	}

	virtual ~Text()
	{
		m_Font = NULL;
	}

	void set(const char* caption, Font* font)
	{
		m_Font = font;
		m_Text = std::string(caption);
		m_TextWidth = m_Font->build(caption, m_Buffer);
	}

	virtual void draw(Context& context, const glm::mat4& mvp)
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

		context.geometry().bind(m_Buffer, m_Font->shader());
		context.geometry().drawArrays(GL_TRIANGLES, 0, m_Buffer.size() / (4 * sizeof(GLfloat)));
		context.geometry().unbind(m_Buffer);
	}

	inline void draw(Context* context, const glm::mat4& mvp) { draw(*context, mvp); }

	inline void setColor(const glm::vec4& c) { m_Color = c; }

	inline const std::string& getCaption() { return m_Text; }

	inline float getWidth() { return m_TextWidth; }

	inline Font* getFont() { return m_Font; }

private:
	Font* m_Font;
	glm::vec4 m_Color;
	std::string m_Text;
	float m_TextWidth;
	Buffer m_Buffer;
};

