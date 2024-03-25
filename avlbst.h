#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}
/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insertHelp(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    void deleteHelp(AVLNode<Key, Value>* n, int diff);
    void turnR(AVLNode<Key, Value>* n);
    void turnL(AVLNode<Key, Value>* n);


};
/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    AVLNode<Key, Value>* x = ((AVLNode<Key, Value>*)(this->root_));

    // TODO
    if (x == NULL) {
      this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
      ((AVLNode<Key, Value>*)(this->root_))->setBalance(0);
      return;
    }
    else {
      // create temp pointer which is x, for bst
      while (x != NULL) {
        // for when same key is found
        if (x->getKey() == new_item.first) {
          x->setValue(new_item.second);
          return;
        }
        // go to the left side
        if (new_item.first < x->getKey()) {
          //if the left side or child is DNE
          if (x->getLeft() == NULL) {
            AVLNode<Key, Value>* toAdd = new AVLNode<Key, Value>(new_item.first, new_item.second, x);
            x->setLeft(toAdd);
            x = x->getLeft();
            break;
          }
          else {
            x = x->getLeft();
          }
        }
        // go to the right side
        else if (new_item.first >= x->getKey()) {
          // if the right side doesn't exist
          if (x->getRight() == NULL) {
            AVLNode<Key, Value>* toAdd = new AVLNode<Key, Value>(new_item.first, new_item.second, x);
            x->setRight(toAdd);
            x = x->getRight();
            break;
          }
          else {
            x = x->getRight();
          }
        }
      }
    }
    // no more insert

    x->setBalance(0);

    // node that isn't the root
    if (x->getParent() != NULL) {
        if (x->getParent()->getBalance() == -1) {
            x->getParent()->setBalance(0);
        }
        else if (x->getParent()->getBalance() == 1) {
            x->getParent()->setBalance(0);
        }
        else if (x->getParent()->getBalance() == 0) {
            if (x == x->getParent()->getLeft()) {
                x->getParent()->setBalance(-1);
            }
            else {
                x->getParent()->setBalance(1);
            }
            insertHelp(x->getParent(), x);
        }
    }
}
template<class Key, class Value>
void AVLTree<Key, Value>::turnR(AVLNode<Key, Value>* n) {
  // node is the root
  if (this->root_ == n) {
    AVLNode<Key, Value>* a = n->getLeft();
    a->setParent(NULL);
    n->setParent(a);
    this->root_ = a;

    if (a->getRight() != NULL) {
      n->setLeft(a->getRight());
      a->getRight()->setParent(n);
    }
    else {
      n->setLeft(NULL);
    }
    
    a->setRight(n);
  }
  else {
    AVLNode<Key, Value>* y = n->getParent();
    AVLNode<Key, Value>* a = n->getLeft();
    n->setParent(a);
    a->setParent(y);

    if (n == y->getRight()) {
      y->setRight(a);
    }
    else {
      y->setLeft(a);
    }

    if (a->getRight() != NULL) {
      n->setLeft(a->getRight());
      a->getRight()->setParent(n);
    }
    else {
      n->setLeft(NULL);
    }

    a->setRight(n);
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::turnL(AVLNode<Key, Value>* n) {
  // this for when node is root
  if (this->root_ == n) {
    AVLNode<Key, Value>* z = n->getRight();
    z->setParent(NULL);
    n->setParent(z);
    this->root_ = z;

    if (z->getLeft() != NULL) {
      n->setRight(z->getLeft());
      z->getLeft()->setParent(n);
    }
    else {
      n->setRight(NULL);
    }
    
    z->setLeft(n);
  }
  else {
    AVLNode<Key, Value>* y = n->getParent();
    AVLNode<Key, Value>* z = n->getRight();
    n->setParent(z);
    z->setParent(y);

    if (n == y->getRight()) {
      y->setRight(z);
    }
    else {
      y->setLeft(z);
    }

    if (z->getLeft() != NULL) {
      n->setRight(z->getLeft());
      z->getLeft()->setParent(n);
    }
    else {
      n->setRight(NULL);
    }

    z->setLeft(n);
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertHelp(AVLNode<Key, Value>* y, AVLNode<Key, Value>* n) {
    if (y == NULL || y->getParent() == NULL) {
        return;
    }
    AVLNode<Key, Value>* b = y->getParent();
    int8_t leftOrRight = -1;
    if (y == b->getRight()) {
      leftOrRight = 1;
    }

    b->setBalance(b->getBalance() + leftOrRight);
      
    if (b->getBalance() == 0) {
      return;
    }
    else if (b->getBalance() == leftOrRight) {
      insertHelp(b, y);
    }
    else {
      //zig zig left
      if (n == y->getLeft() && y == b->getLeft()) {
        turnR(b);
        b->setBalance(0);
        y->setBalance(0);
      }

      //zig zig right
      else if (n == y->getRight() && y == b->getRight()) {
        turnL(b); 
        b->setBalance(0);
        y->setBalance(0);
      }

      //zig zag left and then right
      else if (n == y->getRight() && y == b->getLeft()) {
        turnL(y);
        turnR(b);
        switch (n->getBalance()) {
          case -1:
            n->setBalance(0);
            y->setBalance(0);
            b->setBalance(1);
            break;
          case 0:
            y->setBalance(0);
            b->setBalance(0);
            break;
          case 1:
            n->setBalance(0);
            y->setBalance(-1);
            b->setBalance(0);
            break;
        }
      }

      //zig zag both but right first
      else if (n == y->getLeft() && y == b->getRight()) {
        turnR(y);
        turnL(b);
        switch (n->getBalance()) {
          case -1:
            n->setBalance(0);
            y->setBalance(1);
            b->setBalance(0);
            break;
          case 0:
            y->setBalance(0);
            b->setBalance(0);
            break;
          case 1:
            n->setBalance(0);
            y->setBalance(0);
            b->setBalance(-1);
            break;
        }
      }
    }    
}



/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* x = (AVLNode<Key, Value>*)this->internalFind(key);
    int diff = 0;

    // if the key is there
    if (x != NULL) {
      // case for two child
      if (x->getRight() != NULL && x->getLeft() != NULL) {
        nodeSwap((AVLNode<Key, Value>*)(this->predecessor(x)), x);

        if (x->getParent() != NULL) {
          if (x == x->getParent()->getLeft()) {
            diff = 1;
          }
          else {
            diff = -1;
          }
        }

        // case for one
        if (x->getRight() != NULL) {
          //if temp x) is the left side
          if (x == x->getParent()->getLeft()) {
            x->getParent()->setLeft(x->getRight());
            x->getRight()->setParent(x->getParent());
          }
          //else which means right
          else {
            x->getParent()->setRight(x->getRight());
            x->getRight()->setParent(x->getParent());
          }
        }
        else if (x->getLeft() != NULL) {
          if (x == x->getParent()->getLeft()) {
            x->getParent()->setLeft(x->getLeft());
            x->getLeft()->setParent(x->getParent());
          }
          else {
            x->getParent()->setRight(x->getLeft());
            x->getLeft()->setParent(x->getParent());
          }
        }
        // for zero child
        else {
          if (x == x->getParent()->getLeft()) {
            x->getParent()->setLeft(NULL);
          }
          else {
            x->getParent()->setRight(NULL);
          }
        }
      }

      // for one child
      else if (x->getRight() != NULL) {
        //if x is the root
        if (x->getParent() != NULL) {
          if (x == x->getParent()->getLeft()) {
            diff = 1;
          }
          else {
            diff = -1;
          }
        }
        if (x == this->root_) {
          this->root_ = x->getRight();
          x->getRight()->setParent(NULL);
        }
        //if temp or x is the left side again
        else if (x == x->getParent()->getLeft()) {
          x->getParent()->setLeft(x->getRight());
          x->getRight()->setParent(x->getParent());
        }
        //else means its the right
        else {
          x->getParent()->setRight(x->getRight());
          x->getRight()->setParent(x->getParent());
        }
      }
      else if (x->getLeft() != NULL) {
        if (x->getParent() != NULL) {
          if (x == x->getParent()->getLeft()) {
            diff = 1;
          }
          else {
            diff = -1;
          }
        }
        if (x == this->root_) {
          this->root_ = x->getLeft();
          x->getLeft()->setParent(NULL);
        }
        else if (x == x->getParent()->getLeft()) {
          x->getParent()->setLeft(x->getLeft());
          x->getLeft()->setParent(x->getParent());
        }
        else {
          x->getParent()->setRight(x->getLeft());
          x->getLeft()->setParent(x->getParent());
        }
      }


      // case for no children
      else {
        if (x->getParent() != NULL) {
          if (x == x->getParent()->getLeft()) {
            diff = 1;
          }
          else {
            diff = -1;
          }
        }
        if (x == this->root_) {
          this->root_ = NULL;
        }
        else if (x == x->getParent()->getLeft()) {
          x->getParent()->setLeft(NULL);
        }
        else {
          x->getParent()->setRight(NULL);
        }
      }
      AVLNode<Key, Value>* y = x->getParent();

      delete x;

      deleteHelp(y, diff);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::deleteHelp(AVLNode<Key, Value>* n, int diff) {
  if (n == NULL) {
    return;
  }
  AVLNode<Key, Value>* y = n->getParent();
  int nextDiff = 0;

  if (n->getParent() != NULL) {
    if (n == y->getLeft()){
      nextDiff = 1;
    }
    else {
      nextDiff = -1;
    }
  }


  //right node
  if (n->getBalance() + diff == -2) {
    int leftHeight = this->depth(n->getLeft());
    int rightHeight = this->depth(n->getRight());
    AVLNode<Key, Value>* c = n->getLeft();
    if (rightHeight > leftHeight) {
      c = n->getRight();
    }
    if (c->getBalance() == -1) {
      turnR(n);
      n->setBalance(0);
      c->setBalance(0);
      deleteHelp(y, nextDiff);
    }
    else if (c->getBalance() == 0) {
      turnR(n);
      n->setBalance(-1);
      c->setBalance(1);
    }
    else if (c->getBalance() == 1) {
      AVLNode<Key, Value>* b = c->getRight();
      turnL(c);
      turnR(n);
      if (b->getBalance() == 1) {
        n->setBalance(0);
        c->setBalance(-1);
        b->setBalance(0);
      }
      else if (b->getBalance() == 0) {
        n->setBalance(0);
        c->setBalance(0);
        b->setBalance(0);
      }
      else if (b->getBalance() == -1) {
        n->setBalance(1);
        c->setBalance(0);
        b->setBalance(0);
      }
      deleteHelp(y, nextDiff);
    }
  }

  //left node
  else if (n->getBalance() + diff == 2) {
    int leftHeight = this->depth(n->getLeft());
    int rightHeight = this->depth(n->getRight());
    AVLNode<Key, Value>* c = n->getLeft();
    if (rightHeight > leftHeight) {
      c = n->getRight();
    }

    if (c->getBalance() == 1) {
      turnL(n);
      n->setBalance(0);
      c->setBalance(0);
      deleteHelp(y, nextDiff);
    }
    else if (c->getBalance() == 0) {
      turnL(n);
      n->setBalance(1);
      c->setBalance(-1);
    }
    else if (c->getBalance() == -1) {
      AVLNode<Key, Value>* b = c->getLeft();
      turnR(c);
      turnL(n);
      if (b->getBalance() == -1) {
        n->setBalance(0);
        c->setBalance(1);
        b->setBalance(0);
      }
      else if (b->getBalance() == 0) {
        n->setBalance(0);
        c->setBalance(0);
        b->setBalance(0);
      }
      else if (b->getBalance() == 1) {
        n->setBalance(-1);
        c->setBalance(0);
        b->setBalance(0);
      }
      deleteHelp(y, nextDiff);
    }
  }

  else if (n->getBalance() + diff == -1) {
    n->setBalance(-1);
  }
  else if (n->getBalance() + diff == 1) {
    n->setBalance(1);
  }

  else if (n->getBalance() + diff == 0) {
    n->setBalance(0);
    deleteHelp(y, nextDiff);
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif