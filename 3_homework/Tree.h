#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <string>

class Tree
{   
private:

    struct TreeNode
    {
        int const default_length_of_leafs = 3;
        int ms_data;
        int ms_length;
        TreeNode** leafs = nullptr;
        
        TreeNode(int);
        TreeNode(int, int);
        TreeNode(const TreeNode &);
        ~TreeNode();
    };
    
    TreeNode* m_root;

    std::string SubTreeAsString(TreeNode* node);  // Helper method for Print()

public:
    Tree(): m_root(nullptr) {}
    Tree(int val): m_root(new TreeNode(val)) {}
    Tree(int val, int len): m_root(new TreeNode(val, len)) {}
    Tree(const Tree &rhs): m_root(new TreeNode(*rhs.m_root)) {}    
    ~Tree();

    Tree &operator= (const Tree&);

    TreeNode* getRoot();
    int getValue(TreeNode*);

    void Print();

    void Insert(int);
    void Insert(int, int);
    void Insert(int, TreeNode*, int);
    void Insert(int, int, TreeNode*, int);

    void Remove(TreeNode*&);
    void Remove();
};

#endif