#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/GUI/Viewport.hh>

class View
{
public:
    View()
    : m_Viewport(glm::vec2(0, 0), glm::vec2(0, 0))
    {
    }

	virtual ~View() {};

	virtual const char* name() const = 0;

	virtual void reshape(int width, int height)
	{
	    // TODO : Reshape viewport proportionally
	}

	virtual void draw() = 0;

	virtual void idle() = 0;

    inline void setViewport(const Viewport& viewport) { m_Viewport = viewport; }
    inline const Viewport& getViewport() const { return m_Viewport; }

protected:
	Viewport m_Viewport;
};

