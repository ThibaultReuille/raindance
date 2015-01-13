#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/GUI/Widgets/Widget.hh>

class ScriptWidget : public IWidget
{
public:
    ScriptWidget(const char* name, IWidget* parent, glm::vec3 pos, glm::vec2 dimension, std::string command)
    : IWidget(name, parent, pos, dimension)
    {
        m_Icon = new Icon(glm::vec2(1.0, 1.0), glm::vec2(0.5, -0.5));
        m_Icon->load("script", Assets_Textures_script_png, sizeof(Assets_Textures_script_png));
        m_Command = command;
    }
    virtual ~ScriptWidget()
    {
        delete m_Icon;
    }
    virtual void draw(Context* context, glm::mat4 model, glm::mat4 view, glm::mat4 projection)
    {
        m_Icon->draw(context, projection * view * glm::scale(model, glm::vec3(m_Dimension, 1.0)), glm::vec4(1.0, 1.0, 1.0, 1.0), 0);
    }
    
    void onMouseClick(MessageQueue& messages, const glm::vec2& pos) override
    {
        (void) pos;
        messages.push(new ScriptMessage(m_Command.c_str()));
    }
private:
    std::string m_Command;
};
