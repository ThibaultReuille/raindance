#pragma once

#include "Core/HUD/Widgets/Widget.hh"

class TextWidget : public IWidget
{
public:
    TextWidget(const char* name, IWidget* parent, glm::vec3 pos, glm::vec2 dimension)
    : IWidget(name, parent, pos, dimension)
    {
        m_Text = new Text();
        m_Size = 1.0;
    }

    virtual ~TextWidget()
    {
        delete m_Text;
    }

    virtual void draw(Context* context, glm::mat4 model, glm::mat4 view, glm::mat4 projection)
    {
        (void) context;

        // TODO : Find the font value
        // float fontHeight = 18.0f;

        m_Text->draw(context, projection * view * glm::scale(model, glm::vec3(m_Size, m_Size, 1.0)));
    }

    virtual void onMouseClick(MessageQueue& messages, int x, int y)
    {
        (void) messages;
        (void) x;
        (void) y;
    }

    inline Text& text()
    {
        return *m_Text;
    }

    inline void setSize(float size) { m_Size = size; }

private:
    Text* m_Text;
    float m_Size;
};
