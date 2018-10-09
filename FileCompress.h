#pragma once
#include<iostream>
#include<assert.h>
#include<string>
#include"HuffmanTree.h"

#define __DEBUG__

//ÿ���ַ�����Ϣ
struct CharInfo
{
	char _ch;
	long long _count;
	string _code;//Huffman����

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
	//ѹ����input.txt -> input.txt.huffman
	void Compress(const char* file)//�ļ���ʵ���ַ���
	{
		assert(file);

		//1ͳ���ַ����ֵĴ���
		FILE* fout = fopen(file, "rb");//�����ļ�ָ��
		char ch = fgetc(fout);
		while (!feof(fout))
		{
			_infos[(unsigned char)ch]._count++;//���ֱ����Ǹ���
			ch = fgetc(fout);
		}

		//2����HuffmantTree
		      //��Ӧ��ֻ��Ȩֵ��Ӧ����CharInfor
		      //256���ַ��в���ÿ���ַ������õ���ֻ����Ч�ַ�����������HuffmanTree,�����зǷ�ֵ
		CharInfo invalid;
		invalid._count = 0;
		HuffmanTree<CharInfo> tree(_infos, 256, invalid);

		//3����Huffman����
		string code;
		GetHuffmanCode(tree.GetRoot(),code);






		//д���ַ����ֵ���Ϣ
		_WriteConfig(file);





		//4.����ѹ���ļ�
		string compressFile = file;
		compressFile += ".huffman";
		FILE* fin = fopen(compressFile.c_str(), "wb");//string.c_str()��ȡstring ��ͷָ�룬�൱��ת��const char*;
		assert(fin);
		    //�ļ�ָ�룬ƫ�����������￪ʼ�����ƫ������SEEK_SET�����ļ���ʼλ�ã���SEEK_CUR(��ǰλ��)��SEEK_END(ĩβ)��
		fseek(fout, 0, SEEK_SET);//ԭ�ļ�ָ���Ѿ�����β�ˣ�Ҫ�������õ��ļ���ʼ
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
					fputc(value, fin);//��8��λ�����charд���ļ�
					value = 0;
					pos = 0;
				}
			}
			ch = fgetc(fout);
		}
		//���루���ļ�д���˵���value��ûд��8��λ��
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
	//��һ��ʵ��
	void GetHuffmanCode(Node* root)//�ҵ�Ҷ�ӽ������ص����ɱ���
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
				if (cur == parent->_left)//��·Ϊ0����·Ϊ1
					code.push_back('0');
				else
					code.push_back('1');
				cur = parent;
				parent = cur->_parent;
			}

			reverse(code.begin(), code.end());
			//_infos[root->_w._ch]._code = code;//�ص���⣻�ַ�����ASCLL��
			return;
		}
		GetHuffmanCode(root->_left);
		GetHuffmanCode(root->_right);

	}
	//�ڶ���ʵ��
	void GetHuffmanCode(Node* root, string code)//�ҵ�Ҷ�ӽ���������ɣ����ǿռ����Ĵ�
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


	//��ѹ�� input.txt.huffman->input.txt
	void Uncompress(const char* file)//����Ϊѹ���ļ�
	{
		assert(file);
		string uncompressFile = file;
		size_t pos = uncompressFile.rfind('.');//����ǰ��
		assert(pos != string::npos);//npos������޷�������
		uncompressFile.erase(pos, uncompressFile.size() - pos);

#ifdef __DEBUG__
		uncompressFile += ".unhuffman";
#endif

		FILE* fout = fopen(file, "rb");
		assert(fout);
		FILE* fin = fopen(uncompressFile.c_str(), "wb");
		assert(fin);

		//1���ؽ�huffman��
		CharInfo invalid;
		invalid._count = 0;
		HuffmanTree<CharInfo> tree(_infos, 256, invalid);

		Node* root = tree.GetRoot();
		Node* cur = root;
		long long Filesize = root->_w._count;//�ļ������ַ�����֮�ͣ�huffman�����ص㣩

		//2����ѹ��
		char value = fgetc(fout);
		while (!feof(fout))//feof()�ж��ļ��Ƿ�
		{
			for (int pos = 0; pos < 8; ++pos)
			{
				if (value &(1 << pos))//posλ��Ϊ1������������
				{
					cur = cur->_right;
				}
				else
				{
					cur = cur->_left;
				}
				if (cur->_left == NULL
					&&cur->_right == NULL)//�ߵ���Ҷ�ӽ��
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