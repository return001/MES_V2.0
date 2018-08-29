
// MFCP3SIMPORTDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCP3SIMPORT.h"
#include "MFCP3SIMPORTDlg.h"
#include "afxdialogex.h"
#include "CRC.h"
#include "ThreadPool.h"
#include "Task.h"
#include "ADOManage.h"
#include "DBconfig.h"
#include <shlwapi.h>

#pragma comment(lib,"Shlwapi.lib") //判断文件是否存在的库，如果没有这行，会出现link错误

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//系统消息函数宏定义
#define WM_MainPortThreadControl WM_USER+1304
#define WM_MainDataInsertControl WM_USER+1305
#define WM_MainFontControl WM_USER+1306

//全局变量
volatile BOOL m_MainDownloadControl;//主控线程
HWND MainFormHWND;//主控线程句柄
CString strFolderpath, strOKFolderpath,strFolderFile;//放文件夹路径
CString StrFolder[4] = { L"", L"", L"", L"" };
CString strSingleFilePath = L"";
int simstart1flag = 0;
int simstart3flag = 0;
int simstart2flag = 0;
int simstart4flag = 0;


//串口单文件下载
volatile BOOL m_Port1SINGLEDownloadWrite1;
volatile BOOL m_Port1SINGLEDownloadWrite2;
volatile BOOL m_Port1SINGLEDownloadWrite3;
volatile BOOL m_Port1SINGLEDownloadWrite4;
volatile BOOL m_Port1SINGLEDownloadRead1;
volatile BOOL m_Port1SINGLEDownloadRead2;
volatile BOOL m_Port1SINGLEDownloadReadEnd2;
volatile BOOL m_Port1SINGLEDownloadRead3;
volatile BOOL m_Port1SINGLEDownloadRead4;

//串口1下载
volatile BOOL m_Port1DownloadWrite1;
volatile BOOL m_Port1DownloadWrite2;
volatile BOOL m_Port1DownloadWrite3;
volatile BOOL m_Port1DownloadWrite4;
volatile BOOL m_Port1DownloadRead1;
volatile BOOL m_Port1DownloadRead2;
volatile BOOL m_Port1DownloadReadEnd2;
volatile BOOL m_Port1DownloadRead3;
volatile BOOL m_Port1DownloadRead4;
volatile BOOL m_Port1DownloadControl;

//串口2下载
volatile BOOL m_Port2DownloadWrite1;
volatile BOOL m_Port2DownloadWrite2;
volatile BOOL m_Port2DownloadWrite3;
volatile BOOL m_Port2DownloadWrite4;
volatile BOOL m_Port2DownloadRead1;
volatile BOOL m_Port2DownloadRead2;
volatile BOOL m_Port2DownloadReadEnd2;
volatile BOOL m_Port2DownloadRead3;
volatile BOOL m_Port2DownloadRead4;
volatile BOOL m_Port2DownloadControl;

//串口3下载
volatile BOOL m_Port3DownloadWrite1;
volatile BOOL m_Port3DownloadWrite2;
volatile BOOL m_Port3DownloadWrite3;
volatile BOOL m_Port3DownloadWrite4;
volatile BOOL m_Port3DownloadRead1;
volatile BOOL m_Port3DownloadRead2;
volatile BOOL m_Port3DownloadReadEnd2;
volatile BOOL m_Port3DownloadRead3;
volatile BOOL m_Port3DownloadRead4;
volatile BOOL m_Port3DownloadControl;

//串口4下载
volatile BOOL m_Port4DownloadWrite1;
volatile BOOL m_Port4DownloadWrite2;
volatile BOOL m_Port4DownloadWrite3;
volatile BOOL m_Port4DownloadWrite4;
volatile BOOL m_Port4DownloadRead1;
volatile BOOL m_Port4DownloadRead2;
volatile BOOL m_Port4DownloadReadEnd2;
volatile BOOL m_Port4DownloadRead3;
volatile BOOL m_Port4DownloadRead4;
volatile BOOL m_Port4DownloadControl;

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


// CMFCP3SIMPORTDlg 对话框
CMFCP3SIMPORTDlg::CMFCP3SIMPORTDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCP3SIMPORTDlg::IDD, pParent)
	, initconfigflag(1)
	, simconnect1flag(0)
	, port1handler(NULL)
	, simconnect2flag(0)
	, port2handler(NULL)
	, simconnect3flag(0)
	, port3handler(NULL)
	, simconnect4flag(0)
	, port4handler(NULL)
	, m_pcnameEdit(_T(""))
	, m_pcipEdit(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCP3SIMPORTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PORTLIST1_COMBO, m_portlist1Combo);
	DDX_Control(pDX, IDC_CURRENTINFORMATION_RICHEDIT, m_currentinformationRichedit);
	DDX_Control(pDX, IDC_PORTLIST2_COMBO, m_portlist2Combo);
	DDX_Control(pDX, IDC_PORTLIST3_COMBO, m_portlist3Combo);
	DDX_Control(pDX, IDC_PORTLIST4_COMBO, m_portlist4Combo);
}

BEGIN_MESSAGE_MAP(CMFCP3SIMPORTDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMFCP3SIMPORTDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMFCP3SIMPORTDlg::OnBnClickedCancel)
	ON_CBN_DROPDOWN(IDC_PORTLIST1_COMBO, &CMFCP3SIMPORTDlg::OnCbnDropdownPortlist1Combo)
	ON_BN_CLICKED(IDC_MULTIPLEDOWNLOAD_RADIO, &CMFCP3SIMPORTDlg::OnBnClickedMultipledownloadRadio)
	ON_BN_CLICKED(IDC_SINGLEDOWNLOAD_RADIO, &CMFCP3SIMPORTDlg::OnBnClickedSingledownloadRadio)
	ON_BN_CLICKED(IDC_OPENSIMDATAFOLDERPATH_BUTTON, &CMFCP3SIMPORTDlg::OnBnClickedOpensimdatafolderpathButton)
	ON_BN_CLICKED(IDC_OPENSIMDATAFILEPATH_BUTTON, &CMFCP3SIMPORTDlg::OnBnClickedOpensimdatafilepathButton)
	ON_BN_CLICKED(IDC_PORT1CONNECT_BUTTON, &CMFCP3SIMPORTDlg::OnBnClickedPort1connectButton)
	ON_BN_CLICKED(IDC_STARTDOWNLOAD1_BUTTON, &CMFCP3SIMPORTDlg::OnBnClickedStartdownload1Button)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_MainPortThreadControl, &CMFCP3SIMPORTDlg::MainPortThreadControl)
	ON_MESSAGE(WM_MainDataInsertControl, &CMFCP3SIMPORTDlg::MainDataInsertControl)
	ON_MESSAGE(WM_MainFontControl, &CMFCP3SIMPORTDlg::MainFontControl)
	ON_BN_CLICKED(IDC_DBCONFIG_BUTTON, &CMFCP3SIMPORTDlg::OnBnClickedDbconfigButton)
	ON_BN_CLICKED(IDC_PORT2CONNECT_BUTTON, &CMFCP3SIMPORTDlg::OnBnClickedPort2connectButton)
	ON_BN_CLICKED(IDC_STARTDOWNLOAD2_BUTTON, &CMFCP3SIMPORTDlg::OnBnClickedStartdownload2Button)
	ON_BN_CLICKED(IDC_PORT3CONNECT_BUTTON, &CMFCP3SIMPORTDlg::OnBnClickedPort3connectButton)
	ON_BN_CLICKED(IDC_STARTDOWNLOAD3_BUTTON, &CMFCP3SIMPORTDlg::OnBnClickedStartdownload3Button)
	ON_BN_CLICKED(IDC_PORT4CONNECT_BUTTON, &CMFCP3SIMPORTDlg::OnBnClickedPort4connectButton)
	ON_BN_CLICKED(IDC_STARTDOWNLOAD4_BUTTON, &CMFCP3SIMPORTDlg::OnBnClickedStartdownload4Button)
	ON_BN_CLICKED(IDC_AUTOMULTIPLECONNECT_BUTTON, &CMFCP3SIMPORTDlg::OnBnClickedAutomultipleconnectButton)
	ON_BN_CLICKED(IDC_AUTOMULTIPLESTART_BUTTON, &CMFCP3SIMPORTDlg::OnBnClickedAutomultiplestartButton)
	ON_CBN_DROPDOWN(IDC_PORTLIST2_COMBO, &CMFCP3SIMPORTDlg::OnCbnDropdownPortlist2Combo)
	ON_CBN_DROPDOWN(IDC_PORTLIST3_COMBO, &CMFCP3SIMPORTDlg::OnCbnDropdownPortlist3Combo)
	ON_CBN_DROPDOWN(IDC_PORTLIST4_COMBO, &CMFCP3SIMPORTDlg::OnCbnDropdownPortlist4Combo)
END_MESSAGE_MAP()


// CMFCP3SIMPORTDlg 消息处理程序

BOOL CMFCP3SIMPORTDlg::OnInitDialog()
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

	//初始化本机信息
	GetLocalHostIPName(m_pcnameEdit, m_pcipEdit);//调用函数获得IP和计算机名称
	SetDlgItemText(IDC_PCIP_EDIT, m_pcipEdit);//将计算机的名称显示在名称控件中
	SetDlgItemText(IDC_PCNAME_EDIT, m_pcnameEdit);//将计算机的IP显示在IP控件中

	//初始化串口列表
	FindCommPort((CComboBox*)GetDlgItem(IDC_PORTLIST1_COMBO),Com1No,1);

	//初始化下载模式
	CheckDlgButton(IDC_SINGLEDOWNLOAD_RADIO, 1);
	SetInitConfigWindow();
	GetDlgItem(IDC_STARTDOWNLOAD1_BUTTON)->EnableWindow(FALSE);

	//将ini配置文件信息的东西读出来
	CString strpath;
	GetPrivateProfileString(_T("SimPathName"), _T("simfolderpathname"), _T(""), strpath.GetBuffer(100), 100, _T(".\\SystemInfo.ini"));
	SetDlgItemText(IDC_SIMDATAFOLDERPATH_EDIT, strpath);
	strpath.ReleaseBuffer();
	GetPrivateProfileString(_T("SimPathName"), _T("simfilepathname"), _T(""), strpath.GetBuffer(100), 100, _T(".\\SystemInfo.ini"));
	SetDlgItemText(IDC_SIMDATAFILEPATH_EDIT, strpath);
	strSingleFilePath = strpath;
	strpath.ReleaseBuffer();

	//字体初始化函数
	fontinit();

	//线程池初始化
	m_lpThreadPool = NULL;
	if (!m_lpThreadPool)
	{
		m_lpThreadPool = new CThreadPool();
	}

	MainFormHWND = AfxGetApp()->m_pMainWnd->m_hWnd;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCP3SIMPORTDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
void CMFCP3SIMPORTDlg::OnPaint()
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
HCURSOR CMFCP3SIMPORTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//控件初始化以及控制函数
//当前下载模式的控件控制
void CMFCP3SIMPORTDlg::SetInitConfigWindow()
{
	if (initconfigflag == 0)
	{
		GetDlgItem(IDC_SIMDATAFOLDERPATH_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_OPENSIMDATAFOLDERPATH_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_SIMDATAFILEPATH_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_OPENSIMDATAFILEPATH_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_AUTOMULTIPLECONNECT_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_PORTLIST2_COMBO)->EnableWindow(TRUE);
		GetDlgItem(IDC_PORT2CONNECT_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_PORTLIST3_COMBO)->EnableWindow(TRUE);
		GetDlgItem(IDC_PORT3CONNECT_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_PORTLIST4_COMBO)->EnableWindow(TRUE);
		GetDlgItem(IDC_PORT4CONNECT_BUTTON)->EnableWindow(TRUE);

		GetDlgItem(IDC_AUTOMULTIPLESTART_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_STARTDOWNLOAD2_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_STARTDOWNLOAD3_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_STARTDOWNLOAD4_BUTTON)->EnableWindow(TRUE);

	}
	else
	{
		GetDlgItem(IDC_SIMDATAFOLDERPATH_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_OPENSIMDATAFOLDERPATH_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_SIMDATAFILEPATH_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_OPENSIMDATAFILEPATH_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_AUTOMULTIPLESTART_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_AUTOMULTIPLECONNECT_BUTTON)->EnableWindow(FALSE);

		GetDlgItem(IDC_PORTLIST2_COMBO)->EnableWindow(FALSE);
		GetDlgItem(IDC_PORT2CONNECT_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_STARTDOWNLOAD2_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_PORTLIST3_COMBO)->EnableWindow(FALSE);
		GetDlgItem(IDC_PORT3CONNECT_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_STARTDOWNLOAD3_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_PORTLIST4_COMBO)->EnableWindow(FALSE);
		GetDlgItem(IDC_PORT4CONNECT_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_STARTDOWNLOAD4_BUTTON)->EnableWindow(FALSE);
	}
}

//控制其它窗口控件的
void CMFCP3SIMPORTDlg::Port1SetOtherWindowTrue()
{
	if (initconfigflag == 1)
	{
		GetDlgItem(IDC_DBCONFIG_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_OPENSIMDATAFILEPATH_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_MULTIPLEDOWNLOAD_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_SINGLEDOWNLOAD_RADIO)->EnableWindow(TRUE);
	}
	GetDlgItem(IDC_PORTLIST1_COMBO)->EnableWindow(TRUE);
}

void CMFCP3SIMPORTDlg::Port1SetOtherWindowFalse()
{
	if (initconfigflag == 1)
	{
		GetDlgItem(IDC_DBCONFIG_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_MULTIPLEDOWNLOAD_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_SINGLEDOWNLOAD_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_SIMDATAFILEPATH_EDIT)->EnableWindow(FALSE);
	}
	GetDlgItem(IDC_PORTLIST1_COMBO)->EnableWindow(FALSE);
}

void CMFCP3SIMPORTDlg::PortSetOtherWindowTrue()
{
	GetDlgItem(IDC_DBCONFIG_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_OPENSIMDATAFOLDERPATH_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_SIMDATAFOLDERPATH_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_MULTIPLEDOWNLOAD_RADIO)->EnableWindow(TRUE);
	GetDlgItem(IDC_SINGLEDOWNLOAD_RADIO)->EnableWindow(TRUE);
}

void CMFCP3SIMPORTDlg::PortSetOtherWindowFalse()
{
	GetDlgItem(IDC_DBCONFIG_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_OPENSIMDATAFOLDERPATH_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_SIMDATAFOLDERPATH_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_MULTIPLEDOWNLOAD_RADIO)->EnableWindow(FALSE);
	GetDlgItem(IDC_SINGLEDOWNLOAD_RADIO)->EnableWindow(FALSE);
}


//初始化配置模块函数
//点击批量下载单选框会做的事情
void CMFCP3SIMPORTDlg::OnBnClickedMultipledownloadRadio()
{
	// TODO:  在此添加控件通知处理程序代码
	initconfigflag = 0;
	SetInitConfigWindow();
	GetDlgItem(IDC_STARTDOWNLOAD2_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_STARTDOWNLOAD3_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_STARTDOWNLOAD4_BUTTON)->EnableWindow(FALSE);
	FindCommPort((CComboBox*)GetDlgItem(IDC_PORTLIST2_COMBO), Com2No, 2);
	FindCommPort((CComboBox*)GetDlgItem(IDC_PORTLIST3_COMBO), Com3No, 3);
	FindCommPort((CComboBox*)GetDlgItem(IDC_PORTLIST4_COMBO), Com4No, 4);
}

//点击单个下载单选框会做的事情
void CMFCP3SIMPORTDlg::OnBnClickedSingledownloadRadio()
{
	// TODO:  在此添加控件通知处理程序代码
	initconfigflag = 1;
	SetInitConfigWindow();
}

//点击浏览SIM卡数据路径按钮
void CMFCP3SIMPORTDlg::OnBnClickedOpensimdatafolderpathButton()
{
	// TODO:  在此添加控件通知处理程序代码
	//打开文件夹
	BROWSEINFO bi;
	char name[MAX_PATH];
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	bi.pszDisplayName = LPWSTR(name);
	bi.lpszTitle = L"选择文件夹目录";
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (idl == NULL)
		return ;
	CString strDirectoryPath;
	SHGetPathFromIDList(idl, strDirectoryPath.GetBuffer(MAX_PATH));
	strDirectoryPath.ReleaseBuffer();
	if (strDirectoryPath.IsEmpty())
		return ;
	if (strDirectoryPath.Right(1) != "\\")
		strDirectoryPath += "\\";

	//将选择的路径显示到SIM卡数据卡路径框中
	SetDlgItemText(IDC_SIMDATAFOLDERPATH_EDIT, strDirectoryPath);
	//将路径信息保存到信息文件中
	WritePrivateProfileString(_T("SimPathName"), _T("simfolderpathname"), strDirectoryPath, _T(".\\SystemInfo.ini"));
}

//点击浏览sim卡数据文件按钮
void CMFCP3SIMPORTDlg::OnBnClickedOpensimdatafilepathButton()
{
	// TODO:  在此添加控件通知处理程序代码
	//设置过滤器
	TCHAR szFilter[] = _T("SIM数据文件(*.der)|*.der|");
	//构造打开文件对话框
	CFileDialog fileDlg(TRUE, _T("der"), NULL, 0, szFilter, this);

	CString strFilePath;

	if (IDOK == fileDlg.DoModal())
	{
		// 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里
		strFilePath = fileDlg.GetPathName();
		SetDlgItemText(IDC_SIMDATAFILEPATH_EDIT, strFilePath);
		strSingleFilePath = strFilePath;

		//将路径信息保存到信息文件中
		WritePrivateProfileString(_T("SimPathName"), _T("simfilepathname"), strFilePath, _T(".\\SystemInfo.ini"));
	}
}


//SIM卡数据下载模块函数
//串口通用操作
//从注册表中寻找已经注册串口号然后展示出来
void CMFCP3SIMPORTDlg::FindCommPort(CComboBox *pComboBox, CString &ComNo, int PortNO)
{
	//先获取当前串口号
	int cur = 0;
	int nSel;
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
			if (nSel >= 0 && ComNo == LPWSTR(commName))//如果跟上次选择的相等那就不让当前选择变动，commName[6]是COMX的X。
			{
				cur = i - PortNO+1;
			}
			if (PortNO == 1 && i >= 0)
			{
				pComboBox->AddString(LPWSTR(commName)); // commName就是串口名字
			}
			else if (PortNO == 2 && i >= 1)
			{
				pComboBox->AddString(LPWSTR(commName));
			}
			else if (PortNO == 3 && i >= 2)
			{
				pComboBox->AddString(LPWSTR(commName));
			}
			else if (PortNO == 4 && i >= 3)
			{
				pComboBox->AddString(LPWSTR(commName));
			}
			i++;
		}
		if (pComboBox->GetCount() == 0)
		{
			pComboBox->ResetContent();
			SetRicheditText(L"找不到串口！请检测串口设备是否存在问题！", 1);
			//MessageBox(L"找不到串口！请检测串口设备是否存在问题！", L"提示信息", NULL);
			RegCloseKey(hKey);
			return;
		}
		pComboBox->SetCurSel(cur);
		RegCloseKey(hKey);
	}
}

