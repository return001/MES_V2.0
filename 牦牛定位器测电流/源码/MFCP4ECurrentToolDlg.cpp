
// MFCP4ECurrentToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCP4ECurrentTool.h"
#include "MFCP4ECurrentToolDlg.h"
#include "afxdialogex.h"
#include "ADOManage.h"
#include "DBconfig.h"
#include "ModelConfig.h"
#include "Log.h"
#include "Manager.h"

#include <string>
#include "IT6400.h"
#include <ctime>
#include <vector>
#include <windows.h>
#include "Keysight34461A.h"
#pragma comment(lib,"IT6400.lib")
#pragma comment(lib,"Keysight34461A.lib")
static char InstrName[] = "GPIB0::23::INSTR";//设备连接PC的串口名

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//系统消息函数宏定义
#define WM_MainFontControl WM_USER+1306        //字体提示消息循环

//全局变量
map<int, CString> CMFCP4ECurrentToolDlg::CommandMap;//存放所要发送的指令
map<int, CString> CMFCP4ECurrentToolDlg::CommandReplyMap;//存放指令的回复

int CommandCount;//指令发送数量
int CommandSendInterval;//指令发送间隔
int CommandReadInterval;//指令读取间隔
int StandbyInterval=15000;//待机等待时间
int SleepInterval=15000;//睡眠等待时间
int Sleep2Interval = 6000;//睡眠等待时间
CString Port1LogName;//日志文件名
CString Currentper[5];//存放五次电流
float StandbyAverage;//待机电流平均值
float SleepAverage1;//睡眠电流平均值1
float SleepAverage2;//睡眠电流平均值2

volatile BOOL s_bExit;
volatile BOOL m_MainConrtolFlag;//主控线程标志位
volatile BOOL m_Port1ReadFlag;
volatile BOOL m_Port1SendFlag;

volatile BOOL m_Port1SendFlag1;
volatile BOOL m_Port1SendFlag2;
volatile BOOL m_Port1ReadFlag1;
volatile BOOL m_Port1ReadFlag2;
volatile BOOL m_Port1ReadFlagEnd1;

static bool OutputEnable = false;//不知道干嘛用的
uintptr_t* VisaNameOut = 0;//串口号
double CurrRange = 3;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCP4ECurrentToolDlg 对话框



CMFCP4ECurrentToolDlg::CMFCP4ECurrentToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCP4ECurrentToolDlg::IDD, pParent)
	, m_StandbyUpValue(0)
	, m_StandbyCuValue(_T(""))
	, m_StandbyDownValue(0)
	, m_Port1RidValue(_T(""))
	, m_SleepUpValue(0)
	, m_SleepCuValue1(_T(""))
	, m_SleepDownValue(0)
	, ConnectFlag(FALSE)
	, m_SleepCuValue2(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCP4ECurrentToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PORT1_COMBO, m_Port1Combo);
	DDX_Control(pDX, IDC_MODEL_COMBO, m_ModelCombo);
	DDX_Control(pDX, IDC_CURRENTLOG_RICHEDIT, m_CurrentLog);
	DDX_Control(pDX, IDC_STANDBYUP_EDIT, m_StandbyUpControl);
	DDX_Text(pDX, IDC_STANDBYUP_EDIT, m_StandbyUpValue);
	DDX_Control(pDX, IDC_STANDBYCU_EDIT, m_StandbyCuControl);
	DDX_Text(pDX, IDC_STANDBYCU_EDIT, m_StandbyCuValue);
	DDX_Control(pDX, IDC_STANDBYDOWN_EDIT, m_StandbyDownControl);
	DDX_Text(pDX, IDC_STANDBYDOWN_EDIT, m_StandbyDownValue);
	DDX_Control(pDX, IDC_PORT1RID_EDIT, m_Port1RidControl);
	DDX_Text(pDX, IDC_PORT1RID_EDIT, m_Port1RidValue);
	DDX_Control(pDX, IDC_SLEEPUP_EDIT, m_SleepUpControl);
	DDX_Text(pDX, IDC_SLEEPUP_EDIT, m_SleepUpValue);
	DDX_Control(pDX, IDC_SLEEPCU1_EDIT, m_SleepCuControl1);
	DDX_Text(pDX, IDC_SLEEPCU1_EDIT, m_SleepCuValue1);
	DDX_Control(pDX, IDC_SLEEPDOWN_EDIT, m_SleepDownControl);
	DDX_Text(pDX, IDC_SLEEPDOWN_EDIT, m_SleepDownValue);
	DDX_Text(pDX, IDC_SLEEPCU2_EDIT, m_SleepCuValue2);
	DDX_Control(pDX, IDC_SLEEPCU2_EDIT, m_SleepCuControl2);
}

BEGIN_MESSAGE_MAP(CMFCP4ECurrentToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMFCP4ECurrentToolDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMFCP4ECurrentToolDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_DBCONFIG_BUTTON, &CMFCP4ECurrentToolDlg::OnBnClickedDbconfigButton)
	ON_BN_CLICKED(IDC_MODELCONFIG_BUTTON, &CMFCP4ECurrentToolDlg::OnBnClickedModelconfigButton)
	ON_CBN_SELCHANGE(IDC_MODEL_COMBO, &CMFCP4ECurrentToolDlg::OnCbnSelchangeModelCombo)
	ON_BN_CLICKED(IDC_PORT1CONNECT_BUTTON, &CMFCP4ECurrentToolDlg::OnBnClickedPort1connectButton)
	ON_CBN_KILLFOCUS(IDC_MODEL_COMBO, &CMFCP4ECurrentToolDlg::OnCbnKillfocusModelCombo)
	ON_CBN_SELENDOK(IDC_MODEL_COMBO, &CMFCP4ECurrentToolDlg::OnCbnSelendokModelCombo)
	ON_CBN_DROPDOWN(IDC_PORT1_COMBO, &CMFCP4ECurrentToolDlg::OnCbnDropdownPort1Combo)
	ON_MESSAGE(WM_MainFontControl, &CMFCP4ECurrentToolDlg::MainFontControl)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CMFCP4ECurrentToolDlg 消息处理程序

BOOL CMFCP4ECurrentToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	//初始化机型下拉框
	InitModelDBOperation();

	//将ini配置文件信息的东西读出来
	int intervaltime;
	intervaltime = GetPrivateProfileInt(_T("IntertvalTime"), _T("StandbyInterval"), 15000, _T(".\\SystemInfo.ini"));
	StandbyInterval = intervaltime;//待机等待时间
	intervaltime = GetPrivateProfileInt(_T("IntertvalTime"), _T("SleepInterval1"), 15000, _T(".\\SystemInfo.ini"));
	SleepInterval = intervaltime;//睡眠1等待时间
	intervaltime = GetPrivateProfileInt(_T("IntertvalTime"), _T("SleepInterval2"), 6000, _T(".\\SystemInfo.ini"));
	Sleep2Interval = intervaltime;//睡眠2等待时间

	//初始化串口列表
	FindCommPort((CComboBox*)GetDlgItem(IDC_PORT1_COMBO), Port1No, 1);

	//日志文件名称初始化
	Port1LogName = GetLogTime() + L"Port1Log";

	//字体初始化
	fontinit();

	//初始化本机信息
	GetLocalHostIPName(m_pcnameEdit, m_pcipEdit);//调用函数获得IP和计算机名称
	SetDlgItemText(IDC_PCIP_EDIT, m_pcipEdit);//将计算机的名称显示在名称控件中
	SetDlgItemText(IDC_PCNAME_EDIT, m_pcnameEdit);//将计算机的IP显示在IP控件中

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCP4ECurrentToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCP4ECurrentToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCP4ECurrentToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*控件使能函数*/
//初始模块控件使能函数
void CMFCP4ECurrentToolDlg::InitEnableWindows(BOOL choose)
{
	GetDlgItem(IDC_DBCONFIG_BUTTON)->EnableWindow(choose);
	GetDlgItem(IDC_MODELCONFIG_BUTTON)->EnableWindow(choose);
	GetDlgItem(IDC_MODEL_COMBO)->EnableWindow(choose);
	GetDlgItem(IDC_PORT1_COMBO)->EnableWindow(choose);
}

