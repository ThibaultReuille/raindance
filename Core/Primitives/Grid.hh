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
        glm::vec2 Origin;
        glm::vec2 Shift;

        glm::vec4 Color;
        glm::vec4 BackgroundColor;
    };

    Grid(const Parameters& parameters)
    {
        m_Parameters = parameters;

        update();

        m_Shader = ResourceManager::getInstance().loadShader("Primitives/grid",
                Assets_Shaders_Primitives_grid_vert, sizeof(Assets_Shaders_Primitives_grid_vert),
                Assets_Shaders_Primitives_grid_frag, sizeof(Assets_Shaders_Primitives_grid_frag));

        m_Shader->dump();
    }

    virtual ~Grid()
    {
        ResourceManager::getInstance().unload(m_Shader);   
    }

    void update()
    {
        m_VertexBuffer.clear();
        m_LineBuffer.clear();
        m_TriangleBuffer.clear();

        //                Position
        m_VertexBuffer << glm::vec2(0.0,                      0.0);
        m_VertexBuffer << glm::vec2(m_Parameters.Dimension.x, 0.0);
        m_VertexBuffer << glm::vec2(m_Parameters.Dimension.x, m_Parameters.Dimension.y);
        m_VertexBuffer << glm::vec2(0.0,                      m_Parameters.Dimension.y);

        unsigned char triangles_indices[] = { 0, 1, 3, 2 };
        m_TriangleBuffer.push(triangles_indices, sizeof(triangles_indices));

        m_FirstUpdate = true;

        if (!m_FirstUpdate)
            m_VertexBuffer.update();

        m_VertexBuffer.describe("a_Position", 2, GL_FLOAT, 2 * sizeof(GLfloat), 0);

        if (m_FirstUpdate)
            m_VertexBuffer.generate(Buffer::STATIC);

        m_FirstUpdate = false;
    }

    void draw(Context& context, Transformation& transformation, const Camera& camera)
    {
        transformation.push();

        transformation.translate(glm::vec3(m_Parameters.Origin, 0.0));

        m_Shader->use();

        // m_Shader->uniform("u_Scale").set(m_Parameters.Scale);
        m_Shader->uniform("u_Step").set(m_Parameters.Step);
        m_Shader->uniform("u_Division").set(m_Parameters.Division);
        m_Shader->uniform("u_Shift").set(m_Parameters.Shift);

        m_Shader->uniform("u_Color").set(m_Parameters.Color);
        m_Shader->uniform("u_BackgroundColor").set(m_Parameters.BackgroundColor);

        m_Shader->uniform("u_ModelViewProjection").set(camera.getViewProjectionMatrix() * transformation.state());

        context.geometry().bind(m_VertexBuffer, *m_Shader);
        context.geometry().drawElements(GL_TRIANGLE_STRIP, m_TriangleBuffer.size() / sizeof(unsigned char), GL_UNSIGNED_BYTE, m_TriangleBuffer.ptr());
        context.geometry().unbind(m_VertexBuffer);

        transformation.pop();
    }

    inline Parameters& parameters() { return m_Parameters; }

private:
    Buffer m_VertexBuffer;
    Buffer m_LineBuffer;
    Buffer m_TriangleBuffer;
    bool m_FirstUpdate;

    Parameters m_Parameters;

    Shader::Program* m_Shader;
};

