#pragma once
#include "afxwin.h"

#include "afxmt.h"//CEvent
#include "CurrentTest.h"
#include "WriteIMEIDlg.h"

#include "PackingPrint.h"

#include "md5.h"

#include <vector>      
#include <map>
 //图片显示
#include "ThumbnailBoxDlg.h"

#include <deque>

#include "TServerScoket.h"
#include "RDAHostInterface.h"

#include "PowerControlDlg.h"
#include "Aes.h"
#include "ImageDll.h"
#include <string.h>
#include <algorithm>
#include "Aes.h"
#include "shlwapi.h"
using namespace std;

// IMEIWrite_MulAT 对话框
#define THREAD_NUM 16
#define PORTS_NUM 48
#define AntiDupData 10

#define WM_SEFOCS  WM_USER+111

static CRITICAL_SECTION GETPORT;

static CRITICAL_SECTION CPUCOMMUNICATE;

static CRITICAL_SECTION UseScanResult;

static CRITICAL_SECTION SPEAKVOICE;

static CRITICAL_SECTION SUCCFAILCOUNT;

static CRITICAL_SECTION UPDATEDB[THREAD_NUM/4];

static CRITICAL_SECTION WIFICOMMUNICATE[THREAD_NUM/2];

static CRITICAL_SECTION SHOWONEPIC;

#define STOPDELAY 200
#define ITEMMAX   100

#define PICDATAMAX 150000
typedef struct
{
	LPVOID WinHandle;
	int HandleNum;
	CEdit* m_Result;
	CEdit* Final_Result_Control;
	CThumbnailBoxDlg* picdlg;
	CString ChipRfIDBG;	//后台要上传的数据
	BOOL DayTimeStatep; //白天黑夜状态
	CString RecString;	//串口接收到的图片数据
}
PARA_Handle;

//任务管理器
struct wininfo
{
	char winbuf[100];
	char filename[100];
	CString winname;
	unsigned long pid;
	HICON winicon;
	HWND hwnd;
	short sign;
};

//改变窗口大小
#include ".\ResizableLib\ResizableDialog.h"

//语音播报
#include <sapi.h>

//图片合成
#include ".\JPGUnion\BasicUse.h"
#include ".\JPGUnion\CombineImages.h"
#include ".\JPGUnion\BjfProcess.h"
#include ".\JPGUnion\PictureProcess.h"


/*三合一新增功能全局函数*/
//变量
struct CopyDataMSG
{
	char Port[1024];//串口号
	INT MessageNum;//自定消息
	char MessageChar[1024];//传递字符串
};

//函数
LONG CallBackCrashHandler(EXCEPTION_POINTERS *pException);

/*三合一新增功能到这里结束*/

class IMEIWrite_MulAT : public CResizableDialog
{
	DECLARE_DYNAMIC(IMEIWrite_MulAT)

public:
	IMEIWrite_MulAT(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~IMEIWrite_MulAT();

// 对话框数据
	enum { IDD = IDD_MULTCOMTEST };

protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg HBRUSH IMEIWrite_MulAT::OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor); 
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void OnCbnSelchangeCombo2_Fresh();					//刷新串口
	void OnGetWebSetting();
	BOOL GetSettingFromDB(CAdoInterface& myado,CString m_server,CString m_db,CString m_user,CString m_pwd,CString SoftwareVer,CEdit* m_Result,CEdit* Final_Result_Control);
	BOOL GetATSetting(CString SettingChar);

	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd*, CPoint point);

	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam); 

	afx_msg void OnTimer(UINT nIDEvent);
public:
	CString ATCommandList_CSDBCompare;
public:
	BOOL SIMisOK[THREAD_NUM];
	PARAMETER_MAIN paraArray[MAXPARANUM];

	CFont* font;
	CFont* fontsp;
	CFont* font_fail;
public:
	CEvent RunCommandList1;
	BOOL StopAutoStart;
	BOOL StopSign[THREAD_NUM];
public:
	void UI_LoadConfig();
	void AT_LoadConfig();
	void Delay_Clock(UINT mSecond);

	BOOL OnGetport();
	//void InitCOM(CComboBox* m_Port,CComboBox* m_Baud,int num);//初始化串口
	//BOOL OPen_Serial_Port(CComboBox* m_Port,CComboBox* m_Baud,int HandleNum,BOOL CPUChoose=FALSE);
	BOOL OPen_Serial_PortReadConstant(CComboBox* m_Port,CComboBox* m_Baud,int HandleNum,BOOL CPUChoose=FALSE);
	BOOL CheckConnect_Thread(CComboBox* m_Port,CComboBox* m_Baud,int HandleNum,CEdit* m_Result,CEdit* Final_Result_Control);
