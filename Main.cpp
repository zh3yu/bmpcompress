#include "stdafx.h"
#include "compressor.h"
void test() {
	Compressor compressor;
	compressor.BmpToGray("in.bmp", "out.bmp");
	cout << "��ɫͼƬת��Ϊ�Ҷ�ͼ���" << endl;
	compressor.Encode("out.bmp", "encode.dat");
	cout << "ѹ���ļ����" << endl;
	compressor.Decode("encode.dat", "decode.bmp");
	cout << "��ѹ���ļ����" << endl;
	cout << "Test Over!" << endl;
}

	int main()
	{
		test();
		return 0;
	}
