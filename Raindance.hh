#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Context.hh>
#include <raindance/Core/GLUT.hh>
#include <raindance/Core/GUI/Window.hh>

class RainDance : public GLUT::IContext
{
public:
	RainDance()
    : m_Window(NULL)
	{
	}

	virtual ~RainDance()
	{
	    SAFE_DELETE(m_Window);
	}

	virtual void create(int argc, char** argv)
	{
        glutInit(&argc, argv);
        GLUT::setContext(this);
	}

	virtual void initialize()
	{
	}

	void addWindow(const char* title, unsigned int width, unsigned int height)
	{
	    // TODO : Multi-window system
	    SAFE_DELETE(m_Window);
        m_Window = new Window(title, width, height);
	}

	virtual void run()
	{
        GLUT::setCallbacks();
		glutMainLoop();
	}

	virtual void destroy()
    {
    }

	inline Context& context() { return m_Context; }

protected:
	Context m_Context;
	Window* m_Window;
};

