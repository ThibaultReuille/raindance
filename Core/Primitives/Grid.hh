#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Transformation.hh>
#include <raindance/Core/Primitives/Quad.hh>

class Grid
{
public:
    struct Parameters
    {
        glm::vec2 Dimension;
        glm::vec2 Scale;
        glm::vec2 Step;
        glm::vec2 Division;
        glm::vec2 Shift;

        glm::vec4 Color;
        glm::vec4 BackgroundColor;
    };

    Grid(const Parameters& parameters)
    {
        m_Parameters = parameters;

        m_Shader = ResourceManager::getInstance().loadShader("Primitives/grid",
                Assets_Shaders_Primitives_grid_vert, sizeof(Assets_Shaders_Primitives_grid_vert),
                Assets_Shaders_Primitives_grid_frag, sizeof(Assets_Shaders_Primitives_grid_frag));

        m_Shader->dump();
        update();
    }

    virtual ~Grid()
    {
        ResourceManager::getInstance().unload(m_Shader);   
    }

    void update()
    {
        m_VertexBuffer.clear();

        //                Position
        m_VertexBuffer << glm::vec2(0.0,                      0.0);
        m_VertexBuffer << glm::vec2(m_Parameters.Dimension.x, 0.0);
        m_VertexBuffer << glm::vec2(0.0,                      m_Parameters.Dimension.y);
        m_VertexBuffer << glm::vec2(m_Parameters.Dimension.x, m_Parameters.Dimension.y);

        m_FirstUpdate = true;

        if (!m_FirstUpdate)
            m_VertexBuffer.update();

        m_VertexBuffer.describe("a_Position", 2, GL_FLOAT, sizeof(glm::vec2), 0);

        if (m_FirstUpdate)
            m_VertexBuffer.generate(Buffer::STATIC);

        m_FirstUpdate = false;
    }

    void draw(Context* context, const Camera& camera, Transformation& transformation)
    {
        m_Shader->use();

        // m_Shader->uniform("u_Scale").set(m_Parameters.Scale);
        m_Shader->uniform("u_Step").set(m_Parameters.Step);
        m_Shader->uniform("u_Division").set(m_Parameters.Division);
        m_Shader->uniform("u_Shift").set(m_Parameters.Shift);

        m_Shader->uniform("u_Color").set(m_Parameters.Color);
        m_Shader->uniform("u_BackgroundColor").set(m_Parameters.BackgroundColor);

        m_Shader->uniform("u_ModelViewProjection").set(camera.getViewProjectionMatrix() * transformation.state());

        context->geometry().bind(m_VertexBuffer, *m_Shader);
        context->geometry().drawArrays(GL_TRIANGLE_STRIP, 0, m_VertexBuffer.size() / sizeof(glm::vec2));
        context->geometry().unbind(m_VertexBuffer);
    }

    inline Parameters& parameters() { return m_Parameters; }

private:
    Buffer m_VertexBuffer;
    bool m_FirstUpdate;

    Parameters m_Parameters;

    Shader::Program* m_Shader;
};

