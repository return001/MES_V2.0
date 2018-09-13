
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
#include "ReSimDataDownload.h"
#include "Log.h"
#include "Manager.h"

#pragma comment(lib,"Shlwapi.lib") //判断文件是否存在的库，如果没有这行，会出现link错误

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//系统消息函数宏定义
#define WM_MainPortThreadControl WM_USER+1304  //线程消息循环
#define WM_MainDataInsertControl WM_USER+1305  //数据库消息循环
#define WM_MainFontControl WM_USER+1306        //字体提示消息循环

//全局变量
volatile BOOL m_MainDownloadControl;//主控线程
HWND MainFormHWND;//主控线程句柄
CString strFolderpath, strOKFolderpath,strFolderFile;//放文件夹路径
CString StrFolder[4] = { L"", L"", L"", L"" };//四个串口的文件夹路径
CString strSingleFilePath = L"";//单文件下载的文件路径
int simstart1flag = 0;
int simstart3flag = 0;
int simstart2flag = 0;
int simstart4flag = 0;
CString SinglePortLogName;
CString Port1LogName;
CString Port2LogName;
CString Port3LogName;
CString Port4LogName;
BOOL SinglePortAbnomal=FALSE;
//CString LastPort1RID = L"";
//CString LastPort1IMEI=L"";
BOOL Port1Abnomal=FALSE;
//CString LastPort2RID = L"";
//CString LastPort2IMEI = L"";
BOOL Port2Abnomal = FALSE;
//CString LastPort3RID = L"";
//CString LastPort3IMEI = L"";
BOOL Port3Abnomal = FALSE;
//CString LastPort4RID = L"";
//CString LastPort4IMEI = L"";
BOOL Port4Abnomal = FALSE;

int InteverIMEIRIDTime = 1600;//发指令间隔时间
int InteverIMEIRIDCount = 3;//发指令次数
//串口单文件下载

volatile BOOL m_Port1SINGLEDownloadWrite1;
volatile BOOL m_Port1SINGLEDownloadWrite2;
volatile BOOL m_Port1SINGLEDownloadWrite3;
volatile BOOL m_Port1SINGLEDownloadWrite4;
volatile BOOL m_Port1SINGLEDownloadRead1;
volatile BOOL m_Port1SINGLEDownloadRead2;
volatile BOOL m_Port1SINGLEDownloadReadEnd2;
volatile BOOL m_Port1SINGLEDownloadRead3;
volatile BOOL m_Port1SINGLEDownloadReadEnd3;
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
volatile BOOL m_Port1DownloadReadEnd3;
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
volatile BOOL m_Port2DownloadReadEnd3;
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
volatile BOOL m_Port3DownloadReadEnd3;
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
volatile BOOL m_Port4DownloadReadEnd3;
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
	ON_BN_CLICKED(IDC_OPENREMODLE_BUTTON, &CMFCP3SIMPORTDlg::OnBnClickedOpenremodleButton)
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
	CheckDlgButton(IDC_MULTIPLEDOWNLOAD_RADIO, 1);
	SetInitConfigWindow();
	GetDlgItem(IDC_STARTDOWNLOAD1_BUTTON)->EnableWindow(FALSE);

	OnBnClickedMultipledownloadRadio();

	//将ini配置文件信息的东西读出来，然后放到编辑框中
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

	//日志文件名称初始化
	SinglePortLogName = GetLogTime() + L"SinglePortLog";
	Port1LogName = GetLogTime() + L"Port1Log";
	Port2LogName = GetLogTime() + L"Port2Log";
	Port3LogName = GetLogTime() + L"Port3Log";
	Port4LogName = GetLogTime() + L"Port4Log";

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
	GetDlgItem(IDC_OPENREMODLE_BUTTON)->EnableWindow(TRUE);
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
	GetDlgItem(IDC_OPENREMODLE_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_PORTLIST1_COMBO)->EnableWindow(FALSE);
}

void CMFCP3SIMPORTDlg::PortSetOtherWindowTrue()
{
	GetDlgItem(IDC_DBCONFIG_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_OPENSIMDATAFOLDERPATH_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_SIMDATAFOLDERPATH_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_MULTIPLEDOWNLOAD_RADIO)->EnableWindow(TRUE);
	GetDlgItem(IDC_SINGLEDOWNLOAD_RADIO)->EnableWindow(TRUE);
	GetDlgItem(IDC_OPENREMODLE_BUTTON)->EnableWindow(TRUE);
}

void CMFCP3SIMPORTDlg::PortSetOtherWindowFalse()
{
	GetDlgItem(IDC_DBCONFIG_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_OPENSIMDATAFOLDERPATH_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_SIMDATAFOLDERPATH_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_MULTIPLEDOWNLOAD_RADIO)->EnableWindow(FALSE);
	GetDlgItem(IDC_SINGLEDOWNLOAD_RADIO)->EnableWindow(FALSE);
	GetDlgItem(IDC_OPENREMODLE_BUTTON)->EnableWindow(FALSE);
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

	CString strFilePath,strdefaultDir = strSingleFilePath.Left(strSingleFilePath.GetLength() - 23);
	fileDlg.m_ofn.lpstrInitialDir = strdefaultDir;
	
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


//返工位的东西
//开启返工位按钮
void CMFCP3SIMPORTDlg::OnBnClickedOpenremodleButton()
{
	// TODO:  在此添加控件通知处理程序代码
	INT_PTR nRes1;             // 用于保存DoModal函数的返回值   
	CManager *manager = new CManager;       // 构造对话框类实例   
	nRes1 = manager->DoModal();  // 弹出对话框

	if (IDCANCEL == nRes1)
		return;
	else if (IDOK == nRes1)
	{
		INT_PTR nRes;             // 用于保存DoModal函数的返回值   
		CReSimDataDownload *resimdatadownload = new CReSimDataDownload;       // 构造对话框类实例   
		nRes = resimdatadownload->DoModal();  // 弹出对话框

		if (IDCANCEL == nRes)
			return;
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
			if (nSel >= 0 && ComNo == LPWSTR(commName))//如果跟上次选择的相等那就不让当前选择变动
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
	//SetRicheditText(L"串口初始化成功！", 0);
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

				if (m_simdatafolderPath == L""||m_simdatafolderPath.Find(L"OK") >= 0)
				{
					MessageBox(L"请选择SIM卡数据路径！（即放着未下载过的sim卡数据文件夹，的文件夹）", L"提示信息", NULL);
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
			SetRicheditText(L"开启串口失败", 1);
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
	//关闭连接
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
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"停止");
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
	//开始下载
	if (initconfigflag == 1)
	{
		if (simstart1flag == 0)
		{
			if (!PathFileExists(strSingleFilePath))
			{
				MessageBox(L"文件不存在！请选择其它数据文件！",L"提示信息",NULL);
				return;
			}
			s_bSingleExit = FALSE;//主线程标志位
			OpenThreadPoolTask(PORT_AUTO_THREAD);//开启主线程
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
	//停止下载
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
			SetDlgItemText(IDC_PORT1HINT_STATIC, L"暂停");
			SetPort1EditEmpty();
			StrFolder[0] = L"";
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

			if (m_simdatafolderPath == L""||m_simdatafolderPath.Find(L"OK") >= 0)
			{
				MessageBox(L"请选择SIM卡数据路径！（即放着未下载过的sim卡数据文件夹，的文件夹）", L"提示信息", NULL);
				return;
			}
		}

		port2handler = InitCom(Com2No);

		if (port2handler == NULL)
		{
			SetRicheditText(L"开启串口失败", 1);
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
		SetDlgItemText(IDC_PORT2HINT_STATIC, L"停止");
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
		SetDlgItemText(IDC_PORT2HINT_STATIC, L"暂停");
		SetPort2EditEmpty();
		StrFolder[1] = L"";
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

			if (m_simdatafolderPath == L""||m_simdatafolderPath.Find(L"OK") >= 0)
			{
				MessageBox(L"请选择SIM卡数据路径！（即放着未下载过的sim卡数据文件夹，的文件夹）", L"提示信息", NULL);
				return;
			}
		}

		port3handler = InitCom(Com3No);

		if (port3handler == NULL)
		{
			SetRicheditText(L"开启串口失败", 1);
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
		SetDlgItemText(IDC_PORT3HINT_STATIC, L"停止");
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
		SetDlgItemText(IDC_PORT3HINT_STATIC, L"暂停");
		SetPort3EditEmpty();
		StrFolder[2] = L"";
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

			if (m_simdatafolderPath == L""||m_simdatafolderPath.Find(L"OK") >= 0)
			{
				MessageBox(L"请选择未完成的SIM卡数据路径！（即放着未下载过的sim卡数据文件夹，的文件夹）", L"提示信息", NULL);
				return;
			}
		}

		port4handler = InitCom(Com4No);

		if (port4handler == NULL)
		{
			SetRicheditText(L"开启串口失败", 1);
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
		SetDlgItemText(IDC_PORT4HINT_STATIC, L"停止");
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
		SetDlgItemText(IDC_PORT4HINT_STATIC, L"暂停");
		SetPort4EditEmpty();
		StrFolder[3] = L"";
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

	int findfileend=1,singleflag,countfileend;
	DWORD dwTotalSize;//文件总大小
	CFile ReadFile;
	//CRC变量
	CString strCID, strCIDfile;
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
					strCIDfile = finder.GetFileName();
					strFolderFile = strFolderpath + strCIDfile + L"\\assets.der";//文件夹名称
					//直接判断种子文件是否已经下过，如果已经下载过就移动到OK文件夹里
					
					if (!PathFileExists(strFolderFile))
					{
						MessageBox(L"读取文件错误！请检查目录下是否存在不包含assets.der种子文件的文件夹！",L"提示信息",NULL);
						dlg->OnBnClickedAutomultiplestartButton();
						return;
					}

					ADOManage adosinglesimdatamanage;
					adosinglesimdatamanage.ConnSQL();
					singleflag = adosinglesimdatamanage.SimDataNoIsExitSql(strCIDfile);
					adosinglesimdatamanage.CloseAll();

					if (singleflag == 0)
					{
						bFind = TRUE;
						MoveFileEx(strFolderpath + strCIDfile + "\\", strOKFolderpath + strCIDfile + "\\", MOVEFILE_REPLACE_EXISTING);//然后粘到下好的OK文件夹路径后面
						strFolderFile = L"";
						finder.Close();
						continue;
					}
					findfileend = 1;
                    
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
				Sleep(700);
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
				Sleep(700);
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
				Sleep(700);
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
				Sleep(700);
			}
			Sleep(200);
            
			bFind = finder.FindFile(strFolderpath + "*.*");
			
			countfileend = 0;
			while (bFind)
			{

				bFind = finder.FindNextFile();
				countfileend++;
				if (countfileend > 6)
				{
					break;
				}
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
				Sleep(200);
			}
			if (StrFolder[1] != L""&&simstart2flag == 1)
			{
				::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port2_Test, NULL);
				Sleep(200);
			}
			if (StrFolder[2] != L""&&simstart3flag == 1)
			{
				::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port3_Test, NULL);
				Sleep(200);
			}
			if (StrFolder[3] != L""&&simstart4flag == 1)
			{
				::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port4_Test, NULL);
				Sleep(200);
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
	PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;

	//放指令用变量
	CString strcommand = L"AT^GT_CM=TEST\r\n";

	//一直test，看看串口有没有连接上
	if (SinglePortAbnomal == TRUE)//这里是如果出现异常插拔，就先停顿个2.5秒才继续往下跑
	{
		Sleep(2000);
	}

	dlg->SetPort1EditEmpty();
	//dlg->GetDlgItem(IDC_OPENSIMDATAFILEPATH_BUTTON)->EnableWindow(TRUE);
	m_Port1SINGLEDownloadWrite1 = TRUE;

	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Ready, NULL);

	ClearCommError(dlg->port1handler, &dwErrorFlags, &ComStat);
	::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_SinglePort_SimDataNoIsExit, NULL);
	Sleep(50);
	do
	{
		dlg->PrintLog(L"发:" + strcommand,0);
		//dlg->SetRicheditText(L"发:" + strcommand, 0);
		bWriteStat = WriteFile(dlg->port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(2000);

		if (s_bSingleExit == TRUE)
		{
			dlg->SingleDownloadClosePort1Thread();
			dlg->SingleDownloadRestPort1Thread();
			return;
		}
	} while (m_Port1SINGLEDownloadWrite1);
	SinglePortAbnomal == FALSE;
	return;
}

//单文件串口1的test读线程
void CMFCP3SIMPORTDlg::SingleDownloadRead1Port1Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	//串口变量
	char str[100];
	memset(str, 0, sizeof(str) / sizeof(str[0]));;
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;

	m_Port1SINGLEDownloadRead1 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程
	//dlg->GetDlgItem(IDC_OPENSIMDATAFILEPATH_BUTTON)->EnableWindow(TRUE);
	do
	{
		Sleep(300);
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
	dlg->PrintLog(L"收:" + strread, 0);
	//dlg->SetRicheditText(L"收:" + strread, 0);
	PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
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
	PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

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
	CFile SingleReadFile;
	BOOL bOpen = SingleReadFile.Open(strpath, CFile::modeRead);
	dwTotalSize = SingleReadFile.GetLength();//获取文件总长度
	char *crcBuf = new char[dwTotalSize + 1];
	SingleReadFile.Read(crcBuf, dwTotalSize);
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
	SingleReadFile.Close();

	//然后同时开启读线程
	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Single_Read2, NULL);

	Sleep(100);

	//连接上了就开始读它的RID和IMEI，如果连续发送五条命令都没反应，那就返回Test那里重新检测设备
	for (int i = 0; i < 2; i++)
	{
		PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
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

		m_Port1SINGLEDownloadWrite2 = TRUE;

		do
		{

			if (count == InteverIMEIRIDCount)
			{
				//一直进入发送test指令，如果检测不到，那代表它已经断开了，首先设置一堆放各种读写的变量
				BOOL port1testflag = TRUE, port1failflag = TRUE;
				int port1testcount = 0;
				char port1teststr[100];
				memset(port1teststr, 0, sizeof(port1teststr) / sizeof(port1teststr[0]));
				DWORD port1testreadreal = 0, port1testdwBytesWrite, port1testdwErrorFlags;
				BOOL port1testbReadStat, port1testbWriteStat;
				CString port1teststrread, port1teststrcommand = L"AT^GT_CM=TEST\r\n";
				COMSTAT port1testComStat;

				ClearCommError(dlg->port1handler, &port1testdwErrorFlags, &port1testComStat);
				//这里开始检测机子是否还在连接
				do
				{
					port1testbWriteStat = WriteFile(dlg->port1handler, CT2A(port1teststrcommand), port1teststrcommand.GetLength(), &port1testdwBytesWrite, NULL);//写指令

					Sleep(90);
					port1testbReadStat = ReadFile(dlg->port1handler, port1teststr, 100, &port1testreadreal, 0);//读指令

					//判断读到的东西，如果连续五次没读到回复，那就代表机子已经被干掉了，而不是我们读数据没反应，如果机子有反应，我们还读不到数据，那就代表机子异常得重启一下
					if (port1testbReadStat)
					{
						port1teststrread = port1teststr;

						if (port1teststrread.Find(L"TEST_OK") >= 0)
						{
							if (port1failflag == TRUE)
							{
								::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_READFail, NULL);
								port1failflag = FALSE;
							}
							port1testcount = 0;
						}
					}

					port1testcount++;

					//连续读五次没反应就退出
					if (port1testcount == 5)
					{
						port1testflag = FALSE;
					}

					Sleep(10);

					memset(port1teststr, 0, sizeof(port1teststr) / sizeof(port1teststr[0]));

					dlg->PrintLog(L"发:" + port1teststrcommand, 0);

					PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
				} while (port1testflag);
				//清空一下编辑框
				dlg->SetDlgItemText(IDC_PORT1RID_EDIT, L"");
				dlg->SetDlgItemText(IDC_PORT1IMEI_EDIT, L"");

				dlg->SingleDownloadClosePort1Thread();
				dlg->SingleDownloadRestPort1Thread();
				SingleReadFile.Close();
				return;
			}

			dlg->PrintLog(L"发:" + strcommand, 0);
			//dlg->SetRicheditText(L"发:" + strcommand, 0);
			bWriteStat = WriteFile(dlg->port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);

			count++;
			Sleep(InteverIMEIRIDTime);
			if (s_bSingleExit == TRUE)
			{
				dlg->SingleDownloadClosePort1Thread();
				dlg->SingleDownloadRestPort1Thread();
				return;
			}
		} while (m_Port1SINGLEDownloadWrite2);
	}
	SingleReadFile.Close();
	return;
}

