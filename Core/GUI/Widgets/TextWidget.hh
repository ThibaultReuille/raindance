#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/GUI/Widgets/Widget.hh>

class TextWidget : public IWidget
{
public:
    TextWidget(const char* name, IWidget* parent, glm::vec3 position, glm::vec2 dimension)
    : IWidget(name, parent, position, dimension)
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
        float fontRatio = m_Size * m_Dimension.y / (m_Text->getFont()->getSize() * m_Text->getFont()->getAscender());

        Transformation transformation;

        transformation.set(model);
        transformation.translate(glm::vec3(0.0, -m_Dimension.y, 0.0));
        transformation.scale(glm::vec3(fontRatio, fontRatio, 1.0));

        m_Text->draw(context, projection * view * transformation.state());
    }

    virtual void onMouseClick(MessageQueue& messages, int x, int y)
    {
        (void) messages;
        (void) x;
        (void) y;
    }

    inline Text& text() { return *m_Text; }
    inline void setSize(float size) { m_Size = size; }

private:
    Text* m_Text;
    float m_Size;
};
