#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Camera/Camera.hh>
#include <raindance/Core/Controller.hh>

class OculusRiftCameraController : public CameraControllerBase
{
public:

	void bind(Context* context, CameraVector* cameras) override
	{
		m_Context = context;
		m_Cameras = cameras;

		m_LastTime = m_Context->clock().milliseconds();

		#ifdef RD_OCULUS_RIFT
			m_Context->rift()->setPosition(m_Cameras->elements()[0]->getPosition());
		#endif
	}
	
	void update() override
	{
		Timecode time = m_Context->clock().milliseconds();

		float dt = (float)(time - m_LastTime) / 1000.0f;

		auto direction = m_Acceleration * dt;

		#ifdef RD_OCULUS_RIFT

			auto cam0 = m_Cameras->elements()[0];
			auto cam1 = m_Cameras->elements()[1];

			m_Context->rift()->configure(*cam0, OculusRift::LEFT);
			m_Context->rift()->configure(*cam1, OculusRift::RIGHT);

			static glm::vec3 _position(0.0, 0.0, 1.0);
			static glm::vec3 _target(0.0, 0.0, 0.0);
			static glm::vec3 _up(0.0, 1.0, 0.0);

			auto rift_pos = m_Context->rift()->getPosition();
			
			rift_pos += direction.x * cam0->right();
			rift_pos += direction.y * cam0->up();
			rift_pos += direction.z * cam0->front();

			m_Context->rift()->setPosition(rift_pos);

			m_Context->rift()->lookAt(OculusRift::LEFT, _position, _target, _up, *cam0);
			m_Context->rift()->lookAt(OculusRift::RIGHT, _position, _target, _up, *cam1);
	
			m_Context->rift()->idle();
		#else
			m_Cameras->move(direction);
		#endif
		
		m_LastTime = time;
	}

	void onResize(const Viewport& viewport)
	{
		auto dimension = viewport.getDimension();
		m_Cameras->resize(dimension.x, dimension.y);
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

		m_Cameras->rotate(deltaDiff.x / 2, deltaDiff.y / 2, 0);
		m_LastDiff = diff;
	}

	void zoom(glm::vec3 newTarget, float newRadius, unsigned int time) override
	{
		// TODO : This should use the IPD
		auto cam0 = m_Cameras->elements()[0];
		auto sequence1 = new VertexSequence(cam0->getPositionPtr(), newTarget - newRadius * cam0->front(), time);
	    m_Context->sequencer().track("animation")->insert(sequence1, Track::Event::START);
 		
 		auto cam1 = m_Cameras->elements()[1];
	   	auto sequence2 = new VertexSequence(cam1->getPositionPtr(), newTarget - newRadius * cam1->front(), time);
	    m_Context->sequencer().track("animation")->insert(sequence2, Track::Event::START);
	}

protected:
	Context* m_Context;
	CameraVector* m_Cameras;

	Timecode m_LastTime;
	glm::vec2 m_LastDelta;
	glm::vec2 m_LastDownPosition;
	glm::vec2 m_LastDiff;
	glm::vec3 m_Acceleration;
};