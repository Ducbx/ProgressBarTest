#include<iostream>
#include "afxwin.h"

class Utilities
{
public:
	static void CountFile(CString path, int &nCount)
	{
		CString strRootFolderPath = path + _T("\\*.*");
		CFileFind cfFinder;
		CString strFilePath;
		bool returnValue = false;

		BOOL bIsOk = cfFinder.FindFile(strRootFolderPath);
		while (bIsOk)
		{
			bIsOk = cfFinder.FindNextFile();
			strFilePath = cfFinder.GetFilePath();
			if (cfFinder.IsDirectory() && !cfFinder.IsDots())
			{
				strFilePath = cfFinder.GetFilePath();
				CountFile(strFilePath, nCount);
				continue;
			}
			if (!cfFinder.IsDots())
			{
				nCount++;
			}
		}
		cfFinder.Close();
	}
};