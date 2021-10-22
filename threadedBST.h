#ifndef ASS5_THREADEDBST_H
#define ASS5_THREADEDBST_H

#include <vector>

using namespace std;

class ThreadedBST {

private:
  struct TreeNode {
    explicit TreeNode(int data) : left(nullptr), right(nullptr), data(data){};
    TreeNode *left;
    TreeNode *right;
    int data;

    bool leftThread = false;
    bool rightThread = false;
  };
  TreeNode *root;
  int count = 0;

  void threadLeftSideRecur(TreeNode *threadTarget, TreeNode *threader,
                           TreeNode *prevThreader);
  void threadRightSideRecur(TreeNode *threadTarget, TreeNode *threader,
                            TreeNode *prevThreader);

public:
  explicit ThreadedBST();
  explicit ThreadedBST(int n);
  explicit ThreadedBST(const ThreadedBST &tree);
  ~ThreadedBST();

  void copy(TreeNode *node);
  void clear();
  void insert(TreeNode *node, TreeNode *newNode);
  bool add(int data);
  void balancedAdd(vector<int> vect);
  bool remove(int data);
  void removeEven();
  void removeEvenHelper(TreeNode *node);
  void removeZeroChild(TreeNode *delPtr, TreeNode *prevPtr);
  void removeOneChild(TreeNode *prevPtr, TreeNode *delPtr);
  void removeTwoChild(TreeNode *delPtr, TreeNode *prevPtr, TreeNode *inorderPtr,
                      TreeNode *prevInorderPtr, TreeNode *leftInorderThreader);
  bool retrieve(const TreeNode &node1, TreeNode node2);
  bool contains(int target);
  void thread();
  void inorderPrint() const;
  int getHeight();
  int heightHelper(TreeNode *node) const;
  int getCount() const;

  ThreadedBST &operator=(const ThreadedBST &tree);
};

#endif