/*	char*  Send_Serial_Order(CString* Vaule_Return,CString strCommand_Vaule,int HandleNum,char* EndSign,char* StartSign,int WaitTime=3,int HexFlag=0);*///因蓝牙需要发送十六进制数据，特增加一个十六进制的默认参数，为1的时候就发送和接收十六进制数据


	void InitCOM(CComboBox* m_Port, CComboBox* m_Baud, int num, BOOL RDAFlag = FALSE);

	BOOL OPen_Serial_Port(CComboBox* m_Port, CComboBox* m_Baud, int HandleNum, BOOL CPUChoose = FALSE, BOOL RDAFlag = FALSE);

	char*  Send_Serial_Order(CString* Vaule_Return, CString strCommand_Vaule, int HandleNum, char* EndSign, char* StartSign, int WaitTime = 3, int HexFlag = 0, BOOL RDAFlag = FALSE);

	BOOL CloseHandleControl(HANDLE hObject, BOOL RDAFlag = FALSE,int HandleNum=255);


	void LogShow_exchange(CEdit* m_Result,CEdit* Final_Result_Control,int State,CString Msg_Log,int HandleNum,CString Category="-1",CString ChipRfIDbg=""); 
	bool IMEI_Function_Judge(int i,CString IMEI_FT_Item,char* Serial_Order_Return,int HandleNum,CEdit* m_Result,CEdit* Final_Result_Control);

	CString GetData(char* Serial_Order_Return,CString Start,CString End,int Count=1,int HandleNum=0);//获取有效数据

public:	
	BOOL WriteIMEIFunction_Thread(CComboBox* m_Port,CComboBox* m_Baud,int HandleNum,CEdit* m_Result,CEdit* Final_Result_Control,CEdit* Data_Input_Control);
	__int64 ComputeCD(__int64 nImei);
	BOOL CheckIMEI(CString	m_IMEI);

	void EnableWindow_ALL(BOOL Choose);

	void EnableWindow_StartALL(BOOL Choose);
	void EnableWindow_Start(int HandleNum);
	void EnableWindow_Choose(int HandleNum);
	void SetFocus_Choose(int HandleNum);
	//void SetFocus_ChooseThread(int HandleNum);

	void Get_App_Running();//获取正在运行的应用 
	void OnWndFocus(BOOL ShowWin,BOOL Print,CString IMEI_Print[THREAD_NUM]);
public:
	CArray<wininfo,wininfo&> m_wininfoarray;
	wininfo m_currentwin;
	CEvent        Save_Wait;
public:
	BOOL EnableMenu;

	BOOL COM_IniDone;
	CString COM_Number[PORTS_NUM];

	BOOL COM_State[THREAD_NUM+1];//串口状态 
	BOOL Thread_State[THREAD_NUM];
public:
	COLORREF gColor[THREAD_NUM];
	
	CBT_Addr_Setting IMEI_Setdlg;
	//CBT_Addr_Setting IMEI_Setdlg2;
	//CBT_Addr_Setting IMEI_SetDB[THREAD_NUM];
	HANDLE hPort[THREAD_NUM+1];						//+1-->单片机控制端口
	CString Vaule_Return_Count_CS[THREAD_NUM+1];	//接收到的参数
	CWinThread* Thread_Handle[THREAD_NUM];

	CString IMEI_Input[THREAD_NUM];

	////////////////////////////////////////
	////////////////////////////////////////
public:
	// 写号线程1结果
	CEdit Final_Result_Control1;
	// LOG信息
	CEdit m_Result1;

	CComboBox m_Port1;
	CComboBox m_Baud1;

	afx_msg void OnEnChangeImeia();
	CString IMEI_InputA;
	CEdit IMEI_InputA_Control;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();




	// 线程2
	CEdit Final_Result_Control2;
	CEdit m_Result2;
	CComboBox m_Port2;
	CComboBox m_Baud2;

	afx_msg void OnEnChangeImeia2();
	CString IMEI_InputB;
	CEdit IMEI_InputB_Control;
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton15();


	// 线程3
	CEdit Final_Result_Control3;
	CEdit m_Result3;
	CComboBox m_Port3;
	CComboBox m_Baud3;
	CString IMEI_InputC;
	CEdit IMEI_InputC_Control;
	afx_msg void OnEnChangeImeia3();
	afx_msg void OnBnClickedButton16();
	afx_msg void OnBnClickedButton17();
	// 线程4
	CEdit Final_Result_Control4;
	CEdit m_Result4;
	CComboBox m_Port4;
	CComboBox m_Baud4;
	CString IMEI_InputD;
	CEdit IMEI_InputD_Control;
	afx_msg void OnEnChangeImeia4();
	afx_msg void OnBnClickedButton18();
	afx_msg void OnBnClickedButton19();
	// 线程5
	CEdit Final_Result_Control5;
	CEdit m_Result5;
	CComboBox m_Port5;
	CComboBox m_Baud5;
	CString IMEI_InputE;
	CEdit IMEI_InputE_Control;
	afx_msg void OnEnChangeImeia5();
	afx_msg void OnBnClickedButton20();
	afx_msg void OnBnClickedButton21();
	// 线程6
	CEdit Final_Result_Control6;
	CEdit m_Result6;
	CComboBox m_Port6;
	CComboBox m_Baud6;
	CString IMEI_InputF;
	CEdit IMEI_InputF_Control;
	afx_msg void OnEnChangeImeia6();
	afx_msg void OnBnClickedButton22();
	afx_msg void OnBnClickedButton23();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	// 线程一
	CButton StartA_Control;
	// 线程二
	CButton StartB_Control;
	// 线程3
	CButton StartC_Control;
	// 线程四
	CButton StartD_Control;
	// 线程五
	CButton StartE_Control;
	// 线程六
	CButton StartF_Control;


	// 扫描枪端口
	CComboBox ScanGunPort1;
	CComboBox ScanGunPort2;
	CComboBox ScanGunPort3;
	CComboBox ScanGunPort4;
	CComboBox ScanGunPort5;
	CComboBox ScanGunPort6;
	// 扫描枪波特率
	CComboBox ScanGunBaud1;
	CComboBox ScanGunBaud2;
	CComboBox ScanGunBaud3;
	CComboBox ScanGunBaud4;
	CComboBox ScanGunBaud5;
	CComboBox ScanGunBaud6;
	// 扫描枪类型
	CComboBox ScanGunTpye;

	HANDLE hScanGun[THREAD_NUM];
	CString Vaule_ScanGun[THREAD_NUM];												//扫描枪数据
	CString OtherData_Input[THREAD_NUM];
