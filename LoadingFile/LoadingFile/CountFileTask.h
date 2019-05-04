#pragma once
#include "ICountFileObserver.h"

struct CountFileTask
{
	CString folderPath;
	ICountFileObserver* sender;

	CountFileTask(ICountFileObserver *dlg, CString folderPath)
	{
		this->folderPath = folderPath;
		this->sender = dlg;
	}
};