#include "pch.h"
#include "CSharedMemory.h"
#include <iostream>

CSharedMemory* g_MainClass;
#define SHARED_NAME			_T("Global/IMAGE_DATA_SHARED")

BOOL CSharedMemoryPush::SharedMemoryPush()
{
	BOOL rslt = TRUE;

	if(m_strReadFilePath.size() == 0)
		GetFolderInFileName(m_strReadImagePath, &m_strReadFilePath);

	if (m_strReadFilePath.size() == 0)
	{
		return FALSE;
	}

	CStringA strFilePath;
	strFilePath.Format("%S\\%S", m_strReadImagePath, m_strReadFilePath.front());

	Mat img;
	img = cv::imread(strFilePath.GetBuffer());
	g_MainClass->m_queImage.push(img);

	DWORD dwSize = sizeof(img) * 20;
	HANDLE hFile = CreateFile(TEXT(".\\MMF.dat"), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if (NULL != hFile)
	{
		m_hHandle = ::CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, /*sizeof(img)*/dwSize, SHARED_NAME);
		if (NULL == m_hHandle)
			AfxMessageBox(_T("Create Error"));
	}

	if (NULL == m_hHandle)
	{
		m_hHandle = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, NULL, SHARED_NAME);

		if (NULL == m_hHandle)
		{
			AfxMessageBox(_T("Open Error"));
		}
	}

	if (NULL != m_hHandle)
	{
		m_piMemory = (int*)::MapViewOfFile(m_hHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (NULL == m_piMemory)
		{
			AfxMessageBox(_T("Open Error"));
		}
	}

	if (m_nPushIdx > 10)
		m_nPushIdx = 0;
	DWORD dwStartAddr = sizeof(img)* m_nPushIdx + 20;
	*m_piMemory += dwStartAddr;

	if (NULL != m_piMemory)
	{
		if (img.rows > 0)
		{
			memcpy(m_piMemory, &g_MainClass->m_queImage.front(), sizeof(g_MainClass->m_queImage.front()));
		}
	}
	else
	{
		AfxMessageBox(_T("MapView is NULL"));
	}

	if (NULL != m_piMemory)
	{
		BOOL bUnMap = ::UnmapViewOfFile(m_piMemory);
		if (bUnMap)
		{
			m_piMemory = NULL;
		}
		else
		{
			AfxMessageBox(_T("Unmap Error"));
		}
	}

	if (NULL != m_hHandle)
	{
		BOOL bClose = ::CloseHandle(m_hHandle);
		if (bClose)
		{
			m_hHandle = NULL;
		}
		else
		{
			AfxMessageBox(_T("Close Error"));
		}
	}
	CloseHandle(hFile);
	if (m_strReadFilePath.size() > 0)
	{
		m_strReadFilePath.pop();
		m_nPushIdx++;
	}

	return rslt;
}

void CSharedMemoryPush::ReleasQue()
{
	if (g_MainClass->m_queImage.size() > 0)
	{
		while (TRUE)
		{
			g_MainClass->m_queImage.pop();
			if (g_MainClass->m_queImage.size() == 0)
				break;
		}
	}
}

CSharedMemoryPush::CSharedMemoryPush()
{
	m_nPushIdx = 0;
	//Mat img = Mat(1544, 2064, CV_8UC3);
	//DWORD dwSize = 1544 * 2064 * 3;
	//HANDLE hFile = CreateFile(TEXT(".\\MMF.dat"), GENERIC_READ | GENERIC_WRITE,	0, NULL, CREATE_ALWAYS,	FILE_ATTRIBUTE_NORMAL, NULL);

	//if (NULL != hFile)
	//{
	//	m_hHandle = ::CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, /*sizeof(img)*/dwSize * 10, SHARED_NAME);
	//	if (NULL == m_hHandle)
	//		AfxMessageBox(_T("Create Error"));
	//}

	//if (NULL == m_hHandle)
	//{
	//	m_hHandle = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, NULL, SHARED_NAME);

	//	if (NULL == m_hHandle)
	//	{
	//		AfxMessageBox(_T("Open Error"));
	//	}
	//}

	//if (NULL != m_hHandle)
	//{
	//	m_piMemory = (int*)::MapViewOfFile(m_hHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	//	if (NULL == m_piMemory)
	//	{
	//		AfxMessageBox(_T("Open Error"));
	//	}
	//}
}

