
// MFCApplication1View.cpp: CMFCApplication1View 类的实现
//

#include "pch.h"
#include "framework.h"


#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core.hpp>


#include <Gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCApplication1.h"
#endif

#include "MFCApplication1Doc.h"
#include "MFCApplication1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication1View

IMPLEMENT_DYNCREATE(CMFCApplication1View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication1View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCApplication1View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_32771, &CMFCApplication1View::OnDraw_line)
	ON_COMMAND(ID_NewBmp, &CMFCApplication1View::OnNewbmp)
	ON_COMMAND(ID_Contrast_stretching, &CMFCApplication1View::OnContraststretching)
	ON_COMMAND(ID_TranslatToGray, &CMFCApplication1View::OnTranslattogray)
	ON_COMMAND(ID_NegativeTransform, &CMFCApplication1View::OnNegativetransform)
	ON_COMMAND(ID_LogTransform, &CMFCApplication1View::OnLogtransform)
	ON_COMMAND(ID_PowerLawTransform, &CMFCApplication1View::OnPowerlawtransform)
	ON_COMMAND(ID_GrayLevelSlicing, &CMFCApplication1View::OnGraylevelslicing)
	ON_COMMAND(ID_Bitplaneslicing, &CMFCApplication1View::OnBitplaneslicing)
	ON_COMMAND(ID_histogramE, &CMFCApplication1View::Onhistograme)
	ON_COMMAND(ID_AddSaltPepperNoise, &CMFCApplication1View::OnAddsaltpeppernoise)
	ON_COMMAND(ID_MeanFilter, &CMFCApplication1View::OnMeanfilter)
	ON_COMMAND(ID_MedianFilter, &CMFCApplication1View::OnMedianfilter)
	ON_COMMAND(ID_Laplacian, &CMFCApplication1View::OnLaplacian)
	ON_COMMAND(ID_HistogramEqualizationAndLaplacian, &CMFCApplication1View::OnHistogramequalizationandlaplacian)
	ON_COMMAND(ID_MedianLaplacian, &CMFCApplication1View::OnMedianlaplacian)
	ON_COMMAND(ID_FFT, &CMFCApplication1View::OnFft)

END_MESSAGE_MAP()

// CMFCApplication1View 构造/析构

CMFCApplication1View::CMFCApplication1View() noexcept
{
	// TODO: 在此处添加构造代码
	m_pBmp = new CBmp();
	// 在构造函数中初始化为 nullptr
	gray_data = nullptr;  // 在构造函数中初始化为 nullptr
	noisy_data = nullptr;

}

CMFCApplication1View::~CMFCApplication1View()
{
	delete m_pBmp;
	delete[] gray_data;
	delete[] noisy_data;
}

BOOL CMFCApplication1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFCApplication1View 绘图--main

int bmpWidth=0, bmpHeight=0, biBitCount=0, lineByte=0;
char bmpName[] = "MFC背景小1.bmp";
unsigned char* img_data = new unsigned char[1920 *1080 * 3];//图像大小

// 申请保存灰度图像数据的内存区 
int offset_left = 20;
int offset_top = 60;

void CMFCApplication1View::OnDraw(CDC* pDC)
{
	
	CMFCApplication1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 在此处为本机数据添加绘制代码
	/* // 创建背景代码
	CBitmap bitmap;			//位图类对象
	bitmap.LoadBitmap(IDB_BITMAP1); //从资源中装载入位图

	CDC dcComp;			//创建CDC对象
	dcComp.CreateCompatibleDC(pDC);		//创建设备环境
	dcComp.SelectObject(&bitmap);			//选择位图对象
	BITMAP bmInfo;
	bitmap.GetObject(sizeof(bmInfo), &bmInfo);	//获取位图信息

	// 获取视图的客户区域大小
	CRect clientRect;
	GetClientRect(&clientRect);
	// 将位图填充显示到视图中，使用 StretchBlt 函数
	pDC->StretchBlt(0, 0, clientRect.Width(), clientRect.Height(), &dcComp, 0, 0, bmInfo.bmWidth, bmInfo.bmHeight, SRCCOPY);
	//将位图显示到视图中--大小不变
	//pDC->BitBlt(0, 0, bmInfo.bmWidth, bmInfo.bmHeight, &dcComp, 0, 0, SRCCOPY);
	*/
	if (flag == 1)
	{
	// 读取并绘制图像
	m_pBmp->readAndDrawBMP(pDC, bmpName, offset_left, offset_top);
	
	// 在图片下方添加文字
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	{
		Graphics graphics(pDC->m_hDC);
		Gdiplus::Font font(L"Arial", 12);
		SolidBrush brush(Color(255, 128, 0, 128));  // 文字颜色为紫色

		// 文字的位置
		PointF point(offset_left , offset_top + 150);

		// 绘制文字
		graphics.DrawString(L"原图", -1, &font, point, &brush);
	}
	GdiplusShutdown(gdiplusToken);
		
	}
	
	 
}

// CMFCApplication1View 打印


void CMFCApplication1View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCApplication1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFCApplication1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMFCApplication1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CMFCApplication1View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCApplication1View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCApplication1View 诊断