//单文件串口1的RID、IMEI读线程
void CMFCP3SIMPORTDlg::SingleDownloadRead2Port1Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	//串口变量
	char str[100];
	memset(str, 0, sizeof(str) / sizeof(str[0]));;
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
			Sleep(300);
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
							strcount.Replace(LPCTSTR(" "), LPCTSTR(""));
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
						if (strcount != ""&&strcount != "^"&&strcounttemp.Trim(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789").GetLength() == 0)
						{
							dlg->SetDlgItemText(IDC_PORT1IMEI_EDIT, strcount);

							int singleflag;
							CString strport1RID, strport1IMEI;
							ADOManage adoport1manage3;
							adoport1manage3.ConnSQL();

							dlg->GetDlgItemText(IDC_PORT1RID_EDIT, strport1RID);
							dlg->GetDlgItemText(IDC_PORT1IMEI_EDIT, strport1IMEI);

							singleflag = adoport1manage3.SimDataLastStationSql(strport1IMEI);

							if (singleflag == 0)
							{
								m_Port1SINGLEDownloadWrite2 = FALSE;
								m_Port1SINGLEDownloadRead2 = FALSE;
								m_Port1SINGLEDownloadReadEnd2 = FALSE;
								dlg->PrintLog(L"耦合漏测", 0);
								dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"耦合漏测");
								adoport1manage3.CloseAll();

								//一直进入发送test指令，如果检测不到，那代表它已经断开了
								BOOL port1testflag = TRUE;
								int port1testcount = 0;
								char port1teststr[100];
								memset(port1teststr, 0, sizeof(port1teststr) / sizeof(port1teststr[0]));
								DWORD port1testreadreal = 0, port1testdwBytesWrite, port1testdwErrorFlags;
								BOOL port1testbReadStat, port1testbWriteStat;
								CString port1teststrread, port1teststrcommand = L"AT^GT_CM=TEST\r\n";
								COMSTAT port1testComStat;

								ClearCommError(dlg->port1handler, &port1testdwErrorFlags, &port1testComStat);
								do
								{
									port1testbWriteStat = WriteFile(dlg->port1handler, CT2A(port1teststrcommand), port1teststrcommand.GetLength(), &port1testdwBytesWrite, NULL);

									Sleep(90);
									port1testbReadStat = ReadFile(dlg->port1handler, port1teststr, 100, &port1testreadreal, 0);
									if (port1testbReadStat)
									{
										port1teststrread = port1teststr;
										if (port1teststrread.Find(L"TEST_OK") >= 0)
										{
											port1testcount = 0;
										}
									}
									port1testcount++;
									if (port1testcount == 5)
									{
										port1testflag = FALSE;
									}
									Sleep(10);
									memset(port1teststr, 0, sizeof(port1teststr) / sizeof(port1teststr[0]));
									dlg->PrintLog(L"发:" + port1teststrcommand, 0);
									PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
								} while (port1testflag);

								Sleep(50);
								dlg->SetDlgItemText(IDC_PORT1RID_EDIT, L"");
								dlg->SetDlgItemText(IDC_PORT1IMEI_EDIT, L"");
								dlg->SingleDownloadClosePort1Thread();
								dlg->SingleDownloadRestPort1Thread();
								return ;
							}
							singleflag = adoport1manage3.SimDataIsExitSql(strport1RID, strport1IMEI);
							if (singleflag == 0)
							{
								m_Port1SINGLEDownloadWrite2 = FALSE;
								m_Port1SINGLEDownloadRead2 = FALSE;
								m_Port1SINGLEDownloadReadEnd2 = FALSE;
								dlg->PrintLog(L"工位已测", 0);
								dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"工位已测");
								adoport1manage3.CloseAll();

								//一直进入发送test指令，如果检测不到，那代表它已经断开了
								BOOL port1testflag = TRUE;
								int port1testcount = 0;
								char port1teststr[100];
								memset(port1teststr, 0, sizeof(port1teststr) / sizeof(port1teststr[0]));
								DWORD port1testreadreal = 0, port1testdwBytesWrite, port1testdwErrorFlags;
								BOOL port1testbReadStat, port1testbWriteStat;
								CString port1teststrread, port1teststrcommand = L"AT^GT_CM=TEST\r\n";
								COMSTAT port1testComStat;

								ClearCommError(dlg->port1handler, &port1testdwErrorFlags, &port1testComStat);
								do
								{
									port1testbWriteStat = WriteFile(dlg->port1handler, CT2A(port1teststrcommand), port1teststrcommand.GetLength(), &port1testdwBytesWrite, NULL);

									Sleep(90);
									port1testbReadStat = ReadFile(dlg->port1handler, port1teststr, 100, &port1testreadreal, 0);
									if (port1testbReadStat)
									{
										port1teststrread = port1teststr;
										if (port1teststrread.Find(L"TEST_OK") >= 0)
										{
											port1testcount = 0;
										}
									}
									port1testcount++;
									if (port1testcount == 5)
									{
										port1testflag = FALSE;
									}
									Sleep(10);
									memset(port1teststr, 0, sizeof(port1teststr) / sizeof(port1teststr[0]));
									dlg->PrintLog(L"发:" + port1teststrcommand, 0);
									PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
								} while (port1testflag);


								Sleep(50);
								dlg->SetDlgItemText(IDC_PORT1RID_EDIT, L"");
								dlg->SetDlgItemText(IDC_PORT1IMEI_EDIT, L"");
								dlg->SingleDownloadClosePort1Thread();
								dlg->SingleDownloadRestPort1Thread();
								return ;
							}
							//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_SinglePort_IsExit, NULL);
							adoport1manage3.CloseAll();
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
		//dlg->SetRicheditText(L"收:" + strread, 0);
		dlg->PrintLog(L"收:" + strread, 0);
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

	CFile ReadFile1;
	BOOL bOpen = ReadFile1.Open(strpath, CFile::modeRead);
	dwTotalSize = ReadFile1.GetLength();//获取文件总长度

	//开始写文件
	strcommand = L"AT^GT_CM=SOFTSIM_DATA,";

	//数据缓冲区
	char *pBuf = new char[dwStep + 1];
	memset(pBuf, '\0', dwStep + 1);

	ClearCommError(dlg->port1handler, &dwErrorFlags, &ComStat);

	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Downloading, NULL);
	//PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	while ((dwRead = ReadFile1.Read(pBuf, dwStep)) > 0)
	{
		PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
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
		dlg->PrintLog(L"发:" + strfilewritecommand, 0);
		//dlg->SetRicheditText(L"发:" + strfilewritecommand, 0);
		dwBytesWrite = strcommand.GetLength();
		bWriteStat = WriteFile(dlg->port1handler, CT2A(strfilewritecommand), strfilewritecommand.GetLength(), &dwBytesWrite, NULL);

		intdatano++;
		memset(pBuf, '\0', dwStep + 1);
		delete[] buf1;
		buf1 = NULL;
        Sleep(650);
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
		if (countend == 3)
		{

			::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port1_ErrorInsert, NULL);//先插入失败记录

			//一直进入发送test指令，如果检测不到，那代表它已经断开了
			BOOL port1testflag = TRUE, port1failflag = TRUE;
			int port1testcount = 0;
			char port1teststr[100];
			memset(port1teststr, 0, sizeof(port1teststr) / sizeof(port1teststr[0]));
			DWORD port1testreadreal = 0, port1testdwBytesWrite, port1testdwErrorFlags;
			BOOL port1testbReadStat, port1testbWriteStat;
			CString port1teststrread, port1teststrcommand = L"AT^GT_CM=TEST\r\n";
			COMSTAT port1testComStat;

			ClearCommError(dlg->port1handler, &port1testdwErrorFlags, &port1testComStat);
			do
			{
				port1testbWriteStat = WriteFile(dlg->port1handler, CT2A(port1teststrcommand), port1teststrcommand.GetLength(), &port1testdwBytesWrite, NULL);

				Sleep(90);
				port1testbReadStat = ReadFile(dlg->port1handler, port1teststr, 100, &port1testreadreal, 0);
				if (port1testbReadStat)
				{
					port1teststrread = port1teststr;
					if (port1teststrread.Find(L"TEST_OK") >= 0)
					{
						if (port1failflag == TRUE)
						{
							::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Fail, NULL);
							port1failflag = FALSE;
						}
						port1testcount = 0;
					}
					else//这里是如果一开始就读不到机子，那肯定是被人中途拔掉了，这是异常也就是违规操作
					{
						if (port1failflag == TRUE)
						{
							::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_AbnormolFail, NULL);
							port1failflag = FALSE;
							SinglePortAbnomal = TRUE;
						}
					}
				}

				port1testcount++;

				if (port1testcount == 5)
				{
					port1testflag = FALSE;
				}

				Sleep(10);
				memset(port1teststr, 0, sizeof(port1teststr) / sizeof(port1teststr[0]));
				dlg->PrintLog(L"发:" + port1teststrcommand, 0);
				PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
			} while (port1testflag);

			dlg->SetDlgItemText(IDC_PORT1RID_EDIT, L"");
			dlg->SetDlgItemText(IDC_PORT1IMEI_EDIT, L"");
			dlg->SingleDownloadClosePort1Thread();
			dlg->SingleDownloadRestPort1Thread();
			return;
		}

		dlg->PrintLog(L"发:" + strcommand, 0);
		//dlg->SetRicheditText(L"发:" + strcommand, 0);
		bWriteStat = WriteFile(dlg->port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(2000);
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
	memset(str, 0, sizeof(str) / sizeof(str[0]));;
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;

	m_Port1SINGLEDownloadRead3 = TRUE;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程
	m_Port1SINGLEDownloadReadEnd3 = TRUE;

	do
	{
		Sleep(300);
		bReadStat = ReadFile(dlg->port1handler, str, 200, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			if (strread.Find(L"SoftSim,") >= 0)
			{
					if (strread.Find(L"SoftSim,OK") >= 0)
					{
						//::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Success, NULL);
						//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_SinglePort_OkInsert, NULL);
					}
					if (strread.Find(L"Error") >= 0)
					{
						::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_SinglePort_ErrorInsert, NULL);
						m_Port1SINGLEDownloadWrite3 = FALSE;
						m_Port1SINGLEDownloadRead3 = FALSE;
						m_Port1SINGLEDownloadReadEnd3 = FALSE;

						//一直进入发送test指令，如果检测不到，那代表它已经断开了
						BOOL port1testflag = TRUE, port1failflag = TRUE;
						int port1testcount = 0;
						char port1teststr[100];
						memset(port1teststr, 0, sizeof(port1teststr) / sizeof(port1teststr[0]));
						DWORD port1testreadreal = 0, port1testdwBytesWrite, port1testdwErrorFlags;
						BOOL port1testbReadStat, port1testbWriteStat;
						CString port1teststrread, port1teststrcommand = L"AT^GT_CM=TEST\r\n";
						COMSTAT port1testComStat;

						ClearCommError(dlg->port1handler, &port1testdwErrorFlags, &port1testComStat);
						do
						{
							port1testbWriteStat = WriteFile(dlg->port1handler, CT2A(port1teststrcommand), port1teststrcommand.GetLength(), &port1testdwBytesWrite, NULL);

							Sleep(90);
							port1testbReadStat = ReadFile(dlg->port1handler, port1teststr, 100, &port1testreadreal, 0);
							if (port1testbReadStat)
							{
								port1teststrread = port1teststr;
								if (port1teststrread.Find(L"TEST_OK") >= 0)
								{
									if (port1failflag == TRUE)
									{
										::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Fail, NULL);
										port1failflag = FALSE;
									}
									port1testcount = 0;
								}
								//else
								//{
								//	if (port1failflag == TRUE)
								//	{
								//		::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_AbnormolFail, NULL);
								//		port1failflag = FALSE;
								//		Port1Abnomal = TRUE;
								//	}
								//}
							}
							port1testcount++;
							if (port1testcount == 5)
							{
								port1testflag = FALSE;
							}
							Sleep(10);
							memset(port1teststr, 0, sizeof(port1teststr) / sizeof(port1teststr[0]));
							dlg->PrintLog(L"发:" + port1teststrcommand, 0);
							PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
						} while (port1testflag);

						Sleep(100);

						dlg->SingleDownloadRestPort1Thread();
						dlg->PrintLog(L"收:" + strread, 0);
						//dlg->SetRicheditText(L"收:" + strread, 0);
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
	if (m_Port1SINGLEDownloadReadEnd3 == FALSE)
	{
		return;
	}
	dlg->PrintLog(L"发:" + strread, 0);
	//dlg->SetRicheditText(L"收:" + strread, 0);
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
		if (countend == 3)
		{
			//strcommand = L"AT^GT_CM=reset,1\r\n";
			//ClearCommError(dlg->port1handler, &dwErrorFlags, &ComStat);
			//bWriteStat = WriteFile(dlg->port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
			//dlg->PrintLog(L"发:" + strcommand, 0);

			::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_SinglePort_ErrorInsert, NULL);
			//一直进入发送test指令，如果检测不到，那代表它已经断开了
			BOOL port1testflag = TRUE, port1failflag = TRUE;
			int port1testcount = 0;
			char port1teststr[100];
			memset(port1teststr, 0, sizeof(port1teststr) / sizeof(port1teststr[0]));
			DWORD port1testreadreal = 0, port1testdwBytesWrite, port1testdwErrorFlags;
			BOOL port1testbReadStat, port1testbWriteStat;
			CString port1teststrread, port1teststrcommand = L"AT^GT_CM=aging,off\r\n";
			COMSTAT port1testComStat;

			//老化失败后必须确保老化是关的，所以最好发一条老化关闭指令，接着再判断机子是连接着的还是中途被人直接拔掉了
			port1testbWriteStat = WriteFile(dlg->port1handler, CT2A(port1teststrcommand), port1teststrcommand.GetLength(), &port1testdwBytesWrite, NULL);
			Sleep(500);

			port1teststrcommand = L"AT^GT_CM=TEST\r\n";
			ClearCommError(dlg->port1handler, &port1testdwErrorFlags, &port1testComStat);

			do
			{
				port1testbWriteStat = WriteFile(dlg->port1handler, CT2A(port1teststrcommand), port1teststrcommand.GetLength(), &port1testdwBytesWrite, NULL);

				Sleep(90);
				port1testbReadStat = ReadFile(dlg->port1handler, port1teststr, 100, &port1testreadreal, 0);
				if (port1testbReadStat)
				{
					port1teststrread = port1teststr;
					if (port1teststrread.Find(L"TEST_OK") >= 0)
					{
						if (port1failflag == TRUE)
						{
							::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Fail, NULL);
							port1failflag = FALSE;
						}
						port1testcount = 0;
					}
					else
					{
						if (port1failflag == TRUE)
						{
							::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_AbnormolFail, NULL);
							port1failflag = FALSE;
							SinglePortAbnomal = TRUE;
						}
					}
				}
				port1testcount++;
				if (port1testcount == 5)
				{
					port1testflag = FALSE;
				}
				Sleep(10);
				memset(port1teststr, 0, sizeof(port1teststr) / sizeof(port1teststr[0]));
				dlg->PrintLog(L"发:" + port1teststrcommand, 0);
				PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
			} while (port1testflag);

			Sleep(100);
			m_Port1SINGLEDownloadWrite4 = FALSE;
			m_Port1SINGLEDownloadRead4 = FALSE;

			dlg->SingleDownloadRestPort1Thread();
			return;
		}

		dlg->PrintLog(L"发:" + strcommand, 0);
		//dlg->SetRicheditText(L"发:" + strcommand, 0);
		bWriteStat = WriteFile(dlg->port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(2000);
		countend++;
		if (s_bSingleExit == TRUE)
		{
			dlg->SingleDownloadClosePort1Thread();
			dlg->SingleDownloadRestPort1Thread();
			return;
		}
	} while (m_Port1SINGLEDownloadWrite4);

	dlg->SingleDownloadRestPort1Thread();
	//strcommand = L"AT^GT_CM=reset,1\r\n";
	//ClearCommError(dlg->port1handler, &dwErrorFlags, &ComStat);
	//bWriteStat = WriteFile(dlg->port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
	//dlg->PrintLog(L"发:" + strcommand, 0);
}

//单文件串口1的读老化命令的读线程
void CMFCP3SIMPORTDlg::SingleDownloadRead4Port1Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	char str[100];
	memset(str, 0, sizeof(str) / sizeof(str[0]));;
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;

	m_Port1SINGLEDownloadRead4 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程

	do
	{
		Sleep(300);
		bReadStat = ReadFile(dlg->port1handler, str, 100, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			if (strread.Find(L"aging,on\r\r\nOK!") >= 0)
			{
				::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Success, NULL);
				::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_SinglePort_OkInsert, NULL);
				m_Port1SINGLEDownloadRead4 = FALSE;
				m_Port1SINGLEDownloadWrite4 = FALSE;
			}
			else
			{
				continue;
			}

		}
		if (s_bSingleExit == TRUE)
		{
			dlg->PrintLog(L"收:" + strread, 0);
			dlg->SingleDownloadClosePort1Thread();
			dlg->SingleDownloadRestPort1Thread();
			return;
		}
	} while (m_Port1SINGLEDownloadRead4);
	//dlg->SetRicheditText(L"收:" + strread, 0);
	dlg->PrintLog(L"收:" + strread, 0);
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
	m_Port1SINGLEDownloadReadEnd3 = FALSE;
	m_Port1SINGLEDownloadRead4 = FALSE;
}


//多文件串口下载

//串口1下载逻辑
//串口1的test写线程
void CMFCP3SIMPORTDlg::DownloadWrite1Port1Thread(LPVOID lpParam)
{
	//首先先把主窗口指针传进来，这样才能运用主窗口的函数以及控件
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);//清串口缓存指令

	//int port1testcount=0;

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;

	//放指令用变量
	CString strcommand = L"AT^GT_CM=TEST\r\n";

	//一直test，看看串口有没有连接上
	if (Port1Abnomal == TRUE)//这里是如果出现异常插拔，就先停顿个2.5秒才继续往下跑
	{
		Sleep(2500);
	}

	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Ready, NULL);//发消息，串口结果显示待测试
	
	m_Port1DownloadWrite1 = TRUE;//当前线程标志

	ClearCommError(dlg->port1handler, &dwErrorFlags, &ComStat);//清理一下串口错误缓存
	
	Sleep(50);

	//从这里开始循环每隔两秒循环发指令，一直到接收到回复为止
	do
	{
		dlg->PrintLog(L"发:" + strcommand, 1);//保存本地日志
		//dlg->SetRicheditText(L"发:" + strcommand, 1);
		bWriteStat = WriteFile(dlg->port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);

		//if (port1testcount == 1)
		//{
		//	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Ready, NULL);
		//	//dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"待测试");
		//}
		Sleep(2000);
		//如果被强行关闭了，那就直接退出
		if (s_bExit == TRUE||m_Port1DownloadControl == FALSE)
		{
			dlg->DownloadClosePort1Thread();
			dlg->DownloadRestPort1Thread();
			return;
		}
		//port1testcount++;
	} while (m_Port1DownloadWrite1);
	
	Port1Abnomal = FALSE;//将异常标志位设置为FALSE
	return ;
}

//串口1的test读线程
void CMFCP3SIMPORTDlg::DownloadRead1Port1Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	char str[100];//存放读出来的数据的变量
	memset(str, 0, sizeof(str) / sizeof(str[0]));//先清空
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;

	m_Port1DownloadRead1 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程

	//开始读取串口的回复
	do
	{
		Sleep(300);

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

	dlg->PrintLog(L"收:" + strread, 1);
	//dlg->SetRicheditText(L"收:" + strread, 0);
	PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port1_Write2, NULL);//开启下一条指令线程
}

