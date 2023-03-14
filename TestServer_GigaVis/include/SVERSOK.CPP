////////////////////////////////////////////////////////////////////
//SverSok.cpp
//서버클래스 함수 소스
#include "pch.h"
#include "sversok.h"
//생성자
CSeverSock::CSeverSock()
{
	CAsyncSocket::CAsyncSocket();
	m_bConnect = FALSE;
}
//메세지를 전달할 윈도우 설정
void CSeverSock::SetWnd(HWND hwnd)
{
	m_pHwnd=hwnd;
}

//새로운 클라이언트가 연결되었을때
//실행되는 함수
void CSeverSock::OnAccept( int nErrorCode )
{
	 TRACE("Errocode = %d",nErrorCode);
	 //클라이언트 클래스인 m_pChild로 연결시켜준다.
	 Accept(m_pChild);
	 //새로운 클라이언트가 연결되었다는
	 //WM_ACCEPT_SOCKET 메세지를 전달합니다.
	 SendMessage(m_pHwnd,WM_ACCEPT_SOCKET,0,0);
}
//OnAccept함수 실행후 새로운 클라이언트와 연결된
//m_pChild 소켓을 넘겨줍니다.
CClientSock* CSeverSock::GetAcceptSocket()
{
	return &m_pChild;
}
/////////////////////////////////////////////////////////////////////////






