#include <iostream>
#include <string>

#include "Tree.h"

TreeNode::TreeNode(int data): ms_data(data), ms_length(default_length_of_leafs)
{
    leafs = new TreeNode*[default_length_of_leafs];
    for(int i = 0; i < ms_length; i++)
    {
        leafs[i] = nullptr;
    }
}

TreeNode::TreeNode(int data, int len): ms_data(data), ms_length(len)
{
    if (len <= 0)
    {
        std::cerr << "Lenght of leafs array cannot be less then 1: len = " << len << std::endl;
        std::cerr << "Lenght has changed to default length = " << default_length_of_leafs << std::endl;
        ms_length = default_length_of_leafs;
    }
    leafs = new TreeNode*[ms_length];
    for(int i = 0; i < ms_length; i++)
    {
        leafs[i] = nullptr;
    }
}

TreeNode::TreeNode(const TreeNode &nodeCopy): ms_data(nodeCopy.ms_data), ms_length(nodeCopy.ms_length)
{
    leafs = new TreeNode*[ms_length];
    for(int i = 0; i < ms_length; i++)
    {
        leafs[i] = nullptr;
        if (nodeCopy.leafs[i])
            leafs[i] = new TreeNode(*nodeCopy.leafs[i]);  // recursively call copy constructor
    }
}

TreeNode::~TreeNode()
{
    for(int i = 0; i < ms_length; i++)
    {
        //std::cout << i << " Hello! I am destructor! Address deleting leafs[" << i << "] = "; std::cout << leafs[i] << "\n";
        delete leafs[i];
        leafs[i] = nullptr;
    }
    delete[] leafs;
    leafs = nullptr; 
}

Tree::~Tree()
{
    delete m_root; 
    //std::cout << "Tree was killed with his root!\n";
    m_root = nullptr;
}

Tree& Tree::operator= (const Tree& treeCopy) 
{
    TreeNode* tmp = new TreeNode(*treeCopy.m_root);
    delete m_root; 
    //std::cout << "Tree was killed with his root!\n";
    m_root = tmp;

    return *this;
}

TreeNode* Tree::getRoot() {return m_root;}

int Tree::getValue(TreeNode* node) {return node->ms_data;}

void Tree::Print()
{
    if(!m_root)
    {
        std::cout << "{}\n";
    } 
    else
    {
        std::cout << SubTreeAsString(m_root) << '\n';
    }
}

std::string Tree::SubTreeAsString(TreeNode* node)  /// Print the subtree starting at '*node'
{
    std::string *arrStr = new std::string[node->ms_length]; 

    for(int i = 0; i < node->ms_length; i++)
    {
        if(!node->leafs[i])
        {
            arrStr[i] = {"{}"};
        }
        else
        {
            arrStr[i] = SubTreeAsString(node->leafs[i]);
        }
    }

    std::string result = "{" + std::to_string(node->ms_data) + ": ";
    result += arrStr[0];

    for(int i = 1; i < node->ms_length; i++)
    {
        //if (arrStr[i] == "{}") continue; //skip empty {}
        result += ", " + arrStr[i];
    }
    result += "}";

    delete[] arrStr;
    return result;
}

void Tree::Insert(int val)
{   
    if(!m_root)
    {
        std::cerr << "Tree is not initialized! Initializing it with value " << val << std::endl;
        m_root = new TreeNode(val);
    }
    else
    {
        //InsertValue(val, m_root);
        for(int i = 0; i < m_root->ms_length; i++)
        {
            if (m_root->leafs[i]) continue;
            m_root->leafs[i] = new TreeNode{val};
            return;
        }
        std::cerr << "Problems! Nothing has been done! Root node has not place for " << val << std::endl;
    }
}

