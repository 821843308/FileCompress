#pragma once
#include<iostream>
#include<assert.h>
#include<string>
#include"HuffmanTree.h"

#define __DEBUG__

//每个字符的信息
struct CharInfo
{
	char _ch;
	long long _count;
	string _code;//Huffman编码

	bool operator != (const CharInfo& info)
	{
		return _count != info._count;
	}

	CharInfo operator +(const CharInfo& info)
	{
		CharInfo ret;
		ret._count = _count + info._count;
		return ret;
	}
	bool operator <(const CharInfo& info)
	{
		return _count < info._count;
	}

};
class FileCompress
{
	typedef HuffmanTreeNode<CharInfo> Node;

public:
	FileCompress()
	{
		for (size_t i = 0; i < 256; i++)
		{
			_infos[i]._ch = i;
			_infos[i]._count = 0;
		}
	}
	//压缩；input.txt -> input.txt.huffman
	void Compress(const char* file)//文件其实是字符串
	{
		assert(file);

		//1统计字符出现的次数
		FILE* fout = fopen(file, "rb");//定义文件指针
		char ch = fgetc(fout);
		while (!feof(fout))
		{
			_infos[(unsigned char)ch]._count++;//汉字编码是负的
			ch = fgetc(fout);
		}

		//2构建HuffmantTree
		      //不应该只有权值，应该是CharInfor
		      //256个字符中不是每个字符都有用到，只有有效字符才用来构建HuffmanTree,所以有非法值
		CharInfo invalid;
		invalid._count = 0;
		HuffmanTree<CharInfo> tree(_infos, 256, invalid);

		//3生成Huffman编码
		string code;
		GetHuffmanCode(tree.GetRoot(),code);






		//写入字符出现的信息
		_WriteConfig(file);





		//4.生成压缩文件
		string compressFile = file;
		compressFile += ".huffman";
		FILE* fin = fopen(compressFile.c_str(), "wb");//string.c_str()获取string 的头指针，相当与转成const char*;
		assert(fin);
		    //文件指针，偏移量，从哪里开始计算的偏移量（SEEK_SET（从文件起始位置），SEEK_CUR(当前位置)，SEEK_END(末尾)）
		fseek(fout, 0, SEEK_SET);//原文件指针已经读到尾了，要重新设置到文件开始
		ch = fgetc(fout);

	    char value = 0;//
		int pos = 0;

		while (!feof(fout))
		{
			string& code = _infos[(unsigned char)ch]._code;
			for (size_t i = 0; i < code.size(); ++i)
			{
				if (code[i] == '0')
				{
					value &= ~(1 << pos);
				}
				else if (code[i] == '1')
				{
					value |= (1 << pos);
				}
				else
				{
					assert(false);
				}
				++pos;
				if (pos == 8)
				{
					fputc(value, fin);//满8个位后将这个char写入文件
					value = 0;
					pos = 0;
				}
			}
			ch = fgetc(fout);
		}
		//补齐（当文件写完了但是value还没写满8个位）
		if (pos > 0)
		{
			fputc(value, fin);
		}
		fclose(fout);
		fclose(fin);

	}
	void _WriteConfig(const char* filename)
	{
		string write = filename;
		write += ".config";
		FILE* fin = fopen(write.c_str(), "wb");
		for (int i = 0; i < 256; i++)
		{
			if (_infos[i]._count)
			{
				fputc(_infos[i]._ch, fin);
				fputc(',', fin);

			}
		}
		
	}
	//第一种实现
	void GetHuffmanCode(Node* root)//找到叶子结点后，往回倒生成编码
	{
		if (root == NULL)
			return;

		if (root->_left == NULL
			&&root->_right == NULL)
		{
			//string code;
			string& code = _infos[(unsigned char)root->_w._ch]._code;

			Node* cur = root;
			Node* parent = cur->_parent;
			while (parent)
			{
				if (cur == parent->_left)//左路为0，右路为1
					code.push_back('0');
				else
					code.push_back('1');
				cur = parent;
				parent = cur->_parent;
			}

			reverse(code.begin(), code.end());
			//_infos[root->_w._ch]._code = code;//重点理解；字符就是ASCLL码
			return;
		}
		GetHuffmanCode(root->_left);
		GetHuffmanCode(root->_right);

	}
	//第二种实现
	void GetHuffmanCode(Node* root, string code)//找到叶子结点后编码生成，但是空间消耗大
	{
		if (root == NULL)
			return;

		if (root->_left == NULL
			&&root->_right == NULL)
		{
			_infos[(unsigned char)root->_w._ch]._code = code;
			return;
		}
		GetHuffmanCode(root->_left, code + '0');
		GetHuffmanCode(root->_right, code + '1');
	}


	//解压缩 input.txt.huffman->input.txt
	void Uncompress(const char* file)//参数为压缩文件
	{
		assert(file);
		string uncompressFile = file;
		size_t pos = uncompressFile.rfind('.');//后往前找
		assert(pos != string::npos);//npos是最大无符号整形
		uncompressFile.erase(pos, uncompressFile.size() - pos);

#ifdef __DEBUG__
		uncompressFile += ".unhuffman";
#endif

		FILE* fout = fopen(file, "rb");
		assert(fout);
		FILE* fin = fopen(uncompressFile.c_str(), "wb");
		assert(fin);

		//1、重建huffman树
		CharInfo invalid;
		invalid._count = 0;
		HuffmanTree<CharInfo> tree(_infos, 256, invalid);

		Node* root = tree.GetRoot();
		Node* cur = root;
		long long Filesize = root->_w._count;//文件所有字符个数之和（huffman树的特点）

		//2、解压缩
		char value = fgetc(fout);
		while (!feof(fout))//feof()判断文件是否
		{
			for (int pos = 0; pos < 8; ++pos)
			{
				if (value &(1 << pos))//pos位置为1，往树的右走
				{
					cur = cur->_right;
				}
				else
				{
					cur = cur->_left;
				}
				if (cur->_left == NULL
					&&cur->_right == NULL)//走到了叶子结点
				{
					fputc(cur->_w._ch, fin);
					cur = root;
					if (--Filesize == 0)
					{
						break;
					}
				}
			}

			value = fgetc(fout);
		}
		fclose(fout);
		fclose(fin);
	}
private:
	CharInfo _infos[256];
};