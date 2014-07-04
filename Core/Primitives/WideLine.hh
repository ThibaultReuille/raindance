#pragma once

#include "Core/Headers.hh"
#include "Core/Camera/Camera.hh"

class WideLine
{
public:
    struct Vertex
    {
        Vertex(const glm::vec2& position, const glm::vec4& color)
            : Position(position),
              Color(color)
        {
        }

        glm::vec2 Position;
        glm::vec4 Color;
    };

    WideLine()
        : m_FirstUpdate(true), m_Width(1.0)
    {
        update();
    }

    virtual ~WideLine()
    {
    }

    virtual void update()
    {
        Vertex v[4] =
        {
                Vertex(glm::vec2(-1.0, -1.0), m_Colors[0]),
                Vertex(glm::vec2( 1.0, -1.0), m_Colors[1]),
                Vertex(glm::vec2(-1.0,  1.0), m_Colors[0]),
                Vertex(glm::vec2( 1.0,  1.0), m_Colors[1])
        };

        m_VertexBuffer.clear();
        m_VertexBuffer.push(v, 4 * sizeof(Vertex));

        if (!m_FirstUpdate)
            m_VertexBuffer.update();

        m_VertexBuffer.describe("a_Position", 2, GL_FLOAT, sizeof(Vertex), 0);
        m_VertexBuffer.describe("a_Color",    4, GL_FLOAT, sizeof(Vertex), sizeof(glm::vec2));

        if (m_FirstUpdate)
            m_VertexBuffer.generate(Buffer::DYNAMIC);
        m_FirstUpdate = false;
    }

    const glm::vec3 calculateExtrudeDirection(const Camera& camera) const
    {
        glm::vec3 lineDirection = m_Positions[1] - m_Positions[0];
        glm::vec3 lineMidpoint = 0.5f * (m_Positions[0] + m_Positions[1]);
        glm::vec3 quadNormal = camera.getPosition() - lineMidpoint;
        return (m_Width / 2.0f) * glm::normalize(glm::cross(lineDirection, quadNormal));
    }

    inline void setWidth(float width) { m_Width = width; }
    inline float getWidth() { return m_Width; }

    inline void setPosition(unsigned int i, const glm::vec3& p) { m_Positions[i] = p; }
    inline const glm::vec3& getPosition(unsigned int i) const { return m_Positions[i]; }

    inline void setColor(unsigned int i, const glm::vec4& c) { m_Colors[i] = c; }
    inline const glm::vec4& getColor(unsigned int i) const { return m_Colors[i]; }

    inline Buffer& getVertexBuffer() { return m_VertexBuffer; }

protected:
    bool m_FirstUpdate;

    glm::vec3 m_Positions[2];
    glm::vec4 m_Colors[2];
    float m_Width;

    Buffer m_VertexBuffer;
};