void Tree::Insert(int val, int len)
{   
    if (len <= 0)
    {
        std::cerr << "Nothing has been done! Length of leafs array cannot be less then 1: len = " << len << std::endl;
        return;
    }
    else if(!m_root)
    {
        std::cerr << "Tree is not initialized! Initializing it with value " << val << " and leafs (in root) " << len << std::endl;
        m_root = new TreeNode(val, len);
    }
    else
    {
        //InsertValue(val, len, m_root);
        for(int i = 0; i < m_root->ms_length; i++)
        {
            if (m_root->leafs[i]) continue;
            m_root->leafs[i] = new TreeNode{val, len};
            return;
        }
        std::cerr << "Problems! Nothing has been done! This node has not place for " << val << std::endl;
    }
}

void Tree::Insert(int val, TreeNode* node, int numb)
{
    if (!node)
    {
        std::cerr << "Nothing has been done! Value " << val << " has not been added in this node because this node does not exist!" << std::endl;
        return;
    }
    else if (numb < 0)
    {
        std::cerr << "An array can't have member with index < 0! " << "(inputed:" << numb << ")" << std::endl;
        return;
    }
    else if (numb > node->ms_length-1)
    {
        std::cerr << "Going beyond the array boundary leaf! Length of leafs is " << node->ms_length << "(inputed:" << numb << ")" << std::endl;
        return;
    }
    else if (!node->leafs[numb])
    {	
        std::cerr << "Nothing has been done! Value " << val << " has not been added in this node because this node does not exist!" << std::endl;
        return;
    }
    
    for(int i = 0; i < node->leafs[numb]->ms_length; i++)
    {
        if (node->leafs[numb]->leafs[i]) continue;
        node->leafs[numb]->leafs[i] = new TreeNode{val};
        return;
    }
    std::cerr << "Problems! Nothing has been done! This node has not place for " << val << std::endl;
}

void Tree::Insert(int val, int len, TreeNode* node, int numb)
{  
    if (!node)
    {
        std::cerr << "Nothing has been done! Value " << val << " has not been added in this node because this node does not exist!" << std::endl;
        return;
    }
    else if (len <= 0 && numb < 0)
    {
        std::cerr << "Nothing has been done! Length of leafs array cannot be less then 1: len = " << len << std::endl;
        std::cerr << "An array can't have member with index < 0! " << "(inputed:" << numb << ")" << std::endl;
        return;
    }
    else if (len <= 0 && numb > node->ms_length-1)
    {
        std::cerr << "Nothing has been done! Length of leafs array cannot be less then 1: len = " << len << std::endl;
        std::cerr << "Going beyond the array boundary leaf! Length of leafs is " << node->ms_length << "(inputed:" << numb << ")" << std::endl;
        return;
    }
    else if (len <= 0)
    {
        std::cerr << "Nothing has been done! Length of leafs array cannot be less then 1: val = " << val << std::endl;
        return;
    }
    else if (numb < 0)
    {
        std::cerr << "An array can't have member with index < 0! " << "(inputed:" << numb << ")" << std::endl;
        return;
    }
    else if (numb > node->ms_length-1)
    {
        std::cerr << "Going beyond the array boundary leaf! Length of leafs is " << node->ms_length << "(inputed:" << numb << ")" << std::endl;
        return;
    }
    else if (!node->leafs[numb])
    {	
        std::cerr << "Nothing has been done! Value " << val << " has not been added in this node because this node does not exist!" << std::endl;
        return;
    }


    for(int i = 0; i < node->leafs[numb]->ms_length; i++)
    {
        if (node->leafs[numb]->leafs[i]) continue;
        node->leafs[numb]->leafs[i] = new TreeNode{val, len};
        return;
    }
    std::cerr << "Problems! Nothing has been done! This node has not place for " << val << std::endl;
}

void Tree::Remove() 
{
    Remove(this->m_root);
}

void Tree::Remove(TreeNode*& node) 
{
    if(!node) std::cerr << "Node was not found in tree" << std::endl;   
    delete node; //we can delete node=nullptr --> In this case nothing happend 
    node = nullptr;
}
