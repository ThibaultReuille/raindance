#pragma once

#include "Core/Headers.hh"

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

    enum Synchronization
    {
        INTERNAL,
        EXTERNAL
    };

    typedef typename std::multiset<Event>::iterator EventIterator;

    Track(const char* name)
    {
        m_Name = std::string(name);
        m_LastTimecode = 0;
        m_Synchronization = INTERNAL;
        m_ExternalClock = NULL;
    }

    virtual ~Track() {}


    virtual void insert(Sequence* sequence, Event::Type type)
    {
        m_Events.insert(Event(type, m_LastTimecode + 1, sequence));
    }

    virtual void insert(Sequence* sequence, Event::Type type, Timecode timecode)
    {
        m_Events.insert(Event(type, timecode, sequence));
    }

    virtual void insert(Sequence* sequence, Timecode start, Timecode duration)
    {
        m_Events.insert(Event(Event::START, start, sequence));
        m_Events.insert(Event(Event::STOP, start + duration, sequence));
    }

    void play()
    {
        if (m_Events.size() == 0)
            return;

        Timecode timecode = clock().milliseconds();

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
            // itprocess->dump();
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

    void setSynchronization(Synchronization synchronization)
    {
        if (synchronization == EXTERNAL && m_ExternalClock == NULL)
        {
            LOG("[TRACK] Can't set up external synchronization; external clock wasn't defined!\n");
            return;
        }

        m_Synchronization = synchronization;
    }

    inline void setExternalClock(Clock* clock) { m_ExternalClock = clock; }

    inline Clock& clock() { return m_Synchronization == INTERNAL ? m_InternalClock : *m_ExternalClock; }

    inline EventIterator events_begin() { return m_Events.begin(); }
    inline EventIterator events_from(Timecode timecode) { return m_Events.lower_bound(Event(Event::ONCE, timecode, NULL)); }
    inline EventIterator events_to(Timecode timecode) { return m_Events.upper_bound(Event(Event::ONCE, timecode, NULL)); }
    inline EventIterator events_end() { return m_Events.end(); }

private:
    std::string m_Name;
    std::multiset<Event> m_Events;

    Synchronization m_Synchronization;
    Clock m_InternalClock;
    Clock* m_ExternalClock;

    Timecode m_LastTimecode;
    std::set<Sequence*> m_Playlist;
};

