#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/GUI/Widgets/Widget.hh>

class SliderWidget : public IWidget
{
public:
    SliderWidget(const char* name, IWidget* parent, glm::vec3 position, glm::vec2 dimension)
    : IWidget(name, parent, position, dimension)
    {
        m_Shader = ResourceManager::getInstance().loadShader("slider", Resources_Shaders_Widgets_slider_vert, sizeof(Resources_Shaders_Widgets_slider_vert),
                                                                       Resources_Shaders_Widgets_slider_frag, sizeof(Resources_Shaders_Widgets_slider_frag));
        // m_Shader->dump();

        value(0.5f);
    }
    virtual ~SliderWidget()
    {
        ResourceManager::getInstance().unload(m_Shader);
    }

    void updateSlider()
    {
        m_VertexBuffer.clear();

        m_VertexBuffer << glm::vec3( 0.0, -1.0, 0);
        m_VertexBuffer << glm::vec3( 0.0,  0.0, 0);
        m_VertexBuffer << glm::vec3( 1.0,  0.0, 0);
        m_VertexBuffer << glm::vec3( 1.0, -1.0, 0);

        m_VertexBuffer << glm::vec3(m_Value, -1.0, 0);
        m_VertexBuffer << glm::vec3(m_Value,  0.0, 0);

        m_VertexBuffer.describe("a_Position", 3, GL_FLOAT, 3 * sizeof(GLfloat), 0);

        m_VertexBuffer.generate(Buffer::DYNAMIC);
    }

    virtual void draw(Context* context, glm::mat4 model, glm::mat4 view, glm::mat4 projection)
    {
        static unsigned short int lines_indices[] = { 0, 1, 2, 3 };
        static unsigned short int triangles_indices[] = { 0, 1, 5, 0, 5, 4 };

        m_Shader->use();
        m_Shader->uniform("u_ModelViewProjection").set(projection * view * glm::scale(model, glm::vec3(m_Dimension, 1.0)));
        context->geometry().bind(m_VertexBuffer, *m_Shader);
        context->geometry().drawElements(GL_LINE_LOOP, sizeof(lines_indices) / sizeof(short int), GL_UNSIGNED_SHORT, lines_indices);
        context->geometry().drawElements(GL_TRIANGLES, sizeof(triangles_indices) / sizeof(short int), GL_UNSIGNED_SHORT, triangles_indices);
        context->geometry().unbind(m_VertexBuffer);
    }
    virtual void onMouseClick(MessageQueue& messages, int x, int y)
    {
        (void) y;

        value(((float) x - this->position().x) / this->dimension().x);
        messages.push(static_cast<IMessage*>(new WidgetMessage(m_Name.c_str(), "update")));
    }

    inline void value(const float value) { m_Value = value; updateSlider(); }
    inline float value() { return m_Value; }

private:
    Buffer m_VertexBuffer;
    Shader::Program* m_Shader;
    float m_Value;
};
