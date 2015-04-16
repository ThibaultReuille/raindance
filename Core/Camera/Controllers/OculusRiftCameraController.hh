#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Camera/Camera.hh>
#include <raindance/Core/Controller.hh>

class OculusRiftCameraController : public CameraControllerBase
{
public:

	void bind(Context* context, Camera* camera) override
	{
	    m_Context = context;
		m_Camera = camera;

		m_LastTime = m_Context->clock().milliseconds();

		#ifdef RD_OCULUS_RIFT
			context->rift()->configure(*m_Camera, OculusRift::LEFT);
		#endif
			
		update();
	}
	
	void update() override
	{
		#ifdef RD_OCULUS_RIFT
			glm::vec3 pos = glm::vec3(0.0, 0.0, 20.0);
			glm::vec3 target = glm::vec3(0.0, 0.0, 0.0);
			glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);

			m_Context->rift()->lookAt(OculusRift::LEFT, pos, target, up, *m_Camera);
			m_Context->rift()->idle();
		#else
			Timecode time = m_Context->clock().milliseconds();

			float dt = (float)(time - m_LastTime) / 1000.0f;

			m_Camera->move(m_Acceleration * dt);
			m_LastTime = time;
		#endif
	}

	void onResize(const Viewport& viewport)
	{
		auto dimension = viewport.getDimension();
		m_Camera->resize(dimension.x, dimension.y);
	}
	
	void onKey(int key, int scancode, int action, int mods) override
	{
		(void) scancode;
		(void) mods;

		bool isPressOrRepeat = action == GLFW_PRESS || action == GLFW_REPEAT;

		if (isPressOrRepeat && key == GLFW_KEY_SPACE)
			m_Acceleration = glm::vec3(0, 0, 0);
		else if (key == GLFW_KEY_UP)
			m_Acceleration.z = isPressOrRepeat ?  20.0 : 0.0;
		else if (key == GLFW_KEY_DOWN)
			m_Acceleration.z = isPressOrRepeat ? -20.0 : 0.0;
		else if (key == GLFW_KEY_RIGHT)
			m_Acceleration.x = isPressOrRepeat ?  20.0 : 0.0;
		else if (key == GLFW_KEY_LEFT)
			m_Acceleration.x = isPressOrRepeat ? -20.0 : 0.0;
	}

	void onScroll(double xoffset, double yoffset) override
	{
		(void) xoffset;
		m_Acceleration.z = yoffset;
	}

	void onMouseDown(const glm::vec2& pos) override
	{
		m_LastDownPosition = pos;
		m_LastDiff = glm::vec2(0, 0);
	}

	void onMouseMove(const glm::vec2& pos, const glm::vec2& dpos) override
	{
		(void) dpos;

		glm::vec2 diff = pos - m_LastDownPosition;
		glm::vec2 deltaDiff = diff - m_LastDiff;

		m_Camera->rotate(deltaDiff.x / 2, deltaDiff.y / 2, 0);
		m_LastDiff = diff;
	}

	void zoom(glm::vec3 newTarget, float newRadius, unsigned int time) override
	{
		auto sequence = new VertexSequence(m_Camera->getPositionPtr(), newTarget - newRadius * m_Camera->front(), time);
	    m_Context->sequencer().track("animation")->insert(sequence, Track::Event::START);
	}

protected:
	Context* m_Context;
	Camera* m_Camera;

	Timecode m_LastTime;
	glm::vec2 m_LastDelta;
	glm::vec2 m_LastDownPosition;
	glm::vec2 m_LastDiff;
	glm::vec3 m_Acceleration;
};