
// LoadingFileDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "ICountFileObserver.h"

// CLoadingFileDlg dialog
class CLoadingFileDlg : public CDialogEx
{
// Construction
public:
	CLoadingFileDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LOADINGFILE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	CEdit m_editPath;
	CButton m_btnOpen;
	CStatic m_staticTotalFile;
	CButton m_btnCal;
private:
	CString m_strFolderPath;
	int m_iFileTotal;
	int m_iFileCurrentNumber;
	CWinThread* m_pMyThread;
	HWND m_pProgressHWND;
	HWND m_pLoadingHWND;

public:
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnCal();
	afx_msg LRESULT OnReceiveHWNDMessages(WPARAM, LPARAM);
	afx_msg LRESULT OnReceiveHWNDMessagesTest(WPARAM , LPARAM );
	afx_msg LRESULT OnReceiveCancelSignalMessages(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceiveUpdateMainDlgMessages(WPARAM wParam, LPARAM lParam);

public:
	void CountFile();
	static UINT ExecuteMyThread(LPVOID pParam);
	static UINT ExecuteMyThreadTest(LPVOID pParam);
	static void CountFileDelay(CString path, int &iCount, CLoadingFileDlg* dialog);
	
	void OnCountFileChanged(int nCount, bool& shouldFinished);
	static void HandleFileTest(CString strPath, int& iCount, HWND hwndLoadingDlg, HWND hwndProgressDlg);
private:
	static HANDLE m_hCancel;
};
