#pragma once

#include <raindance/Core/Interface/Visitors/IVisitor.hh>
#include <raindance/Core/Context.hh>


class PrintVisitor : public IVisitor
{
    public:
        PrintVisitor(Context* context = NULL) : m_IndentLevel (0)
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
            insertWhiteSpaces();
            LOG("[Node]\n");
        }

        void visit(Document::Group* group) override
        {
            insertWhiteSpaces();
            LOG("[Group]\n");
            m_IndentLevel++;

            for (auto element : group->getElements())
            {
                element->accept(this);
            }

            m_IndentLevel--;
        }

        void visit(TextArea* textArea) override
        {
            (void) textArea;
            insertWhiteSpaces();
            LOG("[TextArea]\n");
        }

        void visit(CheckBox* checkBox) override
        {
            (void) checkBox;
            insertWhiteSpaces();
            LOG("[CheckBox]\n");
        }

        void visit(Shell* shell) override
        {
            (void) shell;
            insertWhiteSpaces();
            LOG("[Shell]\n");
        }

        inline Context* getContext() { return m_Context; }
        inline void setContext(Context* context) { m_Context = context; }

    private:
        void insertWhiteSpaces ()
        {
            for(int i = 0; i < m_IndentLevel; i++)
                LOG("       ");
        }

    protected:
        Context* m_Context;

    private:
        int m_IndentLevel;
};
