#pragma once
#include"Heap.h"
#include <functional>

template <class W>
struct HuffmanTreeNode
{
	HuffmanTreeNode<W>* _left;
	HuffmanTreeNode<W>* _right;
	HuffmanTreeNode<W>* _parent;//��������Ҷ�ӽ������Huffman������Ҫ������(������ǰ��

	W _w;//Ȩֵ

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
		//�÷º�������ڶ���ģ����������⣨�Ƚ�ָ�����ݣ�
		struct NodeCompare
		{
			bool operator() (Node* l, Node* r)
			{
				return l->_w < r->_w;//������С��
			}
		};
		//�Ƚ�һ��С��
		Heap<Node*, NodeCompare> minHeap;//Ϊʲôģ���һ��������ָ�룿
		for (size_t i = 0; i < n; ++i)
		{
			if (a[i] != invalid)//���ǷǷ�ֵ����������ţ����ܽ�HuffmanTree
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

		_root = minHeap.Top();//С���ڵ����һ����㣨�Ǹ�ָ�룩
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