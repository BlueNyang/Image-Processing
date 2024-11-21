
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
#define CAMERA 32

#define RGBMODE 1
#define YUVMODE 2
#define BMWIDTH 640
#define BMHEIGHT 480
#define IMAGE_SIZE (BMWIDTH * BMHEIGHT)

#define NO_OPERATION 0
#define SHARPENING 1
#define BLURRING 2
#define SOBEL 3
#define SUBTRACT 4
#define INVERT 5

#define CLIP(x) (((x) < 0) ? 0 : (((x) > 255) ? 255 : (x)))

// CImageProGyuTaeAhnView

IMPLEMENT_DYNCREATE(CImageProGyuTaeAhnView, CScrollView)

BEGIN_MESSAGE_MAP(CImageProGyuTaeAhnView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImageProGyuTaeAhnView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_COPY_OUT_TO_IN, &CImageProGyuTaeAhnView::OnCopyResultToInput)
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
	ON_COMMAND(ID_CAMERA_VIEW, &CImageProGyuTaeAhnView::OnCameraView)
	ON_COMMAND(ID_VIDEO_SHARPENING, &CImageProGyuTaeAhnView::OnVideoSharpening)
	ON_COMMAND(ID_VIDEO_BLURRING, &CImageProGyuTaeAhnView::OnVideoBlurring)
	ON_COMMAND(ID_VIDEO_SOBEL, &CImageProGyuTaeAhnView::OnVideoSobel)
	ON_COMMAND(ID_VIDEO_INVERT, &CImageProGyuTaeAhnView::OnVideoInvert)
	ON_COMMAND(ID_CAMERA_SHARPENING, &CImageProGyuTaeAhnView::OnCameraSharpening)
	ON_COMMAND(ID_CAMERA_BLURRING, &CImageProGyuTaeAhnView::OnCameraBlurring)
	ON_COMMAND(ID_CAMERA_SOBEL, &CImageProGyuTaeAhnView::OnCameraSobel)
	ON_COMMAND(ID_CAMERA_SUBTRACT, &CImageProGyuTaeAhnView::OnCameraSubtract)
	ON_COMMAND(ID_CAMERA_INVERT, &CImageProGyuTaeAhnView::OnCameraInvert)
	ON_COMMAND(ID_FFT_2D, &CImageProGyuTaeAhnView::OnFFT2D)
	ON_COMMAND(ID_IFFT_2D, &CImageProGyuTaeAhnView::OnIFFT2D)
	ON_COMMAND(ID_LOW_PASS_FILTER, &CImageProGyuTaeAhnView::OnLowPassFilter)
	ON_COMMAND(ID_HIGH_PASS_FILTER, &CImageProGyuTaeAhnView::OnHighPassFilter)
	ON_COMMAND(ID_NOISE_REDUCTION, &CImageProGyuTaeAhnView::OnNoiseReduction)
END_MESSAGE_MAP()

// CImageProGyuTaeAhnView construction/destruction

int IMGmode;
CImageProGyuTaeAhnView* imageProView_obj;

