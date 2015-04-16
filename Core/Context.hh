#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Clock.hh>
#include <raindance/Core/Scheduler.hh>
#include <raindance/Core/Sequencer/Sequencer.hh>
#include <raindance/Core/Geometry.hh>
#include <raindance/Core/Log.hh>
#include <raindance/Core/Camera/Camera.hh>

#ifdef RD_OCULUS_RIFT
# include <raindance/Core/VR/OculusRift.hh>
#endif

class Context
{
public:
	Context()
	: m_Camera(NULL)
	{
		#ifdef RD_OCULUS_RIFT
			m_Rift = new OculusRift();
		#endif		
	}

	virtual ~Context()
	{
		#ifdef RD_OCULUS_RIFT
			SAFE_DELETE(m_Rift);
		#endif	
	}

	inline Clock& clock() { return m_Clock; }
	inline MessageQueue& messages() { return m_MessageQueue; }
	inline Sequencer& sequencer() { return m_Sequencer; }
	inline Geometry& geometry() { return m_Geometry; }

	inline void setCamera(Camera* camera) { m_Camera = camera; }
	inline Camera* getCamera() { return m_Camera; }

	#ifdef RD_OCULUS_RIFT
		inline OculusRift* rift() { return m_Rift; }
	#endif		

private:
	Clock m_Clock;
	MessageQueue m_MessageQueue;
	Sequencer m_Sequencer;
	Geometry m_Geometry;
	Camera* m_Camera;

	#ifdef RD_OCULUS_RIFT
		OculusRift* m_Rift;
	#endif
};

