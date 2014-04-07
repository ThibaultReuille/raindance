#pragma once

#include "Core/Headers.hh"

#include "Core/Clock.hh"
#include "Core/Sequencer/Sequence.hh"
#include "Core/Sequencer/Track.hh"

class Sequencer
{
public:
	Sequencer()
    {
	    this->add(new Track("default"));
	    m_Delay = 1000;
    }
	virtual ~Sequencer()
	{
	    for (auto t : m_Tracks)
	    {
	        SAFE_DELETE(t);
	    }
	}

	void play()
	{
		for (auto track : m_Tracks)
		    track->play();
	}

	void play(const char* trackName)
	{
	    track(trackName)->play();
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
