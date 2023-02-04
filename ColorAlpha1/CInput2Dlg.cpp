// CInput2Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "ColorAlpha1.h"
#include "CInput2Dlg.h"
#include "afxdialogex.h"


// CInput2Dlg 대화 상자

IMPLEMENT_DYNAMIC(CInput2Dlg, CDialog)

CInput2Dlg::CInput2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_INPUT2, pParent)
	, m_Input1(0)
	, m_Input2(0)
{

}

CInput2Dlg::~CInput2Dlg()
{
}

void CInput2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Input1);
	DDX_Text(pDX, IDC_EDIT2, m_Input2);
}


BEGIN_MESSAGE_MAP(CInput2Dlg, CDialog)
END_MESSAGE_MAP()


// CInput2Dlg 메시지 처리기