CImageProGyuTaeAhnView::CImageProGyuTaeAhnView() noexcept
{
	viewMode = TWO_IMAGES;
	IMGmode = YUVMODE;
	operation = NO_OPERATION;
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
	if (viewMode != CAMERA) std::cout << "[pView] Invalidating" << std::endl;
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
	else if (viewMode == CAMERA) {
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

			for (int i = 0; i < 5; i++) {
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

				if (operation == NO_OPERATION) {
					for (y = 0; y < fi.dwHeight; y++)
					{
						for (x = 0; x < fi.dwWidth; x++)
						{
							pDC->SetPixel(x, fi.dwHeight - y - 1,
								RGB(image[(y * fi.dwWidth + x) * 3 + 2], image[(y * fi.dwWidth + x) * 3 + 1], image[(y * fi.dwWidth + x) * 3]));
						}
					} // for frame
				}
				else if (operation == SHARPENING || operation == BLURRING || operation == SOBEL || operation == INVERT) {
					CImageProGyuTaeAhnDoc* pDoc = GetDocument();
					ASSERT_VALID(pDoc);

					if (!pDoc)
						return;
					int depth = 3;
					pDoc->console_output = false;
					pDoc->imageWidth = fi.dwWidth;
					pDoc->imageHeight = fi.dwHeight;
					pDoc->depth = depth;

					pDoc->input_img = (unsigned char**)malloc(sizeof(unsigned char*) * fi.dwHeight);
					pDoc->output_img = (unsigned char**)malloc(sizeof(unsigned char*) * fi.dwHeight);

					for (y = 0; y < pDoc->imageHeight; y++) {
						pDoc->input_img[y] = (unsigned char*)malloc(sizeof(unsigned char) * fi.dwWidth * depth);
						pDoc->output_img[y] = (unsigned char*)malloc(sizeof(unsigned char) * fi.dwWidth * depth);
					}

					for (y = 0; y < fi.dwHeight; y++)
					{
						for (x = 0; x < fi.dwWidth; x++)
						{
							if (depth == 1)
								pDoc->input_img[y][x] = image[(fi.dwHeight - 1 - y) * fi.dwWidth + x];
							else if (depth == 3)
							{
								pDoc->input_img[y][x * depth] = image[((fi.dwHeight - 1 - y) * fi.dwWidth + x) * 3 + 2];
								pDoc->input_img[y][x * depth + 1] = image[((fi.dwHeight - 1 - y) * fi.dwWidth + x) * 3 + 1];
								pDoc->input_img[y][x * depth + 2] = image[((fi.dwHeight - 1 - y) * fi.dwWidth + x) * 3];
							}
						}
					}

					if (operation == SHARPENING)
						pDoc->RegionSharpening();
					else if (operation == BLURRING)
						pDoc->RegionBlurring();
					else if (operation == SOBEL)
						pDoc->RegionSobel();
					else if (operation == INVERT)
						pDoc->PixelInvert();

					for (y = 0; y < fi.dwHeight; y++)
					{
						for (x = 0; x < fi.dwWidth * depth; x++)
						{
							if (depth == 1) pDC->SetPixel(x, y, RGB(pDoc->output_img[y][x], pDoc->output_img[y][x], pDoc->output_img[y][x]));
							else if (depth == 3) {
								pDC->SetPixel(x, y, RGB(
									pDoc->output_img[y][x * depth],
									pDoc->output_img[y][x * depth + 1],
									pDoc->output_img[y][x * depth + 2]
								));
							}
						}
					}

				} // if operation is SHARPENING or BLURRING or INVERT
				
			} // if stream type is video
		} // for stream
	} // for frame
	std::cout << " >[LoadAVIFile] Done" << std::endl;
	std::cout << " >[LoadAVIFile] Frame Close" << std::endl;
	if (pfrm != 0) AVIStreamGetFrameClose(pfrm);
	AVIStreamRelease(pstm);
	AVIFileRelease(pavi);

	std::cout << " >[LoadAVIFile] Exit" << std::endl;
	AVIFileExit();
}


void FrameCallbackProc(HWND hWnd, VIDEOHDR* hdr) {
	if (hWnd && hdr && hdr->lpData) {
		if (imageProView_obj) {
			imageProView_obj->OnFrame((unsigned char*)hdr->lpData);
		}
	}
}

