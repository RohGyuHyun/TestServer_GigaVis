/////////////////////////////////////////////////////////////////////////////////
//clientsok.h
//클라이언트용 소켓 프로그램 헤더

#define WM_RECEIVE_DATA WM_USER+2
#define WM_CLOSE_SOCK WM_USER+3

class CClientSock : public CAsyncSocket
{

public:
		CClientSock();
		//메세지를 전달할 HWND를 설정하는 함수
		void SetWnd(HWND hwnd);
		//새로운 데이타가 들어왔을대 실행되는 함수
		virtual void OnReceive( int nErrorCode );
		virtual void OnClose(int nErrorCode);
public:
		//메세지를 전달할 HWND
		HWND m_pHwnd;
};
///////////////////////////////////////////////////////////////////////////////