public:
	BOOL TestCheck;																	//公用扫描枪
	CString PublicIMEI;
	BOOL CompareIMEI;																//比对IMEI
public:
	BOOL OPen_ScanGun_Port(CComboBox* m_Port,CComboBox* m_Baud,int HandleNum);		//打开扫描枪端口
	BOOL Get_ScanGun_Data(CEdit* m_Result,CEdit* Final_Result_Control,int HandleNum,char* EndSign);								//获取扫描枪端口数据
	BOOL Check_ScanGun_Data(CEdit* m_Result,CEdit* Final_Result_Control,int HandleNum,CEdit* Data_Input_Control,BOOL ImeiChoose,int LengthLimit);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton25();
	afx_msg void OnBnClickedButton26();
	afx_msg void OnBnClickedButton27();
	afx_msg void OnBnClickedButton28();
	afx_msg void OnBnClickedButton29();
	afx_msg void OnBnClickedButton30();
	afx_msg void OnBnClickedButton31();
	afx_msg void OnBnClickedButton32();
	afx_msg void OnBnClickedButton33();
	afx_msg void OnBnClickedButton34();
	afx_msg void OnBnClickedButton35();
public:
	CPackingPrint PrintInt;
	BOOL PrintOn;//打印做了初始化
	CString Data_FromMachine[THREAD_NUM];
	CString Data1_FromDB[THREAD_NUM];
	CString Data2_FromDB[THREAD_NUM];
	CString Data3_FromDB[THREAD_NUM];
public:
	afx_msg void OnBnClickedButton10();
	// 选择是否打印
	CComboBox PrintSelect;
	afx_msg void OnBnClickedButton37();
	afx_msg void OnBnClickedButton38();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton40();
	// 串口是否为稳定端口
	CComboBox PortType;
	afx_msg void OnBnClickedButton42();

public:
	CString MachineType_CS;				//机型名称
	CString ZhiDanCS;					//制单
	CString ReturnCodeCS;				//返工编码
public:
	long TestTimeStart[THREAD_NUM];			//开始接通测试时间
	CFile cReport[THREAD_NUM];
	CString IMEIRecord[THREAD_NUM];			//IMEI
	CString ChipRfID[THREAD_NUM];
	CString Software_Version[THREAD_NUM];	//软件版本
	CString Software_VersionFull;
	CString BTMac[THREAD_NUM];				//蓝牙地址
	CString BTTestResult[THREAD_NUM];		//蓝牙测试结果
	CString SimNumber[THREAD_NUM];			//SIM卡号
	CString NteTemperature[THREAD_NUM];		//温度
	CString GPSSnrAve[THREAD_NUM];			//GPS平均SNR
	CString InchargeI[THREAD_NUM];			//充电电流
public:
	void WriteLogIni(int LogNum,CString ChipRfIDbg="");//写LOG信息
	void WriteLog(CString Text,int	  LogNum);
	void WriteLogEnd(int LogNum);
