
// TestServer_GigaVisDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "TestServer_GigaVis.h"
#include "TestServer_GigaVisDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
CTestServerGigaVisDlg* g_This;
void SharedMmemoryCalbackFunc(int nType, Mat img)
{
	switch (nType)
	{
	case 0:
		g_This->WriteClient(img);
		break;
	}
}

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestServerGigaVisDlg 대화 상자



CTestServerGigaVisDlg::CTestServerGigaVisDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TESTSERVER_GIGAVIS_DIALOG, pParent)
	, m_Edit_strImagePath(_T(""))
	, m_Edit_nThreadDelay0(1000)
	, m_Edit_nThreadDelay1(1000)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bServerEnd = FALSE;
	m_Server = NULL;
	m_Client = NULL;
}

void CTestServerGigaVisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SEND_LOG, m_List_SendLog);
	DDX_Control(pDX, IDC_LIST_RCV_LOG, m_List_RcvLog);
	DDX_Text(pDX, IDC_EDIT_IMAGE_PATH, m_Edit_strImagePath);
	DDX_Text(pDX, IDC_EDIT_THREAD_DELAY_1, m_Edit_nThreadDelay0);
	DDX_Text(pDX, IDC_EDIT_THREAD_DELAY_2, m_Edit_nThreadDelay1);
}

BEGIN_MESSAGE_MAP(CTestServerGigaVisDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_ACCEPT_SOCKET, OnAccept)
	ON_MESSAGE(WM_RECEIVE_DATA, OnReceive)
	ON_MESSAGE(WM_CLOSE_SOCK, OnClose)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_IMAGE_PATH, &CTestServerGigaVisDlg::OnBnClickedButtonSelectImagePath)
	ON_BN_CLICKED(IDC_BUTTON_THREAD_DELAY_SET_1, &CTestServerGigaVisDlg::OnBnClickedButtonThreadDelaySet)
	ON_BN_CLICKED(IDC_BUTTON_THREAD_START_1, &CTestServerGigaVisDlg::OnBnClickedButtonThreadStart)
	ON_BN_CLICKED(IDC_BUTTON_THREAD_STOP_1, &CTestServerGigaVisDlg::OnBnClickedButtonThreadStop)
	ON_BN_CLICKED(IDC_BUTTON_THREAD_PAUSE_1, &CTestServerGigaVisDlg::OnBnClickedButtonThreadPause)
	ON_BN_CLICKED(IDC_BUTTON_THREAD_RESUME_1, &CTestServerGigaVisDlg::OnBnClickedButtonThreadResume)
	ON_BN_CLICKED(IDC_BUTTON_THREAD_DELAY_SET_2, &CTestServerGigaVisDlg::OnBnClickedButtonThreadDelaySet)
	ON_BN_CLICKED(IDC_BUTTON_THREAD_START_2, &CTestServerGigaVisDlg::OnBnClickedButtonThreadStart)
	ON_BN_CLICKED(IDC_BUTTON_THREAD_STOP_2, &CTestServerGigaVisDlg::OnBnClickedButtonThreadStop)
	ON_BN_CLICKED(IDC_BUTTON_THREAD_PAUSE_2, &CTestServerGigaVisDlg::OnBnClickedButtonThreadPause)
	ON_BN_CLICKED(IDC_BUTTON_THREAD_RESUME_2, &CTestServerGigaVisDlg::OnBnClickedButtonThreadResume)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTestServerGigaVisDlg 메시지 처리기

