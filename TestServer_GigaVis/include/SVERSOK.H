///////////////////////////////////////////////////////////////////
//sversok.h
//������ ���μ��� ���α׷�
#define PACKET_MAX_NUM					204800
#define PACKET_CHAR_STX					0x02
#define PACKET_CHAR_ETX					0x03

#include "Clientsok.h"
#define  WM_ACCEPT_SOCKET WM_USER+5

class CSeverSock : public CAsyncSocket
{

	public:
		CSeverSock();
		//�޼����� ������ HWND����
		void SetWnd(HWND hwnd);		
		//���� Ŭ���̾�Ʈ�� ����� ������ �޴´�.
		CClientSock* GetAcceptSocket();

		//���ο� Ŭ���̾�Ʈ�� ����Ǿ����� 
		virtual void OnAccept( int nErrorCode );

	public:
		//���ο� Ŭ���̾�Ʈ  �� ����Ǿ�����
		//Accept�� ���� Ŭ���� ����
		CClientSock  m_pChild;
		HWND m_pHwnd;
		BOOL m_bConnect;
			
};
/////////////////////////////////////////////////////////////////