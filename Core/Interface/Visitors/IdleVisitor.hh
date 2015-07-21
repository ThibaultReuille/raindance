#pragma once

#include <raindance/Core/Interface/Visitors/IVisitor.hh>
#include <raindance/Core/Context.hh>


class IdleVisitor : public IVisitor
{
    public:
        IdleVisitor(Context* context = NULL)
        {
            m_Context = context;
        }

        ~IdleVisitor()
        {
            m_Context = 0;
        }

        void visit(Document::Node* node) override
        {
            (void) node;
            LOG("Idle(Note)");
        }

        void visit(Document::Group* group) override
        {
            LOG("Idle(Group)");

            for (auto element : group->getElements())
            {
                element->accept(this);
            }
        }

        void visit(TextArea* textArea) override
        {
            (void) textArea;
            LOG("Idle(Text Area)");
        }

        void visit(CheckBox* checkBox) override
        {
            (void) checkBox;
            LOG("Idle(Check Box)");
        }

        void visit(Shell* shell) override
        {
            (void) shell;
            LOG("Idle(Shell)");
        }

        inline Context* getContext() { return m_Context; }


    protected:
        Context* m_Context;
};
