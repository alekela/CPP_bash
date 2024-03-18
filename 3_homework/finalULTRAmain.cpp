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

Tree::TreeNode::TreeNode(int data): ms_data(data), ms_length(default_length_of_leafs)
{
    leafs = new TreeNode*[default_length_of_leafs];
    for(int i = 0; i < ms_length; i++)
    {
        leafs[i] = nullptr;
    }
}

Tree::TreeNode::TreeNode(int data, int len): ms_data(data), ms_length(len)
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

Tree::TreeNode::TreeNode(const TreeNode &nodeCopy): ms_data(nodeCopy.ms_data), ms_length(nodeCopy.ms_length)
{
    leafs = new TreeNode*[ms_length];
    for(int i = 0; i < ms_length; i++)
    {
        leafs[i] = nullptr;
        if (nodeCopy.leafs[i])
            leafs[i] = new TreeNode(*nodeCopy.leafs[i]);  // recursively call copy constructor
    }
}

Tree::TreeNode::~TreeNode()
{
    for(int i = 0; i < ms_length; i++)
    {
        std::cout << i << " Hello! I am destructor! Address deleting leafs[" << i << "] = "; std::cout << leafs[i] << "\n";
        delete leafs[i];
        leafs[i] = nullptr;
    }
    delete[] leafs;
    leafs = nullptr; 
}

Tree::~Tree()
{
    delete m_root; 
    std::cout << "Tree was killed with his root!\n";
    m_root = nullptr;
}

Tree& Tree::operator= (const Tree& treeCopy) 
{
    TreeNode* tmp = new TreeNode(*treeCopy.m_root);
    delete m_root; 
    std::cout << "Tree was killed with his root!\n";
    m_root = tmp;

    return *this;
}

Tree::TreeNode* Tree::getRoot() {return m_root;}

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

    std::string result = "{" + std::to_string(node->ms_data);

    for(int i = 0; i < node->ms_length; i++)
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
    if (!node || !node->leafs[numb])
    {
        std::cerr << "Nothing has been done! Value " << val << " has not been added in this node because this node does not exist!" << std::endl;
        return;
    }
    else if (numb < 0)
    {
        std::cerr << "An array can't have member with index < 0! " << "(inputed:" << numb << ")" << std::endl;
        return;
    }
    else if (numb > node->ms_length)
    {
        std::cerr << "Going beyond the array boundary leaf! Length of leafs is " << node->ms_length << "(inputed:" << numb << ")" << std::endl;
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
    if (!node || !node->leafs[numb])
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
    else if (len <= 0 && numb > node->ms_length)
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
    else if (numb > node->ms_length)
    {
        std::cerr << "Going beyond the array boundary leaf! Length of leafs is " << node->ms_length << "(inputed:" << numb << ")" << std::endl;
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

int main() 
{
    Tree myTree{9};
    myTree.Print();

    myTree.Insert(10);
    myTree.Print();

    myTree.Insert(11, 1);
    myTree.Print();

    myTree.Insert(12, 2);
    myTree.Print();

    myTree.Insert(333);
    myTree.Print();

    myTree.Insert(15, myTree.getRoot(), 0);
    myTree.Print();
    myTree.Insert(99, myTree.getRoot(), 1);
    myTree.Print();

    myTree.Insert(99, myTree.getRoot(), 1);
    myTree.Print();

    Tree myTree1 = myTree;
    myTree1.Print();

    Tree myTree2;
    myTree2.Insert(1, 6);
    myTree2.Print();
    
    std::cout << "+++++++++\n";
    
    myTree1.Print();
    myTree1 = myTree2;
    myTree1.Print();

    std::cout << "-------\n";

    myTree.Print();
    std::cout << myTree.getRoot()->leafs[0] << " = " << myTree.getRoot()->leafs[0]->ms_data << " @@@@@@@@34567890\n";

    myTree.Insert(20, myTree.getRoot()->leafs[0], 1);
    myTree.Print();

    myTree.Remove(myTree.getRoot()->leafs[0]);
    myTree.Print();
    
    myTree.Remove(myTree.getRoot()->leafs[0]);
    myTree.Print();

    std::cout << "!!!\n";

    myTree.Remove();
    myTree.Print();

    myTree1.Insert(100, 1);
    myTree1.Insert(100, 0);
    myTree1.Insert(100, 2);
    myTree1.Insert(20, myTree1.getRoot(), 1);
    myTree1.Print();
    std::cout << "@#$%^&*()(*&^%\n" ;//<< myTree1.getRoot()->leafs[1]->leafs[2] << '\n';
    myTree1.Insert(20, myTree1.getRoot()->leafs[1], 2);
    myTree1.Print();
    myTree1.Remove(myTree1.getRoot()->leafs[1]);

    myTree1.Print();

    myTree2.Print();

    Tree myTree3;
    myTree3.Print();
    
    Tree myTree4{1, 1};
    myTree4.Print();

    Tree myTree5{1, -1};
    myTree5.Print();

    return 0;
}