public:
	// 最终结果
	CEdit Final_Result_Control7;
	CEdit Final_Result_Control8;
	CEdit Final_Result_Control9;
	CEdit Final_Result_Control10;
	CEdit Final_Result_Control11;
	CEdit Final_Result_Control12;
	CEdit Final_Result_Control13;
	CEdit Final_Result_Control14;
	CEdit Final_Result_Control15;
	CEdit Final_Result_Control16;
	CEdit m_Result7;
	CEdit m_Result8;
	CEdit m_Result9;
	CEdit m_Result10;
	CEdit m_Result11;
	CEdit m_Result12;
	CEdit m_Result13;
	CEdit m_Result14;
	CEdit m_Result15;
	CEdit m_Result16;
	CComboBox m_Port7;
	CComboBox m_Port8;
	CComboBox m_Port9;
	CComboBox m_Port10;
	CComboBox m_Port11;
	CComboBox m_Port12;
	CComboBox m_Port13;
	CComboBox m_Port14;
	CComboBox m_Port15;
	CComboBox m_Port16;
	CComboBox m_Baud7;
	CComboBox m_Baud8;
	CComboBox m_Baud9;
	CComboBox m_Baud10;
	CComboBox m_Baud11;
	CComboBox m_Baud12;
	CComboBox m_Baud13;
	CComboBox m_Baud14;
	CComboBox m_Baud15;
	CComboBox m_Baud16;
	CButton Start7_Control;
	CButton Start8_Control;
	CButton Start9_Control;
	CButton Start10_Control;
	CButton Start11_Control;
	CButton Start12_Control;
	CButton Start13_Control;
	CButton Start14_Control;
	CButton Start15_Control;
	CButton Start16_Control;
	CButton StartALL_Control;
	CButton Stop1_Control;
	CButton Stop2_Control;
	CButton Stop3_Control;
	CButton Stop4_Control;
	CButton Stop5_Control;
	CButton Stop6_Control;
	CButton Stop7_Control;
	CButton Stop8_Control;
	CButton Stop9_Control;
	CButton Stop10_Control;
	CButton Stop11_Control;
	CButton Stop12_Control;
	CButton Stop13_Control;
	CButton Stop14_Control;
	CButton Stop15_Control;
	CButton Stop16_Control;
	CButton StopALL_Control;
	CEdit *m_ResultArray[16];
	CEdit *Final_Result_ControlArray[16];
	CButton *m_StopControlArray[16];
	CEdit m_ShowNumberPort1Control;
	CEdit m_ShowNumberPort2Control;
	CEdit m_ShowNumberPort3Control;
	CEdit m_ShowNumberPort4Control;
	CEdit m_ShowNumberPort5Control;
	CEdit m_ShowNumberPort6Control;
	CEdit m_ShowNumberPort7Control;
	CEdit m_ShowNumberPort8Control;
	CEdit *m_ShowNumberPortControlArray[8];
	afx_msg void OnBnClickedButtonstart7();
	afx_msg void OnBnClickedButtonstop7();
	afx_msg void OnBnClickedButtonstart8();
	afx_msg void OnBnClickedButtonstart9();
	afx_msg void OnBnClickedButtonstart10();
	afx_msg void OnBnClickedButtonstart11();
	afx_msg void OnBnClickedButtonstart12();
	afx_msg void OnBnClickedButtonstart13();
	afx_msg void OnBnClickedButtonstart14();
	afx_msg void OnBnClickedButtonstart15();
	afx_msg void OnBnClickedButtonstart16();
	afx_msg void OnBnClickedButtonstop8();
	afx_msg void OnBnClickedButtonstop9();
	afx_msg void OnBnClickedButtonstop10();
	afx_msg void OnBnClickedButtonstop11();
	afx_msg void OnBnClickedButtonstop12();
	afx_msg void OnBnClickedButtonstop13();
	afx_msg void OnBnClickedButtonstop14();
	afx_msg void OnBnClickedButtonstop15();
	afx_msg void OnBnClickedButtonstop16();
public:
	//语音播报
	HRESULT hr;
	ISpVoice*  pVoice;
	void Voice_Ini();
	void Voice_Speak(CString Text);
public:

public:
	BOOL GetExistReturnCode(CAdoInterface& myado,int DataUpNum,CEdit* m_Result,CEdit* Final_Result_Control,CString *ExistMessage,CString ChipRfIDbg="");
	BOOL Data_UpdatePara(CAdoInterface& myado,int DataUpNum,CEdit* m_Result,CEdit* Final_Result_Control,BOOL ErrorUpEnable=TRUE,CString ChipRfIDbg="");
	BOOL Data_UpdatePara2(CAdoInterface& myado,int DataUpNum,CEdit* m_Result,CEdit* Final_Result_Control,BOOL ErrorUpEnable=TRUE,CString ChipRfIDbg="");			//SMT测试
	BOOL Data_UpdatePara3(CAdoInterface& myado, int DataUpNum, CEdit* m_Result, CEdit* Final_Result_Control, BOOL ErrorUpEnable = TRUE, CString ChipRfIDbg = "");			//老化后测试
	BOOL Data_UpdateParaPre(CAdoInterface& myado,int DataUpNum,CEdit* m_Result,CEdit* Final_Result_Control,BOOL* smt_Check);
	BOOL Data_UpdateError(CAdoInterface& myado,int DataUpNum,CEdit* m_Result,CEdit* Final_Result_Control,CString Message,CString Category,CString ChipRfIDbg="");
	BOOL GetExistError(CAdoInterface& myado,int DataUpNum,CEdit* m_Result,CEdit* Final_Result_Control,CString *ExistMessage,CString ChipRfIDbg="");
	// 单片机控制
public:
	BOOL CPUCommunication(CString CpuOrderName,int HandleNum,CString EndSign,CString StartSign,BOOL Enable,CEdit* m_Result=NULL,CEdit* Final_Result_Control=NULL);
public:
	HINSTANCE password_Handle;
public:
	void  SaveConfig_Change();
	void  LoadConfig_Change();

	unsigned short (WINAPI* GetNEWTTL_encrypt_16)(const unsigned char* pData, int nLength);
	bool  LoadDll();

	char HexToASCII(unsigned char  data_hex);
	void HexGroupToString(char *OutStrBuffer, unsigned char *InHexBuffer, unsigned int HexLength);//用于显示串口返回值

	BOOL StringToHexGroup(unsigned char *OutHexBuffer, char *InStrBuffer, unsigned int strLength);//string转HEX "12AB"--->0x12 0xab
	unsigned int SeperateToHex(const char *src, unsigned char *des);							  //"12 AB"--->0x120xAB--->兼容空格
	CString  ShowPassWord(CString InputID,CEdit* m_Result,CEdit* Final_Result_Control,int HandleNum);

	char* AscToHexString(char* pInData, char* pOutData);											//"ab01234" -> "61 62 30 31 32 33 34 "
