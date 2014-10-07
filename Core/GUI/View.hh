#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/GUI/Viewport.hh>

class View
{
public:
    View()
    {
        m_Viewport = NULL;
        m_RenderToTexture = NULL;
    }

	virtual ~View() {};

	virtual const char* name() const = 0;

	virtual void draw() = 0;

	virtual void idle() = 0;

    inline void setViewport(Viewport* viewport) { m_Viewport = viewport; }
    inline Viewport* getViewport() { return m_Viewport; }

    inline void setRenderToTexture(Texture* texture) { m_RenderToTexture = texture; }
    inline Texture* getRenderToTexture() { return m_RenderToTexture; }

protected:
	Viewport* m_Viewport;
	Texture* m_RenderToTexture;
};

