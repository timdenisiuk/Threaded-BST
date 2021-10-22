/**
 * Michael Cho
 * Tim D
 *
 * CSS342
 * **************ADD DESCRIPTION***********
 * */

#include "threadedBST.h"
#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

/** Constructor: Default constructor.
Precondition: None
Postcondition: Creates an empty ThreadedBST tree obj*/
ThreadedBST::ThreadedBST() : root{nullptr}, count{0} {}

/** Constructor: Constructor that takes an integer n as input and
creates a tree with n nodes.
Precondition: None
Postcondition: Creates a ThreadedBST tree obj with n nodes*/
ThreadedBST::ThreadedBST(int n) : root{nullptr}, count{0} {
  vector<int> vect1;
  vector<int> vect2;
  if (n % 2 == 1) {
    if (root == nullptr) {
      int mid = n / 2 + 1;
      add(mid);
    }
    for (int i = 1; i <= n / 2 + 1; i++) {
      vect1.push_back(i);
    }
    for (int i = 1; i <= n / 2; i++) {
      vect2.push_back(i + ((n / 2) + 1));
    }
  } else {
    if (root == nullptr) {
      int mid = n / 2;
      add(mid);
    }
    for (int i = 1; i <= n / 2; i++) {
      vect1.push_back(i);
    }
    for (int i = 1; i <= n / 2; i++) {
      vect2.push_back(i + (n / 2));
    }
  }
  balancedAdd(vect1);
  balancedAdd(vect2);
  thread();
}

/** Constructor: Constructor copy
Precondition: None
Postcondition: Creates a copy of a Threaded BST tree*/
ThreadedBST::ThreadedBST(const ThreadedBST &tree) : root{nullptr}, count{0} {
  if (tree.root == nullptr) {
    root = nullptr;
  } else {
    copy(tree.root);
    thread();
  }
}

/** Destructor: Calls the clear method
Precondition: ThreadedBST tree object must exist
Postcondition: Deletes the ThreadedBST tree object with the clear method*/
ThreadedBST::~ThreadedBST() { clear(); }

/** Clear: Deletes each dynamically created TreeNode in the ThreadedBST
tree object and sets each one to nullptr
Precondition: ThreadedBST tree object must exist
Postcondition: Deletes all TreeNodes*/
void ThreadedBST::clear() {
  TreeNode *curPtr = root;
  TreeNode *delPtr;

  while (curPtr->left != nullptr) {
    curPtr = curPtr->left;
  }
  delPtr = curPtr;
  while (curPtr != root) {
    if (curPtr->rightThread == false) {
      curPtr = curPtr->right;
      delete delPtr;
      delPtr = nullptr;

      if (curPtr->leftThread == false) {
        while (curPtr->leftThread == false)
          curPtr = curPtr->left;
      }
      delPtr = curPtr;
      continue;
    }

    if (curPtr->rightThread == true) {
      curPtr = curPtr->right;
      delete delPtr;
      delPtr = nullptr;
      delPtr = curPtr;
      continue;
    }
  }

  while (curPtr->right != nullptr) {
    curPtr = curPtr->right;
  }
  delPtr = curPtr;
  while (curPtr != root) {
    if (curPtr->leftThread == false) {
      curPtr = curPtr->left;
      delete delPtr;
      delPtr = nullptr;

      if (curPtr->rightThread == false) {
        while (curPtr->rightThread == false)
          curPtr = curPtr->right;
      }
      delPtr = curPtr;
      continue;
    }

    if (curPtr->leftThread == true) {
      curPtr = curPtr->left;
      delete delPtr;
      delPtr = nullptr;
      delPtr = curPtr;
      continue;
    }
  }

  delete delPtr;
  delPtr = nullptr;
}
/** Insert: Inserts nodes according to the rules of a Binary
search tree.
Precondition: ThreadedBST tree object must exist
Postcondition: Places node in tree*/
void ThreadedBST::insert(TreeNode *node, TreeNode *newNode) {
  if (newNode->data < node->data && node->left != nullptr) {
    insert(node->left, newNode);
  }
  if (newNode->data > node->data && node->right != nullptr) {
    insert(node->right, newNode);
  }
  if (newNode->data < node->data && node->left == nullptr) {
    node->left = newNode;
  }
  if (newNode->data > node->data && node->right == nullptr) {
    node->right = newNode;
  }
}