void CImageProGyuTaeAhnView::OnFrame(unsigned char* data) {
	if (operation == SHARPENING || operation == BLURRING || operation == SOBEL || operation == INVERT) {
		CImageProGyuTaeAhnDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);

		if (!pDoc)
			return;

		int depth = 3;
		int x, y, i;
		pDoc->console_output = false;
		pDoc->imageWidth = BMWIDTH;
		pDoc->imageHeight = BMHEIGHT;
		pDoc->depth = depth;

		pDoc->input_img = (unsigned char**)malloc(sizeof(unsigned char*) * BMHEIGHT);
		pDoc->output_img = (unsigned char**)malloc(sizeof(unsigned char*) * BMHEIGHT);

		for (i = 0; i < pDoc->imageHeight; i++) {
			pDoc->input_img[i] = (unsigned char*)malloc(sizeof(unsigned char) * BMWIDTH * depth);
			pDoc->output_img[i] = (unsigned char*)malloc(sizeof(unsigned char) * BMWIDTH * depth);
		}

		if (IMGmode == RGBMODE) {
			for (y = 0; y < BMHEIGHT; y++) {
				for (x = 0; x < BMWIDTH * depth; x++) {
					pDoc->input_img[y][x] = data[y * BMWIDTH * depth + x];
				}
			}
		}
		else if (IMGmode == YUVMODE) {
			unsigned char dest[IMAGE_SIZE * 3];
			unsigned int rgbPos = 0;
			int y0, u, y1, v, r, g, b;
			for (unsigned int k = 0; k < IMAGE_SIZE * 2; k += 4) {
				y0 = data[k];
				u = data[k + 1];
				y1 = data[k + 2];
				v = data[k + 3];

				r = CLIP(1.164 * (y0 - 16) + 1.596 * (v - 128));
				g = CLIP(1.164 * (y0 - 16) - 0.813 * (v - 128) - 0.392 * (u - 128));
				b = CLIP(1.164 * (y0 - 16) + 2.017 * (u - 128));

				dest[rgbPos++] = r;
				dest[rgbPos++] = g;
				dest[rgbPos++] = b;

				r = CLIP(1.164 * (y1 - 16) + 1.596 * (v - 128));
				g = CLIP(1.164 * (y1 - 16) - 0.813 * (v - 128) - 0.392 * (u - 128));
				b = CLIP(1.164 * (y1 - 16) + 2.017 * (u - 128));

				dest[rgbPos++] = r;
				dest[rgbPos++] = g;
				dest[rgbPos++] = b;
			}
			for (y = 0; y < BMHEIGHT; y++) {
				for (x = 0; x < BMWIDTH * depth; x++) {
					pDoc->input_img[y][x] = dest[y * BMWIDTH * depth + x];
				}
			}
		}
		if (operation == SHARPENING)
			pDoc->RegionSharpening();
		else if (operation == BLURRING)
			pDoc->RegionBlurring();
		else if (operation == SOBEL)
			pDoc->RegionSobel();
		else if (operation == INVERT)
			pDoc->PixelInvert();

		if (IMGmode == RGBMODE) {
			for (y = 0; y < BMHEIGHT; y++) {
				for (x = 0; x < BMWIDTH * depth; x++) {
					data[y * BMWIDTH * depth + x] = pDoc->output_img[y][x];
				}
			}
		}
		else if (IMGmode == YUVMODE) {
			unsigned char y, u, v;
			int yuvPos = 0;

			int r, g, b;
			for (int row = 0; row < BMHEIGHT; row++) {
				for (int col = 0; col < BMWIDTH; col++) {
					r = pDoc->output_img[row][col * 3];
					g = pDoc->output_img[row][col * 3 + 1];
					b = pDoc->output_img[row][col * 3 + 2];

					y = (unsigned char)(0.257 * r + 0.504 * g + 0.095 * b + 16);
					u = (unsigned char)(-0.148 * r - 0.291 * g + 0.499 * b + 128);
					v = (unsigned char)(0.439 * r - 0.368 * g - 0.071 * b + 128);
					if (col % 2 == 0) {
						data[yuvPos++] = y;
						data[yuvPos++] = u;
					}
					else {
						data[yuvPos++] = y;
						data[yuvPos++] = v;
					}
				}
			}
		} // IMGmode == YUVMODE
	} // SHARPENING
	else if (operation == SUBTRACT) {
		static unsigned char* image1 = NULL;
		static unsigned char* image2 = NULL;

		if (IMGmode == RGBMODE) {
			int length = IMAGE_SIZE * 3;
			int i;

			if (image1 == NULL) {
				image1 = (unsigned char*)malloc(sizeof(unsigned char) * length);
				for (i = 0; i < length; i++) image1[i] = data[i];
			}
			else if (image2 == NULL) {
				image2 = (unsigned char*)malloc(sizeof(unsigned char) * length);
				for (i = 0; i < length; i++) image2[i] = data[i];
			}
			else {
				for (i = 0; i < length; i++) image1[i] = data[i];
				for (i = 0; i < length; i++) image2[i] = data[i];
			}
			if (image1 && image2) {
				for (int i = 0; i < BMWIDTH * BMHEIGHT; i++) {
					if (abs((image1[i * 3] + image1[i * 3 + 1] + image1[i * 3 + 2]) / 3 -
						(image2[i * 3] + image2[i * 3 + 1] + image2[i * 3 + 2]) / 3) < 20)
					{
						data[i * 3] = (char)0;
						data[i * 3 + 1] = (char)0;
						data[i * 3 + 2] = (char)0;
					}
					else {
						data[i * 3] = (char)255;
						data[i * 3 + 1] = (char)255;
						data[i * 3 + 2] = (char)255;
					}
				}
			}
		} // RGBMODE
		else if (IMGmode == YUVMODE) {
			int length = BMWIDTH * BMHEIGHT * 2;
			int i;
			if (image1 == NULL) {
				image1 = (unsigned char*)malloc(length);
				for (i = 0; i < length; i++) image1[i] = data[i];
			}
			else if (image2 == NULL) {
				image2 = (unsigned char*)malloc(length);
				for (i = 0; i < length; i++) image2[i] = data[i];
			}
			else {
				for (i = 0; i < length; i++) image1[i] = image2[i];
				for (i = 0; i < length; i++) image2[i] = data[i];
			}
			if (image1 && image2) {
				for (int i = 0; i < BMWIDTH * BMHEIGHT; i++) {
					if (abs(image1[i * 2] - image2[i * 2]) < 20)
					{
						data[i * 2] = (char)0;
						data[i * 2 + 1] = (char)128;
					}
					else {
						data[i * 2] = (char)255;
						data[i * 2 + 1] = (char)128;
					}
				}
			}
		} // YUVMODE
	} // SUBTRACT
	Invalidate(FALSE);
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