//初始化串口
HANDLE CMFCP3SIMPORTDlg::InitCom(CString comName)
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
		SetRicheditText(comName+L"串口初始化失败！", 1);
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
int CMFCP3SIMPORTDlg::CloseCom(HANDLE handler)
{
	return CloseHandle(handler);
}

//获取当前串口号
void CMFCP3SIMPORTDlg::GetCommPort(CComboBox *pComboBox, CString &ComNo)
{
	int nSel;
	nSel = pComboBox->GetCurSel();
	pComboBox->GetLBText(nSel, ComNo);

	if (ComNo == "")
	{
		MessageBox(L"请先选择串口号！", L"提示信息", NULL);
	}
}


//串口号1的逻辑操作
//点击串口号下拉框的时候会自动更新串口号
void CMFCP3SIMPORTDlg::OnCbnDropdownPortlist1Combo()
{
	// TODO:  在此添加控件通知处理程序代码
	FindCommPort((CComboBox*)GetDlgItem(IDC_PORTLIST1_COMBO),Com1No,1);
}

//点击连接按钮
void CMFCP3SIMPORTDlg::OnBnClickedPort1connectButton()
{
	// TODO:  在此添加控件通知处理程序代码
	if (simconnect1flag == 0)
	{
		//先获取当前选择的串口号
		GetCommPort((CComboBox*)GetDlgItem(IDC_PORTLIST1_COMBO), Com1No);

		//然后再初始化串口号
		if (initconfigflag == 0)
		{
			if (simconnect2flag == 0 && simconnect3flag == 0 && simconnect4flag == 0)
			{
				GetDlgItemText(IDC_SIMDATAFOLDERPATH_EDIT, m_simdatafolderPath);

				if (m_simdatafolderPath == L"")
				{
					MessageBox(L"请选择SIM卡数据路径！", L"提示信息", NULL);
					return;
				}
			}
		}
		else if (initconfigflag == 1)
		{
			GetDlgItemText(IDC_SIMDATAFILEPATH_EDIT, m_simdatafilePath);
			if (m_simdatafilePath == L"")
			{
				MessageBox(L"请选择SIM卡数据文件！", L"提示信息", NULL);
				return;
			}
		}

		port1handler = InitCom(Com1No);

		if (port1handler == NULL)
		{
			return;
		}

		GetDlgItem(IDC_STARTDOWNLOAD1_BUTTON)->EnableWindow(TRUE);
		simconnect1flag = 1;
		SetDlgItemText(IDC_PORT1CONNECT_BUTTON, L"断开");
		Port1SetOtherWindowFalse();
		PortSetOtherWindowFalse();
		OnBnClickedStartdownload1Button();
		/*
		if (simconnect1flag == 1 && simconnect3flag == 1 && simconnect4flag == 1 && initconfigflag == 0)
		{

			simallconnectflag = 1;
			SetDlgItemText(IDC_AUTOMULTIPLECONNECT_BUTTON, L"一键断开");
			GetDlgItem(IDC_AUTOMULTIPLESTART_BUTTON)->EnableWindow(TRUE);
		}
        */
		return;
	}
	else if (simconnect1flag == 1)
	{
		if (simstart1flag == 1)
		{
			OnBnClickedStartdownload1Button();
		}
		if (!CloseCom(port1handler))
		{
			SetRicheditText(L"关闭串口失败", 1);
			MessageBox(L"关闭串口失败", L"提示信息", NULL);
			return;
		}

		SetRicheditText(L"关闭串口成功", 0);
		simconnect1flag = 0;
		SetDlgItemText(IDC_PORT1CONNECT_BUTTON, L"连接");
		Port1SetOtherWindowTrue();
		GetDlgItem(IDC_STARTDOWNLOAD1_BUTTON)->EnableWindow(FALSE);

		if (simconnect4flag == 0 && simconnect2flag == 0 && simconnect3flag == 0 && initconfigflag == 0)
		{
			PortSetOtherWindowTrue();
			//simallconnectflag = 0;
			//SetDlgItemText(IDC_AUTOMULTIPLECONNECT_BUTTON, L"一键连接");
			//GetDlgItem(IDC_AUTOMULTIPLESTART_BUTTON)->EnableWindow(TRUE);
		}
	}
}

//点击开始下载按钮
void CMFCP3SIMPORTDlg::OnBnClickedStartdownload1Button()
{
	// TODO:  在此添加控件通知处理程序代码
	if (initconfigflag == 1)
	{
		if (simstart1flag == 0)
		{
			if (!PathFileExists(strSingleFilePath))
			{
				MessageBox(L"文件不存在！请选择其它数据文件！",L"提示信息",NULL);
				return;
			}
			s_bSingleExit = FALSE;
			OpenThreadPoolTask(PORT_AUTO_THREAD);
			simstart1flag = 1;
			GetDlgItem(IDC_OPENSIMDATAFILEPATH_BUTTON)->EnableWindow(FALSE);
			SetDlgItemText(IDC_STARTDOWNLOAD1_BUTTON, L"停止下载");
		}
		else if (simstart1flag == 1)
		{
			s_bSingleExit = TRUE;
			simstart1flag = 0;
			GetDlgItem(IDC_OPENSIMDATAFILEPATH_BUTTON)->EnableWindow(TRUE);
			SetDlgItemText(IDC_STARTDOWNLOAD1_BUTTON, L"开始下载");
		}
	}
	else if(initconfigflag == 0)
	{
		if (simstart1flag == 0)
		{
			GetDlgItemText(IDC_SIMDATAFOLDERPATH_EDIT, strFolderpath);

			if (!PathIsDirectory(strFolderpath))
			{
				MessageBox(L"您所选择的文件夹不存在！请重新选择！",L"提示信息",NULL);
				return;
			}

			simstart1flag = 1;
			SetDlgItemText(IDC_STARTDOWNLOAD1_BUTTON, L"停止下载");
			m_Port1DownloadControl = TRUE;
			if (simstart2flag == 0 && simstart3flag == 0 && simstart4flag == 0)
			{
			    s_bExit = FALSE;
				OpenThreadPoolTask(PORT_AUTO_THREAD);
			}
		}
		else if (simstart1flag == 1)
		{
			if (simstart2flag == 0 && simstart3flag == 0 && simstart4flag == 0)
			{
				s_bExit = TRUE;
			}
			m_Port1DownloadControl = FALSE;
			simstart1flag = 0;
			SetDlgItemText(IDC_STARTDOWNLOAD1_BUTTON, L"开始下载");
		}
	}
}

//置串口1的编辑框为空
void CMFCP3SIMPORTDlg::SetPort1EditEmpty()
{
	SetDlgItemText(IDC_PORT1RID_EDIT, L"");
	SetDlgItemText(IDC_PORT1ICCID_EDIT, L"");
	SetDlgItemText(IDC_PORT1CID_EDIT, L"");
	SetDlgItemText(IDC_PORT1IMEI_EDIT, L"");
}


//串口号2的逻辑操作
//点击串口号下拉框的时候会自动更新串口号
void CMFCP3SIMPORTDlg::OnCbnDropdownPortlist2Combo()
{
	// TODO:  在此添加控件通知处理程序代码
	FindCommPort((CComboBox*)GetDlgItem(IDC_PORTLIST2_COMBO), Com2No, 2);
}

//点击连接按钮
void CMFCP3SIMPORTDlg::OnBnClickedPort2connectButton()
{
	// TODO:  在此添加控件通知处理程序代码
	if (simconnect2flag == 0)
	{
		//先获取当前选择的串口号
		GetCommPort((CComboBox*)GetDlgItem(IDC_PORTLIST2_COMBO), Com2No);

		//然后再初始化串口号
		if (simconnect1flag == 0 && simconnect3flag == 0 && simconnect4flag == 0 && simallconnectflag == 0)
		{
			GetDlgItemText(IDC_SIMDATAFOLDERPATH_EDIT, m_simdatafolderPath);

			if (m_simdatafolderPath == L"")
			{
				MessageBox(L"请选择SIM卡数据路径！", L"提示信息", NULL);
				return;
			}
		}

		port2handler = InitCom(Com2No);

		if (port2handler == NULL)
		{
			return;
		}

		simconnect2flag = 1;
		SetDlgItemText(IDC_PORT2CONNECT_BUTTON, L"断开");
		GetDlgItem(IDC_STARTDOWNLOAD2_BUTTON)->EnableWindow(TRUE);
		OnBnClickedStartdownload2Button();
		GetDlgItem(IDC_PORTLIST2_COMBO)->EnableWindow(FALSE);
		PortSetOtherWindowFalse();
		/*
		if (simconnect1flag == 1 && simconnect3flag == 1 && simconnect4flag == 1)
		{

			simallconnectflag = 1;
			//SetDlgItemText(IDC_AUTOMULTIPLECONNECT_BUTTON, L"一键断开");
			//GetDlgItem(IDC_AUTOMULTIPLESTART_BUTTON)->EnableWindow(TRUE);
		}
        */
		return;
	}
	else if (simconnect2flag == 1)
	{
		if (simstart2flag == 1)
		{
			OnBnClickedStartdownload2Button();
		}
		if (!CloseCom(port2handler))
		{
			SetRicheditText(L"关闭串口失败", 1);
			MessageBox(L"关闭串口失败", L"提示信息", NULL);
			return;
		}

		SetRicheditText(L"关闭串口成功", 0);
		simconnect2flag = 0;
		SetDlgItemText(IDC_PORT2CONNECT_BUTTON, L"连接");
		GetDlgItem(IDC_STARTDOWNLOAD2_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_PORTLIST2_COMBO)->EnableWindow(TRUE);
		if (simconnect1flag == 0 && simconnect3flag == 0 && simconnect4flag == 0)
		{
			PortSetOtherWindowTrue();
			//simallconnectflag = 0;
			//SetDlgItemText(IDC_AUTOMULTIPLECONNECT_BUTTON, L"一键连接");
			//GetDlgItem(IDC_AUTOMULTIPLESTART_BUTTON)->EnableWindow(TRUE);
		}
	}
}

//点击开始下载按钮
void CMFCP3SIMPORTDlg::OnBnClickedStartdownload2Button()
{
	// TODO:  在此添加控件通知处理程序代码
	if (simstart2flag == 0)
	{
		GetDlgItemText(IDC_SIMDATAFOLDERPATH_EDIT, strFolderpath);

		if (!PathIsDirectory(strFolderpath))
		{
			MessageBox(L"您所选择的文件夹不存在！请重新选择！", L"提示信息", NULL);
			return;
		}
		simstart2flag = 1;
		SetDlgItemText(IDC_STARTDOWNLOAD2_BUTTON, L"停止下载");
		m_Port2DownloadControl = TRUE;
		if (simstart1flag == 0 && simstart3flag == 0 && simstart4flag == 0)
		{
			s_bExit = FALSE;
			OpenThreadPoolTask(PORT_AUTO_THREAD);
		}
	}
	else if (simstart2flag == 1)
	{
		if (simstart1flag == 0 && simstart3flag == 0 && simstart4flag == 0)
		{
			s_bExit = TRUE;
		}
		m_Port2DownloadControl = FALSE;
		simstart2flag = 0;
		SetDlgItemText(IDC_STARTDOWNLOAD2_BUTTON, L"开始下载");
	}
}

//置串口2的编辑框为空
void CMFCP3SIMPORTDlg::SetPort2EditEmpty()
{
	SetDlgItemText(IDC_PORT2RID_EDIT, L"");
	SetDlgItemText(IDC_PORT2ICCID_EDIT, L"");
	SetDlgItemText(IDC_PORT2CID_EDIT, L"");
	SetDlgItemText(IDC_PORT2IMEI_EDIT, L"");
}


//串口号3的逻辑操作
//点击串口号下拉框的时候会自动更新串口号
void CMFCP3SIMPORTDlg::OnCbnDropdownPortlist3Combo()
{
	// TODO:  在此添加控件通知处理程序代码
	FindCommPort((CComboBox*)GetDlgItem(IDC_PORTLIST3_COMBO), Com3No, 3);
}

//点击连接按钮
void CMFCP3SIMPORTDlg::OnBnClickedPort3connectButton()
{
	// TODO:  在此添加控件通知处理程序代码
	if (simconnect3flag == 0)
	{
		//先获取当前选择的串口号
		GetCommPort((CComboBox*)GetDlgItem(IDC_PORTLIST3_COMBO), Com3No);

		//然后再初始化串口号
		if (simconnect1flag == 0 && simconnect2flag == 0 && simconnect4flag == 0 && simallconnectflag == 0)
		{
			GetDlgItemText(IDC_SIMDATAFOLDERPATH_EDIT, m_simdatafolderPath);

			if (m_simdatafolderPath == L"")
			{
				MessageBox(L"请选择SIM卡数据路径！", L"提示信息", NULL);
				return;
			}
		}

		port3handler = InitCom(Com3No);

		if (port3handler == NULL)
		{
			return;
		}

		simconnect3flag = 1;
		SetDlgItemText(IDC_PORT3CONNECT_BUTTON, L"断开");
		GetDlgItem(IDC_STARTDOWNLOAD3_BUTTON)->EnableWindow(TRUE);
		OnBnClickedStartdownload3Button();
		GetDlgItem(IDC_PORTLIST3_COMBO)->EnableWindow(FALSE);
		PortSetOtherWindowFalse();
		/*
		if (simconnect1flag == 1 && simconnect2flag == 1 && simconnect4flag == 1 && simallconnectflag == 1)
		{
			simallconnectflag = 1;
			SetDlgItemText(IDC_AUTOMULTIPLECONNECT_BUTTON, L"一键断开");
			GetDlgItem(IDC_AUTOMULTIPLESTART_BUTTON)->EnableWindow(TRUE);
		}
        */
		return;
	}
	else if (simconnect3flag == 1)
	{
		if (simstart3flag == 1)
		{
			OnBnClickedStartdownload3Button();
		}
		if (!CloseCom(port3handler))
		{
			SetRicheditText(L"关闭串口失败", 1);
			MessageBox(L"关闭串口失败", L"提示信息", NULL);
			return;
		}

		SetRicheditText(L"关闭串口成功", 0);
		simconnect3flag = 0;
		SetDlgItemText(IDC_PORT3CONNECT_BUTTON, L"连接");
		GetDlgItem(IDC_STARTDOWNLOAD3_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_PORTLIST3_COMBO)->EnableWindow(TRUE);
		if (simconnect1flag == 0 && simconnect2flag == 0 && simconnect4flag == 0)
		{
			PortSetOtherWindowTrue();
			//simallconnectflag = 0;
			//SetDlgItemText(IDC_AUTOMULTIPLECONNECT_BUTTON, L"一键连接");
			//GetDlgItem(IDC_AUTOMULTIPLESTART_BUTTON)->EnableWindow(TRUE);
		}
	}
}

//点击开始下载按钮
void CMFCP3SIMPORTDlg::OnBnClickedStartdownload3Button()
{
	// TODO:  在此添加控件通知处理程序代码
	if (simstart3flag == 0)
	{
		GetDlgItemText(IDC_SIMDATAFOLDERPATH_EDIT, strFolderpath);

		if (!PathIsDirectory(strFolderpath))
		{
			MessageBox(L"您所选择的文件夹不存在！请重新选择！", L"提示信息", NULL);
			return;
		}
		simstart3flag = 1;
		SetDlgItemText(IDC_STARTDOWNLOAD3_BUTTON, L"停止下载");
		m_Port3DownloadControl = TRUE;
		if (simstart2flag == 0 && simstart1flag == 0 && simstart4flag == 0)
		{
			s_bExit = FALSE;
			OpenThreadPoolTask(PORT_AUTO_THREAD);
		}
	}
	else if (simstart3flag == 1)
	{
		if (simstart1flag == 0 && simstart2flag == 0 && simstart4flag == 0)
		{
			s_bExit = TRUE;
		}
		m_Port3DownloadControl = FALSE;
		simstart3flag = 0;
		SetDlgItemText(IDC_STARTDOWNLOAD3_BUTTON, L"开始下载");
	}
}

//置串口3的编辑框为空
void CMFCP3SIMPORTDlg::SetPort3EditEmpty()
{
	SetDlgItemText(IDC_PORT3RID_EDIT, L"");
	SetDlgItemText(IDC_PORT3ICCID_EDIT, L"");
	SetDlgItemText(IDC_PORT3CID_EDIT, L"");
	SetDlgItemText(IDC_PORT3IMEI_EDIT, L"");
}


//串口号4的逻辑操作
//点击串口号下拉框的时候会自动更新串口号
void CMFCP3SIMPORTDlg::OnCbnDropdownPortlist4Combo()
{
	// TODO:  在此添加控件通知处理程序代码
	FindCommPort((CComboBox*)GetDlgItem(IDC_PORTLIST4_COMBO), Com4No, 4);
}

//点击连接按钮
void CMFCP3SIMPORTDlg::OnBnClickedPort4connectButton()
{
	// TODO:  在此添加控件通知处理程序代码
	if (simconnect4flag == 0)
	{
		//先获取当前选择的串口号
		GetCommPort((CComboBox*)GetDlgItem(IDC_PORTLIST4_COMBO), Com4No);

		//然后再初始化串口号
		if (simconnect1flag == 0 && simconnect2flag == 0 && simconnect3flag == 0)
		{
			GetDlgItemText(IDC_SIMDATAFOLDERPATH_EDIT, m_simdatafolderPath);

			if (m_simdatafolderPath == L"")
			{
				MessageBox(L"请选择SIM卡数据路径！", L"提示信息", NULL);
				return;
			}
		}

		port4handler = InitCom(Com4No);

		if (port4handler == NULL)
		{
			return;
		}

		simconnect4flag = 1;
		SetDlgItemText(IDC_PORT4CONNECT_BUTTON, L"断开");
		GetDlgItem(IDC_STARTDOWNLOAD4_BUTTON)->EnableWindow(TRUE);
		OnBnClickedStartdownload4Button();
		GetDlgItem(IDC_PORTLIST4_COMBO)->EnableWindow(FALSE);
		PortSetOtherWindowFalse();
		/*
		if (simconnect1flag == 1 && simconnect2flag == 1 && simconnect3flag == 1)
		{
			simallconnectflag = 1;
			SetDlgItemText(IDC_AUTOMULTIPLECONNECT_BUTTON, L"一键断开");
			GetDlgItem(IDC_AUTOMULTIPLESTART_BUTTON)->EnableWindow(TRUE);
		}
        */
		return;
	}
	else if (simconnect4flag == 1)
	{
		if (simstart4flag == 1)
		{
			OnBnClickedStartdownload4Button();
		}
		if (!CloseCom(port4handler))
		{
			SetRicheditText(L"关闭串口失败", 1);
			MessageBox(L"关闭串口失败", L"提示信息", NULL);
			return;
		}

		SetRicheditText(L"关闭串口成功", 0);
		simconnect4flag = 0;
		SetDlgItemText(IDC_PORT4CONNECT_BUTTON, L"连接");
		GetDlgItem(IDC_STARTDOWNLOAD4_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_PORTLIST4_COMBO)->EnableWindow(TRUE);
		if (simconnect1flag == 0 && simconnect2flag == 0 && simconnect3flag == 0)
		{
			PortSetOtherWindowTrue();
			//simallconnectflag = 0;
			//SetDlgItemText(IDC_AUTOMULTIPLECONNECT_BUTTON, L"一键连接");
			//etDlgItem(IDC_AUTOMULTIPLESTART_BUTTON)->EnableWindow(TRUE);
		}
	}
}

