#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/GUI/Viewport.hh>

class View
{
public:
    View()
    {
        m_Viewport = NULL;
    }

	virtual ~View() {};

	virtual const char* name() const = 0;

	virtual void draw() = 0;

	virtual void idle() = 0;

    inline void setViewport(Viewport* viewport) { m_Viewport = viewport; }
    inline Viewport* getViewport() { return m_Viewport; }

protected:
	Viewport* m_Viewport;
};

