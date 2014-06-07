#pragma once

#include "Core/Headers.hh"

class Plane
{
public:
    enum Side
    {
        POSITIVE,
        NEGATIVE,
        INSIDE
    };

    Plane()
    {
        // Note : Default plane is (Y = 0)
        m_Equation = glm::vec4(0, 1, 0, 0);
    }

    virtual ~Plane()
    {
    }

    void normalize()
    {
        float magnitude = glm::length(glm::vec3(m_Equation));
        m_Equation /= magnitude;
    }

    inline float getDistanceToPoint(const glm::vec3& p) const
    {
        return m_Equation.x * p.x + m_Equation.y * p.y + m_Equation.z * p.z + m_Equation.w;
    }

    Side classify(const glm::vec3& point) const
    {
        float d = getDistanceToPoint(point);
        if (d < 0)
            return NEGATIVE;
        if (d > 0)
            return POSITIVE;
        return INSIDE;
    }

    inline void setEquation(const glm::vec4 equation) { m_Equation = equation; }
    inline const glm::vec4& getEquation() const { return m_Equation; }

private:
    // Plane Equation : ax + by + cz = d
    // (a, b, c) is the normal; d is the distance
    glm::vec4 m_Equation;
};