/*初始化模块*/
//机型配置按钮
void CMFCP4ECurrentToolDlg::OnBnClickedModelconfigButton()
{
	INT_PTR nRes;
	CManager *manager = new CManager;
	nRes = manager->DoModal();

	if (IDCANCEL == nRes)
	{
		InitModelDBOperation();
		return;
	}

	else if (IDOK == nRes)
	{
		INT_PTR nRes;             // 用于保存DoModal函数的返回值   

		CModelConfig modelDlg;
		nRes = modelDlg.DoModal();  // 弹出对话框

		if (IDCANCEL == nRes)
			return;
	}
}

//机型下拉框选择机型后做的操作
void CMFCP4ECurrentToolDlg::OnCbnSelchangeModelCombo()
{
	// TODO:  在此添加控件通知处理程序代码
	FindModelDBOperation();
}

//机型下拉框失去焦点后做的操作
void CMFCP4ECurrentToolDlg::OnCbnKillfocusModelCombo()
{
	// TODO:  在此添加控件通知处理程序代码
	//FindModelDBOperation();
}

//机型下拉框按OK后的操作
void CMFCP4ECurrentToolDlg::OnCbnSelendokModelCombo()
{
	// TODO:  在此添加控件通知处理程序代码
/*	FindModelDBOperation()*/;
}

//查找机型数据库操作
void CMFCP4ECurrentToolDlg::FindModelDBOperation()
{
	ADOManage adomanageFind;
	CString ModelName, strtemp;
	float ftemp;
	int nSel;
	nSel = m_ModelCombo.GetCurSel();
	m_ModelCombo.GetLBText(nSel, ModelName);
	adomanageFind.ConnSQL();
	adomanageFind.CheckConfigByModelNameSql(ModelName);

	//获取待机上下限的值（注意，由于没有浮点型的setdlgitem，所以只能用updatadata的方式来更新到界面）
	ftemp = adomanageFind.m_pRecordSet->GetCollect("StandbyUp");
	m_StandbyUpValue = ftemp;

	ftemp = adomanageFind.m_pRecordSet->GetCollect("StandbyDown");
	m_StandbyDownValue = ftemp;

	ftemp = adomanageFind.m_pRecordSet->GetCollect("SleepUP");
	m_SleepUpValue = ftemp;

	ftemp = adomanageFind.m_pRecordSet->GetCollect("SleepDown");
	m_SleepDownValue = ftemp;
	UpdateData(FALSE);

	CommandMap[0] = adomanageFind.m_pRecordSet->GetCollect("TestCommand").bstrVal;//检测连接指令
	CommandReplyMap[0] = adomanageFind.m_pRecordSet->GetCollect("TestCommandReply").bstrVal;

	CommandMap[1] = adomanageFind.m_pRecordSet->GetCollect("RidCommand").bstrVal;//读RID指令
	CommandReplyMap[1] = adomanageFind.m_pRecordSet->GetCollect("RidCommandReply").bstrVal;

	CommandMap[2] = adomanageFind.m_pRecordSet->GetCollect("StandbyCommand").bstrVal;//待机指令
	CommandReplyMap[2] = adomanageFind.m_pRecordSet->GetCollect("StandbyCommandReply").bstrVal;

	CommandMap[3] = adomanageFind.m_pRecordSet->GetCollect("SleepCommand").bstrVal;//休眠指令
	CommandReplyMap[3] = adomanageFind.m_pRecordSet->GetCollect("SleepCommandReply").bstrVal;

	CommandCount = adomanageFind.m_pRecordSet->GetCollect("Count");//指令发送数量
	CommandSendInterval = adomanageFind.m_pRecordSet->GetCollect("WriteTime");//指令发送间隔
	CommandReadInterval = adomanageFind.m_pRecordSet->GetCollect("ReadTime");//指令读取间隔

	adomanageFind.CloseAll();
}

//初始化机型下拉框数据库操作
void CMFCP4ECurrentToolDlg::InitModelDBOperation()
{
	m_ModelCombo.ResetContent();
	ADOManage adomanageCombo;
	adomanageCombo.ConnSQL();
	adomanageCombo.m_pRecordSet = adomanageCombo.CheckAllInConfigSql();
	while (!adomanageCombo.m_pRecordSet->adoEOF)
	{
		m_ModelCombo.AddString(adomanageCombo.m_pRecordSet->GetCollect("ModelName").bstrVal);
		adomanageCombo.m_pRecordSet->MoveNext();
	}
	adomanageCombo.CloseAll();
}

//点击串口号下拉框的时候会自动更新串口号
void CMFCP4ECurrentToolDlg::OnCbnDropdownPort1Combo()
{
	// TODO:  在此添加控件通知处理程序代码
	FindCommPort((CComboBox*)GetDlgItem(IDC_PORT1_COMBO), Port1No, 1);
}

