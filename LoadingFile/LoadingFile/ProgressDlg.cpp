// ProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoadingFile.h"
#include "ProgressDlg.h"
#include "afxdialogex.h"
#include "MessageDefine.h"

// CProgressDlg dialog

IMPLEMENT_DYNAMIC(CProgressDlg, CDialogEx)

CProgressDlg::CProgressDlg(int totalFileCount, CWnd* pParent /*=NULL*/)
	: CDialogEx(CProgressDlg::IDD, pParent)
{
	m_iTotalFileNumber = totalFileCount;
	m_pProgressHWND = NULL;
}

CProgressDlg::~CProgressDlg()
{
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_STATUS, m_progressStatus);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_STATIC_RATE, m_staticRate);
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CProgressDlg::OnBnClickedBtnCancel)
	ON_MESSAGE(UWM_SENDCALCULATEVAL, OnReceiveValMessages)
END_MESSAGE_MAP()


// CProgressDlg message handlers


BOOL CProgressDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	m_pProgressHWND = this->GetSafeHwnd();

	::SendMessage(GetParent()->GetSafeHwnd(), UWM_SENDHWND2PARENT, (WPARAM)m_pProgressHWND, 0);

	m_progressStatus.SetRange(0, m_iTotalFileNumber);
	// TODO: Add extra initialization here
	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CProgressDlg::OnBnClickedBtnCancel()
{
	::SendMessage(GetParent()->GetSafeHwnd(), UWM_SENDCANCELSIGNAL, 0, 0);

	this->OnCancel();
}

LRESULT CProgressDlg::OnReceiveValMessages(WPARAM wParam, LPARAM lParam)
{
	int temp = (int)wParam;
	m_progressStatus.SetPos(temp);
	int rate = 100*temp / m_iTotalFileNumber;

	CString strTemp;
	strTemp.Format(L"%d", rate);
	strTemp += "%";
	m_staticRate.SetWindowTextW(strTemp);

	if (temp == m_iTotalFileNumber)
	{
		this->OnOK();
	}

	return 0L;
}