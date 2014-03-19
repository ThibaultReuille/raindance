#pragma once

#include "Core/Headers.hh"
#include "Core/Context.hh"
#include "Core/View.hh"
#include "Core/GLUT.hh"
#include "Core/Window.hh"

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