BOOL CTestServerGigaVisDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	g_This = this;
	AfxSocketInit();
	m_Server = new CSeverSock;
	BOOL chk;
	chk = TRUE;
	CString strText, packet;
	delete m_Server;
	//서버 소켓을 만든다.
	m_Server = new CSeverSock;
	AfxSocketInit();
	if (!m_Server->Create(5000))
	{
		chk = FALSE;
		strText.Format(_T("Error : TCP/IP server create fail!"));
		AfxMessageBox(strText);
		return TRUE;
	}

	m_Server->SetWnd(this->m_hWnd);
	//클라이언트가 오기를 기다린다.
	if (!m_Server->Listen())
	{
		chk = FALSE;
		strText.Format(_T("Error : TCP/IP server listen fail!"));
		AfxMessageBox(strText);
		return TRUE;
	}

	if (chk)
	{
		strText.Format(_T("TCP/IP server open OK!"));
	}


	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
	strText.Format(_T("%s\\Log\\Send\\"), strTemp);
	m_SendLog = new CLogFile(strText, _T("Send.log"), &m_List_SendLog, _T("SendMessage"), 500, 50, TRUE);

	strText.Format(_T("%s\\Log\\Rcv\\"), strTemp);
	m_RcvLog = new CLogFile(strText, _T("Rcv.log"), &m_List_RcvLog, _T("RcvMessage"), 500, 50, TRUE);


	m_SendImg = Mat(1544, 2064, CV_8UC3);


	m_PushMem = new CSharedMemoryPush();
	m_PopMem = new CSharedMemoryPop();

	m_PushMem->SetCritcalSection(&m_Critcal);
	m_PopMem->SetCritcalSection(&m_Critcal);

	m_PushMem->SetImageQueue(&m_ShareData);
	m_PopMem->SetImageQueue(&m_ShareData);

	//m_Client = new CClientSock;
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CTestServerGigaVisDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTestServerGigaVisDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CTestServerGigaVisDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CTestServerGigaVisDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (m_ShareData.size() > 0)
	{
		while (TRUE)
		{
			m_ShareData.pop();

			if (m_ShareData.size() == 0)
				break;
		}
	}

	if (m_SendLog != NULL)
	{
		delete m_SendLog;
		m_SendLog = NULL;
	}

	if (m_RcvLog != NULL)
	{
		delete m_RcvLog;
		m_RcvLog = NULL;
	}

	if (m_PushMem != NULL)
	{
		delete m_PushMem;
		m_PushMem = NULL;
	}

	if (m_PopMem != NULL)
	{
		delete m_PopMem;
		m_PopMem = NULL;
	}

	return CDialogEx::DestroyWindow();
}

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg) {
		// 폴더선택 다이얼로그의 초기화가 끝난 경우
	case BFFM_INITIALIZED:
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
		break;
	}
	return 0;
}

void CTestServerGigaVisDlg::OnBnClickedButtonSelectImagePath()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ITEMIDLIST* pidlBrowse;
	WCHAR       pszPathname[MAX_PATH];

	BROWSEINFO  BrInfo;
	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;

	memset(&BrInfo, 0x00, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = _T("Select folder");
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;

	// 이벤트에 대한 사용자정의 함수
	BrInfo.lpfn = BrowseCallbackProc;

	// 사용자정의 함수에 넘겨질 인자로 사용자가 설정하고자 하는 경로를 설정한다.
	// 예를들어 초기폴더경로를 C드라이브로 설정하는 경우
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
	CString strInitPath;
	strInitPath.Format(_T("%s"), strTemp);
	BrInfo.lParam = (LPARAM)strInitPath.GetBuffer();

	pidlBrowse = ::SHBrowseForFolder(&BrInfo);
	if (pidlBrowse != NULL)
	{
		// 선택된 폴더 경로얻음
		SHGetPathFromIDList(pidlBrowse, pszPathname);

		// 경로(pszPathname)를 이용하여 이후작업 추가
		m_Edit_strImagePath.Format(_T("%s"), pszPathname);
		UpdateData(FALSE);

		//InitThread(0);
		GetDlgItem(IDC_BUTTON_THREAD_START_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_THREAD_STOP_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_THREAD_PAUSE_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_THREAD_RESUME_1)->EnableWindow(FALSE);

		//InitThread(1);
		GetDlgItem(IDC_BUTTON_THREAD_START_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_THREAD_STOP_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_THREAD_PAUSE_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_THREAD_RESUME_2)->EnableWindow(FALSE);
	}
}

