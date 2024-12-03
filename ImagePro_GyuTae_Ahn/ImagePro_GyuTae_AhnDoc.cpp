
// ImagePro_GyuTae_AhnDoc.cpp : implementation of the CImageProGyuTaeAhnDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ImagePro_GyuTae_Ahn.h"
#endif

#include "ImagePro_GyuTae_AhnDoc.h"
#include <iostream>
#include <algorithm>
#include <propkey.h>
#include <math.h>
#include "Jpegfile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.14159265358979323846
#define NUM_FRAMES 10

// CImageProGyuTaeAhnDoc

IMPLEMENT_DYNCREATE(CImageProGyuTaeAhnDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageProGyuTaeAhnDoc, CDocument)
END_MESSAGE_MAP()


// CImageProGyuTaeAhnDoc construction/destruction

CImageProGyuTaeAhnDoc::CImageProGyuTaeAhnDoc() noexcept
{
	input_img = NULL;
	compare_img = NULL;
	output_img = NULL;
	imageHeight = 0, imageWidth = 0, depth = 0;

	gImageWidth = 0, gImageHeight = 0;
	gOutput_img = NULL;
	
	console_output = true;
}

CImageProGyuTaeAhnDoc::~CImageProGyuTaeAhnDoc()
{
}

BOOL CImageProGyuTaeAhnDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CImageProGyuTaeAhnDoc serialization

void CImageProGyuTaeAhnDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) {
		SaveImageFile(ar);
	}
	else {
		LoadImageFile(ar);
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CImageProGyuTaeAhnDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CImageProGyuTaeAhnDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CImageProGyuTaeAhnDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CImageProGyuTaeAhnDoc diagnostics

#ifdef _DEBUG
void CImageProGyuTaeAhnDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageProGyuTaeAhnDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImageProGyuTaeAhnDoc commands

// Image Load Operations
void CImageProGyuTaeAhnDoc::Load1Image() {
	if(console_output) std::cout << "[pDoc] Load1Image" << std::endl;
	CFile file;
	CFileDialog dlg(TRUE);

	if(console_output) std::cout << " >[Load1Image] Open AfxMessageBox - Select the image" << std::endl;
	AfxMessageBox("Select the image");

	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK) {
		if(console_output) std::cout << " >[Load1Image] Image selected" << std::endl;
		file.Open(dlg.GetPathName(), CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		LoadImageFile(ar);
		file.Close();
	}
	else if (nResponse == IDCANCEL) {
		if(console_output) std::cout << " >[Load1Image] Cancelled" << std::endl;
		return;
	}
	if(depth != 0) if(console_output) std::cout << " >[Load1Image] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::Load2Images() {
	if(console_output) std::cout << "[pDoc] Load2Images" << std::endl;
	CFile file;
	CFileDialog dlg(TRUE);

	if(console_output) std::cout << " >[Load2Images] Open AfxMessageBox - Select the first image" << std::endl;
	AfxMessageBox("Select the first image");
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK) {
		if(console_output) std::cout << " >[Load2Images] First image selected" << std::endl;
		file.Open(dlg.GetPathName(), CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		LoadImageFile(ar);
		file.Close();
	}
	else if (nResponse == IDCANCEL) {
		if(console_output) std::cout << " >[Load2Images] Cancelled" << std::endl;
		return;
	}

	if(console_output) std::cout << " >[Load2Images] Open AfxMessageBox - Select the second image" << std::endl;
	AfxMessageBox("Select the second image");

	nResponse = dlg.DoModal();
	if (nResponse == IDOK) {
		if(console_output) std::cout << " >[Load2Images] Second image selected" << std::endl;
		file.Open(dlg.GetPathName(), CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		LoadSecondImageFile(ar);
		file.Close();
	}
	else if (nResponse == IDCANCEL) {
		if(console_output) std::cout << " >[Load2Images] Cancelled" << std::endl;
		return;
	}
	if(depth != 0) if(console_output) std::cout << " >[Load2Images] Done" << std::endl;
	else if(console_output) std::cout << " >[Load2Images] Failed" << std::endl;
}

void CImageProGyuTaeAhnDoc::LoadImageFile(CArchive& ar) {
	CheckLoadedImage(ar);
	

	if(console_output) std::cout << " >[LoadImageFile] malloc input_img, output_img" << std::endl;
	input_img = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);
	output_img = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);

	for (int i = 0; i < imageHeight; i++) {
		input_img[i] = (unsigned char*)malloc(imageWidth * depth);
		output_img[i] = (unsigned char*)malloc(imageWidth * depth);
	}
	if(console_output) std::cout << " >[LoadImageFile] malloc done" << std::endl;

	for (int i = 0; i < imageHeight; i++) ar.Read(input_img[i], imageWidth * depth);
	if(console_output) std::cout << " >[LoadImageFile] Loaded" << std::endl;
} // LoadImageFile

void CImageProGyuTaeAhnDoc::LoadSecondImageFile(CArchive& ar) {
	CheckLoadedImage(ar);
	if (depth == 0) {
		return;
	}

	if(console_output) std::cout << " >[LoadSecondImageFile] malloc compare_img" << std::endl;
	compare_img = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);

	for (int i = 0; i < imageHeight; i++) {
		compare_img[i] = (unsigned char*)malloc(imageWidth * depth);
	}
	if(console_output) std::cout << " >[LoadSecondImageFile] malloc done" << std::endl;

	for (int i = 0; i < imageHeight; i++) ar.Read(compare_img[i], imageWidth * depth);
	if(console_output) std::cout << " >[LoadSecondImageFile] Loaded" << std::endl;
}

void CImageProGyuTaeAhnDoc::CheckLoadedImage(CArchive& ar) {
	if(console_output) std::cout << "[pDoc] Load Image File" << std::endl;
	int maxValue;
	CFile* file = ar.GetFile();
	CString fname = file->GetFileName();
	CString type, buf;

	if (strcmp(strrchr(fname, '.'), ".ppm") == 0 || strcmp(strrchr(fname, '.'), ".PPM") == 0 ||
		strcmp(strrchr(fname, '.'), ".pgm") == 0 || strcmp(strrchr(fname, '.'), ".PGM") == 0) {
		if(console_output) std::cout << " >[CheckLoadedImage] ppm or pgm file" << std::endl;
		ar.ReadString(type);

		if(console_output) std::cout << " >[CheckLoadedImage] Read header; type: " << type << std::endl;

		do {
			ar.ReadString(buf);
		} while (buf[0] == '#');
		sscanf_s(buf, "%d %d", &imageWidth, &imageHeight);

		if(console_output) std::cout << " >[CheckLoadedImage] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << std::endl;

		do {
			ar.ReadString(buf);
		} while (buf[0] == '#');
		sscanf_s(buf, "%d", &maxValue);
		if(console_output) std::cout << " >[CheckLoadedImage] maxValue: " << maxValue << std::endl;

		if (strcmp(type, "P5") == 0) depth = 1;
		else if (strcmp(type, "P6") == 0) depth = 3;
		else {
			depth = 0;
			if(console_output) std::cout << " >[CheckLoadedImage] Unsupported file format. Only can use P5 or P6." << std::endl;
			AfxMessageBox("Unsupported file format. Only can use P5 or P6.");
			return;
		}
		if(console_output) std::cout << " >[CheckLoadedImage] depth: " << depth << std::endl;
	}
	else if (strcmp(strrchr(fname, '.'), ".raw") == 0 || strcmp(strrchr(fname, '.'), ".RAW") == 0) {
		if(console_output) std::cout << " >[CheckLoadedImage] raw file" << std::endl;
		if (file->GetLength() != 256 * 256) {
			if(console_output) std::cout << " >[CheckLoadedImage] Unsupported file format. Only can use 256x256 raw file." << std::endl;
			AfxMessageBox("Unsupported file format. Only can use 256x256 raw file.");
			return;
		}

		imageWidth = 256;
		imageHeight = 256;
		depth = 1;

		if(console_output) std::cout << " >[CheckLoadedImage] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
	}
}

void CImageProGyuTaeAhnDoc::SaveImageFile(CArchive& ar) {
	if(console_output) std::cout << "[pDoc] SaveImageFile" << std::endl;
	CFile* file = ar.GetFile();
	CString fname = file->GetFilePath();
	if(console_output) std::cout << " >[SaveImageFile] fname: " << fname << std::endl;

	if (strcmp(strrchr(fname, '.'), ".ppm") == 0 || strcmp(strrchr(fname, '.'), ".PPM") == 0 ||
		strcmp(strrchr(fname, '.'), ".pgm") == 0 || strcmp(strrchr(fname, '.'), ".PGM") == 0) {
		if(console_output) std::cout << " >[SaveImageFile] ppm or pgm file" << std::endl;
		if(depth == 1) ar.WriteString("P5\n");
		else if(depth == 3) ar.WriteString("P6\n");

		char buf[256];

		sprintf_s(buf, "%d %d\n", imageWidth, imageHeight);
		ar.WriteString(buf);	
		ar.WriteString("255\n");
		if(console_output) std::cout << " >[SaveImageFile] header written" << std::endl;

		for (int i = 0; i < imageHeight; i++) ar.Write(output_img[i], imageWidth * depth);
		if(console_output) std::cout << " >[SaveImageFile] image written" << std::endl;
		if(console_output) std::cout << " >[SaveImageFile] Done" << std::endl;
	}
}//SaveImageFile

