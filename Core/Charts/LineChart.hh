#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Buffer.hh>
#include <raindance/Core/Primitives/Quad.hh>
#include <raindance/Core/Icon.hh>
#include <raindance/Core/Text.hh>

class LineChart
{
public:
    struct Point
    {
        glm::vec2 Position;
    };

    class Graph
    {
    public:
        Graph()
        {
            m_Shader = ResourceManager::getInstance().loadShader("LineChart_Graph",
                    Resources_Shaders_Charts_LineChartGraph_vert, sizeof(Resources_Shaders_Charts_LineChartGraph_vert),
                    Resources_Shaders_Charts_LineChartGraph_frag, sizeof(Resources_Shaders_Charts_LineChartGraph_frag));
            // m_Shader->dump();
            m_Icon = new Icon();
            m_Icon->load("chart-disk", Resources_Textures_chart_disk_png, sizeof(Resources_Textures_chart_disk_png));
            m_Dirty = true;
        }

        virtual ~Graph()
        {
            SAFE_DELETE(m_Icon);
            ResourceManager::getInstance().unload(m_Shader);
        }

        void addPoint(const glm::vec2& pos)
        {
            Point point;
            point.Position = pos;
            m_Points.push_back(point);
            m_Dirty = true;
            m_FirstUpdate = true;
        }

        void update()
        {
            if (!m_Dirty)
                return;

            m_VertexBuffer.clear();
            m_LineBuffer.clear();

            unsigned short int vcount = 0;
            for(auto& p : m_Points)
            {
                m_VertexBuffer << p.Position;
                m_LineBuffer << vcount;
                vcount++;
            }

            if (!m_FirstUpdate)
                m_VertexBuffer.update();

            m_VertexBuffer.describe("a_Position", 2, GL_FLOAT, sizeof(Point), 0);

            if (m_FirstUpdate)
                m_VertexBuffer.generate(Buffer::DYNAMIC);

            m_FirstUpdate = false;
            m_Dirty = false;
        }

        void draw(Context& context, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
        {
            m_Shader->use();
            m_Shader->uniform("u_LineColor").set(m_Color);
            m_Shader->uniform("u_ModelViewProjectionMatrix").set(projection * view * model);

            context.geometry().bind(m_VertexBuffer, *m_Shader);
            context.geometry().drawElements(GL_LINE_STRIP, m_LineBuffer.size() / sizeof(unsigned short int), GL_UNSIGNED_SHORT, m_LineBuffer.ptr());
            context.geometry().unbind(m_VertexBuffer);

            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);
            for (auto p : m_Points)
                m_Icon->draw(&context, projection * view * model * glm::translate(glm::mat4(), glm::vec3(p.Position, 0.0)), m_Color, 0);
            glDisable(GL_BLEND);
        }

        inline void setLabel(const char* label) { m_Label = std::string(label); m_Dirty = true; }
        inline void setColor(const glm::vec4& color) { m_Color = color; m_Dirty = true; }

    private:
        std::string m_Label;
        glm::vec4 m_Color;
        std::vector<Point> m_Points;

        Icon* m_Icon;
        Buffer m_VertexBuffer;
        Buffer m_LineBuffer;
        Shader::Program* m_Shader;
        bool m_FirstUpdate;
        bool m_Dirty;
    };

    LineChart(const glm::vec2& xlimits, const glm::vec2& ylimits)
    : m_XLimits(xlimits), m_YLimits(ylimits)
    {
        m_Shader = ResourceManager::getInstance().loadShader("LineChart_Background",
                Resources_Shaders_Charts_LineChartBackground_vert, sizeof(Resources_Shaders_Charts_LineChartBackground_vert),
                Resources_Shaders_Charts_LineChartBackground_frag, sizeof(Resources_Shaders_Charts_LineChartBackground_frag));
        // m_Shader->dump();

        m_BackgroundQuad = new Quad();
        m_BackgroundColor = glm::vec4(BLACK, 1.0);
        m_ShowBackground = true;

        m_BorderColor = glm::vec4(WHITE, 1.0);
        m_ShowBorder = true;

        m_Font = new Font();
        m_Title.set("Untitled", m_Font);
        m_ShowTitle = true;
    }

