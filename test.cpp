#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include"HuffmanTree.h"
#include"FileCompress.h"
using namespace std;

//void TestHuffmanTree()
//{
//	int a[] = { 1, 2, 3, 4 };
//	HuffmanTree<int> Ht(a, sizeof(a) / sizeof(a[0]));
//}

void testFileCompress()
{
	FileCompress fc;
	//fc.Compress("1.doc");
	//fc.Uncompress("1.doc.huffman");
	//fc.Compress("input.jpg");
	//fc.Uncompress("input.jpg.huffman");
	fc.Compress("input.txt");
	fc.Uncompress("input.txt.huffman");
}
int main()
{
	testFileCompress();

	return 0;
}