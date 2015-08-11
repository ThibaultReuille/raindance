#pragma once

#include <raindance/Core/Interface/Visitors/IVisitor.hh>
#include <raindance/Core/Context.hh>


class PrintVisitor : public IVisitor
{
    public:
        PrintVisitor(Context* context = NULL) : indents (0)
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
            printWhiteSpaces();
            LOG("[Node]");
        }

        void visit(Document::Group* group) override
        {
            printWhiteSpaces();
            LOG("[Group]");
            m_Indents++;

            for (auto element : group->getElements())
            {
                element->accept(this);
            }
        }

        void visit(TextArea* textArea) override
        {
            (void) textArea;
            printWhiteSpaces();
            LOG("[TextArea]");
        }

        void visit(CheckBox* checkBox) override
        {
            (void) checkBox;
            printWhiteSpaces();
            LOG("[CheckBox]");
        }

        void visit(Shell* shell) override
        {
            (void) shell;
            printWhiteSpaces();
            LOG("[Shell]");
        }

        inline Context* getContext() { return m_Context; }
        inline void reset () { m_Idents = 0; }

    private:
        void printWhiteSpaces ()
        {
            for(int i = 0; i < m_Indents; i++)
                LOG("   ");
        }

    protected:
        Context* m_Context;

    private:
        int m_Indents;
};
