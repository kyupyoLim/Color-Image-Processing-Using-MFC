
// ColorAlpha1Doc.cpp: CColorAlpha1Doc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ColorAlpha1.h"
#endif

#include "ColorAlpha1Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CColorAlpha1Doc

IMPLEMENT_DYNCREATE(CColorAlpha1Doc, CDocument)

BEGIN_MESSAGE_MAP(CColorAlpha1Doc, CDocument)
END_MESSAGE_MAP()


// CColorAlpha1Doc 생성/소멸

CColorAlpha1Doc::CColorAlpha1Doc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CColorAlpha1Doc::~CColorAlpha1Doc()
{
}

BOOL CColorAlpha1Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CColorAlpha1Doc serialization

void CColorAlpha1Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CColorAlpha1Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
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

// 검색 처리기를 지원합니다.
void CColorAlpha1Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CColorAlpha1Doc::SetSearchContent(const CString& value)
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

// CColorAlpha1Doc 진단

#ifdef _DEBUG
void CColorAlpha1Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CColorAlpha1Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CColorAlpha1Doc 명령

///////////////////////////
///////공통 함수부/////////
///////////////////////////

unsigned char** CColorAlpha1Doc::malloc2D(int h, int w)
{
	// TODO: 여기에 구현 코드 추가.
	unsigned char** retMemory;
	//retMemory = (unsigned char**) malloc(h * sizeof(unsigned char*));
	//for (int i = 0; i < h; i++)
	//	retMemory[i] = (unsigned char*)malloc(w * sizeof(unsigned char));

	retMemory = new unsigned char* [h];//malloc은 함수라서 버그가 발생할 수 있음. new라는 연산자를 이용해서 메모리 할당
	for (int i = 0; i < h; i++) {
		retMemory[i] = new unsigned char[w];
	}
	return retMemory;
}


void CColorAlpha1Doc::free2D(unsigned char** memory, int h)
{
	// TODO: 여기에 구현 코드 추가.
	if (memory == NULL)
		return;
	/*for (int i = 0; i < h; i++)
		free(memory[i]);
	free(memory);
	memory = NULL;*/
	for (int i = 0; i < h; i++)
		delete(memory[i]);//함수 free = 연산자 delete
	delete(memory);
	memory = NULL;
}


BOOL CColorAlpha1Doc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CImage img;
	img.Load(lpszPathName);

	//중요! 영상 크기 알아내기

	m_inH = img.GetHeight();
	m_inW = img.GetWidth();

	//메모리 할당
	m_inImageR = malloc2D(m_inH, m_inW);
	m_inImageG = malloc2D(m_inH, m_inW);
	m_inImageB = malloc2D(m_inH, m_inW);
	
	COLORREF px;
	//CImage 객체의 값 --> 메모리
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			px = img.GetPixel(k, i);
			m_inImageR[i][k] = GetRValue(px);
			m_inImageG[i][k] = GetGValue(px);
			m_inImageB[i][k] = GetBValue(px);
		}
	}
	return TRUE;
}

BOOL CColorAlpha1Doc::OnSaveDocument(LPCTSTR lpszPathName)
{


	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (m_outImageR == NULL) {
		return FALSE;
	}
	CImage img;
	img.Create(m_outW, m_outH, 32);

	unsigned char R, G, B;
	COLORREF px;

	for (int i = 0; i < m_outW; i++)
		for (int k = 0; k < m_outH; k++) {
			R = m_outImageR[k][i];
			G = m_outImageG[k][i];
			B = m_outImageB[k][i];
			px = RGB(R, G, B);
			img.SetPixel(i, k, px);
		}

	img.Save(lpszPathName, Gdiplus::ImageFormatPNG);
	MessageBox(NULL, L"저장", L"성공", NULL);
	return TRUE;
}


void CColorAlpha1Doc::OnCloseDocument()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	free2D(m_inImageR, m_inH);
	free2D(m_inImageG, m_inH);
	free2D(m_inImageB, m_inH);

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	CDocument::OnCloseDocument();
}

///////////////////////////
///////화소점 처리/////////
///////////////////////////
void CColorAlpha1Doc::OnEqualImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL){
		return;
	}
		
	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);
	// 진짜! 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImageR[i][k] = m_inImageR[i][k];
			m_outImageG[i][k] = m_inImageG[i][k];
			m_outImageB[i][k] = m_inImageB[i][k];
		}
	}
}


void CColorAlpha1Doc::OnGrayScale()
{
	// TODO: 여기에 구현 코드 추가.
	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);
	// 진짜! 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			int avg = (int)((int)m_inImageR[i][k] + (int)m_inImageG[i][k] + (int)m_inImageB[i][k]) / 3.0;
			m_outImageR[i][k] = avg;
			m_outImageG[i][k] = avg;
			m_outImageB[i][k] = avg;
		}
	}
}


#include"CInput1Dlg.h"
void CColorAlpha1Doc::OnAddImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	CInput1Dlg dlg;
	if (dlg.DoModal() != IDOK) {//DoModal --> 뒤에 끊기고 화면 나오는 것 --> 즉 대화상자만 사용할 수 있음
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);
	// 진짜! 영상처리 알고리즘
	int value = (int)dlg.m_Input1;//double형이었으니 int로 casting

	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			//R
			if (m_inImageR[i][k] + value > 255){
				m_outImageR[i][k] = 255;
			}
			else {
				m_outImageR[i][k] = m_inImageR[i][k] + value;
			}
			//G
			if (m_inImageG[i][k] + value > 255) {
				m_outImageG[i][k] = 255;
			}
			else {
				m_outImageG[i][k] = m_inImageG[i][k] + value;
			}
			//B
			if (m_inImageB[i][k] + value > 255) {
				m_outImageB[i][k] = 255;
			}
			else {
				m_outImageB[i][k] = m_inImageB[i][k] + value;
			}
			
		}
	}
}


void CColorAlpha1Doc::OnSubImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	CInput1Dlg dlg;
	if (dlg.DoModal() != IDOK) {//DoModal --> 뒤에 끊기고 화면 나오는 것 --> 즉 대화상자만 사용할 수 있음
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);
	// 진짜! 영상처리 알고리즘
	int value = (int)dlg.m_Input1;//double형이었으니 int로 casting

	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			//R
			if (m_inImageR[i][k] - value < 0) {
				m_outImageR[i][k] = 0;
			}
			else {
				m_outImageR[i][k] = m_inImageR[i][k] - value;
			}
			//G
			if (m_inImageG[i][k] - value < 0) {
				m_outImageG[i][k] = 0;
			}
			else {
				m_outImageG[i][k] = m_inImageG[i][k] - value;
			}
			//B
			if (m_inImageB[i][k] - value < 0) {
				m_outImageB[i][k] = 0;
			}
			else {
				m_outImageB[i][k] = m_inImageB[i][k] - value;
			}

		}
	}
}


void CColorAlpha1Doc::OnMulImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	CInput1Dlg dlg;
	if (dlg.DoModal() != IDOK) {//DoModal --> 뒤에 끊기고 화면 나오는 것 --> 즉 대화상자만 사용할 수 있음
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);
	// 진짜! 영상처리 알고리즘
	float value = (float)dlg.m_Input1;//double형이었으니 float로 casting

	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			//R
			if (m_inImageR[i][k] * value > 255.0) {
				m_outImageR[i][k] = 255.0;
			}
			else {
				m_outImageR[i][k] = m_inImageR[i][k] * value;
			}
			//G
			if (m_inImageG[i][k] * value > 255.0) {
				m_outImageG[i][k] = 255.0;
			}
			else {
				m_outImageG[i][k] = m_inImageG[i][k] * value;
			}
			//B
			if (m_inImageB[i][k] * value > 255.0) {
				m_outImageB[i][k] = 255.0;
			}
			else {
				m_outImageB[i][k] = m_inImageB[i][k] * value;
			}

		}
	}
}


void CColorAlpha1Doc::OnDivImage()
{
	// TODO: 여기에 구현 코드 추가.

	if (m_inImageR == NULL) {
		return;
	}

	CInput1Dlg dlg;
	if (dlg.DoModal() != IDOK) {//DoModal --> 뒤에 끊기고 화면 나오는 것 --> 즉 대화상자만 사용할 수 있음
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);
	// 진짜! 영상처리 알고리즘
	float value = (float)dlg.m_Input1;//double형이었으니 float로 casting

	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			//R
			if (m_inImageR[i][k] / value < 0.0) {
				m_outImageR[i][k] = 0.0;
			}
			else {
				m_outImageR[i][k] = m_inImageR[i][k] / value;
			}
			//G
			if (m_inImageG[i][k] / value < 0.0) {
				m_outImageG[i][k] = 0.0;
			}
			else {
				m_outImageG[i][k] = m_inImageG[i][k] / value;
			}
			//B
			if (m_inImageB[i][k] / value < 0.0) {
				m_outImageB[i][k] = 0.0;
			}
			else {
				m_outImageB[i][k] = m_inImageB[i][k] / value;
			}

		}
	}
}

//lookup table기법(미리 계산 표를 만들기)
unsigned char LUT[256];


void CColorAlpha1Doc::OnParaCAPImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);
	// 진짜! 영상처리 알고리즘
	double valueR,valueG,valueB = 0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			valueR = 255.0 - 255.0 * pow((m_inImageR[i][k] / 128.0 - 1.0), 2);
			valueG = 255.0 - 255.0 * pow((m_inImageG[i][k] / 128.0 - 1.0), 2);
			valueB = 255.0 - 255.0 * pow((m_inImageB[i][k] / 128.0 - 1.0), 2);
			
			if (valueR > 255.0) {
				valueR = 255.0;
			}
			else if (valueR < 0.0){
				valueR = 0.0;
			}

			if (valueG > 255.0) {
				valueG = 255.0;
			}
			else if (valueG < 0.0) {
				valueG = 0.0;
			}

			if (valueB > 255.0) {
				valueB = 255.0;
			}
			else if (valueB < 0.0) {
				valueB = 0.0;
			}

			m_outImageR[i][k] += valueR;
			m_outImageG[i][k] += valueG;
			m_outImageB[i][k] += valueB;
		}
	}
}


void CColorAlpha1Doc::OnParaCUPImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);
	// 진짜! 영상처리 알고리즘
	double valueR, valueG, valueB = 0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			valueR = 255.0 * pow((m_inImageR[i][k] / 128.0 - 1.0), 2);
			valueG = 255.0 * pow((m_inImageG[i][k] / 128.0 - 1.0), 2);
			valueB = 255.0 * pow((m_inImageB[i][k] / 128.0 - 1.0), 2);

			if (valueR > 255.0) {
				valueR = 255.0;
			}
			else if (valueR < 0.0) {
				valueR = 0.0;
			}

			if (valueG > 255.0) {
				valueG = 255.0;
			}
			else if (valueG < 0.0) {
				valueG = 0.0;
			}

			if (valueB > 255.0) {
				valueB = 255.0;
			}
			else if (valueB < 0.0) {
				valueB = 0.0;
			}

			m_outImageR[i][k] += valueR;
			m_outImageG[i][k] += valueG;
			m_outImageB[i][k] += valueB;
		}
	}
}


void CColorAlpha1Doc::OnGammaImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	CInput1Dlg dlg;
	if (dlg.DoModal() != IDOK) {//DoModal --> 뒤에 끊기고 화면 나오는 것 --> 즉 대화상자만 사용할 수 있음
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);
	// 진짜! 영상처리 알고리즘

	float value = (float)dlg.m_Input1;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			double r = m_inImageR[i][k];
			double g = m_inImageG[i][k];
			double b = m_inImageB[i][k];

			m_outImageR[i][k] = (unsigned int)255.0 * pow(r / 255.0, value);
			m_outImageG[i][k] = (unsigned int)255.0 * pow(g / 255.0, value);
			m_outImageB[i][k] = (unsigned int)255.0 * pow(b / 255.0, value);

		}
	}
}


void CColorAlpha1Doc::OnReverseImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);
	// 진짜! 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImageR[i][k] = 255 - m_inImageR[i][k];
			m_outImageG[i][k] = 255 - m_inImageG[i][k];
			m_outImageB[i][k] = 255 - m_inImageB[i][k];
		}
	}
}

///////////////////////////
///////기하학 처리/////////
///////////////////////////


void CColorAlpha1Doc::OnLeftRightMirrorImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);
	// 진짜! 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImageR[i][k] = m_inImageR[i][m_inW - k - 1];
			m_outImageG[i][k] = m_inImageG[i][m_inW - k - 1];
			m_outImageB[i][k] = m_inImageB[i][m_inW - k - 1];
		}
	}
}


void CColorAlpha1Doc::OnUpDownMirrorImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);
	// 진짜! 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImageR[i][k] = m_inImageR[m_inH - i - 1][k];
			m_outImageG[i][k] = m_inImageG[m_inH - i - 1][k];
			m_outImageB[i][k] = m_inImageB[m_inH - i - 1][k];
		}
	}
}


void CColorAlpha1Doc::OnUpDownLeftRightMirrorImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);
	// 진짜! 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImageR[i][k] = m_inImageR[m_inH - i - 1][m_inW - k - 1];
			m_outImageG[i][k] = m_inImageG[m_inH - i - 1][m_inW - k - 1];
			m_outImageB[i][k] = m_inImageB[m_inH - i - 1][m_inW - k - 1];
		}
	}
}


