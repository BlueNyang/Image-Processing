
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
	std::cout << "[pDoc] Load1Image" << std::endl;
	CFile file;
	CFileDialog dlg(TRUE);

	std::cout << " >[Load1Image] Open AfxMessageBox - Select the image" << std::endl;
	AfxMessageBox("Select the image");

	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK) {
		std::cout << " >[Load1Image] Image selected" << std::endl;
		file.Open(dlg.GetPathName(), CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		LoadImageFile(ar);
		file.Close();
	}
	else if (nResponse == IDCANCEL) {
		std::cout << " >[Load1Image] Cancelled" << std::endl;
		return;
	}
	if(depth != 0) std::cout << " >[Load1Image] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::Load2Images() {
	std::cout << "[pDoc] Load2Images" << std::endl;
	CFile file;
	CFileDialog dlg(TRUE);

	std::cout << " >[Load2Images] Open AfxMessageBox - Select the first image" << std::endl;
	AfxMessageBox("Select the first image");
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK) {
		std::cout << " >[Load2Images] First image selected" << std::endl;
		file.Open(dlg.GetPathName(), CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		LoadImageFile(ar);
		file.Close();
	}
	else if (nResponse == IDCANCEL) {
		std::cout << " >[Load2Images] Cancelled" << std::endl;
		return;
	}

	std::cout << " >[Load2Images] Open AfxMessageBox - Select the second image" << std::endl;
	AfxMessageBox("Select the second image");

	nResponse = dlg.DoModal();
	if (nResponse == IDOK) {
		std::cout << " >[Load2Images] Second image selected" << std::endl;
		file.Open(dlg.GetPathName(), CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		LoadSecondImageFile(ar);
		file.Close();
	}
	else if (nResponse == IDCANCEL) {
		std::cout << " >[Load2Images] Cancelled" << std::endl;
		return;
	}
	if(depth != 0) std::cout << " >[Load2Images] Done" << std::endl;
	else std::cout << " >[Load2Images] Failed" << std::endl;
}

void CImageProGyuTaeAhnDoc::LoadImageFile(CArchive& ar) {
	CheckLoadedImage(ar);
	

	std::cout << " >[LoadImageFile] malloc input_img, output_img" << std::endl;
	input_img = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);
	output_img = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);

	for (int i = 0; i < imageHeight; i++) {
		input_img[i] = (unsigned char*)malloc(imageWidth * depth);
		output_img[i] = (unsigned char*)malloc(imageWidth * depth);
	}
	std::cout << " >[LoadImageFile] malloc done" << std::endl;

	for (int i = 0; i < imageHeight; i++) ar.Read(input_img[i], imageWidth * depth);
	std::cout << " >[LoadImageFile] Loaded" << std::endl;
} // LoadImageFile

void CImageProGyuTaeAhnDoc::LoadSecondImageFile(CArchive& ar) {
	CheckLoadedImage(ar);
	if (depth == 0) {
		return;
	}

	std::cout << " >[LoadSecondImageFile] malloc compare_img" << std::endl;
	compare_img = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);

	for (int i = 0; i < imageHeight; i++) {
		compare_img[i] = (unsigned char*)malloc(imageWidth * depth);
	}
	std::cout << " >[LoadSecondImageFile] malloc done" << std::endl;

	for (int i = 0; i < imageHeight; i++) ar.Read(compare_img[i], imageWidth * depth);
	std::cout << " >[LoadSecondImageFile] Loaded" << std::endl;
}

