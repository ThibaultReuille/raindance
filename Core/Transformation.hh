#pragma once

#include <vector>

#include <raindance/Core/Headers.hh>

class Transformation
{
public:
	Transformation()
	{
		clear();
	}

	inline glm::mat4 state() const
	{
		return m_Stack[m_Stack.size() - 1];
	}

	inline glm::mat4* ptr()
	{
		return &m_Stack[m_Stack.size() - 1];
	}

	inline void push()
	{
		m_Stack.push_back(m_Stack[m_Stack.size() - 1]);
	}

	inline void pop()
	{
		m_Stack.pop_back();
	}

	inline void set(const glm::mat4& m)
	{
		m_Stack[m_Stack.size() - 1] = m;
	}

	void clear()
	{
		m_Stack.clear();
		m_Stack.push_back(glm::mat4());
	}

	inline void apply(glm::mat4& m)
	{
		m = m * state();
	}

	inline void translate(const glm::vec3& v)
	{
		m_Stack[m_Stack.size() - 1] = glm::translate(m_Stack[m_Stack.size() - 1], v);
	}

	inline void rotate(GLfloat angle, const glm::vec3& v)
	{
		m_Stack[m_Stack.size() - 1] = glm::rotate(m_Stack[m_Stack.size() - 1], glm::radians(angle), v);
	}

	inline void rotate(const glm::quat& rotation)
	{
		m_Stack[m_Stack.size() - 1] = m_Stack[m_Stack.size() - 1] * glm::toMat4(rotation);
	}

	inline void scale(glm::vec3 v)
	{
		m_Stack[m_Stack.size() - 1] = glm::scale(m_Stack[m_Stack.size() - 1], v);
	}

private:
	std::vector<glm::mat4> m_Stack;
};

