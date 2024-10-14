
// ImagePro_GyuTae_AhnView.h : interface of the CImageProGyuTaeAhnView class
//

#pragma once

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")


class CImageProGyuTaeAhnView : public CScrollView
{
protected: // create from serialization only
	CImageProGyuTaeAhnView() noexcept;
	DECLARE_DYNCREATE(CImageProGyuTaeAhnView)

// Attributes
public:
	int viewMode;
	CImageProGyuTaeAhnDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CImageProGyuTaeAhnView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPixelAdd();
	afx_msg void OnPixelSub();
	afx_msg void OnPixelMul();
	afx_msg void OnPixelDiv();
	afx_msg void OnPixelHistoEq();
	afx_msg void OnPixelConstrast();
	afx_msg void OnPixelBinarization();
	afx_msg void OnPixelInvert();
	afx_msg void OnPixelAdd2Images();
	afx_msg void OnPixelSub2Images();
	afx_msg void OnPixelDetection();
	afx_msg void OnRegionSharpening();
	afx_msg void OnRegionBlurring();
	afx_msg void OnRegionSobel();
	afx_msg void OnMedianFilter();
	afx_msg void OnErosion();
	afx_msg void OnRegionRobert();
	afx_msg void OnRegionPrewitt();
	afx_msg void OnRegionEmbossing();
	afx_msg void OnDilation();
	afx_msg void OnOpening();
};

#ifndef _DEBUG  // debug version in ImagePro_GyuTae_AhnView.cpp
inline CImageProGyuTaeAhnDoc* CImageProGyuTaeAhnView::GetDocument() const
   { return reinterpret_cast<CImageProGyuTaeAhnDoc*>(m_pDocument); }
#endif