void CImageProGyuTaeAhnDoc::CheckLoadedImage(CArchive& ar) {
	std::cout << "[pDoc] Load Image File" << std::endl;
	int maxValue;
	CFile* file = ar.GetFile();
	CString fname = file->GetFileName();
	CString type, buf;

	if (strcmp(strrchr(fname, '.'), ".ppm") == 0 || strcmp(strrchr(fname, '.'), ".PPM") == 0 ||
		strcmp(strrchr(fname, '.'), ".pgm") == 0 || strcmp(strrchr(fname, '.'), ".PGM") == 0) {
		std::cout << " >[CheckLoadedImage] ppm or pgm file" << std::endl;
		ar.ReadString(type);

		std::cout << " >[CheckLoadedImage] Read header; type: " << type << std::endl;

		do {
			ar.ReadString(buf);
		} while (buf[0] == '#');
		sscanf_s(buf, "%d %d", &imageWidth, &imageHeight);

		std::cout << " >[CheckLoadedImage] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << std::endl;

		do {
			ar.ReadString(buf);
		} while (buf[0] == '#');
		sscanf_s(buf, "%d", &maxValue);
		std::cout << " >[CheckLoadedImage] maxValue: " << maxValue << std::endl;

		if (strcmp(type, "P5") == 0) depth = 1;
		else if (strcmp(type, "P6") == 0) depth = 3;
		else {
			depth = 0;
			std::cout << " >[CheckLoadedImage] Unsupported file format. Only can use P5 or P6." << std::endl;
			AfxMessageBox("Unsupported file format. Only can use P5 or P6.");
			return;
		}
		std::cout << " >[CheckLoadedImage] depth: " << depth << std::endl;
	}
	else if (strcmp(strrchr(fname, '.'), ".raw") == 0 || strcmp(strrchr(fname, '.'), ".RAW") == 0) {
		std::cout << " >[CheckLoadedImage] raw file" << std::endl;
		if (file->GetLength() != 256 * 256) {
			std::cout << " >[CheckLoadedImage] Unsupported file format. Only can use 256x256 raw file." << std::endl;
			AfxMessageBox("Unsupported file format. Only can use 256x256 raw file.");
			return;
		}

		imageWidth = 256;
		imageHeight = 256;
		depth = 1;

		std::cout << " >[CheckLoadedImage] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
	}
}

void CImageProGyuTaeAhnDoc::SaveImageFile(CArchive& ar) {
	std::cout << "[pDoc] SaveImageFile" << std::endl;
	CFile* file = ar.GetFile();
	CString fname = file->GetFilePath();
	std::cout << " >[SaveImageFile] fname: " << fname << std::endl;

	if (strcmp(strrchr(fname, '.'), ".ppm") == 0 || strcmp(strrchr(fname, '.'), ".PPM") == 0 ||
		strcmp(strrchr(fname, '.'), ".pgm") == 0 || strcmp(strrchr(fname, '.'), ".PGM") == 0) {
		std::cout << " >[SaveImageFile] ppm or pgm file" << std::endl;
		if(depth == 1) ar.WriteString("P5\n");
		else if(depth == 3) ar.WriteString("P6\n");

		char buf[256];

		sprintf_s(buf, "%d %d\n", imageWidth, imageHeight);
		ar.WriteString(buf);	
		ar.WriteString("255\n");
		std::cout << " >[SaveImageFile] header written" << std::endl;

		for (int i = 0; i < imageHeight; i++) ar.Write(output_img[i], imageWidth * depth);
		std::cout << " >[SaveImageFile] image written" << std::endl;
		std::cout << " >[SaveImageFile] Done" << std::endl;
	}
}//SaveImageFile

