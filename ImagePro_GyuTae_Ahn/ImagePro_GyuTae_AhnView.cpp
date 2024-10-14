
// ImagePro_GyuTae_AhnView.cpp : implementation of the CImageProGyuTaeAhnView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ImagePro_GyuTae_Ahn.h"
#endif

#include "ImagePro_GyuTae_AhnDoc.h"
#include "ImagePro_GyuTae_AhnView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <iostream>

#define TWO_IMAGES 1
#define THREE_IMAGES 2

// CImageProGyuTaeAhnView

IMPLEMENT_DYNCREATE(CImageProGyuTaeAhnView, CScrollView)

BEGIN_MESSAGE_MAP(CImageProGyuTaeAhnView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImageProGyuTaeAhnView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_PIXEL_ADD, &CImageProGyuTaeAhnView::OnPixelAdd)
	ON_COMMAND(ID_PIXEL_SUB, &CImageProGyuTaeAhnView::OnPixelSub)
	ON_COMMAND(ID_PIXEL_MUL, &CImageProGyuTaeAhnView::OnPixelMul)
	ON_COMMAND(ID_PIXEL_DIV, &CImageProGyuTaeAhnView::OnPixelDiv)
	ON_COMMAND(ID_PIXEL_HISTO_EQ, &CImageProGyuTaeAhnView::OnPixelHistoEq)
	ON_COMMAND(ID_PIXEL_CONSTRAST, &CImageProGyuTaeAhnView::OnPixelConstrast)
	ON_COMMAND(ID_PIXEL_BINARIZATION, &CImageProGyuTaeAhnView::OnPixelBinarization)
	ON_COMMAND(ID_PIXEL_INVERT, &CImageProGyuTaeAhnView::OnPixelInvert)
	ON_COMMAND(ID_PIXEL_ADD_2_IMAGES, &CImageProGyuTaeAhnView::OnPixelAdd2Images)
	ON_COMMAND(ID_PIXEL_SUB_2_IMAGES, &CImageProGyuTaeAhnView::OnPixelSub2Images)
	ON_COMMAND(ID_PIXEL_DETECTION, &CImageProGyuTaeAhnView::OnPixelDetection)
	ON_COMMAND(ID_Region_Sharpening, &CImageProGyuTaeAhnView::OnRegionSharpening)
	ON_COMMAND(ID_REGION_BLURRING, &CImageProGyuTaeAhnView::OnRegionBlurring)
	ON_COMMAND(ID_REGION_SOBEL, &CImageProGyuTaeAhnView::OnRegionSobel)
	ON_COMMAND(ID_MEDIAN_FILTER, &CImageProGyuTaeAhnView::OnMedianFilter)
	ON_COMMAND(ID_EROSION, &CImageProGyuTaeAhnView::OnErosion)
	ON_COMMAND(ID_REGION_ROBERT, &CImageProGyuTaeAhnView::OnRegionRobert)
	ON_COMMAND(ID_REGION_PREWITT, &CImageProGyuTaeAhnView::OnRegionPrewitt)
	ON_COMMAND(ID_REGION_EMBOSSING, &CImageProGyuTaeAhnView::OnRegionEmbossing)
	ON_COMMAND(ID_DILATION, &CImageProGyuTaeAhnView::OnDilation)
	ON_COMMAND(ID_OPENING, &CImageProGyuTaeAhnView::OnOpening)
END_MESSAGE_MAP()

// CImageProGyuTaeAhnView construction/destruction

CImageProGyuTaeAhnView::CImageProGyuTaeAhnView() noexcept
{
	viewMode = TWO_IMAGES;
}

CImageProGyuTaeAhnView::~CImageProGyuTaeAhnView()
{
}

BOOL CImageProGyuTaeAhnView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CImageProGyuTaeAhnView drawing

void CImageProGyuTaeAhnView::OnDraw(CDC* pDC) {
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->input_img == NULL)
		return;

	std::cout << "[pView] Invalidating" << std::endl;

	if (pDoc->depth == 1) {
		std::cout << " >[OnDraw] Depth 1" << std::endl;
		for (int y = 0; y < pDoc->imageHeight; y++)
			for (int x = 0; x < pDoc->imageWidth; x++)
				pDC->SetPixel(x, y,
					RGB(pDoc->input_img[y][x], pDoc->input_img[y][x], pDoc->input_img[y][x]));

		if (viewMode == THREE_IMAGES) {
			std::cout << " >[OnDraw] Depth 1 - Three Images" << std::endl;
			for (int y = 0; y < pDoc->imageHeight; y++)
				for (int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
						RGB(pDoc->compare_img[y][x], pDoc->compare_img[y][x], pDoc->compare_img[y][x]));

			for (int y = 0; y < pDoc->imageHeight; y++)
				for (int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth * 2 + 60, y,
						RGB(pDoc->output_img[y][x], pDoc->output_img[y][x], pDoc->output_img[y][x]));
		}
		else {
			std::cout << " >[OnDraw] Depth 1 - Two Images" << std::endl;
			for (int y = 0; y < pDoc->imageHeight; y++)
				for (int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
						RGB(pDoc->output_img[y][x], pDoc->output_img[y][x], pDoc->output_img[y][x]));
		}
	}
	else if (pDoc->depth == 3) {
		std::cout << " >[OnDraw] Depth 3" << std::endl;
		for (int y = 0; y < pDoc->imageHeight; y++)
			for (int x = 0; x < pDoc->imageWidth; x++)
				pDC->SetPixel(x, y,
					RGB(pDoc->input_img[y][3 * x], pDoc->input_img[y][3 * x + 1], pDoc->input_img[y][3 * x + 2]));

		if (viewMode == THREE_IMAGES) {
			std::cout << " >[OnDraw] Depth 3 - Three Images" << std::endl;
			for (int y = 0; y < pDoc->imageHeight; y++)
				for (int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
						RGB(pDoc->compare_img[y][3 * x], pDoc->compare_img[y][3 * x + 1], pDoc->compare_img[y][3 * x + 2]));

			for (int y = 0; y < pDoc->imageHeight; y++)
				for (int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth * 2 + 60, y,
						RGB(pDoc->output_img[y][3 * x], pDoc->output_img[y][3 * x + 1], pDoc->output_img[y][3 * x + 2]));
		}
		else {
			std::cout << " >[OnDraw] Depth 3 - Two Images" << std::endl;
			for (int y = 0; y < pDoc->imageHeight; y++)
				for (int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
						RGB(pDoc->output_img[y][3 * x], pDoc->output_img[y][3 * x + 1], pDoc->output_img[y][3 * x + 2]));	
		}
	}
	std::cout << " >[OnDraw] Done" << std::endl;
}//OnDraws