void CImageProGyuTaeAhnView::OnCopyResultToInput() {
	std::cout << "[pView] OnCopyResultToInput" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->CopyResultToInput();
	Invalidate(FALSE);
}

void CImageProGyuTaeAhnView::OnPixelAdd()
{
	std::cout << "[pView] OnPixelAdd" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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

	pDoc->console_output = true;
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
	pDoc->console_output = true;
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

void CImageProGyuTaeAhnView::OnCameraView()
{
	std::cout << "[pView] OnCameraView" << std::endl;
	char DeviceName[80];
	char DeviceVersion[80];
	HWND hWndC = 0;
	HWND parent = m_hWnd;

	for (int wIndex = 0; wIndex < 10; wIndex++) {
		if (capGetDriverDescription(wIndex, DeviceName, sizeof(DeviceName), DeviceVersion, sizeof(DeviceVersion))) {
			std::cout << "[pView] Camera Device Name: " << DeviceName << std::endl;
			std::cout << "[pView] Camera Device Version: " << DeviceVersion << std::endl;
			hWndC = capCreateCaptureWindow("My Own Capture Window",
				WS_CHILD | WS_VISIBLE, 0, 0, BMWIDTH, BMHEIGHT, parent, 0);
			if (capDriverConnect(hWndC, wIndex)) {
				std::cout << "[pView] Camera Connected" << std::endl;
				BITMAPINFO bmi;
				capPreviewRate(hWndC, 8);
				capPreview(hWndC, TRUE);
				memset(&bmi.bmiHeader, 0, sizeof(bmi.bmiHeader));

				if (IMGmode == RGBMODE) {
					std::cout << "[pView] RGB Mode" << std::endl;
					memset(&bmi.bmiHeader, 0, sizeof(bmi.bmiHeader));
					bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
					bmi.bmiHeader.biWidth = BMWIDTH;
					bmi.bmiHeader.biHeight = BMHEIGHT;
					bmi.bmiHeader.biBitCount = 24;
					bmi.bmiHeader.biPlanes = 1;
				}
				else {
					std::cout << "[pView] YUV Mode" << std::endl;
					memset(&bmi.bmiHeader, 0, sizeof(bmi.bmiHeader));
					capGetVideoFormat(hWndC, &bmi, sizeof(BITMAPINFO));
					bmi.bmiHeader.biWidth = BMWIDTH;
					bmi.bmiHeader.biHeight = BMHEIGHT;
					bmi.bmiHeader.biCompression = 0x32595559; // YUY2
					bmi.bmiHeader.biSizeImage = IMAGE_SIZE * 2;
				}
				if (capSetVideoFormat(hWndC, &bmi, sizeof(bmi))) {
					std::cout << "[pView] Set Video Format" << std::endl;
					capSetCallbackOnFrame(hWndC, FrameCallbackProc);
					viewMode = CAMERA;
					imageProView_obj = this;
					return;
				}
				else capDriverDisconnect(hWndC);
			}
			::DestroyWindow(hWndC);
		}
	}
} // OnCameraView

void CImageProGyuTaeAhnView::OnVideoSharpening() {
	std::cout << "[pView] OnAviView" << std::endl;
	CFile file;
	CFileDialog dlg(TRUE);

	std::cout << "[pView] Open avi File" << std::endl;
	if (dlg.DoModal() == IDOK) {
		strcpy_s(AVIFileName, dlg.GetPathName());
		std::cout << "[pView] FileName: " << AVIFileName << std::endl;
		operation = SHARPENING;
		viewMode = AVI_FILE;
	}
	Invalidate(FALSE);
}

void CImageProGyuTaeAhnView::OnVideoBlurring() {
	std::cout << "[pView] OnAviView" << std::endl;
	CFile file;
	CFileDialog dlg(TRUE);

	std::cout << "[pView] Open avi File" << std::endl;
	if (dlg.DoModal() == IDOK) {
		strcpy_s(AVIFileName, dlg.GetPathName());
		std::cout << "[pView] FileName: " << AVIFileName << std::endl;
		operation = BLURRING;
		viewMode = AVI_FILE;
	}
	Invalidate(FALSE);
}

void CImageProGyuTaeAhnView::OnVideoSobel() {
	std::cout << "[pView] OnAviView" << std::endl;
	CFile file;
	CFileDialog dlg(TRUE);

	std::cout << "[pView] Open avi File" << std::endl;
	if (dlg.DoModal() == IDOK) {
		strcpy_s(AVIFileName, dlg.GetPathName());
		std::cout << "[pView] FileName: " << AVIFileName << std::endl;
		operation = SOBEL;
		viewMode = AVI_FILE;
	}
	Invalidate(FALSE);
}

void CImageProGyuTaeAhnView::OnVideoInvert() {
	std::cout << "[pView] OnAviView" << std::endl;
	CFile file;
	CFileDialog dlg(TRUE);

	std::cout << "[pView] Open avi File" << std::endl;
	if (dlg.DoModal() == IDOK) {
		strcpy_s(AVIFileName, dlg.GetPathName());
		std::cout << "[pView] FileName: " << AVIFileName << std::endl;
		operation = INVERT;
		viewMode = AVI_FILE;
	}
	Invalidate(FALSE);
}

void CImageProGyuTaeAhnView::OnCameraSharpening() {
	std::cout << "[pView] OnCameraSharpening" << std::endl;
	operation = SHARPENING;
}

void CImageProGyuTaeAhnView::OnCameraBlurring() {
	std::cout << "[pView] OnCameraBlurring" << std::endl;
	operation = BLURRING;
}

void CImageProGyuTaeAhnView::OnCameraSobel() {
	std::cout << "[pView] OnCameraSobel" << std::endl;
	operation = SOBEL;
}

void CImageProGyuTaeAhnView::OnCameraSubtract() {
	std::cout << "[pView] OnCameraSubtract" << std::endl;
	operation = SUBTRACT;
}

void CImageProGyuTaeAhnView::OnCameraInvert() {
	std::cout << "[pView] OnCameraInvert" << std::endl;
	operation = INVERT;
}

void CImageProGyuTaeAhnView::OnFFT2D() {
	std::cout << "[pView] OnFFT2D" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc) return;
	pDoc->FFT2D();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}

void CImageProGyuTaeAhnView::OnIFFT2D() {
	std::cout << "[pView] OnIFFT2D" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc) return;
	pDoc->IFFT2D();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}

void CImageProGyuTaeAhnView::OnLowPassFilter() {
	std::cout << "[pView] OnLowPassFilter" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc) return;
	pDoc->LowPassFilter();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}

void CImageProGyuTaeAhnView::OnHighPassFilter() {
	std::cout << "[pView] OnHighPassFilter" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc) return;
	pDoc->HighPassFilter();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}

void CImageProGyuTaeAhnView::OnNoiseReduction() {
	std::cout << "[pView] OnNoiseReduction" << std::endl;
	CImageProGyuTaeAhnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc) return;
	pDoc->NoiseReduction();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
}