//点击开始下载按钮
void CMFCP3SIMPORTDlg::OnBnClickedStartdownload4Button()
{
	// TODO:  在此添加控件通知处理程序代码
	if (simstart4flag == 0)
	{
		GetDlgItemText(IDC_SIMDATAFOLDERPATH_EDIT, strFolderpath);

		if (!PathIsDirectory(strFolderpath))
		{
			MessageBox(L"您所选择的文件夹不存在！请重新选择！", L"提示信息", NULL);
			return;
		}
		simstart4flag = 1;
		SetDlgItemText(IDC_STARTDOWNLOAD4_BUTTON, L"停止下载");
		m_Port4DownloadControl = TRUE;
		if (simstart2flag == 0 && simstart3flag == 0 && simstart1flag == 0)
		{
			s_bExit = FALSE;
			OpenThreadPoolTask(PORT_AUTO_THREAD);
		}
	}
	else if (simstart4flag == 1)
	{
		if (simstart1flag == 0 && simstart2flag == 0 && simstart3flag == 0)
		{
			s_bExit = TRUE;
		}
		m_Port4DownloadControl = FALSE;
		simstart4flag = 0;
		SetDlgItemText(IDC_STARTDOWNLOAD4_BUTTON, L"开始下载");
	}
}

//置串口4的编辑框为空
void CMFCP3SIMPORTDlg::SetPort4EditEmpty()
{
	SetDlgItemText(IDC_PORT4RID_EDIT, L"");
	SetDlgItemText(IDC_PORT4ICCID_EDIT, L"");
	SetDlgItemText(IDC_PORT4CID_EDIT, L"");
	SetDlgItemText(IDC_PORT4IMEI_EDIT, L"");
}


//一键下载的逻辑操作
//点击一键连接按钮
void CMFCP3SIMPORTDlg::OnBnClickedAutomultipleconnectButton()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_portlist1Combo.GetCurSel() >= 0&&simconnect1flag==0)
	{
		OnBnClickedPort1connectButton();
	}
	if (m_portlist2Combo.GetCurSel() >= 0 && simconnect2flag == 0)
	{
		OnBnClickedPort2connectButton();
	}
	if (m_portlist3Combo.GetCurSel() >= 0 && simconnect3flag == 0)
	{
		OnBnClickedPort3connectButton();
	}
	if (m_portlist4Combo.GetCurSel() >= 0 && simconnect4flag == 0)
	{
		OnBnClickedPort4connectButton();
	}
}

//点击一键断开按钮
void CMFCP3SIMPORTDlg::OnBnClickedAutomultiplestartButton()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_portlist1Combo.GetCurSel() >= 0 && simconnect1flag == 1)
	{
		OnBnClickedPort1connectButton();
	}
	if (m_portlist2Combo.GetCurSel() >= 0 && simconnect2flag == 1)
	{
		OnBnClickedPort2connectButton();
	}
	if (m_portlist3Combo.GetCurSel() >= 0 && simconnect3flag == 1)
	{
		OnBnClickedPort3connectButton();
	}
	if (m_portlist4Combo.GetCurSel() >= 0 && simconnect4flag == 1)
	{
		OnBnClickedPort4connectButton();
	}
}


//文件下载写命令集合,0是TEST指令,1是RID指令,2是IMEI指令,3是开始下载指令,4是下载结束指令
CString CMFCP3SIMPORTDlg::CommandWriteUnit(int strcommandNo)
{
	CString strCommandWrite[5] = { 
		L"AT^GT_CM=TEST\r\n", //测试连接命令这个直接用就行
		L"AT^GT_CM=ID,1\r\n", //读RID的一个命令
		L"AT^GT_CM=IMEI\r\n", //读IMEI的一个命令
		L"AT^GT_CM=SOFTSIM_DATA,", //写文件的一个命令，后面还要带（第几个,数据大小,数据）
		L"AT^GT_CM=SOFTSIM_DATA,END,"};//表示文件写完了，后面还要带CRC16进制校验码
	return strCommandWrite[strcommandNo];
}

//文件下载写命令对应读字符串集合,0是TEST指令,1是RID指令,2是IMEI指令,3是开始下载指令,4是下载结束指令
CString CMFCP3SIMPORTDlg::CommandReadUnit(int strcommandNo)
{
	switch (strcommandNo)
	{
	case 0:
		return L"TEST_OK";
	case 1:
		return L"Chip RID:";
	case 2:
		return L"IMEI:";
	case 3:
		return L"";//文件下载是不需要读的，所以没有
	case 4:
		return L"SoftSim,";//后面要不就是OK!要不就是Error!要在线程读完后才去判断
	default:
		break;
	}
	return L"";
}


//线程主控函数
//开启线程池函数
void CMFCP3SIMPORTDlg::OpenThreadPoolTask(int Command)
{
	CTask* pTask = new CTask;
	pTask->SetCommand(Command);
	m_lpThreadPool->AddTask(pTask);
}

//消息循环主控函数
afx_msg LRESULT CMFCP3SIMPORTDlg::MainPortThreadControl(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	//这里是单文件下载的
		//一开始先是发送test命令，test命令的写和读同时开
	case MainPort_Single_Test:
		OpenThreadPoolTask(PORT1_SINGLEWRITE1_THREAD);
		OpenThreadPoolTask(PORT1_SINGLEREAD1_THREAD);
		break;
		//然后开始发送RID和IMEI命令，写和读分别开
	case MainPort_Single_Write2:
		OpenThreadPoolTask(PORT1_SINGLEWRITE2_THREAD);
		break;
	case MainPort_Single_Read2:
		OpenThreadPoolTask(PORT1_SINGLEREAD2_THREAD);
		break;
		//接着开始写文件命令，写完文件后会发写文件结束命令
	case MainPort_Single_Write3:
		OpenThreadPoolTask(PORT1_SINGLEWRITE3_THREAD);
		break;
		//发写文件结束命令的同时读文件结束命令会同时开启
	case MainPort_Single_Read3:
		OpenThreadPoolTask(PORT1_SINGLEREAD3_THREAD);
		break;
		//接着开始写老化命令
	case MainPort_Single_Write4:
		OpenThreadPoolTask(PORT1_SINGLEWRITE4_THREAD);
		break;
		//还有读老化命令
	case MainPort_Single_Read4:
		OpenThreadPoolTask(PORT1_SINGLEREAD4_THREAD);
		break;


	//这里开始是文件夹下载的
	//串口1
		//一开始先是发送test命令，test命令的写和读同时开
	case MainPort_Port1_Test:
		OpenThreadPoolTask(PORT1_WRITE1_THREAD);
		OpenThreadPoolTask(PORT1_READ1_THREAD);
		break;
		//然后开始发送RID和IMEI命令，写和读也分开
	case MainPort_Port1_Write2:
		OpenThreadPoolTask(PORT1_WRITE2_THREAD);
		break;
	case MainPort_Port1_Read2:
		OpenThreadPoolTask(PORT1_READ2_THREAD);
		break;
		//接着开始写文件命令，写完文件后会发写文件结束命令
	case MainPort_Port1_Write3:
		OpenThreadPoolTask(PORT1_WRITE3_THREAD);
		break;
		//发写文件结束命令的同时读文件结束命令会同时开启
	case MainPort_Port1_Read3:
		OpenThreadPoolTask(PORT1_READ3_THREAD);
		break;
		//接着开始写老化命令
	case MainPort_Port1_Write4:
		OpenThreadPoolTask(PORT1_WRITE4_THREAD);
		break;
		//还有读老化命令
	case MainPort_Port1_Read4:
		OpenThreadPoolTask(PORT1_READ4_THREAD);
		break;

	//串口2
		//一开始先是发送test命令，test命令的写和读同时开
	case MainPort_Port2_Test:
		OpenThreadPoolTask(PORT2_WRITE1_THREAD);
		OpenThreadPoolTask(PORT2_READ1_THREAD);
		break;
		//然后开始发送RID和IMEI命令，写和读也分开
	case MainPort_Port2_Write2:
		OpenThreadPoolTask(PORT2_WRITE2_THREAD);
		break;
	case MainPort_Port2_Read2:
		OpenThreadPoolTask(PORT2_READ2_THREAD);
		break;
		//接着开始写文件命令，写完文件后会发写文件结束命令
	case MainPort_Port2_Write3:
		OpenThreadPoolTask(PORT2_WRITE3_THREAD);
		break;
		//发写文件结束命令的同时读文件结束命令会同时开启
	case MainPort_Port2_Read3:
		OpenThreadPoolTask(PORT2_READ3_THREAD);
		break;
		//接着开始写老化命令
	case MainPort_Port2_Write4:
		OpenThreadPoolTask(PORT2_WRITE4_THREAD);
		break;
		//还有读老化命令
	case MainPort_Port2_Read4:
		OpenThreadPoolTask(PORT2_READ4_THREAD);
		break;

	//串口3
		//一开始先是发送test命令，test命令的写和读同时开
	case MainPort_Port3_Test:
		OpenThreadPoolTask(PORT3_WRITE1_THREAD);
		OpenThreadPoolTask(PORT3_READ1_THREAD);
		break;
		//然后开始发送RID和IMEI命令，写和读也分开
	case MainPort_Port3_Write2:
		OpenThreadPoolTask(PORT3_WRITE2_THREAD);
		break;
	case MainPort_Port3_Read2:
		OpenThreadPoolTask(PORT3_READ2_THREAD);
		break;
		//接着开始写文件命令，写完文件后会发写文件结束命令
	case MainPort_Port3_Write3:
		OpenThreadPoolTask(PORT3_WRITE3_THREAD);
		break;
		//发写文件结束命令的同时读文件结束命令会同时开启
	case MainPort_Port3_Read3:
		OpenThreadPoolTask(PORT3_READ3_THREAD);
		break;
		//接着开始写老化命令
	case MainPort_Port3_Write4:
		OpenThreadPoolTask(PORT3_WRITE4_THREAD);
		break;
		//还有读老化命令
	case MainPort_Port3_Read4:
		OpenThreadPoolTask(PORT3_READ4_THREAD);
		break;

	//串口4
		//一开始先是发送test命令，test命令的写和读同时开
	case MainPort_Port4_Test:
		OpenThreadPoolTask(PORT4_WRITE1_THREAD);
		OpenThreadPoolTask(PORT4_READ1_THREAD);
		break;
		//然后开始发送RID和IMEI命令，写和读也分开
	case MainPort_Port4_Write2:
		OpenThreadPoolTask(PORT4_WRITE2_THREAD);
		break;
	case MainPort_Port4_Read2:
		OpenThreadPoolTask(PORT4_READ2_THREAD);
		break;
		//接着开始写文件命令，写完文件后会发写文件结束命令
	case MainPort_Port4_Write3:
		OpenThreadPoolTask(PORT4_WRITE3_THREAD);
		break;
		//发写文件结束命令的同时读文件结束命令会同时开启
	case MainPort_Port4_Read3:
		OpenThreadPoolTask(PORT4_READ3_THREAD);
		break;
	case MainPort_Port4_Write4:
		OpenThreadPoolTask(PORT4_WRITE4_THREAD);
		break;
		//还有读老化命令
	case MainPort_Port4_Read4:
		OpenThreadPoolTask(PORT4_READ4_THREAD);
		break;

	default:
		break;
	}
	return 0;
}

//串口主控线程
void CMFCP3SIMPORTDlg::DownloadMainContralThread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	int findfileend=1;
	DWORD dwTotalSize;//文件总大小
	CStdioFile ReadFile;
	//CRC变量
	CString strCID;
	int crc16, strCIDcount;

	if (dlg->initconfigflag == 1)
	{
		strOKFolderpath = strSingleFilePath.Left(strSingleFilePath.GetLength() - 24) + L"OK\\";

		if (!PathIsDirectory(strOKFolderpath))
		{
			::CreateDirectory(strOKFolderpath, NULL);//创建目录,已有的话不影响
		}

		while (!s_bSingleExit)
		{
			::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Single_Test, NULL);
			Sleep(8000);
		}
	}
	else if (dlg->initconfigflag == 0)
	{
		strOKFolderpath = strFolderpath.Left(strFolderpath.GetLength() - 1) + L"OK\\";
		if (!PathIsDirectory(strOKFolderpath))
		{
			::CreateDirectory(strOKFolderpath, NULL);//创建目录,已有的话不影响
		}

		while (!s_bExit)
		{
			//找到一个文件
			CFileFind finder;
			bool bFind = finder.FindFile(strFolderpath + "*.*");

			while (bFind)
			{
				bFind = finder.FindNextFile();
				if (finder.IsDots())
				{
					if (!bFind)
					{
						findfileend = 0;
					}
					continue;
				}
				if (finder.IsDirectory())//是文件夹
				{
					strFolderFile = strFolderpath + finder.GetFileName() + L"\\assets.der";//文件夹名称
				}
				if (StrFolder[0] == strFolderFile)
				{
					if (!bFind)
					{
						findfileend = 0;
					}
					continue;
				}
				else if (StrFolder[1] == strFolderFile)
				{
					if (!bFind)
					{
						findfileend = 0;
					}
					continue;
				}
				else if (StrFolder[2] == strFolderFile)
				{
					if (!bFind)
					{
						findfileend = 0;
					}
					continue;
				}
				else if (StrFolder[3] == strFolderFile)
				{
					if (!bFind)
					{
						findfileend = 0;
					}
					continue;
				}
				//这里放判断种子号的数据库函数
				break;
			}

			finder.Close();

			//用if语句一个一个串口判断，不为空，且处于开始状态，就将文件丢进去
			if (StrFolder[0] == L""&& simstart1flag == 1 && findfileend == 1)
			{
				StrFolder[0] = strFolderFile;
				dlg->SetPort1EditEmpty();
				crc16 = 0;
				strCIDcount = 0;

				//先读文件CRC
				ReadFile.Open(StrFolder[0], CFile::modeRead);
				dwTotalSize = ReadFile.GetLength();//获取文件总长度
				char *crcBuf = new char[dwTotalSize + 1];
				ReadFile.Read(crcBuf, dwTotalSize);
				crc16 = GetCrc16(crcBuf, dwTotalSize);
				dlg->strport1crc16.Format(_T("%04x"), crc16);

				strCID = crcBuf;

				//读文件CID
				strCIDcount = strCID.Find(L"\f", strCIDcount);
				dlg->SetDlgItemText(IDC_PORT1CID_EDIT, strCID.Mid(strCIDcount + 1, 12));

				//读文件ICCID
				strCIDcount = strCID.Find(L"iccid", strCIDcount);
				dlg->SetDlgItemText(IDC_PORT1ICCID_EDIT, strCID.Mid(strCIDcount + 8, 19));
				delete[] crcBuf;
				crcBuf = NULL;
				ReadFile.Close();

				::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port1_Test, NULL);
				Sleep(1000);
			}
			else if (StrFolder[1] == L""&&simstart2flag == 1 && findfileend == 1)
			{
				StrFolder[1] = strFolderFile;
				dlg->SetPort2EditEmpty();
				crc16 = 0;
				strCIDcount = 0;

				//先读文件CRC
				ReadFile.Open(StrFolder[1], CFile::modeRead);
				dwTotalSize = ReadFile.GetLength();//获取文件总长度
				char *crcBuf = new char[dwTotalSize + 1];
				ReadFile.Read(crcBuf, dwTotalSize);
				crc16 = GetCrc16(crcBuf, dwTotalSize);
				dlg->strport2crc16.Format(_T("%04x"), crc16);

				strCID = crcBuf;

				//读文件CID
				strCIDcount = strCID.Find(L"\f", strCIDcount);
				dlg->SetDlgItemText(IDC_PORT2CID_EDIT, strCID.Mid(strCIDcount + 1, 12));

				//读文件ICCID
				strCIDcount = strCID.Find(L"iccid", strCIDcount);
				dlg->SetDlgItemText(IDC_PORT2ICCID_EDIT, strCID.Mid(strCIDcount + 8, 19));
				delete[] crcBuf;
				crcBuf = NULL;
				ReadFile.Close();

				::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port2_Test, NULL);
				Sleep(1000);
			}
			else if (StrFolder[2] == L""&&simstart3flag == 1 && findfileend == 1)
			{
				StrFolder[2] = strFolderFile;
				dlg->SetPort3EditEmpty();

				crc16 = 0;
				strCIDcount = 0;

				//先读文件CRC
				ReadFile.Open(StrFolder[2], CFile::modeRead);
				dwTotalSize = ReadFile.GetLength();//获取文件总长度
				char *crcBuf = new char[dwTotalSize + 1];
				ReadFile.Read(crcBuf, dwTotalSize);
				crc16 = GetCrc16(crcBuf, dwTotalSize);
				dlg->strport3crc16.Format(_T("%04x"), crc16);

				strCID = crcBuf;

				//读文件CID
				strCIDcount = strCID.Find(L"\f", strCIDcount);
				dlg->SetDlgItemText(IDC_PORT3CID_EDIT, strCID.Mid(strCIDcount + 1, 12));

				//读文件ICCID
				strCIDcount = strCID.Find(L"iccid", strCIDcount);
				dlg->SetDlgItemText(IDC_PORT3ICCID_EDIT, strCID.Mid(strCIDcount + 8, 19));
				delete[] crcBuf;
				crcBuf = NULL;
				ReadFile.Close();
				::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port3_Test, NULL);
				Sleep(1000);
			}
			else if (StrFolder[3] == L""&&simstart4flag == 1 && findfileend == 1)
			{
				StrFolder[3] = strFolderFile;
				dlg->SetPort4EditEmpty();

				crc16 = 0;
				strCIDcount = 0;

				//先读文件CRC
				ReadFile.Open(StrFolder[3], CFile::modeRead);
				dwTotalSize = ReadFile.GetLength();//获取文件总长度
				char *crcBuf = new char[dwTotalSize + 1];
				ReadFile.Read(crcBuf, dwTotalSize);
				crc16 = GetCrc16(crcBuf, dwTotalSize);
				dlg->strport4crc16.Format(_T("%04x"), crc16);

				strCID = crcBuf;

				//读文件CID
				strCIDcount = strCID.Find(L"\f", strCIDcount);
				dlg->SetDlgItemText(IDC_PORT4CID_EDIT, strCID.Mid(strCIDcount + 1, 12));

				//读文件ICCID
				strCIDcount = strCID.Find(L"iccid", strCIDcount);
				dlg->SetDlgItemText(IDC_PORT4ICCID_EDIT, strCID.Mid(strCIDcount + 8, 19));
				delete[] crcBuf;
				crcBuf = NULL;
				ReadFile.Close();
				::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port4_Test, NULL);
				Sleep(1000);
			}
			Sleep(500);
            
			bFind = finder.FindFile(strFolderpath + "*.*");
			while (bFind)
			{
				bFind = finder.FindNextFile();
			if (finder.IsDots())
			{
				if (!bFind)
				{
					if (StrFolder[0] == L""&&StrFolder[1] == L""&&StrFolder[2] == L""&&StrFolder[3] == L""&&bFind == 0)
					{
						dlg->OnBnClickedAutomultiplestartButton();
						::MessageBox(dlg->m_hWnd, L"文件已经全部下载完成", L"提示信息", NULL);
					}
				}
				continue;
			}
			}
			finder.Close();

			if (StrFolder[0] != L""&& simstart1flag == 1)
			{
				::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port1_Test, NULL);
				Sleep(500);
			}
			if (StrFolder[1] != L""&&simstart2flag == 1)
			{
				::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port2_Test, NULL);
				Sleep(500);
			}
			if (StrFolder[2] != L""&&simstart3flag == 1)
			{
				::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port3_Test, NULL);
				Sleep(500);
			}
			if (StrFolder[3] != L""&&simstart4flag == 1)
			{
				::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port4_Test, NULL);
				Sleep(500);
			}
		}
	}
	return;
}


