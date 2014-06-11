#pragma once

#include "Core/Headers.hh"
#include "Core/Primitives/Cube.hh"

class OctreeElement
{
public:
    virtual ~OctreeElement() {}
    virtual const glm::vec3& getPosition() const = 0;
};

class Octree
{
public:
    typedef unsigned long ID;

    Octree(const glm::vec3 center, const glm::vec3 dimension)
    {
        m_Center = center;
        m_Dimension = dimension;
        m_Children.assign(8, NULL);

        m_MaxElementsPerLeaf = 32;

        m_Cube = new Cube();
        m_Cube->getVertexBuffer().mute("a_Normal", true);

        m_Shader = ResourceManager::getInstance().loadShader("octant cube",
                Resources_Shaders_Primitives_grid_vert, sizeof(Resources_Shaders_Primitives_grid_vert),
                Resources_Shaders_Primitives_grid_frag, sizeof(Resources_Shaders_Primitives_grid_frag));
    }

    virtual ~Octree()
    {
        for (auto it : m_Children)
            if (it != NULL)
                delete it;

        ResourceManager::getInstance().unload(m_Shader);
    }

    int getOctant(const glm::vec3& position) const
    {
        // NOTE : - means less than center, + means greater than.
        // child:  0 1 2 3 4 5 6 7
        // x:      - - - - + + + +
        // y:      - - + + - - + +
        // z:      - + - + - + - +

        int octant = 0;

        if(position.x >= m_Center.x)
            octant |= 4;
        if(position.y >= m_Center.y)
            octant |= 2;
        if(position.z >= m_Center.z)
            octant |= 1;

        return octant;
    }

    void insert(OctreeElement* element)
    {
        if (isLeaf())
        {
            if (m_Elements.size() < m_MaxElementsPerLeaf)
                m_Elements.push_back(element);
            else
            {
                split();

                // Reinsert elements in newly created leaves.
                for (auto e : m_Elements)
                    m_Children[getOctant(e->getPosition())]->insert(e);
                m_Elements.clear();

                m_Children[getOctant(element->getPosition())]->insert(element);
            }
        }
        else
        {
            m_Children[getOctant(element->getPosition())]->insert(element);
        }
    }

    void findElementsInsideBox(const glm::vec3& bmin, const glm::vec3& bmax, std::vector<OctreeElement*>& results)
    {
        if (isLeaf())
        {
            if (m_Elements.size() == 0)
                return;

            for (auto e : m_Elements)
            {
                const glm::vec3& p = e->getPosition();
                if (p.x > bmax.x || p.y > bmax.y || p.z > bmax.z)
                    continue;
                if (p.x < bmin.x || p.y < bmin.y || p.z < bmin.z)
                    continue;
                results.push_back(e);
            }
        }
        else
        {
            for (int i = 0; i < 8; i++)
            {
                glm::vec3 cmax = m_Children[i]->getCenter() + 0.5f * m_Children[i]->getDimension();
                glm::vec3 cmin = m_Children[i]->getCenter() - 0.5f * m_Children[i]->getDimension();

                if (cmax.x < bmin.x || cmax.y < bmin.y || cmax.z < bmin.z)
                    continue;
                if (cmin.x > bmax.x || cmin.y > bmax.y || cmin.z > bmax.z)
                    continue;

                m_Children[i]->findElementsInsideBox(bmin, bmax, results);
            }
        }
    }

    void findElementsInsideFrustrum(const Frustrum& frustrum, std::vector<OctreeElement*>& results)
    {
        if (isLeaf())
        {
            if (m_Elements.size() == 0)
                return;

            for (auto e : m_Elements)
                if (frustrum.classifyPoint(e->getPosition()) != Frustrum::OUTSIDE)
                    results.push_back(e);
        }
        else
        {
            for (int i = 0; i < 8; i++)
            {
                glm::vec3 cmin = m_Children[i]->getCenter() - 0.5f * m_Children[i]->getDimension();
                glm::vec3 cmax = m_Children[i]->getCenter() + 0.5f * m_Children[i]->getDimension();

                if (frustrum.classifyBox(cmin, cmax) != Frustrum::OUTSIDE)
                    m_Children[i]->findElementsInsideFrustrum(frustrum, results);
            }
        }
    }

    void draw(Context* context, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model)
    {
        if (isLeaf())
        {
            if (m_Elements.size() == 0)
                return;

            Transformation transformation;
            transformation.translate(m_Center);
            transformation.scale(m_Dimension);

            m_Shader->use();
            m_Shader->uniform("u_ModelViewProjection").set(projection * view * model * transformation.state());
            m_Shader->uniform("u_Color").set(glm::vec4(0.5, 0.5, 0.5, 0.5));
            context->geometry().bind(m_Cube->getVertexBuffer(), *m_Shader);
            context->geometry().drawElements(GL_LINES, m_Cube->getLineBuffer().size() / sizeof(short int), GL_UNSIGNED_SHORT, m_Cube->getLineBuffer().ptr());
            context->geometry().unbind(m_Cube->getVertexBuffer());
        }
        else
        {
            for (auto c : m_Children)
                c->draw(context, projection, view, model);
        }
    }

    inline bool isLeaf() const { return m_Children[0] == NULL; }

    inline void setMaxElementsPerLeaf(unsigned int value) { m_MaxElementsPerLeaf = value; }

    inline const glm::vec3& getCenter() const { return m_Center; }
    inline const glm::vec3& getDimension() const { return m_Dimension; }

private:

    void split()
    {
        for (int i = 0 ; i < 8; i++)
        {
            glm::vec3 center = m_Center;

            center.x += (i & 4 ? 1.0 : -1.0) * m_Dimension.x / 4;
            center.y += (i & 2 ? 1.0 : -1.0) * m_Dimension.y / 4;
            center.z += (i & 1 ? 1.0 : -1.0) * m_Dimension.z / 4;

            m_Children[i] = new Octree(center, 0.5f * m_Dimension);
        }
    }

    std::vector<Octree*> m_Children;
    std::vector<OctreeElement*> m_Elements;

    glm::vec3 m_Center;
    glm::vec3 m_Dimension;

    unsigned int m_MaxElementsPerLeaf;

    Cube* m_Cube;
    Shader::Program* m_Shader;
};