//串口1的写RID、IMEI的写线程
void CMFCP3SIMPORTDlg::DownloadWrite2Port1Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Connected, NULL);
	//PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

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
		PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

		int count = 0;//用来判断当前指令已经发了多少次了

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

		::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port1_Read2, NULL);//开启读线程

		do
		{
			//循环中先判断countend也就是指令是否已经发了n次，如果发了n次没收到任何回应，就表示机子那边没回应，此时程序需要准备结束了
			if (count == InteverIMEIRIDCount)
			{
				//一直进入发送test指令，如果检测不到，那代表它已经断开了，首先设置一堆放各种读写的变量
				BOOL port1testflag = TRUE,port1failflag=TRUE;
				int port1testcount = 0;
				char port1teststr[100];
				memset(port1teststr, 0, sizeof(port1teststr) / sizeof(port1teststr[0]));
				DWORD port1testreadreal = 0, port1testdwBytesWrite, port1testdwErrorFlags;
				BOOL port1testbReadStat, port1testbWriteStat;
				CString port1teststrread, port1teststrcommand = L"AT^GT_CM=TEST\r\n";
				COMSTAT port1testComStat;

				ClearCommError(dlg->port1handler, &port1testdwErrorFlags, &port1testComStat);
				//这里开始检测机子是否还在连接
				do
				{
					port1testbWriteStat = WriteFile(dlg->port1handler, CT2A(port1teststrcommand), port1teststrcommand.GetLength(), &port1testdwBytesWrite, NULL);//写指令

					Sleep(90);
					port1testbReadStat = ReadFile(dlg->port1handler, port1teststr, 100, &port1testreadreal, 0);//读指令

					//判断读到的东西，如果连续五次没读到回复，那就代表机子已经被干掉了，而不是我们读数据没反应，如果机子有反应，我们还读不到数据，那就代表机子异常得重启一下
					if (port1testbReadStat)
					{
						port1teststrread = port1teststr;

						if (port1teststrread.Find(L"TEST_OK") >= 0)
						{
							if (port1failflag == TRUE)
							{
								::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_READFail, NULL);
								port1failflag = FALSE;
							}
							port1testcount = 0;
						}
					}

					port1testcount++;

					//连续读五次没反应就退出
					if (port1testcount == 5)
					{
						port1testflag = FALSE;
					}

					Sleep(10);

					memset(port1teststr, 0, sizeof(port1teststr) / sizeof(port1teststr[0]));

					dlg->PrintLog(L"发:" + port1teststrcommand, 1);

					PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
				} while (port1testflag);
				//清空一下编辑框
				dlg->SetDlgItemText(IDC_PORT1RID_EDIT,L"");
				dlg->SetDlgItemText(IDC_PORT1IMEI_EDIT, L"");

				dlg->DownloadClosePort1Thread();
				dlg->DownloadRestPort1Thread();
				return;
			}

			dlg->PrintLog(L"发:" + strcommand, 1);
			//dlg->SetRicheditText(L"发:" + strcommand, 0);
			bWriteStat = WriteFile(dlg->port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
			count++;

			Sleep(InteverIMEIRIDTime);//指令发出间隔

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
	char str[200];
	memset(str, 0, sizeof(str) / sizeof(str[0]));
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;
	CString strcount,strcounttemp;//放RID和IMEI用
	CString strtemp;
	int findcount1, findcount2;//切割读出来的字符串用

	//开始读串口回复
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
			Sleep(200);//每0.2秒读一次

			bReadStat = ReadFile(dlg->port1handler, str, 199, &readreal, 0);
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
							strcount.Replace(LPCTSTR(" "), LPCTSTR(""));
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
						if (strcount != ""&&strcount != "^"&&strcounttemp.Trim(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789").GetLength() == 0)
						{
							dlg->SetDlgItemText(IDC_PORT1IMEI_EDIT, strcount);

							//下面是数据库操作，判断此台机子在数据库的状况
							int port1flag;
							CString strport1RID, strport1IMEI;
							ADOManage adoport1manage3;
							adoport1manage3.ConnSQL();
							dlg->GetDlgItemText(IDC_PORT1RID_EDIT, strport1RID);
							dlg->GetDlgItemText(IDC_PORT1IMEI_EDIT, strport1IMEI);

							port1flag = adoport1manage3.SimDataLastStationSql(strport1IMEI);
							if (port1flag == 0)
							{
								m_Port1DownloadWrite2 = FALSE;
								m_Port1DownloadRead2 = FALSE;
								m_Port1DownloadReadEnd2 = FALSE;
								dlg->PrintLog(L"耦合漏测", 1);
								dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"耦合漏测");
								adoport1manage3.CloseAll();

								//一直进入发送test指令，如果检测不到，那代表它已经断开了
								BOOL port1testflag = TRUE;
								int port1testcount = 0;
								char port1teststr[100];
								memset(port1teststr, 0, sizeof(port1teststr) / sizeof(port1teststr[0]));
								DWORD port1testreadreal = 0, port1testdwBytesWrite, port1testdwErrorFlags;
								BOOL port1testbReadStat, port1testbWriteStat;
								CString port1teststrread, port1teststrcommand = L"AT^GT_CM=TEST\r\n";
								COMSTAT port1testComStat;

								ClearCommError(dlg->port1handler, &port1testdwErrorFlags, &port1testComStat);
								do
								{
									port1testbWriteStat = WriteFile(dlg->port1handler, CT2A(port1teststrcommand), port1teststrcommand.GetLength(), &port1testdwBytesWrite, NULL);

									Sleep(90);
									port1testbReadStat = ReadFile(dlg->port1handler, port1teststr, 100, &port1testreadreal, 0);
									if (port1testbReadStat)
									{
										port1teststrread = port1teststr;
										if (port1teststrread.Find(L"TEST_OK") >= 0)
										{
											port1testcount = 0;
										}
									}
									port1testcount++;
									if (port1testcount == 5)
									{
										port1testflag = FALSE;
									}
									Sleep(10);
									memset(port1teststr, 0, sizeof(port1teststr) / sizeof(port1teststr[0]));
									dlg->PrintLog(L"发:" + port1teststrcommand, 1);
									PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
								} while (port1testflag);


								Sleep(50);
								dlg->SetDlgItemText(IDC_PORT1RID_EDIT, L"");
								dlg->SetDlgItemText(IDC_PORT1IMEI_EDIT, L"");
								dlg->DownloadClosePort1Thread();
								dlg->DownloadRestPort1Thread();
								return ;
							}

							port1flag = adoport1manage3.SimDataIsExitSql(strport1RID, strport1IMEI);

							if (port1flag == 0)
							{
								m_Port1DownloadWrite2 = FALSE;
								m_Port1DownloadRead2 = FALSE;
								m_Port1DownloadReadEnd2 = FALSE;
								dlg->PrintLog(L"工位已测", 1);
								dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"工位已测");
								//if (LastPort1RID != strport1RID&&LastPort1IMEI != strport1IMEI)
								//{
								//	dlg->SetDlgItemText(IDC_PORT1HINT_STATIC, L"工位已测");
								//}
								adoport1manage3.CloseAll();

								//一直进入发送test指令，如果检测不到，那代表它已经断开了
								BOOL port1testflag = TRUE;
								int port1testcount = 0;
								char port1teststr[100];
								memset(port1teststr, 0, sizeof(port1teststr) / sizeof(port1teststr[0]));
								DWORD port1testreadreal = 0, port1testdwBytesWrite, port1testdwErrorFlags;
								BOOL port1testbReadStat, port1testbWriteStat;
								CString port1teststrread, port1teststrcommand = L"AT^GT_CM=TEST\r\n";
								COMSTAT port1testComStat;

								ClearCommError(dlg->port1handler, &port1testdwErrorFlags, &port1testComStat);
								do
								{
									port1testbWriteStat = WriteFile(dlg->port1handler, CT2A(port1teststrcommand), port1teststrcommand.GetLength(), &port1testdwBytesWrite, NULL);

									Sleep(90);
									port1testbReadStat = ReadFile(dlg->port1handler, port1teststr, 100, &port1testreadreal, 0);
									if (port1testbReadStat)
									{
										port1teststrread = port1teststr;
										if (port1teststrread.Find(L"TEST_OK") >= 0)
										{
											port1testcount = 0;
										}
									}
									port1testcount++;
									if (port1testcount == 5)
									{
										port1testflag = FALSE;
									}
									Sleep(10);
									memset(port1teststr, 0, sizeof(port1teststr) / sizeof(port1teststr[0]));
									dlg->PrintLog(L"发:" + port1teststrcommand, 1);
									PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
								} while (port1testflag);


								Sleep(50);
								dlg->SetDlgItemText(IDC_PORT1RID_EDIT, L"");
								dlg->SetDlgItemText(IDC_PORT1IMEI_EDIT, L"");
								dlg->DownloadClosePort1Thread();
								dlg->DownloadRestPort1Thread();
								return;
							}
							adoport1manage3.CloseAll();
							//LastPort1RID = strport1RID;
							//LastPort1IMEI = strport1IMEI;

							//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port1_IsExit, NULL);
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

		if (m_Port1DownloadReadEnd2 == FALSE)//这个判断是保险用
		{
			return;
		}

		if (s_bExit == TRUE || m_Port1DownloadControl == FALSE)
		{
			dlg->DownloadClosePort1Thread();
			dlg->DownloadRestPort1Thread();
			return;
		}
		//dlg->SetRicheditText(L"收:" + strread, 0);
		dlg->PrintLog(L"收:" + strread, 1);
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
	CFile ReadFile1;
	BOOL bOpen = ReadFile1.Open(StrFolder[0], CFile::modeRead);
	dwTotalSize = ReadFile1.GetLength();//获取文件总长度

	//开始写文件
	strcommand = L"AT^GT_CM=SOFTSIM_DATA,";

	//数据缓冲区
	char *pBuf = new char[dwStep + 1];
	memset(pBuf, '\0', dwStep + 1);

	ClearCommError(dlg->port1handler, &dwErrorFlags, &ComStat);

	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Downloading, NULL);
	//PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//这个While循环是用来发送文件数据的
	while ((dwRead = ReadFile1.Read(pBuf, dwStep)) > 0)
	{
		PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
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
		//dlg->SetRicheditText(L"发:" + strfilewritecommand, 0);
		dlg->PrintLog(L"发:" + strfilewritecommand, 1);
		dwBytesWrite = strcommand.GetLength();
		bWriteStat = WriteFile(dlg->port1handler, CT2A(strfilewritecommand), strfilewritecommand.GetLength(), &dwBytesWrite, NULL);

		intdatano++;
		memset(pBuf, '\0', dwStep + 1);
		delete[] buf1;
		buf1 = NULL;
		Sleep(650);
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
		//循环中先判断countend也就是指令是否已经发了n次，如果发了n次没收到任何回应，就表示机子那边没回应，此时程序需要准备结束了
		if (countend == 3)
		{
			::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port1_ErrorInsert, NULL);//先插入失败记录

			//一直进入发送test指令，如果检测不到，那代表它已经断开了
			BOOL port1testflag = TRUE, port1failflag = TRUE;
			int port1testcount = 0;
			char port1teststr[100];
			memset(port1teststr, 0, sizeof(port1teststr) / sizeof(port1teststr[0]));
			DWORD port1testreadreal = 0, port1testdwBytesWrite, port1testdwErrorFlags;
			BOOL port1testbReadStat, port1testbWriteStat;
			CString port1teststrread, port1teststrcommand = L"AT^GT_CM=TEST\r\n";
			COMSTAT port1testComStat;

			ClearCommError(dlg->port1handler, &port1testdwErrorFlags, &port1testComStat);
			do
			{
				port1testbWriteStat = WriteFile(dlg->port1handler, CT2A(port1teststrcommand), port1teststrcommand.GetLength(), &port1testdwBytesWrite, NULL);

				Sleep(90);
				port1testbReadStat = ReadFile(dlg->port1handler, port1teststr, 100, &port1testreadreal, 0);
				if (port1testbReadStat)
				{
					port1teststrread = port1teststr;
					if (port1teststrread.Find(L"TEST_OK") >= 0)
					{
						if (port1failflag == TRUE)
						{
							::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Fail, NULL);
							port1failflag = FALSE;
						}
						port1testcount = 0;
					}
					else//这里是如果一开始就读不到机子，那肯定是被人中途拔掉了，这是异常也就是违规操作
					{
						if (port1failflag == TRUE)
						{
							::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_AbnormolFail, NULL);
							port1failflag = FALSE;
							Port1Abnomal = TRUE;
						}
					}
				}

				port1testcount++;

				if (port1testcount == 5)
				{
					port1testflag = FALSE;
				}

				Sleep(10);
				memset(port1teststr, 0, sizeof(port1teststr) / sizeof(port1teststr[0]));
				dlg->PrintLog(L"发:" + port1teststrcommand, 1);
				PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
			} while (port1testflag);

			dlg->SetDlgItemText(IDC_PORT1RID_EDIT, L"");
			dlg->SetDlgItemText(IDC_PORT1IMEI_EDIT, L"");
			dlg->DownloadClosePort1Thread();
			dlg->DownloadRestPort1Thread();
			return;
		}

		//这里发出写文件结束指令
		//dlg->SetRicheditText(L"发:" + strcommand, 0);
		dlg->PrintLog(L"发:" + strcommand, 1);
		bWriteStat = WriteFile(dlg->port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(1500);
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
	memset(str, 0, sizeof(str) / sizeof(str[0]));;
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;
	PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	m_Port1DownloadRead3 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程
	m_Port1DownloadReadEnd3 = TRUE;

	do
	{
		Sleep(300);
		bReadStat = ReadFile(dlg->port1handler, str, 99, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			if (strread.Find(L"SoftSim,") >= 0)
			{
				if (strread.Find(L"SoftSim,OK") >= 0)
				{
					//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_SinglePort_OkInsert, NULL);
				}
				else if (strread.Find(L"Error") >= 0)
				{
					::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port1_ErrorInsert, NULL);

					m_Port1DownloadWrite3 = FALSE;
					m_Port1DownloadRead3 = FALSE;
					m_Port1DownloadReadEnd3 = FALSE;

					//一直进入发送test指令，如果检测不到，那代表它已经断开了
					BOOL port1testflag = TRUE, port1failflag = TRUE;
					int port1testcount = 0;
					char port1teststr[100];
					memset(port1teststr, 0, sizeof(port1teststr) / sizeof(port1teststr[0]));
					DWORD port1testreadreal = 0, port1testdwBytesWrite, port1testdwErrorFlags;
					BOOL port1testbReadStat, port1testbWriteStat;
					CString port1teststrread, port1teststrcommand = L"AT^GT_CM=TEST\r\n";
					COMSTAT port1testComStat;

					ClearCommError(dlg->port1handler, &port1testdwErrorFlags, &port1testComStat);
					do
					{
						port1testbWriteStat = WriteFile(dlg->port1handler, CT2A(port1teststrcommand), port1teststrcommand.GetLength(), &port1testdwBytesWrite, NULL);

						Sleep(90);
						port1testbReadStat = ReadFile(dlg->port1handler, port1teststr, 100, &port1testreadreal, 0);
						if (port1testbReadStat)
						{
							port1teststrread = port1teststr;
							if (port1teststrread.Find(L"TEST_OK") >= 0)
							{
								if (port1failflag == TRUE)
								{
									::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Fail, NULL);
									port1failflag = FALSE;
								}
								port1testcount = 0;
							}
							//else
							//{
							//	if (port1failflag == TRUE)
							//	{
							//		::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_AbnormolFail, NULL);
							//		port1failflag = FALSE;
							//		Port1Abnomal = TRUE;
							//	}
							//}
						}
						port1testcount++;
						if (port1testcount == 5)
						{
							port1testflag = FALSE;
						}
						Sleep(10);
						memset(port1teststr, 0, sizeof(port1teststr) / sizeof(port1teststr[0]));
						dlg->PrintLog(L"发:" + port1teststrcommand, 1);
						PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
					} while (port1testflag);

					Sleep(100);

					dlg->PrintLog(L"收:" + strread, 1);
					//dlg->SetRicheditText(L"收:" + strread, 0);
					dlg->DownloadRestPort1Thread();
					return;
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

	if (m_Port1DownloadReadEnd3 == FALSE)
	{
		return;
	}
	dlg->PrintLog(L"收:" + strread, 1);
	//dlg->SetRicheditText(L"收:" + strread, 0);
	PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port1_Write4, NULL);
	//dlg->DownloadRestPort1Thread();
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
		dlg->PrintLog(L"发:" + strcommand, 1);
		//dlg->SetRicheditText(L"发:" + strcommand, 0);
		if (countend == 3)
		{
			::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port1_ErrorInsert, NULL);
			//一直进入发送test指令，如果检测不到，那代表它已经断开了
			BOOL port1testflag = TRUE, port1failflag = TRUE;
			int port1testcount = 0;
			char port1teststr[100];
			memset(port1teststr, 0, sizeof(port1teststr) / sizeof(port1teststr[0]));
			DWORD port1testreadreal = 0, port1testdwBytesWrite, port1testdwErrorFlags;
			BOOL port1testbReadStat, port1testbWriteStat;
			CString port1teststrread, port1teststrcommand = L"AT^GT_CM=aging,off\r\n";
			COMSTAT port1testComStat;

			//老化失败后必须确保老化是关的，所以最好发一条老化关闭指令，接着再判断机子是连接着的还是中途被人直接拔掉了
			port1testbWriteStat = WriteFile(dlg->port1handler, CT2A(port1teststrcommand), port1teststrcommand.GetLength(), &port1testdwBytesWrite, NULL);
			Sleep(500);

			port1teststrcommand = L"AT^GT_CM=TEST\r\n";
			ClearCommError(dlg->port1handler, &port1testdwErrorFlags, &port1testComStat);

			do
			{
				port1testbWriteStat = WriteFile(dlg->port1handler, CT2A(port1teststrcommand), port1teststrcommand.GetLength(), &port1testdwBytesWrite, NULL);

				Sleep(90);
				port1testbReadStat = ReadFile(dlg->port1handler, port1teststr, 100, &port1testreadreal, 0);
				if (port1testbReadStat)
				{
					port1teststrread = port1teststr;
					if (port1teststrread.Find(L"TEST_OK") >= 0)
					{
						if (port1failflag == TRUE)
						{
							::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Fail, NULL);
							port1failflag = FALSE;
						}
						port1testcount = 0;
					}
					else
					{
						if (port1failflag == TRUE)
						{
							::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_AbnormolFail, NULL);
							port1failflag = FALSE;
							Port1Abnomal = TRUE;
						}
					}
				}
				port1testcount++;
				if (port1testcount == 5)
				{
					port1testflag = FALSE;
				}
				Sleep(10);
				memset(port1teststr, 0, sizeof(port1teststr) / sizeof(port1teststr[0]));
				dlg->PrintLog(L"发:" + port1teststrcommand, 1);
				PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
			} while (port1testflag);

			Sleep(100);
			m_Port1DownloadWrite4 = FALSE;
			m_Port1DownloadRead4 = FALSE;

			dlg->DownloadRestPort1Thread();
			return;
		}

		//每隔2秒发一次
		bWriteStat = WriteFile(dlg->port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(2000);
		countend++;

		if (s_bExit == TRUE || m_Port1DownloadControl == FALSE)
		{
			//strcommand = L"AT^GT_CM=reset,1\r\n";
			//ClearCommError(dlg->port1handler, &dwErrorFlags, &ComStat);
			//bWriteStat = WriteFile(dlg->port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
			//dlg->PrintLog(L"发:" + strcommand, 1);
			dlg->DownloadClosePort1Thread();
			dlg->DownloadRestPort1Thread();
			return;
		}

		if (m_Port1DownloadWrite4 == FALSE)
		{
			//一直进入发送test指令，如果检测不到，那代表它已经断开了
			BOOL port1testflag = TRUE;
			int port1testcount = 0;
			char port1teststr[100];
			memset(port1teststr, 0, sizeof(port1teststr) / sizeof(port1teststr[0]));
			DWORD port1testreadreal = 0, port1testdwBytesWrite,port1testdwErrorFlags;
			BOOL port1testbReadStat,port1testbWriteStat;
			CString port1teststrread, port1teststrcommand = L"AT^GT_CM=TEST\r\n";
			COMSTAT port1testComStat;

			ClearCommError(dlg->port1handler, &port1testdwErrorFlags, &port1testComStat);
			do
			{
				port1testbWriteStat = WriteFile(dlg->port1handler, CT2A(port1teststrcommand), port1teststrcommand.GetLength(), &port1testdwBytesWrite, NULL);

				Sleep(90);
				port1testbReadStat = ReadFile(dlg->port1handler, port1teststr, 100, &port1testreadreal, 0);
				if (port1testbReadStat)
				{
					port1teststrread = port1teststr;
					if (port1teststrread.Find(L"TEST_OK") >= 0)
					{
						port1testcount = 0;
					}
				}
				port1testcount++;
				if (port1testcount == 5)
				{
					port1testflag = FALSE;
				}
				Sleep(10);
				memset(port1teststr, 0, sizeof(port1teststr) / sizeof(port1teststr[0]));
				dlg->PrintLog(L"发:" + port1teststrcommand, 1);
				PurgeComm(dlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
			} while (port1testflag);
		}
	} while (m_Port1DownloadWrite4);

	dlg->DownloadRestPort1Thread();
	//strcommand = L"AT^GT_CM=reset,1\r\n";
	//ClearCommError(dlg->port1handler, &dwErrorFlags, &ComStat);
	//bWriteStat = WriteFile(dlg->port1handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
	//dlg->PrintLog(L"发:" + strcommand, 1);
}

//串口1的读老化命令的读线程
void CMFCP3SIMPORTDlg::DownloadRead4Port1Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	char str[100];
	memset(str, 0, sizeof(str) / sizeof(str[0]));
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread,strfolderpath, strfoldercut;

	m_Port1DownloadRead4 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程

	do
	{
		Sleep(300);
		bReadStat = ReadFile(dlg->port1handler, str, 100, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			if (strread.Find(L"aging,on\r\r\nOK!") >= 0)
			{
				::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port1_OkInsert, NULL);
				m_Port1DownloadRead4 = FALSE;
				m_Port1DownloadWrite4 = FALSE;
			}
			else
			{
				continue;
			}
		}
		if (s_bExit == TRUE || m_Port1DownloadControl == FALSE)
		{
			dlg->PrintLog(L"收:" + strread, 1);
			dlg->DownloadClosePort1Thread();
			dlg->DownloadRestPort1Thread();
			return;
		}
	} while (m_Port1DownloadRead4);
	dlg->PrintLog(L"收:" + strread, 1);
	//dlg->SetRicheditText(L"收:" + strread, 0);
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
	m_Port1DownloadReadEnd3 = FALSE;
	m_Port1DownloadRead4 = FALSE;
}


