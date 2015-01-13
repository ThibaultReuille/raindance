#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/GUI/Widgets/Widget.hh>
#include <raindance/Core/Text.hh>
#include <raindance/Core/Transformation.hh>

class ClockWidget : public IWidget
{
public:
    ClockWidget(const char* name, IWidget* parent, glm::vec3 pos, glm::vec2 dimension)
    : IWidget(name, parent, pos, dimension)
    {
        m_Clock = NULL;

        m_Icon = new Icon(glm::vec2(1.0, 1.0), glm::vec2(0.5, -0.5));
        m_Icon->load("clock widget", Assets_Textures_clock_png, sizeof(Assets_Textures_clock_png));

        m_TextPosition = glm::vec3(1.5 * m_Dimension.x, 3.0, 0);

        m_Font = new Font();
        m_TextWidget = new TextWidget("clock text widget", NULL , pos, dimension);
        m_TextWidget->text().set("Clock", m_Font);

        m_LastValue = 0;
        m_NeedsUpdate = true;
        update();
    }

    virtual ~ClockWidget()
    {
        SAFE_DELETE(m_Icon);
        SAFE_DELETE(m_TextWidget);
        SAFE_DELETE(m_Font);
        m_Clock = NULL;
    }

    virtual void draw(Context* context, glm::mat4 model, glm::mat4 view, glm::mat4 projection)
    {
        Transformation transformation;

        transformation.push();
        {
            transformation.scale(glm::vec3(m_Dimension, 1.0));
            m_Icon->draw(context, projection * view * model * transformation.state(), glm::vec4(1.0, 1.0, 1.0, 1.0), 0);
        }
        transformation.pop();

        transformation.push();
        {
            transformation.translate(m_TextPosition);
            m_TextWidget->draw(context, model * transformation.state(), view, projection);
        }
        transformation.pop();

        update();
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
                m_TextWidget->text().set(s.str().c_str(), m_Font);
                m_NeedsUpdate = false;
                m_LastValue = value;
            }
        }
    }

    inline void bind(Clock* clock) { m_Clock = clock; }

    inline Text& text() { return m_TextWidget->text(); }

private:
    TextWidget* m_TextWidget;
    glm::vec3 m_TextPosition;
    Font* m_Font;
    Clock* m_Clock;
    Timecode m_LastValue;
    bool m_NeedsUpdate;
};
