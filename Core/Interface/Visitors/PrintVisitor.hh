#pragma once

#include <raindance/Core/Interface/Visitors/IVisitor.hh>
#include <raindance/Core/Context.hh>


class PrintVisitor : public IVisitor
{
    public:
        PrintVisitor(Context* context = NULL)
        {
            m_Context = context;
        }

        ~PrintVisitor()
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
            (void) textArea;
            LOG("Visited TextArea node");
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
