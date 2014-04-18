#pragma once

#include "Core/Headers.hh"
#include "Core/HUD/Widgets/Widget.hh"

class ClockWidget : public IWidget
{
public:
    ClockWidget(const char* name, IWidget* parent, glm::vec3 pos, glm::vec2 dimension)
    : IWidget(name, parent, pos, dimension)
    {
        m_Icon = new Icon(glm::vec2(1.0, 1.0), glm::vec2(0.5, -0.5));
        m_Icon->load("clock widget", Resources_Textures_clock_png, sizeof(Resources_Textures_clock_png));
    }
    virtual ~ClockWidget()
    {
        delete m_Icon;
    }
    virtual void draw(Context* context, glm::mat4 model, glm::mat4 view, glm::mat4 projection)
    {
        m_Icon->draw(context, projection * view * glm::scale(model, glm::vec3(m_Dimension, 1.0)), glm::vec4(1.0, 1.0, 1.0, 1.0), 0);
    }
    virtual void onMouseClick(MessageQueue& messages, int x, int y)
    {
        (void) x;
        (void) y;
        (void) messages;
    }
private:
    std::string m_Text;
};