// Pixel Operations
void CImageProGyuTaeAhnDoc::PixelAdd()
{
	if(console_output) std::cout << "[pDoc] PixelAdd" << std::endl;
	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;
	if(console_output) std::cout << " >[PixelAdd] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
	int value = 0;

	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth * depth; x++) {
			value = input_img[y][x] + 100;
			if (value > 255) output_img[y][x] = 255;
			else output_img[y][x] = value;
		}
	}
	if(console_output) std::cout << " >[PixelAdd] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::PixelSub()
{
	if(console_output) std::cout << "[pDoc] PixelSub" << std::endl;
	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;
	if(console_output) std::cout << " >[PixelSub] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
	int value = 0;

	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth * depth; x++) {
			value = input_img[y][x] - 100;
			if (value < 0) output_img[y][x] = 0;
			else output_img[y][x] = value;
		}
	}
	if(console_output) std::cout << " >[PixelSub] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::PixelMul()
{
	if(console_output) std::cout << "[pDoc] PixelMul" << std::endl;
	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;
	if(console_output) std::cout << " >[PixelMul] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
	float value = 0;

	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth * depth; x++) {
			value = (float)(input_img[y][x] * 2);
			if (value > 255) output_img[y][x] = 255;
			else output_img[y][x] = (int)value;
		}
	}
	if(console_output) std::cout << " >[PixelMul] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::PixelDiv()
{
	if(console_output) std::cout << "[pDoc] PixelDiv" << std::endl;
	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;
	if(console_output) std::cout << " >[PixelDiv] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth * depth; x++) {
			output_img[y][x] = input_img[y][x] / 2;
		}
	}
	if(console_output) std::cout << " >[PixelDiv] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::PixelHistoEQ() {
	if(console_output) std::cout << "[pDoc] PixelHistoEQ" << std::endl;
	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;
	if(console_output) std::cout << " >[PixelHistoEQ] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	float N = (float)(imageHeight * imageWidth);
	int histo[256], sum[256];

	for (int d = 0; d < depth; d ++) {
		int acc_histo = 0;
		for (int i = 0; i < 256; i++) histo[i] = 0;
		if(console_output) std::cout << " >[PixelHistoEQ] histo initialization done" << std::endl;

		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				histo[input_img[y][depth * x + d]] += 1;
			}
		}
		if(console_output) std::cout << " >[PixelHistoEQ] histo calculation done" << std::endl;

		for (int i = 0; i < 256; i++) {
			acc_histo += histo[i];
			sum[i] = acc_histo;
		}
		if(console_output) std::cout << " >[PixelHistoEQ] sum calculation done" << std::endl;

		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				output_img[y][depth * x + d] = (unsigned char)(sum[input_img[y][depth * x + d]] / N * 255);
			}
		}
	}
	if(console_output) std::cout << " >[PixelHistoEQ] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::PixelContrast() {
	if(console_output) std::cout << "[pDoc] PixelContrast" << std::endl;
	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;
	if(console_output) std::cout << " >[PixelContrast] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
	float max = 0, min = 255;

	if (depth == 1) {
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				if (input_img[y][x] > max) max = input_img[y][x];
				if (input_img[y][x] < min) min = input_img[y][x];
			}
		}
	}
	else if (depth == 3) {
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				int i = (input_img[y][3 * x] + input_img[y][3 * x + 1] + input_img[y][3 * x + 2]) / 3;
				if (i > max) max = i;
				if (i < min) min = i;
			}
		}
	}
	if(console_output) std::cout << " >[PixelContrast] max: " << max << ", min: " << min << std::endl;

	if (depth == 1) {
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth * depth; x++) {
				output_img[y][x] = (unsigned char)((input_img[y][x] - min) / (max - min) * 255);
			}
		}
	}
	if (depth == 3) {
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				int i = (input_img[y][3 * x] + input_img[y][3 * x + 1] + input_img[y][3 * x + 2]) / 3;

				int ip = (int)((i - min) / (max - min) * 255);
				output_img[y][3 * x] = (unsigned char)input_img[y][3 * x] * ip / i;
				output_img[y][3 * x + 1] = (unsigned char)input_img[y][3 * x + 1] * ip / i;
				output_img[y][3 * x + 2] = (unsigned char)input_img[y][3 * x + 2] * ip / i;
			}
		}
	}
	if(console_output) std::cout << " >[PixelContrast] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::PixelBinarization(int threshold) {
	if(console_output) std::cout << "[pDoc] PixelBinarization" << std::endl;
	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;
	if(console_output) std::cout << " >[PixelBinarization] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
	if(console_output) std::cout << " >[PixelBinarization] threshold: " << threshold << std::endl;

	if (depth == 1) {
		if(console_output) std::cout << " >[PixelBinarization] Depth 1" << std::endl;
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				if (input_img[y][x] > threshold) output_img[y][x] = 255;
				else output_img[y][x] = 0;
			}
		}
	}
	else if (depth == 3) {
		if(console_output) std::cout << " >[PixelBinarization] Depth 3" << std::endl;
		int i = 0;
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				i = (input_img[y][3 * x] + input_img[y][3 * x + 1] + input_img[y][3 * x + 2]) / 3;
				if (i > threshold) {
					output_img[y][3 * x] = 255;
					output_img[y][3 * x + 1] = 255;
					output_img[y][3 * x + 2] = 255;
				}
				else {
					output_img[y][3 * x] = 0;
					output_img[y][3 * x + 1] = 0;
					output_img[y][3 * x + 2] = 0;
				}
			}
		}
	}
	if(console_output) std::cout << " >[PixelBinarization] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::PixelInvert() {
	if(console_output) std::cout << "[pDoc] PixelInvert" << std::endl;
	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;
	if(console_output) std::cout << " >[PixelInvert] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth * depth; x++) {
			output_img[y][x] = 255 - input_img[y][x];
		}
	}
	if(console_output) std::cout << " >[PixelInvert] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::PixelAdd2Images() {
	if(console_output) std::cout << "[pDoc] PixelAdd2Images" << std::endl;
	int value = 0;
	Load2Images();
	if (input_img == NULL || compare_img == NULL) return;
	if(console_output) std::cout << " >[PixelAdd2Images] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	if(console_output) std::cout << " >[PixelAdd2Images] Calculating" << std::endl;
	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth * depth; x++) {
			value = input_img[y][x] + compare_img[y][x];
			if (value > 255) output_img[y][x] = 255;
			else output_img[y][x] = value;
		}
	}
	if(console_output) std::cout << " >[PixelAdd2Images] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::PixelSub2Images() {
	if(console_output) std::cout << "[pDoc] PixelSub2Images" << std::endl;
	int value = 0;
	Load2Images();
	if (input_img == NULL || compare_img == NULL) return;
	if(console_output) std::cout << " >[PixelSub2Images] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	if(console_output) std::cout << " >[PixelSub2Images] Calculating" << std::endl;
	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth * depth; x++) {
			value = input_img[y][x] - compare_img[y][x];
			if (value < 0) output_img[y][x] = 0;
			else output_img[y][x] = value;
		}
	}
	if(console_output) std::cout << " >[PixelSub2Images] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::PixelDetection2Images() {
	if(console_output) std::cout << "[pDoc] PixelDetection2Images" << std::endl;
	int value = 0;
	Load2Images();
	if (input_img == NULL || compare_img == NULL) return;
	if(console_output) std::cout << " >[PixelDetection2Images] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	if(console_output) std::cout << " >[PixelDetection2Images] Calculating" << std::endl;
	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth * depth; x++) {
			value = abs(input_img[y][x] - compare_img[y][x]);
			if (value < 64) output_img[y][x] = 0;
			else output_img[y][x] = 255;
		}
	}
	if(console_output) std::cout << " >[PixelDetection2Images] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::Convolve(unsigned char** input_img, unsigned char** output_img, int cols, int rows, float mask[][3], int bias, int depth)
{
	if(console_output) std::cout << "[pDoc] Convolve" << std::endl;

	if(console_output) std::cout << " >[Convolve] malloc temp_img" << std::endl;
	unsigned char** temp_img = (unsigned char**)malloc((imageHeight + 2) * sizeof(unsigned char*));

	if(console_output) std::cout << " >[Convolve] calloc temp_img[i]" << std::endl;
	for (int i = 0; i < imageHeight + 2; i++)
		temp_img[i] = (unsigned char*)calloc(depth, (imageWidth + 2));

	if(console_output) std::cout << " >[Convolve] Copying input_img to temp_img" << std::endl;
	for (int y = 1; y < imageHeight + 1; y++)
	{
		for (int x = 1; x < imageWidth + 1; x++)
		{
			if (depth == 1) temp_img[y][x] = input_img[y - 1][x - 1];
			else if (depth == 3) {
				temp_img[y][3 * x] = input_img[y - 1][3 * (x - 1)];
				temp_img[y][3 * x + 1] = input_img[y - 1][3 * (x - 1) + 1];
				temp_img[y][3 * x + 2] = input_img[y - 1][3 * (x - 1) + 2];
			}
		}
	}

	if(console_output) std::cout << " >[Convolve] Convolution..." << std::endl;
	if (depth == 1) {
		if(console_output) std::cout << " >[Convolve] Depth 1" << std::endl;
		int sum = 0;
		for (int y = 0; y < imageHeight; y++)
			for (int x = 0; x < imageWidth; x++) {
				sum = 0;
				for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++)
					sum += (int)(temp_img[y + i][x + j] * mask[i][j]);
				sum += bias;

				if (sum > 255) output_img[y][x] = 255;
				else if (sum < 0) output_img[y][x] = 0;
				else output_img[y][x] = (unsigned char)sum;
			}
	}
	else if (depth == 3) {
		if(console_output) std::cout << " >[Convolve] Depth 3" << std::endl;
		int r, g, b;
		for (int y = 0; y < imageHeight; y++)
			for (int x = 0; x < imageWidth; x++) {
				r = 0, g = 0, b = 0;
				for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) {
					r += (int)(temp_img[y + i][3 * (x + j)] * mask[i][j]);
					g += (int)(temp_img[y + i][3 * (x + j) + 1] * mask[i][j]);
					b += (int)(temp_img[y + i][3 * (x + j) + 2] * mask[i][j]);
				}
				r += bias;
				g += bias;
				b += bias;

				if (r > 255) output_img[y][3 * x] = 255;
				else if (r < 0) output_img[y][3 * x] = 0;
				else output_img[y][3 * x] = (unsigned char)r;

				if (g > 255) output_img[y][3 * x + 1] = 255;
				else if (g < 0) output_img[y][3 * x + 1] = 0;
				else output_img[y][3 * x + 1] = (unsigned char)g;

				if (b > 255) output_img[y][3 * x + 2] = 255;
				else if (b < 0) output_img[y][3 * x + 2] = 0;
				else output_img[y][3 * x + 2] = (unsigned char)b;
			}
	}



	if(console_output) std::cout << " >[Convolve] releasing temp_img memory..." << std::endl;
	for(int i = 0; i < imageHeight + 2; i++) 
	{
		free(temp_img[i]);
	}
	free(temp_img);
	
	if(console_output) std::cout << " >[Convolve] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::Convolve(unsigned char** input_img, unsigned char** output_img, int cols, int rows, float mask[][5], int bias, int depth)
{
	if (console_output) std::cout << "[pDoc] Convolve" << std::endl;

	if (console_output) std::cout << " >[Convolve] malloc temp_img" << std::endl;
	unsigned char** temp_img = (unsigned char**)malloc((imageHeight + 4) * sizeof(unsigned char*));

	if (console_output) std::cout << " >[Convolve] calloc temp_img[i]" << std::endl;
	for (int i = 0; i < imageHeight + 4; i++)
		temp_img[i] = (unsigned char*)calloc(depth, (imageWidth + 4));

	if (console_output) std::cout << " >[Convolve] Copying input_img to temp_img" << std::endl;
	for (int y = 2; y < imageHeight + 2; y++)
	{
		for (int x = 2; x < imageWidth + 2; x++)
		{
			if (depth == 1) temp_img[y][x] = input_img[y - 2][x - 2];
			else if (depth == 3) {
				temp_img[y][3 * x] = input_img[y - 2][3 * (x - 2)];
				temp_img[y][3 * x + 1] = input_img[y - 2][3 * (x - 2) + 1];
				temp_img[y][3 * x + 2] = input_img[y - 2][3 * (x - 2) + 2];
			}
		}
	}

	if (console_output) std::cout << " >[Convolve] Convolution..." << std::endl;
	if (depth == 1) {
		if (console_output) std::cout << " >[Convolve] Depth 1" << std::endl;
		int sum = 0;
		for (int y = 0; y < imageHeight; y++)
			for (int x = 0; x < imageWidth; x++) {
				sum = 0;
				for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++)
					sum += (int)(temp_img[y + i][x + j] * mask[i][j]);
				sum += bias;

				if (sum > 255) output_img[y][x] = 255;
				else if (sum < 0) output_img[y][x] = 0;
				else output_img[y][x] = (unsigned char)sum;
			}
	}
	else if (depth == 3) {
		if (console_output) std::cout << " >[Convolve] Depth 3" << std::endl;
		int r, g, b;
		for (int y = 0; y < imageHeight; y++)
			for (int x = 0; x < imageWidth; x++) {
				r = 0, g = 0, b = 0;
				for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++) {
					r += (int)(temp_img[y + i][3 * (x + j)] * mask[i][j]);
					g += (int)(temp_img[y + i][3 * (x + j) + 1] * mask[i][j]);
					b += (int)(temp_img[y + i][3 * (x + j) + 2] * mask[i][j]);
				}
				r += bias;
				g += bias;
				b += bias;

				if (r > 255) output_img[y][3 * x] = 255;
				else if (r < 0) output_img[y][3 * x] = 0;
				else output_img[y][3 * x] = (unsigned char)r;

				if (g > 255) output_img[y][3 * x + 1] = 255;
				else if (g < 0) output_img[y][3 * x + 1] = 0;
				else output_img[y][3 * x + 1] = (unsigned char)g;

				if (b > 255) output_img[y][3 * x + 2] = 255;
				else if (b < 0) output_img[y][3 * x + 2] = 0;
				else output_img[y][3 * x + 2] = (unsigned char)b;
			}
	}



	if (console_output) std::cout << " >[Convolve] releasing temp_img memory..." << std::endl;
	for (int i = 0; i < imageHeight + 2; i++)
	{
		free(temp_img[i]);
	}
	free(temp_img);

	if (console_output) std::cout << " >[Convolve] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::RegionSharpening() {
	if(console_output) std::cout << "[pDoc] PixelRegionSharpening" << std::endl;
	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;
	if(console_output) std::cout << " >[PixelRegionSharpening] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
	float kernel[3][3] = {
		{0, -1, 0},
		{-1, 5, -1},
		{0, -1, 0}
	};
	if (depth == 1) {
		if(console_output) std::cout << " >[PixelRegionSharpening] Depth 1" << std::endl;
		Convolve(input_img, output_img, imageWidth, imageHeight, kernel, 0, depth);
	}
	else if (depth == 3) {
		if(console_output) std::cout << " >[PixelRegionSharpening] Depth 3" << std::endl;
		unsigned char** R = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);
		unsigned char** G = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);
		unsigned char** B = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);

		for (int i = 0; i < imageHeight; i++) {
			R[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth);
			G[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth);
			B[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth);
		}

		if(console_output) std::cout << " >[PixelRegionSharpening] Copying input_img to R, G, B" << std::endl;
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				R[y][x] = input_img[y][3 * x];
				G[y][x] = input_img[y][3 * x + 1];
				B[y][x] = input_img[y][3 * x + 2];
			}
		}

		if(console_output) std::cout << " >[PixelRegionSharpening] Convolve R, G, B with kernel..." << std::endl;
		Convolve(R, R, imageWidth, imageHeight, kernel, 0, 1);
		Convolve(G, G, imageWidth, imageHeight, kernel, 0, 1);
		Convolve(B, B, imageWidth, imageHeight, kernel, 0, 1);
		
		if(console_output) std::cout << " >[PixelRegionSharpening] Copying R, G, B to output_img" << std::endl;
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				output_img[y][3 * x] = R[y][x];
				output_img[y][3 * x + 1] = G[y][x];
				output_img[y][3 * x + 2] = B[y][x];
			}
		}

		if(console_output) std::cout << " >[PixelRegionSharpening] releasing R, G, B memory..." << std::endl;
		for (int i = 0; i < imageHeight; i++) {
			free(R[i]);
			free(G[i]);
			free(B[i]);
		}
		free(R);
		free(G);
		free(B);
	}
	if(console_output) std::cout << " >[PixelRegionSharpening] Done" << std::endl;
}


