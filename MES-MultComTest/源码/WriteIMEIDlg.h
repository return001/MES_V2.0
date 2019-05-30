// WriteIMEIDlg.h : header file
//

#if !defined(AFX_WRITEIMEIDLG_H__B59F17C9_CC97_4D41_B3AB_136C646D895A__INCLUDED_)
#define AFX_WRITEIMEIDLG_H__B59F17C9_CC97_4D41_B3AB_136C646D895A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColReadOnlyEdit.h"
#include "afxwin.h"

#include "IniFile.H"
#define		STRING_PATH_CONFIG		"\\config\\Seting_CONFIG.dll"
#define		STRING_SECTION_CONFIG		"SETTING"

#define		SERVER_RESULT		"\\tem.dll"
#define		SERVER_RESULT_SECTION		"T_SERVER_SETTING"

#define     STRING_PATH_BLE         "config\\BLE_ITEM.dll"
#define     STRING_SECTION_BLE      "BLE"

#define     MAXPARANUM       100

#include "BT_Addr_Setting.h"
#include "Login.h"
#include "PhoneCommand.h"       //展讯平台

#include "UDP.h"
#include "METAAPP.h"
#include "sp_brom.h"
#include "spmeta.h"

//SOCKET通信
#include "ActiveSock.h"
#include "EventWrapper.h"
/////////////////////////////////////////////////////////////////////////////
// CWriteIMEIDlg dialog
/*typedef enum _MOCOR_VER_E
{
	MOCOR_VER_PRE09A37=0,
	MOCOR_VER_AFTER09A37
} MOCOR_VER_E;   */

#define destPORT 137  //nbtstat name port
#define myPORT 4321

#define USB_COM_PORT  9999
#define BT_ADDRESS_MAX_LENGTH  12   
#if !defined(STRUCT_PARAMETER)
#define STRUCT_PARAMETER
//结构体1
typedef struct {
	signed int iMeta_handle;
	signed int iMetaAP_handle;
	AUTH_HANDLE_T  t_AuthHandle;
	signed int bootstop;

	bool bIsDatabaseInitialized [2]; //[0] for Modem; [1] for AP
	bool bAuthenEnable;
	bool bIsConnected;

} MetaCommon_struct;
static MetaCommon_struct g_Meta;  


//结构体2
typedef struct {

	META_Connect_Req tMETA_Connect_Req;
	META_Connect_Report tMETA_Connect_Report;
	METAAPP_RESULT eMetaApp_Result;
	//E_INPUTBOX_STATUS eRunStatus;//运行状态

		
	META_RESULT eMeta_Result;
	FT_NVRAM_WRITE_REQ tNVRAM_WriteReq;
	FT_NVRAM_READ_REQ tNVRAM_ReadReq;
	FT_NVRAM_READ_CNF tNVRAM_ReadCnf;
	
	HANDLE m_WriteToNVRAMEvent;
	HANDLE m_ReadFromNVRAMEvent;

	META_ConnectByUSB_Req usb_req;
	META_ConnectByUSB_Report usb_report;

}  MetaModem_struct;
static MetaModem_struct g_MetaModem;    

#define META_CONNECT_TIME_OUT  30000  //30000   
#define META_BOOT_TIMERS  3   

//DUT类型
typedef enum {
	TARGET_FEATURE_PHONE = 0,
	TARGET_SMARTPHONE,
	PCMCIA_CARD,
	
} E_TARGET_TYPE;

typedef struct {

	META_ConnectInMETA_Req tMETA_Connect_Req;
	META_ConnectInMETA_Report tMETA_Connect_Report;
	META_RESULT eMeta_Result;
	META_RESULT eMetaAP_Result;    //新加
	METAAPP_RESULT eMetaApp_Result;//新加
	//WM_META_ConnectInMETA_Req tMETA_Connect_ReqWM;      //新加
	//WM_META_ConnectInMETA_Report tMETA_Connect_ReportWM;//新加
	//E_INPUTBOX_STATUS eRunStatus;
	FT_NVRAM_WRITE_REQ tNVRAM_WriteReq;
	FT_NVRAM_READ_REQ tNVRAM_ReadReq;
	FT_NVRAM_READ_CNF tNVRAM_ReadCnf;
	//EXEC_FUNC cb;
	HANDLE m_WriteToNVRAMEvent;
	HANDLE m_ReadFromNVRAMEvent;
} MetaModem6516_struct;
static MetaModem6516_struct g_Meta6516Modem; 

