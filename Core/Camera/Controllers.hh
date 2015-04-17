#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Camera/Camera.hh>
#include <raindance/Core/Controller.hh>

class CameraControllerBase : public Controller
{
public:
	CameraControllerBase() : Controller() {}
	virtual void bind(Context* context, CameraVector* cameras) = 0;
	virtual void update() = 0;
	virtual void zoom(glm::vec3 newTarget, float newRadius, unsigned int time) = 0;
};

#include <raindance/Core/Camera/Controllers/FirstPersonCameraController.hh>
#include <raindance/Core/Camera/Controllers/SphericalCameraController.hh>
#include <raindance/Core/Camera/Controllers/OculusRiftCameraController.hh>

class CameraController : public CameraControllerBase
{
public:
	enum Type
	{
		FIRST_PERSON,
		SPHERICAL,
		OCULUS_RIFT
	};

	CameraController()
	: CameraControllerBase()
	{
		select(FIRST_PERSON);
	}

	void select(Type type)
	{	
		switch(type)
		{
			case FIRST_PERSON:
				m_ActiveController = &m_FirstPerson;
				break;
			case SPHERICAL:
				m_ActiveController = &m_Spherical;
				break;
			case OCULUS_RIFT:
				m_ActiveController = &m_OculusRift;
				break;
			default:
				m_ActiveController = NULL;
				break;
		}
	}

	void bind(Context* context, CameraVector* cameras) override
	{
		m_FirstPerson.bind(context, cameras);
		m_Spherical.bind(context, cameras);
		m_OculusRift.bind(context, cameras); 
	}

	void update() override
	{
		m_ActiveController->update();
	}
 
	void zoom(glm::vec3 newTarget, float newRadius, unsigned int time) override
	{
		m_ActiveController->zoom(newTarget, newRadius, time);
	}

	// ----- Events -----

	void onResize(const Viewport& viewport) override
	{
		m_ActiveController->onResize(viewport);
	}

	void onKey(int key, int scancode, int action, int mods) override
	{
		m_ActiveController->onKey(key, scancode, action, mods);
	}
	
	void onScroll(double xoffset, double yoffset) override
	{
		m_ActiveController->onScroll(xoffset, yoffset);
	}
	
	void onMouseMove(const glm::vec2& pos, const glm::vec2& dpos) override
	{
		m_ActiveController->onMouseMove(pos, dpos);
	}

	void onMouseDown(const glm::vec2& pos) override
	{
		m_ActiveController->onMouseDown(pos);
	}

	void onMouseClick(const glm::vec2& pos) override
	{
		m_ActiveController->onMouseClick(pos);
	}

	void onMouseDoubleClick(const glm::vec2& pos) override
	{
		m_ActiveController->onMouseDoubleClick(pos);
	}

	void onMouseTripleClick(const glm::vec2& pos) override
	{
		m_ActiveController->onMouseTripleClick(pos);
	}

	// ----- Accessors -----

	inline FirstPersonCameraController& getFirstPerson() { return m_FirstPerson; }
	inline SphericalCameraController& getSpherical() { return m_Spherical; }
	inline OculusRiftCameraController& getOculusRift() { return m_OculusRift; }

private:
	CameraControllerBase* m_ActiveController;

	FirstPersonCameraController m_FirstPerson;
	SphericalCameraController m_Spherical;
	OculusRiftCameraController m_OculusRift;
};