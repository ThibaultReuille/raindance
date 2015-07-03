#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Debug.hh>
#include <raindance/Core/Context.hh>
#include <raindance/Core/GUI/Viewport.hh>

namespace GLFW
{
	class Events
	{
	public:

		// TODO: This function should call onResize with the proper Viewport
		virtual void onWindowSize(int width, int height)
		{ LOG("[GLFW] onWindowSize(%i, %i)\n", width, height); }

		virtual void onWindowRefresh()
		{ LOG("[GLFW] onWindowRefresh()\n"); }

	    virtual void onWindowFocus(int focus)
	    { LOG("[GLFW] onWindowFocus(%i)\n", focus); }

		// TODO: This function should call onResize with the proper Viewport
	    virtual void onSetFramebufferSize(int width, int height)
	    { LOG("[GLFW] onSetFramebufferSize(%i, %i)\n", width, height); }

	    virtual void onCursorPos(double xpos, double ypos)
	    { LOG("[GLFW] onCursorPos(%f, %f)\n", xpos, ypos); }

	    virtual void onCursorEnter(int entered)
	    { LOG("[GLFW] onCursorEnter(%i)\n", entered); }
	    
	    virtual void onMouseButton(int button, int action, int mods)
	    { LOG("[GLFW] onMouseButton(%i, %i, %i)\n", button, action, mods); }
	    
	    virtual void onScroll(double xoffset, double yoffset)
	    { LOG("[GLFW] onScroll(%f, %f)\n", xoffset, yoffset); }
	    
	    virtual void onKey(int key, int scancode, int action, int mods)
	    { LOG("[GLFW] onKey(%i, %i, %i, %i)\n", key, scancode, action, mods); }

	    virtual void onChar(unsigned int codepoint)
	    { LOG("[GLFW] onChar(%u)\n", codepoint); }
    
	    // virtual void onDrop(int count, const char** paths)
	    // { LOG("[GLFW] onDrop(%i, %p)\n", count, paths); }

		virtual void onResize(const Viewport& viewport)
		{
			(void) viewport;
		}
	};

	class Window : public Events
	{
	public:
		enum State
    	{
        	ALIVE,
        	CLOSE
    	};

	    struct Settings
	    {
	    	Settings()
	    	{
	    		Title = std::string("Raindance Window");
	    		Width = 1024;
	    		Height = 728;
	    		Fullscreen = false;
	    		Monitor = NULL;
	    	}

	        std::string Title;
	        int Width;
	        int Height;
	        bool Fullscreen;
	        GLFWmonitor* Monitor;
	    };

		Window(Settings* settings)
		{
			m_Settings = *settings;

			LOG("[GLFW] Creating %ix%i window with title '%s'...\n", m_Settings.Width, m_Settings.Height, m_Settings.Title.c_str());
		    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		    //glfwWindowHint(GLFW_SAMPLES, 4);

		    // TODO: glfwWindowHint(GLFW_STEREO, GL_TRUE);

		    if (m_Settings.Monitor == NULL)
		    	m_Settings.Monitor = glfwGetPrimaryMonitor();

			if (m_Settings.Fullscreen)
			{
				getResolution(m_Settings.Monitor, &m_Settings.Width, &m_Settings.Height);
				m_GlfwHandle = glfwCreateWindow(m_Settings.Width, m_Settings.Height, m_Settings.Title.c_str(), m_Settings.Monitor, NULL);	
			}
			else
				m_GlfwHandle = glfwCreateWindow(m_Settings.Width, m_Settings.Height, m_Settings.Title.c_str(), NULL, NULL);

			glfwMakeContextCurrent(m_GlfwHandle);

			glewExperimental = GL_TRUE;
		    GLenum err = glewInit();
		    if (err != GLEW_OK)   
		    {  
		        LOG("GLEW Error: %s\n", glewGetErrorString(err));  
		        return;  
		    }

		    // NOTE : glewInit raises GL errors, we silently ignore and clear them here.
		    checkGLErrors(true);

			const GLubyte* renderer = glGetString(GL_RENDERER);
			const GLubyte* version = glGetString(GL_VERSION);

		    LOG("\nGLEW Version: %s\n", glewGetString(GLEW_VERSION));
			LOG("GL Renderer: %s\n", renderer);
			LOG("GL version: %s\n\n", version);

	        GLuint vao;
	        glGenVertexArrays(1, &vao);
	        glBindVertexArray(vao);
		}

