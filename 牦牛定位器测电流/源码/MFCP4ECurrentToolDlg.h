
// MFCP4ECurrentToolDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "Resource.h"
#include "AutoCombox.h"
#include <map>

using namespace std;

//线程函数
UINT MainControlThread(LPVOID lpParam);//串口主控线程

UINT SendPort1Thread(LPVOID lpParam);//串口写线程
UINT ReadPort1Thread(LPVOID lpParam);//串口读线程

//动态库要用到的函数
double Current();
bool PowerOn();

//全局变量
extern CString Currentper[5];//存放五次电流
extern float StandbyAverage;//待机电流平均值
extern float SleepAverage1;//睡眠电流平均值
extern float SleepAverage2;//睡眠电流平均值
extern int CommandCount;//指令发送数量
extern int CommandSendInterval;//指令发送间隔
extern int CommandReadInterval;//指令读取间隔
extern int StandbyInterval;//待机等待时间
extern int SleepInterval;//睡眠等待时间
extern int StandbyInterval;//待机等待时间
extern int SleepInterval;//睡眠等待时间
extern CString Port1LogName;//日志文件名



// CMFCP4ECurrentToolDlg 对话框
class CMFCP4ECurrentToolDlg : public CDialogEx
{
// 构造
public:
	CMFCP4ECurrentToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCP4ECURRENTTOOL_DIALOG };

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
	//获取本机信息变量和函数
	CString m_pcnameEdit;
	CString m_pcipEdit;

	int GetLocalHostIPName(CString &sLocalName, CString &sIpAddress);

	/*控件使能变量以及函数*/
	BOOL ConnectFlag;//机型、串口和连接标志位
	
	void InitEnableWindows(BOOL choose);//初始模块控件使能函数

	/*初始化模块变量以及函数*/
	static map<int, CString>CommandMap;//存放所要发送的指令
	static map<int, CString>CommandReplyMap;//存放指令的回复
	CString Port1No;//串口名称变量
	HANDLE Port1handler;//串口句柄
	CFont staticHint1font,editfont;//字体变量
	COLORREF clrcolor;//颜色变量
	CBrush m_brush;//刷子变量

	//下拉框控件变量
	CComboBox m_Port1Combo;
	CAutoCombox m_ModelCombo;

	//编辑框控件变量
	CEdit m_StandbyUpControl;
	float m_StandbyUpValue;
	CEdit m_StandbyCuControl;
	CString m_StandbyCuValue;
	CEdit m_StandbyDownControl;
	float m_StandbyDownValue;
	CEdit m_Port1RidControl;
	CString m_Port1RidValue;
	CEdit m_SleepUpControl;
	float m_SleepUpValue;
	CEdit m_SleepCuControl1;
	CString m_SleepCuValue1;
	CEdit m_SleepDownControl;
	float m_SleepDownValue;

	void fontinit();//初始化字体
	afx_msg LRESULT MainFontControl(WPARAM wParam, LPARAM lParam);//字体更改的消息循环
	void InitModelDBOperation();//初始化机型下拉框
	void FindModelDBOperation();//查找机型配置
	HANDLE InitCom(CString comName);//初始化串口
	int CloseCom(HANDLE handler);//关闭串口
	void GetCommPort(CComboBox *pComboBox, CString &ComNo);//获取当前串口号
	void FindCommPort(CComboBox *pComboBox, CString &ComNo, int PortNO);//从注册表中寻找串口号
	BOOL JudgeEu();//判断电流

	afx_msg void OnCbnSelchangeModelCombo();
	afx_msg void OnBnClickedModelconfigButton();
	afx_msg void OnBnClickedPort1connectButton();
	afx_msg void OnCbnKillfocusModelCombo();
	afx_msg void OnCbnSelendokModelCombo();
	afx_msg void OnCbnDropdownPort1Combo();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);


	/*指令与线程的变量与函数*/
	int CommandNo;//当前指令
	CWinThread* DWThread;//下载写线程句柄
	CWinThread* DRThread;//下载写线程句柄
	CWinThread* MainThread;//下载主控线程句柄

	void RestPort1Thread();//重置线程变量
	void SetPort1EditEmpty();//清除窗口


	/*数据库模块变量以及函数*/

	//插入语句
	BOOL DBInsertOperation(CString ECIP, CString Rid, CString StandbyCurrent, CString StandbyAverage, CString SleepCurrent, CString SleepAverage1, CString SleepAverage2, CString TestResult);
	//判断RID是否重复
	int DBJudgeOperation(CString Rid);
	
	afx_msg void OnBnClickedDbconfigButton();


	/*日志模块变量以及函数*/
	CRichEditCtrl m_CurrentLog;

	void SetRicheditText(CString strMsg, int No); //当前日志函数
	void PrintLog(CString strMsg, int No);//日志打印函数
	CString GetTime();	//获取系统时间
	CString GetLogTime();	//获取日志时间


	/*其它函数以及变量*/
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CString m_SleepCuValue2;
	CEdit m_SleepCuControl2;
};
