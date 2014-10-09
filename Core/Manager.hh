#pragma once

template <class Element>
class Manager
{
public:
    typedef unsigned long ID;

    Manager()
    {
        m_NextID = 0;
        m_Active = NULL;
    }

    virtual ~Manager()
    {
        for (auto e : m_Elements)
            delete e.second;
    }

    ID add(Element* element)
    {
        ID id = m_NextID;
        m_Elements[id] = element;
        m_NextID++;
        return id;
    }

    void destroy(ID id)
    {
        Element* e = m_Elements[id];
        if (m_Active == e)
            m_Active = NULL;
        m_Elements.erase(id);
        delete e;
    }

    void bind(ID id)
    {
        m_Active = m_Elements[id];
    }

    // Accessors
    inline Element* element(ID id) { return m_Elements[id]; }
    inline Element* active() { return m_Active; }
    inline std::unordered_map<ID, Element*>& elements() { return m_Elements; }

private:
    ID m_NextID;
    Element* m_Active;
    std::unordered_map<ID, Element*> m_Elements;
};