public:
	CComboBox CPUControlPort;
	CComboBox CPUControlBaud;
	afx_msg void OnBnClickedButtonconncpu();
	afx_msg void OnBnClickedButtondisconncpu();
	// 订单号
	CComboBox OrderNumbersControl;
	CComboBox GPS_SNRLimit;
	CString GPS_SNRLimitCS;
	// 成功失败计数
	int SuccessCount;
	int FailCount;
	// 返工编码
	CComboBox ReturnCodeControl;
	// 电压电流表
	CString SimulationCurrent[THREAD_NUM];
	CComboBox VIControlPort;
	CComboBox VIControlBaud;
	afx_msg void OnBnClickedButtonconncpu2();
	afx_msg void OnBnClickedButtondisconncpu2();
	// 工站选择
	CComboBox WorkStationControl;
	CString WorkStationCS;
public:
	CString	Hostname;
	CString Ipaddress;
	BOOL GetIp_Name(CString& m_hostname,CString& m_ipaddress);
	// 检查SMT是否漏测
	BOOL CheckSMTChoose;
	afx_msg void OnBnClickedCheck39();
	BOOL RTestChoose;
	afx_msg void OnBnClickedCheck40();
	afx_msg void OnCbnSelchangeCombo53();
public:
	void OnBnClickedBtnCopy();
	void FileCopy();
	DWORD CopyFileExLBC(CString strSrcPath, CString strDestPath, CString strExt);
public:
	HANDLE hPicPort[THREAD_NUM];
	CString Vaule_Pic[THREAD_NUM];												//图像数据

	BOOL DayTimeState[THREAD_NUM];												//FALSE夜视,TRUE白天
	CString PicPath[2][THREAD_NUM];												//0夜视,1白天

	CEvent DoorWait[THREAD_NUM];
	BOOL ResetButtonWIFI[THREAD_NUM];											//按钮WIFI信号
	BOOL PicUnionOK[THREAD_NUM];												//图片合成结果

	CEvent SerialPic[THREAD_NUM];
	BOOL SerialPicOver[THREAD_NUM];

	int TimerHandleNum;//定时器显示图片的线程Number
	CThumbnailBoxDlg Picdlg[THREAD_NUM];
	CString UnionPicPath[THREAD_NUM];
public:
	//图片数据
	BOOL GetPicFunction_Thread(int HandleNum,CEdit* m_Result,CEdit* Final_Result_Control,CThumbnailBoxDlg* picdlg,CString ChipRfIDbg,BOOL DayTimeStatep);
	void StartGetPic(int HandleNum,CEdit* m_Result,CEdit* Final_Result_Control);

	BOOL ShowPicFunction_Thread(CString RecString,int HandleNum,CEdit* m_Result,CEdit* Final_Result_Control,CThumbnailBoxDlg* picdlg,CString ChipRfIDbg,BOOL DayTimeStatep);
	void StartShowPic(int HandleNum,CEdit* m_Result,CEdit* Final_Result_Control,CString RecString);
	BOOL Show_Pic_Data(CString RecString,CEdit* m_Result,CEdit* Final_Result_Control,CThumbnailBoxDlg* picdlg,int HandleNum,CString StartSign,CString EndSign,CString ChipRfIDbg,BOOL DayTimeStatep);		//获取图像端口数据

	BOOL OPen_Pic_Port(CComboBox* m_Port,CComboBox* m_Baud,int HandleNum);		//打开图像端口
	BOOL Get_Pic_Data(CEdit* m_Result,CEdit* Final_Result_Control,CThumbnailBoxDlg* picdlg,int HandleNum,CString StartSign,CString EndSign,CString ChipRfIDbg,BOOL DayTimeStatep);		//获取图像端口数据

	BOOL Get_Wifi_Data(CEdit* m_Result,CEdit* Final_Result_Control,int HandleNum,CString StartSign,CString EndSign,BOOL InThread=FALSE);	//获取按钮WIFI数据

	BOOL HEX2JPG();
	BOOL HEX2JPG2();

	BYTE ConvertHexChar(BYTE ch);
	BYTE * StrDecToCHex(int &length, CString str);

	void JPG2HEX();
	BOOL HEX2JPG3(CString str,CString& m_sPath,CString NightDay,int HandleNum,CString ChipRfIDbg="");
	BOOL GetPicChoose;
	//图片合成
	BOOL Jpgunionjpg(CString szPathName1,CString szPathName2,CString strInfo,CString PicName,CString& strDestJpgName);
public:
	ULONG_PTR m_GdiplusToken;
	GdiplusStartupInput m_GdiplusStartupInput;
public:	
	afx_msg void OnBnClickedCheck41();
	// 后台确认
	BOOL BGConfirmChoose;
	CString BGShowPicTime[THREAD_NUM];//图片后台显示时间
	int BGShowPicCount[THREAD_NUM];//图片后台显示次数

	BOOL TestResult[THREAD_NUM];//测试线程的结果
	afx_msg void OnBnClickedCheck42();
	BOOL GPSDataTypeChoose;
	CComboBox WIFI_RSSILimit;
	CString WIFI_RSSILimitCS;
	afx_msg void OnBnClickedCheck43();
	afx_msg void OnCbnSelchangeCombo56();
