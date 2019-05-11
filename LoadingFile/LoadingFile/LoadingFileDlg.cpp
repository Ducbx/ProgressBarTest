
// LoadingFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoadingFile.h"
#include "LoadingFileDlg.h"
#include "afxdialogex.h"
#include "Utilities.h"
#include "ProgressDlg.h"
#include "MessageDefine.h"
#include "CountFileTask.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLoadingFileDlg dialog



CLoadingFileDlg::CLoadingFileDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CLoadingFileDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strFolderPath = "";
	m_iFileTotal = 0;
	m_iFileCurrentNumber = 0;
	m_pMyThread = NULL;
	m_pProgressHWND = NULL;
}
//CEvent* CLoadingFileDlg::m_WaitThread = new CEvent(TRUE, TRUE);
HANDLE CLoadingFileDlg::m_hCancel = CreateEvent(
	NULL,               // default security attributes
	TRUE,               // manual-reset event
	FALSE,              // initial state is nonsignaled
	TEXT("WriteEvent")  // object name
	);

void CLoadingFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PATH, m_editPath);
	DDX_Control(pDX, IDC_BTN_OPEN, m_btnOpen);
	DDX_Control(pDX, IDC_STATIC_TOTAL_FILE, m_staticTotalFile);
	DDX_Control(pDX, IDC_BTN_CAL, m_btnCal);
}

BEGIN_MESSAGE_MAP(CLoadingFileDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN, &CLoadingFileDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_CAL, &CLoadingFileDlg::OnBnClickedBtnCal)
	ON_MESSAGE(UWM_SENDHWND2PARENT, OnReceiveHWNDMessages)
	ON_MESSAGE(UWM_SENDCANCELSIGNAL, OnReceiveCancelSignalMessages)
	ON_MESSAGE(UWM_SENDMSGUPDATEMAINDLG, OnReceiveUpdateMainDlgMessages)
	//ON_MESSAGE(UWM_SENDHWND2PARENT, OnReceiveHWNDMessagesTest)
END_MESSAGE_MAP()


// CLoadingFileDlg message handlers

BOOL CLoadingFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_pLoadingHWND = this->GetSafeHwnd();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLoadingFileDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLoadingFileDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLoadingFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLoadingFileDlg::OnBnClickedBtnOpen()
{
	m_iFileTotal = 0;

	CFolderPickerDialog m_dlg;
	m_dlg.m_ofn.lpstrTitle = _T("Put your title here");
	m_dlg.m_ofn.lpstrInitialDir = _T("C:\\");
	if (m_dlg.DoModal() == IDOK) {
		m_strFolderPath = m_dlg.GetPathName();

		CountFile();
		CString strTemp;
		strTemp.Format(L"%d", m_iFileTotal);
		m_staticTotalFile.SetWindowTextW(strTemp);

		m_editPath.SetWindowTextW(m_strFolderPath);
		UpdateData(FALSE);   // To show updated folder in GUI
	}
}

void CLoadingFileDlg::CountFile()
{
	Utilities::CountFile(m_strFolderPath, m_iFileTotal);
}


void CLoadingFileDlg::OnBnClickedBtnCal()
{
	::ResetEvent(m_hCancel);
	m_iFileCurrentNumber = 0;
	if (m_strFolderPath == "")
	{
		MessageBox(L"Select folder before calculate");
	}
	else if (m_iFileTotal == 0)
	{
		MessageBox(L"No file in this folder");
	}
	else
	{
		CProgressDlg progressDlg(m_iFileTotal, this);
		progressDlg.DoModal();
	}
}


LRESULT CLoadingFileDlg::OnReceiveHWNDMessages(WPARAM wParam, LPARAM lParam)
{
	HWND temp = (HWND)wParam;
	m_pProgressHWND = temp;

	// start a count file task on other thread
	m_iFileCurrentNumber = 0;
	CountFileTask* task = new CountFileTask(this, m_strFolderPath);
	m_pMyThread = AfxBeginThread(ExecuteMyThread, task);
	return 0L;
}


UINT CLoadingFileDlg::ExecuteMyThread(LPVOID pParam)
{
	//send value progress
	CountFileTask *task = (CountFileTask*)pParam;
	int count = 0;
	CountFileDelay(task->folderPath, count, task->sender);
	return 0;
}

