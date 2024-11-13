
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

#include <iostream>
#include <vfw.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TWO_IMAGES 1
#define THREE_IMAGES 2
#define TWO_IMAGES_SCALED 4
#define MORPHING 8
#define AVI_FILE 16

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
	ON_COMMAND(ID_REGION_ROBERT, &CImageProGyuTaeAhnView::OnRegionRobert)
	ON_COMMAND(ID_REGION_PREWITT, &CImageProGyuTaeAhnView::OnRegionPrewitt)
	ON_COMMAND(ID_REGION_EMBOSSING, &CImageProGyuTaeAhnView::OnRegionEmbossing)
	ON_COMMAND(ID_EROSION_0, &CImageProGyuTaeAhnView::OnErosion0)
	ON_COMMAND(ID_EROSION_255, &CImageProGyuTaeAhnView::OnErosion255)
	ON_COMMAND(ID_DILATION_0, &CImageProGyuTaeAhnView::OnDilation0)
	ON_COMMAND(ID_DILATION_255, &CImageProGyuTaeAhnView::OnDilation255)
	ON_COMMAND(ID_OPENING_0, &CImageProGyuTaeAhnView::OnOpening0)
	ON_COMMAND(ID_OPENING_255, &CImageProGyuTaeAhnView::OnOpening255)
	ON_COMMAND(ID_CLOSING_0, &CImageProGyuTaeAhnView::OnClosing0)
	ON_COMMAND(ID_CLOSING_255, &CImageProGyuTaeAhnView::OnClosing255)
	ON_COMMAND(ID_COUNT_CELL, &CImageProGyuTaeAhnView::OnCountCell)
	ON_COMMAND(ID_GEOMETRY_ZOOMIN_PIXEL_COPY, &CImageProGyuTaeAhnView::OnGeometryZoominPixelCopy)
	ON_COMMAND(ID_GEOMETRY_ZOOMIN_INTERPOLATION, &CImageProGyuTaeAhnView::OnGeometryZoominInterpolation)
	ON_COMMAND(ID_GEOMETRY_ZOOMOUT_SUBSAMPLING, &CImageProGyuTaeAhnView::OnGeometryZoomoutSubsampling)
	ON_COMMAND(ID_GEOMETRY_ZOOMOUT_AVERAGE, &CImageProGyuTaeAhnView::OnGeometryZoomoutAverage)
	ON_COMMAND(ID_GEOMETRY_ROTATE, &CImageProGyuTaeAhnView::OnGeometryRotate)
	ON_COMMAND(ID_GEOMETRY_VERTICAL_FLIP, &CImageProGyuTaeAhnView::OnGeometryVerticalFlip)
	ON_COMMAND(ID_GEOMETRY_HORIZONTAL_FLIP, &CImageProGyuTaeAhnView::OnGeometryHorizontalFlip)
	ON_COMMAND(ID_GEOMETRY_WARPING_NORMAL, &CImageProGyuTaeAhnView::OnGeometryWarping)
	ON_COMMAND(ID_GEOMETRY_MY_WARPING, &CImageProGyuTaeAhnView::OnGeometryMyWarping)
	ON_COMMAND(ID_GEOMETRY_WARPING_SMILE, &CImageProGyuTaeAhnView::OnGeometryWarpingSmile)
	ON_COMMAND(ID_GEOMETRY_MORPHING_NORMAL, &CImageProGyuTaeAhnView::OnGeometryMorphing)
	ON_COMMAND(ID_AVI_VIEW, &CImageProGyuTaeAhnView::OnAviView)
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
	std::cout << "[pView] Invalidating" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();

	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		std::cout << "[pView] pDoc is NULL" << std::endl;
		return;
	}

	if (viewMode == AVI_FILE) {
		std::cout << " >[OnDraw] viewMode: AVI FILE" << std::endl;
		LoadAVIFile(pDC);
		return;
	}

	if (pDoc->input_img == NULL)
	{
		//std::cout << "[pView] pDoc input img is NULL" << std::endl;
		return;
	}


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
		else if (viewMode == TWO_IMAGES_SCALED) {
			std::cout << " >[OnDraw] Depth 1 - Two Images Scaled" << std::endl;
			for (int y = 0; y < pDoc->gImageHeight; y++)
				for (int x = 0; x < pDoc->gImageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
						RGB(pDoc->gOutput_img[y][x], pDoc->gOutput_img[y][x], pDoc->gOutput_img[y][x]));
		}
		else if (viewMode == MORPHING) {
			std::cout << " >[OnDraw] Depth 1 - Morphing" << std::endl;
			for (int y = 0; y < pDoc->imageHeight; y++)
				for (int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
						RGB(pDoc->compare_img[y][x], pDoc->compare_img[y][x], pDoc->compare_img[y][x]));

			for (int i = 0; i < 10; i++) {
				for (int y = 0; y < pDoc->imageHeight; y++)
					for (int x = 0; x < pDoc->imageWidth; x++)
						pDC->SetPixel(x + pDoc->imageWidth * 2 + 60, y,
							RGB(pDoc->morphed_img[i][y][x], pDoc->morphed_img[i][y][x], pDoc->morphed_img[i][y][x]));
			}
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
		else if (viewMode == TWO_IMAGES_SCALED) {
			std::cout << " >[OnDraw] Depth 3 - Two Images Scaled" << std::endl;
			for (int y = 0; y < pDoc->gImageHeight; y++)
				for (int x = 0; x < pDoc->gImageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
						RGB(pDoc->gOutput_img[y][3 * x], pDoc->gOutput_img[y][3 * x + 1], pDoc->gOutput_img[y][3 * x + 2]));
		}
		else if (viewMode == MORPHING) {
			std::cout << " >[OnDraw] Depth 3 - Morphing is not supported" << std::endl;
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

void CImageProGyuTaeAhnView::LoadAVIFile(CDC* pDC) {
	std::cout << "[pView] LoadAVIFile: Load" << std::endl;
	LPBITMAPINFOHEADER pbmih; // size of header
	PAVIFILE pavi = NULL;
	PAVISTREAM pstm = NULL;
	PGETFRAME pfrm = NULL;

	AVIFILEINFO fi;
	AVISTREAMINFO si;

	int stm;
	int frame;
	int x, y;
	unsigned char* image;

	std::cout << " >[LoadAVIFile] avi file init" << std::endl;
	AVIFileInit(); //initialize
	AVIFileOpen(&pavi, AVIFileName, OF_READ | OF_SHARE_DENY_NONE, NULL); //open
	AVIFileInfo(pavi, &fi, sizeof(AVIFILEINFO)); //info

	std::cout << " >[LoadAVIFile] Frame - " << fi.dwStreams << std::endl;
	std::cout << " >[LoadAVIFile] Playing..." << std::endl;
	for (stm = 0; stm < fi.dwStreams; stm++) {
		AVIFileGetStream(pavi, &pstm, 0, stm); //stream
		AVIStreamInfo(pstm, &si, sizeof(si));

		if (si.fccType == streamtypeVIDEO) { //when the stream type is Video
			pfrm = AVIStreamGetFrameOpen(pstm, NULL); //frame open
			for (frame = 0; frame < si.dwLength; frame++) { // dwLength = total number of frames
				pbmih = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pfrm, frame); // return address of header
				if (!pbmih) continue;

				image = (unsigned char*)((LPSTR)pbmih + (WORD)pbmih->biSize);

				for (y = 0; y < fi.dwHeight; y++)
				{
					for (x = 0; x < fi.dwWidth; x++)
					{
						pDC->SetPixel(x, fi.dwHeight - y - 1,
							RGB(image[(y * fi.dwWidth + x) * 3 + 2], image[(y * fi.dwWidth + x) * 3 + 1], image[(y * fi.dwWidth + x) * 3]));
					}
				}
			}
		}
	}

	std::cout << " >[LoadAVIFile] Done" << std::endl;
	std::cout << " >[LoadAVIFile] Frame Close" << std::endl;
	if (pfrm != 0) AVIStreamGetFrameClose(pfrm);
	AVIStreamRelease(pstm);
	AVIFileRelease(pavi);

	std::cout << " >[LoadAVIFile] Exit" << std::endl;
	AVIFileExit();
}

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

	pDoc->PixelBinarization(120);

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

void CImageProGyuTaeAhnView::OnErosion(unsigned char background) {
	std::cout << "[pView] OnErosion" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc ->Min_Value_Filter(background);

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}

void CImageProGyuTaeAhnView::OnDilation(unsigned char background)
{
	std::cout << "[pView] OnDilation" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->Max_Value_Filter(background);

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}



void CImageProGyuTaeAhnView::OnOpening(unsigned char background)
{
	std::cout << "[pView] OnOpening" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->Opening(background);

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}

void CImageProGyuTaeAhnView::OnClosing(unsigned char background) {
	std::cout << "[pView] OnClosing" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->Closing(background);

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}

void CImageProGyuTaeAhnView::OnCountCell() {
	std::cout << "[pView] OnCountCell" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->CountCell();

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}

void CImageProGyuTaeAhnView::OnErosion0()
{
	OnErosion(0);
}
void CImageProGyuTaeAhnView::OnDilation255()
{
	OnErosion(255);
}
void CImageProGyuTaeAhnView::OnDilation0()
{
	OnDilation(0);
}
void CImageProGyuTaeAhnView::OnErosion255()
{
	OnDilation(255);
}
void CImageProGyuTaeAhnView::OnOpening0()
{
	OnOpening(0);
}
void CImageProGyuTaeAhnView::OnClosing255() {
	OnOpening(255);
}
void CImageProGyuTaeAhnView::OnClosing0()
{
	OnClosing(0);
}
void CImageProGyuTaeAhnView::OnOpening255()
{
	OnClosing(255);
}

//Geometry Pixel Processing
void CImageProGyuTaeAhnView::OnGeometryZoominPixelCopy()
{
	std::cout << "[pView] OnGeometryZoominPixelCopy" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->GeometryZoominPixelCopy();

	viewMode = TWO_IMAGES_SCALED;
	Invalidate(FALSE);
}

void CImageProGyuTaeAhnView::OnGeometryZoominInterpolation() {
	std::cout << "[pView] OnGeometryZoominPixelInterpolation" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;
	pDoc->GeometryZoominInterpolation();

	viewMode = TWO_IMAGES_SCALED;
	Invalidate(FALSE);
}


void CImageProGyuTaeAhnView::OnGeometryZoomoutSubsampling()
{
	std::cout << "[pView] OnGeometryZoomoutSubsampling" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;
	pDoc->GeometryZoomoutSubsampling();

	viewMode = TWO_IMAGES_SCALED;
	Invalidate(FALSE);
}


void CImageProGyuTaeAhnView::OnGeometryZoomoutAverage()
{
	std::cout << "[pView] OnGeometryZoomoutAverage" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;
	pDoc->GeometryZoomoutAverage();

	viewMode = TWO_IMAGES_SCALED;
	Invalidate(FALSE);
}


void CImageProGyuTaeAhnView::OnGeometryRotate()
{
	std::cout << "[pView] OnGeometryRotate" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;
	pDoc->GeometryRotate();

	viewMode = TWO_IMAGES_SCALED;
	Invalidate(FALSE);
}


void CImageProGyuTaeAhnView::OnGeometryVerticalFlip()
{
	std::cout << "[pView] OnGeometryVerticalFlip" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;
	pDoc->GeometryVerticalFlip();

	viewMode = TWO_IMAGES_SCALED;
	Invalidate(FALSE);
}


void CImageProGyuTaeAhnView::OnGeometryHorizontalFlip()
{
	std::cout << "[pView] OnGeometryHorizontalFlip" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;
	pDoc->GeometryHorizontalFlip();

	viewMode = TWO_IMAGES_SCALED;
	Invalidate(FALSE);
}


void CImageProGyuTaeAhnView::OnGeometryWarping()
{
	std::cout << "[pView] OnGeometryWarping" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;
	pDoc->GeometryWarpingNormal();

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProGyuTaeAhnView::OnGeometryMyWarping()
{
	std::cout << "[pView] OnGeometryWarping" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;
	pDoc->GeometryMyImageWarping();

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProGyuTaeAhnView::OnGeometryWarpingSmile()
{
	std::cout << "[pView] OnGeometryWarping" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;
	pDoc->GeometryMyImageWarping_Smile();

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}


void CImageProGyuTaeAhnView::OnGeometryMorphing()
{
	std::cout << "[pView] OnGeometryMorphing" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;
	pDoc->GeometryMorphingNormal();

	viewMode = MORPHING;
	Invalidate(FALSE);
}


void CImageProGyuTaeAhnView::OnAviView()
{
	std::cout << "[pView] OnAviView" << std::endl;
	CFile file;
	CFileDialog dlg(TRUE);

	std::cout << "[pView] Open avi File" << std::endl;
	if (dlg.DoModal() == IDOK) {
		strcpy_s(AVIFileName, dlg.GetPathName());
		std::cout << "[pView] FileName: " << AVIFileName << std::endl;
		viewMode = AVI_FILE;
	}
	Invalidate(FALSE);
}
