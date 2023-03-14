
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
	, m_Edit_nThreadDelay0(0)
	, m_Edit_nThreadDelay1(0)
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
	AfxSocketInit();
	m_Server = new CSeverSock;
	BOOL chk;
	chk = TRUE;
	CString strTemp, packet;
	delete m_Server;
	//서버 소켓을 만든다.
	m_Server = new CSeverSock;
	AfxSocketInit();
	if (!m_Server->Create(5000))
	{
		chk = FALSE;
		strTemp.Format(_T("Error : TCP/IP server create fail!"));
		AfxMessageBox(strTemp);
		return TRUE;
	}

	m_Server->SetWnd(this->m_hWnd);
	//클라이언트가 오기를 기다린다.
	if (!m_Server->Listen())
	{
		chk = FALSE;
		strTemp.Format(_T("Error : TCP/IP server listen fail!"));
		AfxMessageBox(strTemp);
		return TRUE;
	}

	if (chk)
	{
		strTemp.Format(_T("TCP/IP server open OK!"));
	}

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

