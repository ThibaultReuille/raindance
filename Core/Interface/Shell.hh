#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Transformation.hh>
#include <raindance/Core/Text.hh>
#include <raindance/Core/Font.hh>
#include <raindance/Core/Camera/Camera.hh>
#include <raindance/Core/Interface/TextArea.hh>

class Shell : public Document::Node
{
public:
	Shell(Document::Node* parent = NULL)
	: Document::Node(parent)
	{        
        m_Cursor = 0;
        m_History.push_back(std::string());
        m_Current = m_History.begin();
        m_Scroll = m_History.begin();
        m_Characters = std::string(
        	"abcdefghijklmnopqrstuvwxyz"
        	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        	"01234567890"
        	"!@#$%%ˆ&*()-_=+{}[]|';:<>,./? "
        	"\\\");");
        m_Prompt = "$ ";

		m_HistoryFontFactor = 0.75;
        m_Font = new rd::Font();
        m_Text.set(m_Prompt.c_str(), m_Font);

        m_Output = NULL;
	}

	virtual ~Shell()
	{
		SAFE_DELETE(m_Font);
	}

	void attach(TextArea* output)
	{
		m_Output = output;
	}

    void onKey(int key, int scancode, int action, int mods) override
    {
    	(void) scancode;
    	(void) mods;

        if (action != GLFW_PRESS)
            return;

        switch(key)
        {
            case GLFW_KEY_ENTER:
            	execute(*m_History.begin());
				if (m_History.begin()->size() > 0)
					m_History.insert(m_History.begin(), std::string(""));
				else
					m_History.begin()->clear();
				m_Current = m_History.begin();
				m_Scroll = m_History.begin();
				m_Cursor = 0;
                break;

            case GLFW_KEY_BACKSPACE:
                if (m_Cursor > 0)
               	{
               		m_Cursor--;
               		m_Current->erase(m_Cursor, 1);
               	}
            	break;

            case GLFW_KEY_LEFT:
            	if (m_Cursor > 0)
            		m_Cursor--;
            	break;

            case GLFW_KEY_RIGHT:
            	if (m_Cursor < m_Current->size())
            		m_Cursor++;
            	break;

            case GLFW_KEY_UP:
            	if (std::next(m_Scroll, 1) != m_History.end())
            	{
            		++m_Scroll;
            		clear();
            		write(*m_Scroll);
            	}
            	break;

            case GLFW_KEY_DOWN:
            	if (m_Scroll != m_History.begin())
            	{
            		--m_Scroll;
            		clear();
            		write(*m_Scroll);
            	}
            	break;

            default:
                break;
        }
    }

    void onChar(unsigned int codepoint) override
    {   
        std::string command;
        char c = (char) codepoint;
		command.append(&c, sizeof(char));
		
		if (command.find_first_not_of(m_Characters) != std::string::npos)
			return;
        
		write(command);
    }

    virtual void clear()
    {
		*m_Current = "";
		m_Cursor = 0;
    }

	virtual void write(const std::string& str)
	{
		m_Current->insert(m_Cursor, str);
		m_Cursor += str.size();
	}

	virtual void print(const std::string& str)
	{
		if (m_Output != NULL)
			m_Output->print(str);
	}

	virtual void execute(const std::string& command)
	{
		print(command);
	}

	virtual void accept(IVisitor& visitor) override
	{
	    visitor.visit()
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

		// Command Prompt
		m_Text.set("$", m_Font);
		m_Text.draw(*context, m_Camera.getViewProjectionMatrix() * transformation.state());
		
		// Command Line
		if (m_History.begin()->size() > 0)
		{
			transformation.translate(glm::vec3(m_Font->getSize(), 0, 0));
			m_Text.set(m_History.begin()->c_str(), m_Font);
			m_Text.draw(*context, m_Camera.getViewProjectionMatrix() * transformation.state());
		}
	}

	inline const std::list<std::string>& getHistory() { return m_History; }

private:
	Camera m_Camera;
	rd::Font* m_Font;
	Text m_Text;
	std::string m_Characters;

	float m_HistoryFontFactor;
	std::string m_Prompt;
	unsigned int m_Cursor;
	std::list<std::string>::iterator m_Current;
	std::list<std::string>::iterator m_Scroll;
	std::list<std::string> m_History;

	TextArea* m_Output;
};
