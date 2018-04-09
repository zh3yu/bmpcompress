#pragma once
#pragma pack(1)  
typedef struct tagBITMAPFILEHEADER
{
	unsigned char bfType[2];//�ļ���ʽ  
	unsigned long bfSize;//�ļ���С  
	unsigned short bfReserved1;//����  
	unsigned short bfReserved2;
	unsigned long bfOffBits; //DIB�������ļ��е�ƫ����  
}BITMAPFILEHEADER;  //�ܹ�14�ֽ�
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
	unsigned char rgbBlue; //��ɫ����  
	unsigned char rgbGreen;//��ɫ����  
	unsigned char rgbRed;//��ɫ����  
	unsigned char rgbReserved;
}RGBQUAD;