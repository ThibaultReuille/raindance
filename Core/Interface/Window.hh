#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Context.hh>
#include <raindance/Core/GLFW.hh>
#include <raindance/Core/Interface/DocumentGroup.hh>
#include <raindance/Core/Interface/Canvas.hh>

namespace rd
{

class Window : public GLFW::Window
{
public:

    struct Settings : GLFW::Window::Settings
    {
        // TODO: Add rd::Window specific members here
    };

    Window(Settings* settings)
    : GLFW::Window(settings)
    {
        auto framebuffer = getViewport().getFramebuffer();

        m_Canvas = new Canvas(framebuffer.Width, framebuffer.Height);
    }

    virtual ~Window()
    {
        SAFE_DELETE(m_Canvas);
    }

    virtual void initialize(Context* context)
    {
        m_Context = context;

        auto viewport = getViewport();
        auto framebuffer = viewport.getFramebuffer();
        auto dimension = viewport.getDimension();

        this->onSetFramebufferSize(framebuffer.Width, framebuffer.Height);
        this->onWindowSize((int)dimension.x, (int)dimension.y);
    }

    void onWindowSize(int width, int height) override
    {
        m_Viewport.setDimension(glm::vec2((float) width, (float) height));
        onResize(m_Viewport);
    }

    void onSetFramebufferSize(int width, int height) override
    {
        m_Viewport.setFramebuffer(width, height);
        onResize(m_Viewport);
    }

    virtual void onResize(const Viewport& viewport)
    {
        auto framebuffer = viewport.getFramebuffer();

        m_Canvas->resize(framebuffer.Width, framebuffer.Height);

        m_Body.style().Position = Document::Style::ABSOLUTE;
        m_Body.style().Left = Document::Length(Document::Length::PIXELS, 0.0);
        m_Body.style().Top = Document::Length(Document::Length::PIXELS, 0.0);
        m_Body.style().Width = Document::Length(Document::Length::PIXELS, framebuffer.Width);
        m_Body.style().Height = Document::Length(Document::Length::PIXELS, framebuffer.Height);
        m_Body.update();

        m_Body.onResize(viewport);
    }

    virtual void onScreenshot(bool enter)
    {
        m_Body.onScreenshot(enter);
    }

    void onCursorPos(double xpos, double ypos) override
    {
        // NOTE: Convert window coordinates to viewport coordinates
        
        auto framebuffer = m_Viewport.getFramebuffer();

        auto ratio = glm::vec2(
            framebuffer.Width / m_Viewport.getDimension().x,
            framebuffer.Height / m_Viewport.getDimension().y
        );

        m_Body.onCursorPos(xpos * ratio.x, framebuffer.Height - ypos * ratio.y);
    }

    void onMouseButton(int button, int action, int mods) override
    {
        m_Body.onMouseButton(button, action, mods);
    }

    virtual void focus(Document::Node* node)
    {
    	m_Body.setActiveElement(node);
    }

    virtual void draw(Context* context) = 0;

    virtual void idle(Context* context) = 0;

    inline Document::Group& body() { return m_Body; }

    inline const Viewport& viewport() { return m_Viewport; }

    inline Canvas* canvas() { return m_Canvas; }

protected:
    Context* m_Context;
    Viewport m_Viewport;
    Document::Group m_Body;

    Canvas* m_Canvas;
};

}
