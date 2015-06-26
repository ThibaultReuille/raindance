#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Buffer.hh>
#include <raindance/Core/Icon.hh>
#include <raindance/Core/Text.hh>

class HeightMap
{
public:

    struct Point
    {
        float Value;
        glm::vec4 Color;
    };

    struct Vertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
    };

    HeightMap(unsigned long width, unsigned long height)
    {
        m_Shader = ResourceManager::getInstance().loadShader("HeightMap",
                Assets_Shaders_Charts_HeightMap_vert, sizeof(Assets_Shaders_Charts_HeightMap_vert),
                Assets_Shaders_Charts_HeightMap_frag, sizeof(Assets_Shaders_Charts_HeightMap_frag));
        // m_Shader->dump();

        m_Width = width;
        m_Height = height;
        m_Points.resize(m_Width * m_Height);
        m_Dirty = true;
        m_FirstUpdate = true;

        m_BorderColor = glm::vec4(WHITE, 1.0);
        m_ShowBorder = true;

        m_Font = new rd::Font();
        m_Title.set("Untitled", m_Font);
        m_ShowTitle = true;
    }

    virtual ~HeightMap()
    {
        ResourceManager::getInstance().unload(m_Shader);
        SAFE_DELETE(m_Font);
    }

    void update()
    {
        if (!m_Dirty)
            return;

        m_VertexBuffer.clear();
        m_LineBuffer.clear();
        m_TriangleBuffer.clear();

        for(unsigned long i = 0; i < m_Width; i++)
            for(unsigned long j = 0; j < m_Height; j++)
            {
                float x = static_cast<float>(i) / m_Width;
                float z = static_cast<float>(j) / m_Height;

                m_VertexBuffer << glm::vec3(static_cast<float>(i), m_Points[i + m_Width * j].Value, static_cast<float>(j));
                float gray = 0.5 - fabs(fmod(m_Points[i + m_Width * j].Value, 10.0)) / 10.0;
                m_VertexBuffer << glm::vec4(0.5 * x + 0.5 * gray, gray, 0.5 * z + 0.5 * gray, 1.0);

                if (i == 0 && j == 0)
                    continue;
                else if (i == 0 && j > 0 && j < (m_Height - 1))
                {
                    unsigned short int a = static_cast<unsigned short int>(i + j *       m_Width);
                    unsigned short int b = static_cast<unsigned short int>(i + (j + 1) * m_Width);
                    m_LineBuffer << a << b;
                }
                else if (j == 0 && i > 0 && i < (m_Width - 1))
                {
                    unsigned short int a = static_cast<unsigned short int>(i + j *       m_Width);
                    unsigned short int b = static_cast<unsigned short int>((i + 1) + j * m_Width);
                    m_LineBuffer << a << b;
                }
                else if (i > 0 && j > 0)
                {
                    unsigned short int a = static_cast<unsigned short int>((i - 1) + (j - 1) * m_Width);
                    unsigned short int b = static_cast<unsigned short int>(i       + (j - 1) * m_Width);
                    unsigned short int c = static_cast<unsigned short int>(i       + j       * m_Width);
                    unsigned short int d = static_cast<unsigned short int>((i - 1) + j       * m_Width);

                    m_TriangleBuffer << a << b << c;
                    m_TriangleBuffer << a << c << d;

                    m_LineBuffer << b << c << c << d;
                }
            }

        if (!m_FirstUpdate)
            m_VertexBuffer.update();

        m_VertexBuffer.describe("a_Position", 3, GL_FLOAT, sizeof(Vertex), 0);
        m_VertexBuffer.describe("a_Color", 4, GL_FLOAT, sizeof(Vertex), sizeof(glm::vec3));

        if (m_FirstUpdate)
            m_VertexBuffer.generate(Buffer::DYNAMIC);

        m_FirstUpdate = false;
        m_Dirty = false;
    }

    void draw(Context& context, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
    {
#ifndef EMSCRIPTEN
        // NOTE : Not supported by WebGL
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
#endif
        glLineWidth(1.0);
        glEnable(GL_POLYGON_OFFSET_FILL);

        m_Shader->use();
        m_Shader->uniform("u_ModelViewProjectionMatrix").set(projection * view * model);

        context.geometry().bind(m_VertexBuffer, *m_Shader);

        m_Shader->uniform("u_Mode").set(0);
        context.geometry().drawElements(GL_TRIANGLES, m_TriangleBuffer.size() / sizeof(unsigned short int), GL_UNSIGNED_SHORT, m_TriangleBuffer.ptr());

        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0, 2);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

        m_Shader->uniform("u_Mode").set(1);
        m_Shader->uniform("u_LineColor").set(glm::vec4(0.5, 0.5, 0.5, 0.3));
        context.geometry().drawElements(GL_LINES, m_LineBuffer.size() / sizeof(unsigned short int), GL_UNSIGNED_SHORT, m_LineBuffer.ptr());

        glDisable(GL_POLYGON_OFFSET_FILL);

        context.geometry().unbind(m_VertexBuffer);
        glDisable(GL_BLEND);
    }

    inline void setTitle(const char* title) { m_Title.set(title, m_Font); }
    inline void showTitle(bool visible) { m_ShowTitle = visible; }

    inline void setPoint(unsigned long i, unsigned long j, const Point& point) { m_Points[i + m_Width * j] = point; m_Dirty = true; }
    inline const Point& getPoint(unsigned long i, unsigned long j) const { return m_Points[i + m_Width * j]; }

    inline void setValue(unsigned long i, unsigned long j, float value) { m_Points[i + m_Width * j].Value = value; m_Dirty = true; }
    inline float getValue(unsigned long i, unsigned long j) const { return m_Points[i + m_Width * j].Value; }

    inline void setColor(unsigned long i, unsigned long j, const glm::vec4& color) { m_Points[i + m_Width * j].Color = color; m_Dirty = true; }
    inline const glm::vec4& getColor(unsigned long i, unsigned long j) const { return m_Points[i + m_Width * j].Color; }


private:
    unsigned long m_Width;
    unsigned long m_Height;

    std::vector<Point> m_Points;
    Buffer m_VertexBuffer;
    Buffer m_TriangleBuffer;
    Buffer m_LineBuffer;
    bool m_Dirty;
    bool m_FirstUpdate;

    Shader::Program* m_Shader;

    glm::vec4 m_BorderColor;
    bool m_ShowBorder;

    Text m_Title;
    bool m_ShowTitle;
    rd::Font* m_Font;
};
