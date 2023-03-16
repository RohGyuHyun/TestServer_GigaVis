#pragma once

//Image Data Load -> 공유메모리 Push Class
class CSharedMemoryPush
{
protected:
	CCriticalSection* m_Critcal;
	queue<Mat>* m_queImage;

public:
	CSharedMemoryPush();
	~CSharedMemoryPush();

	BOOL SharedMemoryPush();

	queue < CString> m_strReadFilePath;
	CString m_strReadImagePath;

	BOOL m_bThreadEnd;//Thread End Flag
	int m_nThreadDelayTime;//Thread Sleep Time

	void SetCritcalSection(CCriticalSection* Critcal) { m_Critcal = Critcal; };
	void SetCritcalSection(BOOL isSet);
	void SetImageQueue(queue<Mat>* que) { m_queImage = que; };
};

//공유메모리 Pop -> Image Data Client Send Class
class CSharedMemoryPop
{
protected:
	typedef void(*CALLBACK_FUNCTION_Event)(int, Mat);
	CCriticalSection* m_Critcal;
	queue<Mat>* m_queImage;

public:
	CSharedMemoryPop();
	~CSharedMemoryPop();
	BOOL SharedMemoryPop();

	
	BOOL m_bSendReady;

	BOOL m_bThreadEnd;//Thread End Flag
	int m_nThreadDelayTime;//Thread Sleep Time
	CALLBACK_FUNCTION_Event callEventfunc;

	void SetCritcalSection(CCriticalSection* Critcal) { m_Critcal = Critcal; };
	void SetCritcalSection(BOOL isSet);
	void SetCallBack(CALLBACK_FUNCTION_Event event) { callEventfunc = event; };
	void SetImageQueue(queue<Mat>* que) { m_queImage = que; };
	int GetQueImageCount() { return (int)m_queImage->size(); };
};
