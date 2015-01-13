#pragma once

#include <raindance/Core/Headers.hh>

class Mouse
{
public:

	Mouse()
	{
		m_ClickDelay = 250;
		m_Clicks = 0;
		m_LastClickTime = 0;
	
		m_Clock.reset();
	}

	virtual ~Mouse()
	{
	}

	virtual void onCursorPos(double xpos, double ypos)
	{
		m_LastPosition = m_Position;
		m_Position = glm::vec2(xpos, ypos);
		m_DeltaPosition = m_Position - m_LastPosition;
	}

	virtual void onMouseButton(int button, int action, int mods)
	{
		m_Button = button;
		m_Action = action;
		m_Mods = mods;

		if (m_Button == 0 /* LEFT */)
		{
			if (m_Action == 1 /* DOWN */)
			{
				auto time = m_Clock.milliseconds();
				m_Clicks = time - m_LastClickTime > m_ClickDelay ? 1 : m_Clicks + 1;
				m_LastClickTime = time;
			}
		}
	}

	inline const glm::vec2& position() const { return m_Position; }

	inline const glm::vec2& delta() const { return m_DeltaPosition; }

	inline int button() const { return m_Button; }
	inline int action() const { return m_Action; }
	inline int mods() const { return m_Action; }

	inline int clicks() const { return m_Clicks; }

	bool released()
	{
		Timecode time = m_Clock.milliseconds();
		return time - m_LastClickTime < m_ClickDelay;
	}

private:
	Clock m_Clock;
	glm::vec2 m_LastPosition;
	glm::vec2 m_Position;
	glm::vec2 m_DeltaPosition;

	int m_Button;
	int m_Action;
	int m_Mods;

	Timecode m_LastClickTime;
	int m_Clicks;
	Timecode m_ClickDelay;
};