void CImageProGyuTaeAhnView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = 2048;
	sizeTotal.cy = 1024;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CImageProGyuTaeAhnView printing


void CImageProGyuTaeAhnView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImageProGyuTaeAhnView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImageProGyuTaeAhnView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CImageProGyuTaeAhnView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CImageProGyuTaeAhnView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CImageProGyuTaeAhnView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CImageProGyuTaeAhnView diagnostics

#ifdef _DEBUG
void CImageProGyuTaeAhnView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageProGyuTaeAhnView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageProGyuTaeAhnDoc* CImageProGyuTaeAhnView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProGyuTaeAhnDoc)));
	return (CImageProGyuTaeAhnDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageProGyuTaeAhnView message handlers

void CImageProGyuTaeAhnView::OnPixelAdd()
{
	std::cout << "[pView] OnPixelAdd" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->PixelAdd();

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}

void CImageProGyuTaeAhnView::OnPixelSub()
{
	std::cout << "[pView] OnPixelSub" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->PixelSub();

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProGyuTaeAhnView::OnPixelMul()
{
	std::cout << "[pView] OnPixelMul" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->PixelMul();

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProGyuTaeAhnView::OnPixelDiv()
{
	std::cout << "[pView] OnPixelDiv" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->PixelDiv();

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProGyuTaeAhnView::OnPixelHistoEq()
{
	std::cout << "[pView] OnPixelHistoEq" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->PixelHistoEQ();

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProGyuTaeAhnView::OnPixelConstrast()
{
	std::cout << "[pView] OnPixelConstrast" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->PixelContrast();

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProGyuTaeAhnView::OnPixelBinarization()
{
	std::cout << "[pView] OnPixelBinarization" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->PixelBinarization();

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}

void CImageProGyuTaeAhnView::OnPixelInvert()
{
	std::cout << "[pView] OnPixelInvert" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->PixelInvert();

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProGyuTaeAhnView::OnPixelAdd2Images()
{
	std::cout << "[pView] OnPixelAdd2Images" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->PixelAdd2Images();

	viewMode = THREE_IMAGES;
	Invalidate(FALSE);
}



void CImageProGyuTaeAhnView::OnPixelSub2Images()
{
	std::cout << "[pView] OnPixelSub2Images" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->PixelSub2Images();

	viewMode = THREE_IMAGES;
	Invalidate(FALSE);
}

void CImageProGyuTaeAhnView::OnPixelDetection()
{
	std::cout << "[pView] OnPixelDetection" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->PixelDetection2Images();

	viewMode = THREE_IMAGES;
	Invalidate(FALSE);
}

void CImageProGyuTaeAhnView::OnRegionSharpening()
{
	std::cout << "[pView] OnRegionSharpening" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->RegionSharpening();

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProGyuTaeAhnView::OnRegionBlurring()
{
	std::cout << "[pView] OnRegionBlurring" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->RegionBlurring();

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProGyuTaeAhnView::OnRegionSobel()
{
	std::cout << "[pView] OnRegionSobel" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->RegionSobel();

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}

void CImageProGyuTaeAhnView::OnRegionPrewitt()
{
	std::cout << "[pView] OnRegionPrewitt" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->RegionPrewitt();

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}

void CImageProGyuTaeAhnView::OnRegionRobert()
{
	std::cout << "[pView] OnRegionRobert" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->RegionRobert();

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}

void CImageProGyuTaeAhnView::OnMedianFilter()
{
	std::cout << "[pView] OnMedianFilter" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->RegionMedianFilter();

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}

void CImageProGyuTaeAhnView::OnRegionEmbossing()
{
	std::cout << "[pView] OnRegionEmbossing" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->RegionEmbossing();

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);

}


void CImageProGyuTaeAhnView::OnErosion()
{
	std::cout << "[pView] OnErosion" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->Erosion();

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}

void CImageProGyuTaeAhnView::OnDilation()
{
	std::cout << "[pView] OnDilation" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->Dilation();

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProGyuTaeAhnView::OnOpening()
{
	std::cout << "[pView] OnOpening" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->Opening();

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}
