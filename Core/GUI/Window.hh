#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Context.hh>
#include <raindance/Core/GLFW.hh>

class Window : public GLFW::Window
{
public:

    Window(const char* title, int width, int height, bool fullscreen = false)
    : GLFW::Window(title, width, height, fullscreen)
    {
        m_ClearColor = glm::vec4(0.0, 0.0, 0.0, 0.0);
    }

    virtual ~Window() {}

    virtual void initialize(Context* context)
    {
        m_Context = context;

        auto viewport = getViewport();
        this->onSetFramebufferSize(viewport.getFramebuffer().Width, viewport.getFramebuffer().Height);
        this->onWindowSize((int)viewport.getDimension()[0], (int)viewport.getDimension()[1]);

        glViewport(0, 0, viewport.getFramebuffer().Width, viewport.getFramebuffer().Height);
    }

    virtual void clear()
    {
        glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    virtual void draw(Context* context) = 0;

    virtual void idle(Context* context) = 0;

protected:
    Context* m_Context;

    glm::vec4 m_ClearColor;
};

/* TODO : Reimplement Canvas + HD screenshots

class Window : public IWindow
{
public:
	Window(const char* title, const int width, const int height)
    : IWindow(title, width, height)
	{
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
                // Problem: glewInit failed, something is seriously wrong.
                fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
            }
            LOG("[WINDOW] Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
        #endif

        m_TextureVector.add(new Texture("Color 0", m_Width, m_Height, 4));
        m_Canvas = new Canvas(width, height);
        m_Canvas->bind();
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

    void preDraw(Context* context)
    {
        (void) context;

        if (m_ScreenshotFactor > 0)
        {
            m_Canvas->reshape(m_Width * m_ScreenshotFactor, m_Height * m_ScreenshotFactor);
            m_TextureVector.reshape(m_Width * m_ScreenshotFactor, m_Height * m_ScreenshotFactor);
        }

#ifndef EMSCRIPTEN
        m_Canvas->bind();
        m_Canvas->bindColorTexture(m_TextureVector[0]);
#endif
    }

    virtual void draw(Context* context) = 0;

    void postDraw(Context* context)
    {
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

    inline Canvas* canvas() { return m_Canvas; }
    inline void screenshot(const std::string& filename, int factor = 1)
    {
        m_ScreenshotFactor = factor;
        m_ScreenshotFilename = filename;
    }
    inline int getScreenshotFactor() { return m_ScreenshotFactor; }

protected:


private:
    int m_GlutID;

    int m_ScreenshotFactor;
    std::string m_ScreenshotFilename;

    Canvas* m_Canvas;
    TextureVector m_TextureVector;
    glm::vec4 m_ClearColor;
};
*/