#ifdef _DEBUG
void CMFCApplication1View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication1Doc* CMFCApplication1View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication1Doc)));
	return (CMFCApplication1Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplication1View 消息处理程序

//绘图操作正弦
void CMFCApplication1View::OnDraw_line()
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	{
		Graphics graphics(GetSafeHwnd());

		// 获取视图的客户区域大小
		CRect clientRect;
		GetClientRect(&clientRect);

		// 设置坐标系原点
		PointF origin(50.0f, clientRect.Height() / 2.0f);

		// 绘制直角坐标系
		Pen axisPen(Color::Black, 2.0f);
		graphics.DrawLine(&axisPen, origin.X, origin.Y, clientRect.Width() - 50.0f, origin.Y); // X 轴
		graphics.DrawLine(&axisPen, origin.X, origin.Y, origin.X, 50.0f); // Y 轴

		// 绘制正弦波形
		Pen sineWavePen(Color::Red, 2.0f);
		const int waveLength = 1000;
		const int amplitude = 100;
		const float frequency = 0.01f;

		for (int x = 0; x <= waveLength; ++x) {
			float y = amplitude * sin(frequency * x);
			graphics.DrawLine(&sineWavePen, origin.X + x, origin.Y - y, origin.X + x + 1, origin.Y - amplitude * sin(frequency * (x + 1)));
		}
	}
	
	GdiplusShutdown(gdiplusToken);
	Invalidate();
}

//打开图片--函数在主函数中
void CMFCApplication1View::OnNewbmp()
{
	// TODO: 在此添加命令处理程序代码

	flag = 1;
	
	Invalidate();
}

//对比度拉伸代码
void CMFCApplication1View::OnContraststretching()
{
	
	CClientDC dc(this);
	CDC* pDC = &dc;
	if (gray_data != nullptr) {
		// 寻找图像的最小和最大像素值
		unsigned int minPixelValue = gray_data[0];
		unsigned int maxPixelValue = gray_data[0];
		// 创建临时数组用于对比度拉伸处理
		unsigned char* temp_data = new unsigned char[bmpWidth * bmpHeight];
		for (int i = 0; i < bmpWidth * bmpHeight; ++i) {
			if (gray_data[i] < minPixelValue) {
				minPixelValue = gray_data[i];
			}

			if (gray_data[i] > maxPixelValue) {
				maxPixelValue = gray_data[i];
			}
		}

		// 对比度拉伸的参数
		const int newMinPixelValue = 0;
		const int newMaxPixelValue = 255;

		// 应用对比度拉伸公式到每个像素
		for (int i = 0; i < bmpWidth * bmpHeight; ++i) {
			temp_data[i] = static_cast<unsigned char>(
				(gray_data[i] - minPixelValue) * (newMaxPixelValue - newMinPixelValue) /
				(maxPixelValue - minPixelValue) + newMinPixelValue);
		}

		// 绘制拉伸后的灰度图
		m_pBmp->drawGrayBmp(pDC, temp_data, bmpWidth, bmpHeight, offset_left + 2*bmpWidth, offset_top);
		// 释放临时数组内存
		delete[] temp_data;
		// 更新视图，显示修改后的图像
		//Invalidate();
		// 在图片下方添加文字
		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
		{
		Graphics graphics(pDC->m_hDC);
		Gdiplus::Font font(L"Arial", 12);
		SolidBrush brush(Color(255,128, 0, 128));  // 文字颜色为紫色

		// 文字的位置
		PointF point(offset_left + 2 * bmpWidth, offset_top + bmpHeight + 10);

		// 绘制文字
		graphics.DrawString(L"对比度拉伸后的图", -1, &font, point, &brush);
		}
		GdiplusShutdown(gdiplusToken);

	}
	else {
		// 处理图像未加载的情况
		AfxMessageBox(_T("未加载图片"));
		return;
	}
}


