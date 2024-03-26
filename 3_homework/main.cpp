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
    Tree myTree{1, 5};

    myTree.Insert(10);
    myTree.Insert(11);
    myTree.Insert(12);
    myTree.Insert(13);
    myTree.Insert(14);

    for (int j = 0; j < 5; j++){
    	for (int i = 0; i < 3; i++){
	    myTree.Insert(20 + (((i+1) * (j+1)) * 1023) % 10 , myTree.getRoot(), j);
    	}
    }

    for (int i = 0; i < 5; i++) {
	for (int j = 0; j < 3; j++) {
		myTree.Insert(30 - (((i+1) + (j+1)) * 1023) % 10, myTree.getRoot()->leafs[i], j);
	}
    }


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
