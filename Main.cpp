#include "stdafx.h"
#include "compressor.h"
void test() {
	Compressor compressor;
	compressor.BmpToGray("in.bmp", "out.bmp");
	cout << "彩色图片转化为灰度图完成" << endl;
	compressor.Encode("out.bmp", "encode.dat");
	cout << "压缩文件完成" << endl;
	compressor.Decode("encode.dat", "decode.bmp");
	cout << "解压缩文件完成" << endl;
	cout << "Test Over!" << endl;
}

	int main()
	{
		test();
		return 0;
	}
