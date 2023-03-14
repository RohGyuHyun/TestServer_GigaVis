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

	if (NULL == g_MainClass->m_hHandle)
	{
		g_MainClass->m_hHandle = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, NULL, SHARED_NAME);

		if (NULL == g_MainClass->m_hHandle)
		{
			AfxMessageBox(_T("Open Error"));
		}
	}

	if (NULL != g_MainClass->m_hHandle)
	{
		m_piMemory = (int*)::MapViewOfFile(g_MainClass->m_hHandle, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(img)*10);
		if (NULL == m_piMemory)
		{
			AfxMessageBox(_T("Open Error"));
		}
	}

	if (NULL != m_piMemory)
	{
		if (img.rows > 0)
		{
			memcpy(m_piMemory, &g_MainClass->m_queImage.front(), sizeof(g_MainClass->m_queImage.front()));
			//g_MainClass->m_queImage.pop();
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

	if (m_strReadFilePath.size() > 0)
	{
		m_strReadFilePath.pop();
		m_nPushIdx++;
	}

	return rslt;
}

CSharedMemoryPush::CSharedMemoryPush()
{
	m_nPushIdx = 0;
}

CSharedMemoryPush::~CSharedMemoryPush()
{

}

CSharedMemoryPop::CSharedMemoryPop()
{
	m_PopImg.create(1544, 2064, CV_8UC3);
}

CSharedMemoryPop::~CSharedMemoryPop()
{

}

BOOL CSharedMemoryPop::SharedMemoryPop()
{
	BOOL rslt = TRUE;

	if (NULL == g_MainClass->m_hHandle)
	{
		g_MainClass->m_hHandle = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, NULL, SHARED_NAME);

		if (NULL == g_MainClass->m_hHandle)
		{
			AfxMessageBox(_T("Open Error"));
		}
	}

	if (NULL != g_MainClass->m_hHandle)
	{
		m_piMemory = (int*)::MapViewOfFile(g_MainClass->m_hHandle, FILE_MAP_READ, 0, 0, sizeof(Mat)*10);
		if (NULL == m_piMemory)
		{
			AfxMessageBox(_T("Open Error"));
		}
	}

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

	if(g_MainClass->m_queImage.size() > 0)
		g_MainClass->callEventfunc(0);

	return rslt;
}


CSharedMemory::CSharedMemory()
{
	g_MainClass = this;
	m_bFirst = FALSE;
	m_hHandle = NULL;
	m_bThreadEnd = FALSE;
	m_nThreadDelayTime = 1000;

	Mat img = Mat(1544, 2064, CV_8UC3);

	if (NULL == m_hHandle)
	{
		m_hHandle = ::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(img)*10, SHARED_NAME);
		if (NULL == m_hHandle)
			AfxMessageBox(_T("Create Error"));
	}
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
		m_queImage.empty();
}

void CSharedMemory::SetCritcalSection(BOOL isSet)
{
	if (isSet)
		m_Critcal.Lock();
	else
		m_Critcal.Unlock();
}