#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CProgressDlg dialog

class CProgressDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProgressDlg)

public:
	CProgressDlg(int totalFileCount, CWnd* pParent = NULL);   // standard constructor
	virtual ~CProgressDlg();

// Dialog Data
	enum { IDD = IDD_PROGRESS_DIALOG };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	CProgressCtrl m_progressStatus;
	CButton m_btnCancel;
private:
	int m_iTotalFileNumber;
	HWND m_pProgressHWND;
public:
	afx_msg void OnBnClickedBtnCancel();
	afx_msg LRESULT OnReceiveValMessages(WPARAM wParam, LPARAM lParam);
protected:
	CStatic m_staticRate;
};
