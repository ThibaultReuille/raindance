#pragma once

#include "Core/Clock.hh"
#include "Core/Scheduler.hh"
#include "Core/Sequencer/Sequencer.hh"
#include "Core/Geometry.hh"
#include "Core/Log.hh"
#include "Core/View.hh"
#include "Core/Camera/Camera.hh"

class Context
{
public:
	Context()
	: m_Camera(NULL)
	{
	}

	inline Clock& clock() { return m_Clock; }
	inline MessageQueue& messages() { return m_MessageQueue; }
	inline Sequencer& sequencer() { return m_Sequencer; }
	inline Geometry& geometry() { return m_Geometry; }

	inline void setCamera(Camera* camera) { m_Camera = camera; }
	inline Camera* getCamera() { return m_Camera; }

private:
	Clock m_Clock;
	MessageQueue m_MessageQueue;
	Sequencer m_Sequencer;
	Geometry m_Geometry;

	std::vector<View*> m_Views;

	Camera* m_Camera;
};

