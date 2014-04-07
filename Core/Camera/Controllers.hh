#pragma once

#include "Core/Headers.hh"
#include "Core/Camera/Camera.hh"

class ICameraController : public Controller
{
public:
	virtual void bind(Context* context, Camera* camera) = 0;
	virtual void updateCamera() = 0;
	virtual void onMouseClick(int x, int y) { (void) x; (void) y; }
	virtual void onMouseDoubleClick(int x, int y) { (void) x; (void) y; }
	virtual void onMouseTripleClick(int x, int y) { (void) x; (void) y; }
	virtual void idle() {}
	virtual void reshape(int width, int height) = 0;
};

class SphericalCameraController : public ICameraController
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

	inline void setTarget(const glm::vec3& target) { m_Target = target; }

	inline void setRadius(const float radius) { m_Radius = radius; }

	virtual void updateCamera()
	{
		Timecode time = m_Context->clock().milliseconds();

		float dt = (float)(time - m_LastTime) / 1000.0f;

		m_Angles[0] += m_Acceleration.x * dt;
		m_Angles[1] += m_Acceleration.y * dt;
		m_Radius += m_Acceleration.z * dt;

		const float c_min_radius = 0.001;
		if (m_Radius < c_min_radius)
			m_Radius = c_min_radius;

		m_Angles[0] = fmod(m_Angles[0], 2 * M_PI);
		if (m_Angles[1] >= M_PI) { m_Angles[1] = M_PI - 0.001; }
		if (m_Angles[1] <= M_PI / 10) { m_Angles[1] = M_PI / 10 + 0.001; }

		m_Camera->lookAt(
			glm::vec3(
				m_Target.x + m_Radius * sin(m_Angles[1]) * cos(m_Angles[0]),
				m_Target.y + m_Radius * cos(m_Angles[1]),
				m_Target.z + m_Radius * sin(m_Angles[1]) * sin(m_Angles[0])),
			m_Target,
			glm::vec3(0, 1, 0));

		m_LastTime = time;
	}

	virtual void onSpecial(int key, Controller::KeyEvent event)
	{
		if (key == GLUT_KEY_LEFT)
			m_Acceleration.x = (event == Controller::KEY_DOWN ?  -1.0f : 0.0f);
		else if (key == GLUT_KEY_RIGHT)
			m_Acceleration.x = (event == Controller::KEY_DOWN ?   1.0f : 0.0f);
		else if (key == GLUT_KEY_UP)
			m_Acceleration.z = (event == Controller::KEY_DOWN ? -20.0f : 0.0f);
		else if (key == GLUT_KEY_DOWN)
			m_Acceleration.z = (event == Controller::KEY_DOWN ?  20.0f : 0.0f);
	}
	virtual void reshape(int width, int height)
	{
		m_Camera->reshape(width, height);
	}
	virtual void onKeyboard(unsigned char key, Controller::KeyEvent event)
	{
		(void) key;

		if (event == Controller::KEY_DOWN)
		{
		}
		else if (event == Controller::KEY_UP)
		{
		}
	}
	virtual void onMouseDown(int x, int y)
	{
		(void) x;
		(void) y;
		m_AnglesCopy[0] = m_Angles[0];
		m_AnglesCopy[1] = m_Angles[1];
	}
	virtual void onMouseMove(int x, int y, int dx, int dy)
	{
		(void) x;
		(void) y;
		m_Angles[0] = m_AnglesCopy[0] + (float) dx / 50.0;
		m_Angles[1] = m_AnglesCopy[1] + (float) dy / 50.0;
	}

	void playZoomSequence(glm::vec3 newTarget, float newRadius, unsigned int time)
	{
		m_Context->sequencer().track("animation")->insert(new VertexSequence(&m_Target, newTarget, time), Track::Event::START);
		m_Context->sequencer().track("animation")->insert(new FloatSequence(&m_Radius, newRadius, time), Track::Event::START);
	}

	void playAimSequence(glm::vec3 newTarget, unsigned int time)
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

protected:
	Camera* m_Camera;
	Context* m_Context;
	Timecode m_LastTime;

	GLfloat m_Angles[2];
	GLfloat m_AnglesCopy[2];
	GLfloat m_Radius;
	glm::vec3 m_Target;
	glm::vec3 m_Acceleration;
};

class FirstPersonCameraController : public ICameraController
{
public:
	virtual void bind(Context* context, Camera* camera)
	{
	    m_Context = context;
		m_Camera = camera;

		m_LastDx = 0;
		m_LastDy = 0;

		m_LastTime = m_Context->clock().milliseconds();

		updateCamera();
	}
	virtual void updateCamera()
	{
		Timecode time = m_Context->clock().milliseconds();

		float dt = (float)(time - m_LastTime) / 1000.0f;

		m_Camera->move(m_Acceleration * dt);
		m_LastTime = time;
	}
	virtual void reshape(int width, int height)
	{
		m_Camera->reshape(width, height);
	}
	virtual void onKeyboard(unsigned char key, Controller::KeyEvent event)
	{
		if (event == Controller::KEY_DOWN)
		{
			if (key == ' ')
				m_Acceleration = glm::vec3(0, 0, 0);
		}
	}
	virtual void onSpecial(int key, Controller::KeyEvent event)
	{
		if (key == GLUT_KEY_UP)
			m_Acceleration.z = (event == KEY_DOWN ?  20.0 : 0.0);
		else if (key == GLUT_KEY_DOWN)
			m_Acceleration.z = (event == KEY_DOWN ? -20.0 : 0.0);
		else if (key == GLUT_KEY_RIGHT)
			m_Acceleration.x = (event == KEY_DOWN ?  20.0 : 0.0);
		else if (key == GLUT_KEY_LEFT)
			m_Acceleration.x = (event == KEY_DOWN ? -20.0 : 0.0);
	}
	virtual void onMouseDown(int x, int y)
	{
		(void) x;
		(void) y;
		m_LastDx = 0;
		m_LastDy = 0;
	}
	virtual void onMouseMove(int x, int y, int dx, int dy)
	{
		(void) x;
		(void) y;
		m_Camera->rotate((float)(dx - m_LastDx) / 2, (float)(dy - m_LastDy) / 2, 0);
		m_LastDx = dx;
		m_LastDy = dy;
	}
	void sequence(glm::vec3 newTarget, float newRadius, unsigned int time)
	{
	    m_Context->sequencer().track("animation")->insert(new VertexSequence(&m_Camera->getPosition(), newTarget - newRadius * m_Camera->front(), time), Track::Event::START);
	}

protected:
	Context* m_Context;
	Camera* m_Camera;

	Timecode m_LastTime;
	int m_LastDx;
	int m_LastDy;
	glm::vec3 m_Acceleration;
};
