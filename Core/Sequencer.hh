#pragma once

#include "Core/Headers.hh"

typedef unsigned long Timecode;

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

class Track
{
public:
    class Event
    {
    public:
        enum Type
        {
            START,
            STOP,
            ONCE,
        };

        Event(Type type, Timecode timecode, Sequence* sequence) : m_Type(type), m_Timecode(timecode), m_Sequence(sequence) {}

        virtual ~Event() {}

        void dump () const
        {
            LOG("[EVENT] Timecode: %lu, %s %s\n",
                    m_Timecode,
                    m_Sequence->name().c_str(),
                    m_Type == Event::START ? "START" : (m_Type == Event::STOP ? "STOP" : "ONCE"));
        }

        inline bool operator< (const Event& other) const { return m_Timecode < other.timecode(); }

        inline Type type() const { return m_Type; }
        inline Timecode timecode() const { return m_Timecode; }
        inline Sequence* sequence() const { return m_Sequence; }

    private:
        Type m_Type;
        Timecode m_Timecode;
        Sequence* m_Sequence;
    };

    Track(const char* name) : m_Name(name), m_LastTimecode(0) {}

    virtual ~Track() {}

    virtual void insert(Sequence* sequence, Event::Type type, Timecode timecode)
    {
        m_Sequences.insert(sequence);
        m_Events.insert(Event(type, timecode, sequence));
    }

    virtual void insert(Sequence* sequence, Timecode start, Timecode duration)
    {
        m_Sequences.insert(sequence);
        m_Events.insert(Event(Event::START, start, sequence));
        m_Events.insert(Event(Event::STOP, start + duration, sequence));
    }

    void play(Timecode timecode)
    {
        if (m_Events.size() == 0)
            return;

        if (timecode < m_LastTimecode)
        {
            LOG("[TRACK] Current timecode is lower than previous one. Please rewind.\n");
            return;
        }

        // Search Event Bounds
        std::multiset<Event>::iterator itlow;
        std::multiset<Event>::iterator itup;
        itlow = m_Events.lower_bound(Event(Event::ONCE, m_LastTimecode + (m_LastTimecode == timecode ? 0 : 1), NULL));
        itup = m_Events.upper_bound(Event(Event::ONCE, timecode, NULL));

        // Process
        std::set<Sequence*> once;
        std::multiset<Event>::iterator itprocess;
        for(itprocess = itlow; itprocess != itup; ++itprocess)
        {
            LOG("PROCESS : "); itprocess->dump();
            switch(itprocess->type())
            {
            case Event::START:
                m_Playlist.insert(itprocess->sequence());
                itprocess->sequence()->start(timecode);
                break;
            case Event::STOP:
                m_Playlist.erase(itprocess->sequence());
                itprocess->sequence()->stop(timecode);
                break;
            case Event::ONCE:
                once.insert(itprocess->sequence());
                break;
            default:
                break;
            }
        }

        // Play
        std::set<Sequence*>::iterator itplay;
        for (itplay = once.begin(); itplay != once.end(); ++itplay)
            (*itplay)->play(timecode);
        for (itplay = m_Playlist.begin(); itplay != m_Playlist.end();)
        {
            if ((*itplay)->play(timecode) == Sequence::KILL)
            {
                m_Playlist.erase(itplay++);
            }
            else
                ++itplay;
        }

        // Update
        m_LastTimecode = timecode;
    }

    inline void play(Clock& clock)
    {
        this->play(clock.milliseconds());
    }


    void dump()
    {
        std::multiset<Event>::iterator it;
        LOG("[TRACK] %s :\n", m_Name.c_str());
        for (it = m_Events.begin(); it != m_Events.end(); ++it)
        {
            it->dump();
        }
    }

    inline const std::string& name() { return m_Name; }

private:
    std::string m_Name;
    std::multiset<Event> m_Events;
    std::set<Sequence*> m_Sequences;

    Timecode m_LastTimecode;
    std::set<Sequence*> m_Playlist;
};

class Sequencer
{
public:
	Sequencer()
    {
	    this->add(new Track("default"));
	    m_Delay = 100;
    }
	virtual ~Sequencer()
	{
	    for (auto t : m_Tracks)
	    {
	        SAFE_DELETE(t);
	    }
	}

    inline void add(const char* name, Sequence* sequence, Track::Event::Type type)
    {
        this->track(name)->insert(sequence, type, m_Clock.milliseconds() + m_Delay);
    }
    inline void add(const char* name, Sequence* sequence, Track::Event::Type type, Timecode timecode)
    {
        this->track(name)->insert(sequence, type, timecode);
    }

    inline void add(const char* name, Sequence* sequence, Timecode duration)
    {
        this->track(name)->insert(sequence, m_Clock.milliseconds() + m_Delay, duration);
    }
    inline void add(const char* name, Sequence* sequence, Timecode time, Timecode duration)
    {
        this->track(name)->insert(sequence, time, duration);
    }

	void play()
	{
		for (auto track : m_Tracks)
		    track->play(m_Clock.milliseconds());
	}

	void play(Timecode timecode)
	{
	    for (auto track : m_Tracks)
	        track->play(timecode);
	}

	void play(const char* trackName)
	{
	    track(trackName)->play(m_Clock.milliseconds());
	}

	void dump()
	{
	    for (auto track : m_Tracks)
	        track->dump();
	}

	inline void add(Track* track)
	{
	    for (auto t : m_Tracks)
	        if (track->name() == t->name())
	        {
	            LOG("[SEQUENCER] Track '%s' already exists!\n", track->name().c_str());
	            return;
	        }
	    m_Tracks.push_back(track);
	}

	Track* track(const char* name = "default")
	{
	    std::string sname(name);
	    for (auto track : m_Tracks)
	        if (track->name() == sname)
	            return track;
	    return NULL;
	}

	inline Clock& clock() { return m_Clock; }

protected:
	Clock m_Clock;
    std::vector<Track*> m_Tracks;
    unsigned int m_Delay;
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

