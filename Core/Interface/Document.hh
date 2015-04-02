#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Transformation.hh>
#include <raindance/Core/Controller.hh>
#include <raindance/Core/Interface/Box.hh>
#include <raindance/Core/GUI/Viewport.hh> // TODO: Move Viewport.hh inside Interface/

class Document : public Controller
{
public:

	enum PickRegion
	{
		OUTSIDE = -1,
		MARGIN = 0,
		BORDER = 1,
		PADDING = 2,
		CONTENT = 3
	};

	Document(const glm::vec3& dimension = glm::vec3(0.0, 0.0, 0.0))
	{
		content().X = glm::vec2(0, dimension.x);
		content().Y = glm::vec2(0, dimension.y);
		content().Z = glm::vec2(0, dimension.z);
		content().Color = glm::vec4(WHITE, 1.0);

		padding().X = glm::vec2(0, 0); 
		padding().Y = glm::vec2(0, 0); 
		padding().Z = glm::vec2(0, 0); 
		padding().Color = glm::vec4(WHITE, 0.0);

		border().X = glm::vec2(0, 0); 
		border().Y = glm::vec2(0, 0); 
		border().Z = glm::vec2(0, 0); 
		border().Color = glm::vec4(BLACK, 1.0);

		margin().X = glm::vec2(0, 0); 
		margin().Y = glm::vec2(0, 0); 
		margin().Z = glm::vec2(0, 0); 
		margin().Color = glm::vec4(WHITE, 0.0);

		m_Refresh = true;
		m_Clicks = 0;
	}

	virtual ~Document()
	{
	}

	virtual void setVirtualPosition(const Viewport& viewport, rd::Box& box, const glm::vec2& vpos)
	{
	}

	virtual void setVirtualDimension(const Viewport& viewport, rd::Box& box, const glm::vec2& vdim)
	{
		auto framebuffer = viewport.getFramebuffer();

        box.X = glm::vec2(0, framebuffer.Width * vdim.x);
        box.Y = glm::vec2(0, framebuffer.Height * vdim.y);
        box.Z = glm::vec2(0, 0);
	}

	virtual void draw() {}

	virtual void idle() {}

	virtual Document* pick(const glm::vec2& position, PickRegion* region)
	{
		auto box_position = glm::vec2(0, 0);
		auto box_dimension = getDimension().xy();

		for (int i = 0; i < 4; i++)
		{
			if (position.x < box_position.x || position.x >= box_dimension.x ||
				position.y < box_position.y || position.y >= box_dimension.y)
			{
				auto r = static_cast<PickRegion>(i - 1);
				if (region != NULL)
					*region = r;
				return r == OUTSIDE ? NULL : this;
			}

			box_position += glm::vec2(m_Boxes[i].X[0], m_Boxes[i].Y[0]);
			box_dimension -= glm::vec2(m_Boxes[i].getWidth(), m_Boxes[i].getHeight());
		}

		if (region != NULL)
			*region = CONTENT;
		return this;
	}

	virtual void onResize(const Viewport& viewport)
	{
		LOG("[DEBUG] Document::onResize(window: %f x %f, framebuffer: %i x %i)\n",
			viewport.getDimension().x, viewport.getDimension().y,
			viewport.getFramebuffer().Width, viewport.getFramebuffer().Height);
	}

	void onMouseClick(const glm::vec2& pos) override
	{
		PickRegion region;

		pick(pos,&region);

		if (region == OUTSIDE || region == PADDING)
			return;

		m_Clicks++;

		if (m_Clicks % 2 == 1)
			border().Color = glm::vec4(ORANGE, 1.0);
		else
			border().Color = content().Color;
	}

	inline rd::Box& box(int i) { return m_Boxes[i]; }

	inline rd::Box& margin() { return m_Boxes[0]; }
	inline rd::Box& border() { return m_Boxes[1]; }
	inline rd::Box& padding() { return m_Boxes[2]; }
	inline rd::Box& content() { return m_Boxes[3]; }