void CTestServerGigaVisDlg::WriteClient(Mat img)
{
	CString strPacket, strTemp;
	BYTE* byData, *byPacket;
	
	int nWidth, nHeight, nBpp, nDepth;
	nWidth = img.cols;
	nHeight = img.rows;
	nBpp = img.channels();
	nDepth = img.depth();
	
	int nImageSize = nWidth * nHeight * nBpp;
	byData = new BYTE[nImageSize];
	memcpy(byData, img.data, nImageSize);
	
	char* cImageData;
	cImageData = new char[512];
	memset(cImageData, NULL, sizeof(char) * 512);

	strTemp.Format(_T("%C%d,%d,%d,%d,"), PACKET_CHAR_STX, nHeight, nWidth, nBpp, nDepth);
	strPacket.Format(_T("%s%d,"), strTemp, strTemp.GetLength() + nImageSize + 1);
	int nPacketLen = nImageSize + strPacket.GetLength() + 1;
	strPacket.Format(_T("%s%d,"), strTemp, nPacketLen);
	sprintf_s(cImageData, strPacket.GetLength() + 1, "%S", strPacket);

	
	byPacket = new BYTE[nPacketLen];
	memset(byPacket, NULL, sizeof(BYTE) * nPacketLen);
	int nIdx = 0;

	for (int i = 0; i < strPacket.GetLength(); i++)
	{
		byPacket[nIdx++] = cImageData[i];
	}

	memcpy(&byPacket[nIdx], byData, sizeof(BYTE) * (nImageSize));

	byPacket[nPacketLen - 1] = PACKET_CHAR_ETX;


	int nRslt = 0;
	if(m_Client != NULL)
		nRslt = m_Client->Send(byPacket, nPacketLen);

	CString strText;
	for (int i = 0; i < 26; i++)
		strText.AppendFormat(_T("%C"), byPacket[i]);
	
	
	m_SendLog->WriteText(strText, TRUE);

	delete[] cImageData;
	delete[] byData;
	delete[] byPacket;
}

BOOL CTestServerGigaVisDlg::SendCliendMessage(void* pData)
{


	return TRUE;
}

BOOL CTestServerGigaVisDlg::InitThread(int nIdx)
{
	BOOL rslt = TRUE;
	EndThread(nIdx);
	switch (nIdx)
	{
	case 0:
	{
		m_PushMem->m_strReadImagePath.Format(_T("%s"), m_Edit_strImagePath);
		m_pServerThread[0] = AfxBeginThread(Thread0, m_PushMem, THREAD_PRIORITY_NORMAL);
		m_pServerThread[0]->m_bAutoDelete = FALSE;
	}
		break;
	case 1:
	{
		m_PopMem->m_bSendReady = TRUE;
		m_PopMem->SetCallBack(SharedMmemoryCalbackFunc);
		m_pServerThread[1] = AfxBeginThread(Thread1, m_PopMem, THREAD_PRIORITY_NORMAL);
		m_pServerThread[1]->m_bAutoDelete = FALSE;
	}
		break;
	default:
		rslt = FALSE;
		break;
	}

	return rslt;
}

BOOL CTestServerGigaVisDlg::EndThread(int nIdx)
{
	BOOL rslt = TRUE;
	if (m_pServerThread[nIdx] != NULL)
	{
		DWORD dwRet = 0;

		if(nIdx == 0)
			dwRet = WaitForSingleObject(m_pServerThread[nIdx]->m_hThread, m_Edit_nThreadDelay0);
		else
			dwRet = WaitForSingleObject(m_pServerThread[nIdx]->m_hThread, m_Edit_nThreadDelay1);

		if (dwRet == WAIT_FAILED)
		{
			rslt = FALSE;
		}
		else if (dwRet == WAIT_ABANDONED)
		{
			rslt = FALSE;
		}
		else if (dwRet == WAIT_TIMEOUT)
		{
			rslt = FALSE;
		}
	}

	if (rslt)
	{
		delete m_pServerThread[nIdx];
		m_pServerThread[nIdx] = NULL;
	}

	return rslt;
}