//灰度转换
void CMFCApplication1View::OnTranslattogray()
{
	CClientDC dc(this);
	CDC* pDC = &dc;

	// 读取图像数据到内存
	bool result = m_pBmp->readBmp(bmpName, img_data,
		&bmpWidth, &bmpHeight,
		&biBitCount, &lineByte);

	if (!result) {
		AfxMessageBox(_T("未加载图片"));
		return;
	}

	// 确保在每次转换前释放上一次的灰度图内存
	delete[] gray_data;

	// 申请保存灰度图像数据的内存区
	gray_data = new unsigned char[bmpWidth * bmpHeight];

	// 将彩色图转换为灰度图
	m_pBmp->convertToGray(img_data, gray_data, bmpWidth, bmpHeight, lineByte);

	// 绘制灰度图
	m_pBmp->drawGrayBmp(pDC, gray_data, bmpWidth, bmpHeight, offset_left + bmpWidth, offset_top);
	// 在图片下方添加文字
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
	{
		Graphics graphics(pDC->m_hDC);
		Gdiplus::Font font(L"Arial", 12);
		SolidBrush brush(Color(255, 128, 0, 128));  // 文字颜色为紫色

		// 文字的位置
		PointF point(offset_left + 250, offset_top + bmpHeight + 10);

		// 绘制文字
		graphics.DrawString(L"灰度图", -1, &font, point, &brush);
	}
	GdiplusShutdown(gdiplusToken);
	
}
//反变换
void CMFCApplication1View::OnNegativetransform()
{
	// TODO: 在此添加命令处理程序代码
	if (gray_data != nullptr) 
	{

		// 创建临时数组用于对比度拉伸处理
		unsigned char* neg_transformed_data = new unsigned char[bmpWidth * bmpHeight];
		// 负变换的处理代码
		for (int i = 0; i < bmpWidth * bmpHeight; ++i) {
			neg_transformed_data[i] = 255 - gray_data[i];
		}
		// 获取绘图设备
		CClientDC dc(this);
		CDC* pDC = &dc;

		// 绘制负变换后的灰度图
		m_pBmp->drawGrayBmp(pDC, neg_transformed_data, bmpWidth, bmpHeight, offset_left + 3* bmpWidth, offset_top);

		// 在图片下方添加文字
		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
		{
			Graphics graphics(pDC->m_hDC);
			Gdiplus::Font font(L"Arial", 12);
			SolidBrush brush(Color(255, 128, 0, 128));  // 文字颜色为紫色

			// 文字的位置
			PointF point(offset_left + 3 * bmpWidth, offset_top + bmpHeight + 10);

			// 绘制文字
			graphics.DrawString(L"负变换后的灰度图", -1, &font, point, &brush);
		}
		GdiplusShutdown(gdiplusToken);

		// 释放负变换后的数据的内存
		delete[] neg_transformed_data;
	}
	else {
		// 处理图像未加载的情况
		AfxMessageBox(_T("未加载图片"));

	}
}
#include <cmath>  // 用于对数变换中的对数函数

//log对数
void CMFCApplication1View::OnLogtransform()
{
	// TODO: 在此添加命令处理程序代码
	if (gray_data != nullptr)
	{
		// 创建临时数组用于对数变换处理
		unsigned char* log_transformed_data = new unsigned char[bmpWidth * bmpHeight];

		// 对数变换的处理代码
		const double c = 40.0;  // 可根据需要调整的常数
		for (int i = 0; i < bmpWidth * bmpHeight; ++i) {
			log_transformed_data[i] = static_cast<unsigned char>(c * log(1 + gray_data[i]));
		}

		// 获取绘图设备
		CClientDC dc(this);
		CDC* pDC = &dc;

		// 绘制对数变换后的灰度图
		m_pBmp->drawGrayBmp(pDC, log_transformed_data, bmpWidth, bmpHeight, offset_left , offset_top+ bmpHeight+50);

		// 在图片下方添加文字
		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
		{
			Graphics graphics(pDC->m_hDC);
			Gdiplus::Font font(L"Arial", 12);
			SolidBrush brush(Color(255, 128, 0, 128));  // 文字颜色为紫色

			// 文字的位置
			PointF point(offset_left , offset_top + 2*bmpHeight+55);

			// 绘制文字
			graphics.DrawString(L"对数变换后的灰度图", -1, &font, point, &brush);
		}
		GdiplusShutdown(gdiplusToken);

		// 释放对数变换后的数据的内存
		delete[] log_transformed_data;
	}
	else {
		// 处理图像未加载的情况
		AfxMessageBox(_T("未加载灰度图片或该图片不是灰度图片"));
	}
}

//幂律（n次幂和n次根变换）
void CMFCApplication1View::OnPowerlawtransform()
{
	// TODO: 在此添加命令处理程序代码
	if (gray_data != nullptr) {
		// 创建临时数组用于幂律变换处理
		unsigned char* power_law_transformed_data = new unsigned char[bmpWidth * bmpHeight];
		double gamma = 1.5; // 幂律指数，可以根据需要调整

		// 幂律变换的处理代码
		for (int i = 0; i < bmpWidth * bmpHeight; ++i) {
			double normalized_pixel_value = static_cast<double>(gray_data[i]) / 255.0;
			double transformed_value = 255.0 * pow(normalized_pixel_value, gamma);
			power_law_transformed_data[i] = static_cast<unsigned char>(transformed_value);
		}

		// 获取绘图设备
		CClientDC dc(this);
		CDC* pDC = &dc;

		// 绘制幂律变换后的灰度图
		m_pBmp->drawGrayBmp(pDC, power_law_transformed_data, bmpWidth, bmpHeight, offset_left+ bmpWidth, offset_top + bmpHeight + 50);

		// 在图片下方添加文字
		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
		{
			Graphics graphics(pDC->m_hDC);
			Gdiplus::Font font(L"Arial", 12);
			SolidBrush brush(Color(255, 128, 0, 128));  // 文字颜色为紫色

			// 文字的位置
			PointF point(offset_left + 250, offset_top + 2 * bmpHeight + 55);

			// 绘制文字
			graphics.DrawString(L"幂律变换后的灰度图", -1, &font, point, &brush);
		}
		GdiplusShutdown(gdiplusToken);

		// 释放幂律变换后的数据的内存
		delete[] power_law_transformed_data;
	}
	else {
		// 处理图像未加载的情况
		AfxMessageBox(_T("未加载灰度图片"));
	}
}