//单文件串口下载
//单文件串口1的test写线程
void CMFCP3SIMPORTDlg::SingleDownloadWrite1Port1Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	PurgeComm(port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;

	//放指令用变量
	CString strcommand = L"AT^GT_CM=TEST\r\n";

	//一直test，看看串口有没有连接上
	dlg->SetPort1EditEmpty();
	//dlg->GetDlgItem(IDC_OPENSIMDATAFILEPATH_BUTTON)->EnableWindow(TRUE);
	m_Port1SINGLEDownloadWrite1 = TRUE;

	ClearCommError(dlg->port1handler, &dwErrorFlags, &ComStat);
	::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_SinglePort_SimDataNoIsExit, NULL);
	Sleep(50);
	do
	{
		dlg->SetRicheditText(L"发:" + strcommand, 0);
		bWriteStat = WriteFile(dlg->port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(1500);

		if (s_bSingleExit == TRUE)
		{
			dlg->SingleDownloadClosePort1Thread();
			dlg->SingleDownloadRestPort1Thread();
			return;
		}
	} while (m_Port1SINGLEDownloadWrite1);

	return;
}

//单文件串口1的test读线程
void CMFCP3SIMPORTDlg::SingleDownloadRead1Port1Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	//串口变量
	char str[100];
	memset(str, 0, sizeof(str));
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;

	m_Port1SINGLEDownloadRead1 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程
	//dlg->GetDlgItem(IDC_OPENSIMDATAFILEPATH_BUTTON)->EnableWindow(TRUE);
	do
	{
		Sleep(1520);
		bReadStat = ReadFile(dlg->port1handler, str, 100, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			if (strread.Find(L"TEST_OK")>= 0)
			{
				m_Port1SINGLEDownloadRead1 = FALSE;
				m_Port1SINGLEDownloadWrite1 = FALSE;
			}
		}
		if (s_bSingleExit == TRUE)
		{
			dlg->SingleDownloadClosePort1Thread();
			dlg->SingleDownloadRestPort1Thread();
			return;
		}
	} while (m_Port1SINGLEDownloadRead1);
	dlg->SetRicheditText(L"收:" + strread, 0);
	PurgeComm(port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	//dlg->GetDlgItem(IDC_OPENSIMDATAFILEPATH_BUTTON)->EnableWindow(FALSE);

	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Single_Write2, NULL);
	return;
}

//单文件串口1的读CID、ICCID，写RID、IMEI的写线程
void CMFCP3SIMPORTDlg::SingleDownloadWrite2Port1Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Connected, NULL);
	PurgeComm(port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	DWORD dwTotalSize;//文件总大小

	//放指令用变量
	CString strcommand;
	CString strfilewritecommand;

	//CRC变量
	CString strCID;
	int crc16, strCIDcount;

	//判断IMEI和RID是否为空的变量
	CString strImeiRid;

	//文件下载地址
	CString strpath;
	dlg->GetDlgItemText(IDC_SIMDATAFILEPATH_EDIT, strpath);

	crc16 = 0;
	strCIDcount = 0;

	//先读文件CRC
	CStdioFile ReadFile;
	BOOL bOpen = ReadFile.Open(strpath, CFile::modeRead);
	dwTotalSize = ReadFile.GetLength();//获取文件总长度
	char *crcBuf = new char[dwTotalSize + 1];
	ReadFile.Read(crcBuf, dwTotalSize);
	crc16 = GetCrc16(crcBuf, dwTotalSize);
	dlg->strsinglecrc16.Format(_T("%04x"), crc16);

	strCID = crcBuf;

	//读文件CID
	strCIDcount = strCID.Find(L"\f", strCIDcount);
	dlg->SetDlgItemText(IDC_PORT1CID_EDIT, strCID.Mid(strCIDcount + 1, 12));

	//读文件ICCID
	strCIDcount = strCID.Find(L"iccid", strCIDcount);
	dlg->SetDlgItemText(IDC_PORT1ICCID_EDIT, strCID.Mid(strCIDcount + 8, 19));
	delete[] crcBuf;
	crcBuf = NULL;
	ReadFile.Close();

	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Single_Read2, NULL);

	Sleep(100);
	PurgeComm(port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	//连接上了就开始读它的RID和IMEI，如果连续发送五条命令都没反应，那就返回Test那里重新检测设备
	for (int i = 0; i < 2; i++)
	{
		int count = 0;
		if (i == 0)
		{
			strcommand = L"AT^GT_CM=ID,1\r\n";//读RID的一个命令
		}
		else if (i == 1)
		{
			strcommand = L"AT^GT_CM=IMEI\r\n";//读IMEI的一个命令
		}

		ClearCommError(dlg->port1handler, &dwErrorFlags, &ComStat);

		//然后同时开启读线程
		m_Port1SINGLEDownloadWrite2 = TRUE;

		PurgeComm(port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
		do
		{

			dlg->SetRicheditText(L"发:" + strcommand, 0);
			bWriteStat = WriteFile(dlg->port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);

			if (count == 7)
			{
				::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Fail, NULL);
				dlg->SingleDownloadClosePort1Thread();
				dlg->SingleDownloadRestPort1Thread();
				ReadFile.Close();
				return;
			}
			count++;
			Sleep(600);
			if (s_bSingleExit == TRUE)
			{
				dlg->SingleDownloadClosePort1Thread();
				dlg->SingleDownloadRestPort1Thread();
				return;
			}
		} while (m_Port1SINGLEDownloadWrite2);
	}
	ReadFile.Close();
	return;
}

//单文件串口1的RID、IMEI读线程
void CMFCP3SIMPORTDlg::SingleDownloadRead2Port1Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	//串口变量
	char str[100];
	memset(str, 0, sizeof(str));
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;
	CString strcount, strcounttemp;//放RID和IMEI用
	CString strtemp;
	int findcount1, findcount2;//切割读出来的字符串用

	for (int i = 0; i < 2; i++)
	{
		m_Port1SINGLEDownloadRead2 = TRUE;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程
	    m_Port1SINGLEDownloadReadEnd2 = TRUE;
		int count = 0;
		if (i == 0)
		{
			strtemp = L"Chip RID:";//读RID的一个命令
		}
		else if (i == 1)
		{
			strtemp = L"IMEI:";//读IMEI的一个命令
		}
		do
		{
			Sleep(620);
			bReadStat = ReadFile(dlg->port1handler, str, 100, &readreal, 0);
			if (bReadStat)
			{
				strread = str;
				if ((findcount1 = strread.Find(strtemp)) >= 0)
				{
					//读RID
					if (i == 0)
					{
						strcount = strread.Right(strread.GetLength() - findcount1 - 9);
						findcount2 = strcount.Find(L"\r\n");
						strcount = strcount.Left(findcount2);
						if (strcount != "")
						{
							dlg->SetDlgItemText(IDC_PORT1RID_EDIT, strcount);
						}
						else if (strcount == "")
						{
							continue;
						}
					}
					//读IMEI
					else if (i == 1)
					{
						strcount = strread.Right(strread.GetLength() - findcount1 - 5);
						findcount2 = strcount.Find(L"\r\n");
						strcount = strcount.Left(findcount2);
						strcounttemp = strcount;
						if (strcount != ""&&strcounttemp.Trim(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789").GetLength() == 0)
						{
							dlg->SetDlgItemText(IDC_PORT1IMEI_EDIT, strcount);
							::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port1_IsExit, NULL);
							Sleep(50);
						}
						else
						{
							continue;
						}
					}
					m_Port1SINGLEDownloadWrite2 = FALSE;
					m_Port1SINGLEDownloadRead2 = FALSE;
				}
			}
		} while (m_Port1SINGLEDownloadRead2);
		if (m_Port1SINGLEDownloadReadEnd2 == FALSE)
		{
			return;
		}
		if (s_bSingleExit == TRUE)
		{
			dlg->SingleDownloadClosePort1Thread();
			dlg->SingleDownloadRestPort1Thread();
			return;
		}
		dlg->SetRicheditText(L"收:" + strread, 0);
	}
	PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Single_Write3, NULL);
	return;
}

//单文件串口1的写文件和写结束命令的写线程
void CMFCP3SIMPORTDlg::SingleDownloadWrite3Port1Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	DWORD dwTotalSize;//文件总大小
	DWORD dwStep = 490;//一次读490个字符
	DWORD dwRead;//实际读的字符数

	//放指令用变量
	CString strcommand;
	CString strfilewritecommand;

	//文件下载变量
	CString strdatano, strdatalenth, strdata;//分别为第几条数据,数据长度，数据大小
	int intdatano = 0;//第几条数据

	//文件下载地址
	CString strpath;
	dlg->GetDlgItemText(IDC_SIMDATAFILEPATH_EDIT, strpath);

	CStdioFile ReadFile1;
	BOOL bOpen = ReadFile1.Open(strpath, CFile::modeRead);
	dwTotalSize = ReadFile1.GetLength();//获取文件总长度

	//开始写文件
	strcommand = L"AT^GT_CM=SOFTSIM_DATA,";

	//数据缓冲区
	char *pBuf = new char[dwStep + 1];
	memset(pBuf, '\0', dwStep + 1);

	ClearCommError(dlg->port1handler, &dwErrorFlags, &ComStat);

	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Downloading, NULL);
	PurgeComm(port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	while ((dwRead = ReadFile1.Read(pBuf, dwStep)) > 0)
	{
		//文件分割发送
		strdatano.Format(L"%d", intdatano);
		char *buf1 = new char[2 * (dwRead + 1)];
		memset(buf1, '\0', 2 * (dwRead + 1));
		//转换成16进制
		for (unsigned int j = 0; j < dwRead; j++)
		{
			unsigned char ddd = pBuf[j];
			if ((ddd / 16) < 10)
				buf1[2 * j] = (ddd / 16) + 48;
			else
				buf1[2 * j] = (ddd / 16) - 10 + 65;   //10=A  
			if ((ddd % 16) < 10)
				buf1[2 * j + 1] = (ddd % 16) + 48;
			else
				buf1[2 * j + 1] = (ddd % 16) - 10 + 65; //9=A  
		}
		strdata = buf1;
		strdatalenth.Format(L"%d", dwRead);
		strfilewritecommand = strcommand + strdatano + L"," + strdatalenth + L"," + strdata + L"\r\n";
		dlg->SetRicheditText(L"发:" + strfilewritecommand, 0);
		dwBytesWrite = strcommand.GetLength();
		bWriteStat = WriteFile(dlg->port1handler, CT2A(strfilewritecommand), strfilewritecommand.GetLength(), &dwBytesWrite, NULL);

		intdatano++;
		memset(pBuf, '\0', dwStep + 1);
		delete[] buf1;
		buf1 = NULL;
        Sleep(400);
	}
	delete[] pBuf;
	pBuf = NULL;
	ReadFile1.Close();
	if (s_bSingleExit == TRUE)
	{
		dlg->SingleDownloadClosePort1Thread();
		dlg->SingleDownloadRestPort1Thread();
		return;
	}
	PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	strcommand = L"AT^GT_CM=SOFTSIM_DATA,END," + dlg->strsinglecrc16 + L"\r\n";
	ClearCommError(dlg->port1handler, &dwErrorFlags, &ComStat);

	//同时开启读线程
	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Single_Read3, NULL);
	m_Port1SINGLEDownloadWrite3 = TRUE;

	int countend = 0;
	do
	{
		dlg->SetRicheditText(L"发:" + strcommand, 0);
		if (countend == 3)
		{
			::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Fail, NULL);
			m_Port1SINGLEDownloadWrite3 = FALSE;
			m_Port1SINGLEDownloadRead3 = FALSE;
			return;
		}
		bWriteStat = WriteFile(dlg->port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(1000);
		countend++;
		if (s_bSingleExit == TRUE)
		{
			dlg->SingleDownloadClosePort1Thread();
			dlg->SingleDownloadRestPort1Thread();
			return;
		}
	} while (m_Port1SINGLEDownloadWrite3);

	//dlg->SingleDownloadRestPort1Thread();
	return;
}

//单文件串口1的读结束命令的读线程
void CMFCP3SIMPORTDlg::SingleDownloadRead3Port1Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	char str[100];
	memset(str, 0, sizeof(str));
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;

	m_Port1SINGLEDownloadRead3 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程

	do
	{
		Sleep(1020);
		bReadStat = ReadFile(dlg->port1handler, str, 100, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			if (strread.Find(L"SoftSim,") >= 0)
			{
					if (strread.Find(L"OK") >= 0)
					{
						//::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Success, NULL);
						//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port1_OkInsert, NULL);
					}
					if (strread.Find(L"Error") >= 0)
					{
						::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Fail, NULL);
						::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port1_ErrorInsert, NULL);
						m_Port1SINGLEDownloadWrite3 = FALSE;
						m_Port1SINGLEDownloadRead3 = FALSE;
						dlg->SingleDownloadRestPort1Thread();
						dlg->SetRicheditText(L"收:" + strread, 0);
						return;
					}
				m_Port1SINGLEDownloadRead3 = FALSE;
				m_Port1SINGLEDownloadWrite3 = FALSE;
			}
		}
		if (s_bSingleExit == TRUE)
		{
			dlg->SingleDownloadClosePort1Thread();
			dlg->SingleDownloadRestPort1Thread();
			return;
		}
	} while (m_Port1SINGLEDownloadRead3);
	dlg->SetRicheditText(L"收:" + strread, 0);
	PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Single_Write4, NULL);
	return;
}

//单文件串口1的写老化命令的写线程
void CMFCP3SIMPORTDlg::SingleDownloadWrite4Port1Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;

	//放指令用变量
	CString strcommand;

	strcommand = L"AT^GT_CM=aging,on\r\n";
	ClearCommError(dlg->port1handler, &dwErrorFlags, &ComStat);

	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Ageing, NULL);

	//同时开启读线程
	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Single_Read4, NULL);
	m_Port1SINGLEDownloadWrite4 = TRUE;

	int countend = 0;
	do
	{
		dlg->SetRicheditText(L"发:" + strcommand, 0);
		if (countend == 3)
		{
			::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Fail, NULL);
			m_Port1SINGLEDownloadWrite4 = FALSE;
			m_Port1SINGLEDownloadRead4 = FALSE;

			dlg->SingleDownloadRestPort1Thread();
			return;
		}
		bWriteStat = WriteFile(dlg->port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(1000);
		countend++;
		if (s_bSingleExit == TRUE)
		{
			dlg->SingleDownloadClosePort1Thread();
			dlg->SingleDownloadRestPort1Thread();
			return;
		}
	} while (m_Port1SINGLEDownloadWrite4);

	dlg->SingleDownloadRestPort1Thread();
}

//单文件串口1的读老化命令的读线程
void CMFCP3SIMPORTDlg::SingleDownloadRead4Port1Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	char str[100];
	memset(str, 0, sizeof(str));
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;

	m_Port1SINGLEDownloadRead4 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程

	do
	{
		Sleep(1020);
		bReadStat = ReadFile(dlg->port1handler, str, 100, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			if (strread.Find(L"aging,on\r\r\nOK!") >= 0)
			{
				::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Success, NULL);
				::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port1_OkInsert, NULL);
			}
			m_Port1SINGLEDownloadRead4 = FALSE;
			m_Port1SINGLEDownloadWrite4 = FALSE;
		}
		if (s_bSingleExit == TRUE)
		{
			dlg->SingleDownloadClosePort1Thread();
			dlg->SingleDownloadRestPort1Thread();
			return;
		}
	} while (m_Port1SINGLEDownloadRead4);
	dlg->SetRicheditText(L"收:" + strread, 0);
	PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	dlg->SingleDownloadRestPort1Thread();
	return;
}

//单文件串口重置线程全局变量
void CMFCP3SIMPORTDlg::SingleDownloadRestPort1Thread()
{
	m_TreadPort1SINGLEDownloadWrite1 = TRUE;
	m_TreadPort1SINGLEDownloadWrite2 = TRUE;
	m_TreadPort1SINGLEDownloadWrite3 = TRUE;
	m_TreadPort1SINGLEDownloadWrite4 = TRUE;
	m_TreadPort1SINGLEDownloadRead1 = TRUE;
	m_TreadPort1SINGLEDownloadRead2 = TRUE;
	m_TreadPort1SINGLEDownloadRead3 = TRUE;
	m_TreadPort1SINGLEDownloadRead4 = TRUE;
}

//单文件串口关闭线程全局变量
void CMFCP3SIMPORTDlg::SingleDownloadClosePort1Thread()
{
	m_Port1SINGLEDownloadWrite1 = FALSE;
	m_Port1SINGLEDownloadWrite2 = FALSE;
	m_Port1SINGLEDownloadWrite3 = FALSE;
	m_Port1SINGLEDownloadWrite4 = FALSE;
	m_Port1SINGLEDownloadRead1 = FALSE;
	m_Port1SINGLEDownloadRead2 = FALSE;
	m_Port1SINGLEDownloadReadEnd2 = FALSE;
	m_Port1SINGLEDownloadRead3 = FALSE;
	m_Port1SINGLEDownloadRead4 = FALSE;
}


