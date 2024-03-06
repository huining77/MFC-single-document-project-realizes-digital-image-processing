#include "pch.h"
#include "Bmp.h"

//构造函数
CBmp::CBmp() {}

//析构函数
CBmp::~CBmp() {}

//读取BMP文件
bool CBmp::readBmp(char* bmpName,
	unsigned char* img_data,
	int* bmpWidth, int* bmpHeight,
	int* biBitCount, int* lineByte) {
	//二进制读方式打开指定的图像文件
	FILE* fp;
	fopen_s(&fp, bmpName, "rb");
	if (fp == NULL)
		return false;

	//读取位图文件头结构BITMAPFILEHEADER
	BITMAPFILEHEADER file_header;
	fread(&file_header, sizeof(BITMAPFILEHEADER), 1, fp);

	//定义位图信息头结构变量，读取位图信息头进内存，存放在变量head中
	BITMAPINFOHEADER info_header;
	fread(&info_header, sizeof(BITMAPINFOHEADER), 1, fp);

	//获取图像的宽、高、每像素所占比特数等信息
	*bmpWidth = info_header.biWidth;
	*bmpHeight = info_header.biHeight;
	*biBitCount = info_header.biBitCount;

	//计算图像每行像素所占的字节数（必须是4的倍数）
	*lineByte = (*bmpWidth * *biBitCount / 8 + 3) / 4 * 4;

	//读位图数据进内存
	fread(img_data, 1, (*lineByte) * (*bmpHeight), fp);

	fclose(fp);//关闭文件
	return true;//读取文件成功
}

//绘制BMP图像
void CBmp::drawBmp(CDC* pDC,
	unsigned char* img_data,
	int bmpWidth, int bmpHeight, int lineByte,
	int offset_left, int offset_top)
{
	unsigned char red, green, blue = 0;
	unsigned char* img_line = img_data;
	for (int i = 0; i < bmpHeight; i++) {
		for (int k = 0; k < bmpWidth; k++)
		{
			blue = img_line[3 * k + 0];
			green = img_line[3 * k + 1];
			red = img_line[3 * k + 2];

			pDC->SetPixel(k + offset_left,
				bmpHeight - 1 - i + offset_top,
				RGB(red, green, blue));
		}
		img_line += lineByte;
	}
}

//读取并绘制BMP图像
void CBmp::readAndDrawBMP(CDC* pDC,
	char* bmpName,
	int offset_left, int offset_top)
{
	//申请保存图像数据的内存区，最大图像可以有1920*1080像素
	unsigned char* img_data = new unsigned char[1920 * 1080 * 3];

	//图像相关信息变量
	int bmpWidth, bmpHeight, biBitCount, lineByte;

	//读取图像数据到内存
	bool result = readBmp(bmpName, img_data,
		&bmpWidth, &bmpHeight,
		&biBitCount, &lineByte);

	if (!result) {
		delete[] img_data;
		return;
	}

	//绘制读取的BMP图像
	drawBmp(pDC, img_data,
		bmpWidth, bmpHeight, lineByte,
		offset_left, offset_top);

	delete[] img_data;
}

//将真彩色BMP分离为独立的R,G,B三个通道
bool CBmp::separateRGB(unsigned char* img_data,
	unsigned char* R, unsigned char* G, unsigned char* B,
	int bmpWidth, int bmpHeight, int lineByte)
{
	for (int i = 0; i < bmpHeight; i++) {
		for (int k = 0; k < bmpWidth; k++)
		{
			*R++ = img_data[3 * k + 0];
			*G++ = img_data[3 * k + 1];
			*B++ = img_data[3 * k + 2];
		}
		img_data += lineByte;
	}
	return true;
}

void  CBmp::print_matrix(CDC* pDC,
	unsigned char* img_R,
	unsigned char* img_G,
	unsigned char* img_B,
	int width, int height,
	int offset_left, int offset_top)
{
	unsigned char r, g, b = 0;
	for (int i = height - 1; i >= 0; i--)
		for (int k = 0; k < width; k++) {
			r = (unsigned char)img_R[i * width + k];
			g = (unsigned char)img_G[i * width + k];
			b = (unsigned char)img_B[i * width + k];

			pDC->SetPixel(k + offset_left,
				height - 1 - i + offset_top,
				RGB(b, g, r));
		}
}
void CBmp::readAndDrawBMP_seperate(CDC* pDC,
	char* bmpName,
	int offset_left, int offset_top)
{
	//申请保存图像数据的内存区，最大图像可以有1920*1080像素
	unsigned char* img_data = new unsigned char[1920 * 1080 * 3];

	//图像相关信息变量
	int bmpWidth, bmpHeight, biBitCount, lineByte;

	//读取图像数据到内存
	bool result = readBmp(bmpName, img_data,
		&bmpWidth, &bmpHeight,
		&biBitCount, &lineByte);

	if (!result) {
		delete[] img_data;
		return;
	}

	unsigned char* R = new unsigned char[1920 * 1080];
	unsigned char* G = new unsigned char[1920 * 1080];
	unsigned char* B = new unsigned char[1920 * 1080];

	separateRGB(img_data, R, G, B, bmpWidth, bmpHeight, lineByte);

	//绘制读取的BMP图像
	print_matrix(pDC, R, G, B, bmpWidth, bmpHeight, offset_left, offset_top);

	delete[] img_data;
	delete[] R;
	delete[] G;
	delete[] B;
}

bool CBmp::convertToGray(unsigned char* img_data,
	unsigned char* gray_data,
	int bmpWidth, int bmpHeight, int lineByte)
{
	for (int i = 0; i < bmpHeight; i++) {
		for (int k = 0; k < bmpWidth; k++)
		{
			unsigned char blue = img_data[3 * k + 0];
			unsigned char green = img_data[3 * k + 1];
			unsigned char red = img_data[3 * k + 2];

			// 计算灰度值
			unsigned char gray = static_cast<unsigned char>(0.299 * red + 0.587 * green + 0.114 * blue);

			// 存储灰度值
			*gray_data++ = gray;
		}
		img_data += lineByte;
	}
	return true;
}


void CBmp::drawGrayBmp(CDC* pDC,
	unsigned char* gray_data,
	int bmpWidth, int bmpHeight,
	int offset_left, int offset_top)
{
	unsigned char gray = 0;
	for (int i = 0; i < bmpHeight; i++) {
		for (int k = 0; k < bmpWidth; k++)
		{
			gray = gray_data[i * bmpWidth + k];

			// 绘制灰度图
			pDC->SetPixel(k + offset_left,
				bmpHeight - 1 - i + offset_top,
				RGB(gray, gray, gray));
		}
	}
}
