
// ImagePro_GyuTae_AhnDoc.h : interface of the CImageProGyuTaeAhnDoc class
//


#pragma once


class CImageProGyuTaeAhnDoc : public CDocument
{
protected: // create from serialization only
	CImageProGyuTaeAhnDoc() noexcept;
	DECLARE_DYNCREATE(CImageProGyuTaeAhnDoc)

// Attributes
public:
	unsigned char** input_img;
	unsigned char** compare_img;
	unsigned char** output_img;
	int imageWidth, imageHeight, depth;

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CImageProGyuTaeAhnDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

public:
	//Image file I/O
	void Load1Image();
	void Load2Images();
	void LoadImageFile(CArchive& ar);
	void LoadSecondImageFile(CArchive& ar);
	void CheckLoadedImage(CArchive& ar);
	void SaveImageFile(CArchive& ar);
	//Image Pixel processing
	void PixelAdd();
	void PixelSub();
	void PixelMul();
	void PixelDiv();
	void PixelHistoEQ();
	void PixelContrast();
	void PixelBinarization();
	void PixelInvert();
	void PixelAdd2Images();
	void PixelSub2Images();
	void PixelDetection2Images();

	//Image Pixel Region processing
	void Convolve(unsigned char** input_img, unsigned char** output_img, int cols, int rows, float mask[][3], int bias, int depth);
	void RegionSharpening();
	void RegionBlurring();
	void RegionSobel();
	void RegionPrewitt();
	void RegionRobert();
	void RegionMedianFilter();
	void RegionEmbossing();

	//Image Morphology
	void CopyResultToInput();
	void Erosion();
	void Dilation();
	void Opening();
};
	
