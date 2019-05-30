#pragma once

#include <Windows.h>
#include <iostream>
using namespace std;

#define LOG_DIR           _T("LogFile\\")
#define LOG_FILE          _T("my.log")
#define LOG_SIZE          (16000*1024)


class CLog
{
public:
	CLog();
	~CLog();

	//Static CString GetModuleFullPath(void);
	//Static void WriteLog(const char *pTemp, CString LogName);
	
	CString GetModuleFullPath(void);
	void WriteLog(const char *pTemp, CString LogName);
	CString g_strLogFile;
};