//------------------------------------------------------------------------------
// New boot mode for SP                                                               
static SP_BOOT_ARG_S m_stModeArg;    

typedef struct  {
	unsigned long  read_retry_time;			//     0 means default  (2400 times)
	unsigned long  read_interval_timeout;	// ms, 0 means default  (10 ms)
	int			 * m_p_stopflag;
}Meta_Boot_Arg;

//Errors
typedef enum 
{
	EBOOT_SUCCESS = 10000,
	EBOOT_EXCEPTION,

	//USER
	USER_PRESS_STOP_BUTTON,
	WAIT_FOR_PLATFORM_ERROR,

	// COM PORT ERROR
	OPEN_COM_PORT_ERROR = 11000,
	SETUP_COM_PORT_ERROR,
	GET_COM_STATE_ERROR,
	SET_COM_STATE_ERROR,
	PURGE_COM_PORT_ERROR,
	SET_COMM_TIMEOUTS_ERROR,

	//Boot ERROR
	READ_READY_PATTERN_ERROR = 12000,

	WRITE_META_PATTERN_ERROR,
	READ_META_ACK_PATTERN_ERROR,

	WRITE_ADVMETA_PATTERN_ERROR,
	READ_ADVMETA_ACK_PATTERN_ERROR,

	WRITE_DOWNLOAD_PATTERN_ERROR,
	READ_DOWNLOAD_ACK_PATTERN_ERROR,

	WRITE_FACTORYM_PATTERN_ERROR,
	READ_FACTORYM_ACK_PATTERN_ERROR,

	WRITE_FACTFACT_PATTERN_ERROR,
	READ_FACTFACT_ACK_PATTERN_ERROR,

	WRITE_SWITCHMD_PATTERN_ERROR,
	READ_SWITCHMD_ACK_PATTERN_ERROR,

	WRITE_MPWAIT_PATTERN_ERROR,
	READ_MPWAIT_ACK_PATTERN_ERROR,

	WRITE_MPGOON_PATTERN_ERROR,
	READ_MPGOON_ACK_PATTERN_ERROR,

	WRITE_CLEAN_BOOT_PATTERN_ERROR,
	READ_CLEAN_BOOT_ACK_PATTERN_ERROR,

	//USB PORT ERROR
	OPEN_USB_PORT_ERROR = 13000,
	USB_PORT_READ_FILE_ERROR,
	USB_PORT_READ_FILE_TIMEOUT_ERROR,
	USB_PORT_WRITE_FILE_ERROR,
	USB_PORT_READ_FILE_LEN_ERROR,
	USB_PORT_WRITE_FILE_LEN_ERROR,
	USB_PORT_SET_TIMEOUT_ERROR,
	USB_PORT_TOO_MANY_ERROR,
	USB_PORT_LOAD_WINDEV_DLL_ERROR,

	//ANDROID DOWNLOAD ERROR
	NOT_FLASH_INFO_PKT_ERROR = 14000,
	NOT_PL_INFO_PKT_ERROR,
	NOT_IMAGE_INFO_PKT_ERROR,
	NOT_RESP_INFO_PKT_ERROR,
	NOT_FOUND_INFO_PKT_ERROR,
	OPEN_IMAGE_FILE_ERROR,
	GET_IMAGE_FILE_SIZE_ERROR,
	IMAGE_FILE_PATH_IS_NULL_ERROR,
	READ_IMAGE_FILE_ERROR,
	WRITE_IMAGE_FILE_ERROR,
	SEND_IMAGE_FILE_ERROR,
	UNKNOWN_SEND_IMAGE_CHECK_PACKET_ERROR,
	SEND_PATTERN_ERROR,
	SEND_PATTERN_SIZE_ERROR,
	ANDROID_VERIFY_FAIL_ERROR,

	//Secured download
	CHECK_SECURED_MARK_ERROR,
	GET_ENCRYPT_INFO_ERROR,
	GET_ENCRYPT_DATA_ERROR,
	SET_FILE_POINTER_ERROR,

	//DA download error
	SEND_DA_VERIFY_DATA_ERROR,
	RECEIVE_DA_VERIFY_RESPONSE_ERROR,
	SEND_DA_INFO_ERROR,
	SEND_DA_DATA_ERROR,
	DA_LOCKED_ERROR,

         //Image patch & lock
	INFORM_IMAGE_PACKET_ERROR,
	PATCH_CMD_PACKET_ERROR,
	SEND_SECURITY_PACKET_ERROR,
	IMAGE_LOCK_RESPONSE_ERROR,
	SET_LOCK_RESPONSE_ERROR,
	RECEIVE_CHECKSUM_ERROR,
	META_LOCKED_ERROR,

	//Partition table checking
	SEND_PARTITION_INFO_PACKET_ERROR,
	RECEIVE_PARTITION_RESPONSE_ERROR,

	//Target response error message
	TARGET_NOMEM_ERROR        = 16000,        /* no memory */
	TARGET_NAND_RD_ERROR,
	TARGET_NAND_WR_ERROR,
	TARGET_NAND_ER_ERROR,
	TARGET_WRONG_SEQ_ERROR,
	TARGET_WRONG_ADDR_ERROR,
	TARGET_WRONG_PKT_SZ_ERROR,
	TARGET_EXCEED_BOUNDARY_ERROR,
	TARGET_INVALID_TABLE,
	TARGET_SPACE_NOT_FOUND,
	TARGET_UNKNOWN_ERROR,

	//Other Error
	OPEN_FLASH_BIN_FILE_ERROR    = 20000,
	READ_FLASH_BIN_FILE_ERROR,
	ALLOC_MEMORY_ERROR,

	EBOOT_RESULT_END = 0x7FFFFFFF,

}EBOOT_RESULT;

