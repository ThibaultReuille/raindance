#pragma once

#include "Core/Headers.hh"
#include "Core/HUD/Widgets/Widget.hh"
#include "Core/Text.hh"
#include "Core/Transformation.hh"

class ClockWidget : public IWidget
{
public:
    ClockWidget(const char* name, IWidget* parent, glm::vec3 pos, glm::vec2 dimension)
    : IWidget(name, parent, pos, dimension)
    {
        m_Clock = NULL;

        m_Icon = new Icon(glm::vec2(1.0, 1.0), glm::vec2(0.5, -0.5));
        m_Icon->load("clock widget", Resources_Textures_clock_png, sizeof(Resources_Textures_clock_png));

        m_TextPosition = glm::vec3(m_Dimension.x + 5, -4, 0);

        m_Font = new Font();
        m_Text = new Text();
        m_Text->set("Clock", m_Font);

        m_LastValue = 0;
        m_NeedsUpdate = true;
        update();
    }

    virtual ~ClockWidget()
    {
        SAFE_DELETE(m_Icon);
        SAFE_DELETE(m_Text);
        SAFE_DELETE(m_Font);
        m_Clock = NULL;
    }

    virtual void draw(Context* context, glm::mat4 model, glm::mat4 view, glm::mat4 projection)
    {
        glm::mat4 viewProjection =  projection * view;

        Transformation transformation;

        transformation.push();
        {
            transformation.scale(glm::vec3(m_Dimension, 1.0));
            m_Icon->draw(context, viewProjection * model * transformation.state(), glm::vec4(1.0, 1.0, 1.0, 1.0), 0);
        }
        transformation.pop();

        transformation.push();
        {
            transformation.translate(m_TextPosition);
            m_Text->draw(context, viewProjection * model * transformation.state());
        }
        transformation.pop();

        update();
    }

    virtual void onMouseClick(MessageQueue& messages, int x, int y)
    {
        (void) x;
        (void) y;
        (void) messages;
    }

    void update()
    {
        if (m_Clock != NULL)
        {
            Timecode value = m_Clock->milliseconds();
            if (m_NeedsUpdate || m_LastValue != value)
            {
                std::ostringstream s;
                s << value;
                m_Text->set(s.str().c_str(),  m_Font);
                m_NeedsUpdate = false;
                m_LastValue = value;
            }
        }
    }

    inline void bind(Clock* clock) { m_Clock = clock; }

    inline Text& text() { return *m_Text; }

private:
    Text* m_Text;
    Font* m_Font;
    glm::vec3 m_TextPosition;
    Clock* m_Clock;
    Timecode m_LastValue;
    bool m_NeedsUpdate;
};
