#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Primitives/Cube.hh>

class OctreeElement
{
public:
    virtual ~OctreeElement() {}

    virtual bool isOverlap(const glm::vec3& min, const glm::vec3& max) const = 0;
};

class OctreeFunctor
{
public:
    virtual ~OctreeFunctor() {}

    virtual void apply(OctreeElement* element) = 0;
};

class OctreeNode
{
public:
    OctreeNode(const glm::vec3 center, const glm::vec3 halfdimension)
    {
        m_Center = center;
        m_HalfDimension = halfdimension;
        m_Children.assign(8, NULL);
    }

    virtual ~OctreeNode()
    {
        for (auto it : m_Children)
            if (it != NULL)
                delete it;
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

    void split()
    {
        for (int i = 0 ; i < 8; i++)
        {
            glm::vec3 center = m_Center;

            center.x += (i & 4 ? 1.0f : -1.0f) * m_HalfDimension.x / 2;
            center.y += (i & 2 ? 1.0f : -1.0f) * m_HalfDimension.y / 2;
            center.z += (i & 1 ? 1.0f : -1.0f) * m_HalfDimension.z / 2;

            m_Children[i] = new OctreeNode(center, 0.5f * m_HalfDimension);
        }
    }

    inline bool isLeaf() const { return m_Children[0] == NULL; }

    inline const glm::vec3& getCenter() const { return m_Center; }
    inline const glm::vec3& getHalfDimension() const { return m_HalfDimension; }

    inline std::vector<OctreeNode*>& getChildren() { return m_Children; }
    inline std::vector<OctreeElement*>& getElements() { return m_Elements; }

private:

    std::vector<OctreeElement*> m_Elements;
    std::vector<OctreeNode*> m_Children;

    glm::vec3 m_Center;
    glm::vec3 m_HalfDimension;
};

class Octree
{
public:
    typedef unsigned long ID;

    struct StackElement
    {
        StackElement(OctreeElement* element = NULL, OctreeNode* node = NULL, unsigned int depth = 0)
        : Element(element), Node(node), Depth(depth)
        {
        }

        OctreeElement* Element;
        OctreeNode* Node;
        unsigned int Depth;
    };

    Octree(const glm::vec3& min, const glm::vec3& max)
    {
        const float epsilon = std::numeric_limits<float>::epsilon();
        const glm::vec3 e = glm::vec3(epsilon, epsilon, epsilon);

        glm::vec3 bmin = min - e;
        glm::vec3 bmax = max + e;
        m_Root = new OctreeNode(0.5f * (bmin + bmax), 0.5f * (bmax - bmin));

        m_MaxElementsPerLeaf = 128;
        m_MaxDepth = 10;
        m_ElementCount = 0;

        m_Cube = new Cube();
        m_Cube->getLineVertexBuffer().mute("a_Normal", true);

        m_Shader = ResourceManager::getInstance().loadShader("octree:octant cube",
                Assets_Shaders_Primitives_grid_vert, sizeof(Assets_Shaders_Primitives_grid_vert),
                Assets_Shaders_Primitives_grid_frag, sizeof(Assets_Shaders_Primitives_grid_frag));
    
        m_Shader->use();
        m_Shader->uniform("u_Color").set(glm::vec4(0.5, 0.5, 0.5, 0.5));
    }

    virtual ~Octree()
    {
        SAFE_DELETE(m_Root);

        ResourceManager::getInstance().unload(m_Shader);
    }

