/////////////////////////////////////////////////////////////////////////////////
//Clientsok.cpp
//Ŭ���̾�Ʈ�� ���� Ŭ�����Լ�

#include "pch.h"
#include "Clientsok.h"



//�����ڴ� �ƹ��ϵ� ���ϰ� �������� CAsyncSocket
//�� �Ѱ��ݴϴ�.
CClientSock::CClientSock()
{
	CAsyncSocket::CAsyncSocket();
}
//����Ÿ�� �������� ���� hwnd�� ���ο� ����Ÿ�� ���
//�Դٴ� �޼����� �����ֱ����� hwnd�� �����մϴ�.
void CClientSock::SetWnd(HWND hwnd)
{
	m_pHwnd=hwnd;
}

//���ο� ����Ÿ�� �������� ������ hwnd��
//USER+2�� �޼��� WM_RECEIVE�� �����ϴ�.

void CClientSock::OnReceive( int nErrorCode )
{
	TRACE("Errocode = %d",nErrorCode);
	SendMessage(m_pHwnd,WM_RECEIVE_DATA,0,0);
}

void CClientSock::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	TRACE("Errocode = %d",nErrorCode);
	SendMessage(m_pHwnd, WM_CLOSE_SOCK, 0, 0);
	

	CAsyncSocket::OnClose(nErrorCode);
}

/////////////////////////////////////////////////////////////////////////////////