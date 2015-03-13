#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/GUI/Widgets/Widget.hh>

class SliderWidget : public IWidget
{
public:
    SliderWidget(const char* name, IWidget* parent, glm::vec3 position, glm::vec2 dimension)
    : IWidget(name, parent, position, dimension)
    {
        m_Shader = ResourceManager::getInstance().loadShader("slider", Assets_Shaders_Widgets_slider_vert, sizeof(Assets_Shaders_Widgets_slider_vert),
                                                                       Assets_Shaders_Widgets_slider_frag, sizeof(Assets_Shaders_Widgets_slider_frag),
                                                                       Assets_Shaders_Widgets_slider_geom, sizeof(Assets_Shaders_Widgets_slider_geom));
        //m_Shader->dump();

        m_Color = glm::vec4(0.5, 0.5, 0.5, 1.0);

        m_VertexBuffer << glm::vec2(0.0,  0.0);
        m_VertexBuffer.describe("a_Position", 2, GL_FLOAT, 2 * sizeof(GLfloat), 0);
        m_VertexBuffer.generate(Buffer::STATIC);

        m_Precision = 0.1;
        setValue(0.5f);
    }
    virtual ~SliderWidget()
    {
        ResourceManager::getInstance().unload(m_Shader);
    }

    virtual void draw(Context* context, glm::mat4 model, glm::mat4 view, glm::mat4 projection)
    {
        m_Shader->use();
        m_Shader->uniform("u_ProjectionMatrix").set(projection);
        m_Shader->uniform("u_ModelViewMatrix").set(view * glm::scale(model, glm::vec3(m_Dimension, 1.0)));
        m_Shader->uniform("u_Color").set(m_Color);
        m_Shader->uniform("u_Value").set(getValue());
        context->geometry().bind(m_VertexBuffer, *m_Shader);
        context->geometry().drawArrays(GL_POINTS, 0, m_VertexBuffer.size() / sizeof(glm::vec2));
        context->geometry().unbind(m_VertexBuffer);
    }
    
    void onMouseClick(MessageQueue& messages, const glm::vec2& pos) override
    {
        setValue((pos.x - this->position().x) / this->dimension().x);
        messages.push(static_cast<IMessage*>(new WidgetMessage(m_Name.c_str(), "update")));
    }

    inline float roundWithPrecision(float value, float precision)
    {
        return floor(value * (1.0f / precision) + 0.5) / (1.0f / precision);
    }

    inline void setValue(float value) { m_Value = value; }
    inline float getValue() { return roundWithPrecision(m_Value, m_Precision); }

    inline void setPrecision(float precision) { m_Precision = precision; }
    inline float getPrecision() { return m_Precision; }

private:
    Buffer m_VertexBuffer;
    Shader::Program* m_Shader;
    glm::vec4 m_Color;
    float m_Value;
    float m_Precision;
};