void CColorAlpha1Doc::OnRotateImage1()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	CInput1Dlg dlg;
	if (dlg.DoModal() != IDOK) {//DoModal --> 뒤에 끊기고 화면 나오는 것 --> 즉 대화상자만 사용할 수 있음
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	//배열 초기화
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = 255;
			m_outImageG[i][k] = 255;
			m_outImageB[i][k] = 255;
		}
	}
	// 진짜! 영상처리 알고리즘

	int angle = (int)dlg.m_Input1;//double형이었으니 int로 casting
	int newI, newK = 0;
	//진짜 알고리즘
	//xd = cos*i - sin*k
	//yd = sin*i + cos*k

	double radian = 0;//radian = degree*PI/180.0
	radian = -angle * 3.141592 / 180.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			newI = cos(radian) * i - sin(radian) * k;
			newK = sin(radian) * i + cos(radian) * k;
			
			if ((0 <= newI && newI < m_outH) && (0 <= newK && newK < m_outH)) {
				m_outImageR[newI][newK] = m_inImageR[i][k];
				m_outImageG[newI][newK] = m_inImageG[i][k];
				m_outImageB[newI][newK] = m_inImageB[i][k];
			}
		}
	}
}


void CColorAlpha1Doc::OnRotateImage2()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	CInput1Dlg dlg;
	if (dlg.DoModal() != IDOK) {//DoModal --> 뒤에 끊기고 화면 나오는 것 --> 즉 대화상자만 사용할 수 있음
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);

	int angle = (int)dlg.m_Input1;//double형이었으니 int로 casting

	double seta = angle % 90 * 3.141592 / 180.0;
	double seta_90 = (90 - angle % 90) * 3.141592 / 180.0;

	int tmpSize = 0;
	if (m_inH >= m_inW) {
		tmpSize = m_inH;
	}
	else {
		tmpSize = m_inW;
	}

	m_outH = (int)(tmpSize * cos(seta_90) + tmpSize * cos(seta));
	m_outW = (int)(tmpSize * cos(seta) + tmpSize * cos(seta_90));

	double radian = 0;//radian = degree*PI/180.0
	radian = angle * 3.141592 / 180.0;

	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	//임시 메모리 할당
	unsigned char** tmpImageR;
	unsigned char** tmpImageG;
	unsigned char** tmpImageB;
	
	tmpImageR = malloc2D(m_outH, m_outW);
	tmpImageG = malloc2D(m_outH, m_outW);
	tmpImageB = malloc2D(m_outH, m_outW);

	//배열 초기화
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			tmpImageR[i][k] = 255;
			tmpImageG[i][k] = 255;
			tmpImageB[i][k] = 255;
			m_outImageR[i][k] = 255;
			m_outImageG[i][k] = 255;
			m_outImageB[i][k] = 255;
		}
	}

	int dx = (m_outH - m_inH) / 2;
	int dy = (m_outW - m_inW) / 2;

	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpImageR[i + dx][k + dy] = m_inImageR[i][k];
			tmpImageG[i + dx][k + dy] = m_inImageG[i][k];
			tmpImageB[i + dx][k + dy] = m_inImageB[i][k];
		}
	}
	
	//중앙 위치 구하기
	int oldI, oldK = 0;
	int cX = m_outH / 2;
	int cY = m_outW / 2;

	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {

			oldI = cos(radian) * (i - cX) - sin(radian) * (k - cY) + cX;
			oldK = sin(radian) * (i - cX) + cos(radian) * (k - cY) + cY;


			if ((0 <= oldI && oldI < m_outH) && (0 <= oldK && oldK < m_outW)) {
				m_outImageR[i][k] = tmpImageR[oldI][oldK];
				m_outImageG[i][k] = tmpImageG[oldI][oldK];
				m_outImageB[i][k] = tmpImageB[oldI][oldK];
			}
				
		}
	}
	free2D(tmpImageR, m_outH);
	free2D(tmpImageG, m_outH);
	free2D(tmpImageB, m_outH);
}


void CColorAlpha1Doc::OnZoomInImage1()
{
	// TODO: 여기에 구현 코드 추가.
	//if (m_inImageR == NULL) {
	//	return;
	//}

	//CInput1Dlg dlg;
	//if (dlg.DoModal() != IDOK) {//DoModal --> 뒤에 끊기고 화면 나오는 것 --> 즉 대화상자만 사용할 수 있음
	//	return;
	//}

	//free2D(m_outImageR, m_outH);
	//free2D(m_outImageG, m_outH);
	//free2D(m_outImageB, m_outH);
	//// 출력 영상의 메모리 할당
	//// 
	//// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	//int s_Num = (int)dlg.m_Input1;//double형이었으니 int로 casting
	//m_outH = m_inH * s_Num;
	//m_outW = m_inW * s_Num;
	//// 메모리 할당(배열 선언하기)
	//m_outImageR = malloc2D(m_outH, m_outW);
	//m_outImageG = malloc2D(m_outH, m_outW);
	//m_outImageB = malloc2D(m_outH, m_outW);
	//// 진짜! 영상처리 알고리즘
	//for (int i = 0; i < m_inH; i++) {
	//	for (int k = 0; k < m_inW; k++) {
	//		m_outImageR[i * s_Num][k * s_Num] = m_inImageR[i][k];
	//		m_outImageG[i * s_Num][k * s_Num] = m_inImageG[i][k];
	//		m_outImageB[i * s_Num][k * s_Num] = m_inImageB[i][k];
	//	}
	//}
}


void CColorAlpha1Doc::OnZoomInImage2()
{
	// TODO: 여기에 구현 코드 추가.
	//if (m_inImageR == NULL) {
	//	return;
	//}

	//CInput1Dlg dlg;
	//if (dlg.DoModal() != IDOK) {//DoModal --> 뒤에 끊기고 화면 나오는 것 --> 즉 대화상자만 사용할 수 있음
	//	return;
	//}

	//free2D(m_outImageR, m_outH);
	//free2D(m_outImageG, m_outH);
	//free2D(m_outImageB, m_outH);
	//// 출력 영상의 메모리 할당
	//// 
	//// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	//int s_Num = (int)dlg.m_Input1;//double형이었으니 int로 casting
	//m_outH = m_inH * s_Num;
	//m_outW = m_inW * s_Num;
	//// 메모리 할당(배열 선언하기)
	//m_outImageR = malloc2D(m_outH, m_outW);
	//m_outImageG = malloc2D(m_outH, m_outW);
	//m_outImageB = malloc2D(m_outH, m_outW);
	//// 진짜! 영상처리 알고리즘
	//for (int i = 0; i < m_outH; i++) {
	//	for (int k = 0; k < m_outW; k++) {
	//		m_outImageR[i][k] = m_inImageR[i / s_Num][k / s_Num];
	//		m_outImageG[i][k] = m_inImageG[i / s_Num][k / s_Num];
	//		m_outImageB[i][k] = m_inImageB[i / s_Num][k / s_Num];
	//	}
	//}
}

void CColorAlpha1Doc::OnZoomInImage3()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	//대화상자 띄우기
	CInput1Dlg dlg;
	if (dlg.DoModal() != IDOK) {//DoModal --> 뒤에 끊기고 화면 나오는 것 --> 즉 대화상자만 사용할 수 있음
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	int s_Num = (int)dlg.m_Input1;//double형이었으니 int로 casting
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH * s_Num;
	m_outW = m_inW * s_Num;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	// 진짜! 영상처리 알고리즘


	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImageR[i * s_Num][k * s_Num] = m_inImageR[i][k];
			m_outImageG[i * s_Num][k * s_Num] = m_inImageG[i][k];
			m_outImageB[i * s_Num][k * s_Num] = m_inImageB[i][k];

		}
	}
}
void CColorAlpha1Doc::OnZoomInImage4()
{
	// TODO: 여기에 구현 코드 추가.
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	CInput1Dlg dlg;
	if (dlg.DoModal() != IDOK) {//DoModal --> 뒤에 끊기고 화면 나오는 것 --> 즉 대화상자만 사용할 수 있음
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// 
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	int s_Num = (int)dlg.m_Input1;//double형이었으니 int로 casting
	m_outH = m_inH * s_Num;
	m_outW = m_inW * s_Num;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);
	// 진짜! 영상처리 알고리즘
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = m_inImageR[i / s_Num][k / s_Num];
			m_outImageG[i][k] = m_inImageG[i / s_Num][k / s_Num];
			m_outImageB[i][k] = m_inImageB[i / s_Num][k / s_Num];
		}
	}
}


void CColorAlpha1Doc::OnZoomOutImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	CInput1Dlg dlg;
	if (dlg.DoModal() != IDOK) {//DoModal --> 뒤에 끊기고 화면 나오는 것 --> 즉 대화상자만 사용할 수 있음
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// 
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	int s_Num = (int)dlg.m_Input1;//double형이었으니 int로 casting
	m_outH = m_inH / s_Num;
	m_outW = m_inW / s_Num;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);
	// 진짜! 영상처리 알고리즘
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = m_inImageR[i * s_Num][k * s_Num];
			m_outImageG[i][k] = m_inImageG[i * s_Num][k * s_Num];
			m_outImageB[i][k] = m_inImageB[i * s_Num][k * s_Num];
		}
	}
}

#include"CInput2Dlg.h"
void CColorAlpha1Doc::OnMoveImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	CInput2Dlg dlg;
	if (dlg.DoModal() != IDOK) {//DoModal --> 뒤에 끊기고 화면 나오는 것 --> 즉 대화상자만 사용할 수 있음
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);

	int y_Val = (int)dlg.m_Input1;//double형이었으니 int로 casting
	int x_Val = (int)dlg.m_Input2;//double형이었으니 int로 casting
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);


	// 진짜! 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if ((0 <= i + x_Val && i + x_Val < m_inH) && (0 <= k + y_Val && k + y_Val < m_inH)) {
				m_outImageR[i + x_Val][k + y_Val] = m_inImageR[i][k];
				m_outImageG[i + x_Val][k + y_Val] = m_inImageG[i][k];
				m_outImageB[i + x_Val][k + y_Val] = m_inImageB[i][k];
			}

		}
	}

}

///////////////////////////
////////히스토그램/////////
///////////////////////////

	
void CColorAlpha1Doc::OnHistoStretch()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}
		free2D(m_outImageR, m_outH);
		free2D(m_outImageG, m_outH);
		free2D(m_outImageB, m_outH);

		// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
		m_outH = m_inH;
		m_outW = m_inW;

		// 메모리 할당(배열 선언하기)
		m_outImageR = malloc2D(m_outH, m_outW);
		m_outImageG = malloc2D(m_outH, m_outW);
		m_outImageB = malloc2D(m_outH, m_outW);

		double lowR, highR, lowG, highG, lowB, highB;
		lowR = highR = m_inImageR[0][0];
		lowG = highG = m_inImageG[0][0];
		lowB = highB = m_inImageB[0][0];
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				//R
				if (lowR > m_inImageR[i][k]) {
					lowR = m_inImageR[i][k];
				}
				if (highR < m_inImageR[i][k]) {
					highR = m_inImageR[i][k];
				}
				//G
				if (lowG > m_inImageG[i][k]) {
					lowG = m_inImageG[i][k];
				}
				if (highG < m_inImageG[i][k]) {
					highG = m_inImageG[i][k];
				//B
				}if (lowB > m_inImageB[i][k]) {
					lowB= m_inImageB[i][k];
				}
				if (highB < m_inImageB[i][k]) {
					highB = m_inImageB[i][k];
				}
					
			}
		}
		// 진짜! 영상처리 알고리즘
		double outR, inR, outG, inG, outB, inB;
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				inR = m_inImageR[i][k];
				inG = m_inImageG[i][k];
				inB = m_inImageB[i][k];
				outR = ((inR - lowR) / (double)(highR - lowR)) * 255.0;
				outG = ((inG - lowG) / (double)(highG - lowG)) * 255.0;
				outB = ((inB - lowB) / (double)(highB - lowB)) * 255.0;
				m_outImageR[i][k] = (unsigned char)outR;
				m_outImageG[i][k] = (unsigned char)outG;
				m_outImageB[i][k] = (unsigned char)outB;
			}
		}
	}

void CColorAlpha1Doc::OnEndInSearch()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}
	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);

	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;

	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	double lowR, highR, lowG, highG, lowB, highB;
	lowR = highR = m_inImageR[0][0];
	lowG = highG = m_inImageG[0][0];
	lowB = highB = m_inImageB[0][0];
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			//R
			if (lowR > m_inImageR[i][k]) {
				lowR = m_inImageR[i][k];
			}
			if (highR < m_inImageR[i][k]) {
				highR = m_inImageR[i][k];
			}
			//G
			if (lowG > m_inImageG[i][k]) {
				lowG = m_inImageG[i][k];
			}
			if (highG < m_inImageG[i][k]) {
				highG = m_inImageG[i][k];
				//B
			}if (lowB > m_inImageB[i][k]) {
				lowB = m_inImageB[i][k];
			}
			if (highB < m_inImageB[i][k]) {
				highB = m_inImageB[i][k];
			}

		}
	}

	highR -= 50; lowR += 50;
	highG -= 50; lowG += 50;
	highB -= 50; lowB += 50;

	double outR, inR, outG, inG, outB, inB;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			inR = m_inImageR[i][k];
			inG = m_inImageG[i][k];
			inB = m_inImageB[i][k];
			outR = ((inR - lowR) / (double)(highR - lowR)) * 255.0;
			outG = ((inG - lowG) / (double)(highG - lowG)) * 255.0;
			outB = ((inB - lowB) / (double)(highB - lowB)) * 255.0;
			//R
			if (outR > 255.0) {
				outR = 255.0;
			}
			else if (outR < 0) {
				outR = 0;
			}
			//G
			if (outG > 255.0) {
				outG = 255.0;
			}
			else if (outG < 0) {
				outG = 0;
			}
			//B
			if (outB > 255.0) {
				outB = 255.0;
			}
			else if (outB < 0) {
				outB = 0;
			}

			m_outImageR[i][k] = (unsigned char)outR;
			m_outImageG[i][k] = (unsigned char)outG;
			m_outImageB[i][k] = (unsigned char)outB;
		}
	}
}

