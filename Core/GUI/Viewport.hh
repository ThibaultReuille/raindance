#pragma once

#include <raindance/Core/Headers.hh>

class Viewport
{
public:
    Viewport(const glm::vec2& position, const glm::vec2& dimension)
    {
        m_Position = position;
        m_Dimension = dimension;
    }

    inline void setPosition(const glm::vec2& position) { m_Position = position; }
    inline const glm::vec2& getPosition() const { return m_Position; }

    inline void setDimension(const glm::vec2& dimension) { m_Dimension = dimension; }
    inline const glm::vec2& getDimension() const { return m_Dimension; }

private:
    glm::vec2 m_Position;
    glm::vec2 m_Dimension;
};
