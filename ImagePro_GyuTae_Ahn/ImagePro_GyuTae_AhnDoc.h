
// ImagePro_GyuTae_AhnDoc.h : interface of the CImageProGyuTaeAhnDoc class
//


#pragma once

typedef struct {
	double re;
	double im;
} complex_num;

typedef struct PCX_HEADER {
	char manufacturer;
	char version;
	char encoding;
	char bit_per_pixel;
	short int xmin, ymin;
	short int xmax, ymax;
	short int hres, vres;
	char palette16[48];
	char reserved;
	char color_planes;
	short int bytes_per_line;
	short int palette_type;
	char filler[58];
} pcx_header;


class CImageProGyuTaeAhnDoc : public CDocument
{
protected: // create from serialization only
	CImageProGyuTaeAhnDoc() noexcept;
	DECLARE_DYNCREATE(CImageProGyuTaeAhnDoc)

// Attributes
public:
	typedef struct {
		int Px;
		int Py;
		int Qx;
		int Qy;
	} control_line;

	unsigned char** input_img;
	unsigned char** compare_img;
	unsigned char** output_img;
	int imageWidth, imageHeight, depth;

	int gImageWidth, gImageHeight;
	unsigned char** gOutput_img;

	unsigned char** morphed_img[10];

	complex_num** fft_result;
	complex_num** ifft_result;

	bool console_output;

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
	void PixelBinarization(int threshold);
	void PixelInvert();
	void PixelAdd2Images();
	void PixelSub2Images();
	void PixelDetection2Images();

	//Image Pixel Region processing
	void Convolve(unsigned char** input_img, unsigned char** output_img, int cols, int rows, float mask[][3], int bias, int depth);
	void Convolve(unsigned char** input_img, unsigned char** output_img, int cols, int rows, float mask[][5], int bias, int depth);
	void RegionSharpening();
	void RegionBlurring();
	void RegionSobel();
	void RegionPrewitt();
	void RegionRobert();
	void RegionMedianFilter();
	void RegionEmbossing();

	//Image Morphology
	void CopyResultToInput();
	void Find_Connected_Region(int y, int x, int** label, int label_num);
	void Min_Value_Filter(unsigned char background);
	void Max_Value_Filter(unsigned char background);
	void Opening(unsigned char background);
	void Closing(unsigned char background);
	void CountCell();
	
	//Image Geometric Processing
	void ZoomInDialog(float zoom_in_ratio);
	void GeometryZoominPixelCopy();
	void GeometryZoominInterpolation();
	void GeometryZoomoutSubsampling();
	void GeometryZoomoutAverage();
	void GeometryRotate();
	void GeometryVerticalFlip();
	void GeometryHorizontalFlip();
	void GeometryWarping(control_line source_lines[], control_line dest_lines[], int control_line_num);
	void GeometryWarpingNormal();
	void GeometryMyImageWarping();
	void GeometryMyImageWarping_Smile();
	void GeometryMorphing(control_line source_lines[], control_line dest_lines[], int control_line_num);
	void GeometryMorphingNormal();
	void GeometryMorphingMyImg();

	//Frequency Domain Processing
	int Reverse_Bit_Order(int index, int log2N);
	void Shuffle_Data(complex_num x[], int N, int log2N);
	void Butterfly_Computation(complex_num x[], int N, int log2N, bool inverse);
	void FFT1D(complex_num x[], int N, int log2N, bool inverse);
	void FFT2D();
	void IFFT2D();
	void LowPassFilter();
	void HighPassFilter();
	void NoiseReduction();

	//Image Compression and Restoration
	void LoadPCX();
	void SavePCX();
	void LoadJPEG();
};