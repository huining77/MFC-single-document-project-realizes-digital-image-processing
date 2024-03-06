#include "pch.h"
#include "Bmp.h"

//���캯��
CBmp::CBmp() {}

//��������
CBmp::~CBmp() {}

//��ȡBMP�ļ�
bool CBmp::readBmp(char* bmpName,
	unsigned char* img_data,
	int* bmpWidth, int* bmpHeight,
	int* biBitCount, int* lineByte) {
	//�����ƶ���ʽ��ָ����ͼ���ļ�
	FILE* fp;
	fopen_s(&fp, bmpName, "rb");
	if (fp == NULL)
		return false;

	//��ȡλͼ�ļ�ͷ�ṹBITMAPFILEHEADER
	BITMAPFILEHEADER file_header;
	fread(&file_header, sizeof(BITMAPFILEHEADER), 1, fp);

	//����λͼ��Ϣͷ�ṹ��������ȡλͼ��Ϣͷ���ڴ棬����ڱ���head��
	BITMAPINFOHEADER info_header;
	fread(&info_header, sizeof(BITMAPINFOHEADER), 1, fp);

	//��ȡͼ��Ŀ��ߡ�ÿ������ռ����������Ϣ
	*bmpWidth = info_header.biWidth;
	*bmpHeight = info_header.biHeight;
	*biBitCount = info_header.biBitCount;

	//����ͼ��ÿ��������ռ���ֽ�����������4�ı�����
	*lineByte = (*bmpWidth * *biBitCount / 8 + 3) / 4 * 4;

	//��λͼ���ݽ��ڴ�
	fread(img_data, 1, (*lineByte) * (*bmpHeight), fp);

	fclose(fp);//�ر��ļ�
	return true;//��ȡ�ļ��ɹ�
}

//����BMPͼ��
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

//��ȡ������BMPͼ��
void CBmp::readAndDrawBMP(CDC* pDC,
	char* bmpName,
	int offset_left, int offset_top)
{
	//���뱣��ͼ�����ݵ��ڴ��������ͼ�������1920*1080����
	unsigned char* img_data = new unsigned char[1920 * 1080 * 3];

	//ͼ�������Ϣ����
	int bmpWidth, bmpHeight, biBitCount, lineByte;

	//��ȡͼ�����ݵ��ڴ�
	bool result = readBmp(bmpName, img_data,
		&bmpWidth, &bmpHeight,
		&biBitCount, &lineByte);

	if (!result) {
		delete[] img_data;
		return;
	}

	//���ƶ�ȡ��BMPͼ��
	drawBmp(pDC, img_data,
		bmpWidth, bmpHeight, lineByte,
		offset_left, offset_top);

	delete[] img_data;
}

//�����ɫBMP����Ϊ������R,G,B����ͨ��
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
	//���뱣��ͼ�����ݵ��ڴ��������ͼ�������1920*1080����
	unsigned char* img_data = new unsigned char[1920 * 1080 * 3];

	//ͼ�������Ϣ����
	int bmpWidth, bmpHeight, biBitCount, lineByte;

	//��ȡͼ�����ݵ��ڴ�
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

	//���ƶ�ȡ��BMPͼ��
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

			// ����Ҷ�ֵ
			unsigned char gray = static_cast<unsigned char>(0.299 * red + 0.587 * green + 0.114 * blue);

			// �洢�Ҷ�ֵ
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

			// ���ƻҶ�ͼ
			pDC->SetPixel(k + offset_left,
				bmpHeight - 1 - i + offset_top,
				RGB(gray, gray, gray));
		}
	}
}
