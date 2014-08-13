#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Intersection.hh>
#include <raindance/Core/Controller.hh>
#include <raindance/Core/Camera/Frustrum.hh>

class Camera
{
public:
	enum ProjectionMode
	{
		PERSPECTIVE,
		ORTHOGRAPHIC
	};

	Camera()
	{
		lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, .0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	}

	void lookAt(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up)
	{
		m_Position = position;

		m_AxisZ = position - target;
		m_AxisZ = glm::normalize(m_AxisZ);

		m_AxisX = glm::cross(up, m_AxisZ);
		m_AxisX = glm::normalize(m_AxisX);

		m_AxisY = glm::cross(m_AxisZ, m_AxisX);
		m_AxisY = glm::normalize(m_AxisY);

		m_View[0][0] = m_AxisX.x;
		m_View[1][0] = m_AxisX.y;
		m_View[2][0] = m_AxisX.z;
		m_View[3][0] = - glm::dot(m_AxisX, position);

		m_View[0][1] = m_AxisY.x;
		m_View[1][1] = m_AxisY.y;
		m_View[2][1] = m_AxisY.z;
		m_View[3][1] = - glm::dot(m_AxisY, position);

		m_View[0][2] = m_AxisZ.x;
		m_View[1][2] = m_AxisZ.y;
		m_View[2][2] = m_AxisZ.z;
		m_View[3][2] = - glm::dot(m_AxisZ, position);

		m_Orientation = glm::quat_cast(m_View);
	}

	void lookAt(const glm::vec3& target)
	{
		lookAt(m_Position, target, m_AxisY);
	}

	void move(const glm::vec3 direction)
	{
		m_Position += direction.x * right();
		m_Position += direction.y * up();
		m_Position += direction.z * front();
		updateViewMatrix();
	}

	void rotate(float yaw, float pitch, float roll)
	{
		glm::quat rotation;

		if (yaw != 0.0f)
		{
			rotation = glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0));
			m_Orientation = rotation * m_Orientation;
		}
		if (pitch != 0.0f)
		{
			rotation = glm::angleAxis(glm::radians(pitch), glm::vec3(1, 0, 0));
			m_Orientation = rotation * m_Orientation;
		}
		if (roll != 0.0f)
		{
			rotation = glm::angleAxis(glm::radians(roll), glm::vec3(0, 0, 1));
			m_Orientation = rotation * m_Orientation;
		}

		updateViewMatrix();
	}

	inline const glm::mat4& getViewMatrix() const { return m_View; }

	void updateViewMatrix()
	{
	    // Reconstruct the view matrix from orientation and position

	    m_View = glm::mat4_cast(m_Orientation);

	    m_AxisX = glm::vec3(m_View[0][0], m_View[1][0], m_View[2][0]);
	    m_AxisY = glm::vec3(m_View[0][1], m_View[1][1], m_View[2][1]);
	    m_AxisZ = glm::vec3(m_View[0][2], m_View[1][2], m_View[2][2]);

	    m_View[3][0] = -glm::dot(m_AxisX, m_Position);
	    m_View[3][1] = -glm::dot(m_AxisY, m_Position);
	    m_View[3][2] = -glm::dot(m_AxisZ, m_Position);
	}

	inline const glm::mat4& getProjectionMatrix() const { return m_Projection; }

	inline const glm::mat4 getViewProjectionMatrix() const { return m_Projection * m_View; }

	inline const glm::vec3& getPosition() const { return m_Position; }
	inline glm::vec3* getPositionPtr() { return &m_Position; }

	inline void setPerspectiveProjection(float fovy, float aspect, float near, float far)
	{
		m_ProjectionMode = PERSPECTIVE;
		m_FovY = fovy;
		m_Aspect = aspect;
		m_Near = near;
		m_Far = far;
		m_Projection = glm::perspective(glm::radians(fovy), aspect, near, far);
	}

	inline void setOrthographicProjection(float left, float right, float bottom, float top, float near, float far)
	{
		m_ProjectionMode = ORTHOGRAPHIC;
		m_Left = left;
		m_Right = right;
		m_Bottom = bottom;
		m_Top = top;
		m_NearVal = near;
		m_FarVal = far;
		m_Projection = glm::ortho(left, right, bottom, top, near, far);
	}

	// NOTES
	// . (sx, sy) belongs to ([0, Width], [Height, 0]) (Screen Space)
	// . Careful ! This code only works if front(), right() and up() are normalized
	Ray createRay(int screenX, int screenY)
	{
		glm::vec3 middle = m_Position + front();

		// Rescale between [-0.5, 0.5]
		float sx = 2 * ((float)screenX / (float)m_Width) - 1;
		float sy = 2 * (1 - (float)screenY / (float)m_Height) - 1;

		float f = tan(glm::radians(m_FovY) / 2);

		// Origin = lower left
		glm::vec3 pos = middle + sx * m_Ratio * f * right() + sy * f * up();

		return Ray(m_Position, glm::normalize(pos - m_Position));
	}

	void reshape(int width, int height)
	{
		m_Width = width;
		m_Height = height;
		m_Ratio = (float) width / (float) height;

		glViewport(0, 0, (GLint)m_Width, (GLint)m_Height);

		switch(m_ProjectionMode)
		{
		case PERSPECTIVE:
			this->setPerspectiveProjection(60.0f, m_Ratio, 0.1f, 1024.0f);
			break;
		case ORTHOGRAPHIC:
			this->setOrthographicProjection(m_Left, m_Right, m_Bottom, m_Top, m_NearVal, m_FarVal);
			break;
		};
	}

	glm::vec3 front() const { return -m_AxisZ; }
	glm::vec3 back() const { return  m_AxisZ; }
	glm::vec3 left() const { return -m_AxisX; }
	glm::vec3 right() const { return  m_AxisX; }
	glm::vec3 up() const { return  m_AxisY; }
	glm::vec3 down() const { return -m_AxisY; }

	inline ProjectionMode mode() const { return m_ProjectionMode; }

private:
	int m_Height;
	int m_Width;
	float m_Ratio;
	ProjectionMode m_ProjectionMode;

	glm::vec3 m_Position;
	glm::quat m_Orientation;
	glm::mat4 m_View;
	glm::mat4 m_Projection;

	glm::vec3 m_AxisX;
	glm::vec3 m_AxisY;
	glm::vec3 m_AxisZ;

	// Perspective Camera
	float m_FovY;
	float m_Aspect;
	float m_Near;
	float m_Far;

	// Orthographic Camera
	float m_Left;
	float m_Right;
	float m_Bottom;
	float m_Top;
	float m_NearVal;
	float m_FarVal;
};