	inline const glm::vec3& position() { return m_Position; }
	inline void position(const glm::vec3& position) { m_Position = position; }

	virtual glm::vec3 getDimension()
	{
		return glm::vec3(
			content().getWidth() + padding().getWidth() + border().getWidth() + margin().getWidth(),
			content().getHeight() + padding().getHeight() + border().getHeight() + margin().getHeight(),
			content().getDepth() + padding().getDepth() + border().getDepth() + margin().getDepth()
		);
	}

	inline bool refresh() { return m_Refresh; }
	inline void refresh(bool r) { m_Refresh = r; }

protected:
	int m_Clicks;
	bool m_Refresh;
	glm::vec3 m_Position;
	rd::Box m_Boxes[4]; // Margin, Border, Padding, Content
};

class DocumentCollection : public Document
{
public:
	DocumentCollection(const glm::vec3& dimension = glm::vec3(0, 0, 0))
	: Document(dimension)
	{
		m_Hover = NULL;
	}

	virtual ~DocumentCollection()
	{
	}

	virtual void arrange(glm::vec3 cursor, glm::vec3 dimension)
	{
        auto max_width = 0;

        for (auto doc : m_Elements)
        {
            auto doc_dim = doc->getDimension();

            if (doc_dim.x > max_width)
                max_width = doc_dim.x;

            if (cursor.y + doc_dim.y > dimension.y)
            {
                cursor.x += max_width;
                cursor.y = 0;
                max_width = 0;
            }

            doc->position(cursor);

            cursor.y += doc_dim.y;
        }
	}

	void draw() override
	{
		for (auto doc : m_Elements)
			doc->draw();
	}

	void idle() override
	{
		for (auto doc : m_Elements)
			doc->idle();
	}

	Document* pick(const glm::vec2& position, PickRegion* region) override
	{
		for (auto doc : m_Elements)
        {
            auto pick = doc->pick(position - doc->position().xy(), region);
            if (pick != NULL)
            	return pick;
        }
        if (region != NULL)
        	*region = OUTSIDE;

        return NULL;
	}

    void onResize(const Viewport& viewport) override
    {
       	for (auto doc : m_Elements)
			doc->onResize(viewport);
    } 

	void onMouseDown(const glm::vec2& pos) override
	{
		auto doc = pick(pos, NULL);
		if (doc != NULL)
			doc->onMouseDown(pos - doc->position().xy());
	}

	void onMouseClick(const glm::vec2& pos) override
	{ 
		auto doc = pick(pos, NULL);
		if (doc != NULL)
			doc->onMouseClick(pos - doc->position().xy());
	}

	void onMouseDoubleClick(const glm::vec2& pos) override
	{
		auto doc = pick(pos, NULL);
		if (doc != NULL)
			doc->onMouseDoubleClick(pos - doc->position().xy());
	}

	void onMouseTripleClick(const glm::vec2& pos) override
	{ 
		auto doc = pick(pos, NULL);
		if (doc != NULL)
			doc->onMouseTripleClick(pos - doc->position().xy());
	}

	void onMouseMove(const glm::vec2& pos, const glm::vec2& dpos) override
	{
		auto doc = pick(pos, NULL);
		if (doc != NULL)
			doc->onMouseMove(
				pos - doc->position().xy(),
				dpos - doc->position().xy());
	}

	void onKey(int key, int scancode, int action, int mods) override
	{
		// TODO: Route keys to the active element
		for (auto doc : m_Elements)
			doc->onKey(key, scancode, action, mods);
	}

    void onScroll(double xoffset, double yoffset) override
    {
    	// TODO: Route scroll to the active element
		for (auto doc : m_Elements)
			doc->onScroll(xoffset, yoffset);
    }

	bool refresh()
	{ 
		for (auto doc : m_Elements)
			if (doc->refresh())
				return true;
		return false;
	}

	inline std::vector<Document*>& elements() { return m_Elements; }

protected:
	Document* m_Hover;
	std::vector<Document*> m_Elements;
};