typedef enum
	{
		PreloaderUSB = 0,
		BootROMUSB = 1
	}USBType;

static const GUID GUID_PORT_CLASS_USB2SER ={0x4D36E978L,0xE325,0x11CE,{0xBF,0xC1,0x08,0x00,0x2B,0xE1,0x03,0x18}};


typedef struct {

	//META_ConnectInMETA_Req tMETA_Connect_Req;
	//META_ConnectInMETA_Report tMETA_Connect_Report;
	META_RESULT eMeta_Result;
	META_RESULT eMetaAP_Result;    //新加
	METAAPP_RESULT eMetaApp_Result;//新加
	WM_META_ConnectInMETA_Req tMETA_Connect_Req;      //新加
	WM_META_ConnectInMETA_Report tMETA_Connect_Report;//新加
	//E_INPUTBOX_STATUS eRunStatus;
	FT_NVRAM_WRITE_REQ tNVRAM_WriteReq;
	FT_NVRAM_READ_REQ tNVRAM_ReadReq;
	FT_NVRAM_READ_CNF tNVRAM_ReadCnf;
	//EXEC_FUNC cb;
	HANDLE m_WriteToNVRAMEvent;
	HANDLE m_ReadFromNVRAMEvent;
} MetaAP_struct;
static MetaAP_struct g_MetaAP;  


//WIFI地址
#define EEPROM_SIZE 512  


typedef struct
{
    CString paraID;     //指令唯一的ID号//没用上
    CString paraContent;//对应的指令内容//没用上

	bool     ifCheck;//是否选中
	CString showName;         //显示在列表中的参数名
    CString Low_Limit_Value;  //返回值
	CString High_Limit_Value; //返回值数字范围
	CString Other_ITEM;       //指令
}PARAMETER_MAIN;

//写NVRAM数据类型，用于写MT6592芯片的BT地址
typedef enum
{
    WRITE_BARCODE = 0,
    WRITE_IMEI,
    WRITE_BT,
    WRITE_WIFI,
    WRITE_ETHERNET_MAC,
    WRITE_PRODINFO,
    WRITE_TEMPERATURE
}WriteData_Type_e;