//灰度级别切片
void CMFCApplication1View::OnGraylevelslicing()
{
	// TODO: 在此添加命令处理程序代码
	if (gray_data != nullptr) {
		unsigned char* enhanced_data = new unsigned char[bmpWidth * bmpHeight];
		int lower_threshold = 100;  // 定义下限阈值
		int upper_threshold = 200;  // 定义上限阈值

		for (int i = 0; i < bmpWidth * bmpHeight; ++i) {
			if (gray_data[i] >= lower_threshold && gray_data[i] <= upper_threshold) {
				// 在选择的灰度范围内增强像素值
				enhanced_data[i] = 255;
			}
			else {
				// 在其他范围内抑制像素值
				enhanced_data[i] = 0;
			}
		}

		// 获取绘图设备
		CClientDC dc(this);
		CDC* pDC = &dc;

		// 绘制增强后的灰度图
		m_pBmp->drawGrayBmp(pDC, enhanced_data, bmpWidth, bmpHeight, offset_left + 2*bmpWidth, offset_top + bmpHeight + 50);

		// 在图片下方添加文字
		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
		{
			Graphics graphics(pDC->m_hDC);
			Gdiplus::Font font(L"Arial", 12);
			SolidBrush brush(Color(255, 128, 0, 128));  // 文字颜色为紫色

			// 文字的位置
			PointF point(offset_left + 2 * bmpWidth, offset_top + 2 * bmpHeight + 55);

			// 绘制文字
			graphics.DrawString(L"灰度级别切片", -1, &font, point, &brush);
		}
		GdiplusShutdown(gdiplusToken);

		// 释放内存
		delete[] enhanced_data;
	}
	else {
		// 处理图像未加载的情况
		AfxMessageBox(_T("未加载灰度图片"));
	}
}

//Bit-plane slicing 
void CMFCApplication1View::OnBitplaneslicing()
{
	// TODO: 在此添加命令处理程序代码
	if (gray_data != nullptr) {
		// 获取绘图设备
		CClientDC dc(this);
		CDC* pDC = &dc;

		// 选择要显示的位平面，（从右往左数）
		int bit_plane = 3;

		// 创建临时数组用于存储位平面数据
		unsigned char* bit_plane_data = new unsigned char[bmpWidth * bmpHeight];

		// 提取位平面数据
		for (int i = 0; i < bmpWidth * bmpHeight; ++i) {
			bit_plane_data[i] = (gray_data[i] >> bit_plane) & 1;
		}

		// 缩放位平面数据以便显示
		int scale_factor = 255;
		for (int i = 0; i < bmpWidth * bmpHeight; ++i) {
			bit_plane_data[i] *= scale_factor;
		}

		// 绘制位平面图像
		m_pBmp->drawGrayBmp(pDC, bit_plane_data, bmpWidth, bmpHeight,offset_left + 3*bmpWidth, offset_top + bmpHeight + 50);

		// 在图片下方添加文字
		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
		{
			Graphics graphics(pDC->m_hDC);
			Gdiplus::Font font(L"Arial", 12);
			SolidBrush brush(Color(255, 128, 0, 128));  // 文字颜色为紫色

			// 文字的位置
			PointF point(offset_left + 720, offset_top + 2 * bmpHeight + 55);

			// 绘制文字
			CString text;
			text.Format(L"Bit-plane Slicing (Bit %d)", bit_plane);
			graphics.DrawString(text, -1, &font, point, &brush);
		}
		GdiplusShutdown(gdiplusToken);

		// 释放内存
		delete[] bit_plane_data;
	}
	else {
		// 处理图像未加载的情况
		AfxMessageBox(_T("未加载灰度图片"));
	}
}
#include <algorithm>

//直方图均衡化
void CMFCApplication1View::Onhistograme()
{
	// TODO: 在此添加命令处理程序代码
	if (gray_data != nullptr) {
		// 获取绘图设备
		CClientDC dc(this);
		CDC* pDC = &dc;

		// 计算直方图
		int histogram[256] = { 0 };
		for (int i = 0; i < bmpWidth * bmpHeight; ++i) {
			histogram[gray_data[i]]++;
		}

		// 计算累积分布函数（CDF）
		int cdf[256] = { 0 };
		cdf[0] = histogram[0];
		for (int i = 1; i < 256; ++i) {
			cdf[i] = cdf[i - 1] + histogram[i];
		}

		// 映射灰度级别到临时变量
		unsigned char* temp_data = new unsigned char[bmpWidth * bmpHeight];
		int min_cdf = cdf[0];
		for (int i = 0; i < bmpWidth * bmpHeight; ++i) {
			temp_data[i] = static_cast<unsigned char>(255 * (cdf[gray_data[i]] - min_cdf) / (bmpWidth * bmpHeight - min_cdf));
		}

		// 绘制均衡化后的图像
		m_pBmp->drawGrayBmp(pDC, temp_data, bmpWidth, bmpHeight, offset_left , offset_top+3*bmpHeight);

		// 在图片下方添加文字
		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
		{
			Graphics graphics(pDC->m_hDC);
			Gdiplus::Font font(L"Arial", 12);
			SolidBrush brush(Color(255, 128, 0, 128));  // 文字颜色为紫色

			// 文字的位置
			PointF point(offset_left, offset_top + 4*bmpHeight);

			// 绘制文字
			graphics.DrawString(L"直方图均衡化", -1, &font, point, &brush);
		}

		// 在图像右边绘制直方图
		int hist_width = 200;
		int hist_height = bmpHeight;
		int hist_offset = 50;  // 距离图像的偏移量
		int max_hist_value = *std::max_element(std::begin(histogram), std::end(histogram));

		for (int i = 0; i < 256; ++i) {
			int bar_height = static_cast<int>((static_cast<double>(histogram[i]) / max_hist_value) * hist_height);
			pDC->FillSolidRect(offset_left + bmpWidth + hist_offset + i,
				offset_top + 3 * bmpHeight+ (hist_height - bar_height),
				1, bar_height, RGB(0, 0, 0));  // 用黑色填充直方图条形
		}

		// 释放临时变量的内存
		delete[] temp_data;

		GdiplusShutdown(gdiplusToken);
	}
	else {
		// 处理图像未加载的情况
		AfxMessageBox(_T("未加载灰度图片"));
	}
}


