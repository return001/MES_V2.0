#include "stdafx.h"
#include "Log.h"
#include "semaphoreclass.h"

static CSemaphoreClass g_tSemaphore;
//static CString RectValuetrLogFile;

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
	if (RectValuetrLogFile.IsEmpty())
	{
		CString strPath = GetModuleFullPath();
		CString strLogDir = strPath + LOG_DIR;


		if (!PathFileExists(strLogDir))
		{
			CreateDirectory(strLogDir, NULL);
		}

		RectValuetrLogFile = strLogDir +LogName+_T(".log");
		//RectValuetrLogFile = strLogDir + LOG_FILE;
	}

	CAutoLock cAuto(&g_tSemaphore);
	CFile cfile;

	//打开
	if (!PathFileExists(RectValuetrLogFile))
	{
		BOOL bFlag = cfile.Open(RectValuetrLogFile, CFile::modeCreate | CFile::modeWrite);
		if (!bFlag)
		{
			return;
		}
	}
	else
	{
		CFileStatus fileStaus;
		if (CFile::GetStatus(RectValuetrLogFile, fileStaus))
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

			BOOL bFlag = cfile.Open(RectValuetrLogFile, nOpenFlag);
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