// Pixel Operations
void CImageProGyuTaeAhnDoc::PixelAdd()
{
	std::cout << "[pDoc] PixelAdd" << std::endl;
	if (input_img == NULL) Load1Image();
	std::cout << " >[PixelAdd] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
	int value = 0;

	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth * depth; x++) {
			value = input_img[y][x] + 100;
			if (value > 255) output_img[y][x] = 255;
			else output_img[y][x] = value;
		}
	}
	std::cout << " >[PixelAdd] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::PixelSub()
{
	std::cout << "[pDoc] PixelSub" << std::endl;
	if (input_img == NULL) Load1Image();
	std::cout << " >[PixelSub] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
	int value = 0;

	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth * depth; x++) {
			value = input_img[y][x] - 100;
			if (value < 0) output_img[y][x] = 0;
			else output_img[y][x] = value;
		}
	}
	std::cout << " >[PixelSub] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::PixelMul()
{
	std::cout << "[pDoc] PixelMul" << std::endl;
	if (input_img == NULL) Load1Image();
	std::cout << " >[PixelMul] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
	float value = 0;

	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth * depth; x++) {
			value = (float)(input_img[y][x] * 2);
			if (value > 255) output_img[y][x] = 255;
			else output_img[y][x] = (int)value;
		}
	}
	std::cout << " >[PixelMul] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::PixelDiv()
{
	std::cout << "[pDoc] PixelDiv" << std::endl;
	if (input_img == NULL) Load1Image();
	std::cout << " >[PixelDiv] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth * depth; x++) {
			output_img[y][x] = input_img[y][x] / 2;
		}
	}
	std::cout << " >[PixelDiv] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::PixelHistoEQ() {
	std::cout << "[pDoc] PixelHistoEQ" << std::endl;
	if (input_img == NULL) Load1Image();
	std::cout << " >[PixelHistoEQ] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	float N = (float)(imageHeight * imageWidth);
	int histo[256], sum[256];

	for (int d = 0; d < depth; d ++) {
		int acc_histo = 0;
		for (int i = 0; i < 256; i++) histo[i] = 0;
		std::cout << " >[PixelHistoEQ] histo initialization done" << std::endl;

		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				histo[input_img[y][depth * x + d]] += 1;
			}
		}
		std::cout << " >[PixelHistoEQ] histo calculation done" << std::endl;

		for (int i = 0; i < 256; i++) {
			acc_histo += histo[i];
			sum[i] = acc_histo;
		}
		std::cout << " >[PixelHistoEQ] sum calculation done" << std::endl;

		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				output_img[y][depth * x + d] = (unsigned char)(sum[input_img[y][depth * x + d]] / N * 255);
			}
		}
	}
	std::cout << " >[PixelHistoEQ] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::PixelContrast() {
	std::cout << "[pDoc] PixelContrast" << std::endl;
	if (input_img == NULL) Load1Image();
	std::cout << " >[PixelContrast] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
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
	std::cout << " >[PixelContrast] max: " << max << ", min: " << min << std::endl;

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
	std::cout << " >[PixelContrast] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::PixelBinarization(int threshold) {
	std::cout << "[pDoc] PixelBinarization" << std::endl;
	if (input_img == NULL) Load1Image();
	std::cout << " >[PixelBinarization] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
	std::cout << " >[PixelBinarization] threshold: " << threshold << std::endl;

	if (depth == 1) {
		std::cout << " >[PixelBinarization] Depth 1" << std::endl;
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				if (input_img[y][x] > threshold) output_img[y][x] = 255;
				else output_img[y][x] = 0;
			}
		}
	}
	else if (depth == 3) {
		std::cout << " >[PixelBinarization] Depth 3" << std::endl;
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
	std::cout << " >[PixelBinarization] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::PixelInvert() {
	std::cout << "[pDoc] PixelInvert" << std::endl;
	if (input_img == NULL) Load1Image();
	std::cout << " >[PixelInvert] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth * depth; x++) {
			output_img[y][x] = 255 - input_img[y][x];
		}
	}
	std::cout << " >[PixelInvert] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::PixelAdd2Images() {
	std::cout << "[pDoc] PixelAdd2Images" << std::endl;
	int value = 0;
	Load2Images();
	std::cout << " >[PixelAdd2Images] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	std::cout << " >[PixelAdd2Images] Calculating" << std::endl;
	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth * depth; x++) {
			value = input_img[y][x] + compare_img[y][x];
			if (value > 255) output_img[y][x] = 255;
			else output_img[y][x] = value;
		}
	}
	std::cout << " >[PixelAdd2Images] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::PixelSub2Images() {
	std::cout << "[pDoc] PixelSub2Images" << std::endl;
	int value = 0;
	Load2Images();
	std::cout << " >[PixelSub2Images] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	std::cout << " >[PixelSub2Images] Calculating" << std::endl;
	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth * depth; x++) {
			value = input_img[y][x] - compare_img[y][x];
			if (value < 0) output_img[y][x] = 0;
			else output_img[y][x] = value;
		}
	}
	std::cout << " >[PixelSub2Images] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::PixelDetection2Images() {
	std::cout << "[pDoc] PixelDetection2Images" << std::endl;
	int value = 0;
	Load2Images();
	std::cout << " >[PixelDetection2Images] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	std::cout << " >[PixelDetection2Images] Calculating" << std::endl;
	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth * depth; x++) {
			value = abs(input_img[y][x] - compare_img[y][x]);
			if (value < 64) output_img[y][x] = 0;
			else output_img[y][x] = 255;
		}
	}
	std::cout << " >[PixelDetection2Images] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::Convolve(unsigned char** input_img, unsigned char** output_img, int cols, int rows, float mask[][3], int bias, int depth) {
	std::cout << "[pDoc] Convolve" << std::endl;

	std::cout << " >[Convolve] malloc temp_img" << std::endl;
	unsigned char** temp_img = (unsigned char**)malloc((imageHeight + 2) * sizeof(unsigned char*));

	std::cout << " >[Convolve] calloc temp_img[i]" << std::endl;
	for (int i = 0; i < imageHeight + 2; i++)
		temp_img[i] = (unsigned char*)calloc(depth, (imageWidth + 2));

	std::cout << " >[Convolve] Copying input_img to temp_img" << std::endl;
	for (int y = 1; y < imageHeight - 1; y++)
	{
		for (int x = 1; x < imageWidth - 1; x++)
		{
			if (depth == 1) temp_img[y][x] = input_img[y - 1][x - 1];
			else if (depth == 3) {
				temp_img[y][3 * x] = input_img[y - 1][3 * (x - 1)];
				temp_img[y][3 * x + 1] = input_img[y - 1][3 * (x - 1) + 1];
				temp_img[y][3 * x + 2] = input_img[y - 1][3 * (x - 1) + 2];
			}
		}
	}

	std::cout << " >[Convolve] Convolution..." << std::endl;
	if (depth == 1) {
		std::cout << " >[Convolve] Depth 1" << std::endl;
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
		std::cout << " >[Convolve] Depth 3" << std::endl;
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

	std::cout << " >[Convolve] releasing temp_img memory..." << std::endl;
	for(int i = 0; i < imageHeight + 2; i++) 
	{
		free(temp_img[i]);
	}
	free(temp_img);
	
	std::cout << " >[Convolve] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::RegionSharpening() {
	std::cout << "[pDoc] PixelRegionSharpening" << std::endl;
	if (input_img == NULL) Load1Image();
	std::cout << " >[PixelRegionSharpening] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
	float kernel[3][3] = {
		{0, -1, 0},
		{-1, 5, -1},
		{0, -1, 0}
	};
	if (depth == 1) {
		std::cout << " >[PixelRegionSharpening] Depth 1" << std::endl;
		Convolve(input_img, output_img, imageWidth, imageHeight, kernel, 0, depth);
	}
	else if (depth == 3) {
		std::cout << " >[PixelRegionSharpening] Depth 3" << std::endl;
		unsigned char** R = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);
		unsigned char** G = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);
		unsigned char** B = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);

		for (int i = 0; i < imageHeight; i++) {
			R[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth);
			G[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth);
			B[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth);
		}

		std::cout << " >[PixelRegionSharpening] Copying input_img to R, G, B" << std::endl;
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				R[y][x] = input_img[y][3 * x];
				G[y][x] = input_img[y][3 * x + 1];
				B[y][x] = input_img[y][3 * x + 2];
			}
		}

		std::cout << " >[PixelRegionSharpening] Convolve R, G, B with kernel..." << std::endl;
		Convolve(R, R, imageWidth, imageHeight, kernel, 0, 1);
		Convolve(G, G, imageWidth, imageHeight, kernel, 0, 1);
		Convolve(B, B, imageWidth, imageHeight, kernel, 0, 1);
		
		std::cout << " >[PixelRegionSharpening] Copying R, G, B to output_img" << std::endl;
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				output_img[y][3 * x] = R[y][x];
				output_img[y][3 * x + 1] = G[y][x];
				output_img[y][3 * x + 2] = B[y][x];
			}
		}

		std::cout << " >[PixelRegionSharpening] releasing R, G, B memory..." << std::endl;
		for (int i = 0; i < imageHeight; i++) {
			free(R[i]);
			free(G[i]);
			free(B[i]);
		}
		free(R);
		free(G);
		free(B);
	}
	std::cout << " >[PixelRegionSharpening] Done" << std::endl;
}


