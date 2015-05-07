#pragma once

#include <raindance/Core/Interface/Document.hh>

namespace Document
{

class Group : public Node
{
public:
	Group(Node* parent = NULL)
	: Node(parent)
	{
	}

	virtual ~Group()
	{
	}

	/* TODO: Reactivate when relevant
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
	*/

	void draw(Context* context) override
	{
	    glEnable(GL_SCISSOR_TEST);

		for (auto doc : m_Elements)
		{
	        auto position = doc->position() + glm::vec3(
	            doc->margin().left() + doc->border().left() + doc->padding().left(),
	            doc->margin().bottom() + doc->border().bottom() + doc->padding().bottom(),
	            0);

		    glViewport(position.x, position.y, doc->content().getWidth(), doc->content().getHeight());
		    glScissor(position.x, position.y, doc->content().getWidth(), doc->content().getHeight());

		    doc->draw(context);
		}

    	glDisable(GL_SCISSOR_TEST);
	}

	void idle(Context* context) override
	{
		for (auto doc : m_Elements)
			doc->idle(context);
	}

	void update() override
	{
		Node::update();

		for (auto doc : m_Elements)
			doc->update();
	}

	Node* pick(const glm::vec2& position, PickRegion* region) override
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
			doc->onMouseMove(pos - doc->position().xy(), dpos - doc->position().xy());
	}

	void onKey(int key, int scancode, int action, int mods) override
	{
		// TODO: Route keys to the active element
		for (auto doc : m_Elements)
			doc->onKey(key, scancode, action, mods);
	}

   	void onChar(unsigned codepoint) override
    {
		// TODO: Route chars to the active element
		for (auto doc : m_Elements)
			doc->onChar(codepoint);
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

	void addElement(Node* document)
	{	
		document->parent(this);
		m_Elements.push_back(document);
	}

	inline std::vector<Node*>& getElements() { return m_Elements; }

protected:
	std::vector<Node*> m_Elements;
};

}