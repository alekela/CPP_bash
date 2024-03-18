#include "Tree.h"

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
    myTree.Insert(20, 5, myTree.getRoot()->leafs[0], 1);
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

    std::cout << "@#$%^&*()(*&^%\n";

    myTree1.Insert(1199, myTree1.getRoot()->leafs[0], 2);
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
    
    std::cout << "End!\n";

    return 0;
}