void CImageProGyuTaeAhnDoc::RegionBlurring() {
	if(console_output) std::cout << "[pDoc] PixelRegionBlurring" << std::endl;
	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;
	if(console_output) std::cout << " >[PixelRegionBlurring] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
	float kernel[5][5] = {
		{1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0},
		{1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0},
		{1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0},
		{1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0},
		{1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0, 1.0 / 25.0},
	};
	
	if (depth == 1) {
		if(console_output) std::cout << " >[PixelRegionBlurring] Depth 1" << std::endl;
		Convolve(input_img, output_img, imageWidth, imageHeight, kernel, 0, depth);
	}
	else if (depth == 3) {
		if(console_output) std::cout << " >[PixelRegionBlurring] Depth 3" << std::endl;
		unsigned char** R = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);
		unsigned char** G = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);
		unsigned char** B = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);

		for (int i = 0; i < imageHeight; i++) {
			R[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth);
			G[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth);
			B[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth);
		}

		if(console_output) std::cout << " >[PixelRegionBlurring] Copying input_img to R, G, B" << std::endl;
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				R[y][x] = input_img[y][3 * x];
				G[y][x] = input_img[y][3 * x + 1];
				B[y][x] = input_img[y][3 * x + 2];
			}
		}

		if(console_output) std::cout << " >[PixelRegionBlurring] Convolve R, G, B with kernel..." << std::endl;
		Convolve(R, R, imageWidth, imageHeight, kernel, 0, 1);
		Convolve(G, G, imageWidth, imageHeight, kernel, 0, 1);
		Convolve(B, B, imageWidth, imageHeight, kernel, 0, 1);

		if(console_output) std::cout << " >[PixelRegionBlurring] Copying R, G, B to output_img" << std::endl;
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				output_img[y][3 * x] = R[y][x];
				output_img[y][3 * x + 1] = G[y][x];
				output_img[y][3 * x + 2] = B[y][x];
			}
		}

		if(console_output) std::cout << " >[PixelRegionBlurring] releasing R, G, B memory..." << std::endl;
		for (int i = 0; i < imageHeight; i++) {
			free(R[i]);
			free(G[i]);
			free(B[i]);
		}
		free(R);
		free(G);
		free(B);
	}
	if(console_output) std::cout << " >[PixelRegionBlurring] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::RegionSobel() {
	if(console_output) std::cout << "[pDoc] PixelRegionSobel" << std::endl;
	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;
	if(console_output) std::cout << " >[PixelRegionSobel] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
	float mask1[3][3] = {
		{-1, -2, -1},
		{0, 0, 0},
		{1, 2, 1}
	};
	float mask2[3][3] = {
		{1, 0, -1},
		{2, 0, -2},
		{1, 0, -1}
	};
	 
	if(console_output) std::cout << " >[PixelRegionSobel] malloc Er, Ec" << std::endl;
	unsigned char** Er = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);
	unsigned char** Ec = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);

	for (int i = 0; i < imageHeight; i++) {
		Er[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth * depth);
		Ec[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth * depth);
	}

	if(console_output) std::cout << " >[PixelRegionSobel] Convolve Er, Ec" << std::endl;
	Convolve(input_img, Er, imageWidth, imageHeight, mask1, 0, depth);
	Convolve(input_img, Ec, imageWidth, imageHeight, mask2, 0, depth);

	if(console_output) std::cout << " >[PixelRegionSobel] Calculating output_img" << std::endl;
	if (depth == 1)
	{
		if (console_output) std::cout << " >[PixelRegionSobel] Depth 1" << std::endl;
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				output_img[y][x] = (int)sqrt((float)(Er[y][x] * Er[y][x] + Ec[y][x] * Ec[y][x]));
				if (output_img[y][x] > 255) output_img[y][x] = 255;
				else if (output_img[y][x] < 0) output_img[y][x] = 0;
			}
		}
	}
	else if (depth == 3) {
		if (console_output) std::cout << " >[PixelRegionSobel] Depth 3" << std::endl;
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				int value = 0;
				value += (int)sqrt((float)(Er[y][3 * x] * Er[y][3 * x] + Ec[y][3 * x] * Ec[y][3 * x]));
				value += (int)sqrt((float)(Er[y][3 * x + 1] * Er[y][3 * x + 1] + Ec[y][3 * x + 1] * Ec[y][3 * x + 1]));
				value += (int)sqrt((float)(Er[y][3 * x + 2] * Er[y][3 * x + 2] + Ec[y][3 * x + 2] * Ec[y][3 * x + 2]));
				value /= 3;

				if (value > 255) output_img[y][3 * x] = output_img[y][3 * x + 1] = output_img[y][3 * x + 2] = 255;
				else if (value < 0) output_img[y][3 * x] = output_img[y][3 * x + 1] = output_img[y][3 * x + 2] = 0;
				else output_img[y][3 * x] = output_img[y][3 * x + 1] = output_img[y][3 * x + 2] = value;
			}
		}
	}
	if(console_output) std::cout << " >[PixelRegionSobel] Done" << std::endl;
}//PixelRegionSobel

void CImageProGyuTaeAhnDoc::RegionPrewitt() {
	if(console_output) std::cout << "[pDoc] PixelRegionPrewitt" << std::endl;
	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;
	if(console_output) std::cout << " >[PixelRegionPrewitt] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
	float mask1[3][3] = {
		{-1, -1, -1},
		{0, 0, 0},
		{1, 1, 1}
	};
	float mask2[3][3] = {
		{1, 0, -1},
		{1, 0, -1},
		{1, 0, -1}
	};

	if(console_output) std::cout << " >[PixelRegionPrewitt] malloc Er, Ec" << std::endl;
	unsigned char** Er = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);
	unsigned char** Ec = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);

	for (int i = 0; i < imageHeight; i++) {
		Er[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth * depth);
		Ec[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth * depth);
	}

	if(console_output) std::cout << " >[PixelRegionPrewitt] Convolve Er, Ec" << std::endl;
	Convolve(input_img, Er, imageWidth, imageHeight, mask1, 0, depth);
	Convolve(input_img, Ec, imageWidth, imageHeight, mask2, 0, depth);

	if(console_output) std::cout << " >[PixelRegionPrewitt] Calculating output_img" << std::endl;
	if (depth == 1)
	{
		if (console_output) std::cout << " >[PixelRegionPrewitt] Depth 1" << std::endl;
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				output_img[y][x] = (int)sqrt((float)(Er[y][x] * Er[y][x] + Ec[y][x] * Ec[y][x]));
				if (output_img[y][x] > 255) output_img[y][x] = 255;
				else if (output_img[y][x] < 0) output_img[y][x] = 0;
			}
		}
	}
	else if (depth == 3) {
		if (console_output) std::cout << " >[PixelRegionPrewitt] Depth 3" << std::endl;
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				int value = 0;
				value += (int)sqrt((float)(Er[y][3 * x] * Er[y][3 * x] + Ec[y][3 * x] * Ec[y][3 * x]));
				value += (int)sqrt((float)(Er[y][3 * x + 1] * Er[y][3 * x + 1] + Ec[y][3 * x + 1] * Ec[y][3 * x + 1]));
				value += (int)sqrt((float)(Er[y][3 * x + 2] * Er[y][3 * x + 2] + Ec[y][3 * x + 2] * Ec[y][3 * x + 2]));
				value /= 3;

				if (value > 255) output_img[y][3 * x] = output_img[y][3 * x + 1] = output_img[y][3 * x + 2] = 255;
				else if (value < 0) output_img[y][3 * x] = output_img[y][3 * x + 1] = output_img[y][3 * x + 2] = 0;
				else output_img[y][3 * x] = output_img[y][3 * x + 1] = output_img[y][3 * x + 2] = value;
			}
		}
	}
	if(console_output) std::cout << " >[PixelRegionPrewitt] Done" << std::endl;
}//PixelRegionPrewitt

void CImageProGyuTaeAhnDoc::RegionRobert() {
	if(console_output) std::cout << "[pDoc] PixelRegionRobert" << std::endl;
	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;
	if(console_output) std::cout << " >[PixelRegionRobert] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
	float mask1[3][3] = {
		{-1, 0, 0},
		{0, 1, 0},
		{0, 0, 0}
	};
	float mask2[3][3] = {
		{0, 0, -1},
		{0, 1, 0},
		{0, 0, 0}
	};

	if(console_output) std::cout << " >[PixelRegionRobert] malloc Er, Ec" << std::endl;
	unsigned char** Er = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);
	unsigned char** Ec = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);

	for (int i = 0; i < imageHeight; i++) {
		Er[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth);
		Ec[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth);
	}

	if(console_output) std::cout << " >[PixelRegionRobert] Convolve Er, Ec" << std::endl;
	Convolve(input_img, Er, imageWidth, imageHeight, mask1, 0, depth);
	Convolve(input_img, Ec, imageWidth, imageHeight, mask2, 0, depth);

	if(console_output) std::cout << " >[PixelRegionRobert] Calculating output_img" << std::endl;
	if (depth == 1)
	{
		if (console_output) std::cout << " >[PixelRegionRobert] Depth 1" << std::endl;
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				output_img[y][x] = (int)sqrt((float)(Er[y][x] * Er[y][x] + Ec[y][x] * Ec[y][x]));
				if (output_img[y][x] > 255) output_img[y][x] = 255;
				else if (output_img[y][x] < 0) output_img[y][x] = 0;
			}
		}
	}
	else if (depth == 3) {
		if (console_output) std::cout << " >[PixelRegionRobert] Depth 3" << std::endl;
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				int value = 0;
				value += (int)sqrt((float)(Er[y][3 * x] * Er[y][3 * x] + Ec[y][3 * x] * Ec[y][3 * x]));
				value += (int)sqrt((float)(Er[y][3 * x + 1] * Er[y][3 * x + 1] + Ec[y][3 * x + 1] * Ec[y][3 * x + 1]));
				value += (int)sqrt((float)(Er[y][3 * x + 2] * Er[y][3 * x + 2] + Ec[y][3 * x + 2] * Ec[y][3 * x + 2]));
				value /= 3;

				if (value > 255) output_img[y][3 * x] = output_img[y][3 * x + 1] = output_img[y][3 * x + 2] = 255;
				else if (value < 0) output_img[y][3 * x] = output_img[y][3 * x + 1] = output_img[y][3 * x + 2] = 0;
				else output_img[y][3 * x] = output_img[y][3 * x + 1] = output_img[y][3 * x + 2] = value;
			}
		}
	}
	if(console_output) std::cout << " >[PixelRegionRobert] Done" << std::endl;
}//PixelRegionRobert