void CColorAlpha1Doc::OnHistoEqual()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}
	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);

	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;

	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	// 1단계 : 빈도수 (hist[]) 배열 완성
	int histR[256] = { 0, };
	int histG[256] = { 0, };
	int histB[256] = { 0, };

	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			histR[m_inImageR[i][k]]++;
			histG[m_inImageG[i][k]]++;
			histB[m_inImageB[i][k]]++;
		}
			
	// 2단계 : 누적 히스토그램
	int sumHistR[256] = { 0, };
	int sumHistG[256] = { 0, };
	int sumHistB[256] = { 0, };

	sumHistR[0] = histR[0];
	sumHistG[0] = histG[0];
	sumHistB[0] = histB[0];

	for (int i = 1; i < 256; i++) {
		sumHistR[i] = sumHistR[i - 1] + histR[i];
		sumHistG[i] = sumHistG[i - 1] + histG[i];
		sumHistB[i] = sumHistB[i - 1] + histB[i];
	}
		
	// 3단계 : 정규화된 히스토그램 ==>  normalHist = sumHist * ( 1 / 픽셀수 ) * 255;
	double normalHistoR[256];
	double normalHistoG[256];
	double normalHistoB[256];

	for (int i = 0; i < 256; i++) {
		normalHistoR[i] = sumHistR[i] * (1.0 / (m_inH * m_inW)) * 255.0;
		normalHistoG[i] = sumHistG[i] * (1.0 / (m_inH * m_inW)) * 255.0;
		normalHistoB[i] = sumHistB[i] * (1.0 / (m_inH * m_inW)) * 255.0;
	}
		
	// 4단계 : inImage를 정규화된 값으로 치환.
	// 진짜! 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImageR[i][k] = (unsigned char)normalHistoR[m_inImageR[i][k]];
			m_outImageG[i][k] = (unsigned char)normalHistoG[m_inImageG[i][k]];
			m_outImageB[i][k] = (unsigned char)normalHistoB[m_inImageB[i][k]];
		}
	}
}

double** CColorAlpha1Doc::malloc2D_double(int h, int w)
{
	// TODO: 여기에 구현 코드 추가.
	double** retMemory;

	retMemory = new double* [h];
	for (int i = 0; i < h; i++)
		retMemory[i] = new double[w];
	return retMemory;
}


void CColorAlpha1Doc::free2D_double(double** memory, int h)
{
	// TODO: 여기에 구현 코드 추가.
	if (memory == NULL)
		return;

	for (int i = 0; i < h; i++)
		delete memory[i];
	delete memory;

	memory = NULL;
}

void CColorAlpha1Doc::OnBwImageInput()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	CInput1Dlg dlg;
	if (dlg.DoModal() != IDOK) {//DoModal --> 뒤에 끊기고 화면 나오는 것 --> 즉 대화상자만 사용할 수 있음
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	unsigned char** tmpImageR;
	unsigned char** tmpImageG;
	unsigned char** tmpImageB;
	tmpImageR = malloc2D(m_outH, m_outW);
	tmpImageG = malloc2D(m_outH, m_outW);
	tmpImageB = malloc2D(m_outH, m_outW);
	// 진짜! 영상처리 알고리즘

	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			int avg = (int)((int)m_inImageR[i][k] + (int)m_inImageG[i][k] + (int)m_inImageB[i][k]) / 3.0;
			tmpImageR[i][k] = avg;
			tmpImageG[i][k] = avg;
			tmpImageB[i][k] = avg;
		}
	}

	int value = (int)dlg.m_Input1;//double형이었으니 int로 casting
	
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			
			//R
			if (tmpImageR[i][k] > value) {
				tmpImageR[i][k] = 255;
			}
			if (tmpImageR[i][k] < value) {
				tmpImageR[i][k] = 0;
			}
			//G
			if (tmpImageG[i][k] > value) {
				tmpImageG[i][k] = 255;
			}
			if(tmpImageG[i][k] < value) {
				tmpImageG[i][k] = 0;
			}
			//B
			if (tmpImageB[i][k] > value) {
				tmpImageB[i][k] = 255;
			}
			if(tmpImageB[i][k] < value){
				tmpImageB[i][k] = 0;
			}

			m_outImageR[i][k] = tmpImageR[i][k];
			m_outImageG[i][k] = tmpImageG[i][k];
			m_outImageB[i][k] = tmpImageB[i][k];
		}
	}

	free2D(tmpImageR, m_outH);
	free2D(tmpImageG, m_outH);
	free2D(tmpImageB, m_outH);
}


void CColorAlpha1Doc::OnBwImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	//임시 메모리 할당
	unsigned char** tmpImageR;
	unsigned char** tmpImageG;
	unsigned char** tmpImageB;
	tmpImageR = malloc2D(m_outH, m_outW);
	tmpImageG = malloc2D(m_outH, m_outW);
	tmpImageB = malloc2D(m_outH, m_outW);
	
	//GrayScale 영상 만들기
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			int avg = (int)((int)m_inImageR[i][k] + (int)m_inImageG[i][k] + (int)m_inImageB[i][k]) / 3.0;
			tmpImageR[i][k] = avg;
			tmpImageG[i][k] = avg;
			tmpImageB[i][k] = avg;
		}
	}

	// 진짜! 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			//R
			if (tmpImageR[i][k] > 127) {
				tmpImageR[i][k] = 255;
			}
			if (tmpImageR[i][k] <= 127) {
				tmpImageR[i][k] = 0;
			}
			//G
			if (tmpImageG[i][k] > 127) {
				tmpImageG[i][k] = 255;
			}
			if (tmpImageG[i][k] <= 127) {
				tmpImageG[i][k] = 0;
			}
			//B
			if (tmpImageB[i][k] > 127) {
				tmpImageB[i][k] = 255;
			}
			if (tmpImageB[i][k] <=127) {
				tmpImageB[i][k] = 0;
			}

			m_outImageR[i][k] = tmpImageR[i][k];
			m_outImageG[i][k] = tmpImageG[i][k];
			m_outImageB[i][k] = tmpImageB[i][k];
		}
	}
}


void CColorAlpha1Doc::OnBwAvgImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	//임시 메모리 할당
	unsigned char** tmpImageR;
	unsigned char** tmpImageG;
	unsigned char** tmpImageB;
	tmpImageR = malloc2D(m_outH, m_outW);
	tmpImageG = malloc2D(m_outH, m_outW);
	tmpImageB = malloc2D(m_outH, m_outW);

	//GrayScale 영상 만들기
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			int avg = (int)((int)m_inImageR[i][k] + (int)m_inImageG[i][k] + (int)m_inImageB[i][k]) / 3.0;
			tmpImageR[i][k] = avg;
			tmpImageG[i][k] = avg;
			tmpImageB[i][k] = avg;
		}
	}

	long long hap = 0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			hap += m_inImageR[i][k];
		}
	}

	float avgR = (float)hap / (m_inH * m_inW);

	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			//R
			if (tmpImageR[i][k] > avgR) {
				tmpImageR[i][k] = 255;
			}
			if (tmpImageR[i][k] <= avgR) {
				tmpImageR[i][k] = 0;
			}
			//G
			if (tmpImageG[i][k] > avgR) {
				tmpImageG[i][k] = 255;
			}
			if (tmpImageG[i][k] <= avgR) {
				tmpImageG[i][k] = 0;
			}
			//B
			if (tmpImageB[i][k] > avgR) {
				tmpImageB[i][k] = 255;
			}
			if (tmpImageB[i][k] <= avgR) {
				tmpImageB[i][k] = 0;
			}

			m_outImageR[i][k] = tmpImageR[i][k];
			m_outImageG[i][k] = tmpImageG[i][k];
			m_outImageB[i][k] = tmpImageB[i][k];
		}
	}

}


void CColorAlpha1Doc::OnMidValueBwImage()
{
	//// TODO: 여기에 구현 코드 추가.

	//if (m_inImageR == NULL) {
	//	return;
	//}

	//free2D(m_outImageR, m_outH);
	//free2D(m_outImageG, m_outH);
	//free2D(m_outImageB, m_outH);
	//// 출력 영상의 메모리 할당
	//// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	//m_outH = m_inH;
	//m_outW = m_inW;
	//// 메모리 할당(배열 선언하기)
	//m_outImageR = malloc2D(m_outH, m_outW);
	//m_outImageG = malloc2D(m_outH, m_outW);
	//m_outImageB = malloc2D(m_outH, m_outW);

	////임시 메모리 할당
	//unsigned char** tmpImageR;
	//unsigned char** tmpImageG;
	//unsigned char** tmpImageB;
	//unsigned char** tmpImage;
	//tmpImageR = malloc2D(m_outH, m_outW);
	//tmpImageG = malloc2D(m_outH, m_outW);
	//tmpImageB = malloc2D(m_outH, m_outW);
	//tmpImage = malloc2D(m_outH, m_outW);

	////GrayScale 영상 만들기
	//for (int i = 0; i < m_inH; i++) {
	//	for (int k = 0; k < m_inW; k++) {
	//		int avg = (int)((int)m_inImageR[i][k] + (int)m_inImageG[i][k] + (int)m_inImageB[i][k]) / 3.0;
	//		tmpImageR[i][k] = avg;
	//		tmpImageG[i][k] = avg;
	//		tmpImageB[i][k] = avg;
	//	}
	//}

	//for (int i = 0; i < m_inH; i++) {
	//	for (int k = 0; k < m_inW; k++) {

	//		tmpImage[i][k] = tmpImageR[i][k];
	//	}
	//}
	//
	//int temp = 0;
	//int midVal = 0;

	////for (int i = 0; i < m_inH; i++)//행 정렬
	////{
	////	for (int k = 1; k < m_inH; k++)
	////	{
	////		for (int j = 0; j < m_inH - k; j++)
	////		{
	////			if (tmpImage[i][j] > tmpImage[i][j + 1])
	////			{
	////				temp = tmpImage[i][j + 1];
	////				tmpImage[i][j + 1] = tmpImage[i][j];
	////				tmpImage[i][j] = temp;

	////			}
	////		}
	////	}
	////}

	//for (int i = 0; i < m_inW; i++)//열 정렬
	//{
	//	for (int k = 1; k < m_inW; k++)
	//	{
	//		for (int j = 0; j < m_inW - k; j++)
	//		{
	//			if (tmpImage[j][i] > tmpImage[j + 1][i])
	//			{
	//				temp = tmpImage[j + 1][i];
	//				tmpImage[j + 1][i] = tmpImage[j][i];
	//				tmpImage[j][i] = temp;

	//			}
	//		}
	//	}
	//}

	//midVal = tmpImage[m_inH / 2][m_inW / 2];//순차적으로 배열 했으니 가운데 값 추출
	//
	//
	//for (int i = 0; i < m_inH; i++) {
	//	for (int k = 0; k < m_inW; k++) {
	//		//R
	//		if (tmpImageR[i][k] <= midVal) {
	//			m_outImageR[i][k] = 0;
	//			m_outImageG[i][k] = 0;
	//			m_outImageB[i][k] = 0;
	//		}
	//		else {
	//			m_outImageR[i][k] = 255;
	//			m_outImageG[i][k] = 255;
	//			m_outImageB[i][k] = 255;
	//		}
	//		

	//	}
	//}

	//free2D(tmpImageR, m_outH);//메모리 초기화
	//free2D(tmpImageG, m_outH);//메모리 초기화
	//free2D(tmpImageB, m_outH);//메모리 초기화
	//free2D(tmpImage, m_outH);//메모리 초기화


}


double* CColorAlpha1Doc::RGB2HSI(int R, int G, int B)
{
	// TODO: 여기에 구현 코드 추가.
	double H, S, I;
	double* HSI = new double[3];
	double min_value, angle;
	I = (R + G + B) / 3.0; // 밝기
	if ((R == G) && (G == B)) { // 그레이
		S = 0.0;
		H = 0.0;
	}
	else {

		min_value = min(min(R, G), B); //최소값 추출
		angle = (R - 0.5 * G - 0.5 * B) / (double)sqrt((R - G) * (R - G) + (R - B) * (G - B));

		H = (double)acos(angle) * 57.29577951;
		S = 1.0f - (3.0 / (R + G + B)) * min_value;
	}
	if (B > G) H = 360. - H;

	HSI[0] = H;
	HSI[1] = S;
	HSI[2] = I;

	return HSI;
}


unsigned char* CColorAlpha1Doc::HSI2RGB(double H, double S, double I)
{
	// TODO: 여기에 구현 코드 추가.
	double R, G, B;
	unsigned char* RGB = new unsigned char[3];
	double angle1, angle2, scale;

	if (I == 0.0) { // Black
		RGB[0] = 0;
		RGB[1] = 0;
		RGB[2] = 0;
		return RGB;
	}

	if (H <= 0.0) H += 360.0f;

	scale = 3.0 * I;
	if (H <= 120.0)
	{
		angle1 = H * 0.017453293;
		angle2 = (60.0 - H) * 0.017453293;
		B = (1.0 - S) / 3.0f;
		R = (double)(1.0 + (S * cos(angle1) / cos(angle2))) / 3.0;
		G = 1.0 - R - B;
		B *= scale;
		R *= scale;
		G *= scale;
	}


	else if ((H > 120.0) && (H <= 240.0)) {
		H -= 120.0;
		angle1 = H * 0.017453293;

		angle2 = (60.0 - H) * 0.017453293;
		R = (1.0 - S) / 3.0;
		G = (double)(1.0f + (S * cos(angle1) / cos(angle2))) / 3.0;
		B = 1.0 - R - G;
		R *= scale;
		G *= scale;
		B *= scale;
	}
	else {
		H -= 240.0;
		angle1 = H * 0.017453293;
		angle2 = (60.0 - H) * 0.017453293;
		G = (1.0f - S) / 3.0;
		B = (double)(1.0 + (S * cos(angle1) / cos(angle2))) / 3.0;
		R = 1.0 - G - B;

		R *= scale;
		G *= scale;
		B *= scale;
	}

	RGB[0] = (unsigned char)R;
	RGB[1] = (unsigned char)G;
	RGB[2] = (unsigned char)B;
	return RGB;
}