//添加椒盐噪声
void CMFCApplication1View::OnAddsaltpeppernoise()
{
	// TODO: 在此添加命令处理程序代码
	if (gray_data != nullptr) {
		// 获取绘图设备
		CClientDC dc(this);
		CDC* pDC = &dc;

		// 创建临时数组用于保存添加噪声后的数据
		noisy_data = new unsigned char[bmpWidth * bmpHeight];

		// 复制原始数据到临时数组
		std::copy(gray_data, gray_data + bmpWidth * bmpHeight, noisy_data);

		// 添加椒盐噪声
		srand(static_cast<unsigned int>(time(nullptr)));  // 初始化随机数种子
		const double saltPepperRatio = 0.01;  // 椒盐噪声比例

		for (int i = 0; i < bmpWidth * bmpHeight; ++i) {
			double randomValue = static_cast<double>(rand()) / RAND_MAX;
			if (randomValue < saltPepperRatio / 2) {
				noisy_data[i] = 0;  // 添加椒噪声
			}
			else if (randomValue < saltPepperRatio) {
				noisy_data[i] = 255;  // 添加盐噪声
			}
		}

		// 绘制带有椒盐噪声的图像
		m_pBmp->drawGrayBmp(pDC, noisy_data, bmpWidth, bmpHeight, offset_left+2*bmpWidth, offset_top + 3 * bmpHeight);
		//画第二个
		//m_pBmp->drawGrayBmp(pDC, noisy_data, bmpWidth, bmpHeight, offset_left+2*bmpWidth, offset_top + 3 * bmpHeight);

		// 在图片下方添加文字
		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
		{
			Graphics graphics(pDC->m_hDC);
			Gdiplus::Font font(L"Arial", 12);
			SolidBrush brush(Color(255, 128, 0, 128));  // 文字颜色为紫色

			// 文字的位置
			PointF point(offset_left+2*bmpWidth, offset_top + 4 * bmpHeight);

			// 绘制文字
			graphics.DrawString(L"椒盐噪声图", -1, &font, point, &brush);
		}

		// 释放临时变量的内存
		//delete[] noisy_data;

		GdiplusShutdown(gdiplusToken);
	}
	else {
		// 处理图像未加载的情况
		AfxMessageBox(_T("未加载灰度图片"));
	}
}

//均值滤波
void CMFCApplication1View::OnMeanfilter()
{
	// TODO: 在此添加命令处理程序代码
	if (noisy_data != nullptr) {
		// 获取绘图设备
		CClientDC dc(this);
		CDC* pDC = &dc;

		// 创建临时数组用于保存均值滤波后的数据
		unsigned char* filtered_data = new unsigned char[bmpWidth * bmpHeight];

		// 设置均值滤波的卷积核大小（3x3）
		const int kernelSize = 3;
		const int kernelHalfSize = kernelSize / 2;

		// 应用均值滤波
		for (int y = kernelHalfSize; y < bmpHeight - kernelHalfSize; ++y) {
			for (int x = kernelHalfSize; x < bmpWidth - kernelHalfSize; ++x) {
				int sum = 0;

				// 计算卷积核内的像素值之和
				for (int ky = -kernelHalfSize; ky <= kernelHalfSize; ++ky) {
					for (int kx = -kernelHalfSize; kx <= kernelHalfSize; ++kx) {
						int pixelValue = noisy_data[(y + ky) * bmpWidth + (x + kx)];
						sum += pixelValue;
					}
				}

				// 计算均值并赋值给滤波后的像素
				filtered_data[y * bmpWidth + x] = static_cast<unsigned char>(sum / (kernelSize * kernelSize));
			}
		}

		// 绘制均值滤波后的图像
		m_pBmp->drawGrayBmp(pDC, filtered_data, bmpWidth, bmpHeight, offset_left + 3 * bmpWidth, offset_top + 3 * bmpHeight);

		// 在图片下方添加文字
		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
		{
			Graphics graphics(pDC->m_hDC);
			Gdiplus::Font font(L"Arial", 12);
			SolidBrush brush(Color(255, 128, 0, 128));  // 文字颜色为紫色

			// 文字的位置
			PointF point(offset_left +3* bmpWidth, offset_top + 4 * bmpHeight);

			// 绘制文字
			graphics.DrawString(L"均值滤波后的图像", -1, &font, point, &brush);
		}

		// 释放临时变量的内存
		delete[] filtered_data;

		GdiplusShutdown(gdiplusToken);
	}
	else {
		// 处理图像未加载的情况
		AfxMessageBox(_T("未加载噪声图片"));
	}
}

