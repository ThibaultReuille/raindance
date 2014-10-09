#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/GUI/Canvas.hh>
#include <raindance/Core/GUI/View.hh>

class Window
{
public:
	Window(const char* title, const int width, const int height)
	{
		m_Title = std::string(title);
		m_Width = width;
		m_Height = height;
		m_FullScreen = false;
		m_ScreenshotFactor = 0;
		m_ScreenshotFilename = std::string("hd-shot.tga");
        m_ClearColor = glm::vec4(0.0, 0.0, 0.0, 0.0);

        glutInitWindowSize(m_Width, m_Height);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        m_GlutID = glutCreateWindow(m_Title.c_str());

        #if defined(_MSC_VER) // TODO : Use Glew for Unix too ?
			GLenum err = glewInit();
			if (GLEW_OK != err)
			{
				/* Problem: glewInit failed, something is seriously wrong. */
				fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
			}
			LOG("[WINDOW] Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
		#endif

        m_TextureVector.add(new Texture("Color 0", m_Width, m_Height, 4));
        m_Canvas = new Canvas(width, height);
	    m_Canvas->bind();

	    m_ActiveView = 0;
	}

	virtual ~Window()
	{
	    SAFE_DELETE(m_Canvas);
	}

	int getGlutID() { return m_GlutID; }

	void fullscreen()
	{
        m_FullScreen = !m_FullScreen;

        if (m_FullScreen)
            glutFullScreen();
        else
        {
            glutReshapeWindow(m_Width, m_Height);
            glutPositionWindow(0, 0);
        }
	}

	void reshape(int width, int height)
	{
        m_Width = width;
        m_Height = height;
        m_Canvas->reshape(width, height);
        m_TextureVector.reshape(width, height);
	}

	inline void clear()
	{
        glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void draw(Context* context)
	{
        if (m_ScreenshotFactor > 0)
        {
            m_Canvas->reshape(m_Width * m_ScreenshotFactor, m_Height * m_ScreenshotFactor);
            m_TextureVector.reshape(m_Width * m_ScreenshotFactor, m_Height * m_ScreenshotFactor);
        }

#ifndef EMSCRIPTEN
        m_Canvas->bind();
        m_Canvas->bindColorTexture(m_TextureVector[0]);
#endif
        {
            clear();
            m_Views[m_ActiveView]->draw();
        }

        if (m_ScreenshotFactor > 0)
        {
            m_Canvas->dump(m_ScreenshotFilename.c_str(), m_TextureVector[0]);
            m_Canvas->reshape(m_Width, m_Height);
            m_TextureVector.reshape(m_Width, m_Height);
            m_ScreenshotFactor = 0;
        }

#ifndef EMSCRIPTEN
        // Final render to screen
        m_Canvas->unbind();
        clear();
        m_Canvas->draw(context, m_TextureVector[0]);
#endif
	}

	inline const std::string& title() { return m_Title; }
    inline int width() { return m_Width; }
    inline int height() { return m_Height; }
    inline Canvas* canvas() { return m_Canvas; }
    inline void screenshot(const std::string& filename, int factor = 1)
    {
        m_ScreenshotFactor = factor;
        m_ScreenshotFilename = filename;
    }
    inline int getScreenshotFactor() { return m_ScreenshotFactor; }

    inline void addView(View* view) { m_Views.push_back(view); }

    inline View* getActiveView(){
        if (m_Views.empty())
            return NULL;
        return m_Views[m_ActiveView];
    }

    inline void nextView()
    {
        if (!m_Views.empty())
            m_ActiveView = (m_ActiveView + 1) % m_Views.size();
    }

private:
    int m_GlutID;
    std::string m_Title;
    int m_Width;
    int m_Height;
    bool m_FullScreen;

    int m_ScreenshotFactor;
    std::string m_ScreenshotFilename;

    Canvas* m_Canvas;
    TextureVector m_TextureVector;
    glm::vec4 m_ClearColor;

    std::vector<View*> m_Views;
    int m_ActiveView;
};