void CImageProGyuTaeAhnDoc::RegionMedianFilter() {
	if(console_output) std::cout << "[pDoc] PixelRegionMedianFilter" << std::endl;
	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;
	if(console_output) std::cout << " >[PixelRegionMedianFilter] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	if (depth == 1) {
		if(console_output) std::cout << " >[PixelRegionMedianFilter] Depth 1" << std::endl;
		int mask[9];

		if(console_output) std::cout << " >[PixelRegionMedianFilter] Calculating output_img" << std::endl;
		for (int y = 1; y < imageHeight - 1; y++) {
			for (int x = 1; x < imageWidth - 1; x++) {
				mask[0] = input_img[y - 1][x - 1];
				mask[1] = input_img[y - 1][x];
				mask[2] = input_img[y - 1][x + 1];
				mask[3] = input_img[y][x - 1];
				mask[4] = input_img[y][x];
				mask[5] = input_img[y][x + 1];
				mask[6] = input_img[y + 1][x - 1];
				mask[7] = input_img[y + 1][x];
				mask[8] = input_img[y + 1][x + 1];
				std::sort(mask, mask + 9);
				output_img[y][x] = mask[4];
			}
		}
	}
	else if (depth == 3) {
		if(console_output) std::cout << " >[PixelRegionMedianFilter] Depth 3" << std::endl;
		if (input_img == NULL) Load1Image();
		if(console_output) std::cout << " >[PixelRegionMedianFilter] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
		int mask_r[9];
		int mask_g[9];
		int mask_b[9];

		if(console_output) std::cout << " >[PixelRegionMedianFilter] Calculating output_img" << std::endl;
		for (int y = 1; y < imageHeight - 1; y++) {
			for (int x = 1; x < imageWidth - 1; x++) {
				mask_r[0] = input_img[y - 1][3 * (x - 1)];
				mask_r[1] = input_img[y - 1][3 * x];
				mask_r[2] = input_img[y - 1][3 * (x + 1)];
				mask_r[3] = input_img[y][3 * (x - 1)];
				mask_r[4] = input_img[y][3 * x];
				mask_r[5] = input_img[y][3 * (x + 1)];
				mask_r[6] = input_img[y + 1][3 * (x - 1)];
				mask_r[7] = input_img[y + 1][3 * x];
				mask_r[8] = input_img[y + 1][3 * (x + 1)];
				std::sort(mask_r, mask_r + 9);
				output_img[y][3 * x] = mask_r[4];

				mask_g[0] = input_img[y - 1][3 * (x - 1) + 1];
				mask_g[1] = input_img[y - 1][3 * x + 1];
				mask_g[2] = input_img[y - 1][3 * (x + 1) + 1];
				mask_g[3] = input_img[y][3 * (x - 1) + 1];
				mask_g[4] = input_img[y][3 * x + 1];
				mask_g[5] = input_img[y][3 * (x + 1) + 1];
				mask_g[6] = input_img[y + 1][3 * (x - 1) + 1];
				mask_g[7] = input_img[y + 1][3 * x + 1];
				mask_g[8] = input_img[y + 1][3 * (x + 1) + 1];
				std::sort(mask_g, mask_g + 9);
				output_img[y][3 * x + 1] = mask_g[4];

				mask_b[0] = input_img[y - 1][3 * (x - 1) + 2];
				mask_b[1] = input_img[y - 1][3 * x + 2];
				mask_b[2] = input_img[y - 1][3 * (x + 1) + 2];
				mask_b[3] = input_img[y][3 * (x - 1) + 2];
				mask_b[4] = input_img[y][3 * x + 2];
				mask_b[5] = input_img[y][3 * (x + 1) + 2];
				mask_b[6] = input_img[y + 1][3 * (x - 1) + 2];
				mask_b[7] = input_img[y + 1][3 * x + 2];
				mask_b[8] = input_img[y + 1][3 * (x + 1) + 2];
				std::sort(mask_b, mask_b + 9);
				output_img[y][3 * x + 2] = mask_b[4];
			}
		}
	}
	if(console_output) std::cout << " >[PixelRegionMedianFilter] Done" << std::endl;
}//PixelRegionMedianFilter

void CImageProGyuTaeAhnDoc::RegionEmbossing() {
	if(console_output) std::cout << "[pDoc] PixelRegionEmbossing" << std::endl;

	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;
	if(console_output) std::cout << " >[PixelRegionEmbossing] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	float mask[3][3] = {
		{-1, -1, 0},
		{-1, 0, 1},
		{0, 1, 1}
	};

	if(depth == 1) {
		if(console_output) std::cout << " >[PixelRegionEmbossing] Depth 1" << std::endl;
		Convolve(input_img, output_img, imageWidth, imageHeight, mask, 128, depth);
	}
	else if (depth == 3) {
		if(console_output) std::cout << " >[PixelRegionEmbossing] Depth 3" << std::endl;
		unsigned char** R = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);
		unsigned char** G = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);
		unsigned char** B = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);

		for (int i = 0; i < imageHeight; i++) {
			R[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth);
			G[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth);
			B[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth);
		}

		if(console_output) std::cout << " >[PixelRegionEmbossing] Copying input_img to R, G, B" << std::endl;
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				R[y][x] = input_img[y][3 * x];
				G[y][x] = input_img[y][3 * x + 1];
				B[y][x] = input_img[y][3 * x + 2];
			}
		}

		if(console_output) std::cout << " >[PixelRegionEmbossing] Convolve R, G, B with kernel..." << std::endl;
		Convolve(R, R, imageWidth, imageHeight, mask, 128, 1);
		Convolve(G, G, imageWidth, imageHeight, mask, 128, 1);
		Convolve(B, B, imageWidth, imageHeight, mask, 128, 1);

		if(console_output) std::cout << " >[PixelRegionEmbossing] Copying R, G, B to output_img" << std::endl;
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				output_img[y][3 * x] = R[y][x];
				output_img[y][3 * x + 1] = G[y][x];
				output_img[y][3 * x + 2] = B[y][x];
			}
		}

		if(console_output) std::cout << " >[PixelRegionEmbossing] releasing R, G, B memory..." << std::endl;
		for (int i = 0; i < imageHeight; i++) {
			free(R[i]);
			free(G[i]);
			free(B[i]);
		}
		free(R);
		free(G);
		free(B);
	}

	if(console_output) std::cout << " >[PixelRegionEmbossing] Done" << std::endl;
}//PixelRegionEmbossing

//Morphology
void CImageProGyuTaeAhnDoc::CopyResultToInput() {
	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth; x++) {
			input_img[y][x] = output_img[y][x];
		}
	}
}
void CImageProGyuTaeAhnDoc::Find_Connected_Region(int y, int x, int** label, int label_num) {
	if(label[y][x] != 0) return;

	label[y][x] = label_num;
	
	if (y > 0 && x > 0 && output_img[y - 1][x - 1] == 255)
		Find_Connected_Region(y - 1, x - 1, label, label_num);

	if (y > 0 && output_img[y - 1][x] == 255)
		Find_Connected_Region(y - 1, x, label, label_num);

	if (y > 0 && x < imageWidth - 1 && output_img[y - 1][x + 1] == 255)
		Find_Connected_Region(y - 1, x + 1, label, label_num);
	
	if (x > 0 && output_img[y][x - 1] == 255)
		Find_Connected_Region(y, x - 1, label, label_num);

	if (x < imageWidth - 1 && output_img[y][x + 1] == 255)
		Find_Connected_Region(y, x + 1, label, label_num);

	if (y < imageHeight - 1 && x > 0 && output_img[y + 1][x - 1] == 255)
		Find_Connected_Region(y + 1, x - 1, label, label_num);

	if (y < imageHeight - 1 && output_img[y + 1][x] == 255)
		Find_Connected_Region(y + 1, x, label, label_num);

	if (y < imageHeight - 1 && x < imageWidth - 1 && output_img[y + 1][x + 1] == 255)
		Find_Connected_Region(y + 1, x + 1, label, label_num);
} //Find_Connected_Region

void CImageProGyuTaeAhnDoc::Min_Value_Filter(unsigned char background) {
	if(console_output) std::cout << "[pDoc] Erosion" << std::endl;
	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;
	
	if(console_output) std::cout << " >[Erosion] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	int min;

	if(console_output) std::cout << " >[Erosion] malloc output_img" << std::endl;
	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth; x++) {
			if (y == 0 || x == 0 || y == imageHeight - 1 || x == imageWidth - 1) {
				output_img[y][x] = background;
			}
			else {
				min = 255;
				if (input_img[y - 1][x - 1] < min) min = input_img[y - 1][x - 1];
				if (input_img[y - 1][x] < min) min = input_img[y - 1][x];
				if (input_img[y - 1][x + 1] < min) min = input_img[y - 1][x + 1];
				if (input_img[y][x - 1] < min) min = input_img[y][x - 1];
				if (input_img[y][x] < min) min = input_img[y][x];
				if (input_img[y][x + 1] < min) min = input_img[y][x + 1];
				if (input_img[y + 1][x - 1] < min) min = input_img[y + 1][x - 1];
				if (input_img[y + 1][x] < min) min = input_img[y + 1][x];
				if (input_img[y + 1][x + 1] < min) min = input_img[y + 1][x + 1];
				output_img[y][x] = min;
			}
		}
	}
	if(console_output) std::cout << " >[Erosion] Done" << std::endl;
}//Erosion

void CImageProGyuTaeAhnDoc::Max_Value_Filter(unsigned char background) {
	if(console_output) std::cout << "[pDoc] Dilation" << std::endl;
	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;
	
	if(console_output) std::cout << " >[Dilation] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
	
	int max;

	if(console_output) std::cout << " >[Dilation] malloc output_img" << std::endl;
	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth; x++) {
			if (y == 0 || x == 0 || y == imageHeight - 1 || x == imageWidth - 1) {
				output_img[y][x] = background;
			}
			else {
				max = 0;
				if (input_img[y - 1][x - 1] > max) max = input_img[y - 1][x - 1];
				if (input_img[y - 1][x] > max) max = input_img[y - 1][x];
				if (input_img[y - 1][x + 1] > max) max = input_img[y - 1][x + 1];
				if (input_img[y][x - 1] > max) max = input_img[y][x - 1];
				if (input_img[y][x] > max) max = input_img[y][x];
				if (input_img[y][x + 1] > max) max = input_img[y][x + 1];
				if (input_img[y + 1][x - 1] > max) max = input_img[y + 1][x - 1];
				if (input_img[y + 1][x] > max) max = input_img[y + 1][x];
				if (input_img[y + 1][x + 1] > max) max = input_img[y + 1][x + 1];
				output_img[y][x] = max;
			}
		}
	}
	if(console_output) std::cout << " >[Dilation] Done" << std::endl;
}//Dilation

void CImageProGyuTaeAhnDoc::Opening(unsigned char background) {
	if(console_output) std::cout << "[pDoc] Opening" << std::endl;
	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;
	
	if(console_output) std::cout << " >[Opening] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	Min_Value_Filter(background);
	CopyResultToInput();
	Min_Value_Filter(background);
	CopyResultToInput();
	Min_Value_Filter(background);
	CopyResultToInput();
	Max_Value_Filter(background);
	CopyResultToInput();
	Max_Value_Filter(background);
	CopyResultToInput();
	Max_Value_Filter(background);
}//Opening

void CImageProGyuTaeAhnDoc::Closing(unsigned char background) {
	if(console_output) std::cout << "[pDoc] Closing" << std::endl;
	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;
	
	if(console_output) std::cout << " >[Closing] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	Max_Value_Filter(background);
	CopyResultToInput();
	Max_Value_Filter(background);
	CopyResultToInput();
	Max_Value_Filter(background);
	CopyResultToInput();
	Min_Value_Filter(background);
	CopyResultToInput();
	Min_Value_Filter(background);
	CopyResultToInput();
	Min_Value_Filter(background);
}//Closing