#define MT5921 0x5921
#define MT5931 0x5931
#define MT6620 0x6620
#define MT6628 0x6628
#define MT6571 0x6571
#define MT6572 0x6572
#define MT6582 0x6582
#define MT6592 0x0092
#define MT6595 0x6630
#define MT8127 0x8127

#endif
////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

class CWriteIMEIDlg : public CDialog
{
// Construction
public:
	int autow;
	int TotalCount;
	void PrintError(CString cs);
	CWriteIMEIDlg(CWnd* pParent = NULL);	// standard constructor
	bool INITIAL_PROCESS;//第一次运行时，IMEI要变化，异常处理
	int  INITIAL_Paint;  //初始运行，移动窗口
// Dialog Data
	//{{AFX_DATA(CWriteIMEIDlg)
	enum { IDD = IDD_WRITEIMEI_DIALOG };
	CComboBox	m_Auto;
	CStatic	m_Static_Type;
	CButton	m_Write;
	CComboBox	m_Type;
	CComboBox	m_Baud;

	CString IMEI_OK_LAST;   //调试问题

	CEdit	m_Result;
	CString LOG_Show;
	CString LOG_Show_Old;
	CString LOG_Show_Old_ALL;
	CString LOG_Time;
	CComboBox	m_Port;
	CColReadOnlyEdit	m_Finalresult;
	CString	m_IMEI;
	CString BoardNumber;
	CString Software_version;
	CString IMEI_Num;

	CString Order_IMEI;
	CString Order_BT;
	CString Order_Software_version;
	CString Order_Barcode;

	CString Order_Recover_Status;
	CString Order_Copy_Para;     //数据备份

	//解锁码
	CString Unlock_code_Number;
	//}}AFX_DATA


	//连接数据库
	CString   UP_DB_Data_M;
	CString   Initial_Connect_DB_M;
	CString	  DB_CONNECT_M;
	CString   UP_ONE_OK_SECOND_FAIL;
	CString   UP_SN_COPY;
	CString   UP_BT_COPY;
	CString   UP_BARCODE_COPY;
	CString   UP_IMEI_COPY;

	//IMEI
	CString   IMEI1_Choose_M;
	CString   IMEI2_Choose_M;
	CString   IMEI3_Choose_M;
	CString   IMEI4_Choose_M;
	CString   IMEI_CheckValid_Choose_M;
	CString   AutoIMEI_Choose_M;

	CString   UseSameIMEI_Choose_M;
	CString   IMEI_DB_Choose_M;
	CString   MAC_DB_Choose_M;
	
	//BT_MAC
	CString   BT_MAC_ADDR_Choose_M;
	CString   AutoBTAddr_Choose_M;

	CString   WIFI_MAC_ADDR_Choose_M;
	CString   AutoWIFIAddr_Choose_M;
	//板号
	CString  BARCODE_Choose_M;
	CString  ChangeEN_Choose_M;
	//MSN
	CString   MSN_Upload_Choose_M;
	//ORACLE数据库连接
	CString    ORACLE_Connect_Choose_M;
	//重号
	CString   Check_repeat_Choose_M;

	CString   Software_Version_Check_Choose_M;
	CString   IMEI_MAC_Check_Choose_M;

	CString   Scan_Choose_M;

	CString   Repeat_AutoStart_M;

	//解锁码
	CString  Unlock_Code_Choose_M;

	//入网标
	CString NETCODE_Choose_M;
	CString NETCODE_Only_Num_Choose_M;
	CString JUST_UpDate_DB_Choose_M;
	bool IMEI1_Exist;

	CString IMEI_Exist_Update_NC_Choose_M;

	CString Machine_Type_NC_CompareChoose_M;
	//更新IMEI_MSN对应表中的使用标志
	CString UpdateIMEI_MSN_Choose_M;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWriteIMEIDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CWriteIMEIDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnWrite();
	afx_msg void OnSelchangeAuto();
	afx_msg void OnChangeImei();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CBT_Addr_Setting Setdlg;
	CBT_Addr_Setting Setdlg_Oracle;
protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd*, CPoint point);

	afx_msg HBRUSH OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor); 
