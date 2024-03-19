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
  if (root == nullptr) {
    return true;
  }
  //
  if (root->right == nullptr && root->left == nullptr) {
    return true;
  }
  //
  bool lhand = equalPaths(root->left);
  bool rhand = equalPaths(root->right);

  if(root->left == nullptr || root->right == nullptr) {
    return lhand && rhand;
  }

  if (rhand && lhand && (depth(root->left) != depth(root->right))) {
    return false;
  }
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