void CColorAlpha1Doc::OnChangeSatur()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	CInput1Dlg dlg;
	if (dlg.DoModal() != IDOK) {//DoModal --> 뒤에 끊기고 화면 나오는 것 --> 즉 대화상자만 사용할 수 있음
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	// 진짜! 영상처리 알고리즘
	double value = dlg.m_Input1;//double형이었으니 int로 casting
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			//RGB --> HSI
			//H(색상) : 0~360
			//S(채도) : 0.0~1.0
			//I(명도) : 0~255

			double H, S, I;
			unsigned char R, G, B;
			R = m_inImageR[i][k];
			G= m_inImageG[i][k];
			B= m_inImageB[i][k];

			double* hsi = RGB2HSI(R,G,B);
			H = hsi[0];
			S = hsi[1];
			I = hsi[2];
			delete hsi;

			
			//채도 흐리게
			S = S - value;
			if (S < 0.0) {
				S = 0.0;
			}

			//HSI --> RGB
			unsigned char* rgb;
			rgb = HSI2RGB(H, S, I);
			R = rgb[0];
			G = rgb[1];
			B = rgb[2];
			

 			m_outImageR[i][k] = R;
			m_outImageG[i][k] = G;
			m_outImageB[i][k] = B;
			delete rgb;
		}
	}

}


void CColorAlpha1Doc::OnPickOrange()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);
	// 진짜! 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			//RGB --> HSI
			//H(색상) : 0~360
			//S(채도) : 0.0~1.0
			//I(명도) : 0~255

			double H, S, I;
			unsigned char R, G, B;
			R = m_inImageR[i][k];
			G = m_inImageG[i][k];
			B = m_inImageB[i][k];

			double* hsi = RGB2HSI(R, G, B);
			H = hsi[0];
			S = hsi[1];
			I = hsi[2];
			delete hsi;


			//오렌지 값 추출

			if (280 <= H && H <= 340) {
				m_outImageR[i][k] = m_inImageR[i][k];
				m_outImageG[i][k] = m_inImageG[i][k];
				m_outImageB[i][k] = m_inImageB[i][k];
			}
			else {
				int avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k])/3.0;
				m_outImageR[i][k] = avg;
				m_outImageG[i][k] = avg;
				m_outImageB[i][k] = avg;
			}

		}
	}
}
////////////////////////////
//// HSI 기법 사용 안함 ////
////////////////////////////

void CColorAlpha1Doc::OnEmboss()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{ -1.0, 0.0, 0.0 },
		{  0.0, 0.0, 0.0 },
		{  0.0, 0.0, 1.0 }
	};

	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);

	//임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);

	//임시 입력영상의 초기화
	for (int i = 0; i < m_inH + b_MSIZE; i++) {
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;
		}
	}

	//입력 영상 --> 임시 입력 영상
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double S = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageR[i + m][k + n] * mask[m][n];
			tmpOutImageR[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageG[i + m][k + n] * mask[m][n];
			tmpOutImageG[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageB[i + m][k + n] * mask[m][n];
			tmpOutImageB[i][k] = S;
		}
	}
	// 후처리 (마스크의 합계에 따라서.....)
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpOutImageR[i][k] += 127;
			tmpOutImageG[i][k] += 127;
			tmpOutImageB[i][k] += 127;
		}

		// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;

			}
			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;
			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;

			}
			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;
			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;

			}
			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;
			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}
		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);
	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);

	
}


void CColorAlpha1Doc::OnBlurr3x3NoHsi()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{ 1 / 9.0, 1 / 9.0, 1 / 9.0 },
		{ 1 / 9.0, 1 / 9.0, 1 / 9.0 },
		{ 1 / 9.0, 1 / 9.0, 1 / 9.0 }
	};

	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);

	//임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);

	//임시 입력영상의 초기화
	for (int i = 0; i < m_inH + b_MSIZE; i++) {
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;
		}
	}

	//입력 영상 --> 임시 입력 영상
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double S = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageR[i + m][k + n] * mask[m][n];
			tmpOutImageR[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageG[i + m][k + n] * mask[m][n];
			tmpOutImageG[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageB[i + m][k + n] * mask[m][n];
			tmpOutImageB[i][k] = S;
		}
	}
	// 후처리 (마스크의 합계에 따라서.....)
	/*for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpOutImageR[i][k] += 127;
			tmpOutImageG[i][k] += 127;
			tmpOutImageB[i][k] += 127;
		}*/

		// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;

			}
			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;
			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;

			}
			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;
			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;

			}
			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;
			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}
		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);
	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnBlurr5x5NoHsi()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	const int MSIZE = 5;
	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);

	double mask[MSIZE][MSIZE] = {
		{ 1 / 25.0, 1 / 25.0, 1 / 25.0, 1 / 25.0, 1 / 25.0 },
		{ 1 / 25.0, 1 / 25.0, 1 / 25.0, 1 / 25.0, 1 / 25.0 },
		{ 1 / 25.0, 1 / 25.0, 1 / 25.0, 1 / 25.0, 1 / 25.0 },
		{ 1 / 25.0, 1 / 25.0, 1 / 25.0, 1 / 25.0, 1 / 25.0 },
		{ 1 / 25.0, 1 / 25.0, 1 / 25.0, 1 / 25.0, 1 / 25.0 }
	};

	//임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);

	//임시 입력영상의 초기화
	for (int i = 0; i < m_inH + b_MSIZE; i++) {
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;
		}
	}

	//입력 영상 --> 임시 입력 영상
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double S = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageR[i + m][k + n] * mask[m][n];
			tmpOutImageR[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageG[i + m][k + n] * mask[m][n];
			tmpOutImageG[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageB[i + m][k + n] * mask[m][n];
			tmpOutImageB[i][k] = S;
		}
	}
	// 후처리 (마스크의 합계에 따라서.....)
	/*for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpOutImageR[i][k] += 127;
			tmpOutImageG[i][k] += 127;
			tmpOutImageB[i][k] += 127;
		}*/

		// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;

			}
			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;
			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;

			}
			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;
			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;

			}
			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;
			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}
		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);
	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnGaussianFilterNoHsi()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{ 1 / 16.0, 1 / 8.0, 1 / 16.0 },
		{  1 / 8.0, 1 / 4.0,  1 / 8.0 },
		{ 1 / 16.0, 1 / 8.0, 1 / 16.0 }
	};

	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);


	//임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);

	//임시 입력영상의 초기화
	for (int i = 0; i < m_inH + b_MSIZE; i++) {
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;
		}
	}

	//입력 영상 --> 임시 입력 영상
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double S = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageR[i + m][k + n] * mask[m][n];
			tmpOutImageR[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageG[i + m][k + n] * mask[m][n];
			tmpOutImageG[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageB[i + m][k + n] * mask[m][n];
			tmpOutImageB[i][k] = S;
		}
	}
	// 후처리 (마스크의 합계에 따라서.....)
	/*for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpOutImageR[i][k] += 127;
			tmpOutImageG[i][k] += 127;
			tmpOutImageB[i][k] += 127;
		}*/

		// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;

			}
			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;
			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;

			}
			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;
			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;

			}
			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;
			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}
		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);
	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnSharpening1NoHsi()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{ -1.0, -1.0, -1.0 },
		{ -1.0,  9.0, -1.0 },
		{ -1.0, -1.0, -1.0 }
	};

	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);


	//임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);

	//임시 입력영상의 초기화
	for (int i = 0; i < m_inH + b_MSIZE; i++) {
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;
		}
	}

	//입력 영상 --> 임시 입력 영상
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double S = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageR[i + m][k + n] * mask[m][n];
			tmpOutImageR[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageG[i + m][k + n] * mask[m][n];
			tmpOutImageG[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageB[i + m][k + n] * mask[m][n];
			tmpOutImageB[i][k] = S;
		}
	}
	// 후처리 (마스크의 합계에 따라서.....)
	/*for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpOutImageR[i][k] += 127;
			tmpOutImageG[i][k] += 127;
			tmpOutImageB[i][k] += 127;
		}*/

		// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;

			}
			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;
			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;

			}
			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;
			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;

			}
			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;
			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}
		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);
	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnSharpening2NoHsi()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{  0.0, -1.0,  0.0 },
		{ -1.0,  5.0, -1.0 },
		{  0.0, -1.0,  0.0 }
	};

	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);


	//임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);

	//임시 입력영상의 초기화
	for (int i = 0; i < m_inH + b_MSIZE; i++) {
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;
		}
	}

	//입력 영상 --> 임시 입력 영상
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double S = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageR[i + m][k + n] * mask[m][n];
			tmpOutImageR[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageG[i + m][k + n] * mask[m][n];
			tmpOutImageG[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageB[i + m][k + n] * mask[m][n];
			tmpOutImageB[i][k] = S;
		}
	}
	// 후처리 (마스크의 합계에 따라서.....)
	/*for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpOutImageR[i][k] += 127;
			tmpOutImageG[i][k] += 127;
			tmpOutImageB[i][k] += 127;
		}*/

		// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;

			}
			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;
			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;

			}
			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;
			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;

			}
			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;
			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}
		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);
	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnHpfSharpNoHsi()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{ -1 / 9.0, -1 / 9.0, -1 / 9.0 },
		{ -1 / 9.0,  8 / 9.0, -1 / 9.0 },
		{ -1 / 9.0, -1 / 9.0, -1 / 9.0 }
	};

	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);


	//임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);

	//임시 입력영상의 초기화
	for (int i = 0; i < m_inH + b_MSIZE; i++) {
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;
		}
	}

	//입력 영상 --> 임시 입력 영상
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double S = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageR[i + m][k + n] * mask[m][n];
			tmpOutImageR[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageG[i + m][k + n] * mask[m][n];
			tmpOutImageG[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageB[i + m][k + n] * mask[m][n];
			tmpOutImageB[i][k] = S;
		}
	}
	// 후처리 (마스크의 합계에 따라서.....)
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpOutImageR[i][k] += 127;
			tmpOutImageG[i][k] += 127;
			tmpOutImageB[i][k] += 127;
		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;

			}
			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;
			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;

			}
			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;
			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;

			}
			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;
			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}
		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);
	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnPerEdgeNoHsi()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{   0.0, 0.0, 0.0 },
		{  -1.0, 1.0, 0.0 },
		{   0.0, 0.0, 0.0 }
	};

	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);


	//임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);

	//임시 입력영상의 초기화
	for (int i = 0; i < m_inH + b_MSIZE; i++) {
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;
		}
	}

	//입력 영상 --> 임시 입력 영상
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double S = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageR[i + m][k + n] * mask[m][n];
			tmpOutImageR[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageG[i + m][k + n] * mask[m][n];
			tmpOutImageG[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageB[i + m][k + n] * mask[m][n];
			tmpOutImageB[i][k] = S;
		}
	}
	// 후처리 (마스크의 합계에 따라서.....)
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpOutImageR[i][k] += 127;
			tmpOutImageG[i][k] += 127;
			tmpOutImageB[i][k] += 127;
		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;

			}
			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;
			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;

			}
			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;
			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;

			}
			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;
			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}
		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);
	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnHorEdgeNoHsi()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{  0.0, -1.0,  0.0 },
		{  0.0,  1.0,  0.0 },
		{  0.0,  0.0,  0.0 }
	};

	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);


	//임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);

	//임시 입력영상의 초기화
	for (int i = 0; i < m_inH + b_MSIZE; i++) {
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;
		}
	}

	//입력 영상 --> 임시 입력 영상
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double S = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageR[i + m][k + n] * mask[m][n];
			tmpOutImageR[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageG[i + m][k + n] * mask[m][n];
			tmpOutImageG[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageB[i + m][k + n] * mask[m][n];
			tmpOutImageB[i][k] = S;
		}
	}
	// 후처리 (마스크의 합계에 따라서.....)
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpOutImageR[i][k] += 127;
			tmpOutImageG[i][k] += 127;
			tmpOutImageB[i][k] += 127;
		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;

			}
			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;
			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;

			}
			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;
			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;

			}
			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;
			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}
		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);
	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnPerHorEdgeNoHsi()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{  0.0, -1.0,  0.0 },
		{ -1.0,  2.0,  0.0 },
		{  0.0,  0.0,  0.0 }
	};

	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);


	//임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);

	//임시 입력영상의 초기화
	for (int i = 0; i < m_inH + b_MSIZE; i++) {
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;
		}
	}

	//입력 영상 --> 임시 입력 영상
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double S = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageR[i + m][k + n] * mask[m][n];
			tmpOutImageR[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageG[i + m][k + n] * mask[m][n];
			tmpOutImageG[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageB[i + m][k + n] * mask[m][n];
			tmpOutImageB[i][k] = S;
		}
	}
	// 후처리 (마스크의 합계에 따라서.....)
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpOutImageR[i][k] += 127;
			tmpOutImageG[i][k] += 127;
			tmpOutImageB[i][k] += 127;
		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;

			}
			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;
			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;

			}
			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;
			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;

			}
			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;
			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}
		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);
	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnMRobertsImageNoHsi()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{  -1.0, 0.0, -1.0 },
		{   0.0, 2.0,  0.0 },
		{   0.0, 0.0,  0.0 }
	};

	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);


	//임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);

	//임시 입력영상의 초기화
	for (int i = 0; i < m_inH + b_MSIZE; i++) {
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;
		}
	}

	//입력 영상 --> 임시 입력 영상
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double S = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageR[i + m][k + n] * mask[m][n];
			tmpOutImageR[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageG[i + m][k + n] * mask[m][n];
			tmpOutImageG[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageB[i + m][k + n] * mask[m][n];
			tmpOutImageB[i][k] = S;
		}
	}
	// 후처리 (마스크의 합계에 따라서.....)
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpOutImageR[i][k] += 127;
			tmpOutImageG[i][k] += 127;
			tmpOutImageB[i][k] += 127;
		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;

			}
			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;
			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;

			}
			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;
			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;

			}
			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;
			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}
		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);
	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnPrewittImageNoHsi()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{   0.0, -1.0, -2.0 },
		{   1.0,  0.0, -1.0 },
		{   2.0,  1.0,  0.0 }
	};

	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);


	//임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);

	//임시 입력영상의 초기화
	for (int i = 0; i < m_inH + b_MSIZE; i++) {
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;
		}
	}

	//입력 영상 --> 임시 입력 영상
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double S = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageR[i + m][k + n] * mask[m][n];
			tmpOutImageR[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageG[i + m][k + n] * mask[m][n];
			tmpOutImageG[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageB[i + m][k + n] * mask[m][n];
			tmpOutImageB[i][k] = S;
		}
	}
	// 후처리 (마스크의 합계에 따라서.....)
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpOutImageR[i][k] += 127;
			tmpOutImageG[i][k] += 127;
			tmpOutImageB[i][k] += 127;
		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;

			}
			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;
			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;

			}
			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;
			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;

			}
			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;
			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}
		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);
	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}



