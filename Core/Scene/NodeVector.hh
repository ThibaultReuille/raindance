#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Scene/Node.hh>

namespace Scene
{
    class NodeVector : public Scene::Node
    {
    public:

        typedef std::vector<Node*>::iterator iterator;

        NodeVector()
            : m_Speed(1.0f)
        {
        }

        virtual ~NodeVector()
        {
            clear();
        }

        void clear()
        {
            std::vector<Node*>::iterator it;
            for (it = m_Nodes.begin(); it != m_Nodes.end(); ++it)
                if (*it != NULL)
                    delete *it;

            while (!m_DeadNodes.empty())
                m_DeadNodes.pop();
        }

        unsigned long add(Node* node)
        {
            if (m_DeadNodes.empty())
            {
                m_Nodes.push_back(node);
                return m_Nodes.size() - 1;
            }
            else
            {
                unsigned long id = m_DeadNodes.top();
                m_Nodes[id] = node;
                m_DeadNodes.pop();
                return id;
            }
        }

        void remove(unsigned long id)
        {
            delete m_Nodes[id];
            m_Nodes[id] = NULL;
            m_DeadNodes.push(id);
        }

        inline unsigned long size() { return m_Nodes.size(); }
        inline Node* operator[](unsigned long n) { return m_Nodes[n]; }

        void draw(Context* context, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model)
        {
            std::vector<Node*>::iterator it;
            for (it = m_Nodes.begin(); it != m_Nodes.end(); ++it)
                if (*it != NULL)
                    (*it)->draw(context, projection, view, model);
        }

        void update()
        {
            const float c_MaxRadius = 10000.0f;

            glm::vec3 position;

            std::vector<Node*>::iterator it;
            for (it = m_Nodes.begin(); it != m_Nodes.end(); ++it)
            {
                if (*it != NULL && !(*it)->isPositionLocked())
                {
                    position = (*it)->getPosition() + m_Speed * (*it)->getDirection();
                    if (glm::length(position) < c_MaxRadius)
                        (*it)->setPosition(position);
                }
            }
        }

        void normalizeDirections()
        {
            std::vector<Node*>::iterator it;
            for (it = m_Nodes.begin(); it != m_Nodes.end(); ++it)
                if (*it != NULL)
                    (*it)->normalizeDirection();
        }

        void randomizeDirections()
        {
            std::vector<Node*>::iterator it;
            for (it = m_Nodes.begin(); it != m_Nodes.end(); ++it)
                if (*it != NULL)
                    (*it)->setDirection((*it)->getDirection() * ((float) rand() / RAND_MAX), false);
        }

        virtual bool isOverlap (const glm::vec3& min, const glm::vec3& max) const
        {
            for (auto n : m_Nodes)
                if (n != NULL)
                    if (n->isOverlap(min, max))
                        return true;
            return false;
        }

        inline void setSpeed(float speed) { m_Speed = speed; }
        inline float getSpeed() { return m_Speed; }

        inline iterator begin() { return m_Nodes.begin(); }
        inline iterator end() { return m_Nodes.end(); }

    private:
        std::vector<Node*> m_Nodes;
        std::stack<unsigned long> m_DeadNodes;
        float m_Speed;
    };
}
