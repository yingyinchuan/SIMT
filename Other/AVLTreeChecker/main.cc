#include <iostream>
#include <algorithm>

class TreeNode
{
public:
    int val;
    TreeNode *left;
    TreeNode *right;

    TreeNode(int value) : val(value), left(nullptr), right(nullptr) {}
};

class AVLTreeChecker
{
public:
    bool isAVL(TreeNode *root)
    {
        if (root == nullptr)
        {
            return true;
        }

        int leftHeight = getHeight(root->left);
        int rightHeight = getHeight(root->right);

        if (std::abs(leftHeight - rightHeight) > 1)
        {
            return false;
        }

        return isAVL(root->left) && isAVL(root->right);
    }

private:
    int getHeight(TreeNode *node)
    {
        if (node == nullptr)
        {
            return 0;
        }

        int leftHeight = getHeight(node->left);
        int rightHeight = getHeight(node->right);

        return 1 + std::max(leftHeight, rightHeight);
    }
};

int main()
{
    // 构建二叉树
    TreeNode *root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);

    AVLTreeChecker checker;
    if (checker.isAVL(root))
    {
        std::cout << "The given binary tree is an AVL tree." << std::endl;
    }
    else
    {
        std::cout << "The given binary tree is not an AVL tree." << std::endl;
    }

    // 释放内存
    // 在实际应用中，可能需要实现一个销毁二叉树的函数来释放内存
    return 0;
}