//点击连接按钮
void CMFCP4ECurrentToolDlg::OnBnClickedPort1connectButton()
{
	// TODO:  在此添加控件通知处理程序代码
	GetCommPort((CComboBox*)GetDlgItem(IDC_PORT1_COMBO), Port1No);//先获取当前串口号

	if (ConnectFlag == FALSE)
	{
		CString ModelName;
		GetDlgItemText(IDC_MODEL_COMBO,ModelName);
		//检查机型是否已经选择
		if (ModelName == L"")
		{
			MessageBox(L"请先选择机型", L"提示信息", NULL);
			return;
		}

		Port1handler = InitCom(Port1No);
		if (Port1handler == NULL)
		{
			SetRicheditText(L"开启串口失败", 1);
			PrintLog(L"开启串口失败", 1);
			return;
		}

		SetDlgItemText(IDC_PORT1CONNECT_BUTTON, L"断开");
		InitEnableWindows(FALSE);
		ConnectFlag = TRUE;

		//先将ini配置文件信息的东西读出来
		int intervaltime;
		intervaltime=GetPrivateProfileInt(_T("IntertvalTime"), _T("StandbyInterval"), 15000, _T(".\\SystemInfo.ini"));
		StandbyInterval = intervaltime;//待机等待时间
		intervaltime = GetPrivateProfileInt(_T("IntertvalTime"), _T("SleepInterval1"), 15000, _T(".\\SystemInfo.ini"));
		SleepInterval = intervaltime;//睡眠1等待时间
		intervaltime = GetPrivateProfileInt(_T("IntertvalTime"), _T("SleepInterval2"), 6000, _T(".\\SystemInfo.ini"));
		Sleep2Interval = intervaltime;//睡眠2等待时间

		//开启线程
		ConfMeas(InstrName, CurrRange, VisaNameOut);
		s_bExit = FALSE;
		m_MainConrtolFlag = TRUE;
		MainThread = AfxBeginThread(MainControlThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
	else if (ConnectFlag == TRUE)
	{
		if (!CloseCom(Port1handler))
		{
			SetRicheditText(L"关闭串口失败", 1);
			PrintLog(L"关闭串口失败", 1);
			MessageBox(L"关闭串口1失败", L"提示信息", NULL);
			return;
		}

		SetRicheditText(L"关闭串口成功", 0);
		PrintLog(L"关闭串口成功", 1);

		SetDlgItemText(IDC_PORT1CONNECT_BUTTON, L"连接");
		InitEnableWindows(TRUE);
		ConnectFlag = FALSE;

		s_bExit = TRUE;
		m_MainConrtolFlag = FALSE;
	}
}


/*串口通用操作*/
//从注册表中寻找已经注册串口号然后展示出来
void CMFCP4ECurrentToolDlg::FindCommPort(CComboBox *pComboBox, CString &ComNo, int PortNO)
{
	//先获取当前串口号
	int cur = 0;
	int nSel;
	BOOL curflag = TRUE;
	nSel = pComboBox->GetCurSel();
	if (nSel >= 0)
	{
		pComboBox->GetLBText(nSel, ComNo);
	}

	//先清空一下列表
	pComboBox->ResetContent();

	HKEY hKey;
#ifdef _DEBUG
	ASSERT(pComboBox != NULL);
	pComboBox->AssertValid();
#endif
	if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Hardware\\DeviceMap\\SerialComm", NULL, KEY_READ, &hKey) == ERROR_SUCCESS) // 打开串口注册表
	{
		int i = 0;
		char portName[256], commName[256];
		DWORD dwLong, dwSize;
		while (1)
		{
			dwLong = dwSize = sizeof(portName);
			if (::RegEnumValue(hKey, i, LPWSTR(portName), &dwLong, NULL, NULL, (PUCHAR)commName, &dwSize) == ERROR_NO_MORE_ITEMS) // 枚举串口
			{
				break;
			}
			if (nSel >= 0 && ComNo == LPWSTR(commName))//如果跟上次选择的相等那就不让当前选择变动
			{
				/*cur = i - PortNO+1;*/
				cur = i;
				curflag = FALSE;
			}
			//如果不相等那就按顺序自动分配一下，结合一个标志位使得各个串口不会对cur重复赋值
			else
			{
				if (PortNO == 1 && i == 0 && curflag == TRUE)
				{
					cur = i;
				}
				else if (PortNO == 2 && i == 1 && curflag == TRUE)
				{
					cur = i;
				}
				else if (PortNO == 3 && i == 2 && curflag == TRUE)
				{
					cur = i;
				}
				else if (PortNO == 4 && i == 3 && curflag == TRUE)
				{
					cur = i;
				}
			}
			pComboBox->AddString(LPWSTR(commName));
			//if (PortNO == 1 && i >= 0)
			//{
			//	pComboBox->AddString(LPWSTR(commName)); // commName就是串口名字
			//}
			//else if (PortNO == 2 && i >= 1)
			//{
			//	pComboBox->AddString(LPWSTR(commName));
			//}
			//else if (PortNO == 3 && i >= 2)
			//{
			//	pComboBox->AddString(LPWSTR(commName));
			//}
			//else if (PortNO == 4 && i >= 3)
			//{
			//	pComboBox->AddString(LPWSTR(commName));
			//}
			i++;
		}
		if (pComboBox->GetCount() == 0)
		{
			pComboBox->ResetContent();
			//SetRicheditText(L"找不到串口！请检测串口设备是否存在问题！", 1);
			//MessageBox(L"找不到串口！请检测串口设备是否存在问题！", L"提示信息", NULL);
			RegCloseKey(hKey);
			return;
		}
		pComboBox->SetCurSel(cur);
		RegCloseKey(hKey);
	}
}

//初始化串口
HANDLE CMFCP4ECurrentToolDlg::InitCom(CString comName)
{
	HANDLE hCom;
	hCom = CreateFile(L"\\\\.\\" + comName,//COM口
		GENERIC_READ | GENERIC_WRITE, //允许读和写
		0, //独占方式
		NULL,
		OPEN_EXISTING, //打开而不是创建
		0, //同步方式
		NULL);
	if (hCom == INVALID_HANDLE_VALUE)
	{
		//SetRicheditText(comName+L"串口初始化失败！", 1);
		//MessageBox(L"串口初始化失败！", L"提示信息", NULL);
		return NULL;
	}
	SetupComm(hCom, 100, 100); //输入缓冲区和输出缓冲区的大小都是100
	COMMTIMEOUTS TimeOuts;
	//设定读超时
	TimeOuts.ReadIntervalTimeout = MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier = 0;
	TimeOuts.ReadTotalTimeoutConstant = 0;
	//在读一次输入缓冲区的内容后读操作就立即返回，
	//而不管是否读入了要求的字符。
	//设定写超时
	TimeOuts.WriteTotalTimeoutMultiplier = 1500;
	TimeOuts.WriteTotalTimeoutConstant = 1500;
	SetCommTimeouts(hCom, &TimeOuts); //设置超时

	DCB dcb;
	GetCommState(hCom, &dcb);
	dcb.BaudRate = 115200; //波特率为9600
	dcb.ByteSize = 8; //每个字节有8位
	dcb.Parity = NOPARITY; //无奇偶校验位
	dcb.StopBits = ONESTOPBIT; //1个停止位
	SetCommState(hCom, &dcb);
	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
	SetRicheditText(L"串口初始化成功！", 0);
	return hCom;
}

//关闭串口
int CMFCP4ECurrentToolDlg::CloseCom(HANDLE handler)
{
	return CloseHandle(handler);
}

//获取当前串口号
void CMFCP4ECurrentToolDlg::GetCommPort(CComboBox *pComboBox, CString &ComNo)
{
	int nSel;
	nSel = pComboBox->GetCurSel();
	pComboBox->GetLBText(nSel, ComNo);

	if (ComNo == "")
	{
		MessageBox(L"请先选择串口号！", L"提示信息", NULL);
	}
}


/*指令与线程函数*/
//主控线程
UINT MainControlThread(LPVOID lpParam)
{
	CMFCP4ECurrentToolDlg* dlg;
	dlg = (CMFCP4ECurrentToolDlg*)lpParam;

	dlg->DWThread = AfxBeginThread(SendPort1Thread, dlg, THREAD_PRIORITY_NORMAL, 0, 0, NULL);//开启发送指令线程

	//如果没有被停止，那就一直去开启test线程
	while (m_MainConrtolFlag)
	{
		if (dlg->DWThread == NULL)
			dlg->DWThread = AfxBeginThread(SendPort1Thread, dlg, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		Sleep(4000);
	}

	dlg->MainThread = NULL;
	return 0;
}

//串口发送指令线程
UINT SendPort1Thread(LPVOID lpParam)
{
	CMFCP4ECurrentToolDlg* dlg;
	dlg = (CMFCP4ECurrentToolDlg*)lpParam;

	PurgeComm(dlg->Port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;

	//放指令用变量
	CString strcommand;

	//一直test，看看串口有没有连接上
PortTest:
	//先初始化一些变量
	StandbyAverage = 0;
	SleepAverage1 = 0;
	SleepAverage2 = 0;
	for (int i = 0; i < 5; i++)
	{
		Currentper[i]=L"";
	}
	m_Port1SendFlag = TRUE;
	dlg->CommandNo = 0;
	strcommand = CMFCP4ECurrentToolDlg::CommandMap[dlg->CommandNo] + L"\r\n";
	
	//开始检测机子是否连接
	ClearCommError(dlg->Port1handler, &dwErrorFlags, &ComStat);

	//同时开启读线程
	dlg->DWThread = AfxBeginThread(ReadPort1Thread, dlg, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

	do
	{
		dlg->PrintLog(L"发:" + strcommand,1);
		dlg->SetRicheditText(L"发:" + strcommand, 0);
		bWriteStat = WriteFile(dlg->Port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		if (s_bExit == TRUE)
		{
			dlg->RestPort1Thread();
			return 0;
		}
		Sleep(2000);
	} while (m_Port1SendFlag);

	if (m_MainConrtolFlag == FALSE)
	{
		::PostMessage(dlg->m_hWnd, WM_MainFontControl, Main_Hint1_Stop, NULL);//显示提示
		//dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"停止");
		return 0;
	}
	dlg->SetPort1EditEmpty();

	::PostMessage(dlg->m_hWnd, WM_MainFontControl, Main_Hint1_Connected, NULL);//显示提示

//	RePortAbnomal = FALSE;

	m_Port1ReadFlagEnd1 = TRUE;

	PurgeComm(dlg->Port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	//连接上了就开始测待机电流
	for (int i = 1; i < 3; i++)
	{
		int count = 0;
		dlg->CommandNo = i;//先发读RID指令，然后发shutdown指令
		strcommand = CMFCP4ECurrentToolDlg::CommandMap[dlg->CommandNo] + L"\r\n";
		ClearCommError(dlg->Port1handler, &dwErrorFlags, &ComStat);

		//然后同时开启读线程
		m_Port1SendFlag = TRUE;
		dlg->DWThread = AfxBeginThread(ReadPort1Thread, dlg, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

		do
		{
			dlg->PrintLog(L"发:" + strcommand, 1);
			//dlg->SetRicheditText(L"发:" + strcommand, 0);
			bWriteStat = WriteFile(dlg->Port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);

			if (count == CommandCount)//设定指令发送多少次
			{
				//一直进入发送test指令，如果检测不到，那代表它已经断开了
				BOOL reporttestflag = TRUE, reportfailflag = TRUE;
				int reporttestcount = 0;
				char reportteststr[100];
				memset(reportteststr, 0, sizeof(reportteststr) / sizeof(reportteststr[0]));
				DWORD reporttestreadreal = 0, reporttestdwBytesWrite, reporttestdwErrorFlags;
				BOOL reporttestbReadStat, reporttestbWriteStat;
				CString reportteststrread, reportteststrcommand = CMFCP4ECurrentToolDlg::CommandMap[0] + L"\r\n";
				COMSTAT reporttestComStat;

				ClearCommError(dlg->Port1handler, &reporttestdwErrorFlags, &reporttestComStat);

				do
				{
					reporttestbWriteStat = WriteFile(dlg->Port1handler, CT2A(reportteststrcommand), reportteststrcommand.GetLength(), &reporttestdwBytesWrite, NULL);

					Sleep(90);
					reporttestbReadStat = ReadFile(dlg->Port1handler, reportteststr, 100, &reporttestreadreal, 0);
					if (reporttestbReadStat)
					{
						reportteststrread = reportteststr;
						if (reportteststrread.Find(CMFCP4ECurrentToolDlg::CommandReplyMap[0]) >= 0)
						{
							if (reportfailflag == TRUE)
							{
								::PostMessage(dlg->m_hWnd, WM_MainFontControl, Main_Hint1_AbnomalFail, NULL);//显示提示
								//dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"失败待重启");
								reportfailflag = FALSE;
							}
							reporttestcount = 0;
						}
						else
						{
							if (reportfailflag == TRUE)
							{
								::PostMessage(dlg->m_hWnd, WM_MainFontControl, Main_Hint1_AbnomalFail, NULL);//显示提示
								//dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"异常插拔");
								reportfailflag = FALSE;
								/*RePortAbnomal = TRUE;*/
							}
						}
						reporttestcount = 0;
					}
					reporttestcount++;
					if (reporttestcount == 5)
					{
						reporttestflag = FALSE;
					}
					Sleep(10);
					memset(reportteststr, 0, sizeof(reportteststr) / sizeof(reportteststr[0]));
					dlg->PrintLog(L"发:" + reportteststrcommand, 1);
					PurgeComm(dlg->Port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
				} while (reporttestflag);

				Sleep(100);
				m_Port1SendFlag = FALSE;
				m_Port1ReadFlag = FALSE;
				goto PortTest;
			}
			Sleep(100);
			//if (m_Port1ReadFlagEnd1 == FALSE)
			//{
			//	m_Port1SendFlag = FALSE;
			//	m_Port1ReadFlag = FALSE;

			//	if (count == CommandCount)//设定指令发送多少次
			//	{
			//		//一直进入发送test指令，如果检测不到，那代表它已经断开了
			//		BOOL reporttestflag = TRUE, reportfailflag = TRUE;
			//		int reporttestcount = 0;
			//		char reportteststr[100];
			//		memset(reportteststr, 0, sizeof(reportteststr) / sizeof(reportteststr[0]));
			//		DWORD reporttestreadreal = 0, reporttestdwBytesWrite, reporttestdwErrorFlags;
			//		BOOL reporttestbReadStat, reporttestbWriteStat;
			//		CString reportteststrread, reportteststrcommand = CMFCP4ECurrentToolDlg::CommandMap[0] + L"\r\n";
			//		COMSTAT reporttestComStat;

			//		ClearCommError(dlg->Port1handler, &reporttestdwErrorFlags, &reporttestComStat);

			//		do
			//		{
			//			reporttestbWriteStat = WriteFile(dlg->Port1handler, CT2A(reportteststrcommand), reportteststrcommand.GetLength(), &reporttestdwBytesWrite, NULL);

			//			Sleep(90);
			//			reporttestbReadStat = ReadFile(dlg->Port1handler, reportteststr, 100, &reporttestreadreal, 0);
			//			if (reporttestbReadStat)
			//			{
			//				reportteststrread = reportteststr;
			//				if (reportteststrread.Find(CMFCP4ECurrentToolDlg::CommandReplyMap[0]) >= 0)
			//				{
			//					if (reportfailflag == TRUE)
			//					{
			//						//::PostMessage(dlg->m_hWnd, WM_MainFontControl, Main_Hint1_AbnomalFail, NULL);//显示提示
			//						//dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"失败待重启");
			//						reportfailflag = FALSE;
			//					}
			//					reporttestcount = 0;
			//				}
			//			}
			//			reporttestcount++;
			//			if (reporttestcount == 5)
			//			{
			//				reporttestflag = FALSE;
			//			}
			//			Sleep(10);
			//			memset(reportteststr, 0, sizeof(reportteststr) / sizeof(reportteststr[0]));
			//			dlg->PrintLog(L"发:" + reportteststrcommand, 1);
			//			PurgeComm(dlg->Port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
			//		} while (reporttestflag);
			//		goto PortTest;
			//	}
			//}
				count++;
				Sleep(CommandSendInterval);//指令发送间隔
			} while (m_Port1SendFlag);
		}
	::PostMessage(dlg->m_hWnd, WM_MainFontControl, Main_Hint1_Standbytest, NULL);//显示提示

	//第一次是读待机电流
	Sleep(StandbyInterval);//停顿十秒
	StandbyAverage = Current() * 1000;
	ConfMeas(InstrName, CurrRange, VisaNameOut);//初始化仪器，确保仪器处于我们设定的状态
	if (_isnan(StandbyAverage))
	{
		::PostMessage(dlg->m_hWnd, WM_MainFontControl, Main_Hint1_StandbyFail, NULL);//显示提示
		//dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"读取到非正常电流值，请检查电流测试仪器！");
		goto PortTest;
	}
	else
	{
		CString standbyStr;
		::PostMessage(dlg->m_hWnd, WM_MainFontControl, Main_Hint1_StandbySuccess, NULL);//显示提示
		//dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"待机电流已测！请重启设备！");
		standbyStr.Format(L"%.3f", StandbyAverage);
		dlg->SetDlgItemText(IDC_STANDBYCU_EDIT, standbyStr);
	}

	Sleep(1000);//等一下

	//然后继续检测机子是否已经连接
	m_Port1SendFlag = TRUE;
	dlg->CommandNo = 0;
	strcommand = CMFCP4ECurrentToolDlg::CommandMap[dlg->CommandNo] + L"\r\n";

	//开始检测机子是否连接
	ClearCommError(dlg->Port1handler, &dwErrorFlags, &ComStat);

	//同时开启读线程
	dlg->DWThread = AfxBeginThread(ReadPort1Thread, dlg, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

	do
	{
		dlg->PrintLog(L"发:" + strcommand, 1);
		dlg->SetRicheditText(L"发:" + strcommand, 0);
		bWriteStat = WriteFile(dlg->Port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		if (s_bExit == TRUE)
		{
			dlg->RestPort1Thread();
			return 0;
		}
		Sleep(2000);
	} while (m_Port1SendFlag);

	::PostMessage(dlg->m_hWnd, WM_MainFontControl, Main_Hint1_Sleeptest1, NULL);//显示提示
	//dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"测试睡眠电流中！");

	PurgeComm(dlg->Port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	//连接到了就开始测试睡眠电流
	for (int i = 1; i < 3; i++)
	{
		int count = 0;
		if (i == 1)
		{
			dlg->CommandNo = i;//先发读RID指令
		}
		else if (i == 2)
		{
			dlg->CommandNo = 3;//然后发SLEEP指令
		}
		strcommand = CMFCP4ECurrentToolDlg::CommandMap[dlg->CommandNo] + L"\r\n";
		ClearCommError(dlg->Port1handler, &dwErrorFlags, &ComStat);

		//然后同时开启读线程
		m_Port1SendFlag = TRUE;
		dlg->DWThread = AfxBeginThread(ReadPort1Thread, dlg, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

		do
		{
			dlg->PrintLog(L"发:" + strcommand, 1);
			//dlg->SetRicheditText(L"发:" + strcommand, 0);
			bWriteStat = WriteFile(dlg->Port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);

			if (count == CommandCount)//设定指令发送多少次
			{
				//一直进入发送test指令，如果检测不到，那代表它已经断开了
				BOOL reporttestflag = TRUE, reportfailflag = TRUE;
				int reporttestcount = 0;
				char reportteststr[100];
				memset(reportteststr, 0, sizeof(reportteststr) / sizeof(reportteststr[0]));
				DWORD reporttestreadreal = 0, reporttestdwBytesWrite, reporttestdwErrorFlags;
				BOOL reporttestbReadStat, reporttestbWriteStat;
				CString reportteststrread, reportteststrcommand = CMFCP4ECurrentToolDlg::CommandMap[0] + L"\r\n";
				COMSTAT reporttestComStat;

				ClearCommError(dlg->Port1handler, &reporttestdwErrorFlags, &reporttestComStat);

				do
				{
					reporttestbWriteStat = WriteFile(dlg->Port1handler, CT2A(reportteststrcommand), reportteststrcommand.GetLength(), &reporttestdwBytesWrite, NULL);

					Sleep(90);
					reporttestbReadStat = ReadFile(dlg->Port1handler, reportteststr, 100, &reporttestreadreal, 0);
					if (reporttestbReadStat)
					{
						reportteststrread = reportteststr;
						if (reportteststrread.Find(CMFCP4ECurrentToolDlg::CommandReplyMap[0]) >= 0)
						{
							if (reportfailflag == TRUE)
							{
								::PostMessage(dlg->m_hWnd, WM_MainFontControl, Main_Hint1_AbnomalFail, NULL);//显示提示
								//dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"失败待重启");
								reportfailflag = FALSE;
							}
							reporttestcount = 0;
						}
						else
						{
							if (reportfailflag == TRUE)
							{
								::PostMessage(dlg->m_hWnd, WM_MainFontControl, Main_Hint1_AbnomalFail, NULL);//显示提示
								//dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"异常插拔");
								reportfailflag = FALSE;
								/*RePortAbnomal = TRUE;*/
							}
						}
						reporttestcount = 0;
					}
					reporttestcount++;
					if (reporttestcount == 5)
					{
						reporttestflag = FALSE;
					}
					Sleep(10);
					memset(reportteststr, 0, sizeof(reportteststr) / sizeof(reportteststr[0]));
					dlg->PrintLog(L"发:" + reportteststrcommand, 1);
					PurgeComm(dlg->Port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
				} while (reporttestflag);

				Sleep(100);
				m_Port1SendFlag = FALSE;
				m_Port1ReadFlag = FALSE;
				goto PortTest;
			}
			count++;
			Sleep(CommandSendInterval);//指令发送间隔
		} while (m_Port1SendFlag);
	}

	//第二次是读睡眠电流
	Sleep(SleepInterval);//停顿15秒
	SleepAverage1 = Current() * 1000;
	ConfMeas(InstrName, CurrRange, VisaNameOut);//初始化仪器，确保仪器处于我们设定的状态
	if (_isnan(SleepAverage1))
	{
		::PostMessage(dlg->m_hWnd, WM_MainFontControl, Main_Hint1_SleepFail, NULL);//显示提示
		//dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"读取到非正常电流值，请检查电流测试仪器！！！");
		goto PortTest;
	}
	else
	{
		CString sleepStr1;
		sleepStr1.Format(L"%.3f", SleepAverage1);
		dlg->SetDlgItemText(IDC_SLEEPCU1_EDIT, sleepStr1);
	}

	//先停个4秒，提示重新上电
	::PostMessage(dlg->m_hWnd, WM_MainFontControl, Main_Hint1_Sleeptest2, NULL);//显示提示
	Sleep(2000);

	//提示读第二次睡眠电流
	//::PostMessage(dlg->m_hWnd, WM_MainFontControl, Main_Hint1_Sleeptest2, NULL);//显示提示

	//第三次也是读睡眠电流
	Sleep(Sleep2Interval);//停顿6秒
	SleepAverage2 = Current() * 1000;
	ConfMeas(InstrName, CurrRange, VisaNameOut);//初始化仪器，确保仪器处于我们设定的状态
	if (_isnan(SleepAverage2))
	{
		::PostMessage(dlg->m_hWnd, WM_MainFontControl, Main_Hint1_SleepFail, NULL);//显示提示
		//dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"读取到非正常电流值，请检查电流测试仪器！！！");
		goto PortTest;
	}
	else
	{
		CString sleepStr2;
		sleepStr2.Format(L"%.3f", SleepAverage2);
		dlg->SetDlgItemText(IDC_SLEEPCU2_EDIT, sleepStr2);
	}

	CString RIDstr;
	dlg->GetDlgItemText(IDC_PORT1RID_EDIT, RIDstr);
	//判断最终结果
	BOOL Result,DBIntermission;
	Result = dlg->JudgeEu();
    //测试通过时插入正确结果
	if (Result==TRUE)
	{
		::PostMessage(dlg->m_hWnd, WM_MainFontControl, Main_Hint1_Success, NULL);
		//dlg->DBInsertOperation(dlg->m_pcipEdit, RIDstr, L"", dlg->m_StandbyCuValue, L"", dlg->m_SleepCuValue1, dlg->m_SleepCuValue2, L"1");
	}
	//测试失败时插入失败结果
	else if (Result == FALSE)
	{
		::PostMessage(dlg->m_hWnd, WM_MainFontControl, Main_Hint1_Fail, NULL);
		//dlg->DBInsertOperation(dlg->m_pcipEdit, RIDstr, L"", dlg->m_StandbyCuValue, L"", dlg->m_SleepCuValue1, dlg->m_SleepCuValue2, L"0");
	}


	if (m_MainConrtolFlag == FALSE)
	{

		dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"停止");
		return 0;
	}
	Sleep(1500);

	dlg->DWThread = NULL;
	return 0;
}

//串口读线程
UINT ReadPort1Thread(LPVOID lpParam)
{
	CMFCP4ECurrentToolDlg* dlg;
	dlg = (CMFCP4ECurrentToolDlg*)lpParam;

	//串口变量
	char str[200];
	memset(str, 0, sizeof(str) / sizeof(str[0]));;
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;
	CString strcount, strcounttemp;//放RID和IMEI用
	CString strtemp;
	int findcount1, findcount2;//切割读出来的字符串用

	m_Port1ReadFlag = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程

	do
	{
		Sleep(CommandSendInterval);//指令读间隔
		bReadStat = ReadFile(dlg->Port1handler, str, 199, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			strtemp = CMFCP4ECurrentToolDlg::CommandReplyMap[dlg->CommandNo];
			dlg->PrintLog(L"收:" + strread, 1);
			if ((findcount1 = strread.Find(strtemp)) >= 0)
			{
				//读RID
				if (dlg->CommandNo == 1)
				{
					strcount = strread.Right(strread.GetLength() - findcount1 - 9);
					findcount2 = strcount.Find(L"\r\n");
					strcount = strcount.Left(findcount2);
					if (strcount != "")
					{
						strcount.Replace(LPCTSTR(" "), LPCTSTR(""));
						dlg->SetDlgItemText(IDC_PORT1RID_EDIT, strcount);

						////判断RID是否重复
						//int RIDflag;
						//RIDflag=dlg->DBJudgeOperation(strcount);
						//if (RIDflag == 0)
						//{
						//	::PostMessage(dlg->m_hWnd, WM_MainFontControl, Main_Hint1_Alreadtest, NULL);
						//	m_Port1ReadFlagEnd1 = FALSE;
						//}
					}
					else if (strcount == "")
					{
						continue;
					}
				}
				m_Port1ReadFlag = FALSE;
				m_Port1SendFlag = FALSE;
			}
		}
	} while (m_Port1ReadFlag);
	//dlg->SetRicheditText(L"收:" + strread, 0);
	dlg->PrintLog(L"收:" + strread, 1);

	PurgeComm(dlg->Port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	dlg->DRThread = NULL;
	return 0;
}

//重置线程全局变量
void CMFCP4ECurrentToolDlg::RestPort1Thread()
{
	m_Port1SendFlag1 = TRUE;
	m_Port1SendFlag2 = TRUE;
	m_Port1ReadFlag1 = TRUE;
	m_Port1ReadFlag2 = TRUE;
	m_Port1ReadFlagEnd1 = TRUE;
}

//清空RID和电流编辑框
void CMFCP4ECurrentToolDlg::SetPort1EditEmpty()
{
	SetDlgItemText(IDC_PORT1RID_EDIT, L"");
	SetDlgItemText(IDC_STANDBYCU_EDIT, L"");
	SetDlgItemText(IDC_SLEEPCU1_EDIT, L"");
	SetDlgItemText(IDC_SLEEPCU2_EDIT, L"");
}

//判断电流
BOOL CMFCP4ECurrentToolDlg::JudgeEu()
{
	CString StandbyCuStr, SleepCuStr1, SleepCuStr2;
	float StandbyCuF, SleepCuF1, SleepCuF2;

	//判断待机电流
	GetDlgItemText(IDC_STANDBYCU_EDIT, StandbyCuStr);
	StandbyCuF = _ttof(StandbyCuStr);
	if (StandbyCuF <= m_StandbyDownValue||StandbyCuF > m_StandbyUpValue)
	{
		return FALSE;
	}
	
	//判断睡眠电流1
	GetDlgItemText(IDC_SLEEPCU1_EDIT, SleepCuStr1);
	SleepCuF1 = _ttof(SleepCuStr1);
	if (SleepCuF1 <= m_SleepDownValue || SleepCuF1 > m_SleepUpValue)
	{
		return FALSE;
	}

	//判断睡眠电流2
	GetDlgItemText(IDC_SLEEPCU2_EDIT, SleepCuStr2);
	SleepCuF2 = _ttof(SleepCuStr2);
	if (SleepCuF2 < m_SleepDownValue || SleepCuF2 > m_SleepUpValue)
	{
		return FALSE;
	}

	return TRUE;
}


/*LV要用到的函数*/
//电流测试函数
bool PowerOn()
{
	uintptr_t* VisaNameOut = 0;//串口号

	LVBoolean DefaSetInit = false;
	LVBoolean * DefaSetValid = &DefaSetInit;//默认初始化


	float VSetValue = 3.0;
	LVBoolean VSetInit = false;
	LVBoolean *VsetValid = &VSetInit;//设置电压

	double VLimit = 4.0;
	LVBoolean EnVLimitSet = true;
	LVBoolean VLimitSetInit = false;
	LVBoolean *VLimitSetValid = &VLimitSetInit;//电压上限

	double VInit = 0;
	double *VMeasValue = &VInit;
	LVBoolean VoltMeasInit = false;
	LVBoolean *VMeasValid = &VoltMeasInit;//电压测试值

	float CSetValue = 0.3;
	LVBoolean CSetInit = false;
	LVBoolean* CSetValid = &CSetInit;//电流预设值设置

	LVBoolean EnableOutput = true;
	LVBoolean OutputInit = false;
	LVBoolean *OutputSetValid = &OutputInit;//设置输出
	OutputEnable = (bool)OutputInit;

	Defaultset(InstrName, VisaNameOut, DefaSetValid);
	VLimitConf(InstrName, VLimit, EnVLimitSet, VLimitSetValid);
	VSet(InstrName, VSetValue, VsetValid);
	EnOutput(InstrName, EnableOutput, OutputSetValid);
	return OutputInit;
}

double average(vector<double>::const_iterator begin, vector<double>::const_iterator end, vector<double>::size_type elementsNum)
{
	//获取系统时间
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d/%d/%d %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);


	double currAverage;
	double sum = 0;
	int i = 0;
	CString currstr;
	while (begin != end)
	{
		Currentper[i];
		sum = sum + (*begin);
		currstr.Format(strTime + _T("CU:%3f"), (*begin));
		Currentper[i] = currstr;
		begin++;
		i++;
	}
	currAverage = sum / elementsNum;
	return currAverage;
}

double Current()
{
	double CInit = 0;
	double *CMeasValue = &CInit;
	LVBoolean CMeasInit = false;
	LVBoolean *CMeasValid = &CMeasInit;//电流测量值
	vector<double> cur_Arr; //存放测试电流的值的容器
	double curr_Average = 0;  //存放等待模式下电流的平均值

	/**********************************
	测试电流的平均值
	***********************************/
	//仪器初始化
	//	ConfMeas(InstrName, CurrRange, VisaNameOut);

	for (int i = 0; i < 5; i++)
	{
		//仪器初始化
		ConfMeas(InstrName, CurrRange, VisaNameOut);
		Sleep(400);
		CMeas(InstrName, CMeasValue, CMeasValid);

		if ((bool)CMeasInit)
		{
			cur_Arr.push_back(*CMeasValue);
			cout << *CMeasValue << endl;
		}

	}
	//仪器初始化
	ConfMeas(InstrName, CurrRange, VisaNameOut);
	curr_Average = average(cur_Arr.begin(), cur_Arr.end(), cur_Arr.size());


	/**********************************
	测试电流的平均值
	***********************************/
	return curr_Average;
}


/*数据库模块*/
//插入语句
BOOL CMFCP4ECurrentToolDlg::DBInsertOperation(CString ECIP, CString Rid, CString StandbyCurrent, CString StandbyAverage, CString SleepCurrent, CString SleepAverage1, CString SleepAverage2, CString TestResult)
{
	ADOManage adomanageUpdate;
	adomanageUpdate.ConnSQL();
	adomanageUpdate.TestResultInsertSql(ECIP, Rid, StandbyCurrent, StandbyAverage, SleepCurrent, SleepAverage1, SleepAverage2, TestResult);
	adomanageUpdate.CloseAll();
	return TRUE;
}

//判断RID是否重复
int CMFCP4ECurrentToolDlg::DBJudgeOperation(CString Rid)
{
	ADOManage adomanageJudge;
	CString RIDstr;
	int judgeflag;
	GetDlgItemText(IDC_PORT1RID_EDIT,RIDstr);
	adomanageJudge.ConnSQL();
	judgeflag = adomanageJudge.CheckTestResultByRid(RIDstr);
	adomanageJudge.CloseAll();
	return judgeflag;
}

//数据库配置按钮
void CMFCP4ECurrentToolDlg::OnBnClickedDbconfigButton()
{
	// TODO:  在此添加控件通知处理程序代码
	INT_PTR nRes;             // 用于保存DoModal函数的返回值   

	CDBconfig dbconfigDlg;           // 构造对话框类CTipDlg的实例   
	nRes = dbconfigDlg.DoModal();  // 弹出对话框

	if (IDCANCEL == nRes)
		return;
}


/*日志模块*/
//当前日志函数
void CMFCP4ECurrentToolDlg::SetRicheditText(CString strMsg, int No)
{
	m_CurrentLog.SetSel(-1, -1);
	CHARFORMAT   cf{ sizeof(cf) };//设置这个结构来改变你想要的文字的颜色等属性
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD | CFM_SIZE | CFM_FACE;//这个目前还没解析完成，但如果要输出不同颜色的字体一定要使用richedit
	cf.crTextColor = RGB(255, 0, 0);//设置为红色

	switch (No)
	{
	case 0:
		m_CurrentLog.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
		m_CurrentLog.LineScroll(m_CurrentLog.GetLineCount() - 1 - 3);
		break;
	case 1:
		m_CurrentLog.SetSelectionCharFormat(cf);
		m_CurrentLog.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
		m_CurrentLog.LineScroll(m_CurrentLog.GetLineCount() - 1);
		break;
	default:
		break;
	}

	/*
	CHARFORMAT   cf{ sizeof(cf) };//设置这个结构来改变你想要的文字的颜色等属性
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD | CFM_SIZE | CFM_FACE;//这个目前还没解析完成，但如果要输出不同颜色的字体一定要使用richedit
	if (strMsg == "127.0.0.1>>测试信息")
	{
	cf.crTextColor = RGB(255, 0, 0);
	m_currenttcp1RichEdit.SetSelectionCharFormat(cf); //这个是richedit的专用方法，richedit常用来作为日志输出
	m_currenttcp1RichEdit.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("测试\r\n"));
	}
	cf.crTextColor = RGB(255, 200, 100);
	m_currenttcp1RichEdit.SetSelectionCharFormat(cf);*/
}

//获取系统时间
CString CMFCP4ECurrentToolDlg::GetTime()
{
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d/%d/%d %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	return strTime;
}

//日志打印函数
void CMFCP4ECurrentToolDlg::PrintLog(CString strMsg, int No)
{
	CLog Relog1;//日志文件

	switch (No)
	{
	//case 0:
	//	Relog0.WriteLog(CStringA(L"【" + GetTime() + L"】") + CStringA(strMsg), SinglePortLogName);
	//	break;
	case 1:
		Relog1.WriteLog(CStringA(L"【" + GetTime() + L"】") + CStringA(strMsg), Port1LogName);
		break;
	//case 2:
	//	Relog2.WriteLog(CStringA(L"【" + GetTime() + L"】") + CStringA(strMsg), Port2LogName);
	//	break;
	//case 3:
	//	Relog3.WriteLog(CStringA(L"【" + GetTime() + L"】") + CStringA(strMsg), Port3LogName);
	//	break;
	//case 4:
	//	Relog4.WriteLog(CStringA(L"【" + GetTime() + L"】") + CStringA(strMsg), Port4LogName);
	//	break;
	default:
		break;
	}
}

//获取日志时间
CString CMFCP4ECurrentToolDlg::GetLogTime()
{
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d%02d%02d"), time.wYear, time.wMonth, time.wDay);
	return strTime;
}


/*字体以及颜色处理函数*/
//字体更改的消息循环
afx_msg LRESULT CMFCP4ECurrentToolDlg::MainFontControl(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case Main_Hint1_Ready:
		PrintLog(L"等待连接\r\n", 1);
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"等待连接");
		break;
	case Main_Hint1_Connected:
		PrintLog(L"设备已经连接，读取RID中！\r\n", 1);
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"设备已经连接，读取RID中...");
		break;
	case Main_Hint1_Stop:
		PrintLog(L"停止\r\n", 1);
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"停止");
		break;
	case Main_Hint1_StandbySuccess:
		PrintLog(L"待机电流已测！请按键！\r\n", 1);
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"待机电流已测！请按键！");
		break;
	case Main_Hint1_StandbyFail:
		PrintLog(L"读取到非正常待机电流，请检查电流测试仪器！\r\n", 1);
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"读取到非正常待机电流，请检查电流测试仪器！");
		break;
	case Main_Hint1_SleepSuccess:
		PrintLog(L"第一次睡眠电流已测！请重新上电！\r\n", 1);
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"第一次睡眠电流已测！请重新上电！");
		break;
	case Main_Hint1_SleepFail:
		PrintLog(L"读取到非正常睡眠电流，请检查电流测试仪器！\r\n", 1);
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"读取到非正常睡眠电流，请检查电流测试仪器！");
		break;
	case Main_Hint1_Success:
		PrintLog(L"PASS！请连接下一台机子！\r\n", 1);
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"PASS！请连接下一台机子！");
		break;
	case Main_Hint1_Fail:
		PrintLog(L"FAIL！请连接下一台机子！\r\n", 1);
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"FAIL！请连接下一台机子！");
		break;
	case Main_Hint1_Standbytest:
		PrintLog(L"测试待机电流中，请等待数秒\r\n", 1);
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"测试待机电流中，请等待数秒");
		break;
	case Main_Hint1_Sleeptest1:
		PrintLog(L"测试第一次睡眠电流中，请等待数秒\r\n", 1);
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"测试第一次睡眠电流中，请等待数秒");
		break;
	case Main_Hint1_Sleeptest2:
		PrintLog(L"测试第二次睡眠电流中，请重新重新上电并等待数秒\r\n", 1);
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"测试第二次睡眠电流中，请重新重新上电并等待数秒");
		break;
	case Main_Hint1_AbnomalFail:
		PrintLog(L"异常失败，等待重新连接！\r\n", 1);
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"异常失败，等待重新连接！");
		break;
	case Main_Hint1_Alreadtest:
		PrintLog(L"该机已测！\r\n", 1);
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"该机已测！");
	default:
		break;
	}
	return 0;
}