    virtual ~LineChart()
    {
        ResourceManager::getInstance().unload(m_Shader);
        SAFE_DELETE(m_BackgroundQuad);
        SAFE_DELETE(m_Font);

        for (auto& graph : m_Graphs)
            SAFE_DELETE(graph);
    }

    Graph* addGraph(const char* label, const glm::vec4 color)
    {
        Graph* graph = new Graph();

        graph->setLabel(label);
        graph->setColor(color);
        m_Graphs.push_back(graph);

        return graph;
    }

    void update()
    {
        for (auto graph : m_Graphs)
            graph->update();
    }

    void draw(Context& context, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
    {
#ifndef EMSCRIPTEN
        // NOTE : Not supported by WebGL
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
#endif
        // glLineWidth(1.5);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        if (m_ShowBackground)
        {
            glm::vec3 scale = glm::vec3(fabs(m_XLimits[1] - m_XLimits[0]), fabs(m_YLimits[1] - m_YLimits[0]), 1.0);

            m_Shader->use();
            m_Shader->uniform("u_ModelViewProjectionMatrix").set(projection * view * glm::scale(model, scale));
            m_Shader->uniform("u_Color").set(m_BackgroundColor);

            context.geometry().bind(m_BackgroundQuad->getVertexBuffer(), *m_Shader);
            context.geometry().drawElements(GL_TRIANGLES, m_BackgroundQuad->getTriangleBuffer().size(), GL_UNSIGNED_BYTE, m_BackgroundQuad->getTriangleBuffer().ptr());
            context.geometry().unbind(m_BackgroundQuad->getVertexBuffer());
        }

        if (m_ShowBorder)
        {
            glm::vec3 scale = glm::vec3(fabs(m_XLimits[1] - m_XLimits[0]), fabs(m_YLimits[1] - m_YLimits[0]), 1.0);

            m_Shader->use();
            m_Shader->uniform("u_ModelViewProjectionMatrix").set(projection * view * glm::scale(model, scale));
            m_Shader->uniform("u_Color").set(m_BorderColor);

            context.geometry().bind(m_BackgroundQuad->getVertexBuffer(), *m_Shader);
            context.geometry().drawElements(GL_LINES, m_BackgroundQuad->getLineBuffer().size(), GL_UNSIGNED_BYTE, m_BackgroundQuad->getLineBuffer().ptr());
            context.geometry().unbind(m_BackgroundQuad->getVertexBuffer());
        }

        // Charts
        {
            glm::vec3 translate = glm::vec3(-m_XLimits[0], -m_YLimits[0], 0.0);
            for (auto graph : m_Graphs)
                graph->draw(context, glm::translate(model, translate), view, projection);
        }

        if (m_ShowTitle)
        {
            Transformation transformation;
            float size = fabs(m_XLimits[1] - m_XLimits[0]) / 1000;
            float y = fabs(m_YLimits[1] - m_YLimits[0]);
            transformation.translate(glm::vec3(5 * size, y, 0));
            transformation.scale(glm::vec3(size, size, 1.0));

            m_Title.draw(&context, projection * view * model * transformation.state());
        }

        glEnable(GL_DEPTH_TEST);
    }

    inline void setBackgroundColor(const glm::vec4 color) { m_BackgroundColor = color; }
    inline void showBackground(bool visible) { m_ShowBackground = visible; }

    inline void setBorderColor(const glm::vec4 color) { m_BorderColor = color; }
    inline void showBorder(bool visible) { m_ShowBorder = visible; }

    inline void setTitle(const char* title) { m_Title.set(title, m_Font); }
    inline void showTitle(bool visible) { m_ShowTitle = visible; }

private:
    glm::vec2 m_XLimits;
    glm::vec2 m_YLimits;

    Shader::Program* m_Shader;

    Quad* m_BackgroundQuad;
    glm::vec4 m_BackgroundColor;
    bool m_ShowBackground;

    glm::vec4 m_BorderColor;
    bool m_ShowBorder;

    Text m_Title;
    bool m_ShowTitle;
    Font* m_Font;

    std::vector<Graph*> m_Graphs;
};