//多文件串口下载

//串口1下载逻辑
//串口1的test写线程
void CMFCP3SIMPORTDlg::DownloadWrite1Port1Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	PurgeComm(port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;

	//放指令用变量
	CString strcommand = L"AT^GT_CM=TEST\r\n";

	//一直test，看看串口有没有连接上
	
	m_Port1DownloadWrite1 = TRUE;

	ClearCommError(dlg->port1handler, &dwErrorFlags, &ComStat);
	Sleep(50);
	do
	{
		dlg->SetRicheditText(L"发:" + strcommand, 0);
		bWriteStat = WriteFile(dlg->port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(1500);

		if (s_bExit == TRUE||m_Port1DownloadControl == FALSE)
		{
			dlg->DownloadClosePort1Thread();
			dlg->DownloadRestPort1Thread();
			return;
		}
	} while (m_Port1DownloadWrite1);

	return ;
}

//串口1的test读线程
void CMFCP3SIMPORTDlg::DownloadRead1Port1Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	//串口变量
	char str[100];
	memset(str, 0, sizeof(str));
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;

	m_Port1DownloadRead1 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程
	do
	{
		Sleep(1520);
		bReadStat = ReadFile(dlg->port1handler, str, 100, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			if (strread.Find(L"TEST_OK") >= 0)
			{
				m_Port1DownloadRead1 = FALSE;
				m_Port1DownloadWrite1 = FALSE;
			}
		}
		if (s_bExit == TRUE || m_Port1DownloadControl == FALSE)
		{
			dlg->DownloadClosePort1Thread();
			dlg->DownloadRestPort1Thread();
			return;
		}
	} while (m_Port1DownloadRead1);
	dlg->SetRicheditText(L"收:" + strread, 0);
	PurgeComm(port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port1_Write2, NULL);
}

//串口1的写RID、IMEI的写线程
void CMFCP3SIMPORTDlg::DownloadWrite2Port1Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Connected, NULL);
	PurgeComm(port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;

	//放指令用变量
	CString strcommand;
	CString strfilewritecommand;

	//连接上了就开始读它的RID和IMEI，如果连续发送五条命令都没反应，那就返回Test那里重新检测设备
	for (int i = 0; i < 2; i++)
	{
		int count = 0;
		if (i == 0)
		{
			strcommand = L"AT^GT_CM=ID,1\r\n";//读RID的一个命令
		}
		else if (i == 1)
		{
			strcommand = L"AT^GT_CM=IMEI\r\n";//读IMEI的一个命令
		}

		ClearCommError(dlg->port1handler, &dwErrorFlags, &ComStat);

		//然后同时开启读线程
		m_Port1DownloadWrite2 = TRUE;
		::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port1_Read2, NULL);
		do
		{
			dlg->SetRicheditText(L"发:" + strcommand, 0);
			bWriteStat = WriteFile(dlg->port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);

			if (count == 7)
			{
				::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Fail, NULL);
				dlg->DownloadClosePort1Thread();
				dlg->DownloadRestPort1Thread();
				return;
			}
			count++;
			Sleep(600);
			if (s_bExit == TRUE || m_Port1DownloadControl == FALSE)
			{
				dlg->DownloadClosePort1Thread();
				dlg->DownloadRestPort1Thread();
				return;
			}
		} while (m_Port1DownloadWrite2);
	}
}

//串口1的RID、IMEI读线程
void CMFCP3SIMPORTDlg::DownloadRead2Port1Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	//串口变量
	char str[100];
	memset(str, 0, sizeof(str));
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;
	CString strcount,strcounttemp;//放RID和IMEI用
	CString strtemp;
	int findcount1, findcount2;//切割读出来的字符串用

	for (int i = 0; i < 2; i++)
	{
		m_Port1DownloadRead2 = TRUE;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程
		m_Port1DownloadReadEnd2 = TRUE;
		int count = 0;
		if (i == 0)
		{
			strtemp = L"Chip RID:";//读RID的一个命令
		}
		else if (i == 1)
		{
			strtemp = L"IMEI:";//读IMEI的一个命令
		}
		do
		{
			Sleep(620);
			bReadStat = ReadFile(dlg->port1handler, str, 100, &readreal, 0);
			if (bReadStat)
			{
				strread = str;
				if ((findcount1 = strread.Find(strtemp)) >= 0)
				{
					//读RID
					if (i == 0)
					{
						strcount = strread.Right(strread.GetLength() - findcount1 - 9);
						findcount2 = strcount.Find(L"\r\n");
						strcount = strcount.Left(findcount2);
						if (strcount != "")
						{
							dlg->SetDlgItemText(IDC_PORT1RID_EDIT, strcount);
						}
						else if (strcount == "")
						{
							continue;
						}
					}
					//读IMEI
					else if (i == 1)
					{
						strcount = strread.Right(strread.GetLength() - findcount1 - 5);
						findcount2 = strcount.Find(L"\r\n");
						strcount = strcount.Left(findcount2);
						strcounttemp = strcount;
						if (strcount != ""&&strcounttemp.Trim(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789").GetLength() == 0)
						{
							dlg->SetDlgItemText(IDC_PORT1IMEI_EDIT, strcount);
							//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port1_IsExit, NULL);
							Sleep(50);
						}
						else
						{
							continue;
						}
					}
					m_Port1DownloadWrite2 = FALSE;
					m_Port1DownloadRead2 = FALSE;
				}
			}
		} while (m_Port1DownloadRead2);
		if (m_Port1DownloadReadEnd2 == FALSE)
		{
			return;
		}
		if (s_bExit == TRUE || m_Port1DownloadControl == FALSE)
		{
			dlg->DownloadClosePort1Thread();
			dlg->DownloadRestPort1Thread();
			return;
		}
		dlg->SetRicheditText(L"收:" + strread, 0);
	}
	PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port1_Write3, NULL);
	return;
}

//串口1的写文件和写结束命令的写线程
void CMFCP3SIMPORTDlg::DownloadWrite3Port1Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	DWORD dwTotalSize;//文件总大小
	DWORD dwStep = 490;//一次读490个字符
	DWORD dwRead;//实际读的字符数

	//放指令用变量
	CString strcommand;
	CString strfilewritecommand;

	//文件下载变量
	CString strdatano, strdatalenth, strdata;//分别为第几条数据,数据长度，数据大小
	int intdatano = 0;//第几条数据

	//文件下载地址
	CStdioFile ReadFile1;
	BOOL bOpen = ReadFile1.Open(StrFolder[0], CFile::modeRead);
	dwTotalSize = ReadFile1.GetLength();//获取文件总长度

	//开始写文件
	strcommand = L"AT^GT_CM=SOFTSIM_DATA,";

	//数据缓冲区
	char *pBuf = new char[dwStep + 1];
	memset(pBuf, '\0', dwStep + 1);

	ClearCommError(dlg->port1handler, &dwErrorFlags, &ComStat);

	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Downloading, NULL);
	PurgeComm(port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//这个While循环是用来发送文件数据的
	while ((dwRead = ReadFile1.Read(pBuf, dwStep)) > 0)
	{
		//文件分割发送
		strdatano.Format(L"%d", intdatano);
		char *buf1 = new char[2 * (dwRead + 1)];
		memset(buf1, '\0', 2 * (dwRead + 1));
		//转换成16进制
		for (unsigned int j = 0; j < dwRead; j++)
		{
			unsigned char ddd = pBuf[j];
			if ((ddd / 16) < 10)
				buf1[2 * j] = (ddd / 16) + 48;
			else
				buf1[2 * j] = (ddd / 16) - 10 + 65;   //10=A  
			if ((ddd % 16) < 10)
				buf1[2 * j + 1] = (ddd % 16) + 48;
			else
				buf1[2 * j + 1] = (ddd % 16) - 10 + 65; //9=A  
		}
		strdata = buf1;
		strdatalenth.Format(L"%d", dwRead);
		strfilewritecommand = strcommand + strdatano + L"," + strdatalenth + L"," + strdata + L"\r\n";
		dlg->SetRicheditText(L"发:" + strfilewritecommand, 0);
		dwBytesWrite = strcommand.GetLength();
		bWriteStat = WriteFile(dlg->port1handler, CT2A(strfilewritecommand), strfilewritecommand.GetLength(), &dwBytesWrite, NULL);

		intdatano++;
		memset(pBuf, '\0', dwStep + 1);
		delete[] buf1;
		buf1 = NULL;
		Sleep(400);
	}
	delete[] pBuf;
	pBuf = NULL;
	ReadFile1.Close();
	if (s_bExit == TRUE || m_Port1DownloadControl == FALSE)
	{
		dlg->DownloadClosePort1Thread();
		dlg->DownloadRestPort1Thread();
		return;
	}
	PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//下面开始发送结束语句
	strcommand = L"AT^GT_CM=SOFTSIM_DATA,END," + dlg->strport1crc16 + L"\r\n";
	ClearCommError(dlg->port1handler, &dwErrorFlags, &ComStat);

	//同时开启读线程
	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port1_Read3, NULL);
	m_Port1DownloadWrite3 = TRUE;

	int countend = 0;
	do
	{
		dlg->SetRicheditText(L"发:" + strcommand, 0);
		if (countend == 3)
		{
			::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Fail, NULL);
			m_Port1DownloadWrite3 = FALSE;
			m_Port1DownloadRead3 = FALSE;

			return;
		}
		bWriteStat = WriteFile(dlg->port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(1000);
		countend++;
		if (s_bExit == TRUE || m_Port1DownloadControl == FALSE)
		{
			dlg->DownloadClosePort1Thread();
			dlg->DownloadRestPort1Thread();
			return;
		}
	} while (m_Port1DownloadWrite3);

	//dlg->DownloadRestPort1Thread();
	return;
}

//串口1的读结束命令的读线程
void CMFCP3SIMPORTDlg::DownloadRead3Port1Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	char str[100];
	memset(str, 0, sizeof(str));
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;

	m_Port1DownloadRead3 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程

	do
	{
		Sleep(1020);
		bReadStat = ReadFile(dlg->port1handler, str, 100, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			if (strread.Find(L"SoftSim,") >= 0)
			{
				if (strread.Find(L"OK") >= 0)
				{
					//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port1_OkInsert, NULL);
				}
				else if (strread.Find(L"Error") >= 0)
				{
					::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Fail, NULL);
					m_Port1DownloadWrite3 = FALSE;
					m_Port1DownloadRead3 = FALSE;
					dlg->SetRicheditText(L"收:" + strread, 0);
					dlg->DownloadRestPort1Thread();
					return;
					//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port1_ErrorInsert, NULL);
				}
				m_Port1DownloadRead3 = FALSE;
				m_Port1DownloadWrite3 = FALSE;
			}
		}
		if (s_bExit == TRUE || m_Port1DownloadControl == FALSE)
		{
			dlg->DownloadClosePort1Thread();
			dlg->DownloadRestPort1Thread();
			return;
		}
	} while (m_Port1DownloadRead3);
	dlg->SetRicheditText(L"收:" + strread, 0);
	PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	//dlg->DownloadRestPort1Thread();
	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port1_Write4, NULL);
}

//串口1的写老化命令的写线程
void CMFCP3SIMPORTDlg::DownloadWrite4Port1Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;

	//放指令用变量
	CString strcommand;

	strcommand = L"AT^GT_CM=aging,on\r\n";
	ClearCommError(dlg->port1handler, &dwErrorFlags, &ComStat);

	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Ageing, NULL);

	//同时开启读线程
	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port1_Read4, NULL);
	m_Port1DownloadWrite4 = TRUE;

	int countend = 0;
	do
	{
		dlg->SetRicheditText(L"发:" + strcommand, 0);
		if (countend == 3)
		{
			::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Fail, NULL);
			m_Port1DownloadWrite4 = FALSE;
			m_Port1DownloadRead4 = FALSE;

			dlg->DownloadRestPort1Thread();
			return;
		}
		bWriteStat = WriteFile(dlg->port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(1000);
		countend++;
		if (s_bExit == TRUE || m_Port1DownloadControl == FALSE)
		{
			dlg->DownloadClosePort1Thread();
			dlg->DownloadRestPort1Thread();
			return;
		}
	} while (m_Port1DownloadWrite4);

	dlg->DownloadRestPort1Thread();

}

//串口1的读老化命令的读线程
void CMFCP3SIMPORTDlg::DownloadRead4Port1Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	char str[100];
	memset(str, 0, sizeof(str));
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread,strfolderpath, strfoldercut;;

	m_Port1DownloadRead4 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程

	do
	{
		Sleep(1020);
		bReadStat = ReadFile(dlg->port1handler, str, 100, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			if (strread.Find(L"aging,on\r\r\nOK!") >= 0)
			{
				::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Success, NULL);
				strfolderpath = StrFolder[0];//路径先复制到一个临时变量
				strfolderpath = strfolderpath.Left(strfolderpath.GetLength() - 10);//将末尾的证书文件路径切掉
				strfoldercut = strfolderpath.Right(13);//然后将种子文件夹名称切割出来
				MoveFile(strfolderpath, strOKFolderpath + strfoldercut);//然后粘到下好的OK文件夹路径后面
				StrFolder[0] = L"";
				dlg->SetPort1EditEmpty();
				//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port1_OkInsert, NULL);
			}
			m_Port1DownloadRead4 = FALSE;
			m_Port1DownloadWrite4 = FALSE;
		}
		if (s_bExit == TRUE || m_Port1DownloadControl == FALSE)
		{
			dlg->DownloadClosePort1Thread();
			dlg->DownloadRestPort1Thread();
			return;
		}
	} while (m_Port1DownloadRead4);
	dlg->SetRicheditText(L"收:" + strread, 0);
	PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	//dlg->DownloadRestPort1Thread();
	return;
}

//串口1重置线程全局变量
void CMFCP3SIMPORTDlg::DownloadRestPort1Thread()
{
	m_TreadPort1DownloadWrite1 = TRUE;
	m_TreadPort1DownloadWrite2 = TRUE;
	m_TreadPort1DownloadWrite3 = TRUE;
	m_TreadPort1DownloadWrite4 = TRUE;
	m_TreadPort1DownloadRead1 = TRUE;
	m_TreadPort1DownloadRead2 = TRUE;
	m_TreadPort1DownloadRead3 = TRUE;
	m_TreadPort1DownloadRead4 = TRUE;
}

//串口1关闭线程全局变量
void CMFCP3SIMPORTDlg::DownloadClosePort1Thread()
{
	m_Port1DownloadWrite1 = FALSE;
	m_Port1DownloadWrite2 = FALSE;
	m_Port1DownloadWrite3 = FALSE;
	m_Port1DownloadWrite4 = FALSE;
	m_Port1DownloadRead1 = FALSE;
	m_Port1DownloadRead2 = FALSE;
	m_Port1DownloadReadEnd2 = FALSE;
	m_Port1DownloadRead3 = FALSE;
	m_Port1DownloadRead4 = FALSE;
}


//串口2下载逻辑
//串口2的test写线程
void CMFCP3SIMPORTDlg::DownloadWrite1Port2Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	PurgeComm(port2handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;

	//放指令用变量
	CString strcommand = L"AT^GT_CM=TEST\r\n";

	//一直test，看看串口有没有连接上

	m_Port2DownloadWrite1 = TRUE;

	ClearCommError(dlg->port2handler, &dwErrorFlags, &ComStat);
	Sleep(50);
	do
	{
		dlg->SetRicheditText(L"发:" + strcommand, 0);
		bWriteStat = WriteFile(dlg->port2handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(1500);

		if (s_bExit == TRUE || m_Port2DownloadControl == FALSE)
		{
			dlg->DownloadClosePort2Thread();
			dlg->DownloadRestPort2Thread();
			return;
		}
	} while (m_Port2DownloadWrite1);

	return;
}

//串口2的test读线程
void CMFCP3SIMPORTDlg::DownloadRead1Port2Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	//串口变量
	char str[100];
	memset(str, 0, sizeof(str));
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;

	m_Port2DownloadRead1 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程
	do
	{
		Sleep(1520);
		bReadStat = ReadFile(dlg->port2handler, str, 100, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			if (strread.Find(L"TEST_OK") >= 0)
			{
				m_Port2DownloadRead1 = FALSE;
				m_Port2DownloadWrite1 = FALSE;
			}
		}
		if (s_bExit == TRUE || m_Port2DownloadControl == FALSE)
		{
			dlg->DownloadClosePort2Thread();
			dlg->DownloadRestPort2Thread();
			return;
		}
	} while (m_Port2DownloadRead1);
	dlg->SetRicheditText(L"收:" + strread, 0);
	PurgeComm(port2handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port2_Write2, NULL);
}

//串口2的写RID、IMEI的写线程
void CMFCP3SIMPORTDlg::DownloadWrite2Port2Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint2_Connected, NULL);
	PurgeComm(port2handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;

	//放指令用变量
	CString strcommand;
	CString strfilewritecommand;

	//连接上了就开始读它的RID和IMEI，如果连续发送五条命令都没反应，那就返回Test那里重新检测设备
	for (int i = 0; i < 2; i++)
	{
		int count = 0;
		if (i == 0)
		{
			strcommand = L"AT^GT_CM=ID,1\r\n";//读RID的一个命令
		}
		else if (i == 1)
		{
			strcommand = L"AT^GT_CM=IMEI\r\n";//读IMEI的一个命令
		}

		ClearCommError(dlg->port2handler, &dwErrorFlags, &ComStat);

		//然后同时开启读线程
		m_Port2DownloadWrite2 = TRUE;
		::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port2_Read2, NULL);
		do
		{
			dlg->SetRicheditText(L"发:" + strcommand, 0);
			bWriteStat = WriteFile(dlg->port2handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);

			if (count == 7)
			{
				::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint2_Fail, NULL);
				dlg->DownloadClosePort2Thread();
				dlg->DownloadRestPort2Thread();
				return;
			}
			count++;
			Sleep(600);
			if (s_bExit == TRUE || m_Port2DownloadControl == FALSE)
			{
				dlg->DownloadClosePort2Thread();
				dlg->DownloadRestPort2Thread();
				return;
			}
		} while (m_Port2DownloadWrite2);
	}
}

