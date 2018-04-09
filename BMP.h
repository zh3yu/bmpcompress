#pragma once
#pragma pack(1)  
typedef struct tagBITMAPFILEHEADER
{
	unsigned char bfType[2];//文件格式  
	unsigned long bfSize;//文件大小  
	unsigned short bfReserved1;//保留  
	unsigned short bfReserved2;
	unsigned long bfOffBits; //DIB数据在文件中的偏移量  
}BITMAPFILEHEADER;  //总共14字节
typedef struct tagBITMAPINFOHEADER
{
	unsigned long biSize;
	long biWidth;
	long biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned long biCompression;
	unsigned long biSizeImage;
	long biXPixPerMeter;
	long biYPixPerMeter;
	unsigned long biClrUsed;
	unsigned long biClrImporant;
}BITMAPINFOHEADER;
typedef struct tagRGBQUAD
{
	unsigned char rgbBlue; //蓝色亮度  
	unsigned char rgbGreen;//绿色亮度  
	unsigned char rgbRed;//红色亮度  
	unsigned char rgbReserved;
}RGBQUAD;