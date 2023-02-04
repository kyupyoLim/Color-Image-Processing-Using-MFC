
// ColorAlpha1Doc.h: CColorAlpha1Doc 클래스의 인터페이스
//


#pragma once


class CColorAlpha1Doc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CColorAlpha1Doc() noexcept;
	DECLARE_DYNCREATE(CColorAlpha1Doc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CColorAlpha1Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	unsigned char** m_inImageR = NULL;
	unsigned char** m_inImageG = NULL;
	unsigned char** m_inImageB = NULL;
	unsigned char** m_outImageR = NULL;
	unsigned char** m_outImageG = NULL;
	unsigned char** m_outImageB = NULL;
	int m_inH = 0;
	int m_inW = 0;
	int m_outH = 0;
	int m_outW = 0;
	unsigned char** malloc2D(int h, int w);
	void free2D(unsigned char** memory, int h);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	void OnEqualImage();
	void OnGrayScale();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	void OnAddImage();
	void OnSubImage();
	void OnMulImage();
	void OnDivImage();
	void OnParaCAPImage();
	void OnParaCUPImage();
	void OnGammaImage();
	void OnReverseImage();
	void OnLeftRightMirrorImage();
	void OnUpDownMirrorImage();
	void OnUpDownLeftRightMirrorImage();
	void OnRotateImage1();
	void OnRotateImage2();
	void OnZoomInImage1();
	void OnZoomInImage2();
	void OnZoomOutImage();
	void OnMoveImage();
	void OnHistoStretch();
	double** malloc2D_double(int h, int w);
	void free2D_double(double** memory, int h);
	void OnEndInSearch();
	void OnHistoEqual();
	void OnBwImageInput();
	void OnBwImage();
	void OnBwAvgImage();
	void OnMidValueBwImage();
	double* RGB2HSI(int R, int G, int B);
	unsigned char* HSI2RGB(double H, double S, double I);
	void OnChangeSatur();
	void OnPickOrange();
	void OnEmboss();
	void OnEmbossHsi();
	void OnBlurr3x3();
	void OnBlurr5x5();
	void OnGaussianFilter();
	void OnSharpening1();
	void OnSharpening2();
	void OnHpfSharp();
	void OnPerEdge();
	void OnHorEdge();
	void OnPerHorEdge();
	void OnRobertsImage();
	void OnPrewittImage();
	void OnSobelImage();
	void OnLaplacian1();
	void OnLaplacian2();
	void OnLaplacian3();
	void OnLogImage();
	void OnDogImage7x7();
	void OnDogImage9x9();
	void OnBlurr3x3NoHsi();
	void OnBlurr5x5NoHsi();
	void OnGaussianFilterNoHsi();
	void OnSharpening1NoHsi();
	void OnSharpening2NoHsi();
	void OnHpfSharpNoHsi();
	void OnPerEdgeNoHsi();
	void OnHorEdgeNoHsi();
	void OnPerHorEdgeNoHsi();
	void OnMRobertsImageNoHsi();
	void OnPrewittImageNoHsi();
	void OnSobelImageNoHsi();
	void OnLaplacian1NoHsi();
	void OnLaplacian2NoHsi();
	void OnLaplacian3NoHsi();
	void OnLogImageNoHsi();
	void OnDogImage7x7NoHsi();
	void OnDogImage9x9NoHsi();
	void OnZoomInImage3();
	void OnZoomInImage4();
};