void CImageProGyuTaeAhnDoc::CountCell() {
	if(console_output) std::cout << "[pDoc] CountCell" << std::endl;
	Load1Image();
	if (input_img == NULL) return;
	
	if(console_output) std::cout << " >[CountCell] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	char buf[256];
	int label_no = 0;

	if(console_output) std::cout << " >[CountCell] Pixel Binarization..." << std::endl;
	PixelBinarization(100);
	CopyResultToInput();

	if(console_output) std::cout << " >[CountCell] Pixel Invert..." << std::endl;
	PixelInvert();
	CopyResultToInput();

	if(console_output) std::cout << " >[CountCell] Opening..." << std::endl;
	Opening(0);

	if(console_output) std::cout << " >[CountCell] malloc label" << std::endl;
	int x, y;
	int** label = (int**)malloc(imageHeight * sizeof(int));
	for (y = 0; y < imageHeight; y++) {
		label[y] = (int*)malloc(imageWidth * sizeof(int));
	}

	if(console_output) std::cout << " >[CountCell] initialize label" << std::endl;
	for (y = 0; y < imageHeight; y++) {
		for (x = 0; x < imageWidth; x++) {
			label[y][x] = 0;
		}
	}

	if(console_output) std::cout << " >[CountCell] Counting..." << std::endl;
	for (y = 0; y < imageHeight; y++) {
		for (x = 0; x < imageWidth; x++) {
			if (output_img[y][x] == 255 && label[y][x] == 0) {
				label_no = label_no + 1;
				Find_Connected_Region(y, x, label, label_no);
			}
		}
	}

	sprintf_s(buf, "Number of cells: %d", label_no);
	if(console_output) std::cout << " >[CountCell] " << buf << std::endl;
	AfxMessageBox(buf);
	if(console_output) std::cout << " >[CountCell] Done" << std::endl;
}//CountCell

void CImageProGyuTaeAhnDoc::GeometryZoominPixelCopy() {
	if(console_output) std::cout << "[pDoc] GeometryZoominPixelCopy" << std::endl;
	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;
	
	if(console_output) std::cout << " >[GeometryZoominPixelCopy] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	gImageWidth = imageWidth * 3;
	gImageHeight = imageHeight * 3;

	if(console_output) std::cout << " >[GeometryZoominPixelCopy] malloc gOutput_img" << std::endl;
	gOutput_img = (unsigned char**)malloc(sizeof(unsigned char*) * gImageHeight);

	for (int i = 0; i < gImageHeight; i++) {
		gOutput_img[i] = (unsigned char*)malloc(gImageWidth * depth);
	}
	
	if(console_output) std::cout << " >[GeometryZoominPixelCopy] Copying input_img to gOutput_img" << std::endl;
	if (depth == 1) {
		for (int y = 0; y < gImageHeight; y++)
			for (int x = 0; x < gImageWidth; x++)
				gOutput_img[y][x] = input_img[y / 3][x / 3];
	}
	else if (depth == 3) {
		for (int y = 0; y < gImageHeight; y++) {
			for (int x = 0; x < gImageWidth; x++){
				gOutput_img[y][3 * x] = input_img[y / 3][3 * (x / 3)];
				gOutput_img[y][3 * x + 1] = input_img[y / 3][3 * (x / 3) + 1];
				gOutput_img[y][3 * x + 2] = input_img[y / 3][3 * (x / 3) + 2];
			}

		}
	}
	if(console_output) std::cout << " >[GeometryZoominPixelCopy] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::GeometryZoominInterpolation() {
	if(console_output) std::cout << "[pDoc] GeometryZoominInterpolation" << std::endl;
	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;

	if(console_output) std::cout << " >[GeometryZoominInterpolation] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	float src_x, src_y;
	float alpha, beta;
	int scale_x, scale_y;
	int E, F;
	int Ax, Ay, Bx, By, Cx, Cy, Dx, Dy;

	scale_x = 3;
	scale_y = 3;

	gImageWidth = imageWidth * scale_x;
	gImageHeight = imageHeight * scale_y;

	if(console_output) std::cout << " >[GeometryZoominInterpolation] malloc gOutput_img" << std::endl;
	gOutput_img = (unsigned char**)malloc(gImageHeight * sizeof(unsigned char*));

	for (int i = 0; i < gImageHeight; i++) {
		gOutput_img[i] = (unsigned char*)malloc(gImageWidth * depth);
	}

	if(console_output) std::cout << " >[GeometryZoominInterpolation] Copying input_img to gOutput_img" << std::endl;
	for (int y = 0; y < gImageHeight; y++) {
		for (int x = 0; x < gImageWidth; x++) {
			src_x = (float)x / scale_x;
			src_y = (float)y / scale_y;

			alpha = src_x - x / scale_x;
			beta = src_y - y / scale_y;

			Ax = x / scale_x;
			Ay = y / scale_y;
			Bx = Ax + 1;
			By = Ay;
			Cx = Ax;
			Cy = Ay + 1;
			Dx = Ax + 1;
			Dy = Ay + 1;

			if (Bx > imageWidth - 1) Bx = imageWidth - 1;
			if (Dx > imageWidth - 1) Dx = imageWidth - 1;
			if (Cy > imageHeight - 1) Cy = imageHeight - 1;
			if (Dy > imageHeight - 1) Dy = imageHeight - 1;

			if (depth == 1) {
				E = (int)((1 - alpha) * input_img[Ay][Ax] + alpha * input_img[By][Bx]);
				F = (int)((1 - alpha) * input_img[Cy][Cx] + alpha * input_img[Dy][Dx]);

				gOutput_img[y][x] = (unsigned char)((1 - beta) * E + beta * F);
			}
			//when the image is color image
			else if (depth == 3) {
				E = (int)((1 - alpha) * input_img[Ay][3 * Ax] + alpha * input_img[By][3 * Bx]);
				F = (int)((1 - alpha) * input_img[Cy][3 * Cx] + alpha * input_img[Dy][3 * Dx]);
				gOutput_img[y][3 * x] = (unsigned char)((1 - beta) * E + beta * F);

				E = (int)((1 - alpha) * input_img[Ay][3 * Ax + 1] + alpha * input_img[By][3 * Bx + 1]);
				F = (int)((1 - alpha) * input_img[Cy][3 * Cx + 1] + alpha * input_img[Dy][3 * Dx + 1]);
				gOutput_img[y][3 * x + 1] = (unsigned char)((1 - beta) * E + beta * F);

				E = (int)((1 - alpha) * input_img[Ay][3 * Ax + 2] + alpha * input_img[By][3 * Bx + 2]);
				F = (int)((1 - alpha) * input_img[Cy][3 * Cx + 2] + alpha * input_img[Dy][3 * Dx + 2]);
				gOutput_img[y][3 * x + 2] = (unsigned char)((1 - beta) * E + beta * F);
				
			}
		}
	}
	if(console_output) std::cout << " >[GeometryZoominInterpolation] Done" << std::endl;
} // GeometryZoominInterpolation

void CImageProGyuTaeAhnDoc::GeometryZoomoutSubsampling() {
	if(console_output) std::cout << "[pDoc] GeometryZoomoutSubsampling" << std::endl;
	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;

	if(console_output) std::cout << " >[GeometryZoomoutSubsampling] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	int src_x, src_y;
	int scale_x = 3, scale_y = 3;

	gImageWidth = (imageWidth % scale_x == 0) ? imageWidth / scale_x : imageWidth / scale_x + 1;
	gImageHeight = (imageHeight % scale_y == 0) ? imageHeight / scale_y : imageHeight / scale_y + 1;

	if(console_output) std::cout << " >[GeometryZoomoutSubsampling] malloc gOutput_img" << std::endl;
	gOutput_img = (unsigned char**)malloc(gImageHeight * sizeof(unsigned char*));
	for (int i = 0; i < gImageHeight; i++) {
		gOutput_img[i] = (unsigned char*)malloc(gImageWidth * depth);
	}

	if(console_output) std::cout << " >[GeometryZoomoutSubsampling] Copying input_img to gOutput_img" << std::endl;
	for (int y = 0; y < gImageHeight; y++) {
		for (int x = 0; x < gImageWidth; x++) {
			src_x = x * scale_x;
			src_y = y * scale_y;

			if (depth == 1) {
				gOutput_img[y][x] = input_img[src_y][src_x];
			}
			else if (depth == 3) {
				gOutput_img[y][3 * x] = input_img[src_y][3 * src_x];
				gOutput_img[y][3 * x + 1] = input_img[src_y][3 * src_x + 1];
				gOutput_img[y][3 * x + 2] = input_img[src_y][3 * src_x + 2];
			}
		}
	}
	if(console_output) std::cout << " >[GeometryZoomoutSubsampling] Done" << std::endl;
} // GeometryZoomoutSubsampling

void CImageProGyuTaeAhnDoc::GeometryZoomoutAverage() {
	if(console_output) std::cout << "[pDoc] GeometryZoomoutAverage" << std::endl;
	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;

	if(console_output) std::cout << " >[GeometryZoomoutAverage] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	int src_x, src_y;
	int scale_x = 3, scale_y = 3;
	int sum_r, sum_g, sum_b, sum;

	gImageWidth = (imageWidth % scale_x == 0) ? imageWidth / scale_x : imageWidth / scale_x + 1;
	gImageHeight = (imageHeight % scale_y == 0) ? imageHeight / scale_y : imageHeight / scale_y + 1;

	if(console_output) std::cout << " >[GeometryZoomoutAverage] malloc gOutput_img" << std::endl;
	gOutput_img = (unsigned char**)malloc(gImageHeight * sizeof(unsigned char*));
	for (int i = 0; i < gImageHeight; i++) {
		gOutput_img[i] = (unsigned char*)malloc(gImageWidth * depth);
	}

	if(console_output) std::cout << " >[GeometryZoomoutAverage] Copying input_img to gOutput_img" << std::endl;
	for (int y = 0; y < gImageHeight; y++) {
		for (int x = 0; x < gImageWidth; x++) {
			src_x = x * scale_x;
			src_y = y * scale_y;

			if (depth == 1) {
				sum = 0;
				for (int i = 0; i < scale_y; i++) {
					for (int j = 0; j < scale_x; j++) {
						if (src_y + i < imageHeight && src_x + j < imageWidth) {
							sum += input_img[src_y + i][src_x + j];
						}
					}
				}
				gOutput_img[y][x] = sum / (scale_x * scale_y);
			}
			else if (depth == 3) {
				sum_r = 0;
				sum_g = 0;
				sum_b = 0;
				for (int i = 0; i < scale_y; i++) {
					for (int j = 0; j < scale_x; j++) {
						if (src_y + i < imageHeight && src_x + j < imageWidth) {
							sum_r += input_img[src_y + i][3 * (src_x + j)];
							sum_g += input_img[src_y + i][3 * (src_x + j) + 1];
							sum_b += input_img[src_y + i][3 * (src_x + j) + 2];
						}
					}
				}
				gOutput_img[y][3 * x] = sum_r / (scale_x * scale_y);
				gOutput_img[y][3 * x + 1] = sum_g / (scale_x * scale_y);
				gOutput_img[y][3 * x + 2] = sum_b / (scale_x * scale_y);
			
			}
		}
	}
} //GeometryZoomoutAverage

