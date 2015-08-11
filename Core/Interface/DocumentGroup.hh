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
        m_ActiveElement = NULL;
    }

    virtual ~Group()
    {
    }

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

            doc->background().draw(context);
            doc->draw(context);
        }

        glDisable(GL_SCISSOR_TEST);
    }

    void accept(IVisitor* visitor) override
    {
        visitor->visit(this);
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
        Node* result = NULL;

        auto z = -std::numeric_limits<float>::infinity();
        for (auto element : m_Elements)
        {
            auto pick = element->pick(position - element->position().xy(), region);
            if (pick != NULL && pick->position().z > z)
            {
                z = pick->position().z;
                result = pick;
            }
        }
    
        if (result == NULL && region != NULL)
            *region = OUTSIDE;

        return result;
    }

    void onResize(const Viewport& viewport) override
    {
        for (auto doc : m_Elements)
            doc->onResize(viewport);
    }

    void onMouseDown(const glm::vec2& pos) override
    {
        m_ActiveElement = pick(pos, NULL);
        if (m_ActiveElement != NULL)
            m_ActiveElement->onMouseDown(pos - m_ActiveElement->position().xy());
    }

    void onMouseClick(const glm::vec2& pos) override
    {
        if (m_ActiveElement != NULL)
            m_ActiveElement->onMouseClick(pos - m_ActiveElement->position().xy());
    }

    void onMouseDoubleClick(const glm::vec2& pos) override
    {
        if (m_ActiveElement != NULL)
            m_ActiveElement->onMouseDoubleClick(pos - m_ActiveElement->position().xy());
    }

    void onMouseTripleClick(const glm::vec2& pos) override
    {
        if (m_ActiveElement != NULL)
            m_ActiveElement->onMouseTripleClick(pos - m_ActiveElement->position().xy());
    }

    void onMouseMove(const glm::vec2& pos, const glm::vec2& dpos) override
    {
        if (m_ActiveElement != NULL)
            m_ActiveElement->onMouseMove(pos - m_ActiveElement->position().xy(), dpos - m_ActiveElement->position().xy());
    }

    void onKey(int key, int scancode, int action, int mods) override
    {
        if (m_ActiveElement != NULL)
            m_ActiveElement->onKey(key, scancode, action, mods);
    }

    void onChar(unsigned codepoint) override
    {
        if (m_ActiveElement != NULL)
            m_ActiveElement->onChar(codepoint);
    }

    void onScroll(double xoffset, double yoffset) override
    {
        if (m_ActiveElement != NULL)
            m_ActiveElement->onScroll(xoffset, yoffset);
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

    inline void setActiveElement(Node* node) { m_ActiveElement = node; }
    inline std::vector<Node*>& getElements() { return m_Elements; }

protected:
    Node* m_ActiveElement;
    std::vector<Node*> m_Elements;
};

}
