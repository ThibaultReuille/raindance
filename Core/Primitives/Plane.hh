#pragma once

#include <raindance/Core/Headers.hh>

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

    inline float getDistanceToPoint(const glm::vec3& point) const
    {
        return glm::dot(m_Equation, glm::vec4(point, 1.0));
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
