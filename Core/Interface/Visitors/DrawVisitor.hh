#pragma once

#include <raindance/Core/Interface/Visitors/IVisitor.hh>
#include <raindance/Core/Context.hh>
#include <raindance/Core/Headers.hh>
#include <raindance/Core/Transformation.hh>
#include <raindance/Core/Text.hh>
#include <raindance/Core/Font.hh>
#include <raindance/Core/Camera/Camera.hh>


class DrawVisitor : public IVisitor
{
    public:
        DrawVisitor(Context* context = NULL)
        {
            m_Context = context;
        }

        ~DrawVisitor()
        {
            m_Context = 0;
        }


        void visit(Document::Node* node) override
        {
            (void) node;
            LOG("Visited Node");
        }

        void visit(Document::Group* group) override
        {
            LOG("Visited Document Group");
            for (auto element : group->getElements())
            {
                element->accept(this);
            }
        }

        void visit(TextArea* textArea) override
        {
            float ratio = 2.0;
            float width = ratio * textArea->content().getWidth();
            float height = ratio * textArea->content().getHeight();
            float fontFactor = textArea->fontFactor();
            Camera camera = textArea->camera();
            Font font = textArea->getFont();
            std::list<std::string> lines = textArea->lines();
            Text text = textArea->text();
            Transformation transformation;

            camera.setOrthographicProjection(0, width, 0, height, 0, 1);
            camera.lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

            glDisable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

            transformation.translate(glm::vec3(0.0, - font->getDescender() * font->getSize(), 0.0));

            for (auto it : lines)
            {
                if (it.size() > 0)
                {
                    transformation.push();
                        transformation.scale(glm::vec3(fontFactor, fontFactor, 1.0));
                        m_Text.set(it.c_str(), font);
                        m_Text.draw(*m_Context, camera.getViewProjectionMatrix() * transformation.state());
                    transformation.pop();
                }

                transformation.translate(glm::vec3(0.0, font->getSize() * fontFactor, 0.0));
            }

        }

        void visit(CheckBox* checkBox) override
        {
            (void) checkBox;
            LOG("Visited CheckBox node");
        }

        void visit(Shell* shell) override
        {
            (void) shell;
            LOG("Visited CheckBox node");
        }

        inline Context* getContext() { return m_Context; }


    protected:
        Context* m_Context;
};
