#pragma once

#include "Core/Clock.hh"
#include "Core/Scheduler.hh"
#include "Core/Sequencer.hh"
#include "Core/Geometry.hh"
#include "Core/Log.hh"

class Context
{
public:
	Context()
	: m_Pass(0)
	{
	}

	inline Clock& clock() { return m_Clock; }
	inline MessageQueue& messages() { return m_MessageQueue; }
	inline Sequencer& sequencer() { return m_Sequencer; }
	inline Geometry& geometry() { return m_Geometry; }

	inline void setPass(unsigned int pass) { m_Pass = pass; }
	inline unsigned int getPass() { return m_Pass; }

private:
	Clock m_Clock;
	MessageQueue m_MessageQueue;
	Sequencer m_Sequencer;
	Geometry m_Geometry;

	unsigned int m_Pass;
};