    void insert(OctreeElement* element, OctreeNode* root = NULL)
    {
        std::stack<StackElement> stack;
        StackElement current;

        if (root == NULL)
            root = m_Root;

        stack.push(StackElement(element, root, 0));
        while(!stack.empty())
        {
            current = stack.top();
            stack.pop();

            if (current.Node->isLeaf())
            {
                current.Node->getElements().push_back(current.Element);

                if (current.Node->getElements().size() <= m_MaxElementsPerLeaf || current.Depth >= m_MaxDepth)
                    continue;

                current.Node->split();

                // Reinsert elements in newly created leaves.
                for (auto e : current.Node->getElements())
                {
                    int count = 0;
                    for (auto c : current.Node->getChildren())
                    {
                        glm::vec3 cmax = c->getCenter() + c->getHalfDimension();
                        glm::vec3 cmin = c->getCenter() - c->getHalfDimension();

                        if (e->isOverlap(cmin, cmax))
                        {
                            stack.push(StackElement(e, c, current.Depth + 1));
                            count++;
                        }
                    }
                    if (count == 0)
                    {
                        LOG("[DEBUG] Node lost in split !\n");
                    }
                }
                current.Node->getElements().clear();
            }
            else
            {
                int count = 0;
                for (auto c : current.Node->getChildren())
                {
                    glm::vec3 cmax = c->getCenter() + c->getHalfDimension();
                    glm::vec3 cmin = c->getCenter() - c->getHalfDimension();

                    if (element->isOverlap(cmin, cmax))
                    {
                        stack.push(StackElement(element, c, current.Depth + 1));
                        count++;
                    }
                }
                if (count == 0)
                {
                    LOG("[DEBUG] Node lost in recursion !\n");
                }
            }
        }

        m_ElementCount++;
    }

    void foreachElementsInsideFrustrum(const Frustrum& frustrum, OctreeFunctor* functor)
    {
        std::stack<StackElement> stack;
        StackElement current;

        stack.push(StackElement(NULL, m_Root, 0));
        while(!stack.empty())
        {
            current = stack.top();
            stack.pop();

            if (current.Node->isLeaf())
            {
                for (auto e : current.Node->getElements())
                   functor->apply(e);
            }
            else
            {
                for (auto c : current.Node->getChildren())
                {
                    glm::vec3 cmin = c->getCenter() - c->getHalfDimension();
                    glm::vec3 cmax = c->getCenter() + c->getHalfDimension();

                    if (frustrum.classifyBox(cmin, cmax) != Frustrum::OUTSIDE)
                        stack.push(StackElement(NULL, c, current.Depth + 1));
                }
            }
        }
    }

    void draw(Context* context, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model)
    {
        std::stack<StackElement> stack;
        StackElement current;

        stack.push(StackElement(NULL, m_Root, 0));
        while(!stack.empty())
        {
            current = stack.top();
            stack.pop();

            if (current.Node->isLeaf())
            {
                if(current.Node->getElements().size() == 0)
                    continue;

                Transformation transformation;
                transformation.translate(current.Node->getCenter());
                transformation.scale(2.0f * 2.0f * current.Node->getHalfDimension());

                m_Shader->use();
                m_Shader->uniform("u_ModelViewProjection").set(projection * view * model * transformation.state());

                context->geometry().bind(m_Cube->getLineVertexBuffer(), *m_Shader);        
                context->geometry().drawArrays(GL_LINES, 0, m_Cube->getLineVertexBuffer().size() / sizeof(Cube::Vertex));
                context->geometry().unbind(m_Cube->getLineVertexBuffer());
            }
            else
            {
                for (auto c : current.Node->getChildren())
                    stack.push(StackElement(NULL, c, current.Depth + 1));
            }
        }
    }

    void dump()
    {
        std::stack<StackElement> stack;
        StackElement current;

        stack.push(StackElement(NULL, m_Root, 0));
        while(!stack.empty())
        {
            current = stack.top();
            stack.pop();

            for (unsigned int i = 0; i < current.Depth; i++)
                std::cout << "\t";

            std::cout << current.Node->getElements().size() << std::endl;

            if (!current.Node->isLeaf())
            {
                for (auto c : current.Node->getChildren())
                    stack.push(StackElement(NULL, c, current.Depth + 1));
            }
        }
    }

    inline void setMaxElementsPerLeaf(unsigned int value) { m_MaxElementsPerLeaf = value; }
    inline void setMaxDepth(unsigned int value) { m_MaxDepth = value; }
    inline unsigned int getElementCount() { return m_ElementCount; }

private:

    OctreeNode* m_Root;

    unsigned int m_ElementCount;
    unsigned int m_MaxElementsPerLeaf;
    unsigned int m_MaxDepth;

    Cube* m_Cube;
    Shader::Program* m_Shader;
};