//串口2的RID、IMEI读线程
void CMFCP3SIMPORTDlg::DownloadRead2Port2Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	//串口变量
	char str[100];
	memset(str, 0, sizeof(str));
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;
	CString strcount, strcounttemp;//放RID和IMEI用
	CString strtemp;
	int findcount1, findcount2;//切割读出来的字符串用

	for (int i = 0; i < 2; i++)
	{
		m_Port2DownloadRead2 = TRUE;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程
		m_Port2DownloadReadEnd2 = TRUE;
		int count = 0;
		if (i == 0)
		{
			strtemp = L"Chip RID:";//读RID的一个命令
		}
		else if (i == 1)
		{
			strtemp = L"IMEI:";//读IMEI的一个命令
		}
		do
		{
			Sleep(620);
			bReadStat = ReadFile(dlg->port2handler, str, 100, &readreal, 0);
			if (bReadStat)
			{
				strread = str;
				if ((findcount1 = strread.Find(strtemp)) >= 0)
				{
					//读RID
					if (i == 0)
					{
						strcount = strread.Right(strread.GetLength() - findcount1 - 9);
						findcount2 = strcount.Find(L"\r\n");
						strcount = strcount.Left(findcount2);
						if (strcount != "")
						{
							dlg->SetDlgItemText(IDC_PORT2RID_EDIT, strcount);
						}
						else if (strcount == "")
						{
							continue;
						}
					}
					//读IMEI
					else if (i == 1)
					{
						strcount = strread.Right(strread.GetLength() - findcount1 - 5);
						findcount2 = strcount.Find(L"\r\n");
						strcount = strcount.Left(findcount2);
						strcounttemp = strcount;
						if (strcount != ""&&strcounttemp.Trim(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789").GetLength() == 0)
						{
							dlg->SetDlgItemText(IDC_PORT2IMEI_EDIT, strcount);
							//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port2_IsExit, NULL);
							Sleep(50);
						}
						else 
						{
							continue;
						}
					}
					m_Port2DownloadWrite2 = FALSE;
					m_Port2DownloadRead2 = FALSE;
				}
			}
		} while (m_Port2DownloadRead2);
		if (m_Port2DownloadReadEnd2 == FALSE)
		{
			return;
		}
		if (s_bExit == TRUE || m_Port2DownloadControl == FALSE)
		{
			dlg->DownloadClosePort2Thread();
			dlg->DownloadRestPort2Thread();
			return;
		}
		dlg->SetRicheditText(L"收:" + strread, 0);
	}
	PurgeComm(dlg->port2handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port2_Write3, NULL);
	return;
}

//串口2的写文件和写结束命令的写线程
void CMFCP3SIMPORTDlg::DownloadWrite3Port2Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	DWORD dwTotalSize;//文件总大小
	DWORD dwStep = 490;//一次读490个字符
	DWORD dwRead;//实际读的字符数

	//放指令用变量
	CString strcommand;
	CString strfilewritecommand;

	//文件下载变量
	CString strdatano, strdatalenth, strdata;//分别为第几条数据,数据长度，数据大小
	int intdatano = 0;//第几条数据

	//文件下载地址
	CStdioFile ReadFile2;
	BOOL bOpen = ReadFile2.Open(StrFolder[1], CFile::modeRead);
	dwTotalSize = ReadFile2.GetLength();//获取文件总长度

	//开始写文件
	strcommand = L"AT^GT_CM=SOFTSIM_DATA,";

	//数据缓冲区
	char *pBuf = new char[dwStep + 1];
	memset(pBuf, '\0', dwStep + 1);

	ClearCommError(dlg->port2handler, &dwErrorFlags, &ComStat);

	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint2_Downloading, NULL);
	PurgeComm(port2handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//这个While循环是用来发送文件数据的
	while ((dwRead = ReadFile2.Read(pBuf, dwStep)) > 0)
	{
		//文件分割发送
		strdatano.Format(L"%d", intdatano);
		char *buf1 = new char[2 * (dwRead + 1)];
		memset(buf1, '\0', 2 * (dwRead + 1));
		//转换成16进制
		for (unsigned int j = 0; j < dwRead; j++)
		{
			unsigned char ddd = pBuf[j];
			if ((ddd / 16) < 10)
				buf1[2 * j] = (ddd / 16) + 48;
			else
				buf1[2 * j] = (ddd / 16) - 10 + 65;   //10=A  
			if ((ddd % 16) < 10)
				buf1[2 * j + 1] = (ddd % 16) + 48;
			else
				buf1[2 * j + 1] = (ddd % 16) - 10 + 65; //9=A  
		}
		strdata = buf1;
		strdatalenth.Format(L"%d", dwRead);
		strfilewritecommand = strcommand + strdatano + L"," + strdatalenth + L"," + strdata + L"\r\n";
		dlg->SetRicheditText(L"发:" + strfilewritecommand, 0);
		dwBytesWrite = strcommand.GetLength();
		bWriteStat = WriteFile(dlg->port2handler, CT2A(strfilewritecommand), strfilewritecommand.GetLength(), &dwBytesWrite, NULL);

		intdatano++;
		memset(pBuf, '\0', dwStep + 1);
		delete[] buf1;
		buf1 = NULL;
		Sleep(400);
	}
	delete[] pBuf;
	pBuf = NULL;
	ReadFile2.Close();
	if (s_bExit == TRUE || m_Port2DownloadControl == FALSE)
	{
		dlg->DownloadClosePort2Thread();
		dlg->DownloadRestPort2Thread();
		return;
	}
	PurgeComm(dlg->port2handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//下面开始发送结束语句
	strcommand = L"AT^GT_CM=SOFTSIM_DATA,END," + dlg->strport2crc16 + L"\r\n";
	ClearCommError(dlg->port2handler, &dwErrorFlags, &ComStat);

	//同时开启读线程
	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port2_Read3, NULL);
	m_Port2DownloadWrite3 = TRUE;

	int countend = 0;
	do
	{
		dlg->SetRicheditText(L"发:" + strcommand, 0);
		if (countend == 3)
		{
			::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint2_Fail, NULL);
			m_Port2DownloadWrite3 = FALSE;
			m_Port2DownloadRead3 = FALSE;
			return;
		}
		bWriteStat = WriteFile(dlg->port2handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(1000);
		countend++;
		if (s_bExit == TRUE || m_Port2DownloadControl == FALSE)
		{
			dlg->DownloadClosePort2Thread();
			dlg->DownloadRestPort2Thread();
			return;
		}
	} while (m_Port2DownloadWrite3);

	//dlg->DownloadRestPort2Thread();
	return;
}

//串口2的读结束命令的读线程
void CMFCP3SIMPORTDlg::DownloadRead3Port2Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	char str[100];
	memset(str, 0, sizeof(str));
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread, strfolderpath, strfoldercut;

	m_Port2DownloadRead3 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程

	do
	{
		Sleep(1020);
		bReadStat = ReadFile(dlg->port2handler, str, 100, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			if (strread.Find(L"SoftSim,") >= 0)
			{
				if (strread.Find(L"OK") >= 0)
				{
					//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port2_OkInsert, NULL);
				}
				else if (strread.Find(L"Error") >= 0)
				{
					::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint2_Fail, NULL);
					m_Port2DownloadWrite3 = FALSE;
					m_Port2DownloadRead3 = FALSE;
					dlg->SetRicheditText(L"收:" + strread, 0);
					dlg->DownloadRestPort2Thread();
					return;
					//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port2_ErrorInsert, NULL);
				}
				m_Port2DownloadRead3 = FALSE;
				m_Port2DownloadWrite3 = FALSE;
			}
		}
		if (s_bExit == TRUE || m_Port2DownloadControl == FALSE)
		{
			dlg->DownloadClosePort2Thread();
			dlg->DownloadRestPort2Thread();
			return;
		}
	} while (m_Port2DownloadRead3);
	dlg->SetRicheditText(L"收:" + strread, 0);
	PurgeComm(dlg->port2handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	//dlg->DownloadRestPort2Thread();
	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port2_Write4, NULL);
}

//串口2的写老化命令的写线程
void CMFCP3SIMPORTDlg::DownloadWrite4Port2Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;

	//放指令用变量
	CString strcommand;

	strcommand = L"AT^GT_CM=aging,on\r\n";
	ClearCommError(dlg->port2handler, &dwErrorFlags, &ComStat);

	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint2_Ageing, NULL);

	//同时开启读线程
	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port2_Read4, NULL);
	m_Port2DownloadWrite4 = TRUE;

	int countend = 0;
	do
	{
		dlg->SetRicheditText(L"发:" + strcommand, 0);
		if (countend == 3)
		{
			::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint2_Fail, NULL);
			m_Port2DownloadWrite4 = FALSE;
			m_Port2DownloadRead4 = FALSE;

			dlg->DownloadRestPort2Thread();
			return;
		}
		bWriteStat = WriteFile(dlg->port2handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(1000);
		countend++;
		if (s_bExit == TRUE || m_Port2DownloadControl == FALSE)
		{
			dlg->DownloadClosePort2Thread();
			dlg->DownloadRestPort2Thread();
			return;
		}
	} while (m_Port2DownloadWrite4);

	dlg->DownloadRestPort2Thread();

}

//串口2的读老化命令的读线程
void CMFCP3SIMPORTDlg::DownloadRead4Port2Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	char str[100];
	memset(str, 0, sizeof(str));
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread, strfolderpath, strfoldercut;;

	m_Port2DownloadRead4 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程

	do
	{
		Sleep(1020);
		bReadStat = ReadFile(dlg->port2handler, str, 100, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			if (strread.Find(L"aging,on\r\r\nOK!") >= 0)
			{
				::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint2_Success, NULL);
				strfolderpath = StrFolder[1];//路径先复制到一个临时变量
				strfolderpath = strfolderpath.Left(strfolderpath.GetLength() - 10);//将末尾的证书文件路径切掉
				strfoldercut = strfolderpath.Right(13);//然后将种子文件夹名称切割出来
				MoveFile(strfolderpath, strOKFolderpath + strfoldercut);//然后粘到下好的OK文件夹路径后面
				StrFolder[1] = L"";
				dlg->SetPort2EditEmpty();
				//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port2_OkInsert, NULL);
			}
			m_Port2DownloadRead4 = FALSE;
			m_Port2DownloadWrite4 = FALSE;
		}
		if (s_bExit == TRUE || m_Port2DownloadControl == FALSE)
		{
			dlg->DownloadClosePort2Thread();
			dlg->DownloadRestPort2Thread();
			return;
		}
	} while (m_Port2DownloadRead4);
	dlg->SetRicheditText(L"收:" + strread, 0);
	PurgeComm(dlg->port2handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	//dlg->DownloadRestPort2Thread();
	return;
}

//串口2重置线程全局变量
void CMFCP3SIMPORTDlg::DownloadRestPort2Thread()
{
	m_TreadPort2DownloadWrite1 = TRUE;
	m_TreadPort2DownloadWrite2 = TRUE;
	m_TreadPort2DownloadWrite3 = TRUE;
	m_TreadPort2DownloadWrite4 = TRUE;
	m_TreadPort2DownloadRead1 = TRUE;
	m_TreadPort2DownloadRead2 = TRUE;
	m_TreadPort2DownloadRead3 = TRUE;
	m_TreadPort2DownloadRead4 = TRUE;
}

//串口2关闭线程全局变量
void CMFCP3SIMPORTDlg::DownloadClosePort2Thread()
{
	m_Port2DownloadWrite1 = FALSE;
	m_Port2DownloadWrite2 = FALSE;
	m_Port2DownloadWrite3 = FALSE;
	m_Port2DownloadWrite4 = FALSE;
	m_Port2DownloadRead1 = FALSE;
	m_Port2DownloadRead2 = FALSE;
	m_Port2DownloadReadEnd2 = FALSE;
	m_Port2DownloadRead3 = FALSE;
	m_Port2DownloadRead4 = FALSE;
}


//串口3下载逻辑
//串口3的test写线程
void CMFCP3SIMPORTDlg::DownloadWrite1Port3Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	PurgeComm(port3handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;

	//放指令用变量
	CString strcommand = L"AT^GT_CM=TEST\r\n";

	//一直test，看看串口有没有连接上

	m_Port3DownloadWrite1 = TRUE;

	ClearCommError(dlg->port3handler, &dwErrorFlags, &ComStat);
	Sleep(50);
	do
	{
		dlg->SetRicheditText(L"发:" + strcommand, 0);
		bWriteStat = WriteFile(dlg->port3handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(1500);

		if (s_bExit == TRUE || m_Port3DownloadControl == FALSE)
		{
			dlg->DownloadClosePort3Thread();
			dlg->DownloadRestPort3Thread();
			return;
		}
	} while (m_Port3DownloadWrite1);

	return;
}

//串口3的test读线程
void CMFCP3SIMPORTDlg::DownloadRead1Port3Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	//串口变量
	char str[100];
	memset(str, 0, sizeof(str));
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;

	m_Port3DownloadRead1 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程
	do
	{
		Sleep(1520);
		bReadStat = ReadFile(dlg->port3handler, str, 100, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			if (strread.Find(L"TEST_OK") >= 0)
			{
				m_Port3DownloadRead1 = FALSE;
				m_Port3DownloadWrite1 = FALSE;
			}
		}
		if (s_bExit == TRUE || m_Port3DownloadControl == FALSE)
		{
			dlg->DownloadClosePort3Thread();
			dlg->DownloadRestPort3Thread();
			return;
		}
	} while (m_Port3DownloadRead1);
	dlg->SetRicheditText(L"收:" + strread, 0);
	PurgeComm(port3handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port3_Write2, NULL);
}

//串口3的写RID、IMEI的写线程
void CMFCP3SIMPORTDlg::DownloadWrite2Port3Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint3_Connected, NULL);
	PurgeComm(port3handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;

	//放指令用变量
	CString strcommand;
	CString strfilewritecommand;

	//连接上了就开始读它的RID和IMEI，如果连续发送五条命令都没反应，那就返回Test那里重新检测设备
	for (int i = 0; i < 2; i++)
	{
		int count = 0;
		if (i == 0)
		{
			strcommand = L"AT^GT_CM=ID,1\r\n";//读RID的一个命令
		}
		else if (i == 1)
		{
			strcommand = L"AT^GT_CM=IMEI\r\n";//读IMEI的一个命令
		}

		ClearCommError(dlg->port3handler, &dwErrorFlags, &ComStat);

		//然后同时开启读线程
		m_Port3DownloadWrite2 = TRUE;
		::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port3_Read2, NULL);
		do
		{
			dlg->SetRicheditText(L"发:" + strcommand, 0);
			bWriteStat = WriteFile(dlg->port3handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);

			if (count == 7)
			{
				::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint3_Fail, NULL);
				dlg->DownloadClosePort3Thread();
				dlg->DownloadRestPort3Thread();
				return;
			}
			count++;
			Sleep(600);
			if (s_bExit == TRUE || m_Port3DownloadControl == FALSE)
			{
				dlg->DownloadClosePort3Thread();
				dlg->DownloadRestPort3Thread();
				return;
			}
		} while (m_Port3DownloadWrite2);
	}
}

//串口3的RID、IMEI读线程
void CMFCP3SIMPORTDlg::DownloadRead2Port3Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	//串口变量
	char str[100];
	memset(str, 0, sizeof(str));
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;
	CString strcount,strcounttemp;//放RID和IMEI用
	CString strtemp;
	int findcount1, findcount2;//切割读出来的字符串用

	for (int i = 0; i < 2; i++)
	{
		m_Port3DownloadRead2 = TRUE;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程
		m_Port3DownloadReadEnd2 = TRUE;
		int count = 0;
		if (i == 0)
		{
			strtemp = L"Chip RID:";//读RID的一个命令
		}
		else if (i == 1)
		{
			strtemp = L"IMEI:";//读IMEI的一个命令
		}
		do
		{
			Sleep(620);
			bReadStat = ReadFile(dlg->port3handler, str, 100, &readreal, 0);
			if (bReadStat)
			{
				strread = str;
				if ((findcount1 = strread.Find(strtemp)) >= 0)
				{
					//读RID
					if (i == 0)
					{
						strcount = strread.Right(strread.GetLength() - findcount1 - 9);
						findcount2 = strcount.Find(L"\r\n");
						strcount = strcount.Left(findcount2);
						if (strcount != "")
						{
							dlg->SetDlgItemText(IDC_PORT3RID_EDIT, strcount);
						}
						else if (strcount == "")
						{
							continue;
						}
					}
					//读IMEI
					else if (i == 1)
					{
						strcount = strread.Right(strread.GetLength() - findcount1 - 5);
						findcount2 = strcount.Find(L"\r\n");
						strcount = strcount.Left(findcount2);
						strcounttemp = strcount;
						if (strcount != ""&&strcounttemp.Trim(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789").GetLength() == 0)
						{
							dlg->SetDlgItemText(IDC_PORT3IMEI_EDIT, strcount);
							//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port3_IsExit, NULL);
							Sleep(50);
						}
						else 
						{
							continue;
						}
					}
					m_Port3DownloadWrite2 = FALSE;
					m_Port3DownloadRead2 = FALSE;
				}
			}
		} while (m_Port3DownloadRead2);
		if (m_Port3DownloadReadEnd2 == FALSE)
		{
			return;
		}
		if (s_bExit == TRUE || m_Port3DownloadControl == FALSE)
		{
			dlg->DownloadClosePort3Thread();
			dlg->DownloadRestPort3Thread();
			return;
		}
		dlg->SetRicheditText(L"收:" + strread, 0);
	}
	PurgeComm(dlg->port3handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port3_Write3, NULL);
	return;
}

//串口3的写文件和写结束命令的写线程
void CMFCP3SIMPORTDlg::DownloadWrite3Port3Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	DWORD dwTotalSize;//文件总大小
	DWORD dwStep = 490;//一次读490个字符
	DWORD dwRead;//实际读的字符数

	//放指令用变量
	CString strcommand;
	CString strfilewritecommand;

	//文件下载变量
	CString strdatano, strdatalenth, strdata;//分别为第几条数据,数据长度，数据大小
	int intdatano = 0;//第几条数据

	//文件下载地址
	CStdioFile ReadFile3;
	BOOL bOpen = ReadFile3.Open(StrFolder[2], CFile::modeRead);
	dwTotalSize = ReadFile3.GetLength();//获取文件总长度

	//开始写文件
	strcommand = L"AT^GT_CM=SOFTSIM_DATA,";

	//数据缓冲区
	char *pBuf = new char[dwStep + 1];
	memset(pBuf, '\0', dwStep + 1);

	ClearCommError(dlg->port3handler, &dwErrorFlags, &ComStat);

	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint3_Downloading, NULL);
	PurgeComm(port3handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//这个While循环是用来发送文件数据的
	while ((dwRead = ReadFile3.Read(pBuf, dwStep)) > 0)
	{
		//文件分割发送
		strdatano.Format(L"%d", intdatano);
		char *buf1 = new char[2 * (dwRead + 1)];
		memset(buf1, '\0', 2 * (dwRead + 1));
		//转换成16进制
		for (unsigned int j = 0; j < dwRead; j++)
		{
			unsigned char ddd = pBuf[j];
			if ((ddd / 16) < 10)
				buf1[2 * j] = (ddd / 16) + 48;
			else
				buf1[2 * j] = (ddd / 16) - 10 + 65;   //10=A  
			if ((ddd % 16) < 10)
				buf1[2 * j + 1] = (ddd % 16) + 48;
			else
				buf1[2 * j + 1] = (ddd % 16) - 10 + 65; //9=A  
		}
		strdata = buf1;
		strdatalenth.Format(L"%d", dwRead);
		strfilewritecommand = strcommand + strdatano + L"," + strdatalenth + L"," + strdata + L"\r\n";
		dlg->SetRicheditText(L"发:" + strfilewritecommand, 0);
		dwBytesWrite = strcommand.GetLength();
		bWriteStat = WriteFile(dlg->port3handler, CT2A(strfilewritecommand), strfilewritecommand.GetLength(), &dwBytesWrite, NULL);

		intdatano++;
		memset(pBuf, '\0', dwStep + 1);
		delete[] buf1;
		buf1 = NULL;
		Sleep(400);
	}
	delete[] pBuf;
	pBuf = NULL;
	ReadFile3.Close();
	if (s_bExit == TRUE || m_Port3DownloadControl == FALSE)
	{
		dlg->DownloadClosePort3Thread();
		dlg->DownloadRestPort3Thread();
		return;
	}
	PurgeComm(dlg->port3handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//下面开始发送结束语句
	strcommand = L"AT^GT_CM=SOFTSIM_DATA,END," + dlg->strport3crc16 + L"\r\n";
	ClearCommError(dlg->port3handler, &dwErrorFlags, &ComStat);

	//同时开启读线程
	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port3_Read3, NULL);
	m_Port3DownloadWrite3 = TRUE;

	int countend = 0;
	do
	{
		dlg->SetRicheditText(L"发:" + strcommand, 0);
		if (countend == 3)
		{
			::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint3_Fail, NULL);
			m_Port3DownloadWrite3 = FALSE;
			m_Port3DownloadRead3 = FALSE;
			return;
		}
		bWriteStat = WriteFile(dlg->port3handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(1000);
		countend++;
		if (s_bExit == TRUE || m_Port3DownloadControl == FALSE)
		{
			dlg->DownloadClosePort3Thread();
			dlg->DownloadRestPort3Thread();
			return;
		}
	} while (m_Port3DownloadWrite3);

	//dlg->DownloadRestPort3Thread();
	return;
}