void CImageProGyuTaeAhnDoc::RegionBlurring() {
	std::cout << "[pDoc] PixelRegionBlurring" << std::endl;
	if (input_img == NULL) Load1Image();
	std::cout << " >[PixelRegionBlurring] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
	float kernel[3][3] = {
		{1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
		{1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
		{1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0}
	};
	
	if (depth == 1) {
		std::cout << " >[PixelRegionBlurring] Depth 1" << std::endl;
		Convolve(input_img, output_img, imageWidth, imageHeight, kernel, 0, depth);
	}
	else if (depth == 3) {
		std::cout << " >[PixelRegionBlurring] Depth 3" << std::endl;
		unsigned char** R = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);
		unsigned char** G = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);
		unsigned char** B = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);

		for (int i = 0; i < imageHeight; i++) {
			R[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth);
			G[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth);
			B[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth);
		}

		std::cout << " >[PixelRegionBlurring] Copying input_img to R, G, B" << std::endl;
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				R[y][x] = input_img[y][3 * x];
				G[y][x] = input_img[y][3 * x + 1];
				B[y][x] = input_img[y][3 * x + 2];
			}
		}

		std::cout << " >[PixelRegionBlurring] Convolve R, G, B with kernel..." << std::endl;
		Convolve(R, R, imageWidth, imageHeight, kernel, 0, 1);
		Convolve(G, G, imageWidth, imageHeight, kernel, 0, 1);
		Convolve(B, B, imageWidth, imageHeight, kernel, 0, 1);

		std::cout << " >[PixelRegionBlurring] Copying R, G, B to output_img" << std::endl;
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				output_img[y][3 * x] = R[y][x];
				output_img[y][3 * x + 1] = G[y][x];
				output_img[y][3 * x + 2] = B[y][x];
			}
		}

		std::cout << " >[PixelRegionBlurring] releasing R, G, B memory..." << std::endl;
		for (int i = 0; i < imageHeight; i++) {
			free(R[i]);
			free(G[i]);
			free(B[i]);
		}
		free(R);
		free(G);
		free(B);
	}
	std::cout << " >[PixelRegionBlurring] Done" << std::endl;
}

