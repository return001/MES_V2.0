// Task.h: interface for the CTask class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASK_H__C95E4014_8C9E_4AD9_8AF7_62D73513DB33__INCLUDED_)
#define AFX_TASK_H__C95E4014_8C9E_4AD9_8AF7_62D73513DB33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//主控线程
#define PORT_AUTO_THREAD     23001
//串口单文件下载
#define PORT1_SINGLEWRITE1_THREAD     23010
#define PORT1_SINGLEWRITE2_THREAD     23011
#define PORT1_SINGLEWRITE3_THREAD     23012
#define PORT1_SINGLEREAD1_THREAD   23013
#define PORT1_SINGLEREAD2_THREAD   23014
#define PORT1_SINGLEREAD3_THREAD   23015
#define PORT1_SINGLEWRITE4_THREAD     23016
#define PORT1_SINGLEREAD4_THREAD   23017

//串口1
#define PORT1_WRITE1_THREAD     23020
#define PORT1_WRITE2_THREAD     23021
#define PORT1_WRITE3_THREAD     23022
#define PORT1_READ1_THREAD   23023
#define PORT1_READ2_THREAD   23024
#define PORT1_READ3_THREAD   23025
#define PORT1_WRITE4_THREAD     23026
#define PORT1_READ4_THREAD   23027


//串口2
#define PORT2_WRITE1_THREAD     23030
#define PORT2_WRITE2_THREAD     23031
#define PORT2_WRITE3_THREAD     23032
#define PORT2_READ1_THREAD   23033
#define PORT2_READ2_THREAD   23034
#define PORT2_READ3_THREAD   23035
#define PORT2_WRITE4_THREAD     23036
#define PORT2_READ4_THREAD   23037

//串口3
#define PORT3_WRITE1_THREAD     23040
#define PORT3_WRITE2_THREAD     23041
#define PORT3_WRITE3_THREAD     23042
#define PORT3_READ1_THREAD   23043
#define PORT3_READ2_THREAD   23044
#define PORT3_READ3_THREAD   23045
#define PORT3_WRITE4_THREAD     23046
#define PORT3_READ4_THREAD   23047

//串口4
#define PORT4_WRITE1_THREAD     23050
#define PORT4_WRITE2_THREAD     23051
#define PORT4_WRITE3_THREAD     23052
#define PORT4_READ1_THREAD   23053
#define PORT4_READ2_THREAD   23054
#define PORT4_READ3_THREAD   23055
#define PORT4_WRITE4_THREAD     23056
#define PORT4_READ4_THREAD   23057

/** 主线程退出标志变量 */
extern bool s_bSingleExit;
extern bool s_bExit;
//串口单文件下载
extern bool m_TreadPort1SINGLEDownloadWrite1;
extern bool m_TreadPort1SINGLEDownloadWrite2;
extern bool m_TreadPort1SINGLEDownloadWrite3;
extern bool m_TreadPort1SINGLEDownloadWrite4;
extern bool m_TreadPort1SINGLEDownloadRead1;
extern bool m_TreadPort1SINGLEDownloadRead2;
extern bool m_TreadPort1SINGLEDownloadRead3;
extern bool m_TreadPort1SINGLEDownloadRead4;

//串口1
extern bool m_TreadPort1DownloadWrite1;
extern bool m_TreadPort1DownloadWrite2;
extern bool m_TreadPort1DownloadWrite3;
extern bool m_TreadPort1DownloadWrite4;
extern bool m_TreadPort1DownloadRead1;
extern bool m_TreadPort1DownloadRead2;
extern bool m_TreadPort1DownloadRead3;
extern bool m_TreadPort1DownloadRead4;

//串口2
extern bool m_TreadPort2DownloadWrite1;
extern bool m_TreadPort2DownloadWrite2;
extern bool m_TreadPort2DownloadWrite3;
extern bool m_TreadPort2DownloadWrite4;
extern bool m_TreadPort2DownloadRead1;
extern bool m_TreadPort2DownloadRead2;
extern bool m_TreadPort2DownloadRead3;
extern bool m_TreadPort2DownloadRead4;

//串口3
extern bool m_TreadPort3DownloadWrite1;
extern bool m_TreadPort3DownloadWrite2;
extern bool m_TreadPort3DownloadWrite3;
extern bool m_TreadPort3DownloadWrite4;
extern bool m_TreadPort3DownloadRead1;
extern bool m_TreadPort3DownloadRead2;
extern bool m_TreadPort3DownloadRead3;
extern bool m_TreadPort3DownloadRead4;

//串口4
extern bool m_TreadPort4DownloadWrite1;
extern bool m_TreadPort4DownloadWrite2;
extern bool m_TreadPort4DownloadWrite3;
extern bool m_TreadPort4DownloadWrite4;
extern bool m_TreadPort4DownloadRead1;
extern bool m_TreadPort4DownloadRead2;
extern bool m_TreadPort4DownloadRead3;
extern bool m_TreadPort4DownloadRead4;

class CTask  //任务基类
{
private:

	void* m_pData;
	int threadCommand;

public:
	CTask();
	virtual ~CTask();
	void SetData(void* pData){ m_pData = pData; };
	void* GetData(){ return m_pData; };
	virtual void Run();
	void SetCommand(int setCommand) { threadCommand = setCommand; };
	int GetCommand() { return threadCommand; };
protected:


};

#endif // !defined(AFX_TASK_H__C95E4014_8C9E_4AD9_8AF7_62D73513DB33__INCLUDED_)