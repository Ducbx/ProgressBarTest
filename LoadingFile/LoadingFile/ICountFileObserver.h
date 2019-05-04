#pragma once

class ICountFileObserver
{
public:
	virtual void OnCountFileChanged(int currentCount, bool& shouldFinished) = 0;
};