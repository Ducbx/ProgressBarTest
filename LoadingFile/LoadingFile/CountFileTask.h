#pragma once
#include "ICountFileObserver.h"
#include "LoadingFileDlg.h"

//struct CountFileTask
//{
//	CString folderPath;
//	ICountFileObserver* sender;
//
//	CountFileTask(ICountFileObserver *dlg, CString folderPath)
//	{
//		this->folderPath = folderPath;
//		this->sender = dlg;
//	}
//};

struct CountFileTask
{
	CString folderPath;
	CLoadingFileDlg* sender;

	CountFileTask(CLoadingFileDlg *dlg, CString folderPath)
	{
		this->folderPath = folderPath;
		this->sender = dlg;
	}
};

struct SCountFile{	CString m_strFolderPath;	HWND m_hwndLoadingDlg;	HWND m_hwndProgressDlg;	SCountFile(CString strFolderPath, HWND hwndLoadingDlg, HWND hwndProgressDlg)	{		this->m_strFolderPath = strFolderPath;		this->m_hwndLoadingDlg = hwndLoadingDlg;		this->m_hwndProgressDlg = hwndProgressDlg;	}};