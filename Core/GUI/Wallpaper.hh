#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Transformation.hh>
#include <raindance/Core/Primitives/Quad.hh>
#include <raindance/Core/Resources/Texture.hh>
#include <raindance/Core/Resources/Shader.hh>

class Wallpaper
{
public:
    Wallpaper()
    {
        m_Shader = ResourceManager::getInstance().loadShader("wallpaper", Assets_Shaders_wallpaper_vert, sizeof(Assets_Shaders_wallpaper_vert),
                                                                          Assets_Shaders_wallpaper_frag, sizeof(Assets_Shaders_wallpaper_frag));
        // m_Shader->dump();
        m_Texture = NULL;
        m_Quad.getVertexBuffer().mute("a_Normal", true);
    }

    virtual ~Wallpaper()
    {
        ResourceManager::getInstance().unload(m_Shader);
    }

    void resize(int width, int height)
    {
        LOG("[WALLPAPER] resize(%i, %i)\n", width, height);

        m_Dimension = glm::vec2(width, height);
        m_Camera.setOrthographicProjection(0.0f, m_Dimension[0], 0.0f, m_Dimension[1], 0.001f, 100.f);
        m_Camera.lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    }

    void draw(Context* context)
    {
        if (m_Texture == NULL)
            return;

        Transformation transformation;
        transformation.scale(glm::vec3(m_Dimension, 1.0f));

        m_Shader->use();
        m_Shader->uniform("u_ModelViewProjection").set(m_Camera.getViewProjectionMatrix() * transformation.state());
        m_Shader->uniform("u_Texture").set(*m_Texture);

        context->geometry().bind(m_Quad.getVertexBuffer(), *m_Shader);
        context->geometry().drawElements(GL_TRIANGLES, m_Quad.getTriangleBuffer().size(), GL_UNSIGNED_BYTE, m_Quad.getTriangleBuffer().ptr());
        context->geometry().unbind(m_Quad.getVertexBuffer());
    }

    inline void setTexture(Texture* texture) { m_Texture = texture; }

private:
    Quad m_Quad;
    Shader::Program* m_Shader;
    Texture* m_Texture;
    glm::vec2 m_Dimension;
    Camera m_Camera;
};