//串口3的读结束命令的读线程
void CMFCP3SIMPORTDlg::DownloadRead3Port3Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	char str[100];
	memset(str, 0, sizeof(str));
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread, strfolderpath, strfoldercut;

	m_Port3DownloadRead3 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程

	do
	{
		Sleep(1020);
		bReadStat = ReadFile(dlg->port3handler, str, 100, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			if (strread.Find(L"SoftSim,") >= 0)
			{
				if (strread.Find(L"OK") >= 0)
				{

					//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port3_OkInsert, NULL);
				}
				else if (strread.Find(L"Error") >= 0)
				{
					::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint3_Fail, NULL);
					//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port3_ErrorInsert, NULL);
					m_Port3DownloadWrite3 = FALSE;
					m_Port3DownloadRead3 = FALSE;
					dlg->SetRicheditText(L"收:" + strread, 0);
					dlg->DownloadRestPort3Thread();
					return;
				}
				m_Port3DownloadRead3 = FALSE;
				m_Port3DownloadWrite3 = FALSE;
			}
		}
		if (s_bExit == TRUE || m_Port3DownloadControl == FALSE)
		{
			dlg->DownloadClosePort3Thread();
			dlg->DownloadRestPort3Thread();
			return;
		}
	} while (m_Port3DownloadRead3);
	dlg->SetRicheditText(L"收:" + strread, 0);
	PurgeComm(dlg->port3handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	//dlg->DownloadRestPort3Thread();
	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port3_Write4, NULL);
}

//串口3的写老化命令的写线程
void CMFCP3SIMPORTDlg::DownloadWrite4Port3Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;

	//放指令用变量
	CString strcommand;

	strcommand = L"AT^GT_CM=aging,on\r\n";
	ClearCommError(dlg->port3handler, &dwErrorFlags, &ComStat);

	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint3_Ageing, NULL);

	//同时开启读线程
	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port3_Read4, NULL);
	m_Port3DownloadWrite4 = TRUE;

	int countend = 0;
	do
	{
		dlg->SetRicheditText(L"发:" + strcommand, 0);
		if (countend == 3)
		{
			::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint3_Fail, NULL);
			m_Port3DownloadWrite4 = FALSE;
			m_Port3DownloadRead4 = FALSE;

			dlg->DownloadRestPort3Thread();
			return;
		}
		bWriteStat = WriteFile(dlg->port3handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(1000);
		countend++;
		if (s_bExit == TRUE || m_Port3DownloadControl == FALSE)
		{
			dlg->DownloadClosePort3Thread();
			dlg->DownloadRestPort3Thread();
			return;
		}
	} while (m_Port3DownloadWrite4);

	dlg->DownloadRestPort3Thread();
}

//串口3的读老化命令的读线程
void CMFCP3SIMPORTDlg::DownloadRead4Port3Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	char str[100];
	memset(str, 0, sizeof(str));
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread, strfolderpath, strfoldercut;;

	m_Port3DownloadRead4 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程

	do
	{
		Sleep(1020);
		bReadStat = ReadFile(dlg->port3handler, str, 100, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			if (strread.Find(L"aging,on\r\r\nOK!") >= 0)
			{
				::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint3_Success, NULL);
				strfolderpath = StrFolder[2];//路径先复制到一个临时变量
				strfolderpath = strfolderpath.Left(strfolderpath.GetLength() - 10);//将末尾的证书文件路径切掉
				strfoldercut = strfolderpath.Right(13);//然后将种子文件夹名称切割出来
				MoveFile(strfolderpath, strOKFolderpath + strfoldercut);//然后粘到下好的OK文件夹路径后面
				StrFolder[2] = L"";
				dlg->SetPort3EditEmpty();
				//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port3_OkInsert, NULL);
			}
			m_Port3DownloadRead4 = FALSE;
			m_Port3DownloadWrite4 = FALSE;
		}
		if (s_bExit == TRUE || m_Port3DownloadControl == FALSE)
		{
			dlg->DownloadClosePort3Thread();
			dlg->DownloadRestPort3Thread();
			return;
		}
	} while (m_Port3DownloadRead4);
	dlg->SetRicheditText(L"收:" + strread, 0);
	PurgeComm(dlg->port3handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	//dlg->DownloadRestPort3Thread();
	return;
}

//串口3重置线程全局变量
void CMFCP3SIMPORTDlg::DownloadRestPort3Thread()
{
	m_TreadPort3DownloadWrite1 = TRUE;
	m_TreadPort3DownloadWrite2 = TRUE;
	m_TreadPort3DownloadWrite3 = TRUE;
	m_TreadPort3DownloadWrite4 = TRUE;
	m_TreadPort3DownloadRead1 = TRUE;
	m_TreadPort3DownloadRead2 = TRUE;
	m_TreadPort3DownloadRead3 = TRUE;
	m_TreadPort3DownloadRead4 = TRUE;
}

//串口3关闭线程全局变量
void CMFCP3SIMPORTDlg::DownloadClosePort3Thread()
{
	m_Port3DownloadWrite1 = FALSE;
	m_Port3DownloadWrite2 = FALSE;
	m_Port3DownloadWrite3 = FALSE;
	m_Port3DownloadWrite4 = FALSE;
	m_Port3DownloadRead1 = FALSE;
	m_Port3DownloadRead2 = FALSE;
	m_Port3DownloadReadEnd2 = FALSE;
	m_Port3DownloadRead3 = FALSE;
	m_Port3DownloadRead4 = FALSE;
}


//串口4下载逻辑
//串口4的test写线程
void CMFCP3SIMPORTDlg::DownloadWrite1Port4Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	PurgeComm(port4handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;

	//放指令用变量
	CString strcommand = L"AT^GT_CM=TEST\r\n";

	//一直test，看看串口有没有连接上

	m_Port4DownloadWrite1 = TRUE;

	ClearCommError(dlg->port4handler, &dwErrorFlags, &ComStat);
	Sleep(50);
	do
	{
		dlg->SetRicheditText(L"发:" + strcommand, 0);
		bWriteStat = WriteFile(dlg->port4handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(1500);

		if (s_bExit == TRUE || m_Port4DownloadControl == FALSE)
		{
			dlg->DownloadClosePort4Thread();
			dlg->DownloadRestPort4Thread();
			return;
		}
	} while (m_Port4DownloadWrite1);

	return;
}

//串口4的test读线程
void CMFCP3SIMPORTDlg::DownloadRead1Port4Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	//串口变量
	char str[100];
	memset(str, 0, sizeof(str));
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;

	m_Port4DownloadRead1 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程
	do
	{
		Sleep(1520);
		bReadStat = ReadFile(dlg->port4handler, str, 100, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			if (strread.Find(L"TEST_OK") >= 0)
			{
				m_Port4DownloadRead1 = FALSE;
				m_Port4DownloadWrite1 = FALSE;
			}
		}
		if (s_bExit == TRUE || m_Port4DownloadControl == FALSE)
		{
			dlg->DownloadClosePort4Thread();
			dlg->DownloadRestPort4Thread();
			return;
		}
	} while (m_Port4DownloadRead1);
	dlg->SetRicheditText(L"收:" + strread, 0);
	PurgeComm(port4handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port4_Write2, NULL);
}

//串口4的写RID、IMEI的写线程
void CMFCP3SIMPORTDlg::DownloadWrite2Port4Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint4_Connected, NULL);
	PurgeComm(port4handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;

	//放指令用变量
	CString strcommand;
	CString strfilewritecommand;

	//连接上了就开始读它的RID和IMEI，如果连续发送五条命令都没反应，那就返回Test那里重新检测设备
	for (int i = 0; i < 2; i++)
	{
		int count = 0;
		if (i == 0)
		{
			strcommand = L"AT^GT_CM=ID,1\r\n";//读RID的一个命令
		}
		else if (i == 1)
		{
			strcommand = L"AT^GT_CM=IMEI\r\n";//读IMEI的一个命令
		}

		ClearCommError(dlg->port4handler, &dwErrorFlags, &ComStat);

		//然后同时开启读线程
		m_Port4DownloadWrite2 = TRUE;
		::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port4_Read2, NULL);
		do
		{
			dlg->SetRicheditText(L"发:" + strcommand, 0);
			bWriteStat = WriteFile(dlg->port4handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);

			if (count == 7)
			{
				::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint4_Fail, NULL);
				dlg->DownloadClosePort4Thread();
				dlg->DownloadRestPort4Thread();
				return;
			}
			count++;
			Sleep(600);
			if (s_bExit == TRUE || m_Port4DownloadControl == FALSE)
			{
				dlg->DownloadClosePort4Thread();
				dlg->DownloadRestPort4Thread();
				return;
			}
		} while (m_Port4DownloadWrite2);
	}
}

//串口4的RID、IMEI读线程
void CMFCP3SIMPORTDlg::DownloadRead2Port4Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	//串口变量
	char str[100];
	memset(str, 0, sizeof(str));
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;
	CString strcount,strcounttemp;//放RID和IMEI用
	CString strtemp;
	int findcount1, findcount2;//切割读出来的字符串用

	for (int i = 0; i < 2; i++)
	{
		m_Port4DownloadRead2 = TRUE;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程
		m_Port4DownloadReadEnd2 = TRUE;
		int count = 0;
		if (i == 0)
		{
			strtemp = L"Chip RID:";//读RID的一个命令
		}
		else if (i == 1)
		{
			strtemp = L"IMEI:";//读IMEI的一个命令
		}
		do
		{
			Sleep(620);
			bReadStat = ReadFile(dlg->port4handler, str, 100, &readreal, 0);
			if (bReadStat)
			{
				strread = str;
				if ((findcount1 = strread.Find(strtemp)) >= 0)
				{
					//读RID
					if (i == 0)
					{
						strcount = strread.Right(strread.GetLength() - findcount1 - 9);
						findcount2 = strcount.Find(L"\r\n");
						strcount = strcount.Left(findcount2);
						if (strcount != "")
						{
							dlg->SetDlgItemText(IDC_PORT4RID_EDIT, strcount);
						}
						else if (strcount == "")
						{
							continue;
						}
					}
					//读IMEI
					else if (i == 1)
					{
						strcount = strread.Right(strread.GetLength() - findcount1 - 5);
						findcount2 = strcount.Find(L"\r\n");
						strcount = strcount.Left(findcount2);
						if (strcount != ""&&strcounttemp.Trim(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789").GetLength() == 0)
						{
							dlg->SetDlgItemText(IDC_PORT4IMEI_EDIT, strcount);
							//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port4_IsExit, NULL);
							Sleep(50);
						}
						else 
						{
							continue;
						}
					}
					m_Port4DownloadWrite2 = FALSE;
					m_Port4DownloadRead2 = FALSE;
				}
			}
		} while (m_Port4DownloadRead2);
		if (m_Port4DownloadReadEnd2 == FALSE)
		{
			return;
		}
		if (s_bExit == TRUE || m_Port4DownloadControl == FALSE)
		{
			dlg->DownloadClosePort4Thread();
			dlg->DownloadRestPort4Thread();
			return;
		}
		dlg->SetRicheditText(L"收:" + strread, 0);
	}
	PurgeComm(dlg->port4handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port4_Write3, NULL);
	return;
}

//串口4的写文件和写结束命令的写线程
void CMFCP3SIMPORTDlg::DownloadWrite3Port4Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	DWORD dwTotalSize;//文件总大小
	DWORD dwStep = 490;//一次读490个字符
	DWORD dwRead;//实际读的字符数

	//放指令用变量
	CString strcommand;
	CString strfilewritecommand;

	//文件下载变量
	CString strdatano, strdatalenth, strdata;//分别为第几条数据,数据长度，数据大小
	int intdatano = 0;//第几条数据

	//文件下载地址
	CStdioFile ReadFile2;
	BOOL bOpen = ReadFile2.Open(StrFolder[3], CFile::modeRead);
	dwTotalSize = ReadFile2.GetLength();//获取文件总长度

	//开始写文件
	strcommand = L"AT^GT_CM=SOFTSIM_DATA,";

	//数据缓冲区
	char *pBuf = new char[dwStep + 1];
	memset(pBuf, '\0', dwStep + 1);

	ClearCommError(dlg->port4handler, &dwErrorFlags, &ComStat);

	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint4_Downloading, NULL);
	PurgeComm(port4handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//这个While循环是用来发送文件数据的
	while ((dwRead = ReadFile2.Read(pBuf, dwStep)) > 0)
	{
		//文件分割发送
		strdatano.Format(L"%d", intdatano);
		char *buf1 = new char[2 * (dwRead + 1)];
		memset(buf1, '\0', 2 * (dwRead + 1));
		//转换成16进制
		for (unsigned int j = 0; j < dwRead; j++)
		{
			unsigned char ddd = pBuf[j];
			if ((ddd / 16) < 10)
				buf1[2 * j] = (ddd / 16) + 48;
			else
				buf1[2 * j] = (ddd / 16) - 10 + 65;   //10=A  
			if ((ddd % 16) < 10)
				buf1[2 * j + 1] = (ddd % 16) + 48;
			else
				buf1[2 * j + 1] = (ddd % 16) - 10 + 65; //9=A  
		}
		strdata = buf1;
		strdatalenth.Format(L"%d", dwRead);
		strfilewritecommand = strcommand + strdatano + L"," + strdatalenth + L"," + strdata + L"\r\n";
		dlg->SetRicheditText(L"发:" + strfilewritecommand, 0);
		dwBytesWrite = strcommand.GetLength();
		bWriteStat = WriteFile(dlg->port4handler, CT2A(strfilewritecommand), strfilewritecommand.GetLength(), &dwBytesWrite, NULL);

		intdatano++;
		memset(pBuf, '\0', dwStep + 1);
		delete[] buf1;
		buf1 = NULL;
		Sleep(400);
	}
	delete[] pBuf;
	pBuf = NULL;
	ReadFile2.Close();
	if (s_bExit == TRUE || m_Port4DownloadControl == FALSE)
	{
		dlg->DownloadClosePort4Thread();
		dlg->DownloadRestPort4Thread();
		return;
	}
	PurgeComm(dlg->port4handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//下面开始发送结束语句
	strcommand = L"AT^GT_CM=SOFTSIM_DATA,END," + dlg->strport4crc16 + L"\r\n";
	ClearCommError(dlg->port4handler, &dwErrorFlags, &ComStat);

	//同时开启读线程
	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port4_Read3, NULL);
	m_Port4DownloadWrite3 = TRUE;

	int countend = 0;
	do
	{
		dlg->SetRicheditText(L"发:" + strcommand, 0);
		if (countend == 3)
		{
			::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint4_Fail, NULL);
			m_Port4DownloadWrite3 = FALSE;
			m_Port4DownloadRead3 = FALSE;
			return;
		}
		bWriteStat = WriteFile(dlg->port4handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(1000);
		countend++;
		if (s_bExit == TRUE || m_Port4DownloadControl == FALSE)
		{
			dlg->DownloadClosePort4Thread();
			dlg->DownloadRestPort4Thread();
			return;
		}
	} while (m_Port4DownloadWrite3);

	dlg->DownloadRestPort4Thread();
	return;
}

//串口4的读结束命令的读线程
void CMFCP3SIMPORTDlg::DownloadRead3Port4Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	char str[100];
	memset(str, 0, sizeof(str));
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread, strfolderpath, strfoldercut;

	m_Port4DownloadRead3 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程

	do
	{
		Sleep(1020);
		bReadStat = ReadFile(dlg->port4handler, str, 100, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			if (strread.Find(L"SoftSim,") >= 0)
			{
				if (strread.Find(L"OK") >= 0)
				{

					//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port4_OkInsert, NULL);
				}
				else if (strread.Find(L"Error") >= 0)
				{
					::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint4_Fail, NULL);
					m_Port4DownloadWrite3 = FALSE;
					m_Port4DownloadRead3 = FALSE;
					dlg->SetRicheditText(L"收:" + strread, 0);
					dlg->DownloadRestPort4Thread();
					return;
					//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port4_ErrorInsert, NULL);
				}
				m_Port4DownloadRead3 = FALSE;
				m_Port4DownloadWrite3 = FALSE;
			}
		}
		if (s_bExit == TRUE || m_Port4DownloadControl == FALSE)
		{
			dlg->DownloadClosePort4Thread();
			dlg->DownloadRestPort4Thread();
			return;
		}
	} while (m_Port4DownloadRead3);
	dlg->SetRicheditText(L"收:" + strread, 0);
	PurgeComm(dlg->port4handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	//dlg->DownloadRestPort4Thread();
	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port4_Write4, NULL);
}