/** Add: Adds a specific integer into the tree. Checks for
duplicates using contains. Calls insert to place nodes into
tree.
Precondition: ThreadedBST tree object must exist
Postcondition: Returns true if add is successful*/
bool ThreadedBST::add(int data) {
  if (contains(data)) {
    return false;
  }
  if (root == nullptr) {
    root = new TreeNode(data);
  } else {
    TreeNode *newNode = new TreeNode(data);
    insert(root, newNode);
  }
  count++;
  return true;
}

/** Balanced Add: Used in conjunction with the constructor.
Upon initializtion of a new ThreadedBST tree, balances the nodes
inserted throughout the tree by recursively adding the middle of
each vector one at a time. Base case when vector is empty.
Precondition: ThreadedBST tree object must exist
Postcondition: ThreadedBST tree with balanced nodes*/
void ThreadedBST::balancedAdd(vector<int> vect) {
  if (vect.size() > 0) {
    add(vect.at(vect.size() / 2));
    vect.erase(vect.begin() + vect.size() / 2);

    vector<int> splitLow(vect.begin(), vect.begin() + vect.size() / 2);
    vector<int> splitHigh(vect.begin() + vect.size() / 2, vect.end());

    balancedAdd(splitLow);
    balancedAdd(splitHigh);
  }
}

/** Remove: Remove function that deletes nodes and rethreads based on cases
Precondition: ThreadedBST tree object must exist
Postcondition: Returns true removed successfully*/
bool ThreadedBST::remove(int data) {
  if (!contains(data)) {
    return false;
  }
  TreeNode *prevPtr = root;
  TreeNode *delPtr;
  if (data < root->data)
    delPtr = root->left;
  else if (data > root->data)
    delPtr = root->right;
  else
    delPtr = root;

  while (data != delPtr->data) {
    if (data < delPtr->data) {
      prevPtr = delPtr;
      delPtr = delPtr->left;
    } else if (data > delPtr->data) {
      prevPtr = delPtr;
      delPtr = delPtr->right;
    }
  }

  if (delPtr->leftThread == true &&
      delPtr->rightThread == true) // Removing node with no real connections
  {

    removeZeroChild(delPtr, prevPtr);
  }

  else if (delPtr->left == nullptr) // Removing left edge
  {
    prevPtr->left = delPtr->right;
    delPtr->right->left = nullptr;
    delPtr->right->leftThread = false;

    delete delPtr;
    delPtr = nullptr;
  }

  else if (delPtr->right == nullptr) // Removing Right Edge
  {
    prevPtr->right = delPtr->left;
    delPtr->left->right = nullptr;
    delPtr->left->rightThread = false;

    delete delPtr;
    delPtr = nullptr;
  }

  else if (delPtr->leftThread == false &&
           delPtr->rightThread ==
               false) // Removing node with two real connections
  {
    TreeNode *inorderPtr = delPtr->right;
    TreeNode *leftInorderThreader = delPtr->left;
    TreeNode *prevInorderPtr = nullptr;

    removeTwoChild(delPtr, prevPtr, inorderPtr, prevInorderPtr,
                   leftInorderThreader);

  }

  else if (delPtr->leftThread == false ||
           delPtr->rightThread ==
               false) { // Removing node with one real connection
    removeOneChild(prevPtr, delPtr);
  }

  count--;
  return true;
}

/** Copy: Copies a tree object to current tree. Adds first node
which is root. Then, recursively adds each left and right node
until the bottom each branch is reached (end of branch is has
nullptr or is a left or right thread).
Precondition: ThreadedBST tree object must exist
Postcondition: Tree copied to current tree.*/
void ThreadedBST::copy(TreeNode *node) {
  add(node->data);
  if (!node->leftThread && node->left != nullptr) {
    copy(node->left);
  }
  if (!node->rightThread && node->right != nullptr) {
    copy(node->right);
  }
}

/** Remove Even: Calls the removeEvenHelper method with the root
node of current tree.
Precondition: ThreadedBST tree object must exist
Postcondition: Calls removeEvenHelper*/
void ThreadedBST::removeEven() { removeEvenHelper(this->root); }