//初始化字体
void CMFCP4ECurrentToolDlg::fontinit()
{
	staticHint1font.CreatePointFont(500, L"黑体");
	editfont.CreatePointFont(200, L"黑体");

	GetDlgItem(IDC_STANDBYCU_EDIT)->SetFont(&editfont);
	GetDlgItem(IDC_SLEEPCU1_EDIT)->SetFont(&editfont);
	GetDlgItem(IDC_SLEEPCU2_EDIT)->SetFont(&editfont);
	GetDlgItem(IDC_PORT1HINT_STATIC)->SetFont(&staticHint1font);
}

//颜色变化
HBRUSH CMFCP4ECurrentToolDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	CString str1;
	CString StandbyCuStr, SleepCuStr1, SleepCuStr2;
	float StandbyCuF, SleepCuF1, SleepCuF2;

	if (pWnd->GetDlgCtrlID() == IDC_PORT1HINT_STATIC)
	{
		GetDlgItemText(IDC_PORT1HINT_STATIC, str1);
		if (str1 == "待机电流已测！请按键！" || str1 == "测试第二次睡眠电流中，请重新重新上电并等待数秒")
		{
			m_brush.CreateSolidBrush(RGB(255, 165, 0));
			pDC->SetBkColor(RGB(255, 165, 0));
			pDC->SetTextColor(RGB(0, 0, 0));//用RGB宏改变颜色 
			return m_brush;
		}
		else if (str1 == "PASS！请连接下一台机子！")
		{
			m_brush.CreateSolidBrush(RGB(50, 205, 50));
			pDC->SetBkColor(RGB(50, 205, 50));
			pDC->SetTextColor(RGB(0, 0, 0));//用RGB宏改变颜色
			return m_brush;
		}
		else if (str1 == "FAIL！请连接下一台机子！" || str1 == "异常失败，等待重新连接！" || str1.Find(L"非正常") >= 0 || str1 == "该机已测！")
		{
			m_brush.CreateSolidBrush(RGB(255, 0, 0));
			pDC->SetBkColor(RGB(255, 0, 0));
			pDC->SetTextColor(RGB(0, 0, 0));//用RGB宏改变颜色
			return m_brush;
		}
		//else if (str1 == "返工成功" || str1 == "PASS")
		//{
		//	pDC->SetTextColor(RGB(0, 255, 0));//用RGB宏改变颜色 
		//	pDC->SelectObject(&staticReHintfont);
		//}
		//else if (str1 == "等待连接" || str1 == "Ready")
		//{
		//	pDC->SetTextColor(RGB(65, 105, 225));//用RGB宏改变颜色 
		//	pDC->SelectObject(&staticReHintfont);
		//}
		//else
		//{
		//	pDC->SetTextColor(RGB(0, 0, 0));//用RGB宏改变颜色 
		//	pDC->SelectObject(&staticReHintfont);
		//}
	}
	else if (pWnd->GetDlgCtrlID() == IDC_STANDBYCU_EDIT)
	{
		GetDlgItemText(IDC_STANDBYCU_EDIT, StandbyCuStr);
		if (StandbyCuStr != "")
		{
			StandbyCuF = _ttof(StandbyCuStr);
			if (StandbyCuF > m_StandbyDownValue&&StandbyCuF <= m_StandbyUpValue)
			{
				m_brush.CreateSolidBrush(RGB(50, 205, 50));
				pDC->SetBkColor(RGB(50, 205, 50));
				pDC->SetTextColor(RGB(0, 0, 0));//用RGB宏改变颜色
				return m_brush;
			}
			else
			{
				m_brush.CreateSolidBrush(RGB(255, 0, 0));
				pDC->SetBkColor(RGB(255, 0, 0));
				pDC->SetTextColor(RGB(0, 0, 0));//用RGB宏改变颜色
				return m_brush;
			}
		}
	}
	else if (pWnd->GetDlgCtrlID() == IDC_SLEEPCU1_EDIT)
	{
		GetDlgItemText(IDC_SLEEPCU1_EDIT, SleepCuStr1);
		if (SleepCuStr1 != "")
		{
			SleepCuF1 = _ttof(SleepCuStr1);
			if (SleepCuF1 > m_SleepDownValue&&SleepCuF1 <= m_SleepUpValue)
			{
				m_brush.CreateSolidBrush(RGB(50, 205, 50));
				pDC->SetBkColor(RGB(50, 205, 50));
				pDC->SetTextColor(RGB(0, 0, 0));//用RGB宏改变颜色
				return m_brush;
			}
			else
			{
				m_brush.CreateSolidBrush(RGB(255, 0, 0));
				pDC->SetBkColor(RGB(255, 0, 0));
				pDC->SetTextColor(RGB(0, 0, 0));//用RGB宏改变颜色
				return m_brush;
			}
		}
	}
	else if (pWnd->GetDlgCtrlID() == IDC_SLEEPCU2_EDIT)
	{
		GetDlgItemText(IDC_SLEEPCU2_EDIT, SleepCuStr2);
		if (SleepCuStr2 != "")
		{
			SleepCuF2 = _ttof(SleepCuStr2);
			if (SleepCuF2 > m_SleepDownValue&&SleepCuF2 <= m_SleepUpValue)
			{
				m_brush.CreateSolidBrush(RGB(50, 205, 50));
				pDC->SetBkColor(RGB(50, 205, 50));
				pDC->SetTextColor(RGB(0, 0, 0));//用RGB宏改变颜色
				return m_brush;
			}
			else
			{
				m_brush.CreateSolidBrush(RGB(255, 0, 0));
				pDC->SetBkColor(RGB(255, 0, 0));
				pDC->SetTextColor(RGB(0, 0, 0));//用RGB宏改变颜色
				return m_brush;
			}
		}
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


/*其它函数*/
//获取本机IP和地址
int CMFCP4ECurrentToolDlg::GetLocalHostIPName(CString &sLocalName, CString &sIpAddress)
{
	char szLocalName[256];//定义
	WSADATA wsaData;//定义套接字存储变量
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) == 0)//<span class="con">windows初始化socket网络库，申请2，0的版本，windows socket编程必<img class="selectsearch-hide" id="selectsearch-icon" alt="搜索" src="http://img.baidu.com/img/iknow/qb/select-search.png" />须先初始化。如果出错,则返回0</span>
	{
		if (gethostname(szLocalName, sizeof(szLocalName)) != 0)//产生错误
		{
			sLocalName = _T("没有取得");
			WSACleanup();
			return GetLastError();
		}
		else
		{
			sLocalName = szLocalName;//用sLocalName变量存储获得的计算机名称
			struct hostent FAR * lpHostEnt = gethostbyname(CStringA(sLocalName));
			if (lpHostEnt == NULL)//错误
			{
				sIpAddress = _T("");
				WSACleanup();
				return GetLastError();
			}
			LPSTR lpAddr = lpHostEnt->h_addr_list[0];//获取IP地址
			if (lpAddr)
			{
				struct in_addr inAddr;
				memmove(&inAddr, lpAddr, 4);
				sIpAddress = inet_ntoa(inAddr);//转换为标准格式
				if (sIpAddress.IsEmpty())
				{
					sIpAddress = _T("没有取得");
					WSACleanup();
					return 1;
				}
			}
		}
	}
	WSACleanup();//清空套接字初始化变量
	return 0;
}

//确定按钮
void CMFCP4ECurrentToolDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}

//退出按钮
void CMFCP4ECurrentToolDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	INT_PTR nRes;
	nRes = MessageBox(_T("您确定要退出吗？"), _T("提示"), MB_OKCANCEL | MB_ICONQUESTION);
	// 判断消息对话框返回值。如果为IDCANCEL就return，否则继续向下执行   
	if (IDCANCEL == nRes)
		return;

	CDialogEx::OnCancel();
}

