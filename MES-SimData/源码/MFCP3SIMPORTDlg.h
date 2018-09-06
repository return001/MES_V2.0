
// MFCP3SIMPORTDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "Resource.h"
#include "ThreadPool.h"
#include "Task.h"

//发送指令按钮读写线程函数
UINT SendWritePortThread(LPVOID pParam);
UINT SendReadPortThread(LPVOID pParam);
UINT DownloadPortThread(LPVOID pParam);
extern HWND MainFormHWND;
extern CString strFolderpath, strOKFolderpath;
extern CString StrFolder[4];
extern CString strSingleFilePath;
extern int simstart1flag;
extern int simstart3flag;
extern int simstart2flag;
extern int simstart4flag;
extern CString SinglePortLogName;
extern CString Port1LogName;
extern CString Port2LogName;
extern CString Port3LogName;
extern CString Port4LogName;
extern CString LastPort1RID;
extern CString LastPort1IMEI;
extern CString LastPort2RID;
extern CString LastPort2IMEI;
extern CString LastPort3RID;
extern CString LastPort3IMEI;
extern CString LastPort4RID;
extern CString LastPort4IMEI;


// CMFCP3SIMPORTDlg 对话框
class CMFCP3SIMPORTDlg : public CDialogEx
{
	// 构造
public:
	CMFCP3SIMPORTDlg(CWnd* pParent = NULL);	// 标准构造函数

	// 对话框数据
	enum { IDD = IDD_MFCP3SIMPORT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();


public:

	//字体变量与函数操作
	CFont staticHint1font, staticHint2font, staticHint3font, staticHint4font;
	COLORREF clrcolor;
	void fontinit();//初始化字体

	afx_msg LRESULT MainFontControl(WPARAM wParam, LPARAM lParam);//字体更改的消息循环

	//初始化配置模块函数以及变量
	int initconfigflag;
	CString m_simdatafolderPath, m_simdatafilePath;
	HANDLE port1handler, port2handler, port3handler, port4handler;

	afx_msg void OnBnClickedMultipledownloadRadio();
	afx_msg void OnBnClickedSingledownloadRadio();
	afx_msg void OnBnClickedOpensimdatafolderpathButton();
	afx_msg void OnBnClickedOpensimdatafilepathButton();
	void SetInitConfigWindow();

	//SIM卡数据下载模块函数以及变量
	int simconnect1flag,  simconnect2flag,  simconnect3flag,  simconnect4flag,  simallconnectflag, simallstartflag;
	CString strsinglecrc16, strport1crc16, strport2crc16, strport3crc16, strport4crc16;//放校验码用的
	CString Com1No, Com2No, Com3No, Com4No;//串口名称变量
	int CommandNo;//指令集和的标志

	//串口下拉框
	CComboBox m_portlist1Combo;
	CComboBox m_portlist2Combo;
	CComboBox m_portlist3Combo;
	CComboBox m_portlist4Combo;

	HANDLE InitCom(CString comName);//初始化串口
	int CloseCom(HANDLE handler);//关闭串口
	void GetCommPort(CComboBox *pComboBox, CString &ComNo);//获取当前串口号
	void FindCommPort(CComboBox *pComboBox, CString &ComNo,int PortNO);//从注册表中寻找串口号

	CTask ThreadControl;//线程池的东西
	CThreadPool* m_lpThreadPool;//线程池的东西
	
	afx_msg LRESULT MainPortThreadControl(WPARAM wParam, LPARAM lParam);//线程池的消息循环

	void OpenThreadPoolTask(int Command);//线程池的开启线程函数
	void DownloadMainContralThread(LPVOID lpParam);//线程池的主控线程

	CString CommandWriteUnit(int strcommandNo);//文件下载命令集合,0是TEST指令,1是RID指令,2是IMEI指令,3是开始下载指令,4是下载结束指令
	CString CommandReadUnit(int strcommandNo);//同上
	
	//主窗口控件使能控制函数
	void Port1SetOtherWindowTrue();
	void Port1SetOtherWindowFalse();
	void PortSetOtherWindowTrue();
	void PortSetOtherWindowFalse();

	//清空串口的RID、IMEI等框函数
	void SetPort1EditEmpty();
	void SetPort2EditEmpty();
	void SetPort3EditEmpty();
	void SetPort4EditEmpty();


	//下面是连接、开始和串口下拉框按钮点击事件
	afx_msg void OnCbnDropdownPortlist1Combo();
	afx_msg void OnBnClickedPort1connectButton();
	afx_msg void OnBnClickedStartdownload1Button();
	afx_msg void OnBnClickedPort2connectButton();
	afx_msg void OnBnClickedStartdownload2Button();
	afx_msg void OnBnClickedPort3connectButton();
	afx_msg void OnBnClickedStartdownload3Button();
	afx_msg void OnBnClickedPort4connectButton();
	afx_msg void OnBnClickedStartdownload4Button();
	afx_msg void OnBnClickedAutomultipleconnectButton();
	afx_msg void OnBnClickedAutomultiplestartButton();
	afx_msg void OnCbnDropdownPortlist2Combo();
	afx_msg void OnCbnDropdownPortlist3Combo();
	afx_msg void OnCbnDropdownPortlist4Combo();


