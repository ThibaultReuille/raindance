#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Camera/Camera.hh>
#include <raindance/Core/Controller.hh>

class SphericalCameraController : public CameraControllerBase
{
public:
	virtual void bind(Context* context, Camera* camera)
	{
        m_Context = context;
        m_Camera = camera;

		m_Angles[0] = 0;
		m_Angles[1] = M_PI / 2;
		m_Radius = 50;
		m_Target = glm::vec3(0, 0, 0);

		m_Acceleration = glm::vec3(0, 0, 0);

		m_LastTime = m_Context->clock().milliseconds();

		updateCamera();
	}

	virtual void updateCamera()
	{
		float time = m_Context->clock().seconds();
		float dt = time - m_LastTime;
		m_LastTime = time;

		m_Angles[0] += m_Acceleration.x * dt;
		m_Angles[1] += m_Acceleration.y * dt;
		m_Radius += m_Acceleration.z * dt;

		const float c_min_radius = 0.001;
		if (m_Radius < c_min_radius)
			m_Radius = c_min_radius;

		m_Angles[0] = fmod(m_Angles[0], 2 * M_PI);
		if (m_Angles[1] >= M_PI)
			m_Angles[1] = M_PI - 0.001;
		if (m_Angles[1] <= M_PI / 10) 
			m_Angles[1] = M_PI / 10 + 0.001;

		m_Camera->lookAt(
			glm::vec3(
				m_Target.x + m_Radius * sin(m_Angles[1]) * cos(m_Angles[0]),
				m_Target.y + m_Radius * cos(m_Angles[1]),
				m_Target.z + m_Radius * sin(m_Angles[1]) * sin(m_Angles[0])),
			m_Target,
			glm::vec3(0, 1, 0));
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

		// TODO : Make accessors to these 
		float rotation = 0.5f;
		float zoom = 20.0f;

		if (key == GLFW_KEY_LEFT)
			m_Acceleration.x = isPressOrRepeat ?  -rotation : 0.0f;
		else if (key == GLFW_KEY_RIGHT)
			m_Acceleration.x = isPressOrRepeat ?   rotation : 0.0f;
		else if (key == GLFW_KEY_UP)
			m_Acceleration.z = isPressOrRepeat ? -zoom : 0.0f;
		else if (key == GLFW_KEY_DOWN)
			m_Acceleration.z = isPressOrRepeat ?  zoom : 0.0f;
	}
	
	void onScroll(double xoffset, double yoffset) override
	{
		(void) xoffset;
		m_Acceleration.z = yoffset;
	}

	void onMouseDown(const glm::vec2& pos) override
	{
		m_LastDownPosition = pos;

		m_AnglesCopy[0] = m_Angles[0];
		m_AnglesCopy[1] = m_Angles[1];
	}

	void onMouseMove(const glm::vec2& pos, const glm::vec2& dpos) override
	{
		(void) dpos;

		glm::vec2 diff = pos - m_LastDownPosition;

		m_Angles[0] = m_AnglesCopy[0] + diff.x / 50.0;
		m_Angles[1] = m_AnglesCopy[1] + diff.y / 50.0;
	}

	void zoom(glm::vec3 newTarget, float newRadius, unsigned int time) override
	{
		m_Context->sequencer().track("animation")->insert(new VertexSequence(&m_Target, newTarget, time), Track::Event::START);
		m_Context->sequencer().track("animation")->insert(new FloatSequence(&m_Radius, newRadius, time), Track::Event::START);
	}

	void aim(glm::vec3 newTarget, unsigned int time)
	{
		float newRadius = glm::length(m_Camera->getPosition() - newTarget);
		glm::vec3 newPos = m_Camera->getPosition() - newTarget;
		float newAngle0 = acos(newPos.y / newRadius);
		float newAngle1 = atan(-newPos.z / newPos.x);

		m_Context->sequencer().track("animation")->insert(new VertexSequence(&m_Target, newTarget, time),    Track::Event::START);
		m_Context->sequencer().track("animation")->insert(new FloatSequence(&m_Radius, newRadius, time),     Track::Event::START);
		m_Context->sequencer().track("animation")->insert(new FloatSequence(&m_Angles[0], newAngle0, time),  Track::Event::START);
		m_Context->sequencer().track("animation")->insert(new FloatSequence(&m_Angles[1], newAngle1, time),  Track::Event::START);
	}

	inline void setTarget(const glm::vec3& target) { m_Target = target; }

	inline void setRadius(const float radius) { m_Radius = radius; }

protected:
	Camera* m_Camera;
	Context* m_Context;
	float m_LastTime;

	GLfloat m_Angles[2];
	GLfloat m_AnglesCopy[2];
	glm::vec2 m_LastDownPosition;

	GLfloat m_Radius;
	glm::vec3 m_Target;
	glm::vec3 m_Acceleration;
};
