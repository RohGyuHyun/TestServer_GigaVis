///////////////////////////////////////////////////////////////////
//sversok.h
//서버용 메인소켓 프로그램
#define PACKET_MAX_NUM					204800
#define PACKET_CHAR_STX					0x02
#define PACKET_CHAR_ETX					0x03

#include "Clientsok.h"
#define  WM_ACCEPT_SOCKET WM_USER+5

class CSeverSock : public CAsyncSocket
{

	public:
		CSeverSock();
		//메세지를 전달할 HWND설정
		void SetWnd(HWND hwnd);		
		//현재 클라이언트와 연결된 소켓을 받는다.
		CClientSock* GetAcceptSocket();

		//새로운 클라이언트가 연결되었을대 
		virtual void OnAccept( int nErrorCode );

	public:
		//새로운 클라이언트  가 연결되었을때
		//Accept할 소켓 클래스 변수
		CClientSock  m_pChild;
		HWND m_pHwnd;
		BOOL m_bConnect;
			
};
/////////////////////////////////////////////////////////////////