#pragma once

typedef enum _enErrorCode
{
	enNoneErr				= 0,			//没有错误 
	enUnknownCmdErr			= 1,			//未知的命令号
	enMissingParamErr		= 2,			//缺少参数
	enParamErr				= 3,			//参数错误
	enStateErr  			= 4,			//状态错误
	enParseCmdErr			= 5,			//解析Command错误
	enL1NoRspErr			= 6,			//L1没有响应
	enL1CellSearchFailErr	= 7,			//L1搜索失败
	enL1NotSupportErr		= 8,			/*L1不支持*/
	enFlashAddrErr			= 9,			/*Flash地址越界*/
	enUnknowErr				= 10,          /*未知错误*/      

	ComNoRsp				=-1,			//串口无响应
	ComRspErr				=-2,			//串口响应错误
	ComCheckErr				=-3,			//串口响应校验码错误
	ComNoRspMsg				=-4,			//串口无信息响应
	ComRspMsgErr			=-5,			//串口信息响应错误
	ComMsgCheckErr			=-6,			//串口信息响应校验码错误
	ComRspTypeErr			=-7,			//串口响应类型错误
	ComRspLenErr			=-8,			//串口响应长度错误
	ComMsgLenErr			=-9,			//串口信息响应长度错误
	TblWrErr				=-10,			//APC/AGC表读写数据错误
	IMEIWrErr				=-11,			//IMEI读写错误
	DspRspTypeErr			=-20,			//Dsp响应类型错误(GSM)
	
	ThreadExited			=-100,			//测试线程退出
	SwVerErr				=-101,			//软件版本错误
	PreStationUnpass		=-102,			//未通过前一测试点测试
	HwVerErr				=-103,			//硬件版本错误
}EnumErrorCode;
typedef enum LOSS_BAND
{
	TD2L = 0,
	TD2M,
	TD2H,
	GSM900L,
	GSM900M,
	GSM900H,
	DCSL,
	DCSM,
	DCSH,
	GSM850L,
	GSM850M,
	GSM850H,	
	PCSL,
	PCSM,
	PCSH,
	TD1L,
	TD1M,
	TD1H,
	LTE1L,
	LTE1M,
	LTE1H,
	LTE2L,
	LTE2M,
	LTE2H,
	LTEB1L,
	LTEB1M,
	LTEB1H,
	LTEB3L,
	LTEB3M,
	LTEB3H,
	LTEB7L,
	LTEB7M,
	LTEB7H,
	LTEB17L,
	LTEB17M,
	LTEB17H
}LossBand;



#define WM_SEND_MSG WM_USER+100
class Strategy
{
public:
	Strategy();
	virtual ~Strategy();
	virtual int AlgrithmInterface(CWnd *pWnd,char* scanSN1_Accept,char*scanIMEI1_Accept,char*scanWIFI_Accept,int port,char* Software_Version);
};
class CLeadcoreChip:public Strategy
{
public:
	CLeadcoreChip(void); 
	virtual ~CLeadcoreChip(void);
	int AlgrithmInterface(CWnd *pWnd,char* scanSN1_Accept,char*scanIMEI1_Accept,char*scanWIFI_Accept,int port,char* Software_Version); 

private:
	CWnd * LBC_pWnd;
public:
	static int Tstop;  
public:
	char* scanSN1,*scanIMEI1,*scanWIFI,*Software_Ver;									//扫描输入的SN\IMEI\MAC
	int Com_port;
protected:
	void SendMSG(char* msg);
public:
	int AssociateWnd(CWnd *pWnd,char* scanSN1_Accept,char*scanIMEI1_Accept,char*scanWIFI_Accept,int port,char* Software_Version);

	EnumErrorCode Turn_on(char mode); 
	int RUN_TEST(void);     

	EnumErrorCode Com_init(int Rs232);
	void lowtoup(char *ch);

	EnumErrorCode AT_rd(char *Resp);
	EnumErrorCode TestRead_AT(int *data,char *Resp); 
	void STOP (void);
};
