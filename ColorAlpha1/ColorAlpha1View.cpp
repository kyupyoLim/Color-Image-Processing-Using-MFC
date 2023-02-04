
// ColorAlpha1View.cpp: CColorAlpha1View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ColorAlpha1.h"
#endif

#include "ColorAlpha1Doc.h"
#include "ColorAlpha1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CColorAlpha1View

IMPLEMENT_DYNCREATE(CColorAlpha1View, CView)

BEGIN_MESSAGE_MAP(CColorAlpha1View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(IDM_EQUAL_IMAGE, &CColorAlpha1View::OnEqualImage)
	ON_COMMAND(IDM_GRAY_SCALE, &CColorAlpha1View::OnGrayScale)
	ON_COMMAND(IDM_ADD_IMAGE, &CColorAlpha1View::OnAddImage)
	ON_COMMAND(IDM_SUB_IMAGE, &CColorAlpha1View::OnSubImage)
	ON_COMMAND(IDM_MUL_IMAGE, &CColorAlpha1View::OnMulImage)
	ON_COMMAND(IDM_DIV_IMAGE, &CColorAlpha1View::OnDivImage)
	ON_COMMAND(IDM_PARA_C_A_P_IMAGE, &CColorAlpha1View::OnParaCAPImage)
	ON_COMMAND(IDM_PARA_C_U_P_IMAGE, &CColorAlpha1View::OnParaCUPImage)
	ON_COMMAND(IDM_GAMMA_IMAGE, &CColorAlpha1View::OnGammaImage)
	ON_COMMAND(IDM_REVERSE_IMAGE, &CColorAlpha1View::OnReverseImage)
	ON_COMMAND(IDM_LEFT_RIGHT_MIRROR_IMAGE, &CColorAlpha1View::OnLeftRightMirrorImage)
	ON_COMMAND(IDM_UP_DOWN_MIRROR_IMAGE, &CColorAlpha1View::OnUpDownMirrorImage)
	ON_COMMAND(IDM_UP_DOWN_LEFT_RIGHT_MIRROR_IMAGE, &CColorAlpha1View::OnUpDownLeftRightMirrorImage)
	ON_COMMAND(IDM_ROTATE_IMAGE1, &CColorAlpha1View::OnRotateImage1)
	ON_COMMAND(IDM_ROTATE_IMAGE2, &CColorAlpha1View::OnRotateImage2)
	ON_COMMAND(ID_ZOOM_IN_IMAGE1, &CColorAlpha1View::OnZoomInImage1)
	ON_COMMAND(ID_ZOOM_IN_IMAGE2, &CColorAlpha1View::OnZoomInImage2)
	ON_COMMAND(IDM_ZOOM_OUT_IMAGE, &CColorAlpha1View::OnZoomOutImage)
	ON_COMMAND(IDM_MOVE_IMAGE, &CColorAlpha1View::OnMoveImage)
	ON_COMMAND(IDM_HISTO_STRETCH, &CColorAlpha1View::OnHistoStretch)
	ON_COMMAND(IDM_END_IN_SEARCH, &CColorAlpha1View::OnEndInSearch)
	ON_COMMAND(IDM_HISTO_EQUAL, &CColorAlpha1View::OnHistoEqual)
	ON_COMMAND(IDM_BW_IMAGE_INPUT, &CColorAlpha1View::OnBwImageInput)
	ON_COMMAND(IDM_BW_IMAGE, &CColorAlpha1View::OnBwImage)
	ON_COMMAND(IDM_BW_AVG_IMAGE, &CColorAlpha1View::OnBwAvgImage)
	ON_COMMAND(IDM_MID_VALUE_BW_IMAGE, &CColorAlpha1View::OnMidValueBwImage)
	ON_COMMAND(IDM_CHANGE_SATUR, &CColorAlpha1View::OnChangeSatur)
	ON_COMMAND(IDM_PICK_ORANGE, &CColorAlpha1View::OnPickOrange)
	ON_COMMAND(IDM_EMBOSS, &CColorAlpha1View::OnEmboss)
	ON_COMMAND(IDM_EMBOSS_HSI, &CColorAlpha1View::OnEmbossHsi)
	ON_COMMAND(IDM_BLURR_3X3, &CColorAlpha1View::OnBlurr3x3)
	ON_COMMAND(IDM_BLURR_5X5, &CColorAlpha1View::OnBlurr5x5)
	ON_COMMAND(IDM_GAUSSIAN_FILTER, &CColorAlpha1View::OnGaussianFilter)
	ON_COMMAND(IDM_SHARPENING1, &CColorAlpha1View::OnSharpening1)
	ON_COMMAND(IDM_SHARPENING2, &CColorAlpha1View::OnSharpening2)
	ON_COMMAND(IDM_HPF_SHARP, &CColorAlpha1View::OnHpfSharp)
	ON_COMMAND(IDM_PER_EDGE, &CColorAlpha1View::OnPerEdge)
	ON_COMMAND(IDM_HOR_EDGE, &CColorAlpha1View::OnHorEdge)
	ON_COMMAND(IDM_PER_HOR_EDGE, &CColorAlpha1View::OnPerHorEdge)
	ON_COMMAND(IDM_ROBERTS_IMAGE, &CColorAlpha1View::OnRobertsImage)
	ON_COMMAND(IDM_PREWITT_IMAGE, &CColorAlpha1View::OnPrewittImage)
	ON_COMMAND(IDM_SOBEL_IMAGE, &CColorAlpha1View::OnSobelImage)
	ON_COMMAND(IDM_LAPLACIAN1, &CColorAlpha1View::OnLaplacian1)
	ON_COMMAND(IDM_LAPLACIAN2, &CColorAlpha1View::OnLaplacian2)
	ON_COMMAND(IDM_LAPLACIAN3, &CColorAlpha1View::OnLaplacian3)
	ON_COMMAND(IDM_LOG_IMAGE, &CColorAlpha1View::OnLogImage)
	ON_COMMAND(IDM_DOG_IMAGE_7X7, &CColorAlpha1View::OnDogImage7x7)
	ON_COMMAND(IDM_DOG_IMAGE_9X9, &CColorAlpha1View::OnDogImage9x9)
	ON_COMMAND(IDM_BLURR_3X3_NO_HSI, &CColorAlpha1View::OnBlurr3x3NoHsi)
	ON_COMMAND(IDM_BLURR_5X5_NO_HSI, &CColorAlpha1View::OnBlurr5x5NoHsi)
	ON_COMMAND(IDM_GAUSSIAN_FILTER_NO_HSI, &CColorAlpha1View::OnGaussianFilterNoHsi)
	ON_COMMAND(IDM_SHARPENING1_NO_HSI, &CColorAlpha1View::OnSharpening1NoHsi)
	ON_COMMAND(IDM_SHARPENING2_NO_HSI, &CColorAlpha1View::OnSharpening2NoHsi)
	ON_COMMAND(IDM_HPF_SHARP_NO_HSI, &CColorAlpha1View::OnHpfSharpNoHsi)
	ON_COMMAND(IDM_PER_EDGE_NO_HSI, &CColorAlpha1View::OnPerEdgeNoHsi)
	ON_COMMAND(IDM_HOR_EDGE_NO_HSI, &CColorAlpha1View::OnHorEdgeNoHsi)
	ON_COMMAND(IDM_PER_HOR_EDGE_NO_HSI, &CColorAlpha1View::OnPerHorEdgeNoHsi)
	ON_COMMAND(ID_M_ROBERTS_IMAGE_NO_HSI, &CColorAlpha1View::OnMRobertsImageNoHsi)
	ON_COMMAND(IDM_PREWITT_IMAGE_NO_HSI, &CColorAlpha1View::OnPrewittImageNoHsi)
	ON_COMMAND(IDM_SOBEL_IMAGE_NO_HSI, &CColorAlpha1View::OnSobelImageNoHsi)
	ON_COMMAND(IDM_LAPLACIAN1_NO_HSI, &CColorAlpha1View::OnLaplacian1NoHsi)
	ON_COMMAND(IDM_LAPLACIAN2_NO_HSI, &CColorAlpha1View::OnLaplacian2NoHsi)
	ON_COMMAND(IDM_LAPLACIAN3_NO_HSI, &CColorAlpha1View::OnLaplacian3NoHsi)
	ON_COMMAND(IDM_LOG_IMAGE_NO_HSI, &CColorAlpha1View::OnLogImageNoHsi)
	ON_COMMAND(IDM_DOG_IMAGE_7X7_NO_HSI, &CColorAlpha1View::OnDogImage7x7NoHsi)
	ON_COMMAND(IDM_DOG_IMAGE_9X9_NO_HSI, &CColorAlpha1View::OnDogImage9x9NoHsi)
	ON_COMMAND(IDM_ZOOM_IN_IMAGE3, &CColorAlpha1View::OnZoomInImage3)
	ON_COMMAND(IDM_ZOOM_IN_IMAGE4, &CColorAlpha1View::OnZoomInImage4)
END_MESSAGE_MAP()

// CColorAlpha1View 생성/소멸

CColorAlpha1View::CColorAlpha1View() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CColorAlpha1View::~CColorAlpha1View()
{
}

BOOL CColorAlpha1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CColorAlpha1View 그리기

void CColorAlpha1View::OnDraw(CDC* pDC)
{
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	//int R,G,B;
	///*
	//for (int i = 0; i < outH; i++) {
	//	for (int k = 0; k < outW; k++) {
	//		int px = outImage[i][k];
	//		SetPixel(hdc, k + 50, i + 250, RGB(px, px, px));
	//	}
	//}
	//*/
	//for (int i = 0; i < pDoc->m_inH; i++) {
	//	for (int k = 0; k < pDoc->m_inW; k++) {
	//		R = pDoc->m_inImageR[i][k];
	//		G = pDoc->m_inImageG[i][k];
	//		B = pDoc->m_inImageB[i][k];
	//		pDC->SetPixel(k + 5, i + 5, RGB(R, G, B));
	//	}
	//}
	//for (int i = 0; i < pDoc->m_outH; i++) {
	//	for (int k = 0; k < pDoc->m_outW; k++) {
	//		R = pDoc->m_outImageR[i][k];
	//		G = pDoc->m_outImageG[i][k];
	//		B = pDoc->m_outImageB[i][k];
	//		pDC->SetPixel(pDoc->m_inW + k + 5 + 5, i + 5, RGB(R, G, B));
	//	}
	//}

	
	
	/////////////////////
	/// 성능 개선을 위한 더블 버퍼링 
	////////////////////
	int i, k;
	unsigned char R, G, B;
	// 메모리 DC 선언
	CDC memDC;
	CBitmap* pOldBitmap, bitmap;

	// 화면 DC와 호환되는 메모리 DC 객체를 생성
	memDC.CreateCompatibleDC(pDC);

	// 마찬가지로 화면 DC와 호환되는 Bitmap 생성
	bitmap.CreateCompatibleBitmap(pDC, pDoc->m_inW, pDoc->m_inH);

	pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, pDoc->m_inW, pDoc->m_inH, WHITENESS); // 흰색으로 초기화

	// 출력 영상크기 자동 조절
	double MAXSIZE = 800;
	int iW = pDoc->m_inW;
	int iH = pDoc->m_inH;
	double hop = 1.0;

	if (iH > MAXSIZE || iW > MAXSIZE)
	{
		if (pDoc->m_inW > pDoc->m_inH)
			hop = (pDoc->m_inW / MAXSIZE);
		else
			hop = (pDoc->m_inH / MAXSIZE);

		iW = (int)(pDoc->m_inW / hop);
		iH = (int)(pDoc->m_inH / hop);
	}
	// 메모리 DC에 그리기
	for (i = 0; i < iH; i++) {
		for (k = 0; k < iW; k++) {
			R = pDoc->m_inImageR[(int)(i * hop)][(int)(k * hop)];
			G = pDoc->m_inImageG[(int)(i * hop)][(int)(k * hop)];
			B = pDoc->m_inImageB[(int)(i * hop)][(int)(k * hop)];
			memDC.SetPixel(k, i, RGB(R, G, B));
		}
	}
	// 메모리 DC를 화면 DC에 고속 복사
	pDC->BitBlt(5, 5, pDoc->m_inW, pDoc->m_inH, &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();

	///////////////////

	// 화면 DC와 호환되는 메모리 DC 객체를 생성
	memDC.CreateCompatibleDC(pDC);

	// 마찬가지로 화면 DC와 호환되는 Bitmap 생성
	bitmap.CreateCompatibleBitmap(pDC, pDoc->m_outW, pDoc->m_outH);

	pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, pDoc->m_outW, pDoc->m_outH, WHITENESS); // 흰색으로 초기화

	// 메모리 DC에 그리기
	int oW = pDoc->m_outW;
	int oH = pDoc->m_outH;
	double hop2 = 1.0;

	if (oH > MAXSIZE || oW > MAXSIZE)
	{
		if (pDoc->m_outW > pDoc->m_outH)
			hop2 = (pDoc->m_outW / MAXSIZE);
		else
			hop2 = (pDoc->m_outH / MAXSIZE);

		oW = (int)(pDoc->m_outW / hop);
		oH = (int)(pDoc->m_outH / hop);
	}
	for (i = 0; i < oH; i++) {
		for (k = 0; k < oW; k++) {
			R = pDoc->m_outImageR[(int)(i * hop2)][(int)(k * hop2)];
			G = pDoc->m_outImageG[(int)(i * hop2)][(int)(k * hop2)];
			B = pDoc->m_outImageB[(int)(i * hop2)][(int)(k * hop2)];
			memDC.SetPixel(k, i, RGB(R, G, B));
		}
	}
	// 메모리 DC를 화면 DC에 고속 복사
	pDC->BitBlt(iW + 10, 5, pDoc->m_outW, pDoc->m_outH, &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}


// CColorAlpha1View 인쇄

BOOL CColorAlpha1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CColorAlpha1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CColorAlpha1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CColorAlpha1View 진단

#ifdef _DEBUG
void CColorAlpha1View::AssertValid() const
{
	CView::AssertValid();
}

void CColorAlpha1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CColorAlpha1Doc* CColorAlpha1View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CColorAlpha1Doc)));
	return (CColorAlpha1Doc*)m_pDocument;
}
#endif //_DEBUG


