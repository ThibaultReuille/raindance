#pragma once

#include "Core/Headers.hh"

class Controller
{
public:
	enum KeyEvent
	{
		KEY_UP,
		KEY_DOWN
	};

	Controller()
	{
		clock.reset();

		m_MouseDelay = 250;
		m_MouseLeftDownCount = 0;
		m_LastMouseLeftDownTime = 0;
	}

	virtual ~Controller()
	{
	}

	virtual void idle() = 0;

	virtual void reshape(int width, int height) = 0;

	virtual void onKeyboard(unsigned char key, Controller::KeyEvent event) = 0;

	virtual void onSpecial(int key, Controller::KeyEvent event) = 0;

	virtual void mouse(int button, int state, int x, int y)
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			m_MousePosition[0] = x;
			m_MousePosition[1] = y;
			this->onMouseDown(x, y);

			auto time = clock.milliseconds();
			m_MouseLeftDownCount = time - m_LastMouseLeftDownTime > m_MouseDelay ? 1 : m_MouseLeftDownCount + 1;
			m_LastMouseLeftDownTime = clock.milliseconds();
		}
		else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		{
			Timecode time = clock.milliseconds();
			if (time - m_LastMouseLeftDownTime < m_MouseDelay)
			{
				if (m_MouseLeftDownCount == 1)
					this->onMouseClick(x, y);
				else if (m_MouseLeftDownCount == 2)
					this->onMouseDoubleClick(x, y);
				else if (m_MouseLeftDownCount == 3)
					this->onMouseTripleClick(x, y);
			}
		}
	}

	virtual void motion(int x, int y)
	{
		this->onMouseMove(x, y, x - m_MousePosition[0], y - m_MousePosition[1]);
	}

	virtual void onMouseDown(int x, int y) = 0;
	virtual void onMouseClick(int x, int y) = 0;
	virtual void onMouseDoubleClick(int x, int y) = 0;
	virtual void onMouseTripleClick(int x, int y) = 0;
	virtual void onMouseMove(int x, int y, int dx, int dy) = 0;

private:
	Clock clock;
	int m_MousePosition[2];
	Timecode m_LastMouseLeftDownTime;
	unsigned int m_MouseLeftDownCount;
	Timecode m_MouseDelay;
};

