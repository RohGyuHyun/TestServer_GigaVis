
// TestServer_GigaVisDlg.h: 헤더 파일
//

#pragma once
#include "sversok.h"
#include "logFile.h"
#include "CSharedMemory.h"

#define MAX_CHAR_LENG			1024

// CTestServerGigaVisDlg 대화 상자
class CTestServerGigaVisDlg : public CDialogEx
{
// 생성입니다.
public:
	CTestServerGigaVisDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTSERVER_GIGAVIS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	CSeverSock* m_Server;
	CClientSock* m_Client;
	BOOL m_bServerEnd;
	CLogFile* m_SendLog;
	CLogFile* m_RcvLog;

	CWinThread* m_pServerThread[2];
	UINT static Thread0(LPVOID pParam);
	UINT static Thread1(LPVOID pParam);


	CSharedMemory *m_PushMem;
	CSharedMemory *m_PopMem;

	Mat m_SendImg;

	CCriticalSection m_Critcal;

	int m_nTestIdx;
private:
	BOOL SendCliendMessage(void* pData);
	BOOL InitThread(int nIdx);
	BOOL EndThread(int nIdx);
public:
	void WriteClient();
	void ReadStop();


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List_SendLog;
	CListCtrl m_List_RcvLog;
	CString m_Edit_strImagePath;
	int m_Edit_nThreadDelay0;
	int m_Edit_nThreadDelay1;
	virtual BOOL DestroyWindow();
	afx_msg LRESULT OnAccept(WPARAM, LPARAM);
	afx_msg LRESULT OnClose(WPARAM, LPARAM);
	afx_msg LRESULT OnReceive(WPARAM, LPARAM);
	afx_msg void OnBnClickedButtonSelectImagePath();
	afx_msg void OnBnClickedButtonThreadDelaySet();
	afx_msg void OnBnClickedButtonThreadStart();
	afx_msg void OnBnClickedButtonThreadStop();
	afx_msg void OnBnClickedButtonThreadPause();
	afx_msg void OnBnClickedButtonThreadResume();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
