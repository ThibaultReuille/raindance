#pragma once

#include "Core/Scene.hh"

class WideLine
{
public:
    struct Vertex
    {
        Vertex()
            : Position(glm::vec3(0, 0, 0)),
              Color(glm::vec4(1, 1, 1, 1)),
              Number(-1)
        {
        }

        Vertex(const glm::vec3& position, const glm::vec4& color)
            : Position(position),
              Color(color),
              Number(-1)
        {
        }

        Vertex(const glm::vec3& position, const glm::vec4& color, int number)
            : Position(position),
              Color(color),
              Number(number)
        {
        }

        glm::vec3 Position;
        glm::vec4 Color;
        int Number;
    };

    WideLine()
        : m_FirstUpdate(true)
    {
        m_FirstUpdate = true;
        update();
    }

    virtual ~WideLine()
    {
    }

    virtual void update()
    {
        Vertex v[4] =
        {
                Vertex(m_Positions[0], m_Colors[0], 0),
                Vertex(m_Positions[0], m_Colors[0], 1),
                Vertex(m_Positions[1], m_Colors[1], 2),
                Vertex(m_Positions[1], m_Colors[1], 3)
        };

        m_VertexBuffer.clear();
        for (int i = 0; i < 4; i++)
            m_VertexBuffer.push(&v, sizeof(Vertex));

        if (!m_FirstUpdate)
            m_VertexBuffer.update();

        m_VertexBuffer.describe("a_Position", 3, GL_FLOAT, sizeof(Vertex), 0);
        m_VertexBuffer.describe("a_Color",    4, GL_FLOAT, sizeof(Vertex), sizeof(glm::vec3));
        m_VertexBuffer.describe("a_Number",   1, GL_BYTE,  sizeof(Vertex), sizeof(glm::vec3) + sizeof(glm::vec4));

        if (m_FirstUpdate)
        {
            m_VertexBuffer.generate(Buffer::DYNAMIC);
            m_FirstUpdate = false;
        }
    }

    inline void setPosition(unsigned int i, const glm::vec3& p) { m_Positions[i] = p; }
    inline const glm::vec3& getPosition(unsigned int i) const { return m_Positions[i]; }

    inline void setColor(unsigned int i, const glm::vec4& c) { m_Colors[i] = c; }
    inline const glm::vec4& getColor(unsigned int i) const { return m_Colors[i]; }

    inline Buffer& getVertexBuffer() { return m_VertexBuffer; }

protected:
    glm::vec3 m_Positions[2];
    glm::vec4 m_Colors[2];
    Buffer m_VertexBuffer;
    bool m_FirstUpdate;
};
