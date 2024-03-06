
// MFCApplication1View.h: CMFCApplication1View 类的接口
//

#pragma once
#include "Bmp.h"

class CMFCApplication1View : public CView
{
protected: // 仅从序列化创建
	CMFCApplication1View() noexcept;
	DECLARE_DYNCREATE(CMFCApplication1View)

// 特性
public:
	CMFCApplication1Doc* GetDocument() const;
	//unsigned char* gray_data;  // 在合适的位置声明
	int flag = 0;
private:
	CBmp* m_pBmp; // CBmp 对象
	// 声明灰度图像数据成员
	unsigned char* gray_data;
	unsigned char* noisy_data;
	
// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMFCApplication1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDraw_line();
	afx_msg void OnNewbmp();
	afx_msg void OnContraststretching();
	afx_msg void OnTranslattogray();
	afx_msg void OnNegativetransform();
	afx_msg void OnLogtransform();
	afx_msg void OnPowerlawtransform();
	afx_msg void OnGraylevelslicing();
	afx_msg void OnBitplaneslicing();
	afx_msg void Onhistograme();
	afx_msg void OnAddsaltpeppernoise();
	afx_msg void OnMeanfilter();
	afx_msg void OnMedianfilter();
	afx_msg void OnLaplacian();
	afx_msg void OnHistogramequalizationandlaplacian();
	afx_msg void OnMedianlaplacian();
	afx_msg void OnFft();
};

#ifndef _DEBUG  // MFCApplication1View.cpp 中的调试版本
inline CMFCApplication1Doc* CMFCApplication1View::GetDocument() const
   { return reinterpret_cast<CMFCApplication1Doc*>(m_pDocument); }
#endif

