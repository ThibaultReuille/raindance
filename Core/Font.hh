#pragma once

#include <raindance/Core/Headers.hh>

const std::string g_VertexShader =
"attribute vec2 a_Position;"
"attribute vec2 a_Texcoord;"
""
"uniform mat4 u_ModelViewProjection;"
"uniform vec4 u_Color;"
""
"varying vec2 v_Texcoord;"
"varying vec4 v_Color;"
""
"void main(void)"
"{"
"    v_Color = u_Color;"
"    v_Texcoord = a_Texcoord;"
"    gl_Position = u_ModelViewProjection * vec4(a_Position, 0.0, 1.0);"
"}";

const std::string g_FragmentShader =
"#ifdef GL_ES              \n"
"precision mediump float;  \n"
"#endif                    \n"
""
"uniform sampler2D u_Font;"
""
"varying vec2 v_Texcoord;"
"varying vec4 v_Color;"
""
"void main()"
"{"
"    vec4 tx = texture2D(u_Font, v_Texcoord);"
"    float mask = tx.r;"
"    float delta = 0.3;"
"    float finalalpha = smoothstep(0.5 - delta, 0.5 + delta, mask);"
"    gl_FragColor = vec4(vec3(v_Color), finalalpha);"
"}";

class Font
{
public:

    struct Glyph
    {
        glm::vec2 Dimension;
        glm::vec2 Bearing;
        float AdvanceX;
        glm::vec2 Texcoord[2];

        Glyph(const picojson::value& glyph)
        {
            Dimension = glm::vec2(glyph.get("bbox_width").get<double>(), glyph.get("bbox_height").get<double>());
            Bearing = glm::vec2(glyph.get("bearing_x").get<double>(), glyph.get("bearing_y").get<double>());
            AdvanceX = static_cast<float>(glyph.get("advance_x").get<double>());
            Texcoord[0] = glm::vec2(glyph.get("s0").get<double>(), 1.0 - glyph.get("t0").get<double>());
            Texcoord[1] = glm::vec2(glyph.get("s1").get<double>(), 1.0 - glyph.get("t1").get<double>());

            // dump();
        }

        void dump()
        {
            LOG("Glyph {\n");
            LOG("\tDimension : (%f, %f)\n", Dimension.x, Dimension.y);
            LOG("\tBearing : (%f, %f)\n", Bearing.x, Bearing.y);
            LOG("\tAdvanceX : %f\n", AdvanceX);
            LOG("\tTexcoord 0 : (%f, %f)\n", Texcoord[0].x, Texcoord[0].y);
            LOG("\tTexcoord 1 : (%f, %f)\n", Texcoord[1].x, Texcoord[1].y);
            LOG("}\n");
        }
    };

    Font(const char* name = NULL,
         unsigned char* png_data = NULL, size_t png_data_len = 0,
         unsigned char* json_data = NULL, size_t json_data_len = 0)
    {
        m_Shader = ResourceManager::getInstance().loadShader("shaders/sdf_font", g_VertexShader, g_FragmentShader);
        // m_Shader->dump();

        std::string font_name;

        if (name == NULL || png_data == NULL || json_data == NULL)
        {
            font_name = std::string("fonts/default");

            /*
            png_data = Existence_Light_png;
            png_data_len = Existence_Light_png_len;
            json_data = Existence_Light_json;
            json_data_len = Existence_Light_json_len;
            */

            /*
            png_data = Robotica_png;
            png_data_len = Robotica_png_len;
            json_data = Robotica_json;
            json_data_len = Robotica_json_len;
            */


            /*
            png_data = America_Sans_png;
            png_data_len = America_Sans_png_len;
            json_data = America_Sans_json;
            json_data_len = America_Sans_json_len;
            */

            /*
            png_data = Cuprum_Regular_png;
            png_data_len = Cuprum_Regular_png_len;
            json_data = Cuprum_Regular_json;
            json_data_len = Cuprum_Regular_json_len;
            */

            /*
            png_data = Kimberley_bl_png;
            png_data_len = Kimberley_bl_png_len;
            json_data = Kimberley_bl_json;
            json_data_len = Kimberley_bl_json_len;
            */


            png_data = Sansation_Regular_png;
            png_data_len = Sansation_Regular_png_len;
            json_data = Sansation_Regular_json;
            json_data_len = Sansation_Regular_json_len;

        }
        else
            font_name = std::string(name);

        LOG("[FONT] Loading '%s' font ...\n", font_name.c_str());

        m_Texture = ResourceManager::getInstance().loadTexture(font_name.c_str(), png_data, static_cast<unsigned int>(png_data_len));

        char* json_data_ptr = reinterpret_cast<char*>(json_data);
        std::istrstream json_stream(json_data_ptr, json_data_len);

        json_stream >> m_JsonData;
        if (std::cin.fail())
        {
            LOG("[FONT] PicoJson failed to load font json!\n");
            std::cerr << picojson::get_last_error() << std::endl;
            return;
        }

        m_Size = m_JsonData.get("size").get<double>();
        m_Height = m_JsonData.get("height").get<double>();
		m_SpaceAdvance = m_JsonData.get("space_advance").get<double>();
		m_Ascender = m_JsonData.get("ascender").get<double>();
		m_Descender = m_JsonData.get("descender").get<double>();

        // dump(m_JsonData);
    }

