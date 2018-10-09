#pragma once
#include<iostream>
#include <vector>
#include <functional>
#include<assert.h>
using namespace std;

//仿函数，让代码可以建大堆也可以建小堆（代码只有些符号不同）
template<class T>
struct Less
{
	bool operator ()(const T& l, const T& r)//小于返回ture
	{
		return l < r;
	}
};

template<class T>
struct Greater
{
	bool operator ()(const T& l, const T& r)//大于返回ture
	{
		return l > r;
	}
};
template<class T, class Compare>
class Heap
{
public:
	Heap()
	{}
	Heap(T* a,size_t n)
	{
		_a.reserve(n);
		for (size_t i = 0; i < n; i++)
		{
			_a.push_back(a[i]);
		}

		//建堆
		for (int i = (_a.size() - 2 / 2); i >= 0;--i)
		{
			AdjustDown(i);
		}
	}

	//向下调整算法
	void AdjustDown(int root)//前提左右子树都是大堆
	{
		Compare com;

		int parent = root;
		int child = parent * 2 + 1;
		while (child < _a.size())
		{
			//选出大孩子;若是小堆选小孩子，比谁小；
			if (child + 1 < _a.size()
				&& com(_a[child + 1] , _a[child]))
			{
				++child;

			}

			if (com(_a[child],_a[parent]))//大的孩子大于根节点，交换后继续调整
			{
				swap(_a[child], _a[parent]);
			    parent = child;
				child = parent * 2 + 1;
			}
			else
			{
				break;
			}
		}
	}

	void AdjustDown(int* a, size_t n, int root)
	{
		int parent = root;
		int child = parent * 2 + 1;
		while (child < n)
		{
			//找最大孩子
			if (child + 1 < n
				&&a[child + 1] > a[child])
			{
				++child;
			}
			if (a[child]>a[parent])
			{
				sawp(a[child], a[parent]);
				parent = child;
				child = parent * 2 + 1;
			}
			else
				break;
		}
	}
	//向上调整算法
	void AdjustUp(int child)//最后一个结点下标
	{
		Compare com;
		int parent = (child - 1) / 2;
		while (child>0)
		{
			//if (_a[child] > _a[parent])
			if (com(_a[child], _a[parent]))
			{
				swap(_a[child], _a[parent]);
				child = parent;
				parent = (child - 1) / 2;
			}
			else
			{
				break;
			}
		}
	}

    //往内堆增加结点
	void Push(const T& x)
	{
		_a.push_back(x);
		AdjustUp(_a.size() - 1);
	}

    //删除堆顶
	void Pop()
	{
		assert(!_a.empty());
		swap(_a[0], _a[_a.size() - 1]);
		_a.pop_back();
		AdjustDown(0);

	}
	//返回堆顶元素
	T& Top()
	{
		assert(!_a.empty());
		return _a[0];
	}

	size_t Size()
	{
		return _a.size();
	}
	void HeapSort(int* a, size_t n)
	{
		//建堆
		for (int i = (n - 2) / 2; i >= 0; --i)//第一个非叶子节点开始，向下调整算法
		{
			AdiustDown(a, n, i);
		}

		//排序
		size_t end = n - 1;
		while (end > 0)
		{
			swap(a[0], a[end]);
			AdiustDown(a, end, 0);
			--end;
		}

	}
private:
	vector<T> _a;
};

