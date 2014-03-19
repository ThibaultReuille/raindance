#pragma once

#include "Core/Transformation.hh"
#include "Core/Primitives/Quad.hh"
#include "Core/Resources/Texture.hh"
#include "Core/Resources/Shader.hh"

class Wallpaper
{
public:
    Wallpaper()
    {
        m_Shader = ResourceManager::getInstance().loadShader("wallpaper", Resources_Shaders_wallpaper_vert, sizeof(Resources_Shaders_wallpaper_vert),
                                                                          Resources_Shaders_wallpaper_frag, sizeof(Resources_Shaders_wallpaper_frag));
        // m_Shader->dump();
        m_Texture = NULL;

        update();
    }

    virtual ~Wallpaper()
    {
        ResourceManager::getInstance().unload(m_Shader);
    }

    void setTexture(Texture* texture)
    {
        m_Texture = texture;
    }

    void draw(Context* context)
    {
        if (m_Texture == NULL)
            return;

        Transformation transformation;
        transformation.scale(glm::vec3(static_cast<float>(m_WindowWidth), static_cast<float>(m_WindowHeight), 1.0f));

        m_Shader->use();
        m_Shader->uniform("u_ModelViewProjection").set(m_Camera.getViewProjectionMatrix() * transformation.state());
        m_Shader->uniform("u_Texture").set(*m_Texture);

        context->geometry().bind(m_Quad.getVertexBuffer(), *m_Shader);
        context->geometry().drawElements(GL_TRIANGLES, m_Quad.getTriangleBuffer().size(), GL_UNSIGNED_BYTE, m_Quad.getTriangleBuffer().ptr());
        context->geometry().unbind(m_Quad.getVertexBuffer());
    }

    void update()
    {
        m_WindowWidth = glutGet(GLUT_WINDOW_WIDTH);
        m_WindowHeight = glutGet(GLUT_WINDOW_HEIGHT);
        m_Camera.setOrthographicProjection(0.0f, static_cast<float>(m_WindowWidth), 0.0f, static_cast<float>(m_WindowHeight), 0.001f, 100.f);
        m_Camera.lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    }

private:
    Quad m_Quad;
    Shader::Program* m_Shader;
    Texture* m_Texture;
    unsigned int m_WindowWidth;
    unsigned int m_WindowHeight;
    Camera m_Camera;
};
