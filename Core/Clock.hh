#pragma once

#include <sys/time.h>

#include "Core/Headers.hh"

class Clock
{
public:
	Clock()
	{
		reset();
	}

	void reset()
	{
		gettimeofday(&m_StartTime, NULL);
	}

	unsigned long milliseconds() const
	{
		timeval t;

		gettimeofday(&t, NULL);

		return (t.tv_sec - m_StartTime.tv_sec) * 1000 + (t.tv_usec - m_StartTime.tv_usec) / 1000;
	}

	unsigned long microseconds() const
	{
		timeval t;

		gettimeofday(&t, NULL);

		return (t.tv_sec - m_StartTime.tv_sec) * 1000000 + (t.tv_usec - m_StartTime.tv_usec);
	}

private:
	timeval m_StartTime;
};

