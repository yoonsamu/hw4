#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#endif
#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int depth(Node * root);

bool equalPaths(Node * root)
{
  
  // Add your code below
  // check for the two leaves on the other levels, which might hold false all the way up (trickle?)
   // check if root node is empty
  if (root == nullptr) {
    return true;
  }
  // check if node is a leaf
  if (root->right == nullptr && root->left == nullptr) {
    return true;
  }
  // check for the leaves on both sides
  bool lhand = equalPaths(root->left);
  bool rhand = equalPaths(root->right);

  // check if the leafe is on one side anad empty also
  if(root->left == nullptr || root->right == nullptr) {
    return lhand && rhand;
  }

  // there might be a leaf on both sides
  if (rhand && lhand && (depth(root->left) != depth(root->right))) {
    return false;
  }
  // only return false if there's only one case where leaves are on different levels
  // false trickles up
  return lhand && rhand;
}


int depth(Node* root) {
  if (root == nullptr) {
    return 0;
  }
  int left = depth(root->left);
  int right = depth(root->right);
  
  if (left > right) {
    return left + 1;
  }
  return right + 1;
}