		virtual ~Window()
		{
			glfwDestroyWindow(m_GlfwHandle);
		}

		virtual State state()
		{
			return glfwWindowShouldClose(m_GlfwHandle) ? CLOSE : ALIVE;
		}

		virtual void before(Context* context)
		{
			(void) context;
			glfwMakeContextCurrent(m_GlfwHandle);
		}

		virtual void after(Context* context)
		{
			(void) context;
	        glfwSwapBuffers(m_GlfwHandle);
	        glfwPollEvents();
		}

		// -------------------------

		virtual void getResolution(GLFWmonitor* monitor, int* width, int* height)
		{
		    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			*width = mode->width;
			*height = mode->height;
		}

		virtual Viewport getViewport()
		{
			int width, height;

			glfwGetWindowSize(m_GlfwHandle, &width, &height);
			Viewport viewport(glm::vec2(0, 0), glm::vec2(width, height));

			glfwGetFramebufferSize(m_GlfwHandle, &width, &height);
			viewport.setFramebuffer(width, height);

			return viewport;
		}

		virtual void close()
		{
			glfwSetWindowShouldClose(m_GlfwHandle, CLOSE);
		}

		inline GLFWwindow* getGlfwHandle() { return m_GlfwHandle; }

		inline const Settings& getSettings() { return m_Settings; }

	private:
		Settings m_Settings;
		GLFWwindow* m_GlfwHandle;
	};

	// ----------------------------------------------------------------------

	std::unordered_map<GLFWwindow*, GLFW::Window*> g_Windows;

	static void create(int argc, char** argv)
	{
		(void) argc;
		(void) argv;

		if (!glfwInit())
		{
	        LOG("[GLFW] Couldn't create context!\n");
	        return;
	    }
	}

	static void destroy()
	{
		glfwTerminate();
	}
	
	static void onWindowSizeCallback(GLFWwindow* window, int width, int height)
	{ g_Windows[window]->onWindowSize(width, height); }
	
	static void onWindowRefreshCallback(GLFWwindow* window)
	{ g_Windows[window]->onWindowRefresh(); }

	static void onWindowFocusCallback(GLFWwindow* window, int focus)
	{ g_Windows[window]->onWindowFocus(focus); }
	
	static void onSetFramebufferSizeCallback(GLFWwindow* window, int width, int height)
    { g_Windows[window]->onSetFramebufferSize(width, height); }

	static void onCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
	{ g_Windows[window]->onCursorPos(xpos, ypos); }
	
	static void onCursorEnterCallback(GLFWwindow* window, int entered)
	{ g_Windows[window]->onCursorEnter(entered); }
	
	static void onMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{ g_Windows[window]->onMouseButton(button, action, mods); }
	
	static void onScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{ g_Windows[window]->onScroll(xoffset, yoffset); }
	
	static void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{ g_Windows[window]->onKey(key, scancode, action, mods); }

	static void onCharCallback(GLFWwindow* window, unsigned int codepoint)
	{ g_Windows[window]->onChar(codepoint); }
	
	// static void onDropCallback(GLFWwindow* window, int count, const char** paths)
	// { g_Windows[window]->onDrop(count, paths); }

	static void setCallbacks(GLFW::Window* window)
	{
		auto w = window;
		auto handle = w->getGlfwHandle();

		g_Windows[handle] = w; 

	    glfwSetWindowSizeCallback(handle, onWindowSizeCallback);
	    glfwSetWindowRefreshCallback(handle, onWindowRefreshCallback);
	    glfwSetWindowFocusCallback(handle, onWindowFocusCallback);

	    glfwSetFramebufferSizeCallback(handle, onSetFramebufferSizeCallback);

	    glfwSetCursorPosCallback(handle, onCursorPosCallback);
	    glfwSetCursorEnterCallback(handle, onCursorEnterCallback);
	    
	    glfwSetMouseButtonCallback(handle, onMouseButtonCallback);
	    
	    glfwSetScrollCallback(handle, onScrollCallback);
	    
	    glfwSetKeyCallback(handle, onKeyCallback);
        glfwSetCharCallback(handle, onCharCallback);
	
	    // TODO : glfwSetDropCallback(handle, onDropCallback);
	}
}

