#include "stdafx.h"
#include "RDAHostInterface.h"

CString RDAComReceive[16];
typedef void *DLL_CALLBACK(int host_or_rs232, int commIndex, char *Content);
bool(*DLLAutoTestInit)(DLL_CALLBACK cb);
bool(*HostComPortInit)(int commIndex, char *PortName, char *pRS232PortName);
bool(*HostComPortShutdown)(int commIndex);
int(*HostComPortWriteData)(int commIndex, char *Data);
bool(*HostComPortcReconnect)(int commIndex);

RDAHostInterface::RDAHostInterface()
{
	for (int i = 0; i < 16; i++)
		ComInit[i] = FALSE;
}


RDAHostInterface::~RDAHostInterface()
{
}

//RDA初始化函数
BOOL RDAHostInterface::RDADllInit()
{
	m_RDaHInst = LoadLibraryA("AutoTest.dll");

	if (m_RDaHInst)
	{
		(FARPROC &)DLLAutoTestInit = GetProcAddress(m_RDaHInst, "DLLAutoTestInit");
		(FARPROC &)HostComPortInit = GetProcAddress(m_RDaHInst, "HostComPortInit");
		(FARPROC &)HostComPortWriteData = GetProcAddress(m_RDaHInst, "HostComPortWriteData");
		(FARPROC &)HostComPortShutdown = GetProcAddress(m_RDaHInst, "HostComPortShutdown");
		(FARPROC &)HostComPortcReconnect = GetProcAddress(m_RDaHInst, "HostComPortcReconnect");

		if (DLLAutoTestInit((DLL_CALLBACK*)RecvDataCallBack))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}

//串口初始化
BOOL RDAHostInterface::RDAComInit(int HandleNum, CString PortName)
{
	char* a = (LPSTR)(LPCTSTR)PortName;

	if (ComInit[HandleNum] == FALSE&&PortName!="")
	{
		bool IsSusscc = HostComPortInit(HandleNum, a, NULL);
		Sleep(200);
		if (IsSusscc==true)
		{
			ComInit[HandleNum] = TRUE;
			return TRUE;
		}
	}
		return FALSE;
}

//写串口函数
BOOL RDAHostInterface::RDAComWriteData(int HandleNum, CString PortData)
{
	char *PortDataChar;
	PortData.Replace("AT^GT_CM=", "");//RDA平台的指令要去掉前缀
	PortData.Replace("\r\n", "");

	//RDA平台对标准AT指令做一些处理
	if (PortData.Find("ATD112") != -1 || PortData.Find("ATA") != -1 || PortData.Find("ATH") != -1 || PortData.Find("AT+CSQ") != -1)
	{
		if (PortData.Find("2AT,")==-1)
		{
			PortData = "2AT," + PortData;
		}
	}

	PortDataChar = (LPSTR)(LPCTSTR)PortData;
	if (ComInit[HandleNum] == TRUE)
	{
		int WriteInt = HostComPortWriteData(HandleNum, PortDataChar);
		if (WriteInt == 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}

//关串口函数
BOOL RDAHostInterface::RDAComShutdown(int HandleNum)
{
	GetLastError();
	Sleep(100);
	if (ComInit[HandleNum] == TRUE)
	{
		bool IsSusscc = HostComPortShutdown(HandleNum);
		Sleep(200);
		if (IsSusscc == true)
		{
			ComInit[HandleNum] = FALSE;
		}
		else
		{
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

//串口重新连接函数
BOOL RDAHostInterface::RDAComReconnect(int HandleNum)
{
	if(HostComPortcReconnect(HandleNum))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//释放动态库
BOOL RDAHostInterface::RDADeinitialization()
{
	if (m_RDaHInst != NULL)
		FreeLibrary(m_RDaHInst);
	return TRUE;
}

//回调函数
void RecvDataCallBack(int host_or_rs232, int commIndex, char * pContent)
{
	RDAComReceive[commIndex] += pContent;
	return;
}