	//以下是各个串口的线程
	//单文件下载
	void SingleDownloadWrite1Port1Thread(LPVOID lpParam);
	void SingleDownloadWrite2Port1Thread(LPVOID lpParam);
	void SingleDownloadWrite3Port1Thread(LPVOID lpParam);
	void SingleDownloadWrite4Port1Thread(LPVOID lpParam);
	void SingleDownloadRead1Port1Thread(LPVOID lpParam);
	void SingleDownloadRead2Port1Thread(LPVOID lpParam);
	void SingleDownloadRead3Port1Thread(LPVOID lpParam);
	void SingleDownloadRead4Port1Thread(LPVOID lpParam);
	void SingleDownloadRestPort1Thread();
	void SingleDownloadClosePort1Thread();

	//串口1
	void DownloadWrite1Port1Thread(LPVOID lpParam);
	void DownloadWrite2Port1Thread(LPVOID lpParam);
	void DownloadWrite3Port1Thread(LPVOID lpParam);
	void DownloadWrite4Port1Thread(LPVOID lpParam);
	void DownloadRead1Port1Thread(LPVOID lpParam);
	void DownloadRead2Port1Thread(LPVOID lpParam);
	void DownloadRead3Port1Thread(LPVOID lpParam);
	void DownloadRead4Port1Thread(LPVOID lpParam);
	void DownloadRestPort1Thread();
	void DownloadClosePort1Thread();

	//串口2
	void DownloadWrite1Port2Thread(LPVOID lpParam);
	void DownloadWrite2Port2Thread(LPVOID lpParam);
	void DownloadWrite3Port2Thread(LPVOID lpParam);
	void DownloadWrite4Port2Thread(LPVOID lpParam);
	void DownloadRead1Port2Thread(LPVOID lpParam);
	void DownloadRead2Port2Thread(LPVOID lpParam);
	void DownloadRead3Port2Thread(LPVOID lpParam);
	void DownloadRead4Port2Thread(LPVOID lpParam);
	void DownloadRestPort2Thread();
	void DownloadClosePort2Thread();

	//串口3
	void DownloadWrite1Port3Thread(LPVOID lpParam);
	void DownloadWrite2Port3Thread(LPVOID lpParam);
	void DownloadWrite3Port3Thread(LPVOID lpParam);
	void DownloadWrite4Port3Thread(LPVOID lpParam);
	void DownloadRead1Port3Thread(LPVOID lpParam);
	void DownloadRead2Port3Thread(LPVOID lpParam);
	void DownloadRead3Port3Thread(LPVOID lpParam);
	void DownloadRead4Port3Thread(LPVOID lpParam);
	void DownloadRestPort3Thread();
	void DownloadClosePort3Thread();

	//串口4
	void DownloadWrite1Port4Thread(LPVOID lpParam);
	void DownloadWrite2Port4Thread(LPVOID lpParam);
	void DownloadWrite3Port4Thread(LPVOID lpParam);
	void DownloadWrite4Port4Thread(LPVOID lpParam);
	void DownloadRead1Port4Thread(LPVOID lpParam);
	void DownloadRead2Port4Thread(LPVOID lpParam);
	void DownloadRead3Port4Thread(LPVOID lpParam);
	void DownloadRead4Port4Thread(LPVOID lpParam);
	void DownloadRestPort4Thread();
	void DownloadClosePort4Thread();


	//信息日志模块函数以及变量
	CRichEditCtrl m_currentinformationRichedit;
	void PrintLog(CString strMsg,int No);

	void SetRicheditText(CString strMsg, int No);
	CString GetTime();
	CString GetLogTime();

	//数据库整合函数以及变量
	afx_msg void OnBnClickedDbconfigButton();//数据库配置按钮事件

	afx_msg LRESULT MainDataInsertControl(WPARAM wParam, LPARAM lParam);//数据库消息循环函数

	int SimDataSingleNoIsExitFun(CString strfile);
	int SimDataNoIsExitFun(CString strfile);

	//串口1的
	int SimDataLastStationFun1();
	int SimDataIsExitFun1();
	void SimDataOkInsertFun1();
	void SimDataErrorInsertFun1();

	//串口2的
	int SimDataLastStationFun2();
	int SimDataIsExitFun2();
	void SimDataOkInsertFun2();
	void SimDataErrorInsertFun2();

	//串口3的
	int SimDataLastStationFun3();
	int SimDataIsExitFun3();
	void SimDataOkInsertFun3();
	void SimDataErrorInsertFun3();

	//串口4的
	int SimDataLastStationFun4();
	int SimDataIsExitFun4();
	void SimDataOkInsertFun4();
	void SimDataErrorInsertFun4();

	//其它函数
	void Delay(int time);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);


	//获取本机信息变量和函数
	CString m_pcnameEdit;
	CString m_pcipEdit;

	int GetLocalHostIPName(CString &sLocalName, CString &sIpAddress);
	afx_msg void OnBnClickedOpenremodleButton();
};

