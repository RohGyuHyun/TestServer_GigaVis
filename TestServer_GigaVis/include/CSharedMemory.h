#pragma once

class CSharedMemoryPush
{
public:
	CSharedMemoryPush();
	~CSharedMemoryPush();

	BOOL SharedMemoryPush();

	int m_nPushIdx;
	CString m_strReadImagePath;
	int *m_piMemory;
	queue < CString> m_strReadFilePath;
};

class CSharedMemoryPop
{
public:
	CSharedMemoryPop();
	~CSharedMemoryPop();
	BOOL SharedMemoryPop();

	int* m_piMemory;
	queue<Mat> m_queImage;
	Mat m_PopImg;
};

class CSharedMemory : public CSharedMemoryPush, public CSharedMemoryPop
{
protected:
	typedef void(*CALLBACK_FUNCTION_Event)(int);
	CCriticalSection m_Critcal;
	

public:
	CSharedMemory();
	~CSharedMemory();

	HANDLE m_hHandle;
	queue<Mat> m_queImage;
	BOOL m_bThreadEnd;
	int m_nThreadDelayTime;
	CALLBACK_FUNCTION_Event callEventfunc;

	void SetCritcalSection(BOOL isSet);
	void SetCallBack(CALLBACK_FUNCTION_Event event) { callEventfunc = event; };
};

