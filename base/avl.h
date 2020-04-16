//
// AVL tree
//

#pragma once

#include <iostream>
#include <algorithm>  // std::max
#include <cstdlib>    // std::abs
#include <stack>
#include <vector>
#include <cassert>

using namespace std;

template<typename TKey, typename TValue>
class avltree
{
private:
  struct NODE
  {
    TKey   Key;
    TValue Value;
    int    Height;
    NODE*  Left;
    NODE*  Right;
  };

  NODE* Root;  // pointer to root node of tree (nullptr if empty)
  int   Size;  // # of nodes in the tree (0 if empty)
	
	void _inorder(NODE* cur);
	void _inorder_keys(NODE* cur, std::vector<TKey>& V);
	void _inorder_values(NODE* cur, std::vector<TValue>& V);
	void _inorder_heights(NODE* cur, std::vector<int>& V);
	void _copytree(NODE* cur);
	void _destructor(NODE* cur);
	void _RotateToFix(NODE* Parent, NODE* N);
	void _RightRotate(NODE* Parent, NODE* N);
	void _LeftRotate(NODE* Parent, NODE* N);

public:
  avltree();
  avltree(const avltree& other);
  ~avltree();
  
  avltree& operator=(const avltree& other);

  int size();
  int height();
  
  void clear();
  
  TValue* search(TKey key);
  void insert(TKey key, TValue value);
  
  int distance(TKey k1, TKey k2);

  void inorder();
  std::vector<TKey> inorder_keys();
  std::vector<TValue> inorder_values();
  std::vector<int> inorder_heights();

};