void CImageProGyuTaeAhnDoc::RegionSobel() {
	std::cout << "[pDoc] PixelRegionSobel" << std::endl;
	if (input_img == NULL) Load1Image();
	std::cout << " >[PixelRegionSobel] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
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
	 
	std::cout << " >[PixelRegionSobel] malloc Er, Ec" << std::endl;
	unsigned char** Er = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);
	unsigned char** Ec = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);

	for (int i = 0; i < imageHeight; i++) {
		Er[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth);
		Ec[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth);
	}

	std::cout << " >[PixelRegionSobel] Convolve Er, Ec" << std::endl;
	Convolve(input_img, Er, imageWidth, imageHeight, mask1, 0, depth);
	Convolve(input_img, Ec, imageWidth, imageHeight, mask2, 0, depth);

	std::cout << " >[PixelRegionSobel] Calculating output_img" << std::endl;
	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth; x++) {
			output_img[y][x] = (int)sqrt((float)(Er[y][x] * Er[y][x] + Ec[y][x] * Ec[y][x]));
			if (output_img[y][x] > 255) output_img[y][x] = 255;
			else if (output_img[y][x] < 0) output_img[y][x] = 0;
		}
	}
	std::cout << " >[PixelRegionSobel] Done" << std::endl;
}//PixelRegionSobel

void CImageProGyuTaeAhnDoc::RegionPrewitt() {
	std::cout << "[pDoc] PixelRegionPrewitt" << std::endl;
	if (input_img == NULL) Load1Image();
	std::cout << " >[PixelRegionPrewitt] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
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

	std::cout << " >[PixelRegionPrewitt] malloc Er, Ec" << std::endl;
	unsigned char** Er = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);
	unsigned char** Ec = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);

	for (int i = 0; i < imageHeight; i++) {
		Er[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth);
		Ec[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth);
	}

	std::cout << " >[PixelRegionPrewitt] Convolve Er, Ec" << std::endl;
	Convolve(input_img, Er, imageWidth, imageHeight, mask1, 0, depth);
	Convolve(input_img, Ec, imageWidth, imageHeight, mask2, 0, depth);

	std::cout << " >[PixelRegionPrewitt] Calculating output_img" << std::endl;
	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth; x++) {
			output_img[y][x] = (int)sqrt((float)(Er[y][x] * Er[y][x] + Ec[y][x] * Ec[y][x]));
			if (output_img[y][x] > 255) output_img[y][x] = 255;
			else if (output_img[y][x] < 0) output_img[y][x] = 0;
		}
	}
	std::cout << " >[PixelRegionPrewitt] Done" << std::endl;
}//PixelRegionPrewitt

