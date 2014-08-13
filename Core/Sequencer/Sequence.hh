#pragma once

#include <raindance/Core/Headers.hh>

class Sequence
{
public:
    typedef unsigned long ID;

    enum Status
    {
        LIVE,
        KILL
    };

    Sequence(const char* name) : m_Name(name)
    {
        static ID NextID = 0;
        m_ID = NextID++;
    }
    virtual ~Sequence() {}

    virtual void start(Timecode timecode) = 0;
    virtual Status play(Timecode timecode) = 0;
    virtual void stop(Timecode timecode) = 0;

    inline ID id() const { return m_ID; }
    inline const std::string& name() const { return m_Name; }

private:
    ID m_ID;
    std::string m_Name;
};

class VertexSequence : public Sequence
{
public:
    VertexSequence(glm::vec3* vertex, glm::vec3 target, Timecode duration)
    : Sequence("Vertex")
    {
        m_StartTime = 0;
        m_Iteration = 0;
        m_Duration = duration;
        m_Vertex = vertex;
        m_TargetValue = target;
    }

    virtual ~VertexSequence()
    {
        m_Vertex = NULL;
    }

    virtual void start(Timecode timecode)
    {
        m_StartTime = timecode;
        m_InitialValue = *m_Vertex;
        LOG("%lu > %s START\n", timecode, name().c_str());
    }

    virtual Status play(Timecode timecode)
    {
        Timecode deltaTime = timecode - m_StartTime;

        float t = deltaTime == 0 ? 0 : (float)deltaTime / (float)m_Duration;
        if (t >= 1)
        {
            *m_Vertex = m_TargetValue;
            LOG("%lu > %s KILL\n", timecode, name().c_str());
            return KILL;
        }

        *m_Vertex = (1 - t) * m_InitialValue + t * m_TargetValue;

        return LIVE;
    }

    virtual void stop(Timecode timecode)
    {
        (void) timecode;
    }

private:
    unsigned int m_Iteration;
    Timecode m_StartTime;
    Timecode m_Duration;
    glm::vec3* m_Vertex;
    glm::vec3 m_InitialValue;
    glm::vec3 m_TargetValue;
};

class FloatSequence : public Sequence
{
public:
    FloatSequence(float* f, float target, Timecode duration)
    : Sequence("Float")
    {
        m_StartTime = 0;
        m_InitialValue = 0;
        m_Iteration = 0;
        m_Duration = duration;
        m_Pointer = f;
        m_TargetValue = target;
    }

    virtual ~FloatSequence()
    {
        m_Pointer = NULL;
    }

    virtual void start(Timecode timecode)
    {
        LOG("%lu > %s START\n", timecode, name().c_str());
    }

    virtual Status play(Timecode timecode)
    {
        if (m_Iteration == 0)
        {
            m_StartTime = timecode;
            m_InitialValue = *m_Pointer;
        }

        Timecode deltaTime = timecode - m_StartTime;

        float t = deltaTime == 0 ? 0 : (float)deltaTime / (float)m_Duration;
        if (t >= 1)
        {
            *m_Pointer = m_TargetValue;
            LOG("%lu > %s KILL\n", timecode, name().c_str());
            return KILL;
        }

        *m_Pointer = (1 - t) * m_InitialValue + t * m_TargetValue;

        m_Iteration++;

        return LIVE;
    }

    virtual void stop(Timecode timecode)
    {
        (void) timecode;
    }

private:
    unsigned int m_Iteration;
    Timecode m_StartTime;
    Timecode m_Duration;
    float* m_Pointer;
    float m_InitialValue;
    float m_TargetValue;
};
