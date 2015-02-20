#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/GUI/Widgets/Widget.hh>

class CheckBoxWidget : public IWidget
{
public:
    CheckBoxWidget(const char* name, IWidget* parent, glm::vec3 position, glm::vec2 dimension)
    : IWidget(name, parent, position, dimension)
    {
        m_Shader = ResourceManager::getInstance().loadShader("checkbox", Assets_Shaders_Widgets_checkbox_vert, sizeof(Assets_Shaders_Widgets_checkbox_vert),
                                                                         Assets_Shaders_Widgets_checkbox_frag, sizeof(Assets_Shaders_Widgets_checkbox_frag));
        // m_Shader->dump();

        m_Icon = new Icon();
        m_Icon->load("mark", Assets_Textures_mark_png, sizeof(Assets_Textures_mark_png));

        value(false);

    }
    virtual ~CheckBoxWidget()
    {
        ResourceManager::getInstance().unload(m_Shader);
        delete m_Icon;
    }

    void updateWidget()
    {
        m_VertexBuffer.clear();

        m_VertexBuffer << glm::vec3(0.0,  0.0, 0);
        m_VertexBuffer << glm::vec3(1.0,  0.0, 0);
        m_VertexBuffer << glm::vec3(0.0, -1.0, 0);
        m_VertexBuffer << glm::vec3(1.0, -1.0, 0);

       m_VertexBuffer.describe("a_Position", 3, GL_FLOAT, 3 * sizeof(GLfloat), 0);

        m_VertexBuffer.generate(Buffer::DYNAMIC);
    }

    virtual void draw(Context* context, glm::mat4 model, glm::mat4 view, glm::mat4 projection)
    {
        m_Shader->use();
        m_Shader->uniform("u_ModelViewProjection").set(projection * view * glm::scale(model, glm::vec3(m_Dimension, 1.0)));
        context->geometry().bind(m_VertexBuffer, *m_Shader);
        context->geometry().drawArrays(GL_TRIANGLE_STRIP, 0, m_VertexBuffer.size() / sizeof(glm::vec3));
        context->geometry().unbind(m_VertexBuffer);

        if (m_Value)
        {
            glm::mat4 m;
            m = glm::translate(model, glm::vec3(m_Dimension.x / 2, -m_Dimension.y / 2, 0));
            m = glm::scale(m, glm::vec3(1.5f * m_Dimension, 1.0));

            m_Icon->draw(context, projection * view * m, glm::vec4(1.0, 1.0, 1.0, 1.0), 0);
        }
    }
    
    void onMouseClick(MessageQueue& messages, const glm::vec2& pos) override
    {
        (void) pos;

        value(!m_Value);
        messages.push(static_cast<IMessage*>(new WidgetMessage(m_Name.c_str(), "update")));
    }

    inline void value(bool value) { m_Value = value; updateWidget(); }
    inline bool value() { return m_Value; }

private:
    Buffer m_VertexBuffer;
    Shader::Program* m_Shader;
    Icon* m_Icon;
    bool m_Value;
};