void CLoadingFileDlg::CountFileDelay(CString path, int& nCount, CLoadingFileDlg* observer)
{
	CString strRootFolderPath = path + _T("\\*.*");
	CFileFind cfFinder;
	CString strFilePath;

	BOOL bIsOk = cfFinder.FindFile(strRootFolderPath);
	while (bIsOk)
	{
		if (WaitForSingleObject(m_hCancel, 0) == WAIT_OBJECT_0)
		{
			break;
		}

		//DWORD stat = ::WaitForMultipleObjects(2, waitEvents, FALSE, 10);
		//switch (stat)
		//{
		//case WAIT_OBJECT_0 + 0:
		//	// stopevent CEvent is signalled; proceed to do some work
		//	// Set event
		//	m_WaitThread->SetEvent();
		//	return;

		//case WAIT_OBJECT_0 + 1:
		//case WAIT_FAILED:
		//case WAIT_TIMEOUT:
		//default:
		//	break;
		//}

		bIsOk = cfFinder.FindNextFile();
		strFilePath = cfFinder.GetFilePath();
		if (cfFinder.IsDirectory() && !cfFinder.IsDots())
		{
			strFilePath = cfFinder.GetFilePath();
			CountFileDelay(strFilePath, nCount, observer);
			continue;
		}
		if (!cfFinder.IsDots())
		{
			nCount++;
			bool shouldFinished = false;
			observer->OnCountFileChanged(nCount, shouldFinished);
			/*if (shouldFinished)
			{
				return;
			}*/
		}
	}
	cfFinder.Close();
}

void CLoadingFileDlg::OnCountFileChanged(int nCount, bool& shouldFinished)
{
	//send message to update progressBar in childDlg
	::SendMessage(m_pProgressHWND, UWM_SENDCALCULATEVAL, (WPARAM)nCount, 0);

	//send message to update value in Main Dialog
	::SendMessage(this->GetSafeHwnd(), UWM_SENDMSGUPDATEMAINDLG, (WPARAM)nCount, 0);

	//Check if the task should be stop
	//shouldFinished = (nCount == m_iFileTotal) || m_bCancel;
}

LRESULT CLoadingFileDlg::OnReceiveCancelSignalMessages(WPARAM wParam, LPARAM lParam)
{
	::SetEvent(m_hCancel);

	//Wait until thread finished
	//WaitForSingleObject(m_WaitThread->m_hObject, INFINITE);
	//Sleep(5000);

	return 0L;
}

LRESULT CLoadingFileDlg::OnReceiveUpdateMainDlgMessages(WPARAM wParam, LPARAM lParam)
{
	int temp = (int)wParam;
	CString strTemp;
	strTemp.Format(L"%d", temp);
	m_staticTotalFile.SetWindowTextW(strTemp);

	m_iFileCurrentNumber = temp;

	return 0L;
}

//LRESULT CLoadingFileDlg::OnReceiveHWNDMessagesTest(WPARAM wParam, LPARAM lParam)//{//	HWND hwndTemp = (HWND)wParam;//	m_pProgressHWND = hwndTemp;//	 //start a count file task on other thread//	SCountFile* pTask = new SCountFile(m_strFolderPath, (this->GetSafeHwnd()), m_pProgressHWND);//	m_pMyThread = AfxBeginThread(ExecuteMyThreadTest, pTask);//	return 0L;//}LRESULT CLoadingFileDlg::OnReceiveHWNDMessagesTest(WPARAM wParam, LPARAM lParam){	HWND hwndTemp = (HWND)wParam;	m_pProgressHWND = hwndTemp;	SCountFile *pTask = new SCountFile(m_strFolderPath, m_pLoadingHWND, m_pProgressHWND);	m_pMyThread = AfxBeginThread(ExecuteMyThreadTest, pTask);	return 0L;}UINT CLoadingFileDlg::ExecuteMyThreadTest(LPVOID pParam){	//count and send value progress	SCountFile *pTask = (SCountFile*)pParam;	int iCount = 0;	HandleFileTest(pTask->m_strFolderPath, iCount, pTask->m_hwndLoadingDlg, pTask->m_hwndProgressDlg);	return 0;}void CLoadingFileDlg::HandleFileTest(CString strPath, int& iCount, HWND hwndLoadingDlg, HWND hwndProgressDlg){	CString strRootFolderPath = strPath + _T("\\*.*");	CFileFind cfFinder;	CString strFilePath;	BOOL bShouldFinished = FALSE;	BOOL bIsOk = cfFinder.FindFile(strRootFolderPath);	while (bIsOk)	{		bIsOk = cfFinder.FindNextFile();		strFilePath = cfFinder.GetFilePath();		if (cfFinder.IsDirectory() && !cfFinder.IsDots())		{			strFilePath = cfFinder.GetFilePath();			HandleFileTest(strFilePath, iCount, hwndLoadingDlg, hwndProgressDlg);			continue;		}		if (!cfFinder.IsDots())		{			iCount++;			//send message to update progressBar in childDlg			::SendMessage(hwndProgressDlg, UWM_SENDCALCULATEVAL, (WPARAM)iCount, 0);			//send message to update value in Main Dialog			::SendMessage(hwndLoadingDlg, UWM_SENDMSGUPDATEMAINDLG, (WPARAM)iCount, 0);			/*if (m_bCancelTest)			{				return;			}*/		}	}	cfFinder.Close();	return;}

