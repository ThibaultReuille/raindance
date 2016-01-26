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

		m_Screenshot = false;
		m_ScreenshotFactor = 1.0;
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

	        //if (m_Screenshot)
	        //{
	        //}

	        window->canvas()->bind();

	        window->draw(m_Context);
	        
	        window->canvas()->unbind();
	        window->canvas()->draw(m_Context);

	        if (m_Screenshot)
			{
				window->canvas()->dump(m_ScreenshotFilename.c_str());
				m_Screenshot = false;
			}

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

	void screenshot(const std::string& filename, float factor = 1.0)
	{
		m_Screenshot = true;
		m_ScreenshotFilename = filename;
		m_ScreenshotFactor = factor;
	}

protected:
	Context* m_Context;
	WindowManager m_WindowManager;

	bool m_Screenshot;
	std::string m_ScreenshotFilename;
	float m_ScreenshotFactor;
};

