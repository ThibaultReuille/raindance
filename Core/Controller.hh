#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/GLFW.hh>
#include <raindance/Core/Mouse.hh>

class Controller : public GLFW::Events
{
public:

	enum KeyEvent
	{
		KEY_DOWN = 0,
		KEY_UP = 1
	};

	Controller()
	{
	}

	virtual ~Controller()
	{
	}

	virtual void idle()
	{
	}

	// ---- Mouse Events ----

	void onCursorPos(double xpos, double ypos) override
	{
		m_Mouse.onCursorPos(xpos, ypos);

		if (m_Mouse.button() == 0 /* LEFT */ && m_Mouse.action() == 1 /* DOWN */)
			this->onMouseMove(m_Mouse.position(), m_Mouse.delta());
	}

    void onMouseButton(int button, int action, int mods) override
    {
    	m_Mouse.onMouseButton(button, action, mods);

    	if (m_Mouse.button() == 0 /* LEFT */)
    	{
    		if (m_Mouse.action() == 1 /* DOWN */)
    			this->onMouseDown(m_Mouse.position());
    		else if (m_Mouse.action() == 0 /* UP */ && m_Mouse.released())
			{
				if (m_Mouse.clicks() == 1)
					this->onMouseClick(m_Mouse.position());
				else if (m_Mouse.clicks() == 2)
					this->onMouseDoubleClick(m_Mouse.position());
				else if (m_Mouse.clicks() == 3)
					this->onMouseTripleClick(m_Mouse.position());
			}
    	}
    }

	virtual void onMouseDown(const glm::vec2& pos)
	{ LOG("[CONTROLLER] onMouseDown(%f, %f)\n", pos.x, pos.y); }

	virtual void onMouseClick(const glm::vec2& pos)
	{ LOG("[CONTROLLER] onMouseClick(%f, %f)\n", pos.x, pos.y); }

	virtual void onMouseDoubleClick(const glm::vec2& pos)
	{ LOG("[CONTROLLER] onMouseDoubleClick(%f, %f)\n", pos.x, pos.y); };

	virtual void onMouseTripleClick(const glm::vec2& pos)
	{ LOG("[CONTROLLER] onMouseTripleClick(%f, %f)\n", pos.x, pos.y); }

	virtual void onMouseMove(const glm::vec2& pos, const glm::vec2& dpos)
	{ LOG("[CONTROLLER] onMouseMove((%f, %f), (%f, %f))\n", pos.x, pos.y, dpos.x, dpos.y); }

protected:
	Mouse m_Mouse;
};