void CColorAlpha1Doc::OnSobelImageNoHsi()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{   0.0, -2.0, -2.0 },
		{   2.0,  0.0, -2.0 },
		{   2.0,  2.0,  0.0 }
	};

	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);


	//임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);

	//임시 입력영상의 초기화
	for (int i = 0; i < m_inH + b_MSIZE; i++) {
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;
		}
	}

	//입력 영상 --> 임시 입력 영상
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double S = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageR[i + m][k + n] * mask[m][n];
			tmpOutImageR[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageG[i + m][k + n] * mask[m][n];
			tmpOutImageG[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageB[i + m][k + n] * mask[m][n];
			tmpOutImageB[i][k] = S;
		}
	}
	// 후처리 (마스크의 합계에 따라서.....)
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpOutImageR[i][k] += 127;
			tmpOutImageG[i][k] += 127;
			tmpOutImageB[i][k] += 127;
		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;

			}
			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;
			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;

			}
			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;
			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;

			}
			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;
			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}
		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);
	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);

}


void CColorAlpha1Doc::OnLaplacian1NoHsi()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{  0.0, -1.0, -0.0 },
		{ -1.0,  4.0, -1.0 },
		{  0.0, -1.0,  0.0 }
	};

	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);


	//임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);

	//임시 입력영상의 초기화
	for (int i = 0; i < m_inH + b_MSIZE; i++) {
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;
		}
	}

	//입력 영상 --> 임시 입력 영상
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double S = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageR[i + m][k + n] * mask[m][n];
			tmpOutImageR[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageG[i + m][k + n] * mask[m][n];
			tmpOutImageG[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageB[i + m][k + n] * mask[m][n];
			tmpOutImageB[i][k] = S;
		}
	}
	// 후처리 (마스크의 합계에 따라서.....)
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpOutImageR[i][k] += 127;
			tmpOutImageG[i][k] += 127;
			tmpOutImageB[i][k] += 127;
		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;

			}
			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;
			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;

			}
			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;
			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;

			}
			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;
			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}
		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);
	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnLaplacian2NoHsi()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{  1.0,  1.0,  1.0 },
		{  1.0, -8.0,  1.0 },
		{  1.0,  1.0,  1.0 }
	};

	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);


	//임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);

	//임시 입력영상의 초기화
	for (int i = 0; i < m_inH + b_MSIZE; i++) {
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;
		}
	}

	//입력 영상 --> 임시 입력 영상
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double S = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageR[i + m][k + n] * mask[m][n];
			tmpOutImageR[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageG[i + m][k + n] * mask[m][n];
			tmpOutImageG[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageB[i + m][k + n] * mask[m][n];
			tmpOutImageB[i][k] = S;
		}
	}
	// 후처리 (마스크의 합계에 따라서.....)
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpOutImageR[i][k] += 127;
			tmpOutImageG[i][k] += 127;
			tmpOutImageB[i][k] += 127;
		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;

			}
			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;
			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;

			}
			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;
			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;

			}
			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;
			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}
		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);
	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnLaplacian3NoHsi()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{  -1.0, -1.0,  -1.0 },
		{  -1.0,  8.0,  -1.0 },
		{  -1.0, -1.0,  -1.0 }
	};

	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);


	//임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);

	//임시 입력영상의 초기화
	for (int i = 0; i < m_inH + b_MSIZE; i++) {
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;
		}
	}

	//입력 영상 --> 임시 입력 영상
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double S = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageR[i + m][k + n] * mask[m][n];
			tmpOutImageR[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageG[i + m][k + n] * mask[m][n];
			tmpOutImageG[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageB[i + m][k + n] * mask[m][n];
			tmpOutImageB[i][k] = S;
		}
	}
	// 후처리 (마스크의 합계에 따라서.....)
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpOutImageR[i][k] += 127;
			tmpOutImageG[i][k] += 127;
			tmpOutImageB[i][k] += 127;
		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;

			}
			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;
			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;

			}
			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;
			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;

			}
			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;
			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}
		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);
	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnLogImageNoHsi()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	const int MSIZE = 5;
	double mask[MSIZE][MSIZE] = {
		{  0.0,  0.0, -1.0,  0.0,  0.0 },
		{  0.0, -1.0, -2.0, -1.0,  0.0 },
		{ -1.0, -2.0, 16.0, -2.0, -1.0 },
		{  0.0, -1.0, -2.0, -1.0,  0.0 },
		{  0.0,  0.0, -1.0,  0.0,  0.0 },

	};

	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);


	//임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);

	//임시 입력영상의 초기화
	for (int i = 0; i < m_inH + b_MSIZE; i++) {
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;
		}
	}

	//입력 영상 --> 임시 입력 영상
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double S = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageR[i + m][k + n] * mask[m][n];
			tmpOutImageR[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageG[i + m][k + n] * mask[m][n];
			tmpOutImageG[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageB[i + m][k + n] * mask[m][n];
			tmpOutImageB[i][k] = S;
		}
	}
	// 후처리 (마스크의 합계에 따라서.....)
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpOutImageR[i][k] += 127;
			tmpOutImageG[i][k] += 127;
			tmpOutImageB[i][k] += 127;
		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;

			}
			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;
			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;

			}
			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;
			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;

			}
			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;
			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}
		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);
	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnDogImage7x7NoHsi()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	const int MSIZE = 7;
	double mask[MSIZE][MSIZE] = {
		{   0.0,  0.0, -1.0, -1.0, -1.0,  0.0,  0.0 },
		{   0.0, -2.0, -3.0, -3.0, -3.0, -2.0,  0.0 },
		{  -1.0, -3.0,  5.0,  5.0,  5.0, -3.0, -1.0 },
		{  -1.0, -3.0,  5.0, 16.0,  5.0, -3.0, -1.0 },
		{  -1.0, -3.0,  5.0,  5.0,  5.0, -3.0, -1.0 },
		{   0.0, -2.0, -3.0, -3.0, -3.0, -2.0,  0.0 },
		{   0.0,  0.0, -1.0, -1.0, -1.0,  0.0,  0.0 }
	};

	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);


	//임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);

	//임시 입력영상의 초기화
	for (int i = 0; i < m_inH + b_MSIZE; i++) {
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;
		}
	}

	//입력 영상 --> 임시 입력 영상
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double S = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageR[i + m][k + n] * mask[m][n];
			tmpOutImageR[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageG[i + m][k + n] * mask[m][n];
			tmpOutImageG[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageB[i + m][k + n] * mask[m][n];
			tmpOutImageB[i][k] = S;
		}
	}
	// 후처리 (마스크의 합계에 따라서.....)
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpOutImageR[i][k] += 127;
			tmpOutImageG[i][k] += 127;
			tmpOutImageB[i][k] += 127;
		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;

			}
			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;
			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;

			}
			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;
			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;

			}
			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;
			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}
		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);
	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnDogImage9x9NoHsi()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	const int MSIZE = 9;
	double mask[MSIZE][MSIZE] = {
			{ 0.0,  0.0,  0.0, -1.0, -1.0, -1.0,  0.0,  0.0,  0.0 },
			{ 0.0, -2.0, -3.0, -3.0, -3.0, -3.0, -3.0, -2.0,  0.0 },
			{ 0.0, -3.0, -2.0, -1.0, -1.0, -1.0, -2.0, -3.0,  0.0 },
			{ 0.0, -3.0, -1.0,  9.0,  9.0,  9.0, -1.0, -3.0,  0.0 },
			{-1.0, -3.0, -1.0,  9.0, 19.0,  9.0, -1.0, -3.0, -1.0 },
			{ 0.0, -3.0, -1.0,  9.0,  9.0,  9.0, -1.0, -3.0,  0.0 },
			{ 0.0, -3.0, -2.0, -1.0, -1.0, -1.0, -2.0, -3.0,  0.0 },
			{ 0.0, -2.0, -3.0, -3.0, -3.0, -3.0, -3.0, -2.0,  0.0 },
			{ 0.0,  0.0,  0.0, -1.0, -1.0, -1.0,  0.0,  0.0,  0.0 }
	};

	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);


	//임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);

	//임시 입력영상의 초기화
	for (int i = 0; i < m_inH + b_MSIZE; i++) {
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;
		}
	}

	//입력 영상 --> 임시 입력 영상
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}
	}

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double S = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageR[i + m][k + n] * mask[m][n];
			tmpOutImageR[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageG[i + m][k + n] * mask[m][n];
			tmpOutImageG[i][k] = S;

			S = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageB[i + m][k + n] * mask[m][n];
			tmpOutImageB[i][k] = S;
		}
	}
	// 후처리 (마스크의 합계에 따라서.....)
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpOutImageR[i][k] += 127;
			tmpOutImageG[i][k] += 127;
			tmpOutImageB[i][k] += 127;
		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;

			}
			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;
			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;

			}
			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;
			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;

			}
			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;
			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}
		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);
	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


///////////////////////
//// HSI 기법 사용 ////
///////////////////////

