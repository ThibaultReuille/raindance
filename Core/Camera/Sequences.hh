#pragma once

#include "Core/Camera/Camera.hh"

class CameraTargetSequence : public Sequence
{
public:
	CameraTargetSequence(Camera* camera, glm::vec3 target, unsigned int duration)
    : Sequence("CameraTarget")
	{
	    m_StartTime = 0;
		m_Iteration = 0;
		m_Duration = duration;
		m_Camera = camera;
		m_TargetValue = target;
	}

	virtual ~CameraTargetSequence()
	{
        m_Camera = NULL;
	}

	virtual void start(Timecode timecode)
	{
	    LOG("%lu > %s START\n", timecode, m_Name.c_str());
	    m_StartTime = timecode;
	    m_InitialValue = m_Camera->getPosition() + m_Camera->front();
	}

	virtual Status play(Timecode timecode)
	{
		Timecode deltaTime = timecode - m_StartTime;

		float t = deltaTime == 0 ? 0 : (float)deltaTime / (float)m_Duration;
		if (t >= 1)
		{
			m_Camera->lookAt(m_TargetValue);
	        LOG("%lu > %s KILL\n", timecode, m_Name.c_str());
			return KILL;
		}

		m_Camera->lookAt((1 - t) * m_InitialValue + t * m_TargetValue);

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
	Camera* m_Camera;
	glm::vec3 m_InitialValue;
	glm::vec3 m_TargetValue;
};