void  CImageProGyuTaeAhnDoc::GeometryRotate() {
	if(console_output) std::cout << "[pDoc] GeometryRotate" << std::endl;
	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;

	if(console_output) std::cout << " >[GeometryRotate] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	float angle = 150.0;
	float theta = PI / 180.0 * angle;

	int Cx = imageWidth / 2;
	int Cy = imageHeight / 2;

	int Hy = imageHeight - 1;

	gImageWidth = (int)(imageHeight * fabs(sin(theta)) + imageWidth * fabs(cos(theta)));
	gImageHeight = (int)(imageHeight * fabs(cos(theta)) + imageWidth * fabs(sin(theta)));
	//gImageWidth = (int)(imageHeight * cos(PI / 2.0 - theta) + imageWidth * cos(theta));
	//gImageHeight = (int)(imageHeight * cos(theta) + imageWidth * cos(PI / 2.0 - theta));

	if(console_output) std::cout << " >[GeometryRotate] gImageWidth: " << gImageWidth << ", gImageHeight: " << gImageHeight << std::endl;

	if(console_output) std::cout << " >[GeometryRotate] malloc gOutput_img" << std::endl;
	gOutput_img = (unsigned char**)malloc(gImageHeight * sizeof(unsigned char*));
	for (int i = 0; i < gImageHeight; i++) {
		gOutput_img[i] = (unsigned char*)malloc(gImageWidth * depth);
	}
	for (int y = 0; y < gImageHeight; y++) {
		for (int x = 0; x < gImageWidth; x++) {
			gOutput_img[y][x] = 255;
		}
	}

	int xDiff = (gImageWidth - imageWidth) / 2;
	int yDiff = (gImageHeight - imageHeight) / 2;

	if(console_output) std::cout << " >[GeometryRotate] Copying input_img to gOutput_img" << std::endl;
	int x_source, y_source;
	for (int y = -yDiff; y < gImageHeight - yDiff; y++) {
		for (int x = -xDiff; x < gImageWidth - xDiff; x++) {
			x_source = (int)(((Hy - y) - Cy) * sin(theta) + (x - Cx) * cos(theta) + Cx);
			y_source = (int)(((Hy - y) - Cy) * cos(theta) - (x - Cx) * sin(theta) + Cy);

			y_source = Hy - y_source;
			if (x_source < 0 || x_source > imageWidth - 1 || y_source < 0 || y_source > imageHeight - 1) {
				if (depth == 1) gOutput_img[y + yDiff][x + xDiff] = 255;
				else if (depth == 3) {
					gOutput_img[y + yDiff][3 * (x + xDiff)] = 255;
					gOutput_img[y + yDiff][3 * (x + xDiff) + 1] = 255;
					gOutput_img[y + yDiff][3 * (x + xDiff) + 2] = 255;
				}
			}
			else {
				if (depth == 1) {
					gOutput_img[y + yDiff][x + xDiff] = input_img[y_source][x_source];
				}
				else if (depth == 3) {
					gOutput_img[y + yDiff][3 * (x + xDiff)] = input_img[y_source][3 * x_source];
					gOutput_img[y + yDiff][3 * (x + xDiff) + 1] = input_img[y_source][3 * x_source + 1];
					gOutput_img[y + yDiff][3 * (x + xDiff) + 2] = input_img[y_source][3 * x_source + 2];
				}
			}
		}
	}
} //GeometryRotate

void CImageProGyuTaeAhnDoc::GeometryVerticalFlip() {
	if(console_output) std::cout << "[pDoc] GeometryVerticalFlip" << std::endl;
	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;

	if(console_output) std::cout << " >[GeometryVerticalFlip] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	gImageWidth = imageWidth;
	gImageHeight = imageHeight;

	if(console_output) std::cout << " >[GeometryVerticalFlip] malloc gOutput_img" << std::endl;
	gOutput_img = (unsigned char**)malloc(gImageHeight * sizeof(unsigned char*));
	for (int i = 0; i < gImageHeight; i++) {
		gOutput_img[i] = (unsigned char*)malloc(gImageWidth * depth);
	}

	if(console_output) std::cout << " >[GeometryVerticalFlip] Copying input_img to gOutput_img" << std::endl;
	for (int y = 0; y < gImageHeight; y++) {
		for (int x = 0; x < gImageWidth; x++) {
			if (depth == 1) {
				gOutput_img[y][x] = input_img[gImageHeight - 1 - y][x];
			}
			else if (depth == 3) {
				gOutput_img[y][3 * x] = input_img[gImageHeight - 1 - y][3 * x];
				gOutput_img[y][3 * x + 1] = input_img[gImageHeight - 1 - y][3 * x + 1];
				gOutput_img[y][3 * x + 2] = input_img[gImageHeight - 1 - y][3 * x + 2];
			}
		}
	}
	if(console_output) std::cout << " >[GeometryVerticalFlip] Done" << std::endl;

} //GeometryVerticalFlip

void CImageProGyuTaeAhnDoc::GeometryHorizontalFlip() {
	if(console_output) std::cout << "[pDoc] GeometryHorizontalFlip" << std::endl;
	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;

	if(console_output) std::cout << " >[GeometryHorizontalFlip] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	gImageWidth = imageWidth;
	gImageHeight = imageHeight;

	if(console_output) std::cout << " >[GeometryHorizontalFlip] malloc gOutput_img" << std::endl;
	gOutput_img = (unsigned char**)malloc(gImageHeight * sizeof(unsigned char*));
	for (int i = 0; i < gImageHeight; i++) {
		gOutput_img[i] = (unsigned char*)malloc(gImageWidth * depth);
	}

	if(console_output) std::cout << " >[GeometryHorizontalFlip] Copying input_img to gOutput_img" << std::endl;
	for (int y = 0; y < gImageHeight; y++) {
		for (int x = 0; x < gImageWidth; x++) {
			if (depth == 1) {
				gOutput_img[y][x] = input_img[y][gImageWidth - 1 - x];
			}
			else if (depth == 3) {
				gOutput_img[y][3 * x] = input_img[y][3 * (gImageWidth - 1 - x)];
				gOutput_img[y][3 * x + 1] = input_img[y][3 * (gImageWidth - 1 - x) + 1];
				gOutput_img[y][3 * x + 2] = input_img[y][3 * (gImageWidth - 1 - x) + 2];
			}
		}
	}
	if(console_output) std::cout << " >[GeometryHorizontalFlip] Done" << std::endl;

} //GeometryHorizontalFlip