//串口4的写老化命令的写线程
void CMFCP3SIMPORTDlg::DownloadWrite4Port4Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;

	//放指令用变量
	CString strcommand;

	strcommand = L"AT^GT_CM=aging,on\r\n";
	ClearCommError(dlg->port4handler, &dwErrorFlags, &ComStat);

	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint4_Ageing, NULL);

	//同时开启读线程
	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port4_Read4, NULL);
	m_Port4DownloadWrite4 = TRUE;

	int countend = 0;
	do
	{
		dlg->SetRicheditText(L"发:" + strcommand, 0);
		if (countend == 3)
		{
			::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint4_Fail, NULL);
			m_Port4DownloadWrite4 = FALSE;
			m_Port4DownloadRead4 = FALSE;

			dlg->DownloadRestPort4Thread();
			return;
		}
		bWriteStat = WriteFile(dlg->port4handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(1000);
		countend++;
		if (s_bExit == TRUE || m_Port4DownloadControl == FALSE)
		{
			dlg->DownloadClosePort4Thread();
			dlg->DownloadRestPort4Thread();
			return;
		}
	} while (m_Port4DownloadWrite4);

	//dlg->DownloadRestPort4Thread();
}

//串口4的读老化命令的读线程
void CMFCP3SIMPORTDlg::DownloadRead4Port4Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	char str[100];
	memset(str, 0, sizeof(str));
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread, strfolderpath, strfoldercut;;

	m_Port4DownloadRead4 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程

	do
	{
		Sleep(1020);
		bReadStat = ReadFile(dlg->port4handler, str, 100, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			if (strread.Find(L"aging,on\r\r\nOK!") >= 0)
			{
				::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint4_Success, NULL);
				strfolderpath = StrFolder[3];//路径先复制到一个临时变量
				strfolderpath = strfolderpath.Left(strfolderpath.GetLength() - 10);//将末尾的证书文件路径切掉
				strfoldercut = strfolderpath.Right(13);//然后将种子文件夹名称切割出来
				MoveFile(strfolderpath, strOKFolderpath + strfoldercut);//然后粘到下好的OK文件夹路径后面
				StrFolder[3] = L"";
				dlg->SetPort4EditEmpty();
				//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port4_OkInsert, NULL);
			}
			m_Port4DownloadRead4 = FALSE;
			m_Port4DownloadWrite4 = FALSE;
		}
		if (s_bExit == TRUE || m_Port4DownloadControl == FALSE)
		{
			dlg->DownloadClosePort4Thread();
			dlg->DownloadRestPort4Thread();
			return;
		}
	} while (m_Port4DownloadRead4);
	dlg->SetRicheditText(L"收:" + strread, 0);
	PurgeComm(dlg->port4handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	//dlg->DownloadRestPort4Thread();
	return;
}

//串口4重置线程全局变量
void CMFCP3SIMPORTDlg::DownloadRestPort4Thread()
{
	m_TreadPort4DownloadWrite1 = TRUE;
	m_TreadPort4DownloadWrite2 = TRUE;
	m_TreadPort4DownloadWrite3 = TRUE;
	m_TreadPort4DownloadWrite4 = TRUE;
	m_TreadPort4DownloadRead1 = TRUE;
	m_TreadPort4DownloadRead2 = TRUE;
	m_TreadPort4DownloadRead3 = TRUE;
	m_TreadPort4DownloadRead4 = TRUE;
}

//串口4关闭线程全局变量
void CMFCP3SIMPORTDlg::DownloadClosePort4Thread()
{
	m_Port4DownloadWrite1 = FALSE;
	m_Port4DownloadWrite2 = FALSE;
	m_Port4DownloadWrite3 = FALSE;
	m_Port4DownloadWrite4 = FALSE;
	m_Port4DownloadRead1 = FALSE;
	m_Port4DownloadRead2 = FALSE;
	m_Port4DownloadReadEnd2 = FALSE;
	m_Port4DownloadRead3 = FALSE;
	m_Port4DownloadRead4 = FALSE;
}



//信息日志模块函数
//信息日志核心函数
void CMFCP3SIMPORTDlg::SetRicheditText(CString strMsg, int No)
{
	m_currentinformationRichedit.SetSel(-1, -1);
	CHARFORMAT   cf{ sizeof(cf) };//设置这个结构来改变你想要的文字的颜色等属性
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD | CFM_SIZE | CFM_FACE;//这个目前还没解析完成，但如果要输出不同颜色的字体一定要使用richedit
	cf.crTextColor = RGB(255, 0, 0);//设置为红色

	switch (No)
	{
	case 0:
		m_currentinformationRichedit.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
		m_currentinformationRichedit.LineScroll(m_currentinformationRichedit.GetLineCount() - 1 - 3);
		break;
	case 1:
		m_currentinformationRichedit.SetSelectionCharFormat(cf);
		m_currentinformationRichedit.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
		m_currentinformationRichedit.LineScroll(m_currentinformationRichedit.GetLineCount() - 1);
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
CString CMFCP3SIMPORTDlg::GetTime()
{
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d/%d/%d %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	return strTime;
}


//数据库模块函数
//点击数据库配置按钮
void CMFCP3SIMPORTDlg::OnBnClickedDbconfigButton()
{
	// TODO:  在此添加控件通知处理程序代码
	INT_PTR nRes;             // 用于保存DoModal函数的返回值   

	CDBconfig dbconfigDlg;           // 构造对话框类CTipDlg的实例   
	nRes = dbconfigDlg.DoModal();  // 弹出对话框

	if (IDCANCEL == nRes)
		return;
}

//数据库消息循环函数
afx_msg LRESULT CMFCP3SIMPORTDlg::MainDataInsertControl(WPARAM wParam, LPARAM lParam)
{
	CString strSingleFile, strSingleFileTempPath, strSingleFilecut;
	int singleflag, port1flag;

	switch (wParam)
	{
	case DataInsert_SinglePort_SimDataNoIsExit:
		GetDlgItemText(IDC_SIMDATAFILEPATH_EDIT, strSingleFile);
		strSingleFile = strSingleFile.Right(23);
		singleflag = SimDataSingleNoIsExitFun(strSingleFile.Left(12));
		if (singleflag == 0)
		{
			OnBnClickedStartdownload1Button();
			INT_PTR nRes;
			nRes = MessageBox(_T("SIM数据文件重复，点击确定则选择其它文件，点击取消则停止下载！"), _T("提示消息"), MB_OKCANCEL | MB_ICONQUESTION);
			// 判断消息对话框返回值。如果为IDCANCEL就return，否则继续向下执行   
			if (IDOK == nRes)
			{
				OnBnClickedOpensimdatafilepathButton();
				OnBnClickedStartdownload1Button();
			}
			else if (IDCANCEL == nRes)
			{
				return 0;
			}
		}
		break;
	case DataInsert_Port1_LastStation:
		SimDataLastStationFun1();
		break;
	case DataInsert_Port1_IsExit:
		port1flag = SimDataIsExitFun1();
		if (port1flag == 0)
		{
			m_Port1SINGLEDownloadWrite2 = FALSE;
			m_Port1SINGLEDownloadReadEnd2 = FALSE;
			SetDlgItemText(IDC_PORT1HINT_STATIC,L"工位已测");
			SingleDownloadClosePort1Thread();
			SingleDownloadRestPort1Thread();
		}
		break;
	case DataInsert_Port1_OkInsert:
		strSingleFileTempPath = strSingleFilePath.Left(strSingleFilePath.GetLength() - 10);//将末尾的证书文件路径切掉
		strSingleFilecut = strSingleFileTempPath.Right(13);//然后将种子文件夹名称切割出来
		MoveFile(strSingleFileTempPath, strOKFolderpath + strSingleFilecut);//然后粘到下好的OK文件夹路径后面
		SimDataOkInsertFun1();
		SetPort1EditEmpty();
		OnBnClickedStartdownload1Button();
		INT_PTR nRes;
		nRes = MessageBox(_T("是否继续下载？"), _T("提示消息"), MB_OKCANCEL | MB_ICONQUESTION);
		// 判断消息对话框返回值。如果为IDCANCEL就return，否则继续向下执行   
		if (IDOK == nRes)
		{
			OnBnClickedOpensimdatafilepathButton();
			OnBnClickedStartdownload1Button();
		}
		break;
	case DataInsert_Port1_ErrorInsert:
		SimDataErrorInsertFun1();
		break;
	default:
		break;
	}
	return 0;
}

//单文件下载要独立出来用的判断种子文件是否存在
int CMFCP3SIMPORTDlg::SimDataSingleNoIsExitFun(CString strfile)
{
	int flag;
	ADOManage adosinglesimdatamanage;
	adosinglesimdatamanage.ConnSQL();

	flag = adosinglesimdatamanage.SimDataNoIsExitSql(strfile);

	adosinglesimdatamanage.CloseAll();
	return flag;
}

//批量下载判断种子文件是否存在
int CMFCP3SIMPORTDlg::SimDataNoIsExitFun(CString strfile)
{
	ADOManage adosimdatamanage;
	adosimdatamanage.ConnSQL();

	adosimdatamanage.SimDataNoIsExitSql(strfile);

	adosimdatamanage.CloseAll();
	return 0;
}

//串口1的数据库函数
//判断串口1的机器上个工位
int CMFCP3SIMPORTDlg::SimDataLastStationFun1()
{
	int flag;
	CString strport1IMEI;
	ADOManage adoport1manage3;
	adoport1manage3.ConnSQL();

	GetDlgItemText(IDC_PORT1IMEI_EDIT, strport1IMEI);

	flag = adoport1manage3.SimDataLastStationSql(strport1IMEI);

	adoport1manage3.CloseAll();
	return flag;
}

//判断机器是否烧成功过
int CMFCP3SIMPORTDlg::SimDataIsExitFun1()
{
	int flag;
	CString strport1RID,strport1IMEI;
	ADOManage adoport1manage3;
	adoport1manage3.ConnSQL();

	GetDlgItemText(IDC_PORT1RID_EDIT, strport1RID);
	GetDlgItemText(IDC_PORT1IMEI_EDIT, strport1IMEI);

	flag = adoport1manage3.SimDataIsExitSql(strport1RID,strport1IMEI);

	adoport1manage3.CloseAll();
	return flag;
}

//SIMOK时插入数据
void CMFCP3SIMPORTDlg::SimDataOkInsertFun1()
{
	CString strport1RID, strport1CID, strport1IMEI, strport1ICCID;
	ADOManage adoport1manage1;
	adoport1manage1.ConnSQL();

	GetDlgItemText(IDC_PORT1CID_EDIT, strport1CID);
	GetDlgItemText(IDC_PORT1RID_EDIT, strport1RID);
	GetDlgItemText(IDC_PORT1IMEI_EDIT, strport1IMEI);
	GetDlgItemText(IDC_PORT1ICCID_EDIT, strport1ICCID);

	adoport1manage1.SimDataOkInsertSql(m_pcipEdit,strport1RID,strport1IMEI,strport1CID,strport1ICCID,L"NULL");

	adoport1manage1.CloseAll();
}

//SIMERROE插入数据
void CMFCP3SIMPORTDlg::SimDataErrorInsertFun1()
{
	CString strport1RID, strport1IMEI;
	ADOManage adoport1manage2;
	adoport1manage2.ConnSQL();

	GetDlgItemText(IDC_PORT1RID_EDIT, strport1RID);
	GetDlgItemText(IDC_PORT1IMEI_EDIT, strport1IMEI);

	adoport1manage2.SimDataErrorInsertSql(m_pcipEdit, strport1RID, strport1IMEI, L"NULL");

	adoport1manage2.CloseAll();
}


//其它函数
//获取本机IP和地址
int CMFCP3SIMPORTDlg::GetLocalHostIPName(CString &sLocalName, CString &sIpAddress)
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

//字体初始化函数
void CMFCP3SIMPORTDlg::fontinit()
{
	staticHint1font.CreatePointFont(800, L"黑体");
	staticHint2font.CreatePointFont(800, L"黑体");
	staticHint3font.CreatePointFont(800, L"黑体");
	staticHint4font.CreatePointFont(800, L"黑体");

	GetDlgItem(IDC_PORT1HINT_STATIC)->SetFont(&staticHint1font);
	GetDlgItem(IDC_PORT2HINT_STATIC)->SetFont(&staticHint2font);
	GetDlgItem(IDC_PORT3HINT_STATIC)->SetFont(&staticHint3font);
	GetDlgItem(IDC_PORT4HINT_STATIC)->SetFont(&staticHint4font);

}

//颜色改变函数
HBRUSH CMFCP3SIMPORTDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	CString str1,str2,str3,str4;
	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_PORT1HINT_STATIC)
	{
		GetDlgItemText(IDC_PORT1HINT_STATIC, str1);
		if (str1 == "失败" || str1 == "工位已测")
		{
			pDC->SetTextColor(RGB(255, 0, 0));//用RGB宏改变颜色 
			pDC->SelectObject(&staticHint1font);
		}
		else if (str1 == "成功")
		{
			pDC->SetTextColor(RGB(0, 255, 0));//用RGB宏改变颜色 
			pDC->SelectObject(&staticHint1font);
		}
		else
		{
			pDC->SetTextColor(RGB(0, 0, 0));//用RGB宏改变颜色 
			pDC->SelectObject(&staticHint1font);
		}
	}
	else if (pWnd->GetDlgCtrlID() == IDC_PORT2HINT_STATIC)
	{
		GetDlgItemText(IDC_PORT2HINT_STATIC, str2);
		if (str2 == "失败" || str2 == "工位已测")
		{
			pDC->SetTextColor(RGB(255, 0, 0));//用RGB宏改变颜色 
			pDC->SelectObject(&staticHint2font);
		}
		else if (str2 == "成功")
		{
			pDC->SetTextColor(RGB(0, 255, 0));//用RGB宏改变颜色 
			pDC->SelectObject(&staticHint2font);
		}
		else
		{
			pDC->SetTextColor(RGB(0, 0, 0));//用RGB宏改变颜色 
			pDC->SelectObject(&staticHint2font);
		}
	}
	else if (pWnd->GetDlgCtrlID() == IDC_PORT3HINT_STATIC)
	{ 
		GetDlgItemText(IDC_PORT3HINT_STATIC, str3);
		if (str3 == "失败" || str3 == "工位已测")
		{
			pDC->SetTextColor(RGB(255, 0, 0));//用RGB宏改变颜色 
			pDC->SelectObject(&staticHint3font);
		}
		else if (str3 == "成功")
		{
			pDC->SetTextColor(RGB(0, 255, 0));//用RGB宏改变颜色 
			pDC->SelectObject(&staticHint3font);
		}
		else
		{
			pDC->SetTextColor(RGB(0, 0, 0));//用RGB宏改变颜色 
			pDC->SelectObject(&staticHint3font);
		}
	}
	else if (pWnd->GetDlgCtrlID() == IDC_PORT3HINT_STATIC)
	{
		GetDlgItemText(IDC_PORT4HINT_STATIC, str4);
		if (str4 == "失败" || str4 == "工位已测")
		{
			pDC->SetTextColor(RGB(255, 0, 0));//用RGB宏改变颜色 
			pDC->SelectObject(&staticHint4font);
		}
		else if (str4 == "成功")
		{
			pDC->SetTextColor(RGB(0, 255, 0));//用RGB宏改变颜色 
			pDC->SelectObject(&staticHint4font);
		}
		else
		{
			pDC->SetTextColor(RGB(0, 0, 0));//用RGB宏改变颜色 
			pDC->SelectObject(&staticHint4font);
		}
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

//提示信息更改的消息循环
afx_msg LRESULT CMFCP3SIMPORTDlg::MainFontControl(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	//串口1提示结果
	case Main_Hint1_Ready:
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"就绪");
		break;
	case Main_Hint1_Connected:
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"已连接");
		break;
	case Main_Hint1_Downloading:
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"下载中");
		break;
	case Main_Hint1_Ageing:
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"老化中");
		break;
	case Main_Hint1_Success:
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"成功");
		break;
	case Main_Hint1_Fail:
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"失败");
		break;

	//串口2提示结果
	case Main_Hint2_Ready:
		SetDlgItemText(IDC_PORT2HINT_STATIC, L"就绪");
		break;
	case Main_Hint2_Connected:
		SetDlgItemText(IDC_PORT2HINT_STATIC, L"已连接");
		break;
	case Main_Hint2_Downloading:
		SetDlgItemText(IDC_PORT2HINT_STATIC, L"下载中");
		break;
	case Main_Hint2_Ageing:
		SetDlgItemText(IDC_PORT2HINT_STATIC, L"老化中");
		break;
	case Main_Hint2_Success:
		SetDlgItemText(IDC_PORT2HINT_STATIC, L"成功");
		break;
	case Main_Hint2_Fail:
		SetDlgItemText(IDC_PORT2HINT_STATIC, L"失败");
		break;

	//串口3提示结果
	case Main_Hint3_Ready:
		SetDlgItemText(IDC_PORT3HINT_STATIC, L"就绪");
		break;
	case Main_Hint3_Connected:
		SetDlgItemText(IDC_PORT3HINT_STATIC, L"已连接");
		break;
	case Main_Hint3_Downloading:
		SetDlgItemText(IDC_PORT3HINT_STATIC, L"下载中");
		break;
	case Main_Hint3_Ageing:
		SetDlgItemText(IDC_PORT3HINT_STATIC, L"老化中");
		break;
	case Main_Hint3_Success:
		SetDlgItemText(IDC_PORT3HINT_STATIC, L"成功");
		break;
	case Main_Hint3_Fail:
		SetDlgItemText(IDC_PORT3HINT_STATIC, L"失败");
		break;

	//串口4提示结果
	case Main_Hint4_Ready:
		SetDlgItemText(IDC_PORT4HINT_STATIC, L"就绪");
		break;
	case Main_Hint4_Connected:
		SetDlgItemText(IDC_PORT4HINT_STATIC, L"已连接");
		break;
	case Main_Hint4_Downloading:
		SetDlgItemText(IDC_PORT4HINT_STATIC, L"下载中");
		break;
	case Main_Hint4_Ageing:
		SetDlgItemText(IDC_PORT4HINT_STATIC, L"老化中");
		break;
	case Main_Hint4_Success:
		SetDlgItemText(IDC_PORT4HINT_STATIC, L"成功");
		break;
	case Main_Hint4_Fail:
		SetDlgItemText(IDC_PORT4HINT_STATIC, L"失败");
		break;

	default:
		break;
	}
	return 0;
}

void CMFCP3SIMPORTDlg::Delay(int time)//time*1000为秒数 
{

	clock_t   now = clock();

	while (clock() - now < time);

}

void CMFCP3SIMPORTDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}

void CMFCP3SIMPORTDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	INT_PTR nRes;
	nRes = MessageBox(_T("您确定要退出吗？"), _T("提示"), MB_OKCANCEL | MB_ICONQUESTION);
	// 判断消息对话框返回值。如果为IDCANCEL就return，否则继续向下执行   
	if (IDCANCEL == nRes)
		return;

	CDialogEx::OnCancel();
}
