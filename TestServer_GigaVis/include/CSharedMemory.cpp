#include "pch.h"
#include "CSharedMemory.h"
#include <iostream>

BOOL CSharedMemoryPush::SharedMemoryPush()
{
	BOOL rslt = TRUE;

	if (m_strReadFilePath.size() == 0)
	{
		GetFolderInFileName(m_strReadImagePath, &m_strReadFilePath, _T("bmp"));
	}

	if (m_strReadFilePath.size() == 0)
	{
		return FALSE;
	}

	CStringA strFilePath;
	strFilePath.Format("%S\\%S", m_strReadImagePath, m_strReadFilePath.front());

	Mat img;
	img = cv::imread(strFilePath.GetBuffer());
	if(m_queImage != NULL)
		m_queImage->push(img);
	
	if (m_strReadFilePath.size() > 0)
	{
		m_strReadFilePath.pop();
	}

	return rslt;
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
	m_queImage = NULL;
	m_bThreadEnd = FALSE;
	m_nThreadDelayTime = 1000;
}

CSharedMemoryPush::~CSharedMemoryPush()
{
	
}


/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSharedMemoryPop::SharedMemoryPop()
{
	BOOL rslt = TRUE;
	
	if (m_queImage != NULL)
	{
		if (m_queImage->size() > 0)
		{
			callEventfunc(0, m_queImage->front());
			m_queImage->pop();
		}
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
	m_queImage = NULL;
	m_bThreadEnd = FALSE;
	m_nThreadDelayTime = 1000;
}

CSharedMemoryPop::~CSharedMemoryPop()
{
	
}