void CColorAlpha1Doc::OnEmbossHsi()
{
	// TODO: 여기에 구현 코드 추가.

	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	//////////////////
	// 화소 영역 처리
	//////////////////
	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{ -1.0,  0.0,  0.0 },
		{  0.0,  0.0,  0.0 },
		{  0.0,  0.0,  1.0 }
	};
	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);

	// 임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpInImageH = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageS = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageI = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);
	// 임시 입력영상의 초기화(128)
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;

	// 입력영상 --> 임시입력영상
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}

	///////// RGB --> HSI /////////////
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			double* hsi;
			unsigned char R, G, B;
			R = tmpInImageR[i][k];
			G = tmpInImageG[i][k];
			B = tmpInImageB[i][k];
			hsi = RGB2HSI(R, G, B);

			double H, S, I;
			H = hsi[0];
			S = hsi[1];
			I = hsi[2];
			tmpInImageH[i][k] = H;
			tmpInImageS[i][k] = S;
			tmpInImageI[i][k] = I;
			delete hsi;

		}

	//////////////////////

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double s_value = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			s_value = 0.0;
			for (int m = 0; m < MSIZE; m++) {
				for (int n = 0; n < MSIZE; n++) {
					s_value += tmpInImageI[i + m][k + n] * mask[m][n];
				}
			}
			tmpInImageI[i][k] = s_value;
		}
	}

	// 후처리 : I 값 보정
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpInImageI[i][k] += 127;
		}

	///////// HSI --> RGB /////////////
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			unsigned char* rgb;
			double H, S, I;

			H = tmpInImageH[i][k];
			S = tmpInImageS[i][k];
			I = tmpInImageI[i][k];

			rgb = HSI2RGB(H, S, I);

			tmpOutImageR[i][k] = rgb[0];
			tmpOutImageG[i][k] = rgb[1];
			tmpOutImageB[i][k] = rgb[2];

			delete rgb;

		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;
			}
				
			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;

			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;
			}

			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;

			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;
			}

			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;

			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}
			
		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);

	free2D_double(tmpInImageH, m_inH + b_MSIZE);
	free2D_double(tmpInImageS, m_inH + b_MSIZE);
	free2D_double(tmpInImageI, m_inH + b_MSIZE);

	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnBlurr3x3()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	//////////////////
	// 화소 영역 처리
	//////////////////
	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{ 1 / 9.0, 1 / 9.0, 1 / 9.0 },
		{ 1 / 9.0, 1 / 9.0, 1 / 9.0 },
		{ 1 / 9.0, 1 / 9.0, 1 / 9.0 }
	};
	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);

	// 임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpInImageH = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageS = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageI = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);
	// 임시 입력영상의 초기화(128)
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;

	// 입력영상 --> 임시입력영상
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}

	///////// RGB --> HSI /////////////
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			double* hsi;
			unsigned char R, G, B;
			R = tmpInImageR[i][k];
			G = tmpInImageG[i][k];
			B = tmpInImageB[i][k];
			hsi = RGB2HSI(R, G, B);

			double H, S, I;
			H = hsi[0];
			S = hsi[1];
			I = hsi[2];
			tmpInImageH[i][k] = H;
			tmpInImageS[i][k] = S;
			tmpInImageI[i][k] = I;
			delete hsi;

		}

	//////////////////////

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double s_value = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			s_value = 0.0;
			for (int m = 0; m < MSIZE; m++) {
				for (int n = 0; n < MSIZE; n++) {
					s_value += tmpInImageI[i + m][k + n] * mask[m][n];
				}
			}
			tmpInImageI[i][k] = s_value;
		}
	}

	// 후처리 : I 값 보정
	/*for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpInImageI[i][k] += 127;
		}*/

	///////// HSI --> RGB /////////////
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			unsigned char* rgb;
			double H, S, I;

			H = tmpInImageH[i][k];
			S = tmpInImageS[i][k];
			I = tmpInImageI[i][k];

			rgb = HSI2RGB(H, S, I);

			tmpOutImageR[i][k] = rgb[0];
			tmpOutImageG[i][k] = rgb[1];
			tmpOutImageB[i][k] = rgb[2];

			delete rgb;

		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;
			}

			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;

			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;
			}

			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;

			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;
			}

			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;

			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}

		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);

	free2D_double(tmpInImageH, m_inH + b_MSIZE);
	free2D_double(tmpInImageS, m_inH + b_MSIZE);
	free2D_double(tmpInImageI, m_inH + b_MSIZE);

	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnBlurr5x5()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	//////////////////
	// 화소 영역 처리
	//////////////////
	const int MSIZE = 5;
	double mask[MSIZE][MSIZE] = {
		{ 1 / 25.0, 1 / 25.0, 1 / 25.0, 1 / 25.0, 1 / 25.0 },
		{ 1 / 25.0, 1 / 25.0, 1 / 25.0, 1 / 25.0, 1 / 25.0 },
		{ 1 / 25.0, 1 / 25.0, 1 / 25.0, 1 / 25.0, 1 / 25.0 },
		{ 1 / 25.0, 1 / 25.0, 1 / 25.0, 1 / 25.0, 1 / 25.0 },
		{ 1 / 25.0, 1 / 25.0, 1 / 25.0, 1 / 25.0, 1 / 25.0 }
	};
	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);

	// 임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpInImageH = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageS = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageI = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);
	// 임시 입력영상의 초기화(128)
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;

	// 입력영상 --> 임시입력영상
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}

	///////// RGB --> HSI /////////////
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			double* hsi;
			unsigned char R, G, B;
			R = tmpInImageR[i][k];
			G = tmpInImageG[i][k];
			B = tmpInImageB[i][k];
			hsi = RGB2HSI(R, G, B);

			double H, S, I;
			H = hsi[0];
			S = hsi[1];
			I = hsi[2];
			tmpInImageH[i][k] = H;
			tmpInImageS[i][k] = S;
			tmpInImageI[i][k] = I;
			delete hsi;

		}

	//////////////////////

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double s_value = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			s_value = 0.0;
			for (int m = 0; m < MSIZE; m++) {
				for (int n = 0; n < MSIZE; n++) {
					s_value += tmpInImageI[i + m][k + n] * mask[m][n];
				}
			}
			tmpInImageI[i][k] = s_value;
		}
	}

	// 후처리 : I 값 보정
	/*for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpInImageI[i][k] += 127;
		}*/

	///////// HSI --> RGB /////////////
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			unsigned char* rgb;
			double H, S, I;

			H = tmpInImageH[i][k];
			S = tmpInImageS[i][k];
			I = tmpInImageI[i][k];

			rgb = HSI2RGB(H, S, I);

			tmpOutImageR[i][k] = rgb[0];
			tmpOutImageG[i][k] = rgb[1];
			tmpOutImageB[i][k] = rgb[2];

			delete rgb;

		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;
			}

			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;

			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;
			}

			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;

			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;
			}

			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;

			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}

		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);

	free2D_double(tmpInImageH, m_inH + b_MSIZE);
	free2D_double(tmpInImageS, m_inH + b_MSIZE);
	free2D_double(tmpInImageI, m_inH + b_MSIZE);

	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnGaussianFilter()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	//////////////////
	// 화소 영역 처리
	//////////////////
	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{ 1 / 16.0, 1 / 8.0, 1 / 16.0 },
		{  1 / 8.0, 1 / 4.0,  1 / 8.0 },
		{ 1 / 16.0, 1 / 8.0, 1 / 16.0 }
	};
	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);

	// 임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpInImageH = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageS = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageI = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);
	// 임시 입력영상의 초기화(128)
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;

	// 입력영상 --> 임시입력영상
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}

	///////// RGB --> HSI /////////////
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			double* hsi;
			unsigned char R, G, B;
			R = tmpInImageR[i][k];
			G = tmpInImageG[i][k];
			B = tmpInImageB[i][k];
			hsi = RGB2HSI(R, G, B);

			double H, S, I;
			H = hsi[0];
			S = hsi[1];
			I = hsi[2];
			tmpInImageH[i][k] = H;
			tmpInImageS[i][k] = S;
			tmpInImageI[i][k] = I;
			delete hsi;

		}

	//////////////////////

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double s_value = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			s_value = 0.0;
			for (int m = 0; m < MSIZE; m++) {
				for (int n = 0; n < MSIZE; n++) {
					s_value += tmpInImageI[i + m][k + n] * mask[m][n];
				}
			}
			tmpInImageI[i][k] = s_value;
		}
	}

	// 후처리 : I 값 보정
	/*for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpInImageI[i][k] += 127;
		}*/

	///////// HSI --> RGB /////////////
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			unsigned char* rgb;
			double H, S, I;

			H = tmpInImageH[i][k];
			S = tmpInImageS[i][k];
			I = tmpInImageI[i][k];

			rgb = HSI2RGB(H, S, I);

			tmpOutImageR[i][k] = rgb[0];
			tmpOutImageG[i][k] = rgb[1];
			tmpOutImageB[i][k] = rgb[2];

			delete rgb;

		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;
			}

			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;

			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;
			}

			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;

			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;
			}

			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;

			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}

		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);

	free2D_double(tmpInImageH, m_inH + b_MSIZE);
	free2D_double(tmpInImageS, m_inH + b_MSIZE);
	free2D_double(tmpInImageI, m_inH + b_MSIZE);

	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);

}


void CColorAlpha1Doc::OnSharpening1()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	//////////////////
	// 화소 영역 처리
	//////////////////
	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{ -1.0, -1.0, -1.0 },
		{ -1.0,  9.0, -1.0 },
		{ -1.0, -1.0, -1.0 }
	};
	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);

	// 임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpInImageH = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageS = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageI = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);
	// 임시 입력영상의 초기화(128)
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;

	// 입력영상 --> 임시입력영상
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}

	///////// RGB --> HSI /////////////
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			double* hsi;
			unsigned char R, G, B;
			R = tmpInImageR[i][k];
			G = tmpInImageG[i][k];
			B = tmpInImageB[i][k];
			hsi = RGB2HSI(R, G, B);

			double H, S, I;
			H = hsi[0];
			S = hsi[1];
			I = hsi[2];
			tmpInImageH[i][k] = H;
			tmpInImageS[i][k] = S;
			tmpInImageI[i][k] = I;
			delete hsi;

		}

	//////////////////////

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double s_value = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			s_value = 0.0;
			for (int m = 0; m < MSIZE; m++) {
				for (int n = 0; n < MSIZE; n++) {
					s_value += tmpInImageI[i + m][k + n] * mask[m][n];
				}
			}
			tmpInImageI[i][k] = s_value;
		}
	}

	// 후처리 : I 값 보정
	/*for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpInImageI[i][k] += 127;
		}*/

	///////// HSI --> RGB /////////////
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			unsigned char* rgb;
			double H, S, I;

			H = tmpInImageH[i][k];
			S = tmpInImageS[i][k];
			I = tmpInImageI[i][k];

			rgb = HSI2RGB(H, S, I);

			tmpOutImageR[i][k] = rgb[0];
			tmpOutImageG[i][k] = rgb[1];
			tmpOutImageB[i][k] = rgb[2];

			delete rgb;

		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;
			}

			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;

			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;
			}

			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;

			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;
			}

			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;

			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}

		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);

	free2D_double(tmpInImageH, m_inH + b_MSIZE);
	free2D_double(tmpInImageS, m_inH + b_MSIZE);
	free2D_double(tmpInImageI, m_inH + b_MSIZE);

	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnSharpening2()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	//////////////////
	// 화소 영역 처리
	//////////////////
	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{  0.0, -1.0,  0.0 },
		{ -1.0,  5.0, -1.0 },
		{  0.0, -1.0,  0.0 }
	};
	// 임시 메모리 할당
	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);

	// 임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpInImageH = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageS = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageI = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);
	// 임시 입력영상의 초기화(128)
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;

	// 입력영상 --> 임시입력영상
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}

	///////// RGB --> HSI /////////////
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			double* hsi;
			unsigned char R, G, B;
			R = tmpInImageR[i][k];
			G = tmpInImageG[i][k];
			B = tmpInImageB[i][k];
			hsi = RGB2HSI(R, G, B);

			double H, S, I;
			H = hsi[0];
			S = hsi[1];
			I = hsi[2];
			tmpInImageH[i][k] = H;
			tmpInImageS[i][k] = S;
			tmpInImageI[i][k] = I;
			delete hsi;

		}

	//////////////////////

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double s_value = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			s_value = 0.0;
			for (int m = 0; m < MSIZE; m++) {
				for (int n = 0; n < MSIZE; n++) {
					s_value += tmpInImageI[i + m][k + n] * mask[m][n];
				}
			}
			tmpInImageI[i][k] = s_value;
		}
	}

	// 후처리 : I 값 보정
	/*for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpInImageI[i][k] += 127;
		}*/

	///////// HSI --> RGB /////////////
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			unsigned char* rgb;
			double H, S, I;

			H = tmpInImageH[i][k];
			S = tmpInImageS[i][k];
			I = tmpInImageI[i][k];

			rgb = HSI2RGB(H, S, I);

			tmpOutImageR[i][k] = rgb[0];
			tmpOutImageG[i][k] = rgb[1];
			tmpOutImageB[i][k] = rgb[2];

			delete rgb;

		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;
			}

			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;

			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;
			}

			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;

			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;
			}

			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;

			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}

		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);

	free2D_double(tmpInImageH, m_inH + b_MSIZE);
	free2D_double(tmpInImageS, m_inH + b_MSIZE);
	free2D_double(tmpInImageI, m_inH + b_MSIZE);

	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnHpfSharp()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	//////////////////
	// 화소 영역 처리
	//////////////////
	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{ -1 / 9.0, -1 / 9.0, -1 / 9.0 },
		{ -1 / 9.0,  8 / 9.0, -1 / 9.0 },
		{ -1 / 9.0, -1 / 9.0, -1 / 9.0 }
	};
	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);

	// 임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpInImageH = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageS = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageI = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);
	// 임시 입력영상의 초기화(128)
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;

	// 입력영상 --> 임시입력영상
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}

	///////// RGB --> HSI /////////////
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			double* hsi;
			unsigned char R, G, B;
			R = tmpInImageR[i][k];
			G = tmpInImageG[i][k];
			B = tmpInImageB[i][k];
			hsi = RGB2HSI(R, G, B);

			double H, S, I;
			H = hsi[0];
			S = hsi[1];
			I = hsi[2];
			tmpInImageH[i][k] = H;
			tmpInImageS[i][k] = S;
			tmpInImageI[i][k] = I;
			delete hsi;

		}

	//////////////////////

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double s_value = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			s_value = 0.0;
			for (int m = 0; m < MSIZE; m++) {
				for (int n = 0; n < MSIZE; n++) {
					s_value += tmpInImageI[i + m][k + n] * mask[m][n];
				}
			}
			tmpInImageI[i][k] = s_value;
		}
	}

	// 후처리 : I 값 보정
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpInImageI[i][k] += 127;
		}

	///////// HSI --> RGB /////////////
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			unsigned char* rgb;
			double H, S, I;

			H = tmpInImageH[i][k];
			S = tmpInImageS[i][k];
			I = tmpInImageI[i][k];

			rgb = HSI2RGB(H, S, I);

			tmpOutImageR[i][k] = rgb[0];
			tmpOutImageG[i][k] = rgb[1];
			tmpOutImageB[i][k] = rgb[2];

			delete rgb;

		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;
			}

			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;

			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;
			}

			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;

			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;
			}

			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;

			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}

		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);

	free2D_double(tmpInImageH, m_inH + b_MSIZE);
	free2D_double(tmpInImageS, m_inH + b_MSIZE);
	free2D_double(tmpInImageI, m_inH + b_MSIZE);

	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnPerEdge()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	//////////////////
	// 화소 영역 처리
	//////////////////
	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{   0.0, 0.0, 0.0 },
		{  -1.0, 1.0, 0.0 },
		{   0.0, 0.0, 0.0 }
	};
	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);

	// 임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpInImageH = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageS = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageI = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);
	// 임시 입력영상의 초기화(128)
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;

	// 입력영상 --> 임시입력영상
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}

	///////// RGB --> HSI /////////////
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			double* hsi;
			unsigned char R, G, B;
			R = tmpInImageR[i][k];
			G = tmpInImageG[i][k];
			B = tmpInImageB[i][k];
			hsi = RGB2HSI(R, G, B);

			double H, S, I;
			H = hsi[0];
			S = hsi[1];
			I = hsi[2];
			tmpInImageH[i][k] = H;
			tmpInImageS[i][k] = S;
			tmpInImageI[i][k] = I;
			delete hsi;

		}

	//////////////////////

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double s_value = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			s_value = 0.0;
			for (int m = 0; m < MSIZE; m++) {
				for (int n = 0; n < MSIZE; n++) {
					s_value += tmpInImageI[i + m][k + n] * mask[m][n];
				}
			}
			tmpInImageI[i][k] = s_value;
		}
	}

	// 후처리 : I 값 보정
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpInImageI[i][k] += 127;
		}

	///////// HSI --> RGB /////////////
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			unsigned char* rgb;
			double H, S, I;

			H = tmpInImageH[i][k];
			S = tmpInImageS[i][k];
			I = tmpInImageI[i][k];

			rgb = HSI2RGB(H, S, I);

			tmpOutImageR[i][k] = rgb[0];
			tmpOutImageG[i][k] = rgb[1];
			tmpOutImageB[i][k] = rgb[2];

			delete rgb;

		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;
			}

			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;

			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;
			}

			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;

			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;
			}

			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;

			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}

		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);

	free2D_double(tmpInImageH, m_inH + b_MSIZE);
	free2D_double(tmpInImageS, m_inH + b_MSIZE);
	free2D_double(tmpInImageI, m_inH + b_MSIZE);

	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnHorEdge()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	//////////////////
	// 화소 영역 처리
	//////////////////
	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{  0.0, -1.0,  0.0 },
		{  0.0,  1.0,  0.0 },
		{  0.0,  0.0,  0.0 }
	};
	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);

	// 임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpInImageH = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageS = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageI = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);
	// 임시 입력영상의 초기화(128)
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;

	// 입력영상 --> 임시입력영상
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}

	///////// RGB --> HSI /////////////
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			double* hsi;
			unsigned char R, G, B;
			R = tmpInImageR[i][k];
			G = tmpInImageG[i][k];
			B = tmpInImageB[i][k];
			hsi = RGB2HSI(R, G, B);

			double H, S, I;
			H = hsi[0];
			S = hsi[1];
			I = hsi[2];
			tmpInImageH[i][k] = H;
			tmpInImageS[i][k] = S;
			tmpInImageI[i][k] = I;
			delete hsi;

		}

	//////////////////////

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double s_value = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			s_value = 0.0;
			for (int m = 0; m < MSIZE; m++) {
				for (int n = 0; n < MSIZE; n++) {
					s_value += tmpInImageI[i + m][k + n] * mask[m][n];
				}
			}
			tmpInImageI[i][k] = s_value;
		}
	}

	// 후처리 : I 값 보정
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpInImageI[i][k] += 127;
		}

	///////// HSI --> RGB /////////////
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			unsigned char* rgb;
			double H, S, I;

			H = tmpInImageH[i][k];
			S = tmpInImageS[i][k];
			I = tmpInImageI[i][k];

			rgb = HSI2RGB(H, S, I);

			tmpOutImageR[i][k] = rgb[0];
			tmpOutImageG[i][k] = rgb[1];
			tmpOutImageB[i][k] = rgb[2];

			delete rgb;

		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;
			}

			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;

			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;
			}

			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;

			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;
			}

			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;

			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}

		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);

	free2D_double(tmpInImageH, m_inH + b_MSIZE);
	free2D_double(tmpInImageS, m_inH + b_MSIZE);
	free2D_double(tmpInImageI, m_inH + b_MSIZE);

	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnPerHorEdge()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	//////////////////
	// 화소 영역 처리
	//////////////////
	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{  0.0, -1.0,  0.0 },
		{ -1.0,  2.0,  0.0 },
		{  0.0,  0.0,  0.0 }
	};
	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);

	// 임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpInImageH = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageS = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageI = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);
	// 임시 입력영상의 초기화(128)
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;

	// 입력영상 --> 임시입력영상
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}

	///////// RGB --> HSI /////////////
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			double* hsi;
			unsigned char R, G, B;
			R = tmpInImageR[i][k];
			G = tmpInImageG[i][k];
			B = tmpInImageB[i][k];
			hsi = RGB2HSI(R, G, B);

			double H, S, I;
			H = hsi[0];
			S = hsi[1];
			I = hsi[2];
			tmpInImageH[i][k] = H;
			tmpInImageS[i][k] = S;
			tmpInImageI[i][k] = I;
			delete hsi;

		}

	//////////////////////

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double s_value = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			s_value = 0.0;
			for (int m = 0; m < MSIZE; m++) {
				for (int n = 0; n < MSIZE; n++) {
					s_value += tmpInImageI[i + m][k + n] * mask[m][n];
				}
			}
			tmpInImageI[i][k] = s_value;
		}
	}

	// 후처리 : I 값 보정
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpInImageI[i][k] += 127;
		}

	///////// HSI --> RGB /////////////
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			unsigned char* rgb;
			double H, S, I;

			H = tmpInImageH[i][k];
			S = tmpInImageS[i][k];
			I = tmpInImageI[i][k];

			rgb = HSI2RGB(H, S, I);

			tmpOutImageR[i][k] = rgb[0];
			tmpOutImageG[i][k] = rgb[1];
			tmpOutImageB[i][k] = rgb[2];

			delete rgb;

		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;
			}

			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;

			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;
			}

			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;

			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;
			}

			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;

			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}

		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);

	free2D_double(tmpInImageH, m_inH + b_MSIZE);
	free2D_double(tmpInImageS, m_inH + b_MSIZE);
	free2D_double(tmpInImageI, m_inH + b_MSIZE);

	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnRobertsImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	//////////////////
	// 화소 영역 처리
	//////////////////
	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{  -1.0, 0.0, -1.0 },
		{   0.0, 2.0,  0.0 },
		{   0.0, 0.0,  0.0 }
	};
	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);

	// 임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpInImageH = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageS = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageI = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);
	// 임시 입력영상의 초기화(128)
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;

	// 입력영상 --> 임시입력영상
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}

	///////// RGB --> HSI /////////////
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			double* hsi;
			unsigned char R, G, B;
			R = tmpInImageR[i][k];
			G = tmpInImageG[i][k];
			B = tmpInImageB[i][k];
			hsi = RGB2HSI(R, G, B);

			double H, S, I;
			H = hsi[0];
			S = hsi[1];
			I = hsi[2];
			tmpInImageH[i][k] = H;
			tmpInImageS[i][k] = S;
			tmpInImageI[i][k] = I;
			delete hsi;

		}

	//////////////////////

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double s_value = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			s_value = 0.0;
			for (int m = 0; m < MSIZE; m++) {
				for (int n = 0; n < MSIZE; n++) {
					s_value += tmpInImageI[i + m][k + n] * mask[m][n];
				}
			}
			tmpInImageI[i][k] = s_value;
		}
	}

	// 후처리 : I 값 보정
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpInImageI[i][k] += 127;
		}

	///////// HSI --> RGB /////////////
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			unsigned char* rgb;
			double H, S, I;

			H = tmpInImageH[i][k];
			S = tmpInImageS[i][k];
			I = tmpInImageI[i][k];

			rgb = HSI2RGB(H, S, I);

			tmpOutImageR[i][k] = rgb[0];
			tmpOutImageG[i][k] = rgb[1];
			tmpOutImageB[i][k] = rgb[2];

			delete rgb;

		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;
			}

			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;

			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;
			}

			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;

			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;
			}

			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;

			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}

		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);

	free2D_double(tmpInImageH, m_inH + b_MSIZE);
	free2D_double(tmpInImageS, m_inH + b_MSIZE);
	free2D_double(tmpInImageI, m_inH + b_MSIZE);

	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
	
}


