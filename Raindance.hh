#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Context.hh>
#include <raindance/Core/GLUT.hh>
#include <raindance/Core/GUI/WindowManager.hh>
#include <raindance/Core/GUI/Canvas.hh>

class RainDance : public GLUT::IContext
{
public:
	RainDance()
	{
	    m_WindowManager = new WindowManager();
	}

	virtual ~RainDance()
	{
	    SAFE_DELETE(m_WindowManager);
	}

	virtual void create(int argc, char** argv)
	{
        glutInit(&argc, argv);
        GLUT::setContext(this);
	}

	virtual void initialize()
	{
	}

	virtual void run()
	{
        GLUT::setCallbacks();
		glutMainLoop();
	}

	virtual void postRedisplay()
	{
        for (auto element : m_WindowManager->elements())
        {
            glutSetWindow(element.second->getGlutID());
            glutPostRedisplay();
        }
	}

	virtual void destroy()
    {
    }

	inline Context& context() { return m_Context; }

protected:
	Context m_Context;
	WindowManager* m_WindowManager;
};