/** Remove Even Helper: Recursive method that removes even nodes in
the tree. Stops when the end of a branch is reached (end of branch is
has nullptr or is a left or right thread).
Precondition: ThreadedBST tree object must exist
Postcondition: *************/
void ThreadedBST::removeEvenHelper(TreeNode *node) {
  if (!node->leftThread && node->left != nullptr) {
    removeEvenHelper(node->left);
  }
  if (!node->rightThread && node->right != nullptr) {
    removeEvenHelper(node->right);
  }

  if (node->data % 2 == 0) {
    remove(node->data);
  }
}
/** Remove One Child: Helper function that used for cases with zero children
Precondition: Must be called in remove when it has zero children
Postcondition: Reconnects other nodes to proper place, and deletes node*/

void ThreadedBST::removeZeroChild(TreeNode *delPtr, TreeNode *prevPtr) {
  if (prevPtr->leftThread == true ||
      prevPtr->rightThread ==
          true) // Checks if previous has a thread for threading reasons
  {
    if (delPtr->data > prevPtr->data) {
      prevPtr->right = delPtr->right;
      prevPtr->rightThread = true;
    }
    if (delPtr->data < prevPtr->data) {
      prevPtr->left = delPtr->left;
      prevPtr->leftThread = true;
    }
    delete delPtr;
    delPtr = nullptr;
  }

  else if (prevPtr->leftThread == false &&
           prevPtr->rightThread == false) // Checks if it does not have a thread
                                          // for threading reasons
  {
    if (prevPtr->right == delPtr) {
      prevPtr->right = delPtr->right;
      prevPtr->rightThread = true;
    } else if (prevPtr->left == delPtr) {
      prevPtr->left = delPtr->left;
      prevPtr->leftThread = true;
    }

    delete delPtr;
    delPtr = nullptr;
  }
}

/** Remove One Child: Helper function that used for cases with one child
Precondition: Must be called in remove when it has one child
Postcondition: Reconnects other nodes to proper place, and deletes node*/
void ThreadedBST::removeOneChild(TreeNode *prevPtr, TreeNode *delPtr) {
  if (delPtr->data > prevPtr->data) {
    if (delPtr->rightThread == false) {
      prevPtr->right = delPtr->right;
      delPtr->right->left = prevPtr;

      delete delPtr;
      delPtr = nullptr;
    } else {
      prevPtr->right = delPtr->left;
      delPtr->left->right = delPtr->right;

      delete delPtr;
      delPtr = nullptr;
    }

  } else if (delPtr->data < prevPtr->data) {
    if (delPtr->leftThread == false) {
      prevPtr->left = delPtr->left;
      delPtr->left->right = prevPtr;

      delete delPtr;
      delPtr = nullptr;
    } else {
      prevPtr->left = delPtr->right;
      delPtr->right->left = delPtr->left;

      delete delPtr;
      delPtr = nullptr;
    }
  }
}

