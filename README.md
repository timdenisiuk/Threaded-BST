# Threaded-BST
A Binary Search Tree with threads

## Project Overview

This project involves taking the well know binary search tree, and adding a threaded attribute to it. Threading a binary search tree
involves adding pointers to the next value in the sequence. An example of this would be taking the right most leaf of the left side of a root,
and connecting these two together with a thread. This connection goes both ways.

The main goal of this is to make traversing a binary search tree from a recursive one to an iterative one. An added contraint to this project
is to make the tree level on initalization

## Project Description

Most of the work done is in the initialization of the tree. First, we use a vector to take the values of our tree and seperate them out.
After doing that work, we then add them in the tree in a way that balances it all out and creates a level tree. The hard part is the actual threading of
said tree. When considering the threading process, we have to use a mix of recursion, as well as many test cases, to figure out which node gets threaded to which.

We also had to be able to remove a node from the tree when needed. This also boiled down to many test cases, writing out different methods for each test case
as to not confuse what was happening during each removal process. The only downfall to this program is that once a tree is threaded, it cannot be
added to, only deleted.