void CColorAlpha1Doc::OnPrewittImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	//////////////////
	// 화소 영역 처리
	//////////////////
	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{   0.0, -1.0, -2.0 },
		{   1.0,  0.0, -1.0 },
		{   2.0,  1.0,  0.0 }
	};
	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);

	// 임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpInImageH = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageS = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageI = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);
	// 임시 입력영상의 초기화(128)
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;

	// 입력영상 --> 임시입력영상
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}

	///////// RGB --> HSI /////////////
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			double* hsi;
			unsigned char R, G, B;
			R = tmpInImageR[i][k];
			G = tmpInImageG[i][k];
			B = tmpInImageB[i][k];
			hsi = RGB2HSI(R, G, B);

			double H, S, I;
			H = hsi[0];
			S = hsi[1];
			I = hsi[2];
			tmpInImageH[i][k] = H;
			tmpInImageS[i][k] = S;
			tmpInImageI[i][k] = I;
			delete hsi;

		}

	//////////////////////

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double s_value = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			s_value = 0.0;
			for (int m = 0; m < MSIZE; m++) {
				for (int n = 0; n < MSIZE; n++) {
					s_value += tmpInImageI[i + m][k + n] * mask[m][n];
				}
			}
			tmpInImageI[i][k] = s_value;
		}
	}

	// 후처리 : I 값 보정
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpInImageI[i][k] += 127;
		}

	///////// HSI --> RGB /////////////
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			unsigned char* rgb;
			double H, S, I;

			H = tmpInImageH[i][k];
			S = tmpInImageS[i][k];
			I = tmpInImageI[i][k];

			rgb = HSI2RGB(H, S, I);

			tmpOutImageR[i][k] = rgb[0];
			tmpOutImageG[i][k] = rgb[1];
			tmpOutImageB[i][k] = rgb[2];

			delete rgb;

		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;
			}

			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;

			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;
			}

			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;

			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;
			}

			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;

			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}

		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);

	free2D_double(tmpInImageH, m_inH + b_MSIZE);
	free2D_double(tmpInImageS, m_inH + b_MSIZE);
	free2D_double(tmpInImageI, m_inH + b_MSIZE);

	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnSobelImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	//////////////////
	// 화소 영역 처리
	//////////////////
	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{   0.0, -2.0, -2.0 },
		{   2.0,  0.0, -2.0 },
		{   2.0,  2.0,  0.0 }
	};
	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);

	// 임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpInImageH = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageS = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageI = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);
	// 임시 입력영상의 초기화(128)
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;

	// 입력영상 --> 임시입력영상
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}

	///////// RGB --> HSI /////////////
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			double* hsi;
			unsigned char R, G, B;
			R = tmpInImageR[i][k];
			G = tmpInImageG[i][k];
			B = tmpInImageB[i][k];
			hsi = RGB2HSI(R, G, B);

			double H, S, I;
			H = hsi[0];
			S = hsi[1];
			I = hsi[2];
			tmpInImageH[i][k] = H;
			tmpInImageS[i][k] = S;
			tmpInImageI[i][k] = I;
			delete hsi;

		}

	//////////////////////

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double s_value = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			s_value = 0.0;
			for (int m = 0; m < MSIZE; m++) {
				for (int n = 0; n < MSIZE; n++) {
					s_value += tmpInImageI[i + m][k + n] * mask[m][n];
				}
			}
			tmpInImageI[i][k] = s_value;
		}
	}

	// 후처리 : I 값 보정
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpInImageI[i][k] += 127;
		}

	///////// HSI --> RGB /////////////
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			unsigned char* rgb;
			double H, S, I;

			H = tmpInImageH[i][k];
			S = tmpInImageS[i][k];
			I = tmpInImageI[i][k];

			rgb = HSI2RGB(H, S, I);

			tmpOutImageR[i][k] = rgb[0];
			tmpOutImageG[i][k] = rgb[1];
			tmpOutImageB[i][k] = rgb[2];

			delete rgb;

		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;
			}

			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;

			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;
			}

			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;

			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;
			}

			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;

			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}

		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);

	free2D_double(tmpInImageH, m_inH + b_MSIZE);
	free2D_double(tmpInImageS, m_inH + b_MSIZE);
	free2D_double(tmpInImageI, m_inH + b_MSIZE);

	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnLaplacian1()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	//////////////////
	// 화소 영역 처리
	//////////////////
	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{  0.0, -1.0, -0.0 },
		{ -1.0,  4.0, -1.0 },
		{  0.0, -1.0,  0.0 }
	};
	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);

	// 임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpInImageH = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageS = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageI = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);
	// 임시 입력영상의 초기화(128)
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;

	// 입력영상 --> 임시입력영상
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}

	///////// RGB --> HSI /////////////
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			double* hsi;
			unsigned char R, G, B;
			R = tmpInImageR[i][k];
			G = tmpInImageG[i][k];
			B = tmpInImageB[i][k];
			hsi = RGB2HSI(R, G, B);

			double H, S, I;
			H = hsi[0];
			S = hsi[1];
			I = hsi[2];
			tmpInImageH[i][k] = H;
			tmpInImageS[i][k] = S;
			tmpInImageI[i][k] = I;
			delete hsi;

		}

	//////////////////////

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double s_value = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			s_value = 0.0;
			for (int m = 0; m < MSIZE; m++) {
				for (int n = 0; n < MSIZE; n++) {
					s_value += tmpInImageI[i + m][k + n] * mask[m][n];
				}
			}
			tmpInImageI[i][k] = s_value;
		}
	}

	// 후처리 : I 값 보정
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpInImageI[i][k] += 127;
		}

	///////// HSI --> RGB /////////////
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			unsigned char* rgb;
			double H, S, I;

			H = tmpInImageH[i][k];
			S = tmpInImageS[i][k];
			I = tmpInImageI[i][k];

			rgb = HSI2RGB(H, S, I);

			tmpOutImageR[i][k] = rgb[0];
			tmpOutImageG[i][k] = rgb[1];
			tmpOutImageB[i][k] = rgb[2];

			delete rgb;

		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;
			}

			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;

			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;
			}

			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;

			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;
			}

			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;

			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}

		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);

	free2D_double(tmpInImageH, m_inH + b_MSIZE);
	free2D_double(tmpInImageS, m_inH + b_MSIZE);
	free2D_double(tmpInImageI, m_inH + b_MSIZE);

	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnLaplacian2()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	//////////////////
	// 화소 영역 처리
	//////////////////
	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{  1.0,  1.0,  1.0 },
		{  1.0, -8.0,  1.0 },
		{  1.0,  1.0,  1.0 }
	};
	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);

	// 임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpInImageH = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageS = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageI = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);
	// 임시 입력영상의 초기화(128)
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;

	// 입력영상 --> 임시입력영상
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}

	///////// RGB --> HSI /////////////
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			double* hsi;
			unsigned char R, G, B;
			R = tmpInImageR[i][k];
			G = tmpInImageG[i][k];
			B = tmpInImageB[i][k];
			hsi = RGB2HSI(R, G, B);

			double H, S, I;
			H = hsi[0];
			S = hsi[1];
			I = hsi[2];
			tmpInImageH[i][k] = H;
			tmpInImageS[i][k] = S;
			tmpInImageI[i][k] = I;
			delete hsi;

		}

	//////////////////////

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double s_value = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			s_value = 0.0;
			for (int m = 0; m < MSIZE; m++) {
				for (int n = 0; n < MSIZE; n++) {
					s_value += tmpInImageI[i + m][k + n] * mask[m][n];
				}
			}
			tmpInImageI[i][k] = s_value;
		}
	}

	// 후처리 : I 값 보정
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpInImageI[i][k] += 127;
		}

	///////// HSI --> RGB /////////////
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			unsigned char* rgb;
			double H, S, I;

			H = tmpInImageH[i][k];
			S = tmpInImageS[i][k];
			I = tmpInImageI[i][k];

			rgb = HSI2RGB(H, S, I);

			tmpOutImageR[i][k] = rgb[0];
			tmpOutImageG[i][k] = rgb[1];
			tmpOutImageB[i][k] = rgb[2];

			delete rgb;

		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;
			}

			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;

			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;
			}

			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;

			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;
			}

			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;

			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}

		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);

	free2D_double(tmpInImageH, m_inH + b_MSIZE);
	free2D_double(tmpInImageS, m_inH + b_MSIZE);
	free2D_double(tmpInImageI, m_inH + b_MSIZE);

	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnLaplacian3()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	//////////////////
	// 화소 영역 처리
	//////////////////
	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{  -1.0, -1.0,  -1.0 },
		{  -1.0,  8.0,  -1.0 },
		{  -1.0, -1.0,  -1.0 }
	};
	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);

	// 임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpInImageH = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageS = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageI = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);
	// 임시 입력영상의 초기화(128)
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;

	// 입력영상 --> 임시입력영상
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}

	///////// RGB --> HSI /////////////
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			double* hsi;
			unsigned char R, G, B;
			R = tmpInImageR[i][k];
			G = tmpInImageG[i][k];
			B = tmpInImageB[i][k];
			hsi = RGB2HSI(R, G, B);

			double H, S, I;
			H = hsi[0];
			S = hsi[1];
			I = hsi[2];
			tmpInImageH[i][k] = H;
			tmpInImageS[i][k] = S;
			tmpInImageI[i][k] = I;
			delete hsi;

		}

	//////////////////////

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double s_value = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			s_value = 0.0;
			for (int m = 0; m < MSIZE; m++) {
				for (int n = 0; n < MSIZE; n++) {
					s_value += tmpInImageI[i + m][k + n] * mask[m][n];
				}
			}
			tmpInImageI[i][k] = s_value;
		}
	}

	// 후처리 : I 값 보정
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpInImageI[i][k] += 127;
		}

	///////// HSI --> RGB /////////////
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			unsigned char* rgb;
			double H, S, I;

			H = tmpInImageH[i][k];
			S = tmpInImageS[i][k];
			I = tmpInImageI[i][k];

			rgb = HSI2RGB(H, S, I);

			tmpOutImageR[i][k] = rgb[0];
			tmpOutImageG[i][k] = rgb[1];
			tmpOutImageB[i][k] = rgb[2];

			delete rgb;

		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;
			}

			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;

			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;
			}

			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;

			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;
			}

			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;

			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}

		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);

	free2D_double(tmpInImageH, m_inH + b_MSIZE);
	free2D_double(tmpInImageS, m_inH + b_MSIZE);
	free2D_double(tmpInImageI, m_inH + b_MSIZE);

	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnLogImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	//////////////////
	// 화소 영역 처리
	//////////////////
	const int MSIZE = 5;
	double mask[MSIZE][MSIZE] = {
		{  0.0,  0.0, -1.0,  0.0,  0.0 },
		{  0.0, -1.0, -2.0, -1.0,  0.0 },
		{ -1.0, -2.0, 16.0, -2.0, -1.0 },
		{  0.0, -1.0, -2.0, -1.0,  0.0 },
		{  0.0,  0.0, -1.0,  0.0,  0.0 },
		
	};
	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);

	// 임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpInImageH = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageS = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageI = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);
	// 임시 입력영상의 초기화(128)
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;

	// 입력영상 --> 임시입력영상
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}

	///////// RGB --> HSI /////////////
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			double* hsi;
			unsigned char R, G, B;
			R = tmpInImageR[i][k];
			G = tmpInImageG[i][k];
			B = tmpInImageB[i][k];
			hsi = RGB2HSI(R, G, B);

			double H, S, I;
			H = hsi[0];
			S = hsi[1];
			I = hsi[2];
			tmpInImageH[i][k] = H;
			tmpInImageS[i][k] = S;
			tmpInImageI[i][k] = I;
			delete hsi;

		}

	//////////////////////

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double s_value = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			s_value = 0.0;
			for (int m = 0; m < MSIZE; m++) {
				for (int n = 0; n < MSIZE; n++) {
					s_value += tmpInImageI[i + m][k + n] * mask[m][n];
				}
			}
			tmpInImageI[i][k] = s_value;
		}
	}

	// 후처리 : I 값 보정
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpInImageI[i][k] += 127;
		}

	///////// HSI --> RGB /////////////
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			unsigned char* rgb;
			double H, S, I;

			H = tmpInImageH[i][k];
			S = tmpInImageS[i][k];
			I = tmpInImageI[i][k];

			rgb = HSI2RGB(H, S, I);

			tmpOutImageR[i][k] = rgb[0];
			tmpOutImageG[i][k] = rgb[1];
			tmpOutImageB[i][k] = rgb[2];

			delete rgb;

		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;
			}

			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;

			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;
			}

			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;

			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;
			}

			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;

			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}

		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);

	free2D_double(tmpInImageH, m_inH + b_MSIZE);
	free2D_double(tmpInImageS, m_inH + b_MSIZE);
	free2D_double(tmpInImageI, m_inH + b_MSIZE);

	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnDogImage7x7()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	//////////////////
	// 화소 영역 처리
	//////////////////
	const int MSIZE = 7;
	double mask[MSIZE][MSIZE] = {
		{   0.0,  0.0, -1.0, -1.0, -1.0,  0.0,  0.0 },
		{   0.0, -2.0, -3.0, -3.0, -3.0, -2.0,  0.0 },
		{  -1.0, -3.0,  5.0,  5.0,  5.0, -3.0, -1.0 },
		{  -1.0, -3.0,  5.0, 16.0,  5.0, -3.0, -1.0 },
		{  -1.0, -3.0,  5.0,  5.0,  5.0, -3.0, -1.0 },
		{   0.0, -2.0, -3.0, -3.0, -3.0, -2.0,  0.0 },
		{   0.0,  0.0, -1.0, -1.0, -1.0,  0.0,  0.0 }
	};
	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);

	// 임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpInImageH = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageS = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageI = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);
	// 임시 입력영상의 초기화(128)
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;

	// 입력영상 --> 임시입력영상
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}

	///////// RGB --> HSI /////////////
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			double* hsi;
			unsigned char R, G, B;
			R = tmpInImageR[i][k];
			G = tmpInImageG[i][k];
			B = tmpInImageB[i][k];
			hsi = RGB2HSI(R, G, B);

			double H, S, I;
			H = hsi[0];
			S = hsi[1];
			I = hsi[2];
			tmpInImageH[i][k] = H;
			tmpInImageS[i][k] = S;
			tmpInImageI[i][k] = I;
			delete hsi;

		}

	//////////////////////

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double s_value = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			s_value = 0.0;
			for (int m = 0; m < MSIZE; m++) {
				for (int n = 0; n < MSIZE; n++) {
					s_value += tmpInImageI[i + m][k + n] * mask[m][n];
				}
			}
			tmpInImageI[i][k] = s_value;
		}
	}

	// 후처리 : I 값 보정
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpInImageI[i][k] += 127;
		}

	///////// HSI --> RGB /////////////
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			unsigned char* rgb;
			double H, S, I;

			H = tmpInImageH[i][k];
			S = tmpInImageS[i][k];
			I = tmpInImageI[i][k];

			rgb = HSI2RGB(H, S, I);

			tmpOutImageR[i][k] = rgb[0];
			tmpOutImageG[i][k] = rgb[1];
			tmpOutImageB[i][k] = rgb[2];

			delete rgb;

		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;
			}

			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;

			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;
			}

			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;

			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;
			}

			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;

			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}

		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);

	free2D_double(tmpInImageH, m_inH + b_MSIZE);
	free2D_double(tmpInImageS, m_inH + b_MSIZE);
	free2D_double(tmpInImageI, m_inH + b_MSIZE);

	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}


