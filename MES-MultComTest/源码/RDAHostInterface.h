#pragma once

extern CString RDAComReceive[16];
static void RecvDataCallBack(int host_or_rs232, int commIndex, char * pContent);//回调函数必须是全局函数

class RDAHostInterface
{
public:
	RDAHostInterface();
	~RDAHostInterface();

	HINSTANCE m_RDaHInst;
	BOOL ComInit[16];
	BOOL RDADllInit();//初始化函数
	BOOL RDAComInit(int HandleNum,CString PortName);//串口初始化
	BOOL RDAComWriteData(int HandleNum, CString PortData);//写串口函数
	BOOL RDAComShutdown(int HandleNum);//关串口函数
	BOOL RDAComReconnect(int HandleNum);//串口重新连接函数
	BOOL RDADeinitialization();//释放动态库



};