UINT CTestServerGigaVisDlg::Thread0(LPVOID pParam)
{
	CSharedMemoryPush* pPush = (CSharedMemoryPush*)pParam;
	while (TRUE)
	{
		pPush->SetCritcalSection(TRUE);
		pPush->SharedMemoryPush();
		pPush->SetCritcalSection(FALSE);

		if (pPush->m_bThreadEnd)
			break;

		Sleep(pPush->m_nThreadDelayTime);
	}

	return 0;
}

UINT CTestServerGigaVisDlg::Thread1(LPVOID pParam)
{
	CSharedMemoryPop* pPop = (CSharedMemoryPop*)pParam;
	while (TRUE)
	{
		pPop->SetCritcalSection(TRUE);
		if (pPop->GetQueImageCount() > 0 && pPop->m_bSendReady)
		{
			pPop->m_bSendReady = FALSE;
			pPop->SharedMemoryPop();
		}
		pPop->SetCritcalSection(FALSE);

		if (pPop->m_bThreadEnd)
			break;

		Sleep(pPop->m_nThreadDelayTime);
	}

	return 0;
}

void CTestServerGigaVisDlg::OnBnClickedButtonThreadDelaySet()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nId = GetFocus()->GetDlgCtrlID();
	int nThreadIdx = 0;
	if (nId == IDC_BUTTON_THREAD_DELAY_SET_2)
		nThreadIdx = 1;

	UpdateData(TRUE);
	switch (nThreadIdx)
	{
	case 0:
		m_PushMem->m_nThreadDelayTime = m_Edit_nThreadDelay0;
		break;
	case 1:
		m_PopMem->m_nThreadDelayTime = m_Edit_nThreadDelay1;
		break;
	}

}


void CTestServerGigaVisDlg::OnBnClickedButtonThreadStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nId = GetFocus()->GetDlgCtrlID();
	int nThreadIdx = 0;
	if (nId == IDC_BUTTON_THREAD_START_2)
		nThreadIdx = 1;

	switch (nThreadIdx)
	{
	case 0:
		m_PushMem->m_bThreadEnd = FALSE;
		GetDlgItem(IDC_BUTTON_THREAD_START_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_THREAD_STOP_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_THREAD_PAUSE_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_THREAD_RESUME_1)->EnableWindow(FALSE);
		break;
	case 1:
		m_PopMem->m_bThreadEnd = FALSE;
		GetDlgItem(IDC_BUTTON_THREAD_START_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_THREAD_STOP_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_THREAD_PAUSE_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_THREAD_RESUME_2)->EnableWindow(FALSE);
		break;
	}

	InitThread(nThreadIdx);
}

void CTestServerGigaVisDlg::OnBnClickedButtonThreadStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nId = GetFocus()->GetDlgCtrlID();
	int nThreadIdx = 0;
	if (nId == IDC_BUTTON_THREAD_STOP_2)
		nThreadIdx = 1;


	switch (nThreadIdx)
	{
	case 0:
		m_PushMem->m_bThreadEnd = TRUE;
		GetDlgItem(IDC_BUTTON_THREAD_START_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_THREAD_STOP_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_THREAD_PAUSE_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_THREAD_RESUME_1)->EnableWindow(FALSE);
		break;
	case 1:
		m_PopMem->m_bThreadEnd = TRUE;
		GetDlgItem(IDC_BUTTON_THREAD_START_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_THREAD_STOP_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_THREAD_PAUSE_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_THREAD_RESUME_2)->EnableWindow(FALSE);
		break;
	}


	EndThread(nThreadIdx);
}


void CTestServerGigaVisDlg::OnBnClickedButtonThreadPause()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nId = GetFocus()->GetDlgCtrlID();
	int nThreadIdx = 0;
	if (nId == IDC_BUTTON_THREAD_PAUSE_2)
		nThreadIdx = 1;

	switch (nThreadIdx)
	{
	case 0:
		GetDlgItem(IDC_BUTTON_THREAD_START_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_THREAD_STOP_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_THREAD_PAUSE_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_THREAD_RESUME_1)->EnableWindow(TRUE);
		break;
	case 1:
		GetDlgItem(IDC_BUTTON_THREAD_START_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_THREAD_STOP_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_THREAD_PAUSE_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_THREAD_RESUME_2)->EnableWindow(TRUE);
		break;
	}

	m_pServerThread[nThreadIdx]->SuspendThread();
}


