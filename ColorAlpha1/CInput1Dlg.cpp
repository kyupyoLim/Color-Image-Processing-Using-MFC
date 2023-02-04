// CInput1Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "ColorAlpha1.h"
#include "CInput1Dlg.h"
#include "afxdialogex.h"


// CInput1Dlg 대화 상자

IMPLEMENT_DYNAMIC(CInput1Dlg, CDialog)

CInput1Dlg::CInput1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_INPUT1, pParent)
	, m_Input1(0)
{

}

CInput1Dlg::~CInput1Dlg()
{
}

void CInput1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INPUT1, m_Input1);
}


BEGIN_MESSAGE_MAP(CInput1Dlg, CDialog)
END_MESSAGE_MAP()


// CInput1Dlg 메시지 처리기