//串口2下载逻辑
//串口2的test写线程
void CMFCP3SIMPORTDlg::DownloadWrite1Port2Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	PurgeComm(dlg->port2handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;

	//int port2testcount = 0;

	//放指令用变量
	CString strcommand = L"AT^GT_CM=TEST\r\n";

	//一直test，看看串口有没有连接上
	if (Port2Abnomal == TRUE)//这里是如果出现异常插拔，就先停顿个2.5秒才继续往下跑
	{
		Sleep(2500);
	}

	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint2_Ready, NULL);//发消息，串口结果显示待测试

	m_Port2DownloadWrite1 = TRUE;

	ClearCommError(dlg->port2handler, &dwErrorFlags, &ComStat);

	Sleep(50);
	do
	{
		dlg->PrintLog(L"发:" + strcommand, 2);
		//dlg->SetRicheditText(L"发:" + strcommand, 0);
		bWriteStat = WriteFile(dlg->port2handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		//if (port2testcount == 1)
		//{
		//	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint2_Ready, NULL);
		//	//dlg->SetDlgItemText(IDC_PORT2HINT_STATIC, L"待测试");
		//}
		Sleep(2000);

		if (s_bExit == TRUE || m_Port2DownloadControl == FALSE)
		{
			dlg->DownloadClosePort2Thread();
			dlg->DownloadRestPort2Thread();
			return;
		}
		//port2testcount++;
	} while (m_Port2DownloadWrite1);

	Port2Abnomal = FALSE;//将异常标志位设置为FALSE

	return;
}

//串口2的test读线程
void CMFCP3SIMPORTDlg::DownloadRead1Port2Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	char str[100];
	memset(str, 0, sizeof(str) / sizeof(str[0]));
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;

	m_Port2DownloadRead1 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程
	do
	{
		Sleep(300);
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
	dlg->PrintLog(L"收:" + strread, 2);
	//dlg->SetRicheditText(L"收:" + strread, 0);
	PurgeComm(dlg->port2handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port2_Write2, NULL);
}

