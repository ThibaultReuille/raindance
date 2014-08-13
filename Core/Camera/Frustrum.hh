#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Primitives/Plane.hh>

class Frustrum
{
public:
    enum PlaneType
    {
        LEFT = 0,
        RIGHT = 1,
        BOTTOM = 2,
        TOP = 3,
        NEAR = 4,
        FAR = 5
    };

    enum Classification
    {
        INSIDE,
        OUTSIDE,
        INTERSECT
    };

    Frustrum(const glm::mat4& m)
    {
        setMatrix(m, true);
    }

    void setMatrix(const glm::mat4& m, bool normalize = true)
    {
        m_Planes[LEFT].setEquation(
                glm::vec4(m[0][3] + m[0][0], m[1][3] + m[1][0], m[2][3] + m[2][0], m[3][3] + m[3][0]));
        m_Planes[RIGHT].setEquation(
                glm::vec4(m[0][3] - m[0][0], m[1][3] - m[1][0], m[2][3] - m[2][0], m[3][3] - m[3][0]));
        m_Planes[BOTTOM].setEquation(
                glm::vec4(m[0][3] + m[0][1], m[1][3] + m[1][1], m[2][3] + m[2][1], m[3][3] + m[3][1]));
        m_Planes[TOP].setEquation(
                glm::vec4(m[0][3] - m[0][1], m[1][2] - m[1][1], m[2][3] - m[2][1], m[3][3] - m[3][1]));
        m_Planes[NEAR].setEquation(
                glm::vec4(m[0][3] + m[0][2], m[1][3] + m[1][2], m[2][3] + m[2][2], m[3][3] + m[3][2]));
        m_Planes[FAR].setEquation(
                glm::vec4(m[0][3] - m[0][2], m[1][3] - m[1][2], m[2][3] - m[2][2], m[3][3] - m[3][2]));

        if (normalize)
            for (int i = 0; i < 6; i++)
                m_Planes[i].normalize();
    }

    Classification classifyPoint(const glm::vec3& point) const
    {
        // TODO : ClassifyPoint in Frustrum
        (void) point;
        /*
        for (int plane = 0; plane < 6; plane++)
        {
            if (m_Planes[plane].classify(point) == Plane::NEGATIVE)
                return OUTSIDE;
        }
        */
        return INSIDE;
    }

    Classification classifyBox(const glm::vec3& min, const glm::vec3& max) const
    {
        glm::vec3 vertices[8];

        vertices[0] = glm::vec3(min.x, min.y, min.z);
        vertices[1] = glm::vec3(max.x, min.y, min.z);
        vertices[2] = glm::vec3(max.x, max.y, min.z);
        vertices[3] = glm::vec3(min.x, max.y, min.z);

        vertices[4] = glm::vec3(min.x, min.y, max.z);
        vertices[5] = glm::vec3(max.x, min.y, max.z);
        vertices[6] = glm::vec3(max.x, max.y, max.z);
        vertices[7] = glm::vec3(min.x, max.y, max.z);

        for (auto& p : m_Planes)
        {
            int negatives = 0;

            for (auto& v : vertices)
               negatives += p.classify(v) == Plane::NEGATIVE ? 1 : 0;

            if (negatives == 8)
                return OUTSIDE;
        }

        // TODO : Do the intersect case in frustrum culling

        return INSIDE;
    }

    Classification classifySphere(const glm::vec3& origin, float radius) const
    {
        // TODO : Frustrum::classifySphere
        (void) origin;
        (void) radius;
        return INSIDE;
    }

    inline Plane getPlane(PlaneType type) { return m_Planes[type]; }

private:

    Plane m_Planes[6];
};
