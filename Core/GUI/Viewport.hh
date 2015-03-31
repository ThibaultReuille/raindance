#pragma once

#include <raindance/Core/Headers.hh>

class Viewport
{
public:

    struct Framebuffer
    {
        Framebuffer(int width, int height) : Width(width), Height(height) {}

        inline glm::vec2 getDimension() const { return glm::vec2(Width, Height); }

        int Width;
        int Height;
    };

    Viewport(const glm::vec2& pos = glm::vec2(0.0, 0.0), const glm::vec2& dim = glm::vec2(0.0, 0.0))
    : m_Position(pos), m_Dimension(dim), m_Framebuffer((int) dim.x, (int) dim.y)
    {
    }

    inline void setPosition(const glm::vec2& position) { m_Position = position; }
    inline const glm::vec2& getPosition() const { return m_Position; }

    inline void setDimension(const glm::vec2& dimension) { m_Dimension = dimension; }
    inline const glm::vec2& getDimension() const { return m_Dimension; }

    inline void setFramebuffer(int width, int height) { m_Framebuffer = Framebuffer(width, height); }
    inline const Framebuffer& getFramebuffer() const { return m_Framebuffer; }

private:
    glm::vec2 m_Position;
    glm::vec2 m_Dimension;
    Framebuffer m_Framebuffer;
};
