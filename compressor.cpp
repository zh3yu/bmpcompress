#include"stdafx.h"
#include "compressor.h"  


int Compressor::openFile(const char *in, const char *out) {
	if (!(fpBMP = fopen(in, "rb")))
	{
		printf("打开文件失败\n");
		return 0;
	}
	if (!(fpGray = fopen(out, "wb")))
	{
		printf("创建文件失败\n");
		return 0;
	}
	return 1;
}
void Compressor::select(HuffmanNode *huf_tree, unsigned int n, int *s1)
{
	unsigned long m = ULONG_MAX;
	for (int i = 0; i<n; i++)
		if (!huf_tree[i].parent&&huf_tree[i].weight<m)
		{
			m = huf_tree[i].weight;
			*s1 = i;
		}
	huf_tree[*s1].parent = 1;
}
void Compressor::createTree(HuffmanNode *huf_tree, unsigned int char_kinds, unsigned int node_num)
{
	unsigned int i;
	int s1, s2;
	for (i = char_kinds; i < node_num; ++i)
	{
		select(huf_tree, i, &s1);
		select(huf_tree, i, &s2);
		huf_tree[s1].parent = huf_tree[s2].parent = i;
		huf_tree[i].lchild = s1;
		huf_tree[i].rchild = s2;
		huf_tree[i].weight = huf_tree[s1].weight + huf_tree[s2].weight;
	}
}
void Compressor::createCode(HuffmanNode *huf_tree, unsigned char_kinds)
{
	int cur, next, index;
	char *code_tmp = new char[256];
	code_tmp[255] = '\0';
	for (int i = 0; i<char_kinds; i++)
	{
		index = 256 - 1;
		for (cur = i, next = huf_tree[i].parent; next != 0; cur = next, next = huf_tree[cur].parent)
			if (huf_tree[next].lchild == cur)
				code_tmp[--index] = '0';
			else
				code_tmp[--index] = '1';
		huf_tree[i].code = new char[256 - index];
		strcpy(huf_tree[i].code, &code_tmp[index]);
	}
	delete[] code_tmp;
}
void Compressor::closeFile() {
	fclose(fpBMP);
	fclose(fpGray);
}
bool cmp(TmpNode &a, TmpNode &b) {
	return a.weight>b.weight;
}
int Compressor::Encode(const char * in, const char *out) {
	if (!openFile(in, out)) return 0;
	TmpNode *tmp_nodes = new TmpNode[256];
	for (int i = 0; i<256; i++) {
		tmp_nodes[i].weight = 0;
		tmp_nodes[i].uch = (unsigned char)i;
	}
	unsigned char tmp_char;
	unsigned long file_len = 0, node_num;
	unsigned int char_kinds = 256;
	HuffmanNode *huf_tree;
	while (fread(&tmp_char, 1, 1, fpBMP)) {
		file_len++;
		tmp_nodes[tmp_char].weight++;
	}
	sort(tmp_nodes, tmp_nodes + 256, cmp);
	for (int i = 0; i<char_kinds; i++)
		if (!tmp_nodes[i].weight)
			char_kinds = i;
	if (char_kinds == 1) {
		fwrite((char *)&char_kinds, sizeof(unsigned int), 1, fpGray);
		fwrite((char *)&tmp_nodes[0].uch, 1, 1, fpGray);
		fwrite((char *)&tmp_nodes[0].weight, sizeof(unsigned long), 1, fpGray);
	}
	else {
		node_num = 2 * char_kinds - 1;
		huf_tree = new HuffmanNode[node_num];
		for (int i = 0; i<char_kinds; i++)
		{
			huf_tree[i].uch = tmp_nodes[i].uch;
			huf_tree[i].weight = tmp_nodes[i].weight;
		}
		createTree(huf_tree, char_kinds, node_num);
		createCode(huf_tree, char_kinds);
		fwrite((char *)&char_kinds, sizeof(unsigned int), 1, fpGray);
		for (int i = 0; i<char_kinds; i++)
		{
			fwrite((char *)&huf_tree[i].uch, sizeof(unsigned char), 1, fpGray);
			fwrite((char *)&huf_tree[i].weight, sizeof(unsigned long), 1, fpGray);
		}
		fwrite((char *)&file_len, sizeof(unsigned long), 1, fpGray);
		fclose(fpBMP);
		fpBMP = fopen(in, "rb");
		unsigned char char_temp;
		char code_buf[256] = "\0";
		fread((char *)&char_temp, sizeof(unsigned char), 1, fpBMP);     // 每次读取8bits  
		while (!feof(fpBMP))
		{
			for (int i = 0; i < char_kinds; ++i)
				if (char_temp == huf_tree[i].uch)
					strcat(code_buf, huf_tree[i].code);
			while (strlen(code_buf) >= 8)
			{
				char_temp = 0;
				for (int i = 0; i < 8; ++i)
					char_temp = (char_temp << 1) | (code_buf[i] - '0');
				fwrite((char *)&char_temp, sizeof(unsigned char), 1, fpGray);       // 将字节对应编码存入文件  
				strcpy(code_buf, code_buf + 8);       // 编码缓存去除已处理的前八位  
			}
			fread((char *)&char_temp, sizeof(unsigned char), 1, fpBMP);     // 每次读取8bits  
		}
		unsigned int l = strlen(code_buf);
		if (l)
		{
			char_temp = 0;
			for (int i = 0; i <l; ++i)
				char_temp = (char_temp << 1) | (code_buf[i] - '0');
			char_temp <<= 8 - l;
			fwrite((char *)&char_temp, sizeof(unsigned char), 1, fpGray);       // 存入最后一个字节  
		}
		for (int i = 0; i < char_kinds; ++i)
			delete[] huf_tree[i].code;
		delete[]huf_tree;
	}
	delete[] tmp_nodes;
	closeFile();
	return 1;
}
int Compressor::Decode(const char * in, const char *out) {
	if (!openFile(in, out)) return 0;
	unsigned int i;
	unsigned long file_len;
	unsigned int char_kinds;
	unsigned int node_num;
	HuffmanTree huf_tree;
	unsigned char code_temp;
	unsigned int root;      // 保存根节点索引，供匹配编码使用  
	fread((char *)&char_kinds, sizeof(unsigned int), 1, fpBMP);
	if (char_kinds == 1)
	{
		fread((char *)&code_temp, sizeof(unsigned char), 1, fpBMP);
		fread((char *)&file_len, sizeof(unsigned long), 1, fpBMP);
		while (file_len--)
			fwrite((char *)&code_temp, sizeof(unsigned char), 1, fpGray);
	}
	else
	{
		node_num = 2 * char_kinds - 1;
		huf_tree = new HuffmanNode[node_num];
		for (i = 0; i < char_kinds; ++i)
		{
			fread((char *)&huf_tree[i].uch, sizeof(unsigned char), 1, fpBMP);
			fread((char *)&huf_tree[i].weight, sizeof(unsigned long), 1, fpBMP);
		}
		createTree(huf_tree, char_kinds, node_num);
		fread((char *)&file_len, sizeof(unsigned long), 1, fpBMP);
		root = node_num - 1;
		//开始解码 现在哈弗曼树已经构建好了  
		while (fread((char *)&code_temp, sizeof(unsigned char), 1, fpBMP))
		{
			//现在先取8bits数据，然后一位一位的找  
			for (i = 0; i<8; ++i)
			{
				//128=100000 
				if (code_temp & 128)
					root = huf_tree[root].rchild;
				else
					root = huf_tree[root].lchild;
			
				if (root< char_kinds)
				{
					fwrite((char *)&huf_tree[root].uch, sizeof(unsigned char), 1, fpGray);
					root = node_num - 1;        // 复位为根索引，匹配下一个字符  
				}
				code_temp <<= 1;      // 将编码缓存的下一位移到最高位，供匹配  
			}
		}
		delete[] huf_tree;
	}
	closeFile();
	return 1;
}
int Compressor::BmpToGray(const char *in, const char *out) {
	if (!openFile(in, out)) return 0;
	header = new BITMAPFILEHEADER;
	info = new BITMAPINFOHEADER;
	quad = new RGBQUAD[256];
#ifdef DEBUG
	cout<<sizeof(BITMAPFILEHEADER);
#endif
	int x=fread(header, sizeof(BITMAPFILEHEADER),1, fpBMP);
	int y=fread(info, sizeof(BITMAPINFOHEADER), 1, fpBMP);

	info->biBitCount = 8;
	info->biSizeImage = ((info->biWidth * 3 + 3) / 4) * 4 * info->biHeight;
	header->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
	header->bfSize = header->bfOffBits + info->biSizeImage;

	int i, j, k;
	for (i = 0; i<256; i++)
		quad[i].rgbBlue = quad[i].rgbGreen = quad[i].rgbRed = i;

	fwrite(header, sizeof(BITMAPFILEHEADER), 1, fpGray);
	fwrite(info, sizeof(BITMAPINFOHEADER), 1, fpGray);
	fwrite(quad, sizeof(RGBQUAD), 256, fpGray);

	for (i = 0; i<info->biHeight; i++)
	{
		for (j = 0; j<(info->biWidth + 3) / 4 * 4; j++)
		{
			fread(&RGBData[j], 1, 3, fpBMP);
			GrayData[j] = RGBData[j][0] * 0.114 + RGBData[j][1] * 0.587 + RGBData[j][2] * 0.299;
		}
		fwrite(GrayData, j, 1, fpGray);
	}
	delete header;
	delete info;
	delete[] quad;
	closeFile();
	return 1;
}