public:
	BOOL RecoverVoltage;
	// 图片静态显示
	BOOL PicStaticChoose;



	/*新增功能的整合函数*/

	void StartButtonGatherFun(int HandleNum);//将开始按钮需要处理的流程集合成一个函数
	void StopButtonGatherFun(int HandleNum);//将停止按钮需要处理的流程集合成一个函数

	BOOL DequeContinueControlFun(int HandleNum,deque<int> &ContinueDeq);//统一对队列进行管理，在队列有数据进入之时，就判断这条线程有没有被关闭，是否轮到当前这条线程工作了

	/*三合一新增功能*/
	
	//进程通讯相关的
	protected:
		afx_msg LRESULT MSG_GetSimpleMessage(WPARAM wParam, LPARAM lParam);//获取自定义消息用的系统消息函数
		afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);//用来发送结构体的
	public:
		CString m_SoftVersion;//软件版本，在网络获取函数中使用的
		int m_bVar;//消息传送时回复的值，在网络获取函数中使用的
		HWND ADCTHwnd;//ADCT的句柄

		static map<CString, int>PortStatusMap;//此map容器与StopSign数组联动，键放的是串口号，值放的是StopSign数组对应的下标，利用此函数便可以快速通过串口号来判断端口是否处于占用

		void MSG_SendSimpleMessage();//自定义消息发送函数,发送句柄给总控程序
		void MSG_SendCopyDataMessage(CopyDataMSG *MessageStruct,int replyInt);//利用CopyData来发送结构体

		void ADCTInquire(CString PortNo,INT CommandNo);//ADCT进行查询时
		void ADCTSetup(CString PortNo, INT CommandNo,CString message);//ADCT进行设置时
		void SendADCTTestResult(CString PortNo, CString message);//给ADCT主控程序发送测试结果
		void SendADCTHint(CString PortNo, CString message,int MessageType);//给ADCT主控程序发送提示信息,MessageType,0是提示信息，1是正常信息日志，2是错误信息，3是弹出框
		afx_msg void OnClose();;//程序正常退出
		//LONG WINAPI FreeEIM_UnhandledExceptionFilter(LPEXCEPTION_POINTERS ExceptionInfo); //程序异常退出
		void AfxMessageBoxHint(CString str);//因为三合一的关系，所以要用它来代替程序原本的弹出框

		int InitSetting(CString Zhidan, CString Version);//实现自动获取配置
		int StartPortTest(CString PortNo);//实现串口开始
		int StopPortTest(CString PortNo);//实现串口关闭
		


	/*整机测试新增功能*/

    //注意：耳标研发部门自己设计的蓝牙透传方式，与Dongle蓝牙适配器不适用
	public:
		int OrderNumber;//蓝牙指令条数
		int MacIntercept;//MAC地址取后几位
		int OrderCount;//蓝牙指令发送次数
		int ExitOrderNumber;//蓝牙断开指令条数
		CString BleOrder[10][2];//存放蓝牙指令
		CString ExitOrder[10][2];//存放蓝牙断开指令
		int BleOrderCount[10];//存放蓝牙指令延迟时间
		int BleGetSettingFlag[16];//为了让串口被关掉时连接函数也被关掉，特设此标志位
		CString BleMacAddress[16];//存放蓝牙Mac地址的
		deque<int> ComFreeDeq;//存放空闲的串口
		deque<int> ComoOccupancydDeq;//存放被占用的串口
		CWinThread* BLESetting_Thread_Handle;//蓝牙线程

		int BLEGetSettingFlag;//这个是用来卡获取配置用的标志位，当串口扫描枪扫入MAC地址，并建立起蓝牙连接后才可以继续获取配置，-1表示还没获取配置，0代表正在获取配置，2表示获得MAC但是连接失败，然后就会直接获取配置失败，1表示连接成功可以继续获取配置
		BOOL MacScanEnbleFlag;//Mac地址是否允许扫描标志位，如果为FALSE，表示此时正在进行蓝牙连接，不允许扫入，反之，则允许扫入并连接此蓝牙地址
		BOOL BLEGetSettingEndFlag;//这个是配置是否已经获取完成的标志位

		BOOL GetBluetoothCheckValue;//蓝牙复选框变量

		afx_msg void OnBnClickedResetbleButton();//重置蓝牙配置的标志位，使得蓝牙可以重新获取配置
		afx_msg void OnBnClickedBluetoothCheck();//蓝牙复选框点击函数
		void GetBleOrder();//获取蓝牙配置文件里的指令同时初始化整机测试的变量
		void BleStartPortTest(CString MACStr);//根据ComFreeDeq队列的第一位来开始某个端口
		void BleStopPortTest(int PortNo);//因为是通过扫描枪扫描，然后分配MAC给串口，所以无论成功还是失败之后都要停止该串口
		void DeleteComoOccupancydDeq(int PortNo);//从队列中删除已经测试完的端口号
		void BluetoothCoreFunction(CString MACStr);//蓝牙整机测试核心函数，里面包括获取配置时的蓝牙连接和开始测试时的蓝牙连接
		int BluetoothConnect(int PortNo, CString MACStr, CComboBox* m_Port, CComboBox* m_Baud);//蓝牙连接函数
		void BluetoothHint(int PortNo, CString str);//整机测试串口提示用的函数
		void BluetoothDisconnect(int PortNo);//蓝牙断开函数


	/*Dongle蓝牙适配器透传模块*/

	//注意：Dongle蓝牙适配器与耳标研发部门自己设计的蓝牙透传不通用
	public:
		deque<int> DongleTestDataDeq;//给当前蓝牙测试端口排好队，确保多线程下只有一条线程在访问DongleTestDataArray数组，避免数据访问冲突
		//CString DongleTestDataArray[8];//存放当前端口正在测试的蓝牙地址，用于多路蓝牙测试和蓝牙扫描枪分配
		CString DongleInfo[16][2];//存放MAC地址和令牌，MAC地址用于当作芯片ID上传数据库，令牌用于发送指令
		CString DongleInitCommand[16];//Dongle蓝牙适配器初始化指令，预设16个指令位
		CString DongleSPCommand[4][2];//Dongle蓝牙适配器特殊指令，0为连接指令，1为断开指令，2为获取令牌指令，3为获取版本号
		int DongleSpCommandSleepTime[4];//Dongle蓝牙适配器特殊指令对应的等待时间
		int DongleInitCommandNumber;//初始化指令条数
		int DongleComStartCount;//Dongle蓝牙端口开始数目，用于扫描枪的自动分配，防止将MAC地址分配到无蓝牙连接的端口

		BOOL GetDongleCheckValue;//Dongle蓝牙复选框变量
		BOOL DongleConnectFlag[16];//Dongle蓝牙适配器连接标识，FALSE为未连接，TRUE为已连接
		BOOL DongleCheckRssiFlag[16];//Dongle蓝牙适配器检测强度标识，初始设置为FALSE，在蓝牙测试完后会设置为TRUE，此时CheckConnect_Thread函数就会去检测蓝牙强度，判断蓝牙设备是否被拿走
		int DongleConnectCount[16];//记录蓝牙连接次数，Dongle蓝牙适配器达到一定连接次数后需重置一下，清缓存
	

		int DongleSleepTime;//蓝牙停顿时间
		int DongleConnectRssi, DongleDisonnectRssi;//蓝牙连接强度和蓝牙断开强度
		int DongleSDURTime;//蓝牙扫描时间
		int DongleResetCount;//蓝牙连接X次后就对适配器进行重新初始化
		int DongleScanGunFlag;//蓝牙扫描枪标志位，为真时代表有数据扫入，为假时未分配到MAC地址的串口线程会处于等待状态

		BOOL DongleInit(int HandleNum);//初始化蓝牙适配器
		void DongleInitSetting();//蓝牙设置（变量等）初始化，读ini文件
		BOOL DongleScan(int HandleNum,int RssiStr,CString SoftModel);//蓝牙自动扫描连接
		BOOL DongleConnect(int HandleNum);//连接蓝牙
		BOOL DongleDisConnect(int HandleNum);//断开蓝牙
		BOOL DongleScanGun(int HandleNum);//蓝牙扫描枪功能函数

		CString DongleValueAnalyze(CString CommandName,CString CommandValue);//对蓝牙返回值进行解析
		int DongleCommandNameToInt(CString CommandName);//对特殊转换进行一个包装处理(字符串转int，用于switch)

		CString DongleUnixTimeToDatatime(CString str);//时间戳Unix timestamp转换
		CString DongleHexToASCII(CString str);//字符串十六进制转ASCII码
		CString DongleTemperatureToCString(CString str);//计算温度
		int HexToDec(CString str);//十六进制转十进制
		CString StampToDatetime(time_t nSrc);//整形转换成时间戳
		BYTE * IMEIWrite_MulAT::CStrToByte(int len, CString str);//CString转Byte

		CString Encrypt(CString plainText);//AES加密函数
		CString Decrypt(CString plainText);//AES解密函数


		afx_msg void OnBnClickedDonglebleCheck();
	
		
	/*Dongle蓝牙扫描信号强度新增功能*/
		deque<int> DongleTestRssiDeq;//给蓝牙测信号，访问信号存放数组排个队，如果是蓝牙适配器进入队列，则会进行插队操作(不为空的情况下会排在第二位)
		static map<CString, int>DongleRssiMap;//存放蓝牙地址和信号
		BOOL DongleStatusArray[8];//蓝牙是否开始的状态，只要存在有1就表示需要进行搜索，全部为0表示要停止搜索并清空整个DongleRssiMap
		CString DongleTestRssiInfoArray[8];//存放蓝牙MAC地址

		BOOL DongleScanRssiFun(int HandleNum, CString SoftModel);//蓝牙扫描函数
		BOOL DongleScanGunOnly(int HandleNum);//蓝牙扫描枪功能函数

		CString DongleRssiArray[24][3];//
		int DongleRssiArrayCount;//计算数组目前现有的MAC地址数量

		BOOL GetDongleScanCheckValue;

	/*网络摄像头新增功能*/
	public:
		typedef TServerScoket *PServer;
		PServer SocketServer[8];//声明一个Socket类指针数组
		UINT SocketServerPort[8];//Socket类的端口号
		CString SocketServerPicName[3][8];//网络摄像头图片名字，也可当文件夹名字，图片存放路径为：.//图片缓存/日期/端口号/图片名字

		BOOL ShowSocketPicFunction_Thread(CString RecString, int HandleNum, CEdit* m_Result, CEdit* Final_Result_Control, CThumbnailBoxDlg* picdlg, CString ChipRfIDbg, BOOL DayTimeStatep);
		void StartShowSocketPic(int HandleNum, CEdit* m_Result, CEdit* Final_Result_Control, CString RecString);
		BOOL Show_SocketPic_Data(CString RecString, CEdit* m_Result, CEdit* Final_Result_Control, CThumbnailBoxDlg* picdlg, int HandleNum, CString StartSign, CString EndSign, CString ChipRfIDbg, BOOL DayTimeStatep);		//获取图像端口数据
		BOOL Get_SocketWifi_Data(CEdit* m_Result, CEdit* Final_Result_Control, int HandleNum, CString StartSign, CString EndSign, BOOL InThread = FALSE);	//通过Socket获取按钮WIFI数据
		BOOL ImageAutoJudgeDarkCorner(int HandleNum,CString ImageSrc,int RGB,float Range);//图像暗角判断，参数1图像路径，参数2像素阈值（越小就对越对黑色的判定越宽松），参数3为范围，获取的值小于这个范围返回真，否则返回假
		BOOL ImageAutoJudgeDefinition(int HandleNum,CString ImageSrc, float Range);//图像清晰度判断，参数1图像路径，参数2范围，大于这个范围返回真，小于则返回假

		void CreateDirectoryRecursionFun(CString Src, int StartCount=0);//递归创建目录
		CString GetDatetime();//获取当天日期
		CString GetCurrenttime();//获取当前时分秒
		CString GetTime();//获取系统时间


	/*电流测试新增功能*/
		CString PowerControlInstrNameArray[2];//程控电源设备连接PC的串口名，这里最多采用两台仪器
		CString PowerControlRelayInstrName;//继电器串口号
		int PowerControlRelayPortInstrName[8];//继电器端口号
		CString BackUpPowerInstrName;//备用电源串口号
		double m_Voltage;//电压值
		double m_Range;//量程
		BOOL RelayFlag, BackuppowerFlag;

		typedef CurrentTest *PCurrentTest;
		PCurrentTest CurrentTestArray[2];
		deque<int> Current1Deq,Current2Deq;//存放当前待测以及正在测试的电流端口
		double PortCurrentVauleArray[8];//存放各个端口的电流值

		void PowerControlInitSetting();//初始化程控电源的一些参数（包括仪器地址等）
		BOOL CurrentMainControlFun(int HandleNum);//电流测试核心函数
		void SetVoltageAndRangeVaule(CString ConfigItem,double &m_Voltage,double &m_Range);//设置电压和量程值，如果订单配置里存在电压和量程就获取配置中的值，如果不存在则获取ini中的值
		void SetHighVoltageAndDelayVaule(CString ConfigItem, double &m_HighVoltage, double &m_Delay);
		BOOL CurrentTestFun();//电流测试流程复用整合
		BOOL VoltageTestFun();//电压测试流程复用整合

		afx_msg void OnBnClickedPowercontrolsettingButton();


	/*新增字段防重复功能*/

		CString AntiDupDataArray[16][AntiDupData];//表示[串口号][字段]
		CString AntiDupDataVauleArray[16][AntiDupData];//表示[串口号][值] ,也就是字段对应的值

		CString AntiDupDataLinkOrder[16];//存放Link指令
        int AntiDupDataLinkFlag[16][2];//0列表示查出来的link是否是对应数值(0无此指令，1数值正确，2数值错误)，1列表示是否执行了写LINK，写了的话失败后要将Link置为空
		
		BOOL AntiDupDataSNUploadFlag[16];//表示是否上传SN号，为TRUE时上传SN到Gps_Data_AntiDup表
		BOOL AntiDupDataNoUploadFlag[16];//表示是否上传字段，为TRUE时上传SN到Gps_Data_AntiDup表
	    int AntiDupDataNoUploadCount[16][1];//表示要上传的字段数量，值越大表示要上传的字段越多

		void AntiDupDataInit(int HandleNum);//字段防重复功能变量初始化

		//上传和检查要分开
		BOOL Data_AntiDupSNCheck(CAdoInterface& myado, int HandleNum, CString ChipIDStr);
		BOOL Data_AntiDupSNUpload(CAdoInterface& myado, int HandleNum, CString ChipIDStr);

		//上传和检查要分开
		BOOL Data_AntiDupDataNoCheck(CAdoInterface& myado, int HandleNum, CString ChipIDStr);
		BOOL Data_AntiDupDataNoUpload(CAdoInterface& myado, int HandleNum, CString ChipIDStr);


	/*DAM继电器新增功能*/
		BOOL ExternalCircuit[16][9];
		CString ParaItemName[16];//当前测试项名字

		CString UART_CRC16_Work(unsigned char CRC_Buf[], int CRC_Leni);
		CString PowerSet(int Address, bool PowerAction);

		BOOL CloseExternalCircuit(int HandleNum);//关闭外电的函数
		BOOL DamControlFun(int HandleNum, CString OrderNum);//发送继电器命令函数，为真时表示发送成功，为假时表示发送失败
		BOOL CheckAutoTestChoose;
		afx_msg void OnBnClickedCheck45();



		afx_msg void OnBnClickedDongleblescanCheck();

	/*RDA平台新增功能*/
		RDAHostInterface RdaHostInterface;


		BOOL GetRDAHostCheckValue;

		void OnGetWebSetting_ThreadFun();//获取配置线程函数
		afx_msg void OnBnClickedRdahostCheck();
};
