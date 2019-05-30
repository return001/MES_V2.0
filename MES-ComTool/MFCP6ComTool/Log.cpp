#include "stdafx.h"
#include "Log.h"
#include "semaphoreclass.h"

static CSemaphoreClass g_tSemaphore;
//static CString g_strLogFile;

CLog::CLog()
{
}


CLog::~CLog()
{
}

CString CLog::GetModuleFullPath(void)
{
	TCHAR achDllName[32768] = _T("");
	CString csPath = _T("");

	//AfxGetInstanceHandle()可能获取失败
	// 	GetModuleFileName( AfxGetInstanceHandle(), achDllName, MAX_PATH);

	MEMORY_BASIC_INFORMATION mbi;
	static int dummy;
	UINT dwLength = sizeof(mbi);
	if (dwLength == VirtualQuery(&dummy, &mbi, dwLength))
	{
		GetModuleFileName(reinterpret_cast<HMODULE>(mbi.AllocationBase), achDllName, _countof(achDllName));
		size_t nBuffLen = GetLongPathName(achDllName, 0, 0);
		if (nBuffLen == _tcslen(achDllName))
		{
			csPath = achDllName;
		}
		else
		{
			TCHAR* pChar = new TCHAR[nBuffLen + 1];
			ZeroMemory(pChar, sizeof(TCHAR) * (nBuffLen + 1));
			GetLongPathName(achDllName, pChar, nBuffLen);
			csPath = pChar;
			delete[] pChar;
		}
	}

	csPath = csPath.Left(csPath.ReverseFind(_T('\\')) + 1);

	return csPath;
}

void CLog::WriteLog(const char *pTemp,CString LogName)
{
	if (g_strLogFile.IsEmpty())
	{
		CString strPath = GetModuleFullPath();
		CString strLogDir = strPath + LOG_DIR;


		if (!PathFileExists(strLogDir))
		{
			CreateDirectory(strLogDir, NULL);
		}

		g_strLogFile = strLogDir +LogName+_T(".log");
		//g_strLogFile = strLogDir + LOG_FILE;
	}

	CAutoLock cAuto(&g_tSemaphore);
	CFile cfile;

	//打开
	if (!PathFileExists(g_strLogFile))
	{
		BOOL bFlag = cfile.Open(g_strLogFile, CFile::modeCreate | CFile::modeWrite);
		if (!bFlag)
		{
			return;
		}
	}
	else
	{
		CFileStatus fileStaus;
		if (CFile::GetStatus(g_strLogFile, fileStaus))
		{
			UINT nOpenFlag = 0;
			if (fileStaus.m_size > LOG_SIZE)
			{
				nOpenFlag = CFile::modeCreate | CFile::modeWrite;
			}
			else
			{
				nOpenFlag = CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate;
			}

			BOOL bFlag = cfile.Open(g_strLogFile, nOpenFlag);
			if (!bFlag)
			{
				return;
			}
		}
	}

	//
	if (cfile.m_hFile != CFile::hFileNull)
	{
		cfile.SeekToEnd();
		cfile.Write(pTemp, strlen(pTemp) + 1);
		cfile.Close();
	}
}
