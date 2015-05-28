#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/GUI/Widgets/Widget.hh>
#include <raindance/Core/Icon.hh>

class TimelineWidget : public IWidget
{
public:
    TimelineWidget(const char* name, IWidget* parent, glm::vec3 position, glm::vec2 dimension)
    : IWidget(name, parent, position, dimension)
    {
        m_Shader = ResourceManager::getInstance().loadShader("timeline", Assets_Shaders_Widgets_timeline_vert, sizeof(Assets_Shaders_Widgets_timeline_vert),
                                                                         Assets_Shaders_Widgets_timeline_frag, sizeof(Assets_Shaders_Widgets_timeline_frag),
                                                                         Assets_Shaders_Widgets_timeline_geom, sizeof(Assets_Shaders_Widgets_timeline_geom));
        // m_Shader->dump();

        m_EventIcon = new Icon(glm::vec2(7.5, 7.5));
        m_EventIcon->load("timeline_event_start", Assets_Textures_Shapes_square_png, sizeof(Assets_Textures_Shapes_square_png));
        m_EventIcon->load("timeline_event_stop", Assets_Textures_Shapes_triangle_png, sizeof(Assets_Textures_Shapes_triangle_png));
        m_EventIcon->load("timeline_event_once", Assets_Particle_ball_png, sizeof(Assets_Particle_ball_png));

        m_Track = NULL;

        m_Before = 2000;
        m_After = 6000;
        m_NeedsEventCursorUpdate = true;
        m_LastCurrentTimecode = 0;

        update();
    }

    virtual ~TimelineWidget()
    {
        ResourceManager::getInstance().unload(m_Shader);
        m_Track = NULL;
        SAFE_DELETE(m_EventIcon);
    }

    void update()
    {
        m_VertexBuffer.clear();
        m_VertexBuffer << glm::vec2(0.0, 0.0);
        m_VertexBuffer.describe("a_Position", 2, GL_FLOAT, sizeof(glm::vec2), 0);
        m_VertexBuffer.generate(Buffer::STATIC);
    }

    virtual void draw(Context* context, glm::mat4 model, glm::mat4 view, glm::mat4 projection)
    {
        Transformation transformation;

        glm::mat4 viewProjection = projection * view;

        transformation.push();
        {
            transformation.set(model);
            transformation.scale(glm::vec3(m_Dimension, 1.0));

            m_Shader->use();
            m_Shader->uniform("u_ProjectionMatrix").set(projection);
            m_Shader->uniform("u_ModelViewMatrix").set(view * glm::scale(model, glm::vec3(m_Dimension, 1.0)));
            m_Shader->uniform("u_Color").set(glm::vec4(0.2, 0.2, 0.2, 0.7));
            m_Shader->uniform("u_Value").set((float) m_Before / ((float)m_Before + (float)m_After));
            context->geometry().bind(m_VertexBuffer, *m_Shader);
            context->geometry().drawArrays(GL_POINTS, 0, m_VertexBuffer.size() / sizeof(glm::vec2));
            context->geometry().unbind(m_VertexBuffer);
        }
        transformation.pop();

        if (m_Track != NULL)
        {
            Timecode current = m_Track->clock().milliseconds();

            if (m_NeedsEventCursorUpdate || current != m_LastCurrentTimecode)
            {
                m_EventCursor = m_Track->events_from(current <= m_Before ? 0 : current - m_Before);
                m_NeedsEventCursorUpdate = false;
            }
            m_LastCurrentTimecode = current;

            Timecode lastTimecode = 0;
            glm::vec3 cursor;
            glm::vec4 color;
            unsigned int mode;
            float simultaneous = 0.0f;

            Track::EventIterator eit = m_EventCursor;
            while(eit != m_Track->events_end())
            {
                Timecode timecode = eit->timecode();
                if (timecode >= current + m_After)
                    break;

                switch(eit->type())
                {
                case Track::Event::START:
                    mode = 0;
                    break;
                case Track::Event::STOP:
                    mode = 1;
                    break;
                case Track::Event::ONCE:
                    mode = 2;
                    break;
                default:
                    LOG("[TIMELINE] Unknown event type!\n");
                    continue;
                    break;
                }

                float d = fabs((float)timecode - (float)current);
                if (timecode < current)
                {
                    d /= (float)m_Before;
                    color = glm::vec4(1.0, 0.0, 0.0, 1.0 - d);
                }
                else
                {
                    d /= (float) m_After;
                    color = glm::vec4(d, 1.0,  d, 1.0);
                }

                if (timecode == lastTimecode)
                    simultaneous += 1.0f;
                else
                    simultaneous = 0.0;

                transformation.push();

                cursor.x = m_Dimension.x * ((float)timecode - (float) current + (float) m_Before) / ((float)m_Before + (float)m_After);
                cursor.y = m_Dimension.y / 2 + simultaneous * m_Dimension.y / 5;

                transformation.translate(cursor);
                m_EventIcon->draw(context, viewProjection * model * transformation.state(), color, mode);

                transformation.pop();

                lastTimecode = timecode;
                ++eit;
            }
        }
    }

    inline void bindTrack(Track* track) { m_Track = track; setDirty(); }

    inline void setDirty() { m_NeedsEventCursorUpdate = true; }

private:
    Buffer m_VertexBuffer;
    Shader::Program* m_Shader;
    Icon* m_EventIcon;
    Timecode m_Before;
    Timecode m_After;
    Track* m_Track;
    Timecode m_LastCurrentTimecode;
    Track::EventIterator m_EventCursor;
    bool m_NeedsEventCursorUpdate;
};
