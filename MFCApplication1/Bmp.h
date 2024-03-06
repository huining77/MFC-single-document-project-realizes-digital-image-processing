#pragma once
class CBmp
{
public:
	CBmp();
	~CBmp();

	bool readBmp(char* bmpName,
		unsigned char* img_data,
		int* bmpWidth, int* bmpHeight,
		int* biBitCount, int* lineByte);

	void drawBmp(CDC* pDC,
		unsigned char* img_data,
		int bmpWidth, int bmpHeight, int lineByte,
		int offset_left, int offset_top);

	void readAndDrawBMP(CDC* pDC,
		char* bmpName,
		int offset_left, int offset_top);

	bool separateRGB(unsigned char* img_data,
		unsigned char* R, unsigned char* G, unsigned char* B,
		int bmpWidth, int bmpHeight, int lineByte);

	void  print_matrix(CDC* pDC,
		unsigned char* img_R,
		unsigned char* img_G,
		unsigned char* img_B,
		int width, int height,
		int offset_left, int offset_top);

	void readAndDrawBMP_seperate(CDC* pDC,
		char* bmpName,
		int offset_left, int offset_top);

	bool CBmp::convertToGray(unsigned char* img_data,
		unsigned char* gray_data,
		int bmpWidth, int bmpHeight, int lineByte);
	void CBmp::drawGrayBmp(CDC* pDC,
		unsigned char* gray_data,
		int bmpWidth, int bmpHeight,
		int offset_left, int offset_top);
};