#include <vector>

//中值滤波
void CMFCApplication1View::OnMedianfilter()
{
	// TODO: 在此添加命令处理程序代码
	if (noisy_data != nullptr) {
		// 获取绘图设备
		CClientDC dc(this);
		CDC* pDC = &dc;

		// 创建临时数组用于中值滤波处理
		unsigned char* median_filtered_data = new unsigned char[bmpWidth * bmpHeight];

		// 中值滤波的处理代码
		int filter_size = 3;  // 中值滤波的邻域大小，可以根据实际情况调整
		int filter_radius = filter_size / 2;

		for (int y = filter_radius; y < bmpHeight - filter_radius; ++y) {
			for (int x = filter_radius; x < bmpWidth - filter_radius; ++x) {
				// 获取邻域内的像素值
				std::vector<unsigned char> neighborhood;
				for (int j = -filter_radius; j <= filter_radius; ++j) {
					for (int i = -filter_radius; i <= filter_radius; ++i) {
						neighborhood.push_back(noisy_data[(y + j) * bmpWidth + (x + i)]);
					}
				}

				// 对邻域内的像素值进行排序
				std::sort(neighborhood.begin(), neighborhood.end());

				// 计算中值并赋值给当前像素
				median_filtered_data[y * bmpWidth + x] = neighborhood[filter_size * filter_size / 2];
			}
		}

		// 绘制中值滤波后的图像
		m_pBmp->drawGrayBmp(pDC, median_filtered_data, bmpWidth, bmpHeight, offset_left + 4 * bmpWidth, offset_top + 3 * bmpHeight);

		// 在图片下方添加文字
		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
		{
			Graphics graphics(pDC->m_hDC);
			Gdiplus::Font font(L"Arial", 12);
			SolidBrush brush(Color(255, 128, 0, 128));  // 文字颜色为紫色

			// 文字的位置
			PointF point(offset_left + 4 * bmpWidth, offset_top + 4 * bmpHeight);

			// 绘制文字
			graphics.DrawString(L"中值滤波", -1, &font, point, &brush);
		}

		// 释放临时数组的内存
		delete[] median_filtered_data;

		GdiplusShutdown(gdiplusToken);
	}
	else {
		// 处理图像未加载的情况
		AfxMessageBox(_T("未加载噪声图片"));
	}
}

//Laplacian--拉普拉斯变换
void CMFCApplication1View::OnLaplacian()
{
	// TODO: 在此添加命令处理程序代码
	if (gray_data != nullptr) {
		// 创建一个临时数组用于保存处理后的图像数据
		unsigned char* laplacian_data = new unsigned char[bmpWidth * bmpHeight];

		// 定义拉普拉斯核
		int laplacianKernel[3][3] = {
			{ -1, -1, -1 },
			{ -1,  8, -1 },
			{ -1, -1, -1 }
		};

		// 应用卷积运算
		for (int y = 1; y < bmpHeight - 1; ++y) {
			for (int x = 1; x < bmpWidth - 1; ++x) {
				int sum = 0;
				for (int i = -1; i <= 1; ++i) {
					for (int j = -1; j <= 1; ++j) {
						sum += laplacianKernel[i + 1][j + 1] * gray_data[(y + i) * bmpWidth + (x + j)];
					}
				}
				laplacian_data[y * bmpWidth + x] = static_cast<unsigned char>(sum);
			}
		}
		CClientDC dc(this);
		CDC* pDC = &dc;
		
		m_pBmp->drawGrayBmp(pDC, laplacian_data, bmpWidth, bmpHeight, offset_left, offset_top +4 * bmpHeight+30);

		// 释放临时数组内存
		delete[] laplacian_data;

		// 在图片下方添加文字
		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
		{
			Graphics graphics(pDC->m_hDC);
			Gdiplus::Font font(L"Arial", 12);
			SolidBrush brush(Color(255, 128, 0, 128));  // 文字颜色为紫色

			// 文字的位置
			PointF point(offset_left , offset_top + 5 * bmpHeight+30);

			// 绘制文字
			graphics.DrawString(L"拉普拉斯变换", -1, &font, point, &brush);
		}


		GdiplusShutdown(gdiplusToken);
	}
	else {
		// 处理图像未加载的情况
		AfxMessageBox(_T("未加载灰度图片"));
	}

}

