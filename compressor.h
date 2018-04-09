#include "stdafx.h"
#pragma once
#include "BMP.h"
#pragma pack()  
typedef struct {
	unsigned char uch;
	unsigned long weight;
}TmpNode;
typedef struct Node {
	unsigned char uch;
	unsigned long weight;
	char *code;                     // 字符对应的哈夫曼编码  
	int parent, lchild, rchild;
	Node() { parent = 0; }
} HuffmanNode, *HuffmanTree;
class Compressor
{
private:
	unsigned char RGBData[4000][3];//能容纳最大4000像素的图片
	unsigned char GrayData[4000];
	FILE * fpBMP, *fpGray;
	BITMAPFILEHEADER * header;
	BITMAPINFOHEADER * info;
	RGBQUAD * quad;
	int openFile(const char *, const char *);
	void select(HuffmanNode*, unsigned int, int *);
	void createTree(HuffmanNode *, unsigned int, unsigned int);
	void createCode(HuffmanNode *, unsigned);
	void  closeFile();
public:
	int BmpToGray(const char *, const char *);
	int Decode(const char *, const char *);
	int Encode(const char *, const char *);

};