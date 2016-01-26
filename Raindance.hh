#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Context.hh>
#include <raindance/Core/Interface/WindowManager.hh>

class Raindance
{
public:
	Raindance(int argc, char** argv)
	{
		GLFW::create(argc, argv);

		m_Context = new Context();
	}

	virtual ~Raindance()
	{
		GLFW::destroy();
	}

	virtual void add(rd::Window* window)
	{
		GLFW::setCallbacks(window);
		auto id = m_WindowManager.add(window);
		m_WindowManager.bind(id);
	}

    virtual void run()
    {
    	m_WindowManager.active()->initialize(m_Context);

    	while (m_WindowManager.active()->state() == rd::Window::ALIVE)
    	{
    		Geometry::beginFrame();

    		auto window = m_WindowManager.active();

	        window->before(m_Context);

	        window->canvas()->bind();

	        window->draw(m_Context);
	        
	        window->canvas()->unbind();
	        window->canvas()->draw(m_Context);

	        /*
	        static int count = 0;
	        if (count == 100)
	        	window->canvas()->dump("test.tga");
	   		count++;
			*/
			
	        window->after(m_Context);

   		 	Geometry::endFrame();

	    	idle();

    		checkGLErrors();
    	}
    }

    virtual void stop()
    {
    	windows().active()->close();
    }

	virtual void idle()
	{
		windows().active()->idle(m_Context);
	}

	inline WindowManager& windows() { return m_WindowManager; }

protected:
	Context* m_Context;
	WindowManager m_WindowManager;
};