//直方图+拉普拉斯
void CMFCApplication1View::OnHistogramequalizationandlaplacian()
{
	// TODO: 在此添加命令处理程序代码
	if (gray_data != nullptr) {
		// 获取绘图设备
		CClientDC dc(this);
		CDC* pDC = &dc;

		// 计算直方图
		int histogram[256] = { 0 };
		for (int i = 0; i < bmpWidth * bmpHeight; ++i) {
			histogram[gray_data[i]]++;
		}

		// 计算累积分布函数（CDF）
		int cdf[256] = { 0 };
		cdf[0] = histogram[0];
		for (int i = 1; i < 256; ++i) {
			cdf[i] = cdf[i - 1] + histogram[i];
		}

		// 映射灰度级别到临时变量
		unsigned char* temp_data = new unsigned char[bmpWidth * bmpHeight];
		int min_cdf = cdf[0];
		for (int i = 0; i < bmpWidth * bmpHeight; ++i) {
			temp_data[i] = static_cast<unsigned char>(255 * (cdf[gray_data[i]] - min_cdf) / (bmpWidth * bmpHeight - min_cdf));
		}

		// 应用拉普拉斯变换
		unsigned char* laplacian_data = new unsigned char[bmpWidth * bmpHeight];
		int laplacianKernel[3][3] = {
			{ -1, -1, -1 },
			{ -1,  8, -1 },
			{ -1, -1, -1 }
		};
		for (int y = 1; y < bmpHeight - 1; ++y) {
			for (int x = 1; x < bmpWidth - 1; ++x) {
				int sum = 0;
				for (int i = -1; i <= 1; ++i) {
					for (int j = -1; j <= 1; ++j) {
						sum += laplacianKernel[i + 1][j + 1] * temp_data[(y + i) * bmpWidth + (x + j)];
					}
				}
				laplacian_data[y * bmpWidth + x] = static_cast<unsigned char>(sum);
			}
		}

		// 绘制均衡化后的图像
		//m_pBmp->drawGrayBmp(pDC, temp_data, bmpWidth, bmpHeight, offset_left + 900, offset_top);

		// 绘制经拉普拉斯变换后的图像
		m_pBmp->drawGrayBmp(pDC, laplacian_data, bmpWidth, bmpHeight, offset_left+bmpWidth, offset_top + 4* bmpHeight + 30);

		// 在图片下方添加文字
		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
		{
			Graphics graphics(pDC->m_hDC);
			Gdiplus::Font font(L"Arial", 8);
			SolidBrush brush(Color(255, 128, 0, 128));  // 文字颜色为紫色

			// 文字的位置
			PointF point(offset_left + 900, offset_top + 2 * bmpHeight + 20);

			// 绘制文字
			//graphics.DrawString(L"直方图均衡化", -1, &font, point, &brush);

			// 文字的位置
			PointF point2(offset_left+bmpWidth, offset_top + 5 * bmpHeight + 30);

			// 绘制文字
			graphics.DrawString(L"直方图均衡化+拉普拉斯变换", -1, &font, point2, &brush);
		}

		// 释放临时变量的内存
		delete[] temp_data;
		delete[] laplacian_data;

		GdiplusShutdown(gdiplusToken);
	}
	else {
		// 处理图像未加载的情况
		AfxMessageBox(_T("未加载图片"));
	}
}

//中值滤波+拉普拉斯
void CMFCApplication1View::OnMedianlaplacian()
{
	// TODO: 在此添加命令处理程序代码
	if (gray_data != nullptr) {
		// 创建一个临时数组用于保存处理后的图像数据
		unsigned char* median_laplacian_data = new unsigned char[bmpWidth * bmpHeight];
		unsigned char* temp_data = new unsigned char[bmpWidth * bmpHeight];

		// 中值滤波
		for (int y = 1; y < bmpHeight - 1; ++y) {
			for (int x = 1; x < bmpWidth - 1; ++x) {
				// 获取3x3邻域内的像素值
				unsigned char neighborhood[9] = {
					gray_data[(y - 1) * bmpWidth + x - 1], gray_data[(y - 1) * bmpWidth + x], gray_data[(y - 1) * bmpWidth + x + 1],
					gray_data[y * bmpWidth + x - 1], gray_data[y * bmpWidth + x], gray_data[y * bmpWidth + x + 1],
					gray_data[(y + 1) * bmpWidth + x - 1], gray_data[(y + 1) * bmpWidth + x], gray_data[(y + 1) * bmpWidth + x + 1]
				};

				// 对邻域内像素值进行排序
				std::sort(neighborhood, neighborhood + 9);

				// 取中值作为当前像素值
				temp_data[y * bmpWidth + x] = neighborhood[4];
			}
		}

		// 拉普拉斯变换
		int laplacianKernel[3][3] = {
			{ -1, -1, -1 },
			{ -1,  8, -1 },
			{ -1, -1, -1 }
		};

		// 应用卷积运算
		for (int y = 1; y < bmpHeight - 1; ++y) {
			for (int x = 1; x < bmpWidth - 1; ++x) {
				int sum = 0;
				for (int i = -1; i <= 1; ++i) {
					for (int j = -1; j <= 1; ++j) {
						sum += laplacianKernel[i + 1][j + 1] * temp_data[(y + i) * bmpWidth + (x + j)];
					}
				}
				median_laplacian_data[y * bmpWidth + x] = static_cast<unsigned char>(sum);
			}
		}

		CClientDC dc(this);
		CDC* pDC = &dc;

		// 显示结果
		//m_pBmp->drawGrayBmp(pDC, temp_data, bmpWidth, bmpHeight, offset_left, offset_top);
		m_pBmp->drawGrayBmp(pDC, median_laplacian_data, bmpWidth, bmpHeight, offset_left + 2*bmpWidth, offset_top + 4 * bmpHeight + 30);

		// 在图片下方添加文字
		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
		{
			Graphics graphics(pDC->m_hDC);
			Gdiplus::Font font(L"Arial", 9);
			SolidBrush brush(Color(255, 128, 0, 128));  // 文字颜色为紫色

			// 文字的位置
			PointF point(offset_left + bmpWidth, offset_top + 5 * bmpHeight + 30);

			// 绘制文字
			//graphics.DrawString(L"中值滤波", -1, &font, point, &brush);

			PointF pointLaplacian(offset_left + 2*bmpWidth, offset_top + 5 * bmpHeight + 30);

			// 绘制文字
			graphics.DrawString(L"中值滤波+拉普拉斯变换", -1, &font, pointLaplacian, &brush);
		}

		// 释放临时数组内存
		delete[] median_laplacian_data;
		delete[] temp_data;

		GdiplusShutdown(gdiplusToken);
	}
	else {
		// 处理图像未加载的情况
		AfxMessageBox(_T("未加载灰度图片"));
	}
}

