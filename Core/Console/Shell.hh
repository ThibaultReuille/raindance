#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Transformation.hh>
#include <raindance/Core/Text.hh>
#include <raindance/Core/Font.hh>

class Shell
{
public:
	Shell()
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
        m_Font = new Font();
        m_Text.set(m_Prompt.c_str(), m_Font);
	}

	virtual ~Shell()
	{
		SAFE_DELETE(m_Font);
	}

    void onKey(int key, int scancode, int action, int mods) // TODO: override
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

    void onChar(unsigned int codepoint) // TODO: override
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
		m_Lines.push_front(str);
	}

	virtual void execute(const std::string& command)
	{
		print(command);
	}

	virtual void draw(Context* context, const Camera& camera, Transformation& transformation)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

		transformation.push();

			transformation.scale(glm::vec3(0.4, 0.4, 0.4));
			transformation.translate(glm::vec3(0.0, - m_Font->getDescender() * m_Font->getSize(), 0.0));

			// Command Prompt
			m_Text.set("$", m_Font);
			m_Text.draw(*context, camera.getProjectionMatrix() * camera.getViewMatrix() * transformation.state());
			
			// Command Line
			if (m_History.begin()->size() > 0)
			{
				transformation.push();
					transformation.translate(glm::vec3(m_Font->getSize(), 0, 0));
					m_Text.set(m_History.begin()->c_str(), m_Font);
					m_Text.draw(*context, camera.getProjectionMatrix() * camera.getViewMatrix() * transformation.state());
				transformation.pop();
			}
			transformation.translate(glm::vec3(0.0, m_Font->getSize(), 0.0));

			// Log
			for (auto it : m_Lines)
			{
				if (it.size() > 0)
				{
					transformation.push();
						transformation.scale(glm::vec3(m_HistoryFontFactor, m_HistoryFontFactor, 1.0));
						m_Text.set(it.c_str(), m_Font);
						m_Text.draw(*context, camera.getProjectionMatrix() * camera.getViewMatrix() * transformation.state());
					transformation.pop();
				}
				
				transformation.translate(glm::vec3(0.0, m_Font->getSize() * m_HistoryFontFactor, 0.0));
			}

		transformation.pop();
	}

	inline const std::list<std::string>& getHistory() { return m_History; }

private:
	unsigned int m_Cursor;
	std::list<std::string>::iterator m_Current;
	std::list<std::string>::iterator m_Scroll;
	std::list<std::string> m_History;
	float m_HistoryFontFactor;
	std::list<std::string> m_Lines;
	std::string m_Characters;
	std::string m_Prompt;
	Text m_Text;
	Font* m_Font;
};