public:
	void  SaveConfig();
	void  SaveConfig_Change();
	void  LoadConfig();
	void  LoadConfig_Change();
	void  LoadResult_Time();//获取服务器信息
	void  SaveResult_Time();
	int  CharToHexChar(char ch);
	CEdit IMEI_Control;
	CStatic m_Static_Type_BT;
	CString m_BT_Addr;
	CString m_BT_Addr_Temp1;
	CString m_BT_Addr_Temp2;
	CString string_BT;
	long BtAddrCount_int;
	long BtAddrTotal_int;

	long IMEI_Count_int;

	
	afx_msg void OnBnClickedButtonstart1();
	// 蓝牙地址计数
	//CString BtAddrCount_whole_situation;
	CString BtAddrCount;
	CString BtAddrTotal;
	CString BtAddrStart_M;
	CString BtAddrEnd_M;
	CString m_Result_C;
	//WIFI地址计数
	CString WIFIAddrStart_M;
	CString WIFIAddrEnd_M;
	long    WIFIAddrCount_int;
	long    WIFIAddrTotal_int;
	//网标
	CString NetCodeStart_M;
	CString NetCodeEnd_M;
	CString NetCode_SwiftNumber_M;
	//IMEI号
	CString IMEIStart_M;
	CString IMEIEnd_M;
	//上传信息
	CString Order_Num_up_M;
	CString Machine_Type_M;
	CString Modulation_Tppe_M;
	CString Line_Number_M;
	CString Floor_M;
	CString Product_Address_M;
	CString Is_Return_M;
	CString Software_Version_M;
	CString Hardware_Version_M;
	CString ServerIP_MAC_M;      //包含使用次数设置
	CString ServerIP_MAC_M_NoTimes;
	long    USE_Times_Count;     //使用次数计数
	CString Plat_Limit_Enable;   //平台选择功能权限限制
	CString WatchDog_Time_M;
	CString AutoIMEI_Start_Num_M;//自动生成IMEI号的起始号的前14位，传递给参数    m_int64DefaultIMEI

	CString AutoWIFIAd_Start_Num_M;

	CString Operator_M;
	CString BT_Addr_EndShow_M;
	//连接数据库
	CString m_server_M;
	CString	m_db_M ;
	CString	m_user_M;
	CString	m_pwd_M ;

	//连接ORACLE数据库
	CString ORACLE_m_server_M;
	CString	ORACLE_m_user_M;
	CString	ORACLE_m_pwd_M ;


	CString DestFile_M;




	CString sTemp,szTemp;
	CEdit SN_Control;
	CString m_SN;
	int IMEIorSN;
	// SN的位数
	int SN_Count;
	CString SN_Count_S;
	CString SN_PRE;
	CString IMEI_PRE;
	CEdit IMEI_PRE_Control;
	CEdit SN_PRE_Control;
	CEdit SN_Count_Control;
	afx_msg void OnBnClickedButton2();
	CString m_Type_S;
	int m_Type_N;

	//平台保存
	CString Plat_Form_S;//非控件变量
	//CString Plat_Form_CS;//局部变量
	//int  Plat_Form_N;    //不需要

	afx_msg void OnEnChangeImei3();
	void MAIN_Function();//测试主函数
	// 等待开机时间
	long Wait_time;
	CString Wait_time_S;
	// msn从数据库中获取
	BOOL MSN_Relative_IMEI;
	CString MSN_Relative_IMEI_S;
	CButton MSN_Relative_IMEI_Control;

	BOOL BT_Write_Enable;
	CString BT_Write_Enable_S;
	CButton BT_Write_Enable_Control;
	CEdit m_BT_Addr_Control;

	CString Machine_Type;

	BOOL Recover_Status;
	CButton Recover_Status_Control;
	CString Recover_Status_S;

	// 检查GSM、BT耦合工位使能
	BOOL CHECK_GSM_TEST_Enable;
	BOOL CHECK_BT_TEST_Enable;
	CButton CHECK_GSM_TEST_Enable_Control;
	CButton CHECK_BT_TEST_Enable_Control;
	CString CHECK_GSM_TEST_Enable_S;
	CString CHECK_BT_TEST_Enable_S;
	afx_msg void OnBnClickedButton3();

	CString CheckIMEI_LOG;
	BOOL CheckIMEI();
	__int64 ComputeCD(__int64 nImei);
	// IMEI2
	CString m_IMEI2;
	CString m_IMEI3;
	CString m_IMEI4;
	// WIFI  MAC地址
	CString m_strWifi;
	//软件版本
	CString Version_SoftWare;

	HANDLE m_hEnterModeSuccess;
	HANDLE m_hExitThread;
	//HANDLE m_hEnterModeThread;
	CWinThread* m_hEnterModeThread;
	//static DWORD EnterModeProc(LPVOID lpParam);//线程函数，全局变量
	DWORD EnterModeFunc(WPARAM, LPARAM);
	void ReadBack();
	bool WriteAndThenReadBack();//展讯8810平台;wifi/btMAC写入后重启才生效（META方式）
	void WriteFun();
	BOOL CheckVersion();
	int WriteMoc1();
	int WriteMoc2();

	void ReflashUI();
	void AutoCreateIMEI(LPTSTR lpszIMEI);    //自动生成IMEI地址
	void AutoCreateBTAddr(LPTSTR lpszTempBT);//自动生成BT地址
	void AutoCreateWifiAddr(LPTSTR lpszTempWIFI);

	DWORD m_dwStartTime;
	DWORD m_dwEnterModeWait;//串口需要waiting 一定时间才进模式

	CString m_bUsbMode;//USB模式
	__int64 m_int64DefaultIMEI;
	CString AutoIMEI; //自动生成的IMEI号
	int m_nComPort;   //端口号
	//int WriteMocChoose;//芯片选择
	CString SN_MAC;
	CString MAC;

	CString   TestMode_M;//话机模式

	CString   Unlock_code_M;//解锁码的位数



	afx_msg void OnBnClickedButton4();
	// IMEI控件
	CEdit IMEI2_Control;
	CEdit IMEI3_Control;
	CEdit IMEI4_Control;
	CEdit m_WIFI_Addr_Control;


	void BT_WIFI_Choose(); //BI/WIFI地址写选择
	afx_msg void OnEnChangeImei7();
	afx_msg void OnEnChangeImei10();
	afx_msg void OnEnChangeImei9();
	afx_msg void OnEnChangeBtAddr();
	// BT_MAC地址前面几位
	CString BT_PRE;
	CEdit BT_PRE_Control;
	afx_msg void OnEnChangeBtAddr2();
	// WIFI前缀
	CString WIFI_PRE;
	CEdit WIFI_PRE_Control;
	//IMEI号段
	long    IMEIStart_Number;
	long    IMEIEnd_Number;
	//IMEI\BT_MAC\WIFI_MAC判断结果
	bool    IMEI2_Result,IMEI3_Result,IMEI4_Result,BT_Result,WIFI_Result;//用于数据库关联时，关联的结果判断
	afx_msg void OnEnChangeEdit1();
	// IMEI使用计数
	CString IMEI_Count;
	// MAC计数编辑控制的使能控制
	CEdit BtAddrCount_Control;
	// IMEI计数编辑控件控制
	CEdit IMEI_Count_Control;
	afx_msg void OnBnClickedButton5();

	SP_HANDLE g_hDiagPhone;
	CHANNEL_ATTRIBUTE ca;

	//获取服务器信息
	CString IPCommand(int Select_Order);
	void SendIP_Ping(int Select_Order);
	void OnTimerFunction(UINT nIDEvent);//非定时器
	CString PingOutput;
	void OnReceive();
	CUDP m_UDPSocket;

	//数据库服务器
	CString T_SERVER;
	CString USE_Times_Count_SERVER_CS;

	//MTK
	METAAPP_RESULT MetaAppResult;
	int HandleForMETA;
	METAAPP_RESULT Initialization(void);
	int PowerOn2Connect_SP(int WorkingMS); // Smart phone version of PowerOn2Connect

	//初始化Modem
	bool MetaModemHandleInit ();
	//初始化MetaAP
	bool MetaAPHandleInit () ;

	//初始化Authentication
	bool REQ_AUTH_Create( void );
	bool REQ_AUTH_Load(const char *filepath );
	bool AuthHandleInit ();

	//初始化SP Authentication
	bool SPATE_AUTH_Create( void );
	bool SPATE_AUTH_Load( const char *auth_filepath );
	bool SpAuthHandleInit ();

	//初始化SPATE Authentication
	//bool SPATE_AUTH_Create( void );
	//bool SPATE_AUTH_Load( const char *auth_filepath );
	bool SpSecuHandleInit ();

	//META相关内容初始化总函数
	void MetaContextInit (void);

	//META相关内容初始化清理子函数
	int REQ_AUTH_Unload( void );
	int REQ_AUTH_Destroy( void );
	//META相关内容初始化清理
	void MetaContextDeInit (void);


	CString Get_Printer_Para(CString name);//获取打印设置参数
	bool DB_UP(int Write_Result_In);						//数据库上传操作
	void OnCbnSelchangeCombo2_Fresh();	//刷新串口
	void Leadcore_Write();				//联芯4G模块写号
	//M255-串口AT
	DWORD OnWrite_Func (WPARAM, LPARAM);//M255-串口AT
	//锁网解锁码
	void TranslateMuch(CString m_old);
	//功能机写IMEI号总函数
	void swap_c ( unsigned char* x, unsigned char* y );
	DWORD func_imei_meta_hdlr_feature_phone (WPARAM, LPARAM);

	//Nvram初始化
	bool NvramDatabaseInit ();
	bool IsNvramDatabaseInit (void );
	//META连接参数设置
	void SetMetaConnectReq ( void );
	//META模式连接手机
	bool EnterMetaMode () ;


	//写蓝牙地址函数
	bool func_bt_address_meta_hdlr_feature_phone ( void );
	signed char  Ascii2BCD ( unsigned char  iAscii );
	META_RESULT REQ_WriteBT6611Addr2NVRAM_Start (unsigned int iBTId, unsigned char* pBTAddress);
	META_RESULT REQ_ReadFromNVRAM ( void );
	META_RESULT REQ_WriteNVRAM(void);

	META_RESULT REQ_ReadBT6611AddrFromNVRAM_Start ( unsigned int iBTId, unsigned char* pBTAddress );

	//写WIFI地址函数
	bool func_wifi_mac_meta_hdlr_feature_phone ( void );


	////////////////////////////智能机////////////////////////////

	//智能机写IMEI号总函数
	DWORD func_imei_meta_hdlr_smart_phone_modem (WPARAM, LPARAM);
	//智能机----META连接参数设置
	void Seg_MetaModemConnectReq ( void ) ;
	char Imei[4][16];
	//智能机----METAmodem模式连接手机
	bool EnterMetaModemMode ( void );
	int Connect_with_preloader(void);//连接子函数
	bool search_preloader_port_success;
	int search_kernel_port(void );    //连接子函数
	bool search_kernel_port_success;

	unsigned short kernel_port;

	//切换模式
	bool Switch2WM_META_Mode ( const int meta_handle );
	void Seg_MetaAPConnectReq ( void );

	/////////////////////////////////////////////
	//智能机写BT地址函数
	bool func_bt_address_meta_hdlr_smart_phone ( void );
	//写蓝牙地址函数子函数
	META_RESULT REQ_WriteBTAddr2NVRAM_Start (unsigned int iBTId, unsigned char* pBTAddress);
	META_RESULT REQ_Write2NVRAM(void);
	META_RESULT REQ_ReadFromNVRAM_AP( void );

	META_RESULT REQ_ReadBTAddrFromNVRAM_Start ( unsigned int iBTId, unsigned char* pBTAddress );

	//////////////////////////////////////////////////////////////////////////////////////////////////
	META_RESULT REQ_WriteAP_NVRAM_Start(WriteData_Type_e dataType, char *pInData, unsigned short iRID);//MT6592芯片写蓝牙地址
	META_RESULT ConductBarcodeData(char *pOutData, unsigned short RID_para, char *pInDatabuf, int bufSize);
	META_RESULT ConductBTAddrData(char *pOutData, unsigned short RID_para, char *pInDatabuf, int bufSize,bool m_bWriteNvram);
	META_RESULT ConductWifiAddrData(char *pOutData, unsigned short RID_para, char *pInDatabuf, int bufSize,bool m_bWriteNvram);
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	//智能机Nvram初始化
	bool APNvramDatabaseInit () ;
	bool IsAPNvramDatabaseInit ();
	//智能机----METAAP模式连接手机(写蓝牙地址)
	bool EnterMetaAPMode ( void );
    //智能机特殊处理
	SetCleanBootFlag_REQ req;
	SetCleanBootFlag_CNF cnf;


	//智能机写WIFI_MAC地址总函数
	bool func_wifi_mac_meta_hdlr_smart_phone_Ex ( void );
	//写WIFI地址函数子函数
	META_RESULT REQ_WriteWifiMAC2NVRAM_Start (WiFi_MacAddress_S *MACAddress);
	void Checksum_Generation(unsigned char StorageBuf[]);
	META_RESULT REQ_ReadWifiMACFromNVRAM_Start ( WiFi_MacAddress_S *MACAddress );
	//ApMeta转换Modem
	META_RESULT ApMetaToModemMeta ( void );
	META_RESULT ModemMetaToAPMeta (void);
	// MTK_META规则文件路径 Modem Database
	CString		m_strExchangeFileName;//规则文件交换路径
	CString m_sPath;
	afx_msg void OnBnClickedButton6();
	// 平台选择展讯、MTK智能机、MTK功能机
	CComboBox Plat_Form;
	// Modem_Database文件设置使能
	CButton Modem_Database_Control;
	// 等待时间设置使能
	CEdit Wait_time_Control;
	// 保存当前界面的设置
	CButton SaveCurrentSetting_Control;

	//上传ORACLE数据库
	bool Update_Oracle_Database();
	// 输入的话机板号
	CEdit BARCODE_Input_Control;
	CString BARCODE_Input;
	afx_msg void OnEnChangeBtAddr3();
	afx_msg void OnBnClickedOk();
	// MTK智能机配置文件
	CString m_sPath_AP;
	CString m_strExchangeFileName_Ap;
	afx_msg void OnBnClickedButton7();
	CButton AP_Database_Control;

	void func_four_in_one_hdlr_smart_phone ( void );

	bool   INITIAL_POWER;
	afx_msg void OnBnClickedButton8();
	CButton DB_Find_Control;
	afx_msg void OnBnClickedButtonstart10();

public:
	PARAMETER_MAIN paraArray[MAXPARANUM];
	// BLE传感器线程创建按钮
	CButton Sensor_Test_Thread;
	afx_msg void OnEnChangeNetcode();
	void Only_Update_Netcode();      //网标上传函数
	// 网标号控制
	CEdit NETCODE_Control;
	// 网标前缀
	CEdit NETCODE_PRE_Control;
	CString NETCODE_PRE;
	// 网标扫描输入
	CString NETCODE_Input;
	// 网标位数控制
	CEdit NETCODE_Count_Control;
	int NETCODE_Count;
	CString NETCODE_Count_S;

	//Story
	void Story_Show();

	//联芯4G写号LOG显示
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	// 条码打印机控制配置脚本
	CString PrinterScript;
	CEdit PrinterScript_Control;
	afx_msg void OnBnClickedButtonstart11();
	// 手动选择打印贴纸
	CButton Manul_Print_Control;
	afx_msg void OnBnClickedButtonstart13();
	afx_msg void OnBnClickedButton24();
	afx_msg void OnBnClickedButton33();
public:
	BOOL Permission();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WRITEIMEIDLG_H__B59F17C9_CC97_4D41_B3AB_136C646D895A__INCLUDED_)
