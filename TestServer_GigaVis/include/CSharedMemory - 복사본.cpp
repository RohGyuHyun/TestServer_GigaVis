#include "pch.h"
#include "CSharedMemory.h"
#include <iostream>

#define SHARED_NAME			_T("Global/IMAGE_DATA_SHARED")

BOOL CSharedMemoryPush::SharedMemoryPush()
{
	BOOL rslt = TRUE;

	if (m_strReadFilePath.size() == 0)
	{
		GetFolderInFileName(m_strReadImagePath, &m_strReadFilePath, _T("bmp"));
		callEventfunc(1, (int)m_strReadFilePath.size());
	}

	if (m_strReadFilePath.size() == 0)
	{
		return FALSE;
	}

	CStringA strFilePath;
	strFilePath.Format("%S\\%S", m_strReadImagePath, m_strReadFilePath.front());

	Mat img;
	img = cv::imread(strFilePath.GetBuffer());
	m_queImage.push(img);

	DWORD dwStartAddr = sizeof(img) * m_nPushIdx;
	m_piMemory += dwStartAddr;

	if (NULL != m_piMemory)
	{
		if (img.rows > 0)
		{
			memcpy(m_piMemory, &m_queImage.back(), sizeof(m_queImage.back()));
		}
	}
	else
	{
		AfxMessageBox(_T("MapView is NULL"));
	}
	
	if (m_strReadFilePath.size() > 0)
	{
		m_strReadFilePath.pop();
		m_nPushIdx++;
	}

	return rslt;
}

void CSharedMemoryPush::ReleasQue()
{
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

void CSharedMemoryPush::SetCritcalSection(BOOL isSet)
{
	if (isSet)
		m_Critcal->Lock();
	else
		m_Critcal->Unlock();
}

CSharedMemoryPush::CSharedMemoryPush()
{
	m_hHandle = NULL;
	m_bThreadEnd = FALSE;
	m_nThreadDelayTime = 100;
	m_nPushIdx = 0;
	DWORD dwSize = 1024000;
	HANDLE hFile = CreateFile(TEXT(".\\MMF.dat"), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (NULL != hFile)
	{
		m_hHandle = ::CreateFileMapping(hFile/*INVALID_HANDLE_VALUE*/, NULL, PAGE_READWRITE, 0, /*sizeof(img)*/dwSize, SHARED_NAME);
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
		m_piFirstMemory = m_piMemory;
		if (NULL == m_piMemory)
		{
			AfxMessageBox(_T("Open Error"));
		}
	}
	//CloseHandle(hFile);
}

CSharedMemoryPush::~CSharedMemoryPush()
{
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


	ReleasQue();
}


/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSharedMemoryPop::ReleasQue()
{
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

BOOL CSharedMemoryPop::SharedMemoryPop()
{
	BOOL rslt = TRUE;
	DWORD dwSize = sizeof(Mat) * 20;
	m_hHandle = NULL;

	DWORD dwStartAddr = sizeof(m_PopImg) * m_nPopIdx;
	m_piMemory += dwStartAddr;

	if (NULL != m_piMemory)
	{
		memcpy(&m_PopImg, m_piMemory, sizeof(Mat));
		if (m_PopImg.rows != 0 && m_PopImg.cols != 0)
		{
			SetCritcalSection(TRUE);
			CString strData1, strData2;
			
			for (int i = 0; i < 100; i++)
			{
				strData1.AppendFormat(_T("%C"), m_PopImg.data[i]);
				if (m_queImage.size() > 0)
					strData2.AppendFormat(_T("%C"), m_queImage.back().data[i]);
			}
				
			if(strData1.Compare(strData2))
				m_queImage.push(m_PopImg);
			SetCritcalSection(FALSE);
		}
	}
	else
	{
		AfxMessageBox(_T("MapView is NULL"));
	}

	if (m_queImage.size() > 0)
	{
		callEventfunc(0, 0);
		m_nPopIdx++;
	}

	if (m_queImage.size() == 0)
	{
		rslt = FALSE;
	}

	return rslt;
}

void CSharedMemoryPop::SetCritcalSection(BOOL isSet)
{
	if (isSet)
		m_Critcal->Lock();
	else
		m_Critcal->Unlock();
}

CSharedMemoryPop::CSharedMemoryPop()
{
	m_nMaxSendCount = 0;
	m_hHandle = NULL;
	m_bThreadEnd = FALSE;
	m_nThreadDelayTime = 100;
	m_nPopIdx = 0;
	m_PopImg.create(1544, 2064, CV_8UC3);
	m_hHandle = NULL;
	if (NULL == m_hHandle)
	{
		m_hHandle = ::OpenFileMapping(PAGE_READONLY, NULL, SHARED_NAME);

		if (NULL == m_hHandle)
		{
			AfxMessageBox(_T("Open Error"));
		}
	}

	if (NULL != m_hHandle)
	{
		m_piMemory = (int*)::MapViewOfFile(m_hHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		m_piFirstMemory = m_piMemory;
		if (NULL == m_piMemory)
		{
			AfxMessageBox(_T("Open Error"));
		}
	}
}

CSharedMemoryPop::~CSharedMemoryPop()
{
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

	ReleasQue();
}