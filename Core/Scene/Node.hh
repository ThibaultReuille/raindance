#pragma once

#include <raindance/Core/Headers.hh>
#include <raindance/Core/Transformation.hh>
#include <raindance/Core/Octree.hh>

namespace Scene
{
    class Node : public OctreeElement
    {
    public:
        Node()
            : m_Mark(0), m_PositionLock(false)
        {
            setPosition(glm::vec3(0, 0, 0));
            setOrientation(glm::quat());
            setScale(glm::vec3(1.0, 1.0, 1.0));
            setDirection(glm::vec3(0, 0, 0), false);
        }

        virtual ~Node()
        {
        }

        /*
        void draw(Context* context, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model)
        {
            if (m_DirtyModelMatrix)
                updateModelMatrix();

            m_Drawable->draw(context, projection, view, model * m_ModelMatrix);
        }
        */

        virtual void draw(Context* context, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model) = 0;

        inline void setPosition(const glm::vec3& position) { m_Position = position; m_DirtyModelMatrix = true; }
        inline const glm::vec3& getPosition() const { return m_Position; }

        inline void setOrientation(const glm::quat& orientation) { m_Orientation = orientation; m_DirtyModelMatrix = true; }
        inline const glm::quat& getOrientation() const { return m_Orientation; }

        inline void setScale(const glm::vec3& scale) { m_Scale = scale; m_DirtyModelMatrix = true; }
        inline const glm::vec3& getScale() const { return m_Scale; }

        inline void setDirection(const glm::vec3& direction, bool normalize) { m_Direction = normalize ? glm::normalize(direction) : direction; }
        inline const glm::vec3& getDirection() const { return m_Direction; }

        inline void setMark(int mark) { m_Mark = mark; }
        inline int getMark() { return m_Mark; }

        inline void normalizeDirection()
        {
            if (glm::length2(m_Direction) > 0.0f)
                m_Direction = glm::normalize(m_Direction);
        }

        inline void setPositionLock(const bool lock) { m_PositionLock = lock; }
        inline bool isPositionLocked() { return m_PositionLock; }

        void updateModelMatrix()
        {
            if (!m_DirtyModelMatrix)
                return;

            Transformation transformation;

            transformation.translate(m_Position);
            transformation.rotate(m_Orientation);
            transformation.scale(m_Scale);

            m_ModelMatrix = transformation.state();
            m_DirtyModelMatrix = false;
        }

        inline const glm::mat4& getModelMatrix()
        {
            updateModelMatrix();
            return m_ModelMatrix;
        }

private:
        glm::vec3 m_Position;
        glm::quat m_Orientation;
        glm::vec3 m_Scale;

        glm::mat4 m_ModelMatrix;
        bool m_DirtyModelMatrix;

        glm::vec3 m_Direction;

        int m_Mark;

        bool m_PositionLock;
    };
}