using namespace cv;
// 中心化频谱函数
void fftShift(Mat& mag)
{
	int cx = mag.cols / 2;
	int cy = mag.rows / 2;

	Mat q1(mag, cv::Rect(0, 0, cx, cy));
	Mat q2(mag, cv::Rect(cx, 0, cx, cy));
	Mat q3(mag, cv::Rect(0, cy, cx, cy));
	Mat q4(mag, cv::Rect(cx, cy, cx, cy));

	Mat tmp;
	q1.copyTo(tmp);
	q4.copyTo(q1);
	tmp.copyTo(q4);

	q2.copyTo(tmp);
	q3.copyTo(q2);
	tmp.copyTo(q3);
}
//FFT傅里叶变换
void CMFCApplication1View::OnFft()
{
	// TODO: 在此添加命令处理程序代码

	if (gray_data != nullptr) {
		// 将灰度数据转换为OpenCV的Mat对象，图像尺寸为bmpHeight x bmpWidth，数据类型为8位无符号整数
		Mat image(bmpHeight, bmpWidth, CV_8U, gray_data);

		// 执行傅里叶变换
		Mat padded; // 扩展输入图像到最佳尺寸
		int m = getOptimalDFTSize(image.rows);
		int n = getOptimalDFTSize(image.cols);
		copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, BORDER_CONSTANT, Scalar::all(0));//在图像周围填充零

		// 为频域图像分配内存
		Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };//存储频域表示的复杂图像
		Mat complexImage;
		merge(planes, 2, complexImage);

		// 执行傅里叶变换,从空间域转换到频域。
		dft(complexImage, complexImage);

		// 分离实部和虚部,存储在planes数组中
		split(complexImage, planes);

		// 计算幅度谱,将结果存储在planes数组的第一个平面
		magnitude(planes[0], planes[1], planes[0]);

		// 变换到对数尺度     //对幅度谱进行对数变换以增强可视化效果
		planes[0] += Scalar::all(1);
		log(planes[0], planes[0]);

		// 归一化到 [0, 1]
		normalize(planes[0], planes[0], 0, 1, NORM_MINMAX);

		// 显示未中心化的频谱图
		imshow("Spectrum (Uncentered)", planes[0]);

		// 中心化频谱
		fftShift(planes[0]);

		// 显示中心化后的频谱图
		imshow("Spectrum (Centered)", planes[0]);

		waitKey(0);//等待按键操作,然后关闭图像窗口
	}
	else {
		// 处理图像未加载的情况
		AfxMessageBox(_T("未加载灰度图片"));
	}
}
/*		//懒得搞了QAQ想实现的自己将下面代码放到上函数中并调节位置 
		// 将傅里叶变换后的图像画在MFC界面上
		CClientDC dc(this);
		CDC* pDC = &dc;

		// 创建一个CBitmap对象
		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(pDC, bmpWidth, bmpHeight);

		// 创建一个内存DC，并将位图选入内存DC
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

		// 设置傅里叶变换图像的像素数据
		for (int i = 0; i < bmpHeight; ++i) {
			for (int j = 0; j < bmpWidth; ++j) {
				uchar pixelValue = static_cast<uchar>(planes[0].at<float>(i, j) * 255);
				memDC.SetPixel(j, i, RGB(pixelValue, pixelValue, pixelValue));
			}
		}

		// 将内存DC上的图像绘制到MFC DC上
		pDC->BitBlt(offset_left, offset_top, bmpWidth, bmpHeight, &memDC, 0, 0, SRCCOPY);

		// 释放资源
		memDC.SelectObject(pOldBitmap);
		bitmap.DeleteObject();
*/
