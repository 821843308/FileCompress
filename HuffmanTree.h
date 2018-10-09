#pragma once
#include"Heap.h"
#include <functional>

template <class W>
struct HuffmanTreeNode
{
	HuffmanTreeNode<W>* _left;
	HuffmanTreeNode<W>* _right;
	HuffmanTreeNode<W>* _parent;//三叉链，叶子结点生成Huffman编码需要三叉链(倒着往前）

	W _w;//权值

	HuffmanTreeNode(const W& w)
		:_left(NULL)
		, _right(NULL)
		,_parent(NULL)
		, _w(w)
	{}
};

template<class W>
class HuffmanTree
{
	typedef HuffmanTreeNode<W> Node;
public:
	HuffmanTree()
		:_root(NULL)
	{}

	HuffmanTree(W* a, size_t n,const W& invalid)
	{
		//用仿函数解决第二个模板参数的问题（比较指针内容）
		struct NodeCompare
		{
			bool operator() (Node* l, Node* r)
			{
				return l->_w < r->_w;//建的是小堆
			}
		};
		//先建一个小堆
		Heap<Node*, NodeCompare> minHeap;//为什么模板第一个参数是指针？
		for (size_t i = 0; i < n; ++i)
		{
			if (a[i] != invalid)//不是非法值才能往堆里放，才能建HuffmanTree
			{
				minHeap.Push(new Node(a[i]));
			}
	
		}

		while (minHeap.Size()>1)
		{
			Node* left = minHeap.Top();
			minHeap.Pop();
			Node* right = minHeap.Top();
			minHeap.Pop();

			Node* parent = new Node(left->_w + right->_w);
			parent->_left = left;
			parent->_right = right;

			left->_parent = parent;
			right->_parent = parent;

			minHeap.Push(parent);
		}

		_root = minHeap.Top();//小堆内的最后一个结点（是个指针）
	}
	Node* GetRoot()
	{
		return _root;
	}

private:
	HuffmanTree(const HuffmanTree<W> &);
	HuffmanTree<W> operator = (const HuffmanTree<W>&);
protected :
	Node* _root;
};