void CColorAlpha1Doc::OnDogImage9x9()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_inImageR == NULL) {
		return;
	}

	free2D(m_outImageR, m_outH);
	free2D(m_outImageG, m_outH);
	free2D(m_outImageB, m_outH);
	// 출력 영상의 메모리 할당
	// (중요!) 출력 영상의 크기를 결정 --> 알고리즘에 의존.
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당(배열 선언하기)
	m_outImageR = malloc2D(m_outH, m_outW);
	m_outImageG = malloc2D(m_outH, m_outW);
	m_outImageB = malloc2D(m_outH, m_outW);

	//////////////////
	// 화소 영역 처리
	//////////////////
	const int MSIZE = 9;
	double mask[MSIZE][MSIZE] = {
			{ 0.0,  0.0,  0.0, -1.0, -1.0, -1.0,  0.0,  0.0,  0.0 },
			{ 0.0, -2.0, -3.0, -3.0, -3.0, -3.0, -3.0, -2.0,  0.0 },
			{ 0.0, -3.0, -2.0, -1.0, -1.0, -1.0, -2.0, -3.0,  0.0 },
			{ 0.0, -3.0, -1.0,  9.0,  9.0,  9.0, -1.0, -3.0,  0.0 },
			{-1.0, -3.0, -1.0,  9.0, 19.0,  9.0, -1.0, -3.0, -1.0 },
			{ 0.0, -3.0, -1.0,  9.0,  9.0,  9.0, -1.0, -3.0,  0.0 },
			{ 0.0, -3.0, -2.0, -1.0, -1.0, -1.0, -2.0, -3.0,  0.0 },
			{ 0.0, -2.0, -3.0, -3.0, -3.0, -3.0, -3.0, -2.0,  0.0 },
			{ 0.0,  0.0,  0.0, -1.0, -1.0, -1.0,  0.0,  0.0,  0.0 }
	};
	int b_MSIZE = (int)(MSIZE / 2) * 2;
	int s_MSIZE = (int)(MSIZE / 2);

	// 임시 메모리 할당
	double** tmpInImageR = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageG = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageB = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpInImageH = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageS = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);
	double** tmpInImageI = malloc2D_double(m_inH + b_MSIZE, m_inW + b_MSIZE);

	double** tmpOutImageR = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageG = malloc2D_double(m_outH, m_outW);
	double** tmpOutImageB = malloc2D_double(m_outH, m_outW);
	// 임시 입력영상의 초기화(128)
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 128.0;

	// 입력영상 --> 임시입력영상
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + s_MSIZE][k + s_MSIZE] = m_inImageR[i][k];
			tmpInImageG[i + s_MSIZE][k + s_MSIZE] = m_inImageG[i][k];
			tmpInImageB[i + s_MSIZE][k + s_MSIZE] = m_inImageB[i][k];
		}

	///////// RGB --> HSI /////////////
	for (int i = 0; i < m_inH + b_MSIZE; i++)
		for (int k = 0; k < m_inW + b_MSIZE; k++) {
			double* hsi;
			unsigned char R, G, B;
			R = tmpInImageR[i][k];
			G = tmpInImageG[i][k];
			B = tmpInImageB[i][k];
			hsi = RGB2HSI(R, G, B);

			double H, S, I;
			H = hsi[0];
			S = hsi[1];
			I = hsi[2];
			tmpInImageH[i][k] = H;
			tmpInImageS[i][k] = S;
			tmpInImageI[i][k] = I;
			delete hsi;

		}

	//////////////////////

	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.
	double s_value = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {

			s_value = 0.0;
			for (int m = 0; m < MSIZE; m++) {
				for (int n = 0; n < MSIZE; n++) {
					s_value += tmpInImageI[i + m][k + n] * mask[m][n];
				}
			}
			tmpInImageI[i][k] = s_value;
		}
	}

	// 후처리 : I 값 보정
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			tmpInImageI[i][k] += 127;
		}

	///////// HSI --> RGB /////////////
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			unsigned char* rgb;
			double H, S, I;

			H = tmpInImageH[i][k];
			S = tmpInImageS[i][k];
			I = tmpInImageI[i][k];

			rgb = HSI2RGB(H, S, I);

			tmpOutImageR[i][k] = rgb[0];
			tmpOutImageG[i][k] = rgb[1];
			tmpOutImageB[i][k] = rgb[2];

			delete rgb;

		}

	// 임시 출력영상 --> 출력 영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			//R
			if (tmpOutImageR[i][k] < 0.0) {
				m_outImageR[i][k] = 0;
			}

			else if (tmpOutImageR[i][k] > 255.0) {
				m_outImageR[i][k] = 255;

			}
			else {
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];
			}
			//G
			if (tmpOutImageG[i][k] < 0.0) {
				m_outImageG[i][k] = 0;
			}

			else if (tmpOutImageG[i][k] > 255.0) {
				m_outImageG[i][k] = 255;

			}
			else {
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];
			}
			//B
			if (tmpOutImageB[i][k] < 0.0) {
				m_outImageB[i][k] = 0;
			}

			else if (tmpOutImageB[i][k] > 255.0) {
				m_outImageB[i][k] = 255;

			}
			else {
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}

		}
	}

	free2D_double(tmpInImageR, m_inH + b_MSIZE);
	free2D_double(tmpInImageG, m_inH + b_MSIZE);
	free2D_double(tmpInImageB, m_inH + b_MSIZE);

	free2D_double(tmpInImageH, m_inH + b_MSIZE);
	free2D_double(tmpInImageS, m_inH + b_MSIZE);
	free2D_double(tmpInImageI, m_inH + b_MSIZE);

	free2D_double(tmpOutImageR, m_outH);
	free2D_double(tmpOutImageG, m_outH);
	free2D_double(tmpOutImageB, m_outH);
}






