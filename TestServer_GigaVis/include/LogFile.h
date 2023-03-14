#pragma once

#define LOG_FILE_PATH_LEN		512
#define LOG_FILE_NAME_LEN		100
#define LOG_FILE_FULL_PATH_LEN	512 + LOG_FILE_NAME_LEN
#define LOG_LEN					512
#define LOG_FILE_BACKUP_SIZE	10.0

class CLogFile
{
public:
	CLogFile();
	CLogFile(LPCTSTR strPath, LPCTSTR strFileName, CListCtrl *ctrList = NULL, LPCTSTR strCol = _T("Log"), int nListSize = 1000, int nMaxList = 50, BOOL isListTimeView = FALSE);
	~CLogFile();

private:
	void MakeDir(LPCTSTR path);
	

	TCHAR m_strFilePath[LOG_FILE_PATH_LEN];//���� ���
	TCHAR m_strFileName[LOG_FILE_NAME_LEN];//���� �̸�
	BOOL m_bLogCreate;
	BOOL m_bListCreate;
	CListCtrl *m_ctrListLog;
	int m_nMaxList;
	BOOL m_bListTimeView;

	CCriticalSection m_Critical;
	
public:
	BOOL Create(LPCTSTR strPath, LPCTSTR strFileName);//Log ����
	BOOL WriteText(LPCTSTR strLog, BOOL isList = FALSE, long limit_file = 1000000);//Log �ۼ�
	BOOL LogListInit(CListCtrl *ctrList, LPCTSTR strCol = _T("Log"), int nListSize = 1000, int nMaxList = 50, BOOL isListTimeView = FALSE);//List �ʱ�ȭ
	BOOL ListAllDelete();//List ���� ��ü ����
	BOOL isLogCreate(){return m_bLogCreate;};
	BOOL isListCreate(){return m_bListCreate;};
};
