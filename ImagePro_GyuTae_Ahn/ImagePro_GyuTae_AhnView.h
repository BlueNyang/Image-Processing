
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
	CImageProGyuTaeAhnDoc* GetDocument() const;
	int viewMode;
	char AVIFileName[256];
	int operation;

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
	void LoadAVIFile(CDC* pDC);
	void OnFrame(unsigned char* data);

	afx_msg void OnCopyResultToInput();

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
	afx_msg void OnRegionRobert();
	afx_msg void OnRegionPrewitt();
	afx_msg void OnRegionEmbossing();

	afx_msg void OnErosion(unsigned char background);
	afx_msg void OnErosion0();
	afx_msg void OnErosion255();
	afx_msg void OnDilation(unsigned char background);
	afx_msg void OnDilation0();
	afx_msg void OnDilation255();
	afx_msg void OnOpening(unsigned char background);
	afx_msg void OnOpening0();
	afx_msg void OnOpening255();
	afx_msg void OnClosing(unsigned char background);
	afx_msg void OnClosing0();
	afx_msg void OnClosing255();
	afx_msg void OnCountCell();

	afx_msg void OnZoomInDialog();
	afx_msg void OnGeometryZoominPixelCopy();
	afx_msg void OnGeometryZoominInterpolation();
	afx_msg void OnGeometryZoomoutSubsampling();
	afx_msg void OnGeometryZoomoutAverage();
	afx_msg void OnGeometryRotate();
	afx_msg void OnGeometryVerticalFlip();
	afx_msg void OnGeometryHorizontalFlip();
	afx_msg void OnGeometryWarping();
	afx_msg void OnGeometryMyWarping();
	afx_msg void OnGeometryWarpingSmile();
	afx_msg void OnGeometryMorphing();
	afx_msg void OnGeometryMorphingMyImg();

	afx_msg void OnAviView();
	afx_msg void OnCameraView();

	afx_msg void OnVideoSharpening();
	afx_msg void OnVideoBlurring();
	afx_msg void OnVideoSobel();
	afx_msg void OnVideoInvert();

	afx_msg void OnCameraSharpening();
	afx_msg void OnCameraBlurring();
	afx_msg void OnCameraSobel();
	afx_msg void OnCameraSubtract();
	afx_msg void OnCameraInvert();

	afx_msg void OnFFT2D();
	afx_msg void OnIFFT2D();
	afx_msg void OnLowPassFilter();
	afx_msg void OnHighPassFilter();
	afx_msg void OnNoiseReduction();

	afx_msg void OnLoadPCX();
	afx_msg void OnSavePCX();
	afx_msg void OnLoadJPEG();
};

#ifndef _DEBUG  // debug version in ImagePro_GyuTae_AhnView.cpp
inline CImageProGyuTaeAhnDoc* CImageProGyuTaeAhnView::GetDocument() const
   { return reinterpret_cast<CImageProGyuTaeAhnDoc*>(m_pDocument); }
#endif

