
// ColorAlpha1View.h: CColorAlpha1View 클래스의 인터페이스
//

#pragma once


class CColorAlpha1View : public CView
{
protected: // serialization에서만 만들어집니다.
	CColorAlpha1View() noexcept;
	DECLARE_DYNCREATE(CColorAlpha1View)

// 특성입니다.
public:
	CColorAlpha1Doc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CColorAlpha1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEqualImage();
	afx_msg void OnGrayScale();
	afx_msg void OnAddImage();
	afx_msg void OnSubImage();
	afx_msg void OnMulImage();
	afx_msg void OnDivImage();
	afx_msg void OnParaCAPImage();
	afx_msg void OnParaCUPImage();
	afx_msg void OnGammaImage();
	afx_msg void OnReverseImage();
	afx_msg void OnLeftRightMirrorImage();
	afx_msg void OnUpDownMirrorImage();
	afx_msg void OnUpDownLeftRightMirrorImage();
	afx_msg void OnRotateImage1();
	afx_msg void OnRotateImage2();
	afx_msg void OnZoomInImage1();
	afx_msg void OnZoomInImage2();
	afx_msg void OnZoomOutImage();
	afx_msg void OnMoveImage();
	afx_msg void OnHistoStretch();
	afx_msg void OnEndInSearch();
	afx_msg void OnHistoEqual();
	afx_msg void OnBwImageInput();
	afx_msg void OnBwImage();
	afx_msg void OnBwAvgImage();
	afx_msg void OnMidValueBwImage();
	afx_msg void OnChangeSatur();
	afx_msg void OnPickOrange();
	afx_msg void OnEmboss();
	afx_msg void OnEmbossHsi();
	afx_msg void OnBlurr3x3();
	afx_msg void OnBlurr5x5();
	afx_msg void OnGaussianFilter();
	afx_msg void OnSharpening1();
	afx_msg void OnSharpening2();
	afx_msg void OnHpfSharp();
	afx_msg void OnPerEdge();
	afx_msg void OnHorEdge();
	afx_msg void OnPerHorEdge();
	afx_msg void OnRobertsImage();
	afx_msg void OnPrewittImage();
	afx_msg void OnSobelImage();
	afx_msg void OnLaplacian1();
	afx_msg void OnLaplacian2();
	afx_msg void OnLaplacian3();
	afx_msg void OnLogImage();
	afx_msg void OnDogImage7x7();
	afx_msg void OnDogImage9x9();
	afx_msg void OnBlurr3x3NoHsi();
	afx_msg void OnBlurr5x5NoHsi();
	afx_msg void OnGaussianFilterNoHsi();
	afx_msg void OnSharpening1NoHsi();
	afx_msg void OnSharpening2NoHsi();
	afx_msg void OnHpfSharpNoHsi();
	afx_msg void OnPerEdgeNoHsi();
	afx_msg void OnHorEdgeNoHsi();
	afx_msg void OnPerHorEdgeNoHsi();
	afx_msg void OnMRobertsImageNoHsi();
	afx_msg void OnPrewittImageNoHsi();
	afx_msg void OnSobelImageNoHsi();
	afx_msg void OnLaplacian1NoHsi();
	afx_msg void OnLaplacian2NoHsi();
	afx_msg void OnLaplacian3NoHsi();
	afx_msg void OnLogImageNoHsi();
	afx_msg void OnDogImage7x7NoHsi();
	afx_msg void OnDogImage9x9NoHsi();
	afx_msg void OnZoomInImage3();
	afx_msg void OnZoomInImage4();
};

#ifndef _DEBUG  // ColorAlpha1View.cpp의 디버그 버전
inline CColorAlpha1Doc* CColorAlpha1View::GetDocument() const
   { return reinterpret_cast<CColorAlpha1Doc*>(m_pDocument); }
#endif