CSharedMemoryPush::~CSharedMemoryPush()
{
	/*if (NULL != m_piMemory)
	{
		BOOL bUnMap = ::UnmapViewOfFile(m_piMemory);
		if (bUnMap)
		{
			m_piMemory = NULL;
		}
		else
		{
			AfxMessageBox(_T("Unmap Error"));
		}
	}

	if (NULL != m_hHandle)
	{
		BOOL bClose = ::CloseHandle(m_hHandle);
		if (bClose)
		{
			m_hHandle = NULL;
		}
		else
		{
			AfxMessageBox(_T("Close Error"));
		}
	}*/

	ReleasQue();
}

CSharedMemoryPop::CSharedMemoryPop()
{
	m_nPopIdx = 0;
	m_PopImg.create(1544, 2064, CV_8UC3);
}

CSharedMemoryPop::~CSharedMemoryPop()
{
	ReleasQue();
}

void CSharedMemoryPop::ReleasQue()
{
	if (g_MainClass->m_queImage.size() > 0)
	{
		while (TRUE)
		{
			g_MainClass->m_queImage.pop();
			if (g_MainClass->m_queImage.size() == 0)
				break;
		}
	}
}

BOOL CSharedMemoryPop::SharedMemoryPop()
{
	BOOL rslt = TRUE;
	DWORD dwSize = sizeof(Mat) * 20;
	m_hHandle = NULL;

	//if (NULL != hFile)
	{
		m_hHandle = ::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, /*sizeof(img)*/dwSize, SHARED_NAME);
		if (NULL == m_hHandle)
			AfxMessageBox(_T("Create Error"));
	}


	if (NULL == m_hHandle)
	{
		m_hHandle = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, NULL, SHARED_NAME);

		if (NULL == m_hHandle)
		{
			AfxMessageBox(_T("Open Error"));
		}
	}

	if (NULL != m_hHandle)
	{
		m_piMemory = (int*)::MapViewOfFile(m_hHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (NULL == m_piMemory)
		{
			AfxMessageBox(_T("Open Error"));
		}
	}

	if (m_nPopIdx > 10)
		m_nPopIdx = 0;
	DWORD dwStartAddr = sizeof(m_PopImg) * m_nPopIdx + 20;
	*m_piMemory += dwStartAddr;

	if (NULL != m_piMemory)
	{
		//Ma/t img;
		//img.create(1544, 2064, CV_8UC3);
		memcpy(&m_PopImg, m_piMemory, sizeof(Mat));
		if (m_PopImg.rows != NULL)
		{
			g_MainClass->SetCritcalSection(TRUE);
			g_MainClass->m_queImage.push(m_PopImg);
			g_MainClass->SetCritcalSection(FALSE);
		}
	}
	else
	{
		AfxMessageBox(_T("MapView is NULL"));
	}

	if (NULL != m_piMemory)
	{
		BOOL bUnMap = ::UnmapViewOfFile(m_piMemory);
		if (bUnMap)
		{
			m_piMemory = NULL;
		}
		else
		{
			AfxMessageBox(_T("Unmap Error"));
		}
	}

	if (NULL != m_hHandle)
	{
		BOOL bClose = ::CloseHandle(m_hHandle);
		if (bClose)
		{
			m_hHandle = NULL;
		}
		else
		{
			AfxMessageBox(_T("Close Error"));
		}
	}

	if (g_MainClass->m_queImage.size() > 0)
	{
		g_MainClass->callEventfunc(0);
		m_nPopIdx++;
	}

	return rslt;
}


CSharedMemory::CSharedMemory()
{
	g_MainClass = this;
	m_hHandle = NULL;
	m_bThreadEnd = FALSE;
	m_nThreadDelayTime = 1000;
}
CSharedMemory::~CSharedMemory()
{
	if (NULL != m_hHandle)
	{
		BOOL bClose = ::CloseHandle(m_hHandle);
		if (bClose)
		{
			m_hHandle = NULL;
		}
		else
		{
			AfxMessageBox(_T("Close Error"));
		}
	}

	if (m_queImage.size() > 0)
	{
		while (TRUE)
		{
			m_queImage.pop();
			if (m_queImage.size() == 0)
				break;
		}
	}
}

void CSharedMemory::SetCritcalSection(BOOL isSet)
{
	if (isSet)
		m_Critcal->Lock();
	else
		m_Critcal->Unlock();
}