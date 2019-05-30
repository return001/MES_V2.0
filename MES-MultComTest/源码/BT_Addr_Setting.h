#pragma once

#include "AdoInterface.h"
#include "afxwin.h"
#include "reportctrl.h"
#include "afxcmn.h"
#include "Para1.h"

// CBT_Addr_Setting 对话框


class CBT_Addr_Setting : public CDialog
{
	DECLARE_DYNAMIC(CBT_Addr_Setting)

public:
	CBT_Addr_Setting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBT_Addr_Setting();

// 对话框数据
	enum { IDD = IDD_ALLSETTING };
	CAdoInterface myado;
	CAdoInterface Mulmyado[16];
	//CPara1 m_para1;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	//afx_msg void OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult);  //自定义类
	//afx_msg void OnRvnItemClick(NMHDR* pNMHDR, LRESULT* pResult);
/*
public:
	void ImportListData(int paraNum,PARAMETER paraArray[]);
	int LoadIniFile(CString strLoadFile,PARAMETER paraArray[]);
	void LoadParaList(CString strLoadFile);//装载测试项相关设置


	void SaveBLE_Setting(CString strLoadFile);//保存设置
	int  ExportListData(void);//(PARAMETER exportArray[]);	
	void SaveIniFile(CString strSaveFile,int paraNum,PARAMETER paraArray[]);*/


	DECLARE_MESSAGE_MAP()
public:
	// 蓝牙地址信息
	CString BtAddrStart;
	CString BtAddrEnd;
	// 连接数据库
	BOOL DB_CONNECT;
	// 数据库信息
	CString m_server;
	CString m_db;
	CString m_user;
	CString m_pwd;
	// 上传信息
	CString Order_Num_up;
	CString Machine_Type;
	CString Modulation_Tppe;
	CString Line_Number;
	CString Floor;
	CString Product_Address;
	CString Is_Return;
	CString Software_Version;
	CString Operator;
	CString BT_Addr_EndShow;
	CString Hardware_Version;
	// 选择IMEI1
	BOOL IMEI1_Choose;
	BOOL IMEI2_Choose;
	BOOL IMEI3_Choose;
	BOOL IMEI4_Choose;
	// 写蓝牙MAC地址
	BOOL BT_MAC_ADDR_Choose;
	// WIFI_MAC地址
	BOOL WIFI_MAC_ADDR_Choose;
	// 检查IMEI是否合法
	BOOL IMEI_CheckValid_Choose;
	// 选择是否自动生成IMEI
	BOOL AutoIMEI_Choose;
	// 检查软件版本选择
	BOOL Software_Version_Check_Choose;
	// 超时时间设置
	CString WatchDog_Time;
	// 自动生成IMEI号时，设置的IMEI起始号的高14位
	CString AutoIMEI_Start_Num;
	// 选择采用相同的IMEI
	BOOL UseSameIMEI_Choose;
	// 自动生成蓝牙地址选择
	BOOL AutoBTAddr_Choose;
	// WIFI起始地址前几位
	CString AutoWIFIAd_Start_Num;
	// 自动生成WIFI地址
	BOOL AutoWIFIAddr_Choose;
	// 手机模式选择
	CComboBox TestMode_Control;
	CString TestMode_S;
	int TestMode_N;
	// WIFI起始地址
	CString WIFIAddrStart;
	CString WIFIAddrEnd;
	// IMEI起始号
	CString IMEIStart;
	CString IMEIEnd;
	// 数据库来关联IMEI1
	BOOL IMEI_DB_Choose;
	// BT\WIFI_MAC 地址关联IMEI1选择
	BOOL MAC_DB_Choose;
	// 上传MSN到数据库选择
	BOOL MSN_Upload_Choose;
	// 连接数据库检查是否重号
	BOOL Check_repeat_Choose;
	// 初始化的时候选择是否连接数据库
	BOOL Initial_Connect_DB;
	// 测试成功后上传数据到数据库
	BOOL UP_DB_Data;
	// 读出话机的IMEI\BT_WIFI_MAC与输入的对比
	BOOL IMEI_MAC_Check_Choose;
	// 扫描枪模式选择
	BOOL Scan_Choose;
	// 工具使用权限控制
	CButton Software_Version_Check_Choose_Control;
	CButton IMEI_MAC_Check_Choose_Control;
	CButton IMEI_CheckValid_Choose_Control;
	// 权限号
	CString ServerIP_MAC;
	// 自动生成号段时，自动循环开始写号
	BOOL Repeat_AutoStart;
	// ORACLE数据库连接选择
	BOOL ORACLE_Connect_Choose;
	// ORACLE数据库服务器地址
	CString ORACLE_m_server;
	CString ORACLE_m_user;
	CString ORACLE_m_pwd;
	// 选择板号
	BOOL BARCODE_Choose;
	// IPServer_MAC使能
	BOOL ChangeEN_Choose;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl m_tab;
	afx_msg void OnBnClickedOk();
	// 是否生产解锁码
	//CButton Unlock_Code_Choose;
	BOOL Unlock_Code_Choose;
	// 解锁码的位数控制
	CComboBox Unlock_code_Control;
	int Unlock_code_N;
	CString Unlock_code_S;
	// 网标选择
	BOOL NETCODE_Choose;
	// 网标只为数字
	BOOL NETCODE_Only_Num_Choose;
	// 仅上传数据库选择
	BOOL JUST_UpDate_DB_Choose;
	// 更新IMEI-MSN表中的已使用标志
	BOOL UpdateIMEI_MSN_Choose;
	// 网标起始号
	CString NetCodeStart;
	CString NetCodeEnd;
	// 后几位是流水号
	CString NetCode_SwiftNumber;
	// 写好表已经存在IMEI,将更新对应的网标
	BOOL IMEI_Exist_Update_NC_Choose;
	// 根据网标查询当前的机型来对比
	BOOL Machine_Type_NC_CompareChoose;
public:
	void FileSelectWizard(TCHAR *pszDir);
	// LOG转存共享文件夹
	CString DestFile;
	afx_msg void OnBnClickedButtonstart12();
};