void CImageProGyuTaeAhnDoc::RegionRobert() {
	std::cout << "[pDoc] PixelRegionRobert" << std::endl;
	if (input_img == NULL) Load1Image();
	std::cout << " >[PixelRegionRobert] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
	float mask1[3][3] = {
		{1, 0, 0},
		{0, -1, 0},
		{0, 0, 0}
	};
	float mask2[3][3] = {
		{0, 1, 0},
		{-1, 0, 0},
		{0, 0, 0}
	};

	std::cout << " >[PixelRegionRobert] malloc Er, Ec" << std::endl;
	unsigned char** Er = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);
	unsigned char** Ec = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);

	for (int i = 0; i < imageHeight; i++) {
		Er[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth);
		Ec[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth);
	}

	std::cout << " >[PixelRegionRobert] Convolve Er, Ec" << std::endl;
	Convolve(input_img, Er, imageWidth, imageHeight, mask1, 0, depth);
	Convolve(input_img, Ec, imageWidth, imageHeight, mask2, 0, depth);

	std::cout << " >[PixelRegionRobert] Calculating output_img" << std::endl;
	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth; x++) {
			output_img[y][x] = (int)sqrt((float)(Er[y][x] * Er[y][x] + Ec[y][x] * Ec[y][x]));
			if (output_img[y][x] > 255) output_img[y][x] = 255;
			else if (output_img[y][x] < 0) output_img[y][x] = 0;
		}
	}
	std::cout << " >[PixelRegionRobert] Done" << std::endl;
}//PixelRegionRobert

void CImageProGyuTaeAhnDoc::RegionMedianFilter() {
	std::cout << "[pDoc] PixelRegionMedianFilter" << std::endl;
	if (input_img == NULL) Load1Image();
	std::cout << " >[PixelRegionMedianFilter] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	if (depth == 1) {
		std::cout << " >[PixelRegionMedianFilter] Depth 1" << std::endl;
		int mask[9];

		std::cout << " >[PixelRegionMedianFilter] Calculating output_img" << std::endl;
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
		std::cout << " >[PixelRegionMedianFilter] Depth 3" << std::endl;
		if (input_img == NULL) Load1Image();
		std::cout << " >[PixelRegionMedianFilter] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
		int mask_r[9];
		int mask_g[9];
		int mask_b[9];

		std::cout << " >[PixelRegionMedianFilter] Calculating output_img" << std::endl;
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
	std::cout << " >[PixelRegionMedianFilter] Done" << std::endl;
}//PixelRegionMedianFilter

void CImageProGyuTaeAhnDoc::RegionEmbossing() {
	std::cout << "[pDoc] PixelRegionEmbossing" << std::endl;

	if (input_img == NULL) Load1Image();
	std::cout << " >[PixelRegionEmbossing] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	float mask[3][3] = {
		{-1, -1, 0},
		{-1, 0, 1},
		{0, 1, 1}
	};

	if(depth == 1) {
		std::cout << " >[PixelRegionEmbossing] Depth 1" << std::endl;
		Convolve(input_img, output_img, imageWidth, imageHeight, mask, 128, depth);
	}
	else if (depth == 3) {
		std::cout << " >[PixelRegionEmbossing] Depth 3" << std::endl;
		unsigned char** R = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);
		unsigned char** G = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);
		unsigned char** B = (unsigned char**)malloc(sizeof(unsigned char*) * imageHeight);

		for (int i = 0; i < imageHeight; i++) {
			R[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth);
			G[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth);
			B[i] = (unsigned char*)malloc(sizeof(unsigned char) * imageWidth);
		}

		std::cout << " >[PixelRegionEmbossing] Copying input_img to R, G, B" << std::endl;
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				R[y][x] = input_img[y][3 * x];
				G[y][x] = input_img[y][3 * x + 1];
				B[y][x] = input_img[y][3 * x + 2];
			}
		}

		std::cout << " >[PixelRegionEmbossing] Convolve R, G, B with kernel..." << std::endl;
		Convolve(R, R, imageWidth, imageHeight, mask, 128, 1);
		Convolve(G, G, imageWidth, imageHeight, mask, 128, 1);
		Convolve(B, B, imageWidth, imageHeight, mask, 128, 1);

		std::cout << " >[PixelRegionEmbossing] Copying R, G, B to output_img" << std::endl;
		for (int y = 0; y < imageHeight; y++) {
			for (int x = 0; x < imageWidth; x++) {
				output_img[y][3 * x] = R[y][x];
				output_img[y][3 * x + 1] = G[y][x];
				output_img[y][3 * x + 2] = B[y][x];
			}
		}

		std::cout << " >[PixelRegionEmbossing] releasing R, G, B memory..." << std::endl;
		for (int i = 0; i < imageHeight; i++) {
			free(R[i]);
			free(G[i]);
			free(B[i]);
		}
		free(R);
		free(G);
		free(B);
	}

	std::cout << " >[PixelRegionEmbossing] Done" << std::endl;
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
	std::cout << "[pDoc] Erosion" << std::endl;
	if (input_img == NULL) Load1Image();
	
	std::cout << " >[Erosion] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	int min;

	std::cout << " >[Erosion] malloc output_img" << std::endl;
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
	std::cout << " >[Erosion] Done" << std::endl;
}//Erosion

