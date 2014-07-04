#pragma once

#include "Core/Headers.hh"

class Ray
{
public:
    Ray(glm::vec3 position, glm::vec3 direction)
    {
        m_Position = position;
        m_Direction = direction;
    }

    virtual ~Ray()
    {
    }

    inline const glm::vec3 position() const { return m_Position; }

    inline const glm::vec3 direction() const { return m_Direction; }

private:
    glm::vec3 m_Position;
    glm::vec3 m_Direction;
};