//串口2的写RID、IMEI的写线程
void CMFCP3SIMPORTDlg::DownloadWrite2Port2Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint2_Connected, NULL);

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
		PurgeComm(dlg->port2handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

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
			if (count == InteverIMEIRIDCount)
			{
				//一直进入发送test指令，如果检测不到，那代表它已经断开了，首先设置一堆放各种读写的变量
				BOOL port2testflag = TRUE, port2failflag = TRUE;
				int port2testcount = 0;
				char port2teststr[100];
				memset(port2teststr, 0, sizeof(port2teststr) / sizeof(port2teststr[0]));
				DWORD port2testreadreal = 0, port2testdwBytesWrite, port2testdwErrorFlags;
				BOOL port2testbReadStat, port2testbWriteStat;
				CString port2teststrread, port2teststrcommand = L"AT^GT_CM=TEST\r\n";
				COMSTAT port2testComStat;

				ClearCommError(dlg->port2handler, &port2testdwErrorFlags, &port2testComStat);
				//这里开始检测机子是否还在连接
				do
				{
					port2testbWriteStat = WriteFile(dlg->port2handler, CT2A(port2teststrcommand), port2teststrcommand.GetLength(), &port2testdwBytesWrite, NULL);//写指令

					Sleep(90);
					port2testbReadStat = ReadFile(dlg->port2handler, port2teststr, 100, &port2testreadreal, 0);//读指令

					//判断读到的东西，如果连续五次没读到回复，那就代表机子已经被干掉了，而不是我们读数据没反应，如果机子有反应，我们还读不到数据，那就代表机子异常得重启一下
					if (port2testbReadStat)
					{
						port2teststrread = port2teststr;

						if (port2teststrread.Find(L"TEST_OK") >= 0)
						{
							if (port2failflag == TRUE)
							{
								::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint2_READFail, NULL);
								port2failflag = FALSE;
							}
							port2testcount = 0;
						}
					}

					port2testcount++;

					//连续读五次没反应就退出
					if (port2testcount == 5)
					{
						port2testflag = FALSE;
					}

					Sleep(10);

					memset(port2teststr, 0, sizeof(port2teststr) / sizeof(port2teststr[0]));

					dlg->PrintLog(L"发:" + port2teststrcommand, 2);

					PurgeComm(dlg->port2handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
				} while (port2testflag);
				//清空一下编辑框
				dlg->SetDlgItemText(IDC_PORT2RID_EDIT, L"");
				dlg->SetDlgItemText(IDC_PORT2IMEI_EDIT, L"");

				dlg->DownloadClosePort2Thread();
				dlg->DownloadRestPort2Thread();
				return;
			}

			dlg->PrintLog(L"发:" + strcommand, 2);
			//dlg->SetRicheditText(L"发:" + strcommand, 0);
			bWriteStat = WriteFile(dlg->port2handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
			count++;

			Sleep(InteverIMEIRIDTime);

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
	memset(str, 0, sizeof(str) / sizeof(str[0]));
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
			Sleep(200);
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
							strcount.Replace(LPCTSTR(" "), LPCTSTR(""));
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
						if (strcount != ""&&strcount != "^"&&strcounttemp.Trim(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789").GetLength() == 0)
						{
							dlg->SetDlgItemText(IDC_PORT2IMEI_EDIT, strcount);

							int port2flag;
							CString strport2RID, strport2IMEI;
							ADOManage adoport2manage4;
							adoport2manage4.ConnSQL();

							dlg->GetDlgItemText(IDC_PORT2RID_EDIT, strport2RID);
							dlg->GetDlgItemText(IDC_PORT2IMEI_EDIT, strport2IMEI);

							port2flag = adoport2manage4.SimDataLastStationSql(strport2IMEI);
							if (port2flag == 0)
							{
								m_Port2DownloadWrite2 = FALSE;
								m_Port2DownloadRead2 = FALSE;
								m_Port2DownloadReadEnd2 = FALSE;
								dlg->PrintLog(L"耦合漏测", 2);
								dlg->SetDlgItemText(IDC_PORT2HINT_STATIC, L"耦合漏测");
								adoport2manage4.CloseAll();

								//一直进入发送test指令，如果检测不到，那代表它已经断开了
								BOOL port2testflag = TRUE;
								int port2testcount = 0;
								char port2teststr[100];
								memset(port2teststr, 0, sizeof(port2teststr) / sizeof(port2teststr[0]));
								DWORD port2testreadreal = 0, port2testdwBytesWrite, port2testdwErrorFlags;
								BOOL port2testbReadStat, port2testbWriteStat;
								CString port2teststrread, port2teststrcommand = L"AT^GT_CM=TEST\r\n";
								COMSTAT port2testComStat;

								ClearCommError(dlg->port2handler, &port2testdwErrorFlags, &port2testComStat);
								do
								{
									port2testbWriteStat = WriteFile(dlg->port2handler, CT2A(port2teststrcommand), port2teststrcommand.GetLength(), &port2testdwBytesWrite, NULL);

									Sleep(90);
									port2testbReadStat = ReadFile(dlg->port2handler, port2teststr, 100, &port2testreadreal, 0);
									if (port2testbReadStat)
									{
										port2teststrread = port2teststr;
										if (port2teststrread.Find(L"TEST_OK") >= 0)
										{
											port2testcount = 0;
										}
									}
									port2testcount++;
									if (port2testcount == 5)
									{
										port2testflag = FALSE;
									}
									Sleep(10);
									memset(port2teststr, 0, sizeof(port2teststr) / sizeof(port2teststr[0]));
									dlg->PrintLog(L"发:" + port2teststrcommand, 2);
									PurgeComm(dlg->port2handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
								} while (port2testflag);

								Sleep(50);
								dlg->SetDlgItemText(IDC_PORT2RID_EDIT, L"");
								dlg->SetDlgItemText(IDC_PORT2IMEI_EDIT, L"");
								dlg->DownloadClosePort2Thread();
								dlg->DownloadRestPort2Thread();
								return ;
							}

							port2flag = adoport2manage4.SimDataIsExitSql(strport2RID, strport2IMEI);
							if (port2flag == 0)
							{
								m_Port2DownloadWrite2 = FALSE;
								m_Port2DownloadRead2 = FALSE;
								m_Port2DownloadReadEnd2 = FALSE;
								dlg->PrintLog(L"工位已测", 2);
								dlg->SetDlgItemText(IDC_PORT2HINT_STATIC, L"工位已测");
								adoport2manage4.CloseAll();

								//一直进入发送test指令，如果检测不到，那代表它已经断开了
								BOOL port2testflag = TRUE;
								int port2testcount = 0;
								char port2teststr[100];
								memset(port2teststr, 0, sizeof(port2teststr) / sizeof(port2teststr[0]));
								DWORD port2testreadreal = 0, port2testdwBytesWrite, port2testdwErrorFlags;
								BOOL port2testbReadStat, port2testbWriteStat;
								CString port2teststrread, port2teststrcommand = L"AT^GT_CM=TEST\r\n";
								COMSTAT port2testComStat;

								ClearCommError(dlg->port2handler, &port2testdwErrorFlags, &port2testComStat);
								do
								{
									port2testbWriteStat = WriteFile(dlg->port2handler, CT2A(port2teststrcommand), port2teststrcommand.GetLength(), &port2testdwBytesWrite, NULL);

									Sleep(90);
									port2testbReadStat = ReadFile(dlg->port2handler, port2teststr, 100, &port2testreadreal, 0);
									if (port2testbReadStat)
									{
										port2teststrread = port2teststr;
										if (port2teststrread.Find(L"TEST_OK") >= 0)
										{
											port2testcount = 0;
										}
									}
									port2testcount++;
									if (port2testcount == 5)
									{
										port2testflag = FALSE;
									}
									Sleep(10);
									memset(port2teststr, 0, sizeof(port2teststr) / sizeof(port2teststr[0]));
									dlg->PrintLog(L"发:" + port2teststrcommand, 2);
									PurgeComm(dlg->port2handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
								} while (port2testflag);

								Sleep(50);
								dlg->SetDlgItemText(IDC_PORT2RID_EDIT, L"");
								dlg->SetDlgItemText(IDC_PORT2IMEI_EDIT, L"");
								dlg->DownloadClosePort2Thread();
								dlg->DownloadRestPort2Thread();
								return ;
							}
							//LastPort2RID = strport2RID;
							//LastPort2IMEI = strport2IMEI;
							adoport2manage4.CloseAll();
							//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port2_IsExit, NULL);
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
		dlg->PrintLog(L"收:" + strread, 2);
		//dlg->SetRicheditText(L"收:" + strread, 0);
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
	CFile ReadFile2;
	BOOL bOpen = ReadFile2.Open(StrFolder[1], CFile::modeRead);
	dwTotalSize = ReadFile2.GetLength();//获取文件总长度

	//开始写文件
	strcommand = L"AT^GT_CM=SOFTSIM_DATA,";

	//数据缓冲区
	char *pBuf = new char[dwStep + 1];
	memset(pBuf, '\0', dwStep + 1);

	ClearCommError(dlg->port2handler, &dwErrorFlags, &ComStat);

	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint2_Downloading, NULL);
	//PurgeComm(dlg->port2handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//这个While循环是用来发送文件数据的
	while ((dwRead = ReadFile2.Read(pBuf, dwStep)) > 0)
	{
		PurgeComm(dlg->port2handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
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
		dlg->PrintLog(L"发:" + strfilewritecommand, 2);
		//dlg->SetRicheditText(L"发:" + strfilewritecommand, 0);
		dwBytesWrite = strcommand.GetLength();
		bWriteStat = WriteFile(dlg->port2handler, CT2A(strfilewritecommand), strfilewritecommand.GetLength(), &dwBytesWrite, NULL);

		intdatano++;
		memset(pBuf, '\0', dwStep + 1);
		delete[] buf1;
		buf1 = NULL;
		Sleep(650);
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
		if (countend == 3)
		{
			::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port2_ErrorInsert, NULL);//先插入失败记录

			//一直进入发送test指令，如果检测不到，那代表它已经断开了
			BOOL port2testflag = TRUE, port2failflag = TRUE;
			int port2testcount = 0;
			char port2teststr[100];
			memset(port2teststr, 0, sizeof(port2teststr) / sizeof(port2teststr[0]));
			DWORD port2testreadreal = 0, port2testdwBytesWrite, port2testdwErrorFlags;
			BOOL port2testbReadStat, port2testbWriteStat;
			CString port2teststrread, port2teststrcommand = L"AT^GT_CM=TEST\r\n";
			COMSTAT port2testComStat;

			ClearCommError(dlg->port2handler, &port2testdwErrorFlags, &port2testComStat);
			do
			{
				port2testbWriteStat = WriteFile(dlg->port2handler, CT2A(port2teststrcommand), port2teststrcommand.GetLength(), &port2testdwBytesWrite, NULL);

				Sleep(90);
				port2testbReadStat = ReadFile(dlg->port2handler, port2teststr, 100, &port2testreadreal, 0);
				if (port2testbReadStat)
				{
					port2teststrread = port2teststr;
					if (port2teststrread.Find(L"TEST_OK") >= 0)
					{
						if (port2failflag == TRUE)
						{
							::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint2_Fail, NULL);
							port2failflag = FALSE;
						}
						port2testcount = 0;
					}
					else//这里是如果一开始就读不到机子，那肯定是被人中途拔掉了，这是异常也就是违规操作
					{
						if (port2failflag == TRUE)
						{
							::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint2_AbnormolFail, NULL);
							port2failflag = FALSE;
							Port2Abnomal = TRUE;
						}
					}
				}

				port2testcount++;

				if (port2testcount == 5)
				{
					port2testflag = FALSE;
				}

				Sleep(10);
				memset(port2teststr, 0, sizeof(port2teststr) / sizeof(port2teststr[0]));
				dlg->PrintLog(L"发:" + port2teststrcommand, 2);
				PurgeComm(dlg->port2handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
			} while (port2testflag);

			dlg->SetDlgItemText(IDC_PORT2RID_EDIT, L"");
			dlg->SetDlgItemText(IDC_PORT2IMEI_EDIT, L"");
			dlg->DownloadClosePort2Thread();
			dlg->DownloadRestPort2Thread();
			return;
		}

		dlg->PrintLog(L"发:" + strcommand, 2);
		//dlg->SetRicheditText(L"发:" + strcommand, 0);
		bWriteStat = WriteFile(dlg->port2handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(1500);
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
	memset(str, 0, sizeof(str) / sizeof(str[0]));;
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;

	m_Port2DownloadRead3 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程
	m_Port2DownloadReadEnd3 = TRUE;

	do
	{
		Sleep(300);
		bReadStat = ReadFile(dlg->port2handler, str, 100, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			if (strread.Find(L"SoftSim,") >= 0)
			{
				if (strread.Find(L"SoftSim,OK") >= 0)
				{
					//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port2_OkInsert, NULL);
				}
				else if (strread.Find(L"Error") >= 0)
				{
					::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port2_ErrorInsert, NULL);
					m_Port2DownloadWrite3 = FALSE;
					m_Port2DownloadRead3 = FALSE;
					m_Port2DownloadReadEnd3 = FALSE;

					//一直进入发送test指令，如果检测不到，那代表它已经断开了
					BOOL port2testflag = TRUE, port2failflag = TRUE;
					int port2testcount = 0;
					char port2teststr[100];
					memset(port2teststr, 0, sizeof(port2teststr) / sizeof(port2teststr[0]));
					DWORD port2testreadreal = 0, port2testdwBytesWrite, port2testdwErrorFlags;
					BOOL port2testbReadStat, port2testbWriteStat;
					CString port2teststrread, port2teststrcommand = L"AT^GT_CM=TEST\r\n";
					COMSTAT port2testComStat;

					ClearCommError(dlg->port2handler, &port2testdwErrorFlags, &port2testComStat);
					do
					{
						port2testbWriteStat = WriteFile(dlg->port2handler, CT2A(port2teststrcommand), port2teststrcommand.GetLength(), &port2testdwBytesWrite, NULL);

						Sleep(90);
						port2testbReadStat = ReadFile(dlg->port2handler, port2teststr, 100, &port2testreadreal, 0);
						if (port2testbReadStat)
						{
							port2teststrread = port2teststr;
							if (port2teststrread.Find(L"TEST_OK") >= 0)
							{
								if (port2failflag == TRUE)
								{
									::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint2_Fail, NULL);
									port2failflag = FALSE;
								}
								port2testcount = 0;
							}
							//else
							//{
							//	if (port2failflag == TRUE)
							//	{
							//		::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint2_AbnormolFail, NULL);
							//		port2failflag = FALSE;
							//		Port2Abnomal = TRUE;
							//	}
							//}
						}
						port2testcount++;
						if (port2testcount == 5)
						{
							port2testflag = FALSE;
						}
						Sleep(10);
						memset(port2teststr, 0, sizeof(port2teststr) / sizeof(port2teststr[0]));
						dlg->PrintLog(L"发:" + port2teststrcommand, 2);
						PurgeComm(dlg->port2handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
					} while (port2testflag);

					Sleep(100);
					dlg->PrintLog(L"收:" + strread, 2);
					//dlg->SetRicheditText(L"收:" + strread, 0);
					dlg->DownloadRestPort2Thread();
					return;
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
	if (m_Port2DownloadReadEnd3 == FALSE)
	{
		return;
	}
	dlg->PrintLog(L"收:" + strread, 2);
	//dlg->SetRicheditText(L"收:" + strread, 0);
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
		if (countend == 3)
		{
			::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port2_ErrorInsert, NULL);
			//一直进入发送test指令，如果检测不到，那代表它已经断开了
			BOOL port2testflag = TRUE, port2failflag = TRUE;
			int port2testcount = 0;
			char port2teststr[100];
			memset(port2teststr, 0, sizeof(port2teststr) / sizeof(port2teststr[0]));
			DWORD port2testreadreal = 0, port2testdwBytesWrite, port2testdwErrorFlags;
			BOOL port2testbReadStat, port2testbWriteStat;
			CString port2teststrread, port2teststrcommand = L"AT^GT_CM=aging,off\r\n";
			COMSTAT port2testComStat;

			//老化失败后必须确保老化是关的，所以最好发一条老化关闭指令，接着再判断机子是连接着的还是中途被人直接拔掉了
			port2testbWriteStat = WriteFile(dlg->port2handler, CT2A(port2teststrcommand), port2teststrcommand.GetLength(), &port2testdwBytesWrite, NULL);
			Sleep(500);

			port2teststrcommand = L"AT^GT_CM=TEST\r\n";
			ClearCommError(dlg->port2handler, &port2testdwErrorFlags, &port2testComStat);

			do
			{
				port2testbWriteStat = WriteFile(dlg->port2handler, CT2A(port2teststrcommand), port2teststrcommand.GetLength(), &port2testdwBytesWrite, NULL);

				Sleep(90);
				port2testbReadStat = ReadFile(dlg->port2handler, port2teststr, 100, &port2testreadreal, 0);
				if (port2testbReadStat)
				{
					port2teststrread = port2teststr;
					if (port2teststrread.Find(L"TEST_OK") >= 0)
					{
						if (port2failflag == TRUE)
						{
							::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint2_Fail, NULL);
							port2failflag = FALSE;
						}
						port2testcount = 0;
					}
					else
					{
						if (port2failflag == TRUE)
						{
							::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint2_AbnormolFail, NULL);
							port2failflag = FALSE;
							Port2Abnomal = TRUE;
						}
					}
				}
				port2testcount++;
				if (port2testcount == 5)
				{
					port2testflag = FALSE;
				}
				Sleep(10);
				memset(port2teststr, 0, sizeof(port2teststr) / sizeof(port2teststr[0]));
				dlg->PrintLog(L"发:" + port2teststrcommand, 2);
				PurgeComm(dlg->port2handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
			} while (port2testflag);

			Sleep(100);
			m_Port2DownloadWrite4 = FALSE;
			m_Port2DownloadRead4 = FALSE;

			dlg->DownloadRestPort2Thread();
			return;
		}

		dlg->PrintLog(L"发:" + strcommand, 2);
		//dlg->SetRicheditText(L"发:" + strcommand, 0);
		bWriteStat = WriteFile(dlg->port2handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(2000);
		countend++;
		if (s_bExit == TRUE || m_Port2DownloadControl == FALSE)
		{
			//strcommand = L"AT^GT_CM=reset,1\r\n";
			//ClearCommError(dlg->port2handler, &dwErrorFlags, &ComStat);
			//bWriteStat = WriteFile(dlg->port2handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
			//dlg->PrintLog(L"发:" + strcommand, 2);
			dlg->DownloadClosePort2Thread();
			dlg->DownloadRestPort2Thread();
			return;
		}

		if (m_Port2DownloadWrite4 == FALSE)
		{
			//一直进入发送test指令，如果检测不到，那代表它已经断开了
			BOOL port2testflag = TRUE;
			int port2testcount = 0;
			char port2teststr[100];
			memset(port2teststr, 0, sizeof(port2teststr) / sizeof(port2teststr[0]));
			DWORD port2testreadreal = 0, port2testdwBytesWrite, port2testdwErrorFlags;
			BOOL port2testbReadStat, port2testbWriteStat;
			CString port2teststrread, port2teststrcommand = L"AT^GT_CM=TEST\r\n";
			COMSTAT port2testComStat;

			ClearCommError(dlg->port2handler, &port2testdwErrorFlags, &port2testComStat);
			do
			{
				port2testbWriteStat = WriteFile(dlg->port2handler, CT2A(port2teststrcommand), port2teststrcommand.GetLength(), &port2testdwBytesWrite, NULL);

				Sleep(90);
				port2testbReadStat = ReadFile(dlg->port2handler, port2teststr, 100, &port2testreadreal, 0);
				if (port2testbReadStat)
				{
					port2teststrread = port2teststr;
					if (port2teststrread.Find(L"TEST_OK") >= 0)
					{
						port2testcount = 0;
					}
				}
				port2testcount++;
				if (port2testcount == 5)
				{
					port2testflag = FALSE;
				}
				Sleep(10);
				memset(port2teststr, 0, sizeof(port2teststr) / sizeof(port2teststr[0]));
				dlg->PrintLog(L"发:" + port2teststrcommand, 2);
				PurgeComm(dlg->port2handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
			} while (port2testflag);
		}

	} while (m_Port2DownloadWrite4);

	dlg->DownloadRestPort2Thread();

	//strcommand = L"AT^GT_CM=reset,1\r\n";
	//ClearCommError(dlg->port2handler, &dwErrorFlags, &ComStat);
	//bWriteStat = WriteFile(dlg->port2handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
	//dlg->PrintLog(L"发:" + strcommand, 2);
}

//串口2的读老化命令的读线程
void CMFCP3SIMPORTDlg::DownloadRead4Port2Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	char str[100];
	memset(str, 0, sizeof(str) / sizeof(str[0]));;
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread, strfolderpath, strfoldercut;;

	m_Port2DownloadRead4 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程

	do
	{
		Sleep(300);
		bReadStat = ReadFile(dlg->port2handler, str, 100, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			if (strread.Find(L"aging,on\r\r\nOK!") >= 0)
			{

				::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port2_OkInsert, NULL);
				m_Port2DownloadRead4 = FALSE;
				m_Port2DownloadWrite4 = FALSE;
			}
			else
			{
				continue;
			}
		}
		if (s_bExit == TRUE || m_Port2DownloadControl == FALSE)
		{
			dlg->PrintLog(L"收:" + strread, 2);
			dlg->DownloadClosePort2Thread();
			dlg->DownloadRestPort2Thread();
			return;
		}
	} while (m_Port2DownloadRead4);
	dlg->PrintLog(L"收:" + strread, 2);
	//dlg->SetRicheditText(L"收:" + strread, 0);
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
	m_Port2DownloadReadEnd3 = FALSE;
	m_Port2DownloadRead4 = FALSE;
}


//串口3下载逻辑
//串口3的test写线程
void CMFCP3SIMPORTDlg::DownloadWrite1Port3Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	PurgeComm(dlg->port3handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	//int port3testcount = 0;

	//放指令用变量
	CString strcommand = L"AT^GT_CM=TEST\r\n";

	//一直test，看看串口有没有连接上
	if (Port3Abnomal == TRUE)//这里是如果出现异常插拔，就先停顿个2.5秒才继续往下跑
	{
		Sleep(2500);
	}

	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint3_Ready, NULL);//发消息，串口结果显示待测试


	m_Port3DownloadWrite1 = TRUE;

	ClearCommError(dlg->port3handler, &dwErrorFlags, &ComStat);
	Sleep(50);
	do
	{
		dlg->PrintLog(L"发:" + strcommand, 3);
		//dlg->SetRicheditText(L"发:" + strcommand, 0);
		bWriteStat = WriteFile(dlg->port3handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		//if (port3testcount == 1)
		//{
		//	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint3_Ready, NULL);
		//	//dlg->SetDlgItemText(IDC_PORT3HINT_STATIC, L"待测试");
		//}
		Sleep(2000);

		if (s_bExit == TRUE || m_Port3DownloadControl == FALSE)
		{
			dlg->DownloadClosePort3Thread();
			dlg->DownloadRestPort3Thread();
			return;
		}
		//port3testcount++;
	} while (m_Port3DownloadWrite1);

	Port3Abnomal = FALSE;//将异常标志位设置为FALSE
	return;
}

//串口3的test读线程
void CMFCP3SIMPORTDlg::DownloadRead1Port3Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	//串口变量
	char str[100];
	memset(str, 0, sizeof(str) / sizeof(str[0]));;
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;

	m_Port3DownloadRead1 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程
	do
	{
		Sleep(300);
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
	dlg->PrintLog(L"收:" + strread, 3);
	//dlg->SetRicheditText(L"收:" + strread, 0);
	PurgeComm(dlg->port3handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port3_Write2, NULL);
}

//串口3的写RID、IMEI的写线程
void CMFCP3SIMPORTDlg::DownloadWrite2Port3Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint3_Connected, NULL);

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
	PurgeComm(dlg->port3handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

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

			if (count == InteverIMEIRIDCount)
			{
				//一直进入发送test指令，如果检测不到，那代表它已经断开了，首先设置一堆放各种读写的变量
				BOOL port3testflag = TRUE, port3failflag = TRUE;
				int port3testcount = 0;
				char port3teststr[100];
				memset(port3teststr, 0, sizeof(port3teststr) / sizeof(port3teststr[0]));
				DWORD port3testreadreal = 0, port3testdwBytesWrite, port3testdwErrorFlags;
				BOOL port3testbReadStat, port3testbWriteStat;
				CString port3teststrread, port3teststrcommand = L"AT^GT_CM=TEST\r\n";
				COMSTAT port3testComStat;

				ClearCommError(dlg->port3handler, &port3testdwErrorFlags, &port3testComStat);
				//这里开始检测机子是否还在连接
				do
				{
					port3testbWriteStat = WriteFile(dlg->port3handler, CT2A(port3teststrcommand), port3teststrcommand.GetLength(), &port3testdwBytesWrite, NULL);//写指令

					Sleep(90);
					port3testbReadStat = ReadFile(dlg->port3handler, port3teststr, 100, &port3testreadreal, 0);//读指令

					//判断读到的东西，如果连续五次没读到回复，那就代表机子已经被干掉了，而不是我们读数据没反应，如果机子有反应，我们还读不到数据，那就代表机子异常得重启一下
					if (port3testbReadStat)
					{
						port3teststrread = port3teststr;

						if (port3teststrread.Find(L"TEST_OK") >= 0)
						{
							if (port3failflag == TRUE)
							{
								::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint3_READFail, NULL);
								port3failflag = FALSE;
							}
							port3testcount = 0;
						}
					}

					port3testcount++;

					//连续读五次没反应就退出
					if (port3testcount == 5)
					{
						port3testflag = FALSE;
					}

					Sleep(10);

					memset(port3teststr, 0, sizeof(port3teststr) / sizeof(port3teststr[0]));

					dlg->PrintLog(L"发:" + port3teststrcommand, 3);

					PurgeComm(dlg->port3handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
				} while (port3testflag);
				//清空一下编辑框
				dlg->SetDlgItemText(IDC_PORT3RID_EDIT, L"");
				dlg->SetDlgItemText(IDC_PORT3IMEI_EDIT, L"");

				dlg->DownloadClosePort3Thread();
				dlg->DownloadRestPort3Thread();
				return;
			}

			dlg->PrintLog(L"发:" + strcommand, 3);
			//dlg->SetRicheditText(L"发:" + strcommand, 0);
			bWriteStat = WriteFile(dlg->port3handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
			count++;

			Sleep(InteverIMEIRIDTime);
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
	memset(str, 0, sizeof(str) / sizeof(str[0]));;
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
			Sleep(200);

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
							strcount.Replace(LPCTSTR(" "), LPCTSTR(""));
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
						if (strcount != ""&&strcount != "^"&&strcounttemp.Trim(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789").GetLength() == 0)
						{
							dlg->SetDlgItemText(IDC_PORT3IMEI_EDIT, strcount);

							int port3flag;
							CString strport3RID, strport3IMEI;
							ADOManage adoport3manage5;
							adoport3manage5.ConnSQL();

							dlg->GetDlgItemText(IDC_PORT3RID_EDIT, strport3RID);
							dlg->GetDlgItemText(IDC_PORT3IMEI_EDIT, strport3IMEI);

							port3flag = adoport3manage5.SimDataLastStationSql(strport3IMEI);

							if (port3flag == 0)
							{
								m_Port3DownloadWrite2 = FALSE;
								m_Port3DownloadRead2 = FALSE;
								m_Port3DownloadReadEnd2 = FALSE;
								dlg->PrintLog(L"耦合漏测", 3);
								dlg->SetDlgItemText(IDC_PORT3HINT_STATIC, L"耦合漏测");
								adoport3manage5.CloseAll();

								//一直进入发送test指令，如果检测不到，那代表它已经断开了
								BOOL port3testflag = TRUE;
								int port3testcount = 0;
								char port3teststr[100];
								memset(port3teststr, 0, sizeof(port3teststr) / sizeof(port3teststr[0]));
								DWORD port3testreadreal = 0, port3testdwBytesWrite, port3testdwErrorFlags;
								BOOL port3testbReadStat, port3testbWriteStat;
								CString port3teststrread, port3teststrcommand = L"AT^GT_CM=TEST\r\n";
								COMSTAT port3testComStat;

								ClearCommError(dlg->port3handler, &port3testdwErrorFlags, &port3testComStat);
								do
								{
									port3testbWriteStat = WriteFile(dlg->port3handler, CT2A(port3teststrcommand), port3teststrcommand.GetLength(), &port3testdwBytesWrite, NULL);

									Sleep(90);
									port3testbReadStat = ReadFile(dlg->port3handler, port3teststr, 100, &port3testreadreal, 0);
									if (port3testbReadStat)
									{
										port3teststrread = port3teststr;
										if (port3teststrread.Find(L"TEST_OK") >= 0)
										{
											port3testcount = 0;
										}
									}
									port3testcount++;
									if (port3testcount == 5)
									{
										port3testflag = FALSE;
									}
									Sleep(10);
									memset(port3teststr, 0, sizeof(port3teststr) / sizeof(port3teststr[0]));
									dlg->PrintLog(L"发:" + port3teststrcommand, 3);
									PurgeComm(dlg->port3handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
								} while (port3testflag);

								Sleep(50);
								dlg->SetDlgItemText(IDC_PORT3RID_EDIT, L"");
								dlg->SetDlgItemText(IDC_PORT3IMEI_EDIT, L"");
								dlg->DownloadClosePort3Thread();
								dlg->DownloadRestPort3Thread();
								return ;
							}

							port3flag = adoport3manage5.SimDataIsExitSql(strport3RID, strport3IMEI);

							if (port3flag == 0)
							{
								m_Port3DownloadWrite2 = FALSE;
								m_Port3DownloadRead2 = FALSE;
								m_Port3DownloadReadEnd2 = FALSE;
								dlg->PrintLog(L"工位已测", 3);
								dlg->SetDlgItemText(IDC_PORT3HINT_STATIC, L"工位已测");
								adoport3manage5.CloseAll();

								//一直进入发送test指令，如果检测不到，那代表它已经断开了
								BOOL port3testflag = TRUE;
								int port3testcount = 0;
								char port3teststr[100];
								memset(port3teststr, 0, sizeof(port3teststr) / sizeof(port3teststr[0]));
								DWORD port3testreadreal = 0, port3testdwBytesWrite, port3testdwErrorFlags;
								BOOL port3testbReadStat, port3testbWriteStat;
								CString port3teststrread, port3teststrcommand = L"AT^GT_CM=TEST\r\n";
								COMSTAT port3testComStat;

								ClearCommError(dlg->port3handler, &port3testdwErrorFlags, &port3testComStat);
								do
								{
									port3testbWriteStat = WriteFile(dlg->port3handler, CT2A(port3teststrcommand), port3teststrcommand.GetLength(), &port3testdwBytesWrite, NULL);

									Sleep(90);
									port3testbReadStat = ReadFile(dlg->port3handler, port3teststr, 100, &port3testreadreal, 0);
									if (port3testbReadStat)
									{
										port3teststrread = port3teststr;
										if (port3teststrread.Find(L"TEST_OK") >= 0)
										{
											port3testcount = 0;
										}
									}
									port3testcount++;
									if (port3testcount == 5)
									{
										port3testflag = FALSE;
									}
									Sleep(10);
									memset(port3teststr, 0, sizeof(port3teststr) / sizeof(port3teststr[0]));
									dlg->PrintLog(L"发:" + port3teststrcommand, 3);
									PurgeComm(dlg->port3handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
								} while (port3testflag);


								Sleep(50);
								dlg->SetDlgItemText(IDC_PORT3RID_EDIT, L"");
								dlg->SetDlgItemText(IDC_PORT3IMEI_EDIT, L"");
								dlg->DownloadClosePort3Thread();
								dlg->DownloadRestPort3Thread();
								return ;
							}
							//LastPort3RID = strport3RID;
							//LastPort3IMEI = strport3IMEI;
							adoport3manage5.CloseAll();
							//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port3_IsExit, NULL);
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
		dlg->PrintLog(L"收:" + strread, 3);
		//dlg->SetRicheditText(L"收:" + strread, 0);
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
	CFile ReadFile3;
	BOOL bOpen = ReadFile3.Open(StrFolder[2], CFile::modeRead);
	dwTotalSize = ReadFile3.GetLength();//获取文件总长度

	//开始写文件
	strcommand = L"AT^GT_CM=SOFTSIM_DATA,";

	//数据缓冲区
	char *pBuf = new char[dwStep + 1];
	memset(pBuf, '\0', dwStep + 1);

	ClearCommError(dlg->port3handler, &dwErrorFlags, &ComStat);

	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint3_Downloading, NULL);
	//PurgeComm(dlg->port3handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//这个While循环是用来发送文件数据的
	while ((dwRead = ReadFile3.Read(pBuf, dwStep)) > 0)
	{
		PurgeComm(dlg->port3handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
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
		dlg->PrintLog(L"发:" + strfilewritecommand, 3);
		//dlg->SetRicheditText(L"发:" + strfilewritecommand, 0);
		dwBytesWrite = strcommand.GetLength();
		bWriteStat = WriteFile(dlg->port3handler, CT2A(strfilewritecommand), strfilewritecommand.GetLength(), &dwBytesWrite, NULL);

		intdatano++;
		memset(pBuf, '\0', dwStep + 1);
		delete[] buf1;
		buf1 = NULL;
		Sleep(650);
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
		if (countend == 3)
		{
			::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port3_ErrorInsert, NULL);//先插入失败记录

			//一直进入发送test指令，如果检测不到，那代表它已经断开了
			BOOL port3testflag = TRUE, port3failflag = TRUE;
			int port3testcount = 0;
			char port3teststr[100];
			memset(port3teststr, 0, sizeof(port3teststr) / sizeof(port3teststr[0]));
			DWORD port3testreadreal = 0, port3testdwBytesWrite, port3testdwErrorFlags;
			BOOL port3testbReadStat, port3testbWriteStat;
			CString port3teststrread, port3teststrcommand = L"AT^GT_CM=TEST\r\n";
			COMSTAT port3testComStat;

			ClearCommError(dlg->port3handler, &port3testdwErrorFlags, &port3testComStat);
			do
			{
				port3testbWriteStat = WriteFile(dlg->port3handler, CT2A(port3teststrcommand), port3teststrcommand.GetLength(), &port3testdwBytesWrite, NULL);

				Sleep(90);
				port3testbReadStat = ReadFile(dlg->port3handler, port3teststr, 100, &port3testreadreal, 0);
				if (port3testbReadStat)
				{
					port3teststrread = port3teststr;
					if (port3teststrread.Find(L"TEST_OK") >= 0)
					{
						if (port3failflag == TRUE)
						{
							::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint3_Fail, NULL);
							port3failflag = FALSE;
						}
						port3testcount = 0;
					}
					else//这里是如果一开始就读不到机子，那肯定是被人中途拔掉了，这是异常也就是违规操作
					{
						if (port3failflag == TRUE)
						{
							::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint3_AbnormolFail, NULL);
							port3failflag = FALSE;
							Port3Abnomal = TRUE;
						}
					}
				}

				port3testcount++;

				if (port3testcount == 5)
				{
					port3testflag = FALSE;
				}

				Sleep(10);
				memset(port3teststr, 0, sizeof(port3teststr) / sizeof(port3teststr[0]));
				dlg->PrintLog(L"发:" + port3teststrcommand, 3);
				PurgeComm(dlg->port3handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
			} while (port3testflag);

			dlg->SetDlgItemText(IDC_PORT3RID_EDIT, L"");
			dlg->SetDlgItemText(IDC_PORT3IMEI_EDIT, L"");
			dlg->DownloadClosePort3Thread();
			dlg->DownloadRestPort3Thread();
			return;
		}

		dlg->PrintLog(L"发:" + strcommand, 3);
		//dlg->SetRicheditText(L"发:" + strcommand, 0);
		bWriteStat = WriteFile(dlg->port3handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(1500);
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
	memset(str, 0, sizeof(str) / sizeof(str[0]));;
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;

	m_Port3DownloadRead3 = TRUE;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程
	m_Port3DownloadReadEnd3 = TRUE;
	do
	{
		Sleep(300);
		bReadStat = ReadFile(dlg->port3handler, str, 100, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			if (strread.Find(L"SoftSim,") >= 0)
			{
				if (strread.Find(L"SoftSim,OK") >= 0)
				{

					//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port3_OkInsert, NULL);
				}
				else if (strread.Find(L"Error") >= 0)
				{
					::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port3_ErrorInsert, NULL);
					m_Port3DownloadWrite3 = FALSE;
					m_Port3DownloadRead3 = FALSE;
					m_Port3DownloadReadEnd3 = FALSE;

					//一直进入发送test指令，如果检测不到，那代表它已经断开了
					BOOL port3testflag = TRUE, port3failflag = TRUE;
					int port3testcount = 0;
					char port3teststr[100];
					memset(port3teststr, 0, sizeof(port3teststr) / sizeof(port3teststr[0]));
					DWORD port3testreadreal = 0, port3testdwBytesWrite, port3testdwErrorFlags;
					BOOL port3testbReadStat, port3testbWriteStat;
					CString port3teststrread, port3teststrcommand = L"AT^GT_CM=TEST\r\n";
					COMSTAT port3testComStat;

					ClearCommError(dlg->port3handler, &port3testdwErrorFlags, &port3testComStat);
					do
					{
						port3testbWriteStat = WriteFile(dlg->port3handler, CT2A(port3teststrcommand), port3teststrcommand.GetLength(), &port3testdwBytesWrite, NULL);

						Sleep(90);
						port3testbReadStat = ReadFile(dlg->port3handler, port3teststr, 100, &port3testreadreal, 0);
						if (port3testbReadStat)
						{
							port3teststrread = port3teststr;
							if (port3teststrread.Find(L"TEST_OK") >= 0)
							{
								if (port3failflag == TRUE)
								{
									::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint3_Fail, NULL);
									port3failflag = FALSE;
								}
								port3testcount = 0;
							}
							//else
							//{
							//	if (port3failflag == TRUE)
							//	{
							//		::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_AbnormolFail, NULL);
							//		port3failflag = FALSE;
							//		Port3Abnomal = TRUE;
							//	}
							//}
						}
						port3testcount++;
						if (port3testcount == 5)
						{
							port3testflag = FALSE;
						}
						Sleep(10);
						memset(port3teststr, 0, sizeof(port3teststr) / sizeof(port3teststr[0]));
						dlg->PrintLog(L"发:" + port3teststrcommand, 3);
						PurgeComm(dlg->port3handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
					} while (port3testflag);

					Sleep(100);
					dlg->PrintLog(L"收:" + strread, 3);
					//dlg->SetRicheditText(L"收:" + strread, 0);
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
	if (m_Port3DownloadReadEnd3 == FALSE)
	{
		return;
	}
	dlg->PrintLog(L"收:" + strread, 3);
	//dlg->SetRicheditText(L"收:" + strread, 0);
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
		if (countend == 3)
		{
			::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port3_ErrorInsert, NULL);
			//一直进入发送test指令，如果检测不到，那代表它已经断开了
			BOOL port3testflag = TRUE, port3failflag = TRUE;
			int port3testcount = 0;
			char port3teststr[100];
			memset(port3teststr, 0, sizeof(port3teststr) / sizeof(port3teststr[0]));
			DWORD port3testreadreal = 0, port3testdwBytesWrite, port3testdwErrorFlags;
			BOOL port3testbReadStat, port3testbWriteStat;
			CString port3teststrread, port3teststrcommand = L"AT^GT_CM=aging,off\r\n";
			COMSTAT port3testComStat;

			//老化失败后必须确保老化是关的，所以最好发一条老化关闭指令，接着再判断机子是连接着的还是中途被人直接拔掉了
			port3testbWriteStat = WriteFile(dlg->port3handler, CT2A(port3teststrcommand), port3teststrcommand.GetLength(), &port3testdwBytesWrite, NULL);
			Sleep(500);

			port3teststrcommand = L"AT^GT_CM=TEST\r\n";
			ClearCommError(dlg->port3handler, &port3testdwErrorFlags, &port3testComStat);

			do
			{
				port3testbWriteStat = WriteFile(dlg->port3handler, CT2A(port3teststrcommand), port3teststrcommand.GetLength(), &port3testdwBytesWrite, NULL);

				Sleep(90);
				port3testbReadStat = ReadFile(dlg->port3handler, port3teststr, 100, &port3testreadreal, 0);
				if (port3testbReadStat)
				{
					port3teststrread = port3teststr;
					if (port3teststrread.Find(L"TEST_OK") >= 0)
					{
						if (port3failflag == TRUE)
						{
							::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint3_Fail, NULL);
							port3failflag = FALSE;
						}
						port3testcount = 0;
					}
					else
					{
						if (port3failflag == TRUE)
						{
							::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint3_AbnormolFail, NULL);
							port3failflag = FALSE;
							Port3Abnomal = TRUE;
						}
					}
				}
				port3testcount++;
				if (port3testcount == 5)
				{
					port3testflag = FALSE;
				}
				Sleep(10);
				memset(port3teststr, 0, sizeof(port3teststr) / sizeof(port3teststr[0]));
				dlg->PrintLog(L"发:" + port3teststrcommand, 3);
				PurgeComm(dlg->port3handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
			} while (port3testflag);

			Sleep(100);
			m_Port3DownloadWrite4 = FALSE;
			m_Port3DownloadRead4 = FALSE;

			dlg->DownloadRestPort3Thread();
			return;
		}

		dlg->PrintLog(L"发:" + strcommand, 3);
		//dlg->SetRicheditText(L"发:" + strcommand, 0);
		bWriteStat = WriteFile(dlg->port3handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(2000);
		countend++;
		if (s_bExit == TRUE || m_Port3DownloadControl == FALSE)
		{
			//strcommand = L"AT^GT_CM=reset,1\r\n";
			//ClearCommError(dlg->port3handler, &dwErrorFlags, &ComStat);
			//bWriteStat = WriteFile(dlg->port3handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
			//dlg->PrintLog(L"发:" + strcommand, 3);

			dlg->DownloadClosePort3Thread();
			dlg->DownloadRestPort3Thread();
			return;
		}

		if (m_Port3DownloadWrite4 == FALSE)
		{
			//一直进入发送test指令，如果检测不到，那代表它已经断开了
			BOOL port3testflag = TRUE;
			int port3testcount = 0;
			char port3teststr[100];
			memset(port3teststr, 0, sizeof(port3teststr) / sizeof(port3teststr[0]));
			DWORD port3testreadreal = 0, port3testdwBytesWrite, port3testdwErrorFlags;
			BOOL port3testbReadStat, port3testbWriteStat;
			CString port3teststrread, port3teststrcommand = L"AT^GT_CM=TEST\r\n";
			COMSTAT port3testComStat;

			ClearCommError(dlg->port3handler, &port3testdwErrorFlags, &port3testComStat);
			do
			{
				port3testbWriteStat = WriteFile(dlg->port3handler, CT2A(port3teststrcommand), port3teststrcommand.GetLength(), &port3testdwBytesWrite, NULL);

				Sleep(90);
				port3testbReadStat = ReadFile(dlg->port3handler, port3teststr, 100, &port3testreadreal, 0);
				if (port3testbReadStat)
				{
					port3teststrread = port3teststr;
					if (port3teststrread.Find(L"TEST_OK") >= 0)
					{
						port3testcount = 0;
					}
				}
				port3testcount++;
				if (port3testcount == 5)
				{
					port3testflag = FALSE;
				}
				Sleep(10);
				memset(port3teststr, 0, sizeof(port3teststr) / sizeof(port3teststr[0]));
				dlg->PrintLog(L"发:" + port3teststrcommand, 3);
				PurgeComm(dlg->port3handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
			} while (port3testflag);
		}
	} while (m_Port3DownloadWrite4);

	dlg->DownloadRestPort3Thread();

	//strcommand = L"AT^GT_CM=reset,1\r\n";
	//ClearCommError(dlg->port3handler, &dwErrorFlags, &ComStat);
	//bWriteStat=WriteFile(dlg->port3handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
	//dlg->PrintLog(L"发:" + strcommand, 3);
}

//串口3的读老化命令的读线程
void CMFCP3SIMPORTDlg::DownloadRead4Port3Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	char str[100];
	memset(str, 0, sizeof(str) / sizeof(str[0]));;
	DWORD readreal = 0;
	BOOL bReadStat;
	CString strlog0;

	//其余变量
	CString strread, strfolderpath, strfoldercut;;

	m_Port3DownloadRead4 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程

	do
	{
		Sleep(300);
		bReadStat = ReadFile(dlg->port3handler, str, 100, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			dlg->PrintLog(L"收:" + strread, 3);
			if (strread.Find(L"OK") >= 0)
			{
				::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port3_OkInsert, NULL);
				m_Port3DownloadRead4 = FALSE;
				m_Port3DownloadWrite4 = FALSE;
			}
			else
			{
				continue;
			}
		}
		if (s_bExit == TRUE || m_Port3DownloadControl == FALSE)
		{
			dlg->PrintLog(L"收:" + strread, 3);
			dlg->DownloadClosePort3Thread();
			dlg->DownloadRestPort3Thread();
			return;
		}
	} while (m_Port3DownloadRead4);
	dlg->PrintLog(L"收:" + strread, 3);
	//dlg->SetRicheditText(L"收:" + strread, 0);
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
	m_Port3DownloadReadEnd3 = FALSE;
	m_Port3DownloadRead4 = FALSE;
}


//串口4下载逻辑
//串口4的test写线程
void CMFCP3SIMPORTDlg::DownloadWrite1Port4Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	PurgeComm(dlg->port4handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	//int port4testcount = 0;
	//dlg->PrintLog(L"test线程开启", 4);

	//放指令用变量
	CString strcommand = L"AT^GT_CM=TEST\r\n";

	//一直test，看看串口有没有连接上
	if (Port4Abnomal == TRUE)//这里是如果出现异常插拔，就先停顿个2.5秒才继续往下跑
	{
		Sleep(2500);
	}

	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint4_Ready, NULL);//发消息，串口结果显示待测试

	m_Port4DownloadWrite1 = TRUE;

	ClearCommError(dlg->port4handler, &dwErrorFlags, &ComStat);
	Sleep(50);
	do
	{
		dlg->PrintLog(L"发:" + strcommand, 4);
		//dlg->SetRicheditText(L"发:" + strcommand, 0);
		bWriteStat = WriteFile(dlg->port4handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		//if (port4testcount == 1)
		//{
		//	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint4_Ready, NULL);
		//	//dlg->SetDlgItemText(IDC_PORT4HINT_STATIC, L"待测试");
		//}
		Sleep(2000);

		if (s_bExit == TRUE || m_Port4DownloadControl == FALSE)
		{
			dlg->DownloadClosePort4Thread();
			dlg->DownloadRestPort4Thread();
			return;
		}
		//port4testcount++;
	} while (m_Port4DownloadWrite1);
	//dlg->PrintLog(L"test线程关闭", 4);
	Port4Abnomal = FALSE;//将异常标志位设置为FALSE
	return;
}

//串口4的test读线程
void CMFCP3SIMPORTDlg::DownloadRead1Port4Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	//串口变量
	char str[100];
	memset(str, 0, sizeof(str) / sizeof(str[0]));;
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;

	m_Port4DownloadRead1 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程
	do
	{
		Sleep(300);
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
	dlg->PrintLog(L"收:" + strread, 4);
	//dlg->SetRicheditText(L"收:" + strread, 0);
	PurgeComm(dlg->port4handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	::PostMessage(MainFormHWND, WM_MainPortThreadControl, MainPort_Port4_Write2, NULL);
}

//串口4的写RID、IMEI的写线程
void CMFCP3SIMPORTDlg::DownloadWrite2Port4Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;
	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint4_Connected, NULL);


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
	PurgeComm(dlg->port4handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
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
			if (count == InteverIMEIRIDCount)
			{
				//一直进入发送test指令，如果检测不到，那代表它已经断开了，首先设置一堆放各种读写的变量
				BOOL port4testflag = TRUE, port4failflag = TRUE;
				int port4testcount = 0;
				char port4teststr[100];
				memset(port4teststr, 0, sizeof(port4teststr) / sizeof(port4teststr[0]));
				DWORD port4testreadreal = 0, port4testdwBytesWrite, port4testdwErrorFlags;
				BOOL port4testbReadStat, port4testbWriteStat;
				CString port4teststrread, port4teststrcommand = L"AT^GT_CM=TEST\r\n";
				COMSTAT port4testComStat;

				ClearCommError(dlg->port4handler, &port4testdwErrorFlags, &port4testComStat);
				//这里开始检测机子是否还在连接
				do
				{
					port4testbWriteStat = WriteFile(dlg->port4handler, CT2A(port4teststrcommand), port4teststrcommand.GetLength(), &port4testdwBytesWrite, NULL);//写指令

					Sleep(90);
					port4testbReadStat = ReadFile(dlg->port4handler, port4teststr, 100, &port4testreadreal, 0);//读指令

					//判断读到的东西，如果连续五次没读到回复，那就代表机子已经被干掉了，而不是我们读数据没反应，如果机子有反应，我们还读不到数据，那就代表机子异常得重启一下
					if (port4testbReadStat)
					{
						port4teststrread = port4teststr;

						if (port4teststrread.Find(L"TEST_OK") >= 0)
						{
							if (port4failflag == TRUE)
							{
								::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint4_READFail, NULL);
								port4failflag = FALSE;
							}
							port4testcount = 0;
						}
					}

					port4testcount++;

					//连续读五次没反应就退出
					if (port4testcount == 5)
					{
						port4testflag = FALSE;
					}

					Sleep(10);

					memset(port4teststr, 0, sizeof(port4teststr) / sizeof(port4teststr[0]));

					dlg->PrintLog(L"发:" + port4teststrcommand, 4);

					PurgeComm(dlg->port4handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
				} while (port4testflag);
				//清空一下编辑框
				dlg->SetDlgItemText(IDC_PORT4RID_EDIT, L"");
				dlg->SetDlgItemText(IDC_PORT4IMEI_EDIT, L"");

				dlg->DownloadClosePort4Thread();
				dlg->DownloadRestPort4Thread();
				return;
			}

			dlg->PrintLog(L"发:" + strcommand, 4);
			//dlg->SetRicheditText(L"发:" + strcommand, 0);
			bWriteStat = WriteFile(dlg->port4handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);

			count++;
			Sleep(InteverIMEIRIDTime);
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
	memset(str, 0, sizeof(str) / sizeof(str[0]));;
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
			Sleep(200);
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
							strcount.Replace(LPCTSTR(" "), LPCTSTR(""));
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
						if (strcount != ""&&strcount!="^"&&strcounttemp.Trim(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789").GetLength() == 0)
						{
							dlg->SetDlgItemText(IDC_PORT4IMEI_EDIT, strcount);

							int port4flag;
							CString strport4RID, strport4IMEI;
							ADOManage adoport4manage6;
							adoport4manage6.ConnSQL();

							dlg->GetDlgItemText(IDC_PORT4RID_EDIT, strport4RID);
							dlg->GetDlgItemText(IDC_PORT4IMEI_EDIT, strport4IMEI);

							port4flag = adoport4manage6.SimDataLastStationSql(strport4IMEI);

							if (port4flag == 0)
							{
								m_Port4DownloadWrite2 = FALSE;
								m_Port4DownloadRead2 = FALSE;
								m_Port4DownloadReadEnd2 = FALSE;
								dlg->PrintLog(L"耦合漏测", 4);
								dlg->SetDlgItemText(IDC_PORT4HINT_STATIC, L"耦合漏测");
								adoport4manage6.CloseAll();

								//一直进入发送test指令，如果检测不到，那代表它已经断开了
								BOOL port4testflag = TRUE;
								int port4testcount = 0;
								char port4teststr[100];
								memset(port4teststr, 0, sizeof(port4teststr) / sizeof(port4teststr[0]));
								DWORD port4testreadreal = 0, port4testdwBytesWrite, port4testdwErrorFlags;
								BOOL port4testbReadStat, port4testbWriteStat;
								CString port4teststrread, port4teststrcommand = L"AT^GT_CM=TEST\r\n";
								COMSTAT port4testComStat;

								ClearCommError(dlg->port4handler, &port4testdwErrorFlags, &port4testComStat);
								do
								{
									port4testbWriteStat = WriteFile(dlg->port4handler, CT2A(port4teststrcommand), port4teststrcommand.GetLength(), &port4testdwBytesWrite, NULL);

									Sleep(90);
									port4testbReadStat = ReadFile(dlg->port4handler, port4teststr, 100, &port4testreadreal, 0);
									if (port4testbReadStat)
									{
										port4teststrread = port4teststr;
										if (port4teststrread.Find(L"TEST_OK") >= 0)
										{
											port4testcount = 0;
										}
									}
									port4testcount++;
									if (port4testcount == 5)
									{
										port4testflag = FALSE;
									}
									Sleep(10);
									memset(port4teststr, 0, sizeof(port4teststr) / sizeof(port4teststr[0]));
									dlg->PrintLog(L"发:" + port4teststrcommand, 4);
									PurgeComm(dlg->port4handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
								} while (port4testflag);


								Sleep(50);
								dlg->SetDlgItemText(IDC_PORT4RID_EDIT, L"");
								dlg->SetDlgItemText(IDC_PORT4IMEI_EDIT, L"");
								dlg->DownloadClosePort4Thread();
								dlg->DownloadRestPort4Thread();
								return ;
							}

							port4flag = adoport4manage6.SimDataIsExitSql(strport4RID, strport4IMEI);

							if (port4flag == 0)
							{
								m_Port4DownloadWrite2 = FALSE;
								m_Port4DownloadRead2 = FALSE;
								m_Port4DownloadReadEnd2 = FALSE;
								dlg->PrintLog(L"工位已测", 4);
								dlg->SetDlgItemText(IDC_PORT4HINT_STATIC, L"工位已测");
								adoport4manage6.CloseAll();

								//一直进入发送test指令，如果检测不到，那代表它已经断开了
								BOOL port4testflag = TRUE;
								int port4testcount = 0;
								char port4teststr[100];
								memset(port4teststr, 0, sizeof(port4teststr) / sizeof(port4teststr[0]));
								DWORD port4testreadreal = 0, port4testdwBytesWrite, port4testdwErrorFlags;
								BOOL port4testbReadStat, port4testbWriteStat;
								CString port4teststrread, port4teststrcommand = L"AT^GT_CM=TEST\r\n";
								COMSTAT port4testComStat;

								ClearCommError(dlg->port4handler, &port4testdwErrorFlags, &port4testComStat);
								do
								{
									port4testbWriteStat = WriteFile(dlg->port4handler, CT2A(port4teststrcommand), port4teststrcommand.GetLength(), &port4testdwBytesWrite, NULL);

									Sleep(90);
									port4testbReadStat = ReadFile(dlg->port4handler, port4teststr, 100, &port4testreadreal, 0);
									if (port4testbReadStat)
									{
										port4teststrread = port4teststr;
										if (port4teststrread.Find(L"TEST_OK") >= 0)
										{
											port4testcount = 0;
										}
									}
									port4testcount++;
									if (port4testcount == 5)
									{
										port4testflag = FALSE;
									}
									Sleep(10);
									memset(port4teststr, 0, sizeof(port4teststr) / sizeof(port4teststr[0]));
									dlg->PrintLog(L"发:" + port4teststrcommand, 4);
									PurgeComm(dlg->port4handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
								} while (port4testflag);


								Sleep(50);
								dlg->SetDlgItemText(IDC_PORT4RID_EDIT, L"");
								dlg->SetDlgItemText(IDC_PORT4IMEI_EDIT, L"");
								dlg->DownloadClosePort4Thread();
								dlg->DownloadRestPort4Thread();
								return ;
							}
							//LastPort4RID = strport4RID;
							//LastPort4IMEI = strport4IMEI;
							adoport4manage6.CloseAll();
							//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port4_IsExit, NULL);
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
		dlg->PrintLog(L"收:" + strread, 4);
		//dlg->SetRicheditText(L"收:" + strread, 0);
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
	CFile ReadFile4;
	BOOL bOpen = ReadFile4.Open(StrFolder[3], CFile::modeRead);
	dwTotalSize = ReadFile4.GetLength();//获取文件总长度

	//开始写文件
	strcommand = L"AT^GT_CM=SOFTSIM_DATA,";

	//数据缓冲区
	char *pBuf = new char[dwStep + 1];
	memset(pBuf, '\0', dwStep + 1);

	ClearCommError(dlg->port4handler, &dwErrorFlags, &ComStat);

	::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint4_Downloading, NULL);
	//PurgeComm(dlg->port4handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//这个While循环是用来发送文件数据的
	while ((dwRead = ReadFile4.Read(pBuf, dwStep)) > 0)
	{
		PurgeComm(dlg->port4handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
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
		dlg->PrintLog(L"发:" + strfilewritecommand, 4);
		//dlg->SetRicheditText(L"发:" + strfilewritecommand, 0);
		dwBytesWrite = strcommand.GetLength();
		bWriteStat = WriteFile(dlg->port4handler, CT2A(strfilewritecommand), strfilewritecommand.GetLength(), &dwBytesWrite, NULL);

		intdatano++;
		memset(pBuf, '\0', dwStep + 1);
		delete[] buf1;
		buf1 = NULL;
		Sleep(650);
	}
	delete[] pBuf;
	pBuf = NULL;
	ReadFile4.Close();
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
		if (countend == 3)
		{
			::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port4_ErrorInsert, NULL);//先插入失败记录

			//一直进入发送test指令，如果检测不到，那代表它已经断开了
			BOOL port4testflag = TRUE, port4failflag = TRUE;
			int port4testcount = 0;
			char port4teststr[100];
			memset(port4teststr, 0, sizeof(port4teststr) / sizeof(port4teststr[0]));
			DWORD port4testreadreal = 0, port4testdwBytesWrite, port4testdwErrorFlags;
			BOOL port4testbReadStat, port4testbWriteStat;
			CString port4teststrread, port4teststrcommand = L"AT^GT_CM=TEST\r\n";
			COMSTAT port4testComStat;

			ClearCommError(dlg->port4handler, &port4testdwErrorFlags, &port4testComStat);
			do
			{
				port4testbWriteStat = WriteFile(dlg->port4handler, CT2A(port4teststrcommand), port4teststrcommand.GetLength(), &port4testdwBytesWrite, NULL);

				Sleep(90);
				port4testbReadStat = ReadFile(dlg->port4handler, port4teststr, 100, &port4testreadreal, 0);
				if (port4testbReadStat)
				{
					port4teststrread = port4teststr;
					if (port4teststrread.Find(L"TEST_OK") >= 0)
					{
						if (port4failflag == TRUE)
						{
							::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint4_Fail, NULL);
							port4failflag = FALSE;
						}
						port4testcount = 0;
					}
					else//这里是如果一开始就读不到机子，那肯定是被人中途拔掉了，这是异常也就是违规操作
					{
						if (port4failflag == TRUE)
						{
							::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint4_AbnormolFail, NULL);
							port4failflag = FALSE;
							Port4Abnomal = TRUE;
						}
					}
				}

				port4testcount++;

				if (port4testcount == 5)
				{
					port4testflag = FALSE;
				}

				Sleep(10);
				memset(port4teststr, 0, sizeof(port4teststr) / sizeof(port4teststr[0]));
				dlg->PrintLog(L"发:" + port4teststrcommand, 4);
				PurgeComm(dlg->port4handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
			} while (port4testflag);

			dlg->SetDlgItemText(IDC_PORT4RID_EDIT, L"");
			dlg->SetDlgItemText(IDC_PORT4IMEI_EDIT, L"");
			dlg->DownloadClosePort4Thread();
			dlg->DownloadRestPort4Thread();
			return;
		}

		dlg->PrintLog(L"发:" + strcommand, 4);
		//dlg->SetRicheditText(L"发:" + strcommand, 0);
		bWriteStat = WriteFile(dlg->port4handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(1500);
		countend++;
		if (s_bExit == TRUE || m_Port4DownloadControl == FALSE)
		{
			dlg->DownloadClosePort4Thread();
			dlg->DownloadRestPort4Thread();
			return;
		}
	} while (m_Port4DownloadWrite3);

	//dlg->DownloadRestPort4Thread();
	return;
}

//串口4的读结束命令的读线程
void CMFCP3SIMPORTDlg::DownloadRead3Port4Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	char str[100];
	memset(str, 0, sizeof(str) / sizeof(str[0]));;
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;

	m_Port4DownloadRead3 = TRUE;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程
	m_Port4DownloadReadEnd3 = TRUE;
	do
	{
		Sleep(300);
		bReadStat = ReadFile(dlg->port4handler, str, 100, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			if (strread.Find(L"SoftSim,") >= 0)
			{
				if (strread.Find(L"SoftSim,OK") >= 0)
				{

					//::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port4_OkInsert, NULL);
				}
				else if (strread.Find(L"Error") >= 0)
				{
					::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port4_ErrorInsert, NULL);
					m_Port4DownloadWrite3 = FALSE;
					m_Port4DownloadRead3 = FALSE;
					m_Port4DownloadReadEnd3 = FALSE;

					//一直进入发送test指令，如果检测不到，那代表它已经断开了
					BOOL port4testflag = TRUE, port4failflag = TRUE;
					int port4testcount = 0;
					char port4teststr[100];
					memset(port4teststr, 0, sizeof(port4teststr) / sizeof(port4teststr[0]));
					DWORD port4testreadreal = 0, port4testdwBytesWrite, port4testdwErrorFlags;
					BOOL port4testbReadStat, port4testbWriteStat;
					CString port4teststrread, port4teststrcommand = L"AT^GT_CM=TEST\r\n";
					COMSTAT port4testComStat;

					ClearCommError(dlg->port4handler, &port4testdwErrorFlags, &port4testComStat);
					do
					{
						port4testbWriteStat = WriteFile(dlg->port4handler, CT2A(port4teststrcommand), port4teststrcommand.GetLength(), &port4testdwBytesWrite, NULL);

						Sleep(90);
						port4testbReadStat = ReadFile(dlg->port4handler, port4teststr, 100, &port4testreadreal, 0);
						if (port4testbReadStat)
						{
							port4teststrread = port4teststr;
							if (port4teststrread.Find(L"TEST_OK") >= 0)
							{
								if (port4failflag == TRUE)
								{
									::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint4_Fail, NULL);
									port4failflag = FALSE;
								}
								port4testcount = 0;
							}
							//else
							//{
							//	if (port4failflag == TRUE)
							//	{
							//		::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint4_AbnormolFail, NULL);
							//		port4failflag = FALSE;
							//		Port1Abnomal = TRUE;
							//	}
							//}
						}
						port4testcount++;
						if (port4testcount == 5)
						{
							port4testflag = FALSE;
						}
						Sleep(10);
						memset(port4teststr, 0, sizeof(port4teststr) / sizeof(port4teststr[0]));
						dlg->PrintLog(L"发:" + port4teststrcommand, 4);
						PurgeComm(dlg->port4handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
					} while (port4testflag);

					Sleep(100);
					dlg->PrintLog(L"收:" + strread, 4);
					//dlg->SetRicheditText(L"收:" + strread, 0);
					dlg->DownloadRestPort4Thread();
					return;
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
	if (m_Port4DownloadReadEnd3 == FALSE)
	{
		return;
	}
	dlg->PrintLog(L"收:" + strread, 4);
	//dlg->SetRicheditText(L"收:" + strread, 0);
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
		dlg->PrintLog(L"发:" + strcommand, 4);
		//dlg->SetRicheditText(L"发:" + strcommand, 0);
		if (countend == 3)
		{
			::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port4_ErrorInsert, NULL);
			//一直进入发送test指令，如果检测不到，那代表它已经断开了
			BOOL port4testflag = TRUE, port4failflag = TRUE;
			int port4testcount = 0;
			char port4teststr[100];
			memset(port4teststr, 0, sizeof(port4teststr) / sizeof(port4teststr[0]));
			DWORD port4testreadreal = 0, port4testdwBytesWrite, port4testdwErrorFlags;
			BOOL port4testbReadStat, port4testbWriteStat;
			CString port4teststrread, port4teststrcommand = L"AT^GT_CM=aging,off\r\n";
			COMSTAT port4testComStat;

			//老化失败后必须确保老化是关的，所以最好发一条老化关闭指令，接着再判断机子是连接着的还是中途被人直接拔掉了
			port4testbWriteStat = WriteFile(dlg->port4handler, CT2A(port4teststrcommand), port4teststrcommand.GetLength(), &port4testdwBytesWrite, NULL);
			Sleep(500);

			port4teststrcommand = L"AT^GT_CM=TEST\r\n";
			ClearCommError(dlg->port4handler, &port4testdwErrorFlags, &port4testComStat);

			do
			{
				port4testbWriteStat = WriteFile(dlg->port4handler, CT2A(port4teststrcommand), port4teststrcommand.GetLength(), &port4testdwBytesWrite, NULL);

				Sleep(90);
				port4testbReadStat = ReadFile(dlg->port4handler, port4teststr, 100, &port4testreadreal, 0);
				if (port4testbReadStat)
				{
					port4teststrread = port4teststr;
					if (port4teststrread.Find(L"TEST_OK") >= 0)
					{
						if (port4failflag == TRUE)
						{
							::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint4_Fail, NULL);
							port4failflag = FALSE;
						}
						port4testcount = 0;
					}
					else
					{
						if (port4failflag == TRUE)
						{
							::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint4_AbnormolFail, NULL);
							port4failflag = FALSE;
							Port4Abnomal = TRUE;
						}
					}
				}
				port4testcount++;
				if (port4testcount == 5)
				{
					port4testflag = FALSE;
				}
				Sleep(10);
				memset(port4teststr, 0, sizeof(port4teststr) / sizeof(port4teststr[0]));
				dlg->PrintLog(L"发:" + port4teststrcommand, 4);
				PurgeComm(dlg->port4handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
			} while (port4testflag);

			Sleep(100);
			m_Port4DownloadWrite4 = FALSE;
			m_Port4DownloadRead4 = FALSE;

			dlg->DownloadRestPort4Thread();
			return;
		}
		bWriteStat = WriteFile(dlg->port4handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(2000);
		countend++;
		if (s_bExit == TRUE || m_Port4DownloadControl == FALSE)
		{
			//strcommand = L"AT^GT_CM=reset,1\r\n";
			//ClearCommError(dlg->port4handler, &dwErrorFlags, &ComStat);
			//bWriteStat = WriteFile(dlg->port4handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
			//dlg->PrintLog(L"发:" + strcommand, 4);

			dlg->DownloadClosePort4Thread();
			dlg->DownloadRestPort4Thread();
			return;
		}

		if (m_Port4DownloadWrite4 == FALSE)
		{
			//一直进入发送test指令，如果检测不到，那代表它已经断开了
			BOOL port4testflag = TRUE;
			int port4testcount = 0;
			char port4teststr[100];
			memset(port4teststr, 0, sizeof(port4teststr) / sizeof(port4teststr[0]));
			DWORD port4testreadreal = 0, port4testdwBytesWrite, port4testdwErrorFlags;
			BOOL port4testbReadStat, port4testbWriteStat;
			CString port4teststrread, port4teststrcommand = L"AT^GT_CM=TEST\r\n";
			COMSTAT port4testComStat;

			ClearCommError(dlg->port4handler, &port4testdwErrorFlags, &port4testComStat);
			do
			{
				port4testbWriteStat = WriteFile(dlg->port4handler, CT2A(port4teststrcommand), port4teststrcommand.GetLength(), &port4testdwBytesWrite, NULL);

				Sleep(90);
				port4testbReadStat = ReadFile(dlg->port4handler, port4teststr, 100, &port4testreadreal, 0);
				if (port4testbReadStat)
				{
					port4teststrread = port4teststr;
					if (port4teststrread.Find(L"TEST_OK") >= 0)
					{
						port4testcount = 0;
					}
				}
				port4testcount++;
				if (port4testcount == 5)
				{
					port4testflag = FALSE;
				}
				Sleep(10);
				memset(port4teststr, 0, sizeof(port4teststr) / sizeof(port4teststr[0]));
				dlg->PrintLog(L"发:" + port4teststrcommand, 4);
				PurgeComm(dlg->port4handler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
			} while (port4testflag);
		}
	} while (m_Port4DownloadWrite4);

	dlg->DownloadRestPort4Thread();

	//strcommand = L"AT^GT_CM=reset,1\r\n";
	//ClearCommError(dlg->port4handler, &dwErrorFlags, &ComStat);
	//bWriteStat = WriteFile(dlg->port4handler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
	//dlg->PrintLog(L"发:" + strcommand, 4);
}

//串口4的读老化命令的读线程
void CMFCP3SIMPORTDlg::DownloadRead4Port4Thread(LPVOID lpParam)
{
	CMFCP3SIMPORTDlg* dlg;
	dlg = (CMFCP3SIMPORTDlg*)lpParam;

	//串口变量
	char str[100];
	memset(str, 0, sizeof(str) / sizeof(str[0]));;
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread, strfolderpath, strfoldercut;;

	m_Port4DownloadRead4 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程

	do
	{
		Sleep(200);
		bReadStat = ReadFile(dlg->port4handler, str, 100, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			if (strread.Find(L"aging,on\r\r\nOK!") >= 0)
			{
				::PostMessage(MainFormHWND, WM_MainDataInsertControl, DataInsert_Port4_OkInsert, NULL);
				m_Port4DownloadRead4 = FALSE;
				m_Port4DownloadWrite4 = FALSE;
			}
			else
			{
				continue;
			}
		}
		if (s_bExit == TRUE || m_Port4DownloadControl == FALSE)
		{
			dlg->PrintLog(L"收:" + strread, 4);
			dlg->DownloadClosePort4Thread();
			dlg->DownloadRestPort4Thread();
			return;
		}
	} while (m_Port4DownloadRead4);
	dlg->PrintLog(L"收:" + strread, 4);
	//dlg->SetRicheditText(L"收:" + strread, 0);
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
	m_Port4DownloadReadEnd3 = FALSE;
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

//获取日志时间
CString CMFCP3SIMPORTDlg::GetLogTime()
{
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d%02d%02d"), time.wYear, time.wMonth, time.wDay);
	return strTime;
}

//日志打印函数
void CMFCP3SIMPORTDlg::PrintLog(CString strMsg, int No)
{
	CLog Relog0,Relog1,Relog2,Relog3,Relog4;//日志文件

	switch (No)
	{
	case 0:
		Relog0.WriteLog(CStringA(L"【" + GetTime() + L"】") + CStringA(strMsg), SinglePortLogName);
		break;
	case 1:
		Relog1.WriteLog(CStringA(L"【" + GetTime() + L"】") + CStringA(strMsg), Port1LogName);
		break;
	case 2:
		Relog2.WriteLog(CStringA(L"【" + GetTime() + L"】") + CStringA(strMsg), Port2LogName);
		break;
	case 3:
		Relog3.WriteLog(CStringA(L"【" + GetTime() + L"】") + CStringA(strMsg), Port3LogName);
		break;
	case 4:
		Relog4.WriteLog(CStringA(L"【" + GetTime() + L"】") + CStringA(strMsg), Port4LogName);
		break;
	default:
		break;
	}
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
	CString strport1folderpath, strport1foldercut;
	CString strport2folderpath, strport2foldercut;
	CString strport3folderpath, strport3foldercut;
	CString strport4folderpath, strport4foldercut;

	int singleflag, port1flag, port2flag, port3flag, port4flag;

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
	case DataInsert_SinglePort_LastStation:
		SimDataLastStationFun1();
		break;
	case DataInsert_SinglePort_IsExit:
		singleflag = SimDataLastStationFun1();
		if (singleflag == 0)
		{
			m_Port1SINGLEDownloadWrite2 = FALSE;
			m_Port1SINGLEDownloadReadEnd2 = FALSE;
			SetDlgItemText(IDC_PORT1HINT_STATIC, L"耦合漏测");
			Sleep(10);
			SingleDownloadClosePort1Thread();
			SingleDownloadRestPort1Thread();
			return 0;
		}
		singleflag = SimDataIsExitFun1();
		if (singleflag == 0)
		{
			m_Port1SINGLEDownloadWrite2 = FALSE;
			m_Port1SINGLEDownloadReadEnd2 = FALSE;
			SetDlgItemText(IDC_PORT1HINT_STATIC,L"工位已测");
			Sleep(10);
			SingleDownloadClosePort1Thread();
			SingleDownloadRestPort1Thread();
			return 0;
		}
		break;
	case DataInsert_SinglePort_OkInsert:
		strSingleFileTempPath = strSingleFilePath.Left(strSingleFilePath.GetLength() - 10);//将末尾的证书文件路径切掉
		strSingleFilecut = strSingleFileTempPath.Right(13);//然后将种子文件夹名称切割出来
		SimDataOkInsertFun1();
		Sleep(20);
		MoveFileEx(strSingleFileTempPath, strOKFolderpath + strSingleFilecut, MOVEFILE_REPLACE_EXISTING);//然后粘到下好的OK文件夹路径后面
		Sleep(20);
		//SetPort1EditEmpty();
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
	case DataInsert_SinglePort_ErrorInsert:
		SimDataErrorInsertFun1();
		break;

	//多文件下载
		//串口1
	case DataInsert_Port1_IsExit:
		port1flag = SimDataLastStationFun1();
		if (port1flag == 0)
		{
			m_Port1DownloadWrite2 = FALSE;
			m_Port1DownloadReadEnd2 = FALSE;
			SetDlgItemText(IDC_PORT1HINT_STATIC, L"耦合漏测");
			Sleep(10);
			DownloadClosePort1Thread();
			DownloadRestPort1Thread();
			return 0;
		}
		port1flag = SimDataIsExitFun1();
		if (port1flag == 0)
		{
			m_Port1DownloadWrite2 = FALSE;
			m_Port1DownloadReadEnd2 = FALSE;
			SetDlgItemText(IDC_PORT1HINT_STATIC, L"工位已测");
			Sleep(10);
			DownloadClosePort1Thread();
			DownloadRestPort1Thread();
			return 0;
		}
		break;
	case DataInsert_Port1_OkInsert:
		strport1folderpath = StrFolder[0];//路径先复制到一个临时变量
		strport1folderpath = strport1folderpath.Left(strport1folderpath.GetLength() - 10);//将末尾的证书文件路径切掉
		strport1foldercut = strport1folderpath.Right(13);//然后将种子文件夹名称切割出来
		SimDataOkInsertFun1();
		Sleep(20);
		MoveFileEx(strport1folderpath, strOKFolderpath + strport1foldercut, MOVEFILE_REPLACE_EXISTING);//然后粘到下好的OK文件夹路径后面
		StrFolder[0] = L"";
		Sleep(20);
		::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint1_Success, NULL);
		//SetPort1EditEmpty();
		break;
	case DataInsert_Port1_ErrorInsert:
		SimDataErrorInsertFun1();
		break;

		//串口2
	case DataInsert_Port2_IsExit:
		port2flag = SimDataLastStationFun2();
		if (port2flag == 0)
		{
			m_Port2DownloadWrite2 = FALSE;
			m_Port2DownloadReadEnd2 = FALSE;
			SetDlgItemText(IDC_PORT2HINT_STATIC, L"耦合漏测");
			Sleep(10);
			DownloadClosePort2Thread();
			DownloadRestPort2Thread();
			return 0;
		}
		port2flag = SimDataIsExitFun2();
		if (port2flag == 0)
		{
			m_Port2DownloadWrite2 = FALSE;
			m_Port2DownloadReadEnd2 = FALSE;
			SetDlgItemText(IDC_PORT2HINT_STATIC, L"工位已测");
			Sleep(10);
			DownloadClosePort2Thread();
			DownloadRestPort2Thread();
			return 0;
		}
		break;
	case DataInsert_Port2_OkInsert:
		strport2folderpath = StrFolder[1];//路径先复制到一个临时变量
		strport2folderpath = strport2folderpath.Left(strport2folderpath.GetLength() - 10);//将末尾的证书文件路径切掉
		strport2foldercut = strport2folderpath.Right(13);//然后将种子文件夹名称切割出来
		SimDataOkInsertFun2();
		Sleep(20);
		MoveFileEx(strport2folderpath, strOKFolderpath + strport2foldercut, MOVEFILE_REPLACE_EXISTING);//然后粘到下好的OK文件夹路径后面
		StrFolder[1] = L"";
		Sleep(20);
		::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint2_Success, NULL);
		//SetPort2EditEmpty();
		break;
	case DataInsert_Port2_ErrorInsert:
		SimDataErrorInsertFun2();
		break;

		//串口3
	case DataInsert_Port3_IsExit:
		port3flag = SimDataLastStationFun3();
		if (port3flag == 0)
		{
			m_Port3DownloadWrite2 = FALSE;
			m_Port3DownloadReadEnd2 = FALSE;
			SetDlgItemText(IDC_PORT3HINT_STATIC, L"耦合漏测");
			Sleep(10);
			DownloadClosePort3Thread();
			DownloadRestPort3Thread();
			return 0;
		}
		port3flag = SimDataIsExitFun3();
		if (port3flag == 0)
		{
			m_Port3DownloadWrite2 = FALSE;
			m_Port3DownloadReadEnd2 = FALSE;
			SetDlgItemText(IDC_PORT3HINT_STATIC, L"工位已测");
			Sleep(10);
			DownloadClosePort3Thread();
			DownloadRestPort3Thread();
			return 0;
		}
		break;
	case DataInsert_Port3_OkInsert:
		strport3folderpath = StrFolder[2];//路径先复制到一个临时变量
		strport3folderpath = strport3folderpath.Left(strport3folderpath.GetLength() - 10);//将末尾的证书文件路径切掉
		strport3foldercut = strport3folderpath.Right(13);//然后将种子文件夹名称切割出来
		SimDataOkInsertFun3();
		Sleep(20);
		MoveFileEx(strport3folderpath, strOKFolderpath + strport3foldercut, MOVEFILE_REPLACE_EXISTING);//然后粘到下好的OK文件夹路径后面
		StrFolder[2] = L"";
		Sleep(20);
		::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint3_Success, NULL);
		//SetPort3EditEmpty();
		break;
	case DataInsert_Port3_ErrorInsert:
		SimDataErrorInsertFun3();
		break;

		//串口4
	case DataInsert_Port4_IsExit:
		port4flag = SimDataLastStationFun4();
		if (port4flag == 0)
		{
			m_Port4DownloadWrite2 = FALSE;
			m_Port4DownloadReadEnd2 = FALSE;
			SetDlgItemText(IDC_PORT4HINT_STATIC, L"耦合漏测");
			Sleep(10);
			DownloadClosePort4Thread();
			DownloadRestPort4Thread();
			return 0;
		}
		port4flag = SimDataIsExitFun4();
		if (port4flag == 0)
		{
			m_Port4DownloadWrite2 = FALSE;
			m_Port4DownloadReadEnd2 = FALSE;
			SetDlgItemText(IDC_PORT4HINT_STATIC, L"工位已测");
			Sleep(10);
			DownloadClosePort4Thread();
			DownloadRestPort4Thread();
			return 0;
		}
		break;
	case DataInsert_Port4_OkInsert:
		strport4folderpath = StrFolder[3];//路径先复制到一个临时变量
		strport4folderpath = strport4folderpath.Left(strport4folderpath.GetLength() - 10);//将末尾的证书文件路径切掉
		strport4foldercut = strport4folderpath.Right(13);//然后将种子文件夹名称切割出来
		SimDataOkInsertFun4();
		Sleep(20);
		MoveFileEx(strport4folderpath, strOKFolderpath + strport4foldercut, MOVEFILE_REPLACE_EXISTING);//然后粘到下好的OK文件夹路径后面
		StrFolder[3] = L"";
		Sleep(20);
		::PostMessage(MainFormHWND, WM_MainFontControl, Main_Hint4_Success, NULL);
		//SetPort4EditEmpty();
		break;
	case DataInsert_Port4_ErrorInsert:
		SimDataErrorInsertFun4();
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
//判断串口1的机器上个工位（也就是耦合位）
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
	SetPort1EditEmpty();
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

//串口2的数据库函数
//判断串口2的机器上个工位（也就是耦合位）
int CMFCP3SIMPORTDlg::SimDataLastStationFun2()
{
	int flag;
	CString strport2IMEI;
	ADOManage adoport2manage4;
	adoport2manage4.ConnSQL();

	GetDlgItemText(IDC_PORT2IMEI_EDIT, strport2IMEI);

	flag = adoport2manage4.SimDataLastStationSql(strport2IMEI);

	adoport2manage4.CloseAll();
	return flag;
}

//判断机器是否烧成功过
int CMFCP3SIMPORTDlg::SimDataIsExitFun2()
{
	int flag;
	CString strport2RID, strport2IMEI;
	ADOManage adoport2manage4;
	adoport2manage4.ConnSQL();

	GetDlgItemText(IDC_PORT2RID_EDIT, strport2RID);
	GetDlgItemText(IDC_PORT2IMEI_EDIT, strport2IMEI);

	flag = adoport2manage4.SimDataIsExitSql(strport2RID, strport2IMEI);

	adoport2manage4.CloseAll();
	return flag;
}

//SIMOK时插入数据
void CMFCP3SIMPORTDlg::SimDataOkInsertFun2()
{
	CString strport2RID, strport2CID, strport2IMEI, strport2ICCID;
	ADOManage adoport2manage4;
	adoport2manage4.ConnSQL();

	GetDlgItemText(IDC_PORT2CID_EDIT, strport2CID);
	GetDlgItemText(IDC_PORT2RID_EDIT, strport2RID);
	GetDlgItemText(IDC_PORT2IMEI_EDIT, strport2IMEI);
	GetDlgItemText(IDC_PORT2ICCID_EDIT, strport2ICCID);

	adoport2manage4.SimDataOkInsertSql(m_pcipEdit, strport2RID, strport2IMEI, strport2CID, strport2ICCID, L"NULL");

	adoport2manage4.CloseAll();
	SetPort2EditEmpty();
}

//SIMERROE插入数据
void CMFCP3SIMPORTDlg::SimDataErrorInsertFun2()
{
	CString strport2RID, strport2IMEI;
	ADOManage adoport2manage4;
	adoport2manage4.ConnSQL();

	GetDlgItemText(IDC_PORT2RID_EDIT, strport2RID);
	GetDlgItemText(IDC_PORT2IMEI_EDIT, strport2IMEI);

	adoport2manage4.SimDataErrorInsertSql(m_pcipEdit, strport2RID, strport2IMEI, L"NULL");

	adoport2manage4.CloseAll();
}

//串口3的数据库函数
//判断串口3的机器上个工位（也就是耦合位）
int CMFCP3SIMPORTDlg::SimDataLastStationFun3()
{
	int flag;
	CString strport3IMEI;
	ADOManage adoport3manage5;
	adoport3manage5.ConnSQL();

	GetDlgItemText(IDC_PORT3IMEI_EDIT, strport3IMEI);

	flag = adoport3manage5.SimDataLastStationSql(strport3IMEI);

	adoport3manage5.CloseAll();
	return flag;
}

//判断机器是否烧成功过
int CMFCP3SIMPORTDlg::SimDataIsExitFun3()
{
	int flag;
	CString strport3RID, strport3IMEI;
	ADOManage adoport3manage5;
	adoport3manage5.ConnSQL();

	GetDlgItemText(IDC_PORT3RID_EDIT, strport3RID);
	GetDlgItemText(IDC_PORT3IMEI_EDIT, strport3IMEI);

	flag = adoport3manage5.SimDataIsExitSql(strport3RID, strport3IMEI);

	adoport3manage5.CloseAll();
	return flag;
}

//SIMOK时插入数据
void CMFCP3SIMPORTDlg::SimDataOkInsertFun3()
{
	CString strport3RID, strport3CID, strport3IMEI, strport3ICCID;
	ADOManage adoport3manage5;
	adoport3manage5.ConnSQL();

	GetDlgItemText(IDC_PORT3CID_EDIT, strport3CID);
	GetDlgItemText(IDC_PORT3RID_EDIT, strport3RID);
	GetDlgItemText(IDC_PORT3IMEI_EDIT, strport3IMEI);
	GetDlgItemText(IDC_PORT3ICCID_EDIT, strport3ICCID);

	adoport3manage5.SimDataOkInsertSql(m_pcipEdit, strport3RID, strport3IMEI, strport3CID, strport3ICCID, L"NULL");

	adoport3manage5.CloseAll();
	SetPort3EditEmpty();
}

//SIMERROE插入数据
void CMFCP3SIMPORTDlg::SimDataErrorInsertFun3()
{
	CString strport3RID, strport3IMEI;
	ADOManage adoport3manage5;
	adoport3manage5.ConnSQL();

	GetDlgItemText(IDC_PORT3RID_EDIT, strport3RID);
	GetDlgItemText(IDC_PORT3IMEI_EDIT, strport3IMEI);

	adoport3manage5.SimDataErrorInsertSql(m_pcipEdit, strport3RID, strport3IMEI, L"NULL");

	adoport3manage5.CloseAll();
}

//串口4的数据库函数
//判断串口4的机器上个工位（也就是耦合位）
int CMFCP3SIMPORTDlg::SimDataLastStationFun4()
{
	int flag;
	CString strport4IMEI;
	ADOManage adoport4manage6;
	adoport4manage6.ConnSQL();

	GetDlgItemText(IDC_PORT4IMEI_EDIT, strport4IMEI);

	flag = adoport4manage6.SimDataLastStationSql(strport4IMEI);

	adoport4manage6.CloseAll();
	return flag;
}

//判断机器是否烧成功过
int CMFCP3SIMPORTDlg::SimDataIsExitFun4()
{
	int flag;
	CString strport4RID, strport4IMEI;
	ADOManage adoport4manage6;
	adoport4manage6.ConnSQL();

	GetDlgItemText(IDC_PORT4RID_EDIT, strport4RID);
	GetDlgItemText(IDC_PORT4IMEI_EDIT, strport4IMEI);

	flag = adoport4manage6.SimDataIsExitSql(strport4RID, strport4IMEI);

	adoport4manage6.CloseAll();
	return flag;
}

//SIMOK时插入数据
void CMFCP3SIMPORTDlg::SimDataOkInsertFun4()
{
	CString strport4RID, strport4CID, strport4IMEI, strport4ICCID;
	ADOManage adoport4manage6;
	adoport4manage6.ConnSQL();

	GetDlgItemText(IDC_PORT4CID_EDIT, strport4CID);
	GetDlgItemText(IDC_PORT4RID_EDIT, strport4RID);
	GetDlgItemText(IDC_PORT4IMEI_EDIT, strport4IMEI);
	GetDlgItemText(IDC_PORT4ICCID_EDIT, strport4ICCID);

	adoport4manage6.SimDataOkInsertSql(m_pcipEdit, strport4RID, strport4IMEI, strport4CID, strport4ICCID, L"NULL");

	adoport4manage6.CloseAll();
	SetPort4EditEmpty();
}

//SIMERROE插入数据
void CMFCP3SIMPORTDlg::SimDataErrorInsertFun4()
{
	CString strport4RID, strport4IMEI;
	ADOManage adoport4manage6;
	adoport4manage6.ConnSQL();

	GetDlgItemText(IDC_PORT4RID_EDIT, strport4RID);
	GetDlgItemText(IDC_PORT4IMEI_EDIT, strport4IMEI);

	adoport4manage6.SimDataErrorInsertSql(m_pcipEdit, strport4RID, strport4IMEI, L"NULL");

	adoport4manage6.CloseAll();
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
	staticHint1font.CreatePointFont(750, L"黑体");
	staticHint2font.CreatePointFont(750, L"黑体");
	staticHint3font.CreatePointFont(750, L"黑体");
	staticHint4font.CreatePointFont(750, L"黑体");

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
		if (str1 == "失败等待重启" || str1 == "工位已测" || str1 == "耦合漏测" || str1 == "需要返工" || str1 == L"读错误请重启" || str1 == L"设备异常插拔")
		{
			pDC->SetTextColor(RGB(255, 0, 0));//用RGB宏改变颜色 
			pDC->SelectObject(&staticHint1font);
		}
		else if (str1 == "待测试")
		{
			pDC->SetTextColor(RGB(65, 105, 225));//用RGB宏改变颜色 
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
		if (str2 == "失败等待重启" || str2 == "工位已测" || str2 == "耦合漏测" || str2 == "需要返工" || str2 == L"读错误请重启" || str2 == L"设备异常插拔")
		{
			pDC->SetTextColor(RGB(255, 0, 0));//用RGB宏改变颜色 
			pDC->SelectObject(&staticHint2font);
		}
		else if (str2 == "待测试")
		{
			pDC->SetTextColor(RGB(65, 105, 225));//用RGB宏改变颜色 
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
		if (str3 == "失败等待重启" || str3 == "工位已测" || str3 == "耦合漏测" || str3 == "需要返工" || str3 == L"读错误请重启" || str3 == L"设备异常插拔")
		{
			pDC->SetTextColor(RGB(255, 0, 0));//用RGB宏改变颜色 
			pDC->SelectObject(&staticHint3font);
		}
		else if (str3 == "待测试")
		{
			pDC->SetTextColor(RGB(65, 105, 225));//用RGB宏改变颜色 
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
	else if (pWnd->GetDlgCtrlID() == IDC_PORT4HINT_STATIC)
	{
		GetDlgItemText(IDC_PORT4HINT_STATIC, str4);
		if (str4 == "失败等待重启" || str4 == "工位已测" || str4 == "耦合漏测" || str4 == "需要返工" || str4 == L"读错误请重启" || str4 == L"设备异常插拔")
		{
			pDC->SetTextColor(RGB(255, 0, 0));//用RGB宏改变颜色 
			pDC->SelectObject(&staticHint4font);
		}
		else if (str4 == "待测试")
		{
			pDC->SetTextColor(RGB(65, 105, 225));//用RGB宏改变颜色 
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
		PrintLog(L"待测试\r\n", 1);
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"待测试");
		break;
	case Main_Hint1_Connected:
		PrintLog(L"读取数据中\r\n", 1);
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"读取数据中");
		break;
	case Main_Hint1_Downloading:
		PrintLog(L"下载中\r\n", 1);
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"下载中");
		break;
	case Main_Hint1_Ageing:
		PrintLog(L"老化中\r\n", 1);
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"老化中");
		break;
	case Main_Hint1_Success:
		PrintLog(L"成功\r\n", 1);
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"成功");
		break;
	case Main_Hint1_Fail:
		PrintLog(L"失败等待重启\r\n", 1);
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"失败等待重启");
		break;
	case Main_Hint1_CoupleFail:
		PrintLog(L"耦合漏测\r\n", 1);
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"耦合漏测");
		break;
	case Main_Hint1_READFail:
		PrintLog(L"读错误请重启\r\n", 1);
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"读错误请重启");
		break;
	case Main_Hint1_AbnormolFail:
		PrintLog(L"设备异常插拔\r\n", 1);
		SetDlgItemText(IDC_PORT1HINT_STATIC, L"设备异常插拔");
		break;

	//串口2提示结果
	case Main_Hint2_Ready:
		PrintLog(L"待测试\r\n", 2);
		SetDlgItemText(IDC_PORT2HINT_STATIC, L"待测试");
		break;
	case Main_Hint2_Connected:
		PrintLog(L"读取数据中\r\n", 2);
		SetDlgItemText(IDC_PORT2HINT_STATIC, L"读取数据中");
		break;
	case Main_Hint2_Downloading:
		PrintLog(L"下载中\r\n", 2);
		SetDlgItemText(IDC_PORT2HINT_STATIC, L"下载中");
		break;
	case Main_Hint2_Ageing:
		PrintLog(L"老化中\r\n", 2);
		SetDlgItemText(IDC_PORT2HINT_STATIC, L"老化中");
		break;
	case Main_Hint2_Success:
		PrintLog(L"成功\r\n", 2);
		SetDlgItemText(IDC_PORT2HINT_STATIC, L"成功");
		break;
	case Main_Hint2_Fail:
		PrintLog(L"失败等待重启\r\n", 2);
		SetDlgItemText(IDC_PORT2HINT_STATIC, L"失败等待重启");
		break;
	case Main_Hint2_CoupleFail:
		PrintLog(L"耦合漏测\r\n", 2);
		SetDlgItemText(IDC_PORT2HINT_STATIC, L"耦合漏测");
		break;
	case Main_Hint2_READFail:
		PrintLog(L"读错误请重启\r\n", 2);
		SetDlgItemText(IDC_PORT2HINT_STATIC, L"读错误请重启");
		break;
	case Main_Hint2_AbnormolFail:
		PrintLog(L"设备异常插拔\r\n", 2);
		SetDlgItemText(IDC_PORT2HINT_STATIC, L"设备异常插拔");
		break;

	//串口3提示结果
	case Main_Hint3_Ready:
		PrintLog(L"待测试\r\n", 3);
		SetDlgItemText(IDC_PORT3HINT_STATIC, L"待测试");
		break;
	case Main_Hint3_Connected:
		PrintLog(L"读取数据中\r\n", 3);
		SetDlgItemText(IDC_PORT3HINT_STATIC, L"读取数据中");
		break;
	case Main_Hint3_Downloading:
		PrintLog(L"下载中\r\n", 3);
		SetDlgItemText(IDC_PORT3HINT_STATIC, L"下载中");
		break;
	case Main_Hint3_Ageing:
		PrintLog(L"老化中\r\n", 3);
		SetDlgItemText(IDC_PORT3HINT_STATIC, L"老化中");
		break;
	case Main_Hint3_Success:
		PrintLog(L"成功\r\n", 3);
		SetDlgItemText(IDC_PORT3HINT_STATIC, L"成功");
		break;
	case Main_Hint3_Fail:
		PrintLog(L"失败等待重启\r\n", 3);
		SetDlgItemText(IDC_PORT3HINT_STATIC, L"失败等待重启");
		break;
	case Main_Hint3_CoupleFail:
		PrintLog(L"耦合漏测\r\n", 3);
		SetDlgItemText(IDC_PORT3HINT_STATIC, L"耦合漏测");
		break;
	case Main_Hint3_READFail:
		PrintLog(L"读错误请重启\r\n", 3);
		SetDlgItemText(IDC_PORT3HINT_STATIC, L"读错误请重启");
		break;
	case Main_Hint3_AbnormolFail:
		PrintLog(L"设备异常插拔\r\n", 3);
		SetDlgItemText(IDC_PORT3HINT_STATIC, L"设备异常插拔");
		break;

	//串口4提示结果
	case Main_Hint4_Ready:
		PrintLog(L"待测试\r\n", 4);
		SetDlgItemText(IDC_PORT4HINT_STATIC, L"待测试");
		break;
	case Main_Hint4_Connected:
		PrintLog(L"读取数据中\r\n", 4);
		SetDlgItemText(IDC_PORT4HINT_STATIC, L"读取数据中");
		break;
	case Main_Hint4_Downloading:
		PrintLog(L"下载中\r\n", 4);
		SetDlgItemText(IDC_PORT4HINT_STATIC, L"下载中");
		break;
	case Main_Hint4_Ageing:
		PrintLog(L"老化中\r\n", 4);
		SetDlgItemText(IDC_PORT4HINT_STATIC, L"老化中");
		break;
	case Main_Hint4_Success:
		PrintLog(L"成功\r\n", 4);
		SetDlgItemText(IDC_PORT4HINT_STATIC, L"成功");
		break;
	case Main_Hint4_Fail:
		PrintLog(L"失败等待重启\r\n", 4);
		SetDlgItemText(IDC_PORT4HINT_STATIC, L"失败等待重启");
		break;
	case Main_Hint4_CoupleFail:
		PrintLog(L"耦合漏测\r\n", 4);
		SetDlgItemText(IDC_PORT4HINT_STATIC, L"耦合漏测");
		break;
	case Main_Hint4_READFail:
		PrintLog(L"读错误请重启\r\n", 4);
		SetDlgItemText(IDC_PORT4HINT_STATIC, L"读错误请重启");
		break;
	case Main_Hint4_AbnormolFail:
		PrintLog(L"设备异常插拔\r\n", 4);
		SetDlgItemText(IDC_PORT4HINT_STATIC, L"设备异常插拔");
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