void CImageProGyuTaeAhnDoc::Max_Value_Filter(unsigned char background) {
	std::cout << "[pDoc] Dilation" << std::endl;
	if (input_img == NULL) Load1Image();
	
	std::cout << " >[Dilation] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;
	
	int max;

	std::cout << " >[Dilation] malloc output_img" << std::endl;
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
	std::cout << " >[Dilation] Done" << std::endl;
}//Dilation

void CImageProGyuTaeAhnDoc::Opening(unsigned char background) {
	std::cout << "[pDoc] Opening" << std::endl;
	if (input_img == NULL) Load1Image();
	
	std::cout << " >[Opening] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

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
	std::cout << "[pDoc] Closing" << std::endl;
	if (input_img == NULL) Load1Image();
	
	std::cout << " >[Closing] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

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
	std::cout << "[pDoc] CountCell" << std::endl;
	Load1Image();
	
	std::cout << " >[CountCell] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	char buf[256];
	int label_no = 0;

	std::cout << " >[CountCell] Pixel Binarization..." << std::endl;
	PixelBinarization(100);
	CopyResultToInput();

	std::cout << " >[CountCell] Pixel Invert..." << std::endl;
	PixelInvert();
	CopyResultToInput();

	std::cout << " >[CountCell] Opening..." << std::endl;
	Opening(0);

	std::cout << " >[CountCell] malloc label" << std::endl;
	int x, y;
	int** label = (int**)malloc(imageHeight * sizeof(int));
	for (y = 0; y < imageHeight; y++) {
		label[y] = (int*)malloc(imageWidth * sizeof(int));
	}

	std::cout << " >[CountCell] initialize label" << std::endl;
	for (y = 0; y < imageHeight; y++) {
		for (x = 0; x < imageWidth; x++) {
			label[y][x] = 0;
		}
	}

	std::cout << " >[CountCell] Counting..." << std::endl;
	for (y = 0; y < imageHeight; y++) {
		for (x = 0; x < imageWidth; x++) {
			if (output_img[y][x] == 255 && label[y][x] == 0) {
				label_no = label_no + 1;
				Find_Connected_Region(y, x, label, label_no);
			}
		}
	}

	sprintf_s(buf, "Number of cells: %d", label_no);
	std::cout << " >[CountCell] " << buf << std::endl;
	AfxMessageBox(buf);
	std::cout << " >[CountCell] Done" << std::endl;
}//CountCell

void CImageProGyuTaeAhnDoc::GeometryZoominPixelCopy() {
	std::cout << "[pDoc] GeometryZoominPixelCopy" << std::endl;
	if (input_img == NULL) Load1Image();
	
	std::cout << " >[GeometryZoominPixelCopy] imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", depth: " << depth << std::endl;

	gImageWidth = imageWidth * 3;
	gImageHeight = imageHeight * 3;

	std::cout << " >[GeometryZoominPixelCopy] malloc gOutput_img" << std::endl;
	gOutput_img = (unsigned char**)malloc(sizeof(unsigned char*) * gImageHeight);

	for (int i = 0; i < gImageHeight; i++) {
		gOutput_img[i] = (unsigned char*)malloc(gImageWidth * depth);
	}
	
	std::cout << " >[GeometryZoominPixelCopy] Copying input_img to gOutput_img" << std::endl;
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
	std::cout << " >[GeometryZoominPixelCopy] Done" << std::endl;
}