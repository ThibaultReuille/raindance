#pragma once
#include <raindance/Core/Headers.hh>
#include <raindance/Core/GUI/Widgets/Widget.hh>


class CheckBox : public Document::Node
{
public:
    CheckBox(Document::Node* parent = NULL)
    : Document::Node(parent)
    {
        m_Shader = ResourceManager::getInstance()
             .loadShader("checkbox", Assets_Shaders_Widgets_checkbox_vert, sizeof(Assets_Shaders_Widgets_checkbox_vert),
                                    Assets_Shaders_Widgets_checkbox_frag, sizeof(Assets_Shaders_Widgets_checkbox_frag));

        m_Shader->dump();

        m_Icon = new Icon();
        m_Icon->load("mark", Assets_Textures_mark_png, sizeof(Assets_Textures_mark_png));
        value(false);
    }

    virtual ~CheckBox()
    {
        ResourceManager::getInstance().unload(m_Shader);
        delete m_Icon;
    }

    void accept(IVisitor* visitor) override
    {
        visitor->visit(this);
    }

    void draw(Context* context) override
    {
        m_Camera.setOrthographicProjection(0, this->content().getWidth(), 0, this->content().getHeight(), 0, 1);
        m_Camera.lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

        glm::mat4 model = glm::mat4();
        glm::mat4 view = m_Camera.getViewMatrix();
        glm::mat4 projection = m_Camera.getProjectionMatrix();
        glm::vec2 dimension = glm::vec2(this->content().getWidth(), this->content().getHeight());

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

        m_Shader->use();
        m_Shader->uniform("u_ModelViewProjection").set(projection * view * glm::scale(model, glm::vec3(dimension, 1.0)));
        context->geometry().bind(m_VertexBuffer, *m_Shader);
        context->geometry().drawArrays(GL_TRIANGLE_STRIP, 0, m_VertexBuffer.size() / sizeof(glm::vec3));
        context->geometry().unbind(m_VertexBuffer);

        if (m_Value)
        {
            glm::mat4 m;
            m = glm::translate(model, glm::vec3(dimension.x / 2, dimension.y / 2, 0));
            m = glm::scale(m, glm::vec3(1.2f * dimension, 1.0));
            m_Icon->draw(context, projection * view * m, glm::vec4(1.0, 1.0, 1.0, 1.0), 0);
        }

    }

/*
   void onMouseClick(MessageQueue& messages, const glm::vec2& pos) override
   {
        (void) pos;
        value(!m_Value);
        //messages.push(static_cast<IMessage*>(new WidgetMessage(m_Name.c_str(), "update")));
    }
*/

    void onMouseClick(const glm::vec2& pos) override
    {
        PickRegion region;
        pick(pos,&region);

        if (region == OUTSIDE || region == PADDING)
            return;

        value(!m_Value);
        update();
    }

    inline void value(bool value) { m_Value = value; }
    inline bool value() { return m_Value; }

private:
    Camera m_Camera;
    Buffer m_VertexBuffer;
    Shader::Program* m_Shader;
    Icon* m_Icon;
    bool m_Value;
};
