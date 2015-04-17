#pragma once

#include <raindance/Core/Camera/Camera.hh>

class CameraVector : ICamera
{
public:

	CameraVector()
	{
	}

	virtual ~CameraVector()
	{
		// TODO: This segfaults because Camera elements are stored in several vectors
		for (auto element : m_Elements)
		{
			SAFE_DELETE(element);
		}
	}

	int push(Camera* element)
	{
		m_Elements.push_back(element);
		return m_Elements.size() - 1;
	}

	void resize(int width, int height) override
	{
		for (auto element : m_Elements)
			element->resize(width, height);
	}

	void lookAt(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up) override
	{
		for (auto element : m_Elements)
			element->lookAt(position, target, up);
	}

	void move(const glm::vec3 direction) override
	{
		for (auto element : m_Elements)
			element->move(direction);
	}

 	void rotate(const glm::quat& rotation) override
	{
		for (auto element : m_Elements)
			element->rotate(rotation);
	}

	void rotate(float yaw, float pitch, float roll) override
	{
		for (auto element : m_Elements)
			element->rotate(yaw, pitch, roll);
	}

	void update() override
	{
		for (auto element : m_Elements)
			element->update();
	}

	void setPerspectiveProjection(float fovy, float aspect, float nearValue, float farValue) override
	{
		for (auto element : m_Elements)
			element->setPerspectiveProjection(fovy, aspect, nearValue, farValue);
	}

	void setOrthographicProjection(float left, float right, float bottom, float top, float nearVal, float farVal) override
	{
		for (auto element : m_Elements)
			element->setOrthographicProjection(left, right, bottom, top, nearVal, farVal);
	}

	inline Camera* operator[](std::size_t nth) { return m_Elements[nth]; }

	inline std::vector<Camera*>& elements() { return m_Elements; }

private:
	std::vector<Camera*> m_Elements;
};