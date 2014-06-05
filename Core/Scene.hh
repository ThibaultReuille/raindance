#pragma once

#include "Core/Headers.hh"
#include "Core/Transformation.hh"
#include "Core/Octree.hh"

namespace Scene
{
	class IDrawable
	{
	public:
		virtual ~IDrawable() = 0;
		virtual void draw(Context* context, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model) = 0;
	};

	IDrawable::~IDrawable() {}

	class Node : public IDrawable
	{
	public:
		Node(IDrawable* drawable, bool owner)
			: m_PositionLock(false)
		{
			setDrawable(drawable, owner);
			setPosition(glm::vec3(0, 0, 0));
			setOrientation(glm::quat());
			setScale(glm::vec3(1.0, 1.0, 1.0));
			setDirection(glm::vec3(0, 0, 0), false);
		}

		virtual ~Node()
		{
			if (m_Owner)
				delete m_Drawable;
		}

		void draw(Context* context, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model)
		{
			if (m_DirtyModelMatrix)
				updateModelMatrix();

			m_Drawable->draw(context, projection, view, model * m_ModelMatrix);
		}

		inline void setDrawable(IDrawable* drawable, bool owner) { m_Drawable = drawable; m_Owner = owner; }
		inline IDrawable* getDrawable() { return m_Drawable; }

		inline bool isOwner() const { return m_Owner; }

		inline void setPosition(const glm::vec3& position) { m_Position = position; m_DirtyModelMatrix = true; }
		inline const glm::vec3& getPosition() const { return m_Position; }

		inline void setOrientation(const glm::quat& orientation) { m_Orientation = orientation; m_DirtyModelMatrix = true; }
		inline const glm::quat& getOrientation() const { return m_Orientation; }

		inline void setScale(const glm::vec3& scale) { m_Scale = scale; m_DirtyModelMatrix = true; }
		inline const glm::vec3& getScale() const { return m_Scale; }

		inline void setDirection(const glm::vec3& direction, bool normalize) { m_Direction = normalize ? glm::normalize(direction) : direction; }
		inline const glm::vec3& getDirection() const { return m_Direction; }

		inline void normalizeDirection()
		{
			if (glm::length2(m_Direction) > 0.0f)
				m_Direction = glm::normalize(m_Direction);
		}

		inline void setPositionLock(const bool lock) { m_PositionLock = lock; }
		inline bool isPositionLocked() { return m_PositionLock; }

	private:

		void updateModelMatrix()
		{
		    Transformation transformation;

		    transformation.translate(m_Position);
		    transformation.rotate(m_Orientation);
		    transformation.scale(m_Scale);

			m_ModelMatrix = transformation.state();
			m_DirtyModelMatrix = false;
		}

		glm::vec3 m_Position;
		glm::quat m_Orientation;
		glm::vec3 m_Scale;

		glm::mat4 m_ModelMatrix;
		bool m_DirtyModelMatrix;

		glm::vec3 m_Direction;

		IDrawable* m_Drawable;
		bool m_Owner;

		bool m_PositionLock;
	};

    class OctreeNode : public OctreeElement
    {
    public:
        OctreeNode(Node* node) { m_Node = node; }

        virtual ~OctreeNode() { m_Node = NULL; }

        virtual const glm::vec3& getPosition() const { return m_Node->getPosition(); }

    private:
        Node* m_Node;
    };

	class NodeVector : public IDrawable
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

		inline void setSpeed(float speed) { m_Speed = speed; }
		inline float getSpeed() { return m_Speed; }

		inline iterator begin() { return m_Nodes.begin(); }
		inline iterator end() { return m_Nodes.end(); }

	private:
		std::vector<Node*> m_Nodes;
		std::stack<unsigned long> m_DeadNodes;
		float m_Speed;
	};
};