void CImageProGyuTaeAhnDoc::GeometryWarping(control_line source_lines[], control_line dest_lines[], int control_line_num) {
	if(console_output) std::cout << "[pDoc] GeometryMorphing" << std::endl;
	Load1Image();
	if (input_img == NULL) return;

	if(console_output) std::cout << " >[GeometryMorphing] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	int num_lines = control_line_num;
	if(console_output) std::cout << " >[GeometryMorphing] num_lines: " << num_lines << std::endl;

	double u;
	double h;
	double d;
	double tx, ty; 
	double xp, yp; 
	double weight; 
	double totalWeight;
	double a = 0.001;
	double b = 2.0;
	double p = 0.75;
	int x1, x2, y1, y2;
	int src_x1, src_y1, src_x2, src_y2;
	double src_line_length, dest_line_length;
	int source_x, source_y;
	int last_row, last_col;
	last_row = imageHeight - 1;
	last_col = imageWidth - 1;
	if(console_output) std::cout << " >[GeometryMorphing] a: " << a << ", b: " << b << ", p: " << p << std::endl;

	if(console_output) std::cout << " >[GeometryMorphing] Calculating..." << std::endl;
	for (int y = 0; y < imageHeight; y++)
	{
		for (int x = 0; x < imageWidth; x++)
		{
			totalWeight = 0.0;
			tx = 0.0;
			ty = 0.0;

			for (int line = 0; line < num_lines; line++)
			{
				x1 = dest_lines[line].Px;
				y1 = dest_lines[line].Py;
				x2 = dest_lines[line].Qx;
				y2 = dest_lines[line].Qy;
				dest_line_length = sqrt((double)(x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
				u = (double)((x - x1) * (x2 - x1) + (y - y1) * (y2 - y1)) /
					(double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
				h = (double)((y - y1) * (x2 - x1) - (x - x1) * (y2 - y1)) / dest_line_length;

				if (u < 0) d = sqrt((double)(x - x1) * (x - x1) + (y - y1) * (y - y1));
				else if (u > 1) d = sqrt((double)(x - x2) * (x - x2) + (y - y2) * (y - y2));
				else d = fabs(h);
				src_x1 = source_lines[line].Px;
				src_y1 = source_lines[line].Py;
				src_x2 = source_lines[line].Qx;
				src_y2 = source_lines[line].Qy;
				src_line_length = sqrt((double)(src_x2 - src_x1) * (src_x2 - src_x1) + (src_y2 - src_y1) * (src_y2 - src_y1));

				xp = src_x1 + u * (src_x2 - src_x1) - h * (src_y2 - src_y1) / src_line_length;
				yp = src_y1 + u * (src_y2 - src_y1) + h * (src_x2 - src_x1) / src_line_length;
				weight = pow((pow((double)(dest_line_length), p) / (a + d)), b);
				tx += (xp - x) * weight;
				ty += (yp - y) * weight;
				totalWeight += weight;
			}
			source_x = x + (int)(tx / totalWeight + 0.5);
			source_y = y + (int)(ty / totalWeight + 0.5);
			if (source_x < 0) source_x = 0;
			if (source_x > last_col) source_x = last_col;
			if (source_y < 0) source_y = 0;
			if (source_y > last_row) source_y = last_row;
			output_img[y][x] = input_img[source_y][source_x];
		}
	}
	if(console_output) std::cout << " >[GeometryMorphing] Done" << std::endl;
} // GeometryWarping

void CImageProGyuTaeAhnDoc::GeometryWarpingNormal() {
	control_line source_lines[23] = {
		{116,7,207,5},{34,109,90,21},{55,249,30,128},{118,320,65,261},
		{123,321,171,321},{179,319,240,264},{247,251,282,135},{281,114,228,8},
		{78,106,123,109},{187,115,235,114},{72,142,99,128},{74,150,122,154},
		{108,127,123,146},{182,152,213,132},{183,159,229,157},{219,131,240,154},
		{80,246,117,212},{127,222,146,223},{154,227,174,221},{228,252,183,213},
		{114,255,186,257},{109,258,143,277},{152,278,190,262} };

	control_line dest_lines[23] = {
		{120,8,200,6},{12,93,96,16},{74,271,16,110},{126,336,96,290},
		{142,337,181,335},{192,335,232,280},{244,259,288,108},{285,92,212,13},
		{96,135,136,118},{194,119,223,125},{105,145,124,134},{110,146,138,151},
		{131,133,139,146},{188,146,198,134},{189,153,218,146},{204,133,221,140},
		{91,268,122,202},{149,206,159,209},{170,209,181,204},{235,265,208,199},
		{121,280,205,284},{112,286,160,301},{166,301,214,287} };

	GeometryWarping(source_lines, dest_lines, 23);
}

void CImageProGyuTaeAhnDoc::GeometryMyImageWarping() {
	control_line source_lines[5] = {
		{0, 0, 299, 0}, {0, 339, 299, 339}, {0, 0, 0, 339}, {299, 0, 299, 339}, {161, 168, 161, 205} };

	control_line dest_lines[5] = {
		{0, 0, 299, 0}, {0, 339, 299, 339}, {0, 0, 0, 339}, {299, 0, 299, 339}, {161, 168, 161, 235} };

	GeometryWarping(source_lines, dest_lines, 5);
} // GeometryWarping

void CImageProGyuTaeAhnDoc::GeometryMyImageWarping_Smile() {
	control_line source_lines[8] = {
		{0, 0, 299, 0}, {0, 339, 299, 339}, {0, 0, 0, 339}, {299, 0, 299, 339},
		{132, 248, 151, 245}, {151, 245, 166, 246}, {166, 246, 185, 245}, {185, 245, 205, 244} };

	control_line dest_lines[8] = {
		{0, 0, 299, 0}, {0, 339, 299, 339}, {0, 0, 0, 339}, {299, 0, 299, 339},
		{127, 238, 151, 245}, {151, 245, 166, 246}, {166, 246, 185, 245}, {185, 245, 207, 219} };

	GeometryWarping(source_lines, dest_lines, 5);
} // GeometryWarping

void CImageProGyuTaeAhnDoc::GeometryMorphing(control_line source_lines[], control_line dest_lines[], int control_line_num) {
	if(console_output) std::cout << "[pDoc] GeometryMorphing" << std::endl;
	Load2Images();
	if (input_img == NULL || compare_img == NULL) return;

	if(console_output) std::cout << " >[GeometryMorphing] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	int num_lines = control_line_num;
	
	if(console_output) std::cout << " >[GeometryMorphing] num_lines: " << num_lines << std::endl;

	double u;
	double h;
	double d;
	double tx, ty;
	double xp, yp;
	double weight;
	double totalWeight;
	double a = 0.001, b = 2.0, p = 0.75;
	unsigned char** warped_img;
	unsigned char** warped_img2;
	double fweight;
	control_line warp_lines[23];
	double tx2, ty2, xp2, yp2;
	int dest_x1, dest_y1, dest_x2, dest_y2, source_x2, source_y2;
	int x1, x2, y1, y2, src_x1, src_y1, src_x2, src_y2;
	double src_line_length, dest_line_length;
	int source_x, source_y, last_row, last_col;

	if(console_output) std::cout << " >[GeometryMorphing] a: " << a << ", b: " << b << ", p: " << p << std::endl;

	if(console_output) std::cout << " >[GeometryMorphing] malloc warped_img" << std::endl;
	warped_img = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));
	for (int i = 0; i < imageHeight; i++) {
		warped_img[i] = (unsigned char*)malloc(imageWidth * depth);
	}

	if(console_output) std::cout << " >[GeometryMorphing] malloc warped_img2" << std::endl;
	warped_img2 = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));
	for (int i = 0; i < imageHeight; i++) {
		warped_img2[i] = (unsigned char*)malloc(imageWidth * depth);
	}

	for (int i = 0; i < NUM_FRAMES; i++) {
		morphed_img[i] = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));
		for (int j = 0; j < imageHeight; j++) {
			morphed_img[i][j] = (unsigned char*)malloc(imageWidth * depth);
		}
	}
	last_row = imageHeight - 1;
	last_col = imageWidth - 1;

	for (int frame = 1; frame <= NUM_FRAMES; frame++)
	{
		fweight = (double)(frame) / NUM_FRAMES;
		for (int line = 0; line < num_lines; line++)
		{
			warp_lines[line].Px = (int)(source_lines[line].Px +
				(dest_lines[line].Px - source_lines[line].Px) * fweight);
			warp_lines[line].Py = (int)(source_lines[line].Py +
				(dest_lines[line].Py - source_lines[line].Py) * fweight);
			warp_lines[line].Qx = (int)(source_lines[line].Qx +
				(dest_lines[line].Qx - source_lines[line].Qx) * fweight);
			warp_lines[line].Qy = (int)(source_lines[line].Qy +
				(dest_lines[line].Qy - source_lines[line].Qy) * fweight);
		}
		for (int y = 0; y < imageHeight; y++)
		{
			for (int x = 0; x < imageWidth; x++)
			{
				totalWeight = 0.0;
				tx = 0.0;
				ty = 0.0;
				tx2 = 0.0;
				ty2 = 0.0;
				for (int line = 0; line < num_lines; line++)
				{
					x1 = warp_lines[line].Px;
					y1 = warp_lines[line].Py;
					x2 = warp_lines[line].Qx;
					y2 = warp_lines[line].Qy;
					dest_line_length = sqrt((double)(x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
					u = (double)((x - x1) * (x2 - x1) + (y - y1) * (y2 - y1)) /
						(double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
					h = (double)((y - y1) * (x2 - x1) - (x - x1) * (y2 - y1)) / dest_line_length;

					if (u < 0) d = sqrt((double)(x - x1) * (x - x1) + (y - y1) * (y - y1));
					else if (u > 1) d = sqrt((double)(x - x2) * (x - x2) + (y - y2) * (y - y2));
					else d = fabs(h);
					src_x1 = source_lines[line].Px;
					src_y1 = source_lines[line].Py;
					src_x2 = source_lines[line].Qx;
					src_y2 = source_lines[line].Qy;
					src_line_length = sqrt((double)(src_x2 - src_x1) * (src_x2 - src_x1) + (src_y2 - src_y1) * (src_y2 - src_y1));
					dest_x1 = dest_lines[line].Px;
					dest_y1 = dest_lines[line].Py;
					dest_x2 = dest_lines[line].Qx;
					dest_y2 = dest_lines[line].Qy;
					dest_line_length = sqrt((double)(dest_x2 - dest_x1) * (dest_x2 - dest_x1) + (dest_y2 - dest_y1) * (dest_y2 - dest_y1));

					xp = src_x1 + u * (src_x2 - src_x1) - h * (src_y2 - src_y1) / src_line_length;
					yp = src_y1 + u * (src_y2 - src_y1) + h * (src_x2 - src_x1) / src_line_length;
					xp2 = dest_x1 + u * (dest_x2 - dest_x1) - h * (dest_y2 - dest_y1) / dest_line_length;
					yp2 = dest_y1 + u * (dest_y2 - dest_y1) + h * (dest_x2 - dest_x1) / dest_line_length;
					weight = pow((pow((double)(dest_line_length), p) / (a + d)), b);

					tx += (xp - x) * weight;
					ty += (yp - y) * weight;
					tx2 += (xp2 - x) * weight;
					ty2 += (yp2 - y) * weight;
					totalWeight += weight;
				}
				source_x = x + (int)(tx / totalWeight + 0.5);
				source_y = y + (int)(ty / totalWeight + 0.5);

				source_x2 = x + (int)(tx2 / totalWeight + 0.5);
				source_y2 = y + (int)(ty2 / totalWeight + 0.5);

				if (source_x < 0) source_x = 0;
				if (source_x > last_col) source_x = last_col;
				if (source_y < 0) source_y = 0;
				if (source_y > last_row) source_y = last_row;
				if (source_x2 < 0) source_x2 = 0;
				if (source_x2 > last_col) source_x2 = last_col;
				if (source_y2 < 0) source_y2 = 0;
				if (source_y2 > last_row) source_y2 = last_row;
				warped_img[y][x] = input_img[source_y][source_x];
				warped_img2[y][x] = compare_img[source_y2][source_x2];
			}
		}
		// 모핑 결과 합병
		for (int y = 0; y < imageHeight; y++)
			for (int x = 0; x < imageWidth; x++) {
				int val = (int)((1.0 - fweight) * warped_img[y][x] +
					fweight * warped_img2[y][x]);
				if (val < 0) val = 0;
				if (val > 255) val = 255;
				morphed_img[frame - 1][y][x] = val;
			}
	}
}

void CImageProGyuTaeAhnDoc::GeometryMorphingNormal() {
	control_line source_lines[23] =
		{ {116,7,207,5},{34,109,90,21},{30,128,55,249},{65,261,118,320},
		{123,321,171,321},{179,319,240,264},{247,251,282,135},{228,8,281,114},
		{78,106,123,109},{187,115,235,114},{72,142,99,128},{74,150,122,154},
		{108,127,123,146},{182,152,213,132},{183,159,229,157},{219,131,240,154},
		{80,246,117,212},{127,222,146,223},{154,227,174,221},{183,213,228,252,},
		{114,255,186,257},{109,258,143,277},{152,278,190,262} };
	control_line dest_lines[23] =
		{ {120,8,200,6},{12,93,96,16},{16,110,74,271},{96,290,126,336},
		{142,337,181,335},{192,335,232,280},{244,259,288,108},{212,13,285,92},
		{96,135,136,118},{194,119,223,125},{105,145,124,134},{110,146,138,151},
		{131,133,139,146},{188,146,198,134},{189,153,218,146},{204,133,221,140},
		{91,268,122,202},{149,206,159,209},{170,209,181,204},{208,199,235,265},
		{121,280,205,284},{112,286,160,301},{166,301,214,287} };

	GeometryMorphing(source_lines, dest_lines, 23);
}

void CImageProGyuTaeAhnDoc::GeometryMorphingMyImg() {
	control_line source_lines[23] = {
		{87, 75, 229, 70}, {63, 204, 80, 81}, {63, 206, 97, 289}, {106, 290, 137, 297},
		{147, 299, 197, 243}, {207, 292, 236, 277}, {241, 271, 263, 209}, { 244, 87, 265, 201 },
		{90, 141, 136, 136}, {187, 132, 229, 137}, {100, 166, 126, 153}, {102, 171, 134, 171},
		{130, 235, 140, 171}, {186, 160, 201, 152}, {187, 166, 224, 163}, {205, 154, 224, 160},
		{114, 269, 143, 209}, {143, 216, 160, 212}, {173, 210, 183, 213}, {192, 203, 206, 225},
		{139, 245, 205, 239}, {137, 253, 170, 259}, {175, 258, 206, 245} };

	control_line dest_lines[23] =
	{ {120,8,200,6},{12,93,96,16},{16,110,74,271},{96,290,126,336},
	{142,337,181,335},{192,335,232,280},{244,259,288,108},{212,13,285,92},
	{96,135,136,118},{194,119,223,125},{105,145,124,134},{110,146,138,151},
	{131,133,139,146},{188,146,198,134},{189,153,218,146},{204,133,221,140},
	{91,268,122,202},{149,206,159,209},{170,209,181,204},{208,199,235,265},
	{121,280,205,284},{112,286,160,301},{166,301,214,287} };

	GeometryMorphing(source_lines, dest_lines, 23);
}

int CImageProGyuTaeAhnDoc::Reverse_Bit_Order(int index, int log2N) {
	unsigned int r = 0, b;
	for (unsigned int k = 0; k < log2N; k++) {
		b = (index & (1 << k)) >> k;
		r = (r << 1) | b;
	}
	return r;
}

void CImageProGyuTaeAhnDoc::Shuffle_Data(complex_num x[], int N, int log2N) {
	complex_num* temp = (complex_num*)malloc(N * sizeof(complex_num));
	for (int i = 0; i < N; i++) {
		temp[i].re = x[Reverse_Bit_Order(i, log2N)].re;
		temp[i].im = x[Reverse_Bit_Order(i, log2N)].im;
	}
	for (int i = 0; i < N; i++) {
		x[i].re = temp[i].re;
		x[i].im = temp[i].im;
	}
	free(temp);
}

void CImageProGyuTaeAhnDoc::Butterfly_Computation(complex_num x[], int N, int log2N, bool inverse) {
	
	complex_num* W = (complex_num*)malloc(sizeof(complex_num) * N / 2);
	complex_num temp;
	
	int groupSize, start, j;

	for (int k = 1; k <= log2N; k++) {
		groupSize = (int)pow((float)2, (int)k);
		for (int i = 0; i < groupSize / 2; i++) {
			W[i].re = cos(i * 2.0 * PI / (double)groupSize);
			if (inverse) W[i].im = sin(i * 2.0 * PI / (double)groupSize);
			else W[i].im = -sin(i * 2.0 * PI / (double)groupSize);
		}
		start = 0;
		for (int l = 0; l < N / groupSize; l++) {
			for (int i = start; i < start + groupSize / 2; i++) {
				j = i + groupSize / 2;
				temp.re = W[i - start].re * x[j].re - W[i - start].im * x[j].im;
				temp.im = W[i - start].im * x[j].re + W[i - start].re * x[j].im;
				x[j].re = x[i].re - temp.re;
				x[j].im = x[i].im - temp.im;
				x[i].re = x[i].re + temp.re;
				x[i].im = x[i].im + temp.im;
			}
			start = start + groupSize;
		}
	}
	if (inverse) {
		for (int i = 0; i < N; i++) {
			x[i].re = x[i].re / N;
			x[i].im = x[i].im / N;
		}
	}
}

void CImageProGyuTaeAhnDoc::FFT1D(complex_num x[], int N, int log2N, bool inverse) {
	Shuffle_Data(x, N, log2N);
	Butterfly_Computation(x, N, log2N, inverse);
}

void CImageProGyuTaeAhnDoc::FFT2D() {
	if(console_output) std::cout << "[pDoc] FFT2D" << std::endl;

	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;

	unsigned char** temp_img;

	int num = imageWidth;

	int log2N = 0;

	while (num >= 2) {
		num >>= 1;
		log2N++;
	}

	if (console_output) std::cout << " >[FFT2D] Malloc" << std::endl;
	complex_num* data = (complex_num*)malloc(imageWidth * sizeof(complex_num));

	fft_result = (complex_num**)malloc(imageHeight * sizeof(complex_num*));
	temp_img = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));

	for (int i = 0; i < imageHeight; i++) {
		fft_result[i] = (complex_num*)malloc(imageWidth * sizeof(complex_num));
		temp_img[i] = (unsigned char*)malloc(imageWidth);
	}

	if (console_output) std::cout << " >[FFT2D] Calculating..." << std::endl;
	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth; x++) {
			data[x].re = input_img[y][x];
			data[x].im = 0;
		}

		FFT1D(data, imageWidth, log2N, false);

		for (int x = 0; x < imageWidth; x++) {
			fft_result[y][x].re = data[x].re;
			fft_result[y][x].im = data[x].im;
		}
	}

	num = imageHeight;
	log2N = 0;
	while (num >= 2) {
		num >>= 1;
		log2N++;
	}

	free(data);
	data = (complex_num*)malloc(imageHeight * sizeof(complex_num));

	for (int x = 0; x < imageWidth; x++) {
		for (int y = 0; y < imageHeight; y++) {
			data[y].re = fft_result[y][x].re;
			data[y].im = fft_result[y][x].im;
		}

		FFT1D(data, imageHeight, log2N, false);

		for (int y = 0; y < imageHeight; y++) {
			fft_result[y][x].re = data[y].re;
			fft_result[y][x].im = data[y].im;
		}
	}
	
	int value;
	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth; x++) {
			value = 20 * (int)log(fabs(sqrt(fft_result[y][x].re * fft_result[y][x].re + fft_result[y][x].im * fft_result[y][x].im)));
			if (value > 255) value = 255;
			if (value < 0) value = 0;
			output_img[y][x] = (unsigned char)value;
		}
	}

	for (int i = 0; i < imageHeight; i += imageHeight / 2) {
		for (int j = 0; j < imageWidth; j += imageWidth / 2) {
			for (int row = 0; row < imageHeight / 2; row++) {
				for (int col = 0; col < imageWidth / 2; col++) {
					temp_img[(imageHeight / 2 - 1) - row + i][(imageWidth / 2 - 1) - col + j]
						= output_img[i + row][j + col];
				}
			}
		}
	}
	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth; x++) {
			output_img[y][x] = temp_img[y][x];
		}
	}
} // FFT2D