/** Remove One Child: Helper function that used for cases with two children
Precondition: Must be called in remove when it has two children
Postcondition: Reconnects other nodes to proper place, and deletes node*/
void ThreadedBST::removeTwoChild(TreeNode *delPtr, TreeNode *prevPtr,
                                 TreeNode *inorderPtr, TreeNode *prevInorderPtr,
                                 TreeNode *leftInorderThreader) {
  if (delPtr == root) // Removing root
  {
    while (inorderPtr->leftThread == false) {
      prevInorderPtr = inorderPtr;
      inorderPtr = inorderPtr->left;
    }

    while (leftInorderThreader->rightThread == false)
      leftInorderThreader = leftInorderThreader->right;

    if (prevInorderPtr != nullptr) {
      prevInorderPtr->left = inorderPtr;
      prevInorderPtr->leftThread = true;
    }
    if (inorderPtr->rightThread == false && prevInorderPtr != nullptr) {
      prevInorderPtr->left = inorderPtr->right;
      prevInorderPtr->leftThread = false;
    }

    inorderPtr->left = delPtr->left;
    inorderPtr->leftThread = false;
    inorderPtr->right = delPtr->right;
    inorderPtr->rightThread = false;
    leftInorderThreader->right = inorderPtr;

    delete delPtr;
    delPtr = nullptr;

    root = inorderPtr;
  }

  else {

    while (inorderPtr->leftThread == false) {
      prevInorderPtr = inorderPtr;
      inorderPtr = inorderPtr->left;
    }

    while (leftInorderThreader->rightThread == false)
      leftInorderThreader = leftInorderThreader->right;

    inorderPtr->left = delPtr->left;
    inorderPtr->leftThread = false;

    leftInorderThreader->right = inorderPtr;
    if (prevInorderPtr == nullptr)
      inorderPtr->rightThread = true;

    if (prevInorderPtr != nullptr) {
      inorderPtr->right = delPtr->right;
      inorderPtr->rightThread = false;
      prevInorderPtr->left = inorderPtr;
      prevInorderPtr->leftThread = true;
    } else if (inorderPtr->rightThread == false && prevInorderPtr != nullptr) {
      inorderPtr->right = delPtr->right;
      inorderPtr->rightThread = false;
      prevInorderPtr->left = inorderPtr->right;
      prevInorderPtr->leftThread = false;
    }
    if (inorderPtr->data < prevPtr->data)
      prevPtr->left = inorderPtr;
    else
      prevPtr->right = inorderPtr;

    delete delPtr;
    delPtr = nullptr;
  }
}

/** Contains: Boolean method that checks whether a particular value exists
within the ThreadedBST tree.
Precondition: ThreadedBST tree object must exist
Postcondition: Returns true if the value is found*/
bool ThreadedBST::contains(int target) {
  if (root == nullptr)
    return false;

  TreeNode *curPtr = root;

  while (curPtr->data != target) {

    if (target < curPtr->data) {
      if (curPtr->left == nullptr)
        return false;
      else
        curPtr = curPtr->left;
    }

    else if (target > curPtr->data) {
      if (curPtr->right == nullptr)
        return false;
      else
        curPtr = curPtr->right;
    }
  }

  return true;
}

/** Thread: Main method for threading, calling on helpers for each side
Precondition: Tree must be fully created, as cannot be rethreaded with this
method Postcondition: returns a fully threaded BST*/
void ThreadedBST::thread() {
  TreeNode *threader = nullptr;
  TreeNode *prevThreader = nullptr;
  threadLeftSideRecur(root, threader, prevThreader);
  threadRightSideRecur(root, threader, prevThreader);
}

/** Thread Recursion: Helper methods for threading each side from root
Precondition: thread must be called somewhere else
Postcondition: Threads each side of the tree from root*/
void ThreadedBST::threadLeftSideRecur(TreeNode *threadTarget,
                                      TreeNode *threader,
                                      TreeNode *prevThreader) {

  while (threadTarget != nullptr) // Traversing
  {
    if (threadTarget->data > root->data) // Base case to end traversing left
      break;
    if (threadTarget->left != nullptr &&
        threadTarget->leftThread ==
            false) // Checks to see if left side has something to thread, will
                   // do for each node
      threader = threadTarget->left;
    else {
      threadTarget =
          threadTarget
              ->right; // Moves to the right when finished with left-most side
      continue;
    }

    if (threader->left != nullptr &&
        threader->leftThread ==
            false) // will thread the left side of the target before going right
      threadLeftSideRecur(threadTarget->left, threader, prevThreader);

    prevThreader = threader;
    while (threader != nullptr) {

      if (threader->right == nullptr) {
        threader->right = threadTarget; // Threads right
        threader->rightThread = true;   // Lets computer know right is a thread
        break;
      }
      threader = threader->right; // Traversing to next inorder spot

      if (threader->left == nullptr) {
        threader->left = prevThreader; // Threads left as it traverses right
        threader->leftThread = true;
        prevThreader = threader;
        continue;
      }
    }
    if (threadTarget->right != nullptr && threadTarget != root) {
      if (threadTarget->right->leftThread == false) {
        threader = threadTarget->right;
        while (threader->left != nullptr)
          threader = threader->left;
        threader->left = threadTarget;
        threader->leftThread = true;
      }
    }

    threadTarget = threadTarget->right;
  }
}

