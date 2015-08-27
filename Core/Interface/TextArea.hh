#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Transformation.hh>
#include <raindance/Core/Text.hh>
#include <raindance/Core/Font.hh>
#include <raindance/Core/Camera/Camera.hh>

class TextArea : public Document::Node
{
public:
	TextArea(Document::Node* parent = NULL)
	: Document::Node(parent)
	{        
        m_Characters = std::string(
        	"abcdefghijklmnopqrstuvwxyz"
        	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        	"01234567890"
        	"!@#$%%ˆ&*()-_=+{}[]|';:<>,./? "
        	"\\\");");

		m_FontFactor = 0.75;
        m_Font = new rd::Font();
	}

	virtual ~TextArea()
	{
		SAFE_DELETE(m_Font);
	}

    virtual void clear()
    {
		m_Lines.clear();
    }

	virtual void print(const std::string& str)
	{
		m_Lines.push_front(str);
	}

	void draw(Context* context) override
	{
        float ratio = 2.0;

		m_Camera.setOrthographicProjection(0, ratio * this->content().getWidth(), 0, ratio * this->content().getHeight(), 0, 1);
		m_Camera.lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

		Transformation transformation;

		transformation.translate(glm::vec3(0.0, - m_Font->getDescender() * m_Font->getSize(), 0.0));

		for (auto it : m_Lines)
		{
			if (it.size() > 0)
			{
				transformation.push();
					transformation.scale(glm::vec3(m_FontFactor, m_FontFactor, 1.0));
					m_Text.set(it.c_str(), m_Font);
					m_Text.draw(*context, m_Camera.getViewProjectionMatrix() * transformation.state());
				transformation.pop();
			}
			
			transformation.translate(glm::vec3(0.0, m_Font->getSize() * m_FontFactor, 0.0));
		}
	}


private:
	Camera m_Camera;

	float m_FontFactor;
	std::list<std::string> m_Lines;
	std::string m_Characters;
	Text m_Text;
	rd::Font* m_Font;
};