void CImageProGyuTaeAhnDoc::IFFT2D() {
	if (console_output) std::cout << "[pDoc] IFFT2D" << std::endl;

	if (input_img == NULL) Load1Image();
	if (input_img == NULL) return;
	if (fft_result == NULL) {
		std::cout << " >[IFFT2D] FFT2D first" << std::endl;
		AfxMessageBox("FFT2D first");
		return;
	}

	int num = imageWidth;
	int log2N = 0;

	while (num >= 2) {
		num >>= 1;
		log2N++;
	}

	if (console_output) std::cout << " >[IFFT2D] Malloc" << std::endl;
	complex_num* data = (complex_num*)malloc(imageWidth * sizeof(complex_num));

	ifft_result = (complex_num**)malloc(imageHeight * sizeof(complex_num*));
	for (int i = 0; i < imageHeight; i++) {
		ifft_result[i] = (complex_num*)malloc(imageWidth * sizeof(complex_num));
	}

	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth; x++) {
			data[x].re = fft_result[y][x].re;
			data[x].im = fft_result[y][x].im;
		}

		FFT1D(data, imageWidth, log2N, true);

		for (int x = 0; x < imageWidth; x++) {
			ifft_result[y][x].re = data[x].re;
			ifft_result[y][x].im = data[x].im;
		}
	}

	num = imageHeight;
	log2N = 0;
	while (num >= 2) {
		num >>= 1;
		log2N++;
	}

	free(data);
	data = (complex_num*)malloc(imageHeight * sizeof(complex_num));

	for (int x = 0; x < imageWidth; x++) {
		for (int y = 0; y < imageHeight; y++) {
			data[y].re = ifft_result[y][x].re;
			data[y].im = ifft_result[y][x].im;
		}

		FFT1D(data, imageHeight, log2N, true);

		for (int y = 0; y < imageHeight; y++) {
			ifft_result[y][x].re = data[y].re;
			ifft_result[y][x].im = data[y].im;
		}
	}

	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth; x++) {
			output_img[y][x] = (unsigned char)ifft_result[y][x].re;
		}
	}
} // IFFT2D

void CImageProGyuTaeAhnDoc::LowPassFilter() {
	if (console_output) std::cout << "[pDoc] LowPassFilter" << std::endl;
	int u, v;
	double B;

	double D0 = 32.0;
	double N = 2.0;

	FFT2D();

	if (console_output) std::cout << " >[LowPassFilter] Filtering..." << std::endl;
	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth; x++) {
			u = x, v = y;
			if (u > imageWidth / 2) u = imageWidth - u;
			if (v > imageHeight / 2) v = imageHeight - v;

			B = 1.0 / (1.0 + pow(sqrt((double)(u * u + v * v)) / D0, 2 * N));

			fft_result[y][x].re = fft_result[y][x].re * B;
			fft_result[y][x].im = fft_result[y][x].im * B;
		}
	}
	IFFT2D();
} // LowPassFilter

void CImageProGyuTaeAhnDoc::HighPassFilter() {
	if (console_output) std::cout << "[pDoc] HighPassFilter" << std::endl;
	int u, v;
	double B;

	double D0 = 32.0;
	double N = 2.0;

	FFT2D();

	if (console_output) std::cout << " >[HighPassFilter] Filtering..." << std::endl;
	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth; x++) {
			u = x, v = y;
			if (u > imageWidth / 2) u = imageWidth - u;
			if (v > imageHeight / 2) v = imageHeight - v;

			B = 1.0 / (1.0 + pow(D0 / sqrt((double)(u * u + v * v)), 2 * N));

			fft_result[y][x].re = fft_result[y][x].re * B;
			fft_result[y][x].im = fft_result[y][x].im * B;
		}
	}
	IFFT2D();
} // HighPassFilter

void CImageProGyuTaeAhnDoc::NoiseReduction() {
	if (console_output) std::cout << "[pDoc] NoiseReduction" << std::endl;
	FFT2D();
	if (console_output) std::cout << " >[NoiseReduction] Filtering..." << std::endl;
	fft_result[0][64].re = 0.0;
	fft_result[0][64].im = 0.0;
	fft_result[0][192].re = 0.0;
	fft_result[0][192].im = 0.0;
	IFFT2D();
} // NoiseReduction

void CImageProGyuTaeAhnDoc::LoadPCX() {
	if (console_output) std::cout << "[pDoc] LoadPCX" << std::endl;
	int run_length;
	unsigned char c;
	pcx_header pcxhead;
	CString fname;
	CFile file;
	CFileDialog dlg(TRUE);

	if (console_output) std::cout << " >[LoadPCX] Open File" << std::endl;
	if (dlg.DoModal() == IDOK) {
		fname = dlg.GetPathName();
		file.Open(fname, CFile::modeRead);

		auto ext = strrchr(fname, '.');
		if (console_output) std::cout << " >[LoadPCX] File path: "<< fname << std::endl;
		if (console_output) std::cout << " >[LoadPCX] File extension: " << ext << std::endl;

		if (strcmp(ext, ".PCX") == 0 || strcmp(ext, ".pcx") == 0) {
			file.Read(&pcxhead, sizeof(pcx_header));
			imageWidth = (pcxhead.xmax - pcxhead.xmin) + 1;
			imageHeight = (pcxhead.ymax - pcxhead.ymin) + 1;

			depth = pcxhead.color_planes;
			if (depth != 1) {
				if (console_output) std::cout << " >[LoadPCX] Not a monochrome image" << std::endl;
				AfxMessageBox("Not a monochrome image");
				return;
			}

			input_img = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));
			output_img = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));

			for (int i = 0; i < imageHeight; i++) {
				input_img[i] = (unsigned char*)malloc(imageWidth);
				output_img[i] = (unsigned char*)malloc(imageWidth);
			}

			for (int y = 0; y < imageHeight; y++) {
				for (int x = 0; x < imageWidth;) {
					file.Read(&c, 1);
					if ((c & 0xc0) == 0xc0) {
						run_length = c & 0x3f;
						file.Read(&c, 1);
						while (run_length--) input_img[y][x++] = c;
					}
					else {
						input_img[y][x++] = c;
					}
				}
			}
		}
		if (console_output) std::cout << " >[LoadPCX] Done" << std::endl;
	}
	else {
		if (console_output) std::cout << " >[LoadPCX] Cancelled" << std::endl;
	}
} // LoadPCX

void CImageProGyuTaeAhnDoc::SavePCX() {
	if (console_output) std::cout << "[pDoc] SavePCX" << std::endl;
	int run_length;
	unsigned char c, cur, prev;
	pcx_header pcxhead;
	CString fname;
	CFile file;
	CFileDialog dlg(TRUE);

	if (console_output) std::cout << " >[SavePCX] Select File Name" << std::endl;
	if (dlg.DoModal() == IDOK) {
		fname = dlg.GetPathName();
		file.Open(fname, CFile::modeCreate | CFile::modeWrite);
		if (console_output) std::cout << " >[SavePCX] File Name: " << fname << std::endl;

		auto ext = strrchr(fname, '.');
		if (strcmp(ext, ".PCX") == 0 || strcmp(ext, ".pcx") == 0) {
			memset(&pcxhead, 0, 128);
			pcxhead.manufacturer = 10;
			pcxhead.version = 5;
			pcxhead.encoding = 1;
			pcxhead.bit_per_pixel = 8;
			pcxhead.xmin = 0;
			pcxhead.ymin = 0;
			pcxhead.xmax = imageWidth - 1;
			pcxhead.ymax = imageHeight - 1;
			pcxhead.hres = 150;
			pcxhead.vres = 150;
			pcxhead.color_planes = 1;
			pcxhead.bytes_per_line = imageWidth;
			pcxhead.palette_type = 1;
			file.Write(&pcxhead, 128);

			if (console_output) std::cout << " >[LoadPCX] Save File..." << std::endl;
			for (int y = 0; y < imageHeight; y++) {
				run_length = 0;
				for (int x = 0; x < imageWidth; x++) {
					cur = input_img[y][x];
					run_length++;
					
					if (x < imageWidth - 1 && cur == input_img[y][x + 1] && run_length < 63) {
						continue;
					}
					else if (run_length > 1 || (0xC0 & cur) == 0xC0) {
						c = 0xC0 | (unsigned char)run_length;
						file.Write(&c, 1);
						file.Write(&cur, 1);
						run_length = 0;
					}
					else {
						file.Write(&cur, 1);
						run_length = 0;
					}
				}
			}
		}
		file.Close();
	}
	else {
		if (console_output) std::cout << " >[LoadPCX] Cancelled" << std::endl;
	}
	if (console_output) std::cout << " >[LoadPCX] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::LoadJPEG() {
	int run_length;
	unsigned char c;
	CString fname;
	BYTE* m_buffer;
	CFile file;
	CFileDialog dlg(TRUE);

	if (console_output) std::cout << " >[LoadJPEG] Open File" << std::endl;
	if (dlg.DoModal() == IDOK) {
		fname = dlg.GetPathName();

		if (console_output) std::cout << " >[LoadJPEG] File path: " << fname << std::endl;
		auto ext = strrchr(fname, '.');
		if (console_output) std::cout << " >[LoadJPEG] File extension: " << ext << std::endl;
		if (strcmp(ext, ".JPG") == 0 || strcmp(ext, ".jpg") == 0) {
			UINT width, height;

			m_buffer = JpegFile::JpegFileToRGB(fname, &width, &height);

			imageWidth = width;
			imageHeight = height;
			depth = 3;

			input_img = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));
			output_img = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));

			for (int i = 0; i < imageHeight; i++) {
				input_img[i] = (unsigned char*)malloc(imageWidth * depth);
				output_img[i] = (unsigned char*)malloc(imageWidth * depth);
			}

			for (int y = 0; y < imageHeight; y++) {
				for (int x = 0; x < imageWidth; x++) {
					input_img[y][x * depth] = m_buffer[(y * imageWidth + x) * depth];
					input_img[y][x * depth + 1] = m_buffer[(y * imageWidth + x) * depth + 1];
					input_img[y][x * depth + 2] = m_buffer[(y * imageWidth + x) * depth + 2];
				}
			}
		}
	}
} // LoadJPEG