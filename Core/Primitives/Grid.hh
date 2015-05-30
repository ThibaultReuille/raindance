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
        //glm::vec4 BackgroundColor;
    };

    Grid(const Parameters& parameters)
    {
        m_Parameters = parameters;

        m_Shader = ResourceManager::getInstance().loadShader("Primitives/grid",
                Assets_Shaders_Primitives_grid_vert, sizeof(Assets_Shaders_Primitives_grid_vert),
                Assets_Shaders_Primitives_grid_frag, sizeof(Assets_Shaders_Primitives_grid_frag),
                Assets_Shaders_Primitives_grid_geom, sizeof(Assets_Shaders_Primitives_grid_geom));
        // m_Shader->dump();

        m_VertexBuffer << glm::vec2(0.0, 0.0);
        m_VertexBuffer.describe("a_Zero", 2, GL_FLOAT, sizeof(glm::vec2), 0);
        m_VertexBuffer.generate(Buffer::STATIC);
    }

    virtual ~Grid()
    {
        ResourceManager::getInstance().unload(m_Shader);
    }

    void draw(Context* context, const Camera& camera, Transformation& transformation)
    {
        m_Shader->use();

        m_Shader->uniform("u_ModelViewProjectionMatrix").set(camera.getViewProjectionMatrix() * transformation.state());

        m_Shader->uniform("u_Color").set(m_Parameters.Color);
        //m_Shader->uniform("u_BackgroundColor").set(m_Parameters.BackgroundColor);

        m_Shader->uniform("u_Dimension").set(m_Parameters.Dimension);
        m_Shader->uniform("u_Step").set(m_Parameters.Step);
        m_Shader->uniform("u_Division").set(m_Parameters.Division);
        m_Shader->uniform("u_Shift").set(m_Parameters.Shift);

        context->geometry().bind(m_VertexBuffer, *m_Shader);
        context->geometry().drawArrays(GL_POINTS, 0, m_VertexBuffer.size() / sizeof(glm::vec2));
        context->geometry().unbind(m_VertexBuffer);
    }

    inline Parameters& parameters() { return m_Parameters; }

private:
    Parameters m_Parameters;
    Shader::Program* m_Shader;
    Buffer m_VertexBuffer;
};

