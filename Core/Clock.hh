#pragma once

#include <sys/time.h>

#include <raindance/Core/Headers.hh>

typedef unsigned long Timecode;

class Clock
{
public:
    enum State
    {
        STARTED,
        PAUSED
    };

	Clock()
	{
	    gettimeofday(&m_ZeroTime, NULL);
        pause();
        reset();
		start();
        LOG("[CLOCK] Clock created at (%lu s, %lu us).\n", static_cast<unsigned long int>(m_ZeroTime.tv_sec), static_cast<unsigned long int>(m_ZeroTime.tv_usec));
	}

	void reset()
	{
		m_ElapsedTime.tv_sec = 0;
		m_ElapsedTime.tv_usec = 0;
	}

	void start()
	{
	    if (m_State == STARTED)
	        return;

	    m_State = STARTED;
	    gettimeofday(&m_LastTime, NULL);
	}

	void pause()
	{
	    m_State = PAUSED;
	}

    timeval timecode()
    {
        timeval tc;

        if (m_State == STARTED)
        {
            timeval t;
            gettimeofday(&t, NULL);

            m_ElapsedTime.tv_sec += t.tv_sec - m_LastTime.tv_sec;
            m_ElapsedTime.tv_usec += t.tv_usec - m_LastTime.tv_usec;

            m_LastTime = t;
        }

        tc.tv_sec = m_ElapsedTime.tv_sec;
        tc.tv_usec = m_ElapsedTime.tv_usec;

        return tc;
    }

    unsigned long milliseconds()
    {
        timeval tc = timecode();
		return tc.tv_sec * 1000 + tc.tv_usec / 1000;
	}

	unsigned long microseconds()
	{
	    timeval tc = timecode();
		return tc.tv_sec * 1000000 + tc.tv_usec;
	}

	inline float seconds()
	{
	    timeval tc = timecode();
	    return static_cast<float>(tc.tv_sec) + static_cast<float>(tc.tv_usec) / 1000000;
	}

	inline State state() { return m_State; }

private:
	timeval m_ZeroTime;
	timeval m_ElapsedTime;
    timeval m_LastTime;
	State m_State;
};