    virtual ~Font()
    {
        ResourceManager::getInstance().unload(m_Texture);
        ResourceManager::getInstance().unload(m_Shader);
    }

    void dump(const picojson::value& v, int tab = 0)
    {
        if (v.is<picojson::null>())
            std::cout << "null" << std::endl;
        else if (v.is<bool>())
            std::cout << (v.get<bool>() ? "true" : "false") << std::endl;
        else if (v.is<double>())
            std::cout << v.get<double>() << " (double)" << std::endl;
        else if (v.is<std::string>())
            std::cout << "'" << v.get<std::string>() << "'" << std::endl;
        else if (v.is<picojson::array>())
        {
            std::cout << "[" << std::endl;

            const picojson::array& a = v.get<picojson::array>();
            for (picojson::array::const_iterator i = a.begin(); i != a.end(); ++i)
            {
                dump(*i, tab + 1);
            }

            std::cout << "]" << std::endl;
        }
        else if (v.is<picojson::object>())
        {
            std::cout << "{" << std::endl;

            const picojson::object& o = v.get<picojson::object>();
            for (picojson::object::const_iterator i = o.begin(); i != o.end(); ++i)
            {
                std::cout << std::string(tab + 1, '\t');
                std::cout << "'" << i->first << "' : ";
                dump(i->second, tab + 1);
            }

            std::cout << std::string(tab, '\t') << "}" << std::endl;
        }
    }

    virtual float build(const char* str, Buffer& buffer)
    {
        buffer.clear();

        glm::vec2 p = glm::vec2(0.0, 0.0);

        std::string s (str);
        for (size_t i = 0; i < s.length(); i++)        {

            picojson::value v = m_JsonData.get("glyph_data").get(std::string(1, s[i]));

            if (v.is<picojson::null>())
            {
                LOG("[FONT] Warning : No glyph for characted '%c' !\n", s[i]);
                p.x += m_SpaceAdvance;
                continue;
            }

            Glyph g(v);

            if (i > 0)
            {
                picojson::value kernval = v.get("kernings").get(std::string(1, s[i - 1]));
                if (!kernval.is<picojson::null>())
                    p.x += kernval.get<double>() * m_Size;
            }

            float w = g.Dimension.x * m_Size;
            float h = g.Dimension.y * m_Size;

            float x = p.x + g.Bearing.x * m_Size;
            float y = p.y + g.Bearing.y * m_Size;

            #define RD_RESCALE_Y(Y) ()

            //        Position                   Texcoord
            buffer << glm::vec2(x,     y - h) << glm::vec2(g.Texcoord[0].x, rescale(g, g.Texcoord[0].y)); // 0
            buffer << glm::vec2(x + w, y - h) << glm::vec2(g.Texcoord[1].x, rescale(g, g.Texcoord[0].y)); // 1
            buffer << glm::vec2(x,     y    ) << glm::vec2(g.Texcoord[0].x, rescale(g, g.Texcoord[1].y)); // 2

            buffer << glm::vec2(x + w, y - h) << glm::vec2(g.Texcoord[1].x, rescale(g, g.Texcoord[0].y)); // 1
            buffer << glm::vec2(x + w, y    ) << glm::vec2(g.Texcoord[1].x, rescale(g, g.Texcoord[1].y)); // 3
            buffer << glm::vec2(x,     y    ) << glm::vec2(g.Texcoord[0].x, rescale(g, g.Texcoord[1].y)); // 2

            p.x += g.AdvanceX * m_Size;

            str++;
        }

        if (buffer.isGenerated())
        {
            buffer.update();
            buffer.describe("a_Position", 2, GL_FLOAT, 4 * sizeof(GLfloat), 0);
            buffer.describe("a_Texcoord", 2, GL_FLOAT, 4 * sizeof(GLfloat), 2 * sizeof(GLfloat));
        }
        else
        {
            buffer.describe("a_Position", 2, GL_FLOAT, 4 * sizeof(GLfloat), 0);
            buffer.describe("a_Texcoord", 2, GL_FLOAT, 4 * sizeof(GLfloat), 2 * sizeof(GLfloat));
            buffer.generate(Buffer::DYNAMIC);
        }

        return p.x;
    }

    inline const Texture& texture() { return *m_Texture; }
    inline Shader::Program& shader() { return *m_Shader; }

    inline float getSize() const { return m_Size; }
    inline float getHeight() const { return m_Height; }
    inline float getSpaceAdvance() const { return m_SpaceAdvance; }
    inline float getAscender() const { return m_Ascender; }
    inline float getDescender() const { return m_Descender; }

private:
    inline float rescale(const Glyph& g, const float v) const { return g.Texcoord[0].y + (g.Texcoord[1].y - g.Texcoord[0].y) - (v - g.Texcoord[0].y); }

    Shader::Program* m_Shader;
    Texture* m_Texture;
    picojson::value m_JsonData;
    float m_Size;
    float m_Height;
    float m_SpaceAdvance;
    float m_Ascender;
    float m_Descender;
};