void ThreadedBST::threadRightSideRecur(TreeNode *threadTarget,
                                       TreeNode *threader,
                                       TreeNode *prevThreader) {
  while (threadTarget != nullptr) // Traversing
  {
    if (threadTarget->data < root->data) // Base case to end traversing left
      break;
    if (threadTarget->right != nullptr &&
        threadTarget->rightThread ==
            false) // Checks to see if left side has something to thread, will
                   // do for each node
      threader = threadTarget->right;
    else {
      threadTarget =
          threadTarget
              ->left; // Moves to the right when finished with left-most side
      continue;
    }

    if (threader->right != nullptr &&
        threader->rightThread ==
            false) // will thread the left side of the target before going right
      threadRightSideRecur(threadTarget->right, threader, prevThreader);

    prevThreader = threader;
    while (threader != nullptr) {

      if (threader->left == nullptr) {
        threader->left = threadTarget; // Threads right
        threader->leftThread = true;   // Lets computer know right is a thread
        break;
      }
      threader = threader->left; // Traversing to next inorder spot

      if (threader->right == nullptr) {
        threader->right = prevThreader; // Threads left as it traverses right
        threader->rightThread = true;
        prevThreader = threader;
        continue;
      }
    }
    if (threadTarget->left != nullptr && threadTarget != root) {
      if (threadTarget->left->rightThread == false) {
        threader = threadTarget->left;
        while (threader->right != nullptr)
          threader = threader->right;
        threader->right = threadTarget;
        threader->rightThread = true;
      }
    }

    threadTarget = threadTarget->left;
  }
}

/** In Order: Iterator that uses threads to print out values of a threaded BST
Precondition: Tree must be threaded for this to work
Postcondition: Prints out values in console */
void ThreadedBST::inorderPrint() const {
  TreeNode *curr = root;

  while (curr->left != nullptr) {
    curr = curr->left;
  }
  stack<int> reversetraversal; // Used for when traversing left side of tree
                               // with threads

  while (curr != root) {
    if (curr->rightThread == false) { // Will prioritize non-threaded links
                                      // first
      cout << curr->data << ", ";
      curr = curr->right;
      if (curr->leftThread ==
          false) { // Used for next inorder value if it has one
        while (curr->leftThread == false)
          curr = curr->left;
      }
      continue;
    }

    if (curr->rightThread == true) { // Threaded links secondary priority
      cout << curr->data << ", ";
      curr = curr->right;
      continue;
    }
  }
  cout << curr->data << ", ";

  while (curr->right != nullptr)
    curr = curr->right;

  while (curr != root) {
    if (curr->leftThread == false) {
      reversetraversal.push(
          curr->data); // Pushing to stack for printing in reverse
      curr = curr->left;
      if (curr->rightThread == false) {
        while (curr->rightThread == false)
          curr = curr->right;
      }
      continue;
    }

    if (curr->leftThread == true) {
      reversetraversal.push(curr->data);
      curr = curr->left;
      continue;
    }
  }

  while (!reversetraversal.empty()) {
    cout << reversetraversal.top() << ", ";
    reversetraversal.pop();
  }
}

/** Get Height: Calls the height helper method and returns the height
Precondition:ThreadedBST tree object must exist.
Postcondition: Returns the height of the tree*/
int ThreadedBST::getHeight() { return heightHelper(root); }

/** Height Helper: Recursive method that traverses through the tree and
tallys up the height of the tree.
Precondition:ThreadedBST tree object must exist
Postcondition: Returns the height of the tree*/
int ThreadedBST::heightHelper(TreeNode *node) const {
  if (node == nullptr) {
    return 0;
  } else {
    return 1 + max(heightHelper(node->left), heightHelper(node->right));
  }
}

/** Get Count: Returns the TreeNode count of the ThreadedBST tree
Precondition:ThreadedBST tree object must exist
Postcondition: Returns int count*/
int ThreadedBST::getCount() const { return count; }

/** Operator=: Overloaded operator used for
easily creating copy constructors
Precondition: A tree must already exist for this to*/
ThreadedBST &ThreadedBST::operator=(const ThreadedBST &tree) {
  if (tree.root == nullptr) {
    this->root = nullptr;
  } else {
    copy(tree.root);
    thread();
  }
  return *this;
}