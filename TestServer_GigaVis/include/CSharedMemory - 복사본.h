#pragma once

//Image Data Load -> 공유메모리 Push Class
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
	int *m_piMemory;//Push SharedMemory Address
	int* m_piFirstMemory;//First SharedMemory Address
	queue < CString> m_strReadFilePath;
	queue<Mat> m_queImage;
	HANDLE m_hHandle;
	void ReleasQue();//Que Image Data Releas

	BOOL m_bThreadEnd;//Thread End Flag
	int m_nThreadDelayTime;//Thread Sleep Time
	CALLBACK_FUNCTION_Event callEventfunc;

	void SetCritcalSection(CCriticalSection* Critcal) { m_Critcal = Critcal; };
	void SetCritcalSection(BOOL isSet);
	void SetCallBack(CALLBACK_FUNCTION_Event event) { callEventfunc = event; };
};

//공유메모리 Pop -> Image Data Client Send Class
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
	int* m_piMemory;//Pop SharedMemory Address
	int* m_piFirstMemory;//First SharedMemory Address
	queue<Mat> m_queImage;
	Mat m_PopImg;
	BOOL m_bSendReady;
	HANDLE m_hHandle;
	void ReleasQue();//Que Image Data Releas

	BOOL m_bThreadEnd;//Thread End Flag
	int m_nThreadDelayTime;//Thread Sleep Time
	CALLBACK_FUNCTION_Event callEventfunc;

	void SetCritcalSection(CCriticalSection* Critcal) { m_Critcal = Critcal; };
	void SetCritcalSection(BOOL isSet);
	void SetCallBack(CALLBACK_FUNCTION_Event event) { callEventfunc = event; };
	int m_nMaxSendCount;
};
