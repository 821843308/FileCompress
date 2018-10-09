#pragma once
#include<iostream>
#include <vector>
#include <functional>
#include<assert.h>
using namespace std;

//�º������ô�����Խ����Ҳ���Խ�С�ѣ�����ֻ��Щ���Ų�ͬ��
template<class T>
struct Less
{
	bool operator ()(const T& l, const T& r)//С�ڷ���ture
	{
		return l < r;
	}
};

template<class T>
struct Greater
{
	bool operator ()(const T& l, const T& r)//���ڷ���ture
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

		//����
		for (int i = (_a.size() - 2 / 2); i >= 0;--i)
		{
			AdjustDown(i);
		}
	}

	//���µ����㷨
	void AdjustDown(int root)//ǰ�������������Ǵ��
	{
		Compare com;

		int parent = root;
		int child = parent * 2 + 1;
		while (child < _a.size())
		{
			//ѡ������;����С��ѡС���ӣ���˭С��
			if (child + 1 < _a.size()
				&& com(_a[child + 1] , _a[child]))
			{
				++child;

			}

			if (com(_a[child],_a[parent]))//��ĺ��Ӵ��ڸ��ڵ㣬�������������
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
			//�������
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
	//���ϵ����㷨
	void AdjustUp(int child)//���һ������±�
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

    //���ڶ����ӽ��
	void Push(const T& x)
	{
		_a.push_back(x);
		AdjustUp(_a.size() - 1);
	}

    //ɾ���Ѷ�
	void Pop()
	{
		assert(!_a.empty());
		swap(_a[0], _a[_a.size() - 1]);
		_a.pop_back();
		AdjustDown(0);

	}
	//���ضѶ�Ԫ��
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
		//����
		for (int i = (n - 2) / 2; i >= 0; --i)//��һ����Ҷ�ӽڵ㿪ʼ�����µ����㷨
		{
			AdiustDown(a, n, i);
		}

		//����
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

