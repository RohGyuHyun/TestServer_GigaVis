#pragma once

class CSharedMemoryPush
{
protected:
	typedef void(*CALLBACK_FUNCTION_Event)(int, int);
	CCriticalSection* m_Critcal;
public:
	CSharedMemoryPush();
	~CSharedMemoryPush();

	BOOL SharedMemoryPush();

	int m_nPushIdx;
	CString m_strReadImagePath;
	int *m_piMemory;
	int* m_piFirstMemory;
	queue < CString> m_strReadFilePath;
	queue<Mat> m_queImage;
	HANDLE m_hHandle;
	void ReleasQue();

	BOOL m_bThreadEnd;
	int m_nThreadDelayTime;
	CALLBACK_FUNCTION_Event callEventfunc;

	void SetCritcalSection(CCriticalSection* Critcal) { m_Critcal = Critcal; };
	void SetCritcalSection(BOOL isSet);
	void SetCallBack(CALLBACK_FUNCTION_Event event) { callEventfunc = event; };
};

class CSharedMemoryPop
{
protected:
	typedef void(*CALLBACK_FUNCTION_Event)(int, int);
	CCriticalSection* m_Critcal;
public:
	CSharedMemoryPop();
	~CSharedMemoryPop();
	BOOL SharedMemoryPop();

	int m_nPopIdx;
	int* m_piMemory;
	int* m_piFirstMemory;
	queue<Mat> m_queImage;
	Mat m_PopImg;
	BOOL m_bSendReady;
	HANDLE m_hHandle;
	void ReleasQue();

	BOOL m_bThreadEnd;
	int m_nThreadDelayTime;
	CALLBACK_FUNCTION_Event callEventfunc;

	void SetCritcalSection(CCriticalSection* Critcal) { m_Critcal = Critcal; };
	void SetCritcalSection(BOOL isSet);
	void SetCallBack(CALLBACK_FUNCTION_Event event) { callEventfunc = event; };
	int m_nMaxSendCount;
};