// CColorAlpha1View 메시지 처리기


void CColorAlpha1View::OnEqualImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnEqualImage();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnGrayScale()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnGrayScale();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnAddImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnAddImage();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnSubImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnSubImage();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnMulImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnMulImage();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnDivImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnDivImage();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnParaCAPImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnParaCAPImage();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnParaCUPImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnParaCUPImage();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnGammaImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnGammaImage();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnReverseImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnReverseImage();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnLeftRightMirrorImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnLeftRightMirrorImage();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnUpDownMirrorImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnUpDownMirrorImage();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnUpDownLeftRightMirrorImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnUpDownLeftRightMirrorImage();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnRotateImage1()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnRotateImage1();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnRotateImage2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnRotateImage2();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnZoomInImage1()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnZoomInImage1();
	Invalidate(TRUE);

}


void CColorAlpha1View::OnZoomInImage2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnZoomInImage2();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnZoomOutImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnZoomOutImage();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnMoveImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnMoveImage();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnHistoStretch()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnHistoStretch();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnEndInSearch()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnEndInSearch();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnHistoEqual()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnHistoEqual();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnBwImageInput()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnBwImageInput();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnBwImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnBwImage();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnBwAvgImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnBwAvgImage();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnMidValueBwImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnMidValueBwImage();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnChangeSatur()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnChangeSatur();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnPickOrange()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnPickOrange();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnEmboss()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnEmboss();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnEmbossHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnEmbossHsi();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnBlurr3x3()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnBlurr3x3();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnBlurr5x5()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnBlurr5x5();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnGaussianFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnGaussianFilter();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnSharpening1()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnSharpening1();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnSharpening2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnSharpening2();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnHpfSharp()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnHpfSharp();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnPerEdge()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnPerEdge();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnHorEdge()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnHorEdge();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnPerHorEdge()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnPerHorEdge();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnRobertsImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnRobertsImage();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnPrewittImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnPrewittImage();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnSobelImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnSobelImage();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnLaplacian1()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnLaplacian1();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnLaplacian2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnLaplacian2();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnLaplacian3()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnLaplacian3();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnLogImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnLogImage();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnDogImage7x7()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnDogImage7x7();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnDogImage9x9()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnDogImage9x9();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnBlurr3x3NoHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnBlurr3x3NoHsi();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnBlurr5x5NoHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnBlurr5x5NoHsi();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnGaussianFilterNoHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnGaussianFilterNoHsi();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnSharpening1NoHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnSharpening1NoHsi();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnSharpening2NoHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnSharpening2NoHsi();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnHpfSharpNoHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnHpfSharpNoHsi();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnPerEdgeNoHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnPerEdgeNoHsi();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnHorEdgeNoHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnHorEdgeNoHsi();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnPerHorEdgeNoHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnPerHorEdgeNoHsi();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnMRobertsImageNoHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnMRobertsImageNoHsi();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnPrewittImageNoHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnPrewittImageNoHsi();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnSobelImageNoHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnSobelImageNoHsi();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnLaplacian1NoHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnLaplacian1NoHsi();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnLaplacian2NoHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnLaplacian2NoHsi();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnLaplacian3NoHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnLaplacian3NoHsi();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnLogImageNoHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnLogImageNoHsi();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnDogImage7x7NoHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnDogImage7x7NoHsi();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnDogImage9x9NoHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnDogImage9x9NoHsi();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnZoomInImage3()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnZoomInImage3();
	Invalidate(TRUE);
}


void CColorAlpha1View::OnZoomInImage4()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorAlpha1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnZoomInImage4();
	Invalidate(TRUE);
}