void CTestServerGigaVisDlg::OnBnClickedButtonThreadResume()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nId = GetFocus()->GetDlgCtrlID();
	int nThreadIdx = 0;
	if (nId == IDC_BUTTON_THREAD_RESUME_2)
		nThreadIdx = 1;

	switch (nThreadIdx)
	{
	case 0:
		GetDlgItem(IDC_BUTTON_THREAD_START_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_THREAD_STOP_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_THREAD_PAUSE_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_THREAD_RESUME_1)->EnableWindow(FALSE);
		break;
	case 1:
		GetDlgItem(IDC_BUTTON_THREAD_START_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_THREAD_STOP_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_THREAD_PAUSE_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_THREAD_RESUME_2)->EnableWindow(FALSE);
		break;
	}

	m_pServerThread[nThreadIdx]->ResumeThread();
}

LRESULT CTestServerGigaVisDlg::OnAccept(WPARAM wParam, LPARAM lParam)
{
	//새로운 클라이언트와 연결할 소켓을 만들고

	m_Client = new CClientSock;

	//서버 소켓으로부터 현재 연결된 소켓을
	//받는다.
	m_Client = m_Server->GetAcceptSocket();

	//hwnd설정
	m_Client->SetWnd(this->m_hWnd);

	return TRUE;
}

LRESULT CTestServerGigaVisDlg::OnClose(WPARAM wParam, LPARAM lParam)
{
	//접속 중인 클라이언트가 해제 되면 메시지 출력
	//기존 서버 소켓을 닫는다.
	m_Server->Close();
	//delete m_Client;
	delete m_Server;

	AfxSocketInit();
	m_Server = new CSeverSock;
	BOOL chk;
	chk = TRUE;
	CString strText, packet;
	delete m_Server;
	//서버 소켓을 만든다.
	m_Server = new CSeverSock;
	AfxSocketInit();
	if (!m_Server->Create(5000))
	{
		chk = FALSE;
		strText.Format(_T("Error : TCP/IP server create fail!"));
		AfxMessageBox(strText);
		return TRUE;
	}

	m_Server->SetWnd(this->m_hWnd);
	//클라이언트가 오기를 기다린다.
	if (!m_Server->Listen())
	{
		chk = FALSE;
		strText.Format(_T("Error : TCP/IP server listen fail!"));
		AfxMessageBox(strText);
		return TRUE;
	}

	if (chk)
	{
		strText.Format(_T("TCP/IP server open OK!"));
	}

	return TRUE;
}

LRESULT CTestServerGigaVisDlg::OnReceive(WPARAM wParam, LPARAM lParam)
{
	BYTE temp[PACKET_MAX_NUM];
	memset(temp, NULL, PACKET_MAX_NUM);
	int nRcvLen = m_Client->Receive(temp, PACKET_MAX_NUM);

	CString strText;
	for (int i = 0; i < nRcvLen; i++)
		strText.AppendFormat(_T("%C"), temp[i]);
	
	m_RcvLog->WriteText(strText, TRUE);


	if (!strText.Mid(1, 3).Compare(_T("ERR")))
	{

	}


	m_PopMem->m_bSendReady = TRUE;

	return TRUE;
}

void CTestServerGigaVisDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	switch ((int)nIDEvent)
	{
	case 0:
		if (m_PushMem->m_bThreadEnd)
		{
			KillTimer(0);
			GetDlgItem(IDC_BUTTON_THREAD_START_1)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_THREAD_STOP_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_THREAD_PAUSE_1)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_THREAD_RESUME_1)->EnableWindow(FALSE);
		}
		break;
	case 1:
		if (m_PopMem->m_bThreadEnd)
		{
			KillTimer(1);
			GetDlgItem(IDC_BUTTON_THREAD_START_2)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_THREAD_STOP_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_THREAD_PAUSE_2)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_THREAD_RESUME_2)->EnableWindow(FALSE);
		}
		break;
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}
