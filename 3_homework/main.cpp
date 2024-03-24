#include "Tree.h"
#include <vector>

int check_Node(TreeNode* node) {
    if (not node) {
        return -1;
    }
    for (int i = 0; i < node->ms_length; i++) {
        if (node->leafs[i]) {
            return 1;
        }
    }
    return 0;
}


void find(TreeNode* root, int* max_tmp, int* max_ans, std::vector<TreeNode*>* list_tmp, std::vector<TreeNode*>* list_ans) {
    if (check_Node(root) == 0) {
        (*list_tmp).push_back(root);
        (*max_tmp) += root->ms_data;
        if ((*max_tmp) > (*max_ans)) {
            (*max_ans) = (*max_tmp);
            (*list_ans) = (*list_tmp);
        }
        (*list_tmp).pop_back();
        (*max_tmp) -= root->ms_data;
    }
    else if (check_Node(root) == 1) {
        (*list_tmp).push_back(root);
        (*max_tmp) += root->ms_data;
        for (int i = 0; i < root->ms_length; i++) {
            find(root->leafs[i], max_tmp, max_ans, list_tmp, list_ans);
        }
        (*list_tmp).pop_back();
        (*max_tmp) -= root->ms_data;

    }
}


int main() 
{
    Tree myTree{9};
    myTree.Insert(10, 2);
    myTree.Insert(11);
    myTree.Insert(12, 1);
    myTree.Insert(15, 2, myTree.getRoot(), 1);
    std::cout << "Tree: ";
    myTree.Print();


    int max_ans = -1;
    int max_tmp = 0;
    std::vector<TreeNode*> list_tmp;
    std::vector<TreeNode*> list_ans;

    find(myTree.getRoot(), &max_tmp, &max_ans, &list_tmp, &list_ans);
    std::cout << "Maximum sum of path in the tree = " << max_ans << std::endl;
    std::cout << "Path: ";
    for (int i = 0; i < list_ans.size() - 1; i++) {
        std::cout << list_ans[i]->ms_data << "+";
    }
    std::cout << list_ans[list_ans.size() - 1]->ms_data << std::endl;

    return 0;
}