/**
 * Michael Cho
 * Tim D
 *
 * CSS342
 *
 * This is the main driver file that is running the tests.
 * */

#include "threadedBST.h"
#include <cassert>
#include <iostream>

using namespace std;

// Testing constuctors, inorderPrint, and removeEven.
void test1() {
  cout << "\n****Begin Tests for Constuctor****\n";
  cout << "\nCreating ThreadedBST tree1 (n = 20):\n";
  ThreadedBST tree1(20);
  tree1.inorderPrint();

  cout << "\n\n****Testing Copy Constructor****\n";
  cout << "\nCopying tree1 into tree2:\n";
  ThreadedBST tree2(tree1);
  tree2.inorderPrint();
  cout << "\n\nDeleting tree2 even nodes:\n";
  tree2.removeEven();
  tree2.inorderPrint();

  cout << "\n\n****Testing Overloaded = Operator****\n";
  ThreadedBST tree3;
  cout << endl << "Copying tree1 into tree3:\n";
  tree3 = tree1;
  tree3.inorderPrint();
  cout << "\n\nDeleting tree3 even nodes:\n";
  tree3.removeEven();
  tree3.inorderPrint();
  cout << endl;
}

// Testing methods contains, add, and remove.
void test2() {
  // Creating tree
  ThreadedBST tree1;
  tree1.add(6);
  tree1.add(3);
  tree1.add(1);
  tree1.add(2);
  tree1.add(5);
  tree1.add(8);
  tree1.add(7);
  tree1.add(11);
  tree1.add(9);

  // Tree does not contain 13
  assert(!tree1.contains(13));

  // Adding node 13
  assert(tree1.add(13));
  assert(tree1.contains(13));
  tree1.thread();

  // Testing removal of node 13
  assert(tree1.remove(13));
  assert(!tree1.contains(13));

  // Printing current tree
  cout << "\nPrinting tree from test2:\n";
  tree1.inorderPrint();
}

// User test. Creates a tree with a user specified number of nodes.
// Then, copies the tree and deletes the even nodes from the copied
// tree.
void officialtest() {
  cout << "\n\n****Begin User Test****\n\n";
  cout << "Please enter number of tree nodes: ";
  int n;
  cin >> n;
  while (n <= 0) {
    cout << "Not a valid number, please input a number greater than 0";
    cin >> n;
  }
  cout << "\nCreating ThreadedBST tree with " << n << " nodes:\n";
  ThreadedBST tree(n);
  tree.inorderPrint();
  cout << endl << "\nMaking copy of tree and deleting even nodes:\n";
  ThreadedBST treecopy(tree);
  treecopy.removeEven();
  treecopy.inorderPrint();
}

int main() {
  test1();
  test2();
  // officialtest();
  cout << endl << endl << "Done." << endl;
  return 0;
}