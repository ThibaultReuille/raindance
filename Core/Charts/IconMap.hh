#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Buffer.hh>
#include <raindance/Core/Primitives/Quad.hh>
#include <raindance/Core/Icon.hh>
#include <raindance/Core/Text.hh>

class IconMap
{
public:
    struct Layout
    {
        unsigned long Shape;
        glm::vec4 Color;
        float Size;
    };

    IconMap(unsigned long width, unsigned long height)
    {
        m_Width = width;
        m_Height = height;

        m_Icon = new Icon(glm::vec2(1.0, 1.0), glm::vec2(0.5, -0.5));
        m_Icon->load("disk",        Assets_Textures_Shapes_disk_png,         sizeof(Assets_Textures_Shapes_disk_png));
        m_Icon->load("square",      Assets_Textures_Shapes_square_png,       sizeof(Assets_Textures_Shapes_square_png));
        m_Icon->load("triangle",    Assets_Textures_Shapes_triangle_png,     sizeof(Assets_Textures_Shapes_triangle_png));
        m_Icon->load("losange",     Assets_Textures_Shapes_losange_png,      sizeof(Assets_Textures_Shapes_losange_png));
        m_Icon->load("pentagon",    Assets_Textures_Shapes_pentagon_png,     sizeof(Assets_Textures_Shapes_pentagon_png));
        m_Icon->load("hexagon",     Assets_Textures_Shapes_hexagon_png,      sizeof(Assets_Textures_Shapes_hexagon_png));
        m_Icon->load("octagon",     Assets_Textures_Shapes_octagon_png,      sizeof(Assets_Textures_Shapes_octagon_png));
        m_Icon->load("star",        Assets_Textures_Shapes_star_png,         sizeof(Assets_Textures_Shapes_star_png));
        m_Icon->load("heart",       Assets_Textures_Shapes_heart_png,        sizeof(Assets_Textures_Shapes_heart_png));
        m_Icon->load("cross",       Assets_Textures_Shapes_cross_png,        sizeof(Assets_Textures_Shapes_cross_png));
        m_Icon->load("cloud",       Assets_Textures_Shapes_cloud_png,        sizeof(Assets_Textures_Shapes_cloud_png));
        m_Icon->load("semicircle",  Assets_Textures_Shapes_semicircle_png,   sizeof(Assets_Textures_Shapes_semicircle_png));
        m_Icon->load("patch",       Assets_Textures_Shapes_patch_png,        sizeof(Assets_Textures_Shapes_patch_png));
        m_Icon->load("house",       Assets_Textures_Shapes_house_png,        sizeof(Assets_Textures_Shapes_house_png));
        m_Icon->load("triangle1",   Assets_Textures_Shapes_triangle1_png,    sizeof(Assets_Textures_Shapes_triangle1_png));
        m_Icon->load("triangle2",   Assets_Textures_Shapes_triangle2_png,    sizeof(Assets_Textures_Shapes_triangle2_png));

        m_Layouts.resize(width * height, NULL);

        m_Font = new Font();
        m_Shader = NULL;
    }

    virtual ~IconMap()
    {
        SAFE_DELETE(m_Icon);
    }

    void update()
    {
    }

    void draw(Context& context, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
    {
        Transformation transformation;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

        for (unsigned long j = 0; j < m_Height; j++)
            for (unsigned long i = 0; i < m_Width; i++)
            {
                Layout* layout = m_Layouts[i + m_Width * j];
                if (layout == NULL)
                    continue;

                glm::vec3 position;
                position.x = static_cast<float>(i);
                position.y = -static_cast<float>(j);
                position.z = 0;

                transformation.set(model);
                transformation.translate(position);
                transformation.scale(glm::vec3(layout->Size, layout->Size, layout->Size));
                m_Icon->draw(&context, projection * view * transformation.state(), layout->Color, layout->Shape);
            }

        glDisable(GL_BLEND);
    }

    void set(unsigned long x, unsigned long y, unsigned long shape, const glm::vec4& color, float size)
    {
        if (x >= m_Width || y >= m_Height)
        {
            LOG("[ICONMAP] Position (%zu, %zu) is out of bounds!\n", x, y);
            return;
        }

        unsigned long index = x + m_Width * y;
        if (m_Layouts[index] == NULL)
            m_Layouts[index] = new Layout();

        m_Layouts[index]->Shape = shape;
        m_Layouts[index]->Color = color;
        m_Layouts[index]->Size = size;
    }

    inline unsigned long width() { return m_Width; }
    inline unsigned long height() { return m_Height; }
    inline const Icon* icon() { return m_Icon; }

private:
    unsigned long m_Width;
    unsigned long m_Height;

    Shader::Program* m_Shader;
    Icon* m_Icon;

    Font* m_Font;

    std::vector<Layout*> m_Layouts;
};
