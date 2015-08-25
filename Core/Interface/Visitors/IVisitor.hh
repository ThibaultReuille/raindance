#pragma once

namespace Document
{
    class Node;
    class Group;
}

class TextArea;
class CheckBox;
class Shell;

class IVisitor
{
    public:
        virtual void visit(Document::Node* node) = 0;
        virtual void visit(Document::Group* group) = 0;
        virtual void visit(TextArea* textArea) = 0;
        virtual void visit(CheckBox* checkBox) = 0;
        virtual void visit(Shell* shell) = 0;
};
