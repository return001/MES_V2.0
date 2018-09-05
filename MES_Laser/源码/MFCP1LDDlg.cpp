
// MFCP1LDDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCP1LD.h"
#include "MFCP1LDDlg.h"
#include "afxdialogex.h"
#include "ADOManage.h"
#include "DBConfig.h"
#include "SetImeiRangeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

volatile BOOL m_LdPortRun;//读串口全局变量，专门用于线程的

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


// CMFCP1LDDlg 对话框

CMFCP1LDDlg::CMFCP1LDDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCP1LDDlg::IDD, pParent)
	, m_pcnameEdit(_T(""))
	, m_pcipEdit(_T(""))
	, radioflag(TRUE)
	, STEPLDFlag(1)
	, ScanFlag(1)
	, IMEI15Flag(1)
	, m_pCLDStartDlg(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    
	//制单号全局变量
	ZhiDanNO = _T("");

	//IMEI全局变量
	IMEIStaStart = _T("");
	IMEIStaEnd = _T("");
	IMEIStaCRStart = _T("");
	IMEIStaCREnd = _T("");

	//给镭雕字段等赋初始值
	LdVariableChar[0] = _T("IMEI");
	LdVariableChar[1] = _T("SN");
	LdVariableChar[2] = _T("SIM");
	LdVariableChar[3] = _T("VIP");
	LdVariableChar[4] = _T("ICCID");
	LdVariableChar[5] = _T("BAT");
	LdVariableChar[6] = _T("MAC");
	LdVariableChar[7] = _T("Equipment number");

	for (int i = 0; i < 8; i++)
	{
	LdVariableInt[i] = -1;
	}
	LdVariableInt[0] = 0;
	LdVariableCount = 1;
}

CMFCP1LDDlg::~CMFCP1LDDlg()
{
	// 如果非模态对话框已经创建则删除它   
	if (NULL != m_pCLDStartDlg)
	{
		// 删除非模态对话框对象   
		delete m_pCLDStartDlg;
	}
}

void CMFCP1LDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ZHIDANAUTO_COMBO, m_zhidanautoCombo);
	DDX_Control(pDX, IDC_DBCURTAIN_LIST, m_dbcurtainList);
	DDX_Control(pDX, IDC_CURRENTTCP_RICHEDIT, m_currentRichedit);
	DDX_Control(pDX, IDC_IMEISTART_EDIT, m_imeistartEdit);
	DDX_Control(pDX, IDC_IMEIEND_EDIT, m_imeiendEdit);
	DDX_Control(pDX, IDC_IMEISTARTSHOWONLY_EDIT, m_imeistartshowonlyEdit);
	DDX_Control(pDX, IDC_IMEIENDSHOWONLY_EDIT, m_imeiendshowonlyEdit);
}

BEGIN_MESSAGE_MAP(CMFCP1LDDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMFCP1LDDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMFCP1LDDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_DBCONFIG_BUTTON, &CMFCP1LDDlg::OnBnClickedDbconfigButton)
	ON_BN_CLICKED(IDC_SELECTMODEL_BUTTON, &CMFCP1LDDlg::OnBnClickedSelectmodleButton)
	ON_BN_CLICKED(IDC_OPENMODEL_BUTTON, &CMFCP1LDDlg::OnBnClickedOpenmodleButton)
	ON_BN_CLICKED(IDC_UPDATEZHIDAN_BUTTON, &CMFCP1LDDlg::OnBnClickedUpdatezhidanButton)
	ON_CBN_KILLFOCUS(IDC_ZHIDANAUTO_COMBO, &CMFCP1LDDlg::OnCbnKillfocusZhidanautoCombo)
	ON_BN_CLICKED(IDC_NORMOLLD_RADIO, &CMFCP1LDDlg::OnBnClickedNormolldRadio)
	ON_BN_CLICKED(IDC_RELD_RADIO, &CMFCP1LDDlg::OnBnClickedReldRadio)
	ON_CBN_SELCHANGE(IDC_ZHIDANAUTO_COMBO, &CMFCP1LDDlg::OnCbnSelchangeZhidanautoCombo)
	ON_BN_CLICKED(IDC_READCURRENTIMEI_BUTTON, &CMFCP1LDDlg::OnBnClickedReadcurrentimeiButton)
	ON_BN_CLICKED(IDC_READIMEI_BUTTON, &CMFCP1LDDlg::OnBnClickedReadimeiButton)
	ON_EN_KILLFOCUS(IDC_IMEISTART_EDIT, &CMFCP1LDDlg::OnEnKillfocusImeistartEdit)
	ON_EN_KILLFOCUS(IDC_IMEIEND_EDIT, &CMFCP1LDDlg::OnEnKillfocusImeiendEdit)
	ON_BN_CLICKED(IDC_SELECTALLDATA_BUTTON, &CMFCP1LDDlg::OnBnClickedSelectalldataButton)
	ON_BN_CLICKED(IDC_SELECTREPETDATA_BUTTON, &CMFCP1LDDlg::OnBnClickedSelectrepetdataButton)
	ON_BN_CLICKED(IDC_SELECTMISSINGDATA_BUTTON, &CMFCP1LDDlg::OnBnClickedSelectmissingdataButton)
	ON_BN_CLICKED(IDC_OPENLDSYSTEM_BUTTON, &CMFCP1LDDlg::OnBnClickedOpenldsystemButton)
	ON_BN_CLICKED(IDC_CLOSELDSYSTEM_BUTTON, &CMFCP1LDDlg::OnBnClickedCloseldsystemButton)
	ON_BN_CLICKED(IDC_LDONECE_BUTTON, &CMFCP1LDDlg::OnBnClickedLdoneceButton)
	ON_BN_CLICKED(IDC_STEPLD_BUTTON, &CMFCP1LDDlg::OnBnClickedStepldButton)
	ON_BN_CLICKED(IDC_IMEI_CHECK, &CMFCP1LDDlg::OnBnClickedImeiCheck)
	ON_BN_CLICKED(IDC_SCANMODE_BUTTON, &CMFCP1LDDlg::OnBnClickedScanmodeButton)
	ON_BN_CLICKED(IDC_PARITY_CHECK, &CMFCP1LDDlg::OnBnClickedParityCheck)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SETIMEIRANGE_BUTTON, &CMFCP1LDDlg::OnBnClickedSetimeirangeButton)
END_MESSAGE_MAP()


// CMFCP1LDDlg 消息处理程序

BOOL CMFCP1LDDlg::OnInitDialog()
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
	/*
	//读取模版打开路径
	CString str;
	GetPrivateProfileString(_T("ModelPathName"), _T("modelpathname"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
	SetDlgItemText(IDC_MODELPATH_EDIT, str);
	str.ReleaseBuffer();
    */

	//初始化本机信息
	GetLocalHostIPName(m_pcnameEdit, m_pcipEdit);//调用函数获得IP和计算机名称
	SetDlgItemText(IDC_PCIP_EDIT, m_pcipEdit);//将计算机的名称显示在名称控件中
	SetDlgItemText(IDC_PCNAME_EDIT, m_pcnameEdit);//将计算机的IP显示在IP控件中

	//初始化订单
	InitComboBox();
	
	//初始化控件
	GetDlgItem(IDC_IMEI_CHECK)->EnableWindow(FALSE);
	GetDlgItem(IDC_NORMOLLD_RADIO)->EnableWindow(FALSE);
	GetDlgItem(IDC_RELD_RADIO)->EnableWindow(FALSE);
	GetDlgItem(IDC_PARITY_CHECK)->EnableWindow(FALSE);
	GetDlgItem(IDC_MODELPATH_EDIT)->EnableWindow(FALSE);
	CRLDInformationwindowfalse();
	LDSystemInformationwindowfalse();
	CheckDlgButton(IDC_IMEI_CHECK, 1);
	CheckDlgButton(IDC_NORMOLLD_RADIO, 1);
	CheckDlgButton(IDC_PARITY_CHECK, 1);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCP1LDDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCP1LDDlg::OnPaint()
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
HCURSOR CMFCP1LDDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//控制窗体中控件获得使能和失去使能的函数
void CMFCP1LDDlg::InitInformationwindowtrue()
{
	GetDlgItem(IDC_IMEI_CHECK)->EnableWindow(TRUE);
	GetDlgItem(IDC_NORMOLLD_RADIO)->EnableWindow(TRUE);
	GetDlgItem(IDC_RELD_RADIO)->EnableWindow(TRUE);
	GetDlgItem(IDC_PARITY_CHECK)->EnableWindow(TRUE);
}

void CMFCP1LDDlg::CRLDInformationwindowtrue()
{
	GetDlgItem(IDC_IMEISTART_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_IMEIEND_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_READCURRENTIMEI_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_READIMEI_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_OPENLDSYSTEM_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_SELECTMODEL_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_OPENMODEL_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_SELECTALLDATA_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_SELECTREPETDATA_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_SELECTMISSINGDATA_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_SETIMEIRANGE_BUTTON)->EnableWindow(TRUE);
}

void CMFCP1LDDlg::LDSystemInformationwindowtrue()
{
	GetDlgItem(IDC_LDONECE_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_STEPLD_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_CLOSELDSYSTEM_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_SCANMODE_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_UPDATEZHIDAN_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_ZHIDANAUTO_COMBO)->EnableWindow(FALSE);
	GetDlgItem(IDC_DBCONFIG_BUTTON)->EnableWindow(FALSE);
}

void CMFCP1LDDlg::InitInformationwindowfalse()
{
	GetDlgItem(IDC_IMEI_CHECK)->EnableWindow(FALSE);
	GetDlgItem(IDC_NORMOLLD_RADIO)->EnableWindow(FALSE);
	GetDlgItem(IDC_RELD_RADIO)->EnableWindow(FALSE);
	GetDlgItem(IDC_PARITY_CHECK)->EnableWindow(FALSE);
}

void CMFCP1LDDlg::CRLDInformationwindowfalse()
{
	GetDlgItem(IDC_IMEISTART_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_IMEIEND_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_READCURRENTIMEI_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_READIMEI_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_OPENLDSYSTEM_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_SELECTMODEL_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_OPENMODEL_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_SELECTALLDATA_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_SELECTREPETDATA_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_SELECTMISSINGDATA_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_SETIMEIRANGE_BUTTON)->EnableWindow(FALSE);
}

void CMFCP1LDDlg::LDSystemInformationwindowfalse()
{
	GetDlgItem(IDC_LDONECE_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_STEPLD_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_CLOSELDSYSTEM_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_SCANLD_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_SCANMODE_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_UPDATEZHIDAN_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_ZHIDANAUTO_COMBO)->EnableWindow(TRUE);
	GetDlgItem(IDC_DBCONFIG_BUTTON)->EnableWindow(TRUE);
}


//初始化配置模块
//数据库配置按钮
void CMFCP1LDDlg::OnBnClickedDbconfigButton()
{
	// TODO:  在此添加控件通知处理程序代码
	INT_PTR nRes;             // 用于保存DoModal函数的返回值   

	CDBconfig dbconfigDlg;           // 构造对话框类CTipDlg的实例   
	nRes = dbconfigDlg.DoModal();  // 弹出对话框

	if (IDCANCEL == nRes)
		return;
}

//选择模版按钮
void CMFCP1LDDlg::OnBnClickedSelectmodleButton()
{
	// TODO:  在此添加控件通知处理程序代码
	//设置过滤器
	TCHAR szFilter[] = _T("镭雕模版文件(*.ezd)|*.ezd|所有文件(*.*)|*.*||");
	//构造打开文件对话框
	CFileDialog fileDlg(TRUE, _T("ezd"), NULL, 0, szFilter, this);

	CString strFilePath;

	if (IDOK == fileDlg.DoModal())
	{
		// 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里
		strFilePath = fileDlg.GetPathName();
		SetDlgItemText(IDC_MODELPATH_EDIT, strFilePath);

		//将路径信息保存到信息文件中
		//WritePrivateProfileString(_T("ModelPathName"), _T("modelpathname"), strFilePath, _T(".\\SystemInfo.ini"));
	}
}

//打开模版按钮
void CMFCP1LDDlg::OnBnClickedOpenmodleButton()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strFilePath;

	GetDlgItemText(IDC_MODELPATH_EDIT, strFilePath);
	ShellExecute(NULL, _T("open"), strFilePath, NULL, NULL, SW_SHOWNORMAL);
}

//给combox添加数据库中的订单号，更新按钮和开启后自动初始化都用这个函数
void CMFCP1LDDlg::InitComboBox()
{
	m_zhidanautoCombo.ResetContent();
	ADOManage adomanage;
	adomanage.ConnSQL();
	adomanage.GetZhiDan();
	while (!adomanage.m_pRecordSet->adoEOF)
	{
		m_zhidanautoCombo.AddString(adomanage.m_pRecordSet->GetCollect("ZhiDan").bstrVal);
		adomanage.m_pRecordSet->MoveNext();
	}
	adomanage.CloseAll();
}

//根据制单号获取IMEI和模版路径
void CMFCP1LDDlg::GetImeiByZhidan()
{
	_variant_t imeitemp;//用来放getcollect变量的
	ADOManage adomanage;
	adomanage.ConnSQL();
	adomanage.GetIMEIByZhiDan(ZhiDanNO);
	SetDlgItemText(IDC_MODELPATH_EDIT, adomanage.m_pRecordSet->GetCollect("LD_template1").bstrVal);
	IMEIStaStart = adomanage.m_pRecordSet->GetCollect("IMEIStart").bstrVal;
	IMEIStaEnd = adomanage.m_pRecordSet->GetCollect("IMEIEnd").bstrVal;
	SetDlgItemText(IDC_IMEISTARTSHOWONLY_EDIT, IMEIStaStart);
	SetDlgItemText(IDC_IMEIENDSHOWONLY_EDIT, IMEIStaEnd);
	SetDlgItemText(IDC_IMEISTART_EDIT, IMEIStaStart);
	SetDlgItemText(IDC_IMEIEND_EDIT, IMEIStaEnd);

	//用_variant_t类型才能兼容判断这个字段是否为空
	imeitemp = adomanage.m_pRecordSet->GetCollect("LDIMEICurrent");
	
	//如果最后一个镭雕号码为NULL就将开始号放到当前镭雕号码中，如果不为空就将最后一个镭雕号放入到当前镭雕号码和起始号中
	if (imeitemp.vt == VT_NULL)
	{
		IMEIStaCRStart = IMEIStaStart;
		IMEIStaCREnd = IMEIStaEnd;
		SetDlgItemText(IDC_CURRENTIMEI_EDIT, IMEIStaStart);
	}
	else
	{
		IMEIStaCRStart = adomanage.m_pRecordSet->GetCollect("LDIMEICurrent").bstrVal;
        IMEIStaCREnd = IMEIStaEnd;
		SetDlgItemText(IDC_IMEISTART_EDIT, IMEIStaCRStart);
		SetDlgItemText(IDC_CURRENTIMEI_EDIT, IMEIStaCRStart);
	}

	adomanage.CloseAll();
}

//更新制单号按钮
void CMFCP1LDDlg::OnBnClickedUpdatezhidanButton()
{
	// TODO:  在此添加控件通知处理程序代码
	SetDlgItemText(IDC_IMEISTARTSHOWONLY_EDIT, L"");
	SetDlgItemText(IDC_IMEIENDSHOWONLY_EDIT, L"");
	SetDlgItemText(IDC_MODELPATH_EDIT, L"");
	SetDlgItemText(IDC_IMEISTART_EDIT, L"");
	SetDlgItemText(IDC_IMEIEND_EDIT, L"");
	SetDlgItemText(IDC_CURRENTIMEI_EDIT, L"");
	SetDlgItemText(IDC_IMEINUMBER_EDIT, L"");
	SetDlgItemText(IDC_HDIMEINUMBER_EDIT, L"");
	SetDlgItemText(IDC_HDREIMEINUMBER_EDIT, L"");
	SetDlgItemText(IDC_SCANLD_EDIT, L"");

	InitComboBox();

	GetDlgItem(IDC_MODELPATH_EDIT)->EnableWindow(FALSE);

	GetDlgItem(IDC_IMEI_CHECK)->EnableWindow(FALSE);
	GetDlgItem(IDC_NORMOLLD_RADIO)->EnableWindow(FALSE);
	GetDlgItem(IDC_RELD_RADIO)->EnableWindow(FALSE);
	GetDlgItem(IDC_PARITY_CHECK)->EnableWindow(FALSE);

	CRLDInformationwindowfalse();
	LDSystemInformationwindowfalse();
}

//控制选择订单号后的软件行为
void CMFCP1LDDlg::ZhiDanControl()
{
	//获取选择
	int nSel;
	nSel = m_zhidanautoCombo.GetCurSel();
	m_zhidanautoCombo.GetLBText(nSel, ZhiDanNO);
	SetDlgItemText(IDC_ZHIDANAUTO_COMBO, ZhiDanNO);//选中的那一瞬间是没有及时更新到编辑框的，所以得先获取临时数据后更新到编辑框中

	//制单号不存在就禁用某些窗口并且将LdVariable重置，切换制单号也重置LdVariable
	if (JudgeZhidan(ZhiDanNO))
	{
		MessageBox(L"制单号不存在！", L"提示信息", NULL);
		SetDlgItemText(IDC_IMEISTARTSHOWONLY_EDIT, L"");
		SetDlgItemText(IDC_IMEIENDSHOWONLY_EDIT, L"");
		SetDlgItemText(IDC_MODELPATH_EDIT, L"");
		SetDlgItemText(IDC_IMEISTART_EDIT, L"");
		SetDlgItemText(IDC_IMEIEND_EDIT, L"");
		SetDlgItemText(IDC_CURRENTIMEI_EDIT, L"");
		SetDlgItemText(IDC_IMEINUMBER_EDIT, L"");
		SetDlgItemText(IDC_HDIMEINUMBER_EDIT, L"");
		SetDlgItemText(IDC_HDREIMEINUMBER_EDIT, L"");
		SetDlgItemText(IDC_SCANLD_EDIT, L"");

		InitComboBox();

		GetDlgItem(IDC_MODELPATH_EDIT)->EnableWindow(FALSE);

		GetDlgItem(IDC_IMEI_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_NORMOLLD_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_RELD_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_PARITY_CHECK)->EnableWindow(FALSE);
		CheckDlgButton(IDC_RELD_RADIO, 0);
		CheckDlgButton(IDC_IMEI_CHECK, 1);
		CheckDlgButton(IDC_NORMOLLD_RADIO, 1);
		CheckDlgButton(IDC_PARITY_CHECK, 1);

		CRLDInformationwindowfalse();
		LDSystemInformationwindowfalse();

		for (int i = 0; i < 8; i++)
		{
			LdVariableInt[i] = -1;
		}

		LdVariableInt[0] = 0;
		LdVariableCount = 1;

		return;
	}
	GetDlgItem(IDC_IMEI_CHECK)->EnableWindow(TRUE);
	GetDlgItem(IDC_NORMOLLD_RADIO)->EnableWindow(TRUE);
	GetDlgItem(IDC_RELD_RADIO)->EnableWindow(TRUE);
	GetDlgItem(IDC_PARITY_CHECK)->EnableWindow(TRUE);
	CheckDlgButton(IDC_RELD_RADIO, 0);
	CheckDlgButton(IDC_IMEI_CHECK, 1);
	CheckDlgButton(IDC_NORMOLLD_RADIO, 1);
	CheckDlgButton(IDC_PARITY_CHECK, 1);

	GetDlgItem(IDC_MODELPATH_EDIT)->EnableWindow(TRUE);

	for (int i = 0; i < 8; i++)
	{
		LdVariableInt[i] = -1;
	}
	LdVariableInt[0] = 0;
	LdVariableCount = 1;

	CRLDInformationwindowtrue();
	GetImeiByZhidan();
	GetDataNumber();
}

//选择制单号后会做的事情
void CMFCP1LDDlg::OnCbnSelchangeZhidanautoCombo()
{
	// TODO:  在此添加控件通知处理程序代码
	ZhiDanControl();
}

//制单号框失去焦点后会做的事情
void CMFCP1LDDlg::OnCbnKillfocusZhidanautoCombo()
{
	// TODO:  在此添加控件通知处理程序代码
	ZhiDanControl();
}

//判断制单号是否存在
BOOL CMFCP1LDDlg::JudgeZhidan(CString zhidan)
{
	_RecordsetPtr recordzhidan;
	int flag = 1;
	ADOManage adomanage;
	adomanage.ConnSQL();
	recordzhidan = adomanage.JudgeZhiDan(zhidan);
	if (!recordzhidan->adoEOF)
	{
		flag = 0;
	}
	adomanage.CloseAll();
	return flag;
}

//获取本机IP和地址
int CMFCP1LDDlg::GetLocalHostIPName(CString &sLocalName, CString &sIpAddress)
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


//镭雕模式选择模块
//点击正常镭雕模式
void CMFCP1LDDlg::OnBnClickedNormolldRadio()
{
	// TODO:  在此添加控件通知处理程序代码
	radioflag = TRUE;
}

//点击重新镭雕模式
void CMFCP1LDDlg::OnBnClickedReldRadio()
{
	// TODO:  在此添加控件通知处理程序代码
	radioflag = FALSE;
}

//点击IMEI复选框
void CMFCP1LDDlg::OnBnClickedImeiCheck()
{
	// TODO:  在此添加控件通知处理程序代码
	//如果IMEI复选框被取消，则将LdVariableInt中代表IMEI的0标志变为-1，如果被选中则往VariableInt数组里塞个0进去
	if (!((CButton *)GetDlgItem(IDC_IMEI_CHECK))->GetCheck())
	{
		for (int i = 0; i < 8; i++)
		{
			if (LdVariableInt[i] == 0)
			{
				LdVariableInt[i] = -1;
				LdVariableCount--;
				break;
			}
			i++;
		}
	}
	else
	{
		for (int i = 0; i < 8;i++)
		{
			if (LdVariableInt[i] == -1)
			{
				LdVariableInt[i] = 0;
				LdVariableCount++;
				break;
			}
			i++;
		}
	}
}

//点击生成IMEI校验位复选框
void CMFCP1LDDlg::OnBnClickedParityCheck()
{
	// TODO:  在此添加控件通知处理程序代码
	//如果生成IMEI校验位复选框被取消，则将IMEI15Flag标志变为0，如果被选中置为1
	if (!((CButton *)GetDlgItem(IDC_PARITY_CHECK))->GetCheck())
	{
		IMEI15Flag = 0;
	}
	else
	{
		IMEI15Flag = 1;
	}
}


//当前镭雕信息模块
//点击读上一镭雕号
void CMFCP1LDDlg::OnBnClickedReadcurrentimeiButton()
{
	// TODO:  在此添加控件通知处理程序代码
	_variant_t imeitemp;//用来放getcollect变量的
	ADOManage adomanage;
	adomanage.ConnSQL();
	adomanage.GetIMEIByZhiDan(ZhiDanNO);

	//用_variant_t类型才能兼容判断这个字段是否为空
	imeitemp = adomanage.m_pRecordSet->GetCollect("LDIMEICurrent");

	//如果最后一个镭雕号码为NULL就将开始号放到当前镭雕号码中，如果不为空就将最后一个镭雕号放入到当前镭雕号码和起始号中
	if (imeitemp.vt == VT_NULL)
	{
		IMEIStaCRStart = IMEIStaStart;
		IMEIStaCREnd = IMEIStaEnd;
		SetDlgItemText(IDC_CURRENTIMEI_EDIT, IMEIStaStart);
	}
	else
	{
		IMEIStaCRStart = adomanage.m_pRecordSet->GetCollect("LDIMEICurrent").bstrVal;
		IMEIStaCREnd = IMEIStaEnd;
		SetDlgItemText(IDC_IMEISTART_EDIT, IMEIStaCRStart);
		SetDlgItemText(IDC_CURRENTIMEI_EDIT, IMEIStaCRStart);
	}
	GetDataNumber();
	adomanage.CloseAll();
}

//点击重置，则将当前镭雕信息中的IMEI起始号和结束号重置为制单号中原本的IMEI号和起始号
void CMFCP1LDDlg::OnBnClickedReadimeiButton()
{
	// TODO:  在此添加控件通知处理程序代码
	ADOManage adomanage;
	adomanage.ConnSQL();
	adomanage.GetIMEIByZhiDan(ZhiDanNO);
	IMEIStaStart = adomanage.m_pRecordSet->GetCollect("IMEIStart").bstrVal;
	IMEIStaEnd = adomanage.m_pRecordSet->GetCollect("IMEIEnd").bstrVal;
	SetDlgItemText(IDC_IMEISTART_EDIT, IMEIStaStart);
	SetDlgItemText(IDC_IMEIEND_EDIT, IMEIStaEnd);
	SetDlgItemText(IDC_CURRENTIMEI_EDIT, IMEIStaStart);
	IMEIStaCRStart = IMEIStaStart;
	IMEIStaCREnd = IMEIStaEnd;
	GetDataNumber();
	adomanage.CloseAll();
}

//判断IMEI起始号是否正确
void CMFCP1LDDlg::OnEnKillfocusImeistartEdit()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str1, str2, str3;

	unsigned long long a, b;
	GetDlgItemText(IDC_IMEISTART_EDIT, str1);
	GetDlgItemText(IDC_IMEIEND_EDIT, str2);
	if (str2 == "")
	{
		return;
	}
	if (str1 == "")
	{
		SetDlgItemText(IDC_IMEISTART_EDIT, IMEIStaCRStart);
		MessageBox(_T("号段不能为空！"), _T("提示信息"), NULL);
		return;
	}
	a = _atoi64(CStringA(str1));
	b = _atoi64(CStringA(str2));

	if (a < _atoi64(CStringA(IMEIStaStart)))
	{
		SetDlgItemText(IDC_IMEISTART_EDIT, IMEIStaCRStart);
		MessageBox(_T("号段比订单中起始号小！请重新设置！"), _T("提示信息"), NULL);
		return;
	}
	else if (a > _atoi64(CStringA(IMEIStaEnd)))
	{
		SetDlgItemText(IDC_IMEISTART_EDIT, IMEIStaCRStart);
		MessageBox(_T("号段比订单中结束号大！请重新设置！"), _T("提示信息"), NULL);
		return;
	}
	else if (a > b)
	{
		SetDlgItemText(IDC_IMEISTART_EDIT, IMEIStaCRStart);
		MessageBox(_T("号段比当前设置的结束号大！请重新设置！"), _T("提示信息"), NULL);
		return;
	}

	if (radioflag == FALSE)
	{
		ADOManage adomanage;
		adomanage.ConnSQL();
		str3 = CreateIMEI15(str1);
		adomanage.JudgeIMEIExit(str3);
		if (adomanage.m_pRecordSet->adoEOF)
		{
			MessageBox(L"设置的起始号还未镭雕过！", L"提示信息", NULL);
			SetDlgItemText(IDC_IMEISTART_EDIT, IMEIStaCRStart);
			return;
		}
		adomanage.CloseAll();
	}

	IMEIStaCRStart = str1;
	SetDlgItemText(IDC_IMEISTART_EDIT, IMEIStaCRStart);
	GetDataNumber();
}

//判断IMEI结束号是否正确
void CMFCP1LDDlg::OnEnKillfocusImeiendEdit()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str1, str2, str3;

	unsigned long long a, b;
	GetDlgItemText(IDC_IMEISTART_EDIT, str1);
	GetDlgItemText(IDC_IMEIEND_EDIT, str2);
	if (str2 == "")
	{
		SetDlgItemText(IDC_IMEIEND_EDIT, IMEIStaCREnd);
		MessageBox(_T("号段不能为空！"), _T("提示信息"), NULL);
		return;
	}
	a = _atoi64(CStringA(str1));
	b = _atoi64(CStringA(str2));

	if (b < _atoi64(CStringA(IMEIStaStart)))
	{
		SetDlgItemText(IDC_IMEIEND_EDIT, IMEIStaCREnd);
		MessageBox(_T("号段比订单中起始号小！请重新设置！"), _T("提示信息"), NULL);
		return;
	}
	else if (b > _atoi64(CStringA(IMEIStaEnd)))
	{
		SetDlgItemText(IDC_IMEIEND_EDIT, IMEIStaCREnd);
		MessageBox(_T("号段比订单中结束号大！请重新设置！"), _T("提示信息"), NULL);
		return;
	}
	else if (b < a)
	{
		SetDlgItemText(IDC_IMEIEND_EDIT, IMEIStaCREnd);
		MessageBox(_T("号段比当前设置的起始号小！请重新设置！"), _T("提示信息"), NULL);
		return;
	}

	if (radioflag == FALSE)
	{
		ADOManage adomanage;
		adomanage.ConnSQL();
		str3 = CreateIMEI15(str2);
		adomanage.JudgeIMEIExit(str3);
		if (adomanage.m_pRecordSet->adoEOF)
		{
			MessageBox(L"设置的结束号还未镭雕过！", L"提示信息", NULL);
			SetDlgItemText(IDC_IMEIEND_EDIT, IMEIStaCREnd);
			return;
		}
		adomanage.CloseAll();
	}

	IMEIStaCREnd = str2;
	SetDlgItemText(IDC_IMEIEND_EDIT, IMEIStaCREnd);
	GetDataNumber();
}

//获取镭雕数据量
void CMFCP1LDDlg::GetDataNumber()
{
	CString str1, str2, strorder;
	int x, y, z;
	GetDlgItemText(IDC_IMEISTART_EDIT, str1);
	GetDlgItemText(IDC_IMEIEND_EDIT, str2);


	StrToIntEx((LPCTSTR)str1, STIF_DEFAULT, &x);
	StrToIntEx((LPCTSTR)str2, STIF_DEFAULT, &y);

	z = y - x + 1;

	SetDlgItemInt(IDC_IMEINUMBER_EDIT, z);


	ADOManage adomanage;
	adomanage.ConnSQL();
	int nSel;
	nSel = m_zhidanautoCombo.GetCurSel();
	m_zhidanautoCombo.GetLBText(nSel, strorder);
	int a = adomanage.GetImeicount(strorder, str1, str2);
	SetDlgItemInt(IDC_HDIMEINUMBER_EDIT, a);
	SetDlgItemInt(IDC_HDREIMEINUMBER_EDIT, 0);
	adomanage.CloseAll();
}

//设置镭雕号段
void CMFCP1LDDlg::OnBnClickedSetimeirangeButton()
{
	// TODO:  在此添加控件通知处理程序代码
	INT_PTR nRes;             // 用于保存DoModal函数的返回值   

	CSetImeiRangeDlg SetImeiRangeDlg;
	nRes = SetImeiRangeDlg.DoModal();

	//当对话框那边点击确定退出来后
	if (IDCANCEL == nRes)
	{
		GetDataNumber();
		return;
	}
	CDialogEx::OnCancel();
}


//镭雕系统模块
//打开镭雕系统按钮
void CMFCP1LDDlg::OnBnClickedOpenldsystemButton()
{
	// TODO:  在此添加控件通知处理程序代码

	if (radioflag == FALSE)
	{
		INT_PTR nRes;
		nRes = MessageBox(_T("您确定要重新镭雕吗？"), _T("提示"), MB_OKCANCEL | MB_ICONQUESTION);
		// 判断消息对话框返回值。如果为IDCANCEL就return，否则继续向下执行   
		if (IDCANCEL == nRes)
			return;
	}

	//判断是否选择至少一个字段
	if (LdVariableCount == 0)
	{
		MessageBox(_T("请至少选择一个镭雕字段！"), _T("错误提示"), NULL);
		return;
	}


	//判断IMEI号是否为空
	CString str1, str2, str3, str4;

	GetDlgItemText(IDC_ZHIDANAUTO_COMBO, str1);
	GetDlgItemText(IDC_IMEISTART_EDIT, str2);
	GetDlgItemText(IDC_IMEIEND_EDIT, str3);
	if (str1 == "" || str2 == "" || str3 == "")
	{
		MessageBox(_T("IMEI信息不全，请订单号、IMEI起始号和结束号填写完整！！"), _T("错误提示"), NULL);
		return;
	}

	//再判断重新镭雕的号段是否已经镭过
	if (radioflag == FALSE)
	{
		ADOManage adomanage;
		adomanage.ConnSQL();
		str4 = CreateIMEI15(str2);
		if (!adomanage.JudgeIMEIExit(str4))
		{
			MessageBox(L"设置的起始号还未镭雕过！", L"提示信息", NULL);
			adomanage.CloseAll();
			return;
		}

		str4 = CreateIMEI15(str3);
		adomanage.JudgeIMEIExit(str4);
		if (!adomanage.JudgeIMEIExit(str4))
		{
			MessageBox(L"设置的结束号还未镭雕过！", L"提示信息", NULL);
			adomanage.CloseAll();
			return;
		}

		adomanage.CloseAll();
	}

	//先初始化
	//这镭雕软件启动时间有点长，建立一个非模态对话框来提示，等系统启动完毕之后就这个对话框自动关闭
	if (NULL == m_pCLDStartDlg)
	{
		m_pCLDStartDlg = new CLDStartDlg();
		m_pCLDStartDlg->Create(IDD_LDSTART_DIALOG, this);
	}
	m_pCLDStartDlg->ShowWindow(SW_SHOW);
	WindowTimeClose(L"镭雕系统启动中请稍等...", 100);
	//MessageBox(L"镭雕系统启动时间大概需要十几秒，请耐心等待！", L"提示信息", MB_OK);
	//SetRicheditText(L"镭雕系统正在启动中，请稍等...", 0);
	
	DllFlag = markezd.lmc1_Initial(L"g:\\VS_PROJECT\\EzCad", TRUE, AfxGetMainWnd()->m_hWnd);
	//DllFlag = markezd.lmc1_Initial(L"", FALSE, AfxGetMainWnd()->m_hWnd);

	//启动完毕后关闭掉上面那个模态对话框
	CWnd* hWnd = FindWindow(NULL, _T("提示信息"));
	if (hWnd)
	{
		hWnd->PostMessage(WM_CLOSE, NULL, NULL);
	}


	DllStr.Format(_T("%d"), DllFlag);

	if (DllFlag == 0)
	{
		SetRicheditText(L"镭雕系统启动成功", 0);
	}
	else
	{
		SetRicheditText(L"镭雕系统启动失败，错误代码" + DllStr, 1);
		return;
	}

	//读镭雕模版
	CString strmodlepath;
	GetDlgItemText(IDC_MODELPATH_EDIT, strmodlepath);

	DllFlag = markezd.lmc1_LoadEzdFile((LPTSTR)(LPCTSTR)(strmodlepath));//从CString转换成TCHAR*类型，要转两次

	DllStr.Format(_T("%d"), DllFlag);

	if (DllFlag == 0)
	{
		SetRicheditText(L"镭雕模版读取成功", 0);
	}
	else
	{
		SetRicheditText(L"镭雕模版读取失败,请检查模版路径是否有误，错误代码" + DllStr, 1);
		OnBnClickedCloseldsystemButton();
		return;
	}

	
	//检查镭雕模版中是否存在指定对象名称，此函数找得到对象名称会返回0，找不到就返回1并直接关闭镭雕系统
	if (FindLdName())
	{
		OnBnClickedCloseldsystemButton();
		return;
	}
    
    //然后将镭雕系统一些控件获得使能
	LDSystemInformationwindowtrue();
    //同时禁用掉其它的一些控件
	CRLDInformationwindowfalse();
	InitInformationwindowfalse();

	SetDlgItemInt(IDC_HDREIMEINUMBER_EDIT, 0);//每次开启系统都要将重新镭雕数量置为0
}

//寻找镭雕对象名称
int CMFCP1LDDlg::FindLdName()
{
	double a, b, c, d, e;
	//一个一个去找是否存在对象名称，找不到就返回1，找得到就返回0
	for (int i = 0; i < LdVariableCount; i++)
	{
		DllFlag = markezd.lmc1_GetEntSize((LPTSTR)(LPCTSTR)LdVariableChar[i], a, b, c, d, e);
		if (DllFlag != 0)
		{
			DllStr.Format(_T("%d"), DllFlag);
			SetRicheditText(L"镭雕系统启动失败，错误代码" + DllStr, 1);
			MessageBox(L"找不到对象" + LdVariableChar[i], _T("错误信息"), NULL);
			return 1;
		}
	}
	return 0;
}

//替换指定对象名称的值
int CMFCP1LDDlg::ChangeLdName(CString LdVariable, CString strld)
{
	DllFlag = markezd.lmc1_ChangeTextByName((LPTSTR)(LPCTSTR)LdVariable, (LPTSTR)(LPCTSTR)strld);
	DllStr.Format(_T("%d"), DllFlag);
	if (DllFlag == 0)
	{
		SetRicheditText(L"镭雕中...", 0);
	}
	else
	{
		SetRicheditText(L"替换对象的值失败，错误代码" + DllStr, 1);
		return 0;
	}
	return 1;
}

//开始镭雕函数
int CMFCP1LDDlg::LdCore()
{
	CString StrImeiLd; //镭雕用的变量
	int imeicount, imeirecount;
	ADOManage adomanage;
	adomanage.ConnSQL();

	//有无校验位要区分开
	if (IMEI15Flag == 1)
	{
	StrImeiLd = CreateIMEI15(IMEIStaCRStart);
	}
	else if (IMEI15Flag == 0)
	{
		StrImeiLd = IMEIStaCRStart;
	}

	//判断制单号是否为空
	if (JudgeZhidan(ZhiDanNO))
	{
		MessageBox(L"订单号不存在！", L"提示信息", NULL);
		adomanage.CloseDB();
		return 0;
	}

    //在正常镭雕的情况下,返回0代表IMEI不存在,就不用执行while，返回1就直接给IMEI号+1，一直到不重复为止
	while (adomanage.JudgeIMEIExit(StrImeiLd)&&radioflag==TRUE)
	{
		//先判断IMEI号段是否已经全部镭雕完
		if (IMEIStaCRStart > IMEIStaCREnd)
		{
			SetRicheditText(_T("本次IMEI号段已全部镭雕完毕"), 0);
			MessageBox(L"本次IMEI号段已全部镭雕完毕！", L"提示信息", NULL);
			OnBnClickedCloseldsystemButton();
			adomanage.CloseAll();
			return 0;
		}

		//如果数据库已经存在，先显示重复信息，接着IMEI号+1
		SetRicheditText(_T("IMEI号") + StrImeiLd + _T("已经存在"), 1);

		unsigned long long imeiint = 0;
		imeiint = _atoi64(CStringA(IMEIStaCRStart)) + 1;
		IMEIStaCRStart = _ui64toa(imeiint, CT2A(IMEIStaCRStart), 10);

		//然后更新当前镭雕信息，有无校验位要区分开
		if (IMEI15Flag == 1)
		{
			StrImeiLd = CreateIMEI15(IMEIStaCRStart);
			SetDlgItemText(IDC_CURRENTIMEI_EDIT, IMEIStaCRStart);//显示当前要镭雕的IMEI号
		}
		else if (IMEI15Flag == 0)
		{
			StrImeiLd = IMEIStaCRStart;
			SetDlgItemText(IDC_CURRENTIMEI_EDIT, IMEIStaCRStart);
		}
	}
	
	//先给模版赋值
	for (int i = 0; i < LdVariableCount; i++)
	{
		switch (i)
		{
		case 0:
			ChangeLdName(LdVariableChar[i], StrImeiLd);
			break;
		case 1:
	        ChangeLdName(LdVariableChar[i], L"SN");
			break;
		default:
			break;
		}
	}

	//然后开始镭雕
	DllFlag = markezd.lmc1_Mark(TRUE);
	DllStr.Format(_T("%d"), DllFlag);

	if (DllFlag == 0)
	{
		InsertData(StrImeiLd);//正常\重新镭雕插入更新操作集成
		SetRicheditText(L"IMEI号:" + StrImeiLd + L"已镭雕完成", 0);

		//镭雕完后IMEI号+1
		unsigned long long imeiint = 0;
		imeiint = _atoi64(CStringA(IMEIStaCRStart)) + 1;
		IMEIStaCRStart = _ui64toa(imeiint, CT2A(IMEIStaCRStart), 10);

		//然后更新当前镭雕信息
		StrImeiLd = CreateIMEI15(IMEIStaCRStart);
		SetDlgItemText(IDC_CURRENTIMEI_EDIT, IMEIStaCRStart);//显示当前要镭雕的IMEI号

		//更新已处理数据量，如果是重新镭雕则更新本次重镭数量
		if (radioflag == TRUE)
		{
			imeicount = GetDlgItemInt(IDC_HDIMEINUMBER_EDIT);
			imeicount++;
			SetDlgItemInt(IDC_HDIMEINUMBER_EDIT, imeicount);
		}
		else
		{
			imeirecount = GetDlgItemInt(IDC_HDREIMEINUMBER_EDIT);
			imeirecount++;
			SetDlgItemInt(IDC_HDREIMEINUMBER_EDIT, imeirecount);
		}

		//接着再判断IMEI号段是否已经全部镭雕完
		if (IMEIStaCRStart > IMEIStaCREnd)
		{
			SetRicheditText(_T("本次IMEI号段已全部镭雕完毕"), 0);
			MessageBox(L"本次IMEI号段已全部镭雕完毕！", L"提示信息", NULL);
			OnBnClickedCloseldsystemButton();
			adomanage.CloseAll();
			return 0;
		}
	}
	else
	{
		SetRicheditText(L"镭雕失败，错误代码" + DllStr, 1);
		adomanage.CloseAll();
		return 0;
	}
	return 1;
}

//镭雕一次按钮
void CMFCP1LDDlg::OnBnClickedLdoneceButton()
{
	// TODO:  在此添加控件通知处理程序代码
	LdCore();
}

//脚踏或旋转打标模式按钮（未完成，得明天亲自去调试）
void CMFCP1LDDlg::OnBnClickedStepldButton()
{
	// TODO:  在此添加控件通知处理程序代码
	if (STEPLDFlag == 1)
	{
		SetDlgItemText(IDC_STEPLD_BUTTON, _T("停止脚踏或旋转打标"));
		GetDlgItem(IDC_SCANLD_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SCANMODE_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_LDONECE_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_CLOSELDSYSTEM_BUTTON)->EnableWindow(FALSE);
		STEPLDFlag = 0;
		//开启子线程
		LdReadPortThread = AfxBeginThread(ThreadReadLdPort, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		return;
	}
	else
	{
		SetDlgItemText(IDC_STEPLD_BUTTON, _T("脚踏或旋转打标模式"));
		GetDlgItem(IDC_SCANLD_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SCANMODE_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_LDONECE_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_CLOSELDSYSTEM_BUTTON)->EnableWindow(TRUE);
		STEPLDFlag = 1;
		m_LdPortRun = FALSE;
		return;
	}
}

//读踏板或者旋转串口用的子线程
UINT ThreadReadLdPort(LPVOID pParam)
{
	CMFCP1LDDlg* dlg;
	dlg = (CMFCP1LDDlg*)pParam;
	m_LdPortRun = TRUE;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程
	while (m_LdPortRun)
	{
		dlg->markezd.lmc1_ReadPort(dlg->LdPort);
		if (dlg->LdPort == 141)
		{
			dlg->OnBnClickedLdoneceButton();
		}
		Sleep(500);//停止0.5秒然后接着跑
	}
	
	return 0;
}

//关闭系统按钮
void CMFCP1LDDlg::OnBnClickedCloseldsystemButton()
{
	// TODO:  在此添加控件通知处理程序代码
	DllFlag = markezd.lmc1_Close();

	DllStr.Format(_T("%d"), DllFlag);

	if (DllFlag == 0)
	{
		SetRicheditText(L"关闭镭雕系统成功", 0);
	}
	else
	{
		SetRicheditText(L"关闭镭雕系统失败，错误代码" + DllStr, 1);
		return;
	}

	//然后将镭雕系统一些控件获得使能
	LDSystemInformationwindowfalse();
	//同时禁用掉其它的一些控件
	CRLDInformationwindowtrue();
	InitInformationwindowtrue();
}

//点击扫描枪模式按钮
void CMFCP1LDDlg::OnBnClickedScanmodeButton()
{
	// TODO:  在此添加控件通知处理程序代码
	if (ScanFlag == 1)
	{
		SetDlgItemText(IDC_SCANMODE_BUTTON, _T("停止扫描枪模式"));
		GetDlgItem(IDC_STEPLD_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_LDONECE_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_CLOSELDSYSTEM_BUTTON)->EnableWindow(FALSE);
		ScanFlag = 0;
		GetDlgItem(IDC_SCANLD_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SCANLD_EDIT)->SetFocus();
	}
	else
	{
		SetDlgItemText(IDC_SCANMODE_BUTTON, _T("进入扫描枪模式"));
		GetDlgItem(IDC_STEPLD_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_LDONECE_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_CLOSELDSYSTEM_BUTTON)->EnableWindow(TRUE);
		ScanFlag = 1;
		GetDlgItem(IDC_SCANLD_EDIT)->EnableWindow(FALSE);
		SetDlgItemText(IDC_SCANLD_EDIT, L"");
	}
}

//按键响应函数的重写
BOOL CMFCP1LDDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		int nVirtKey = (int)pMsg->wParam;

        //当ScanFlag为0的时候代表此时已经启动扫描枪模式
		if (nVirtKey == VK_RETURN&&ScanFlag == 0)
		{
			//如果是回车在这里做你要做的事情
			if (pMsg->hwnd == GetDlgItem(IDC_SCANLD_EDIT)->GetSafeHwnd())
			{
				CString strimei;
				GetDlgItem(IDC_SCANLD_EDIT)->EnableWindow(FALSE);
				GetDlgItemText(IDC_SCANLD_EDIT, strimei);
				ScanInsertData(strimei);
				SetDlgItemText(IDC_SCANLD_EDIT, L"");
				GetDlgItem(IDC_SCANLD_EDIT)->EnableWindow(TRUE);
				GetDlgItem(IDC_SCANLD_EDIT)->SetFocus();
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

//正常\重新镭雕插入更新操作集成
void CMFCP1LDDlg::InsertData(CString strimei)
{
	ADOManage adomanage;
	adomanage.ConnSQL();

	//如果此刻单选框是正常镭雕，那就执行插入语句，如果是重新镭雕， 那就执行更新语句
	if (((CButton*)GetDlgItem(IDC_NORMOLLD_RADIO))->GetCheck() == TRUE)
	{
		adomanage.AddNew(m_pcipEdit, ZhiDanNO, strimei);
		if (IMEI15Flag == 1)
		{
			adomanage.UpdateIMEICurrent(ZhiDanNO, strimei.Left(strimei.GetLength() - 1));
		}
		else if (IMEI15Flag == 0)
		{
			adomanage.UpdateIMEICurrent(ZhiDanNO, strimei);
		}
	}
	else if (((CButton*)GetDlgItem(IDC_RELD_RADIO))->GetCheck() == TRUE)
	{
		adomanage.UpdateReld(strimei);
	}

	adomanage.CloseAll();
}

//扫描枪的数据库插入操作
void CMFCP1LDDlg::ScanInsertData(CString strimei)
{
	CString StrImeiTemp; //临时变量
	int imeicount, imeirecount;
	ADOManage adomanage;
	adomanage.ConnSQL();

	//有无校验位要区分开，无校验位不用把最后一位干掉
	if (IMEI15Flag == 1)
	{
		StrImeiTemp = strimei.Left(strimei.GetLength() - 1);
	}
	else if (IMEI15Flag == 0)
	{
		StrImeiTemp = strimei;
	}

	//判断制单号是否为空
	if (JudgeZhidan(ZhiDanNO))
	{
		WindowTimeClose(_T("订单号不存在！"), 1500);
		return ;
	}

	//(strimei == CreateIMEI15(StrImeiTemp) || IMEI15Flag == 1)这里面，无校验位的时候就一定为真，即不需要做校验位判断，有校验位就看StrImeiTemp生成校验位后是否与strimei相同
	if (!(StrImeiTemp <= IMEIStaEnd&&StrImeiTemp >= IMEIStaStart&&StrImeiTemp.GetLength() == IMEIStaStart.GetLength() && (strimei == CreateIMEI15(StrImeiTemp) || IMEI15Flag == 0)))
	{
		WindowTimeClose(_T("扫入的IMEI号不在号段范围内！"), 1500);
		return;
	}

	//在正常镭雕的情况下,返回0代表IMEI不存在,返回1就代表IMEI号存在
	if (adomanage.JudgeIMEIExit(strimei) && radioflag == TRUE)
	{
		WindowTimeClose(_T("扫入的IMEI号已存在在数据库中！"), 1500);
		return;
	}
	else if ((!adomanage.JudgeIMEIExit(strimei)) && radioflag == FALSE)
	{
		WindowTimeClose(_T("扫入的IMEI号并未镭雕过！"), 1500);
		return;
	}

	SetDlgItemText(IDC_CURRENTIMEI_EDIT, strimei);//显示当前要镭雕的IMEI号


	//先给模版赋值
	for (int i = 0; i < LdVariableCount; i++)
	{
		switch (i)
		{
		case 0:
			ChangeLdName(LdVariableChar[i], strimei);
			break;
		case 1:
			ChangeLdName(LdVariableChar[i], L"SN");
			break;
		default:
			break;
		}
	}

	//然后开始镭雕
	DllFlag = markezd.lmc1_Mark(TRUE);
	DllStr.Format(_T("%d"), DllFlag);

	if (DllFlag == 0)
	{
		SetRicheditText(L"扫入的IMEI号:" + strimei + L"已镭雕完成", 0);

		//更新已处理数据量，如果是重新镭雕则更新本次重镭数量
		//如果此刻单选框是正常镭雕，那就执行插入语句，如果是重新镭雕，那就执行更新语句
		if (radioflag == TRUE)
		{
			adomanage.AddNew(m_pcipEdit, ZhiDanNO, strimei);
			//adomanage.UpdateIMEICurrent(ZhiDanNO, strimei.Left(strimei.GetLength() - 1));
			imeicount = GetDlgItemInt(IDC_HDIMEINUMBER_EDIT);
			imeicount++;
			SetDlgItemInt(IDC_HDIMEINUMBER_EDIT, imeicount);
		}
		else
		{
			adomanage.UpdateReld(strimei);
			imeirecount = GetDlgItemInt(IDC_HDREIMEINUMBER_EDIT);
			imeirecount++;
			SetDlgItemInt(IDC_HDREIMEINUMBER_EDIT, imeirecount);
		}
	}
	else
	{
		WindowTimeClose(_T("镭雕失败，错误代码") + DllStr, 1500);
		adomanage.CloseDB();
		GetDlgItem(IDC_SCANLD_EDIT)->SetFocus();
		return ;
	}

	adomanage.CloseAll();
}

//生成IMEI校验位
CString CMFCP1LDDlg::CreateIMEI15(CString imei)
{
	long long a = 0;
	int j = 0;
	char imeiChar[14];
	strncpy(imeiChar, CStringA(imei), sizeof(imeiChar));
	//	std::string sz2 = CT2A(imei.GetBuffer());

	int resultInt = 0;
	for (int i = 0; i < 14; i++)
	{
		int a = (int)imeiChar[i] - '0';
		i++;
		int temp = ((int)imeiChar[i] - '0') * 2;
		int b = temp < 10 ? temp : temp - 9;
		resultInt += a + b;
	}
	resultInt %= 10;
	GetLastError();
	resultInt = (resultInt == 0 ? 0 : 10 - resultInt);
	CString resultStr, abc;
	resultStr.Format(_T("%d"), resultInt);
	abc = imei + resultStr;
	imei.ReleaseBuffer();
	return abc;
}

//窗口在N毫秒后自动关闭
void CMFCP1LDDlg::WindowTimeClose(CString str, int strtime)
{
	SetRicheditText(str, 1);
	SetTimer(1, strtime, NULL);//启动定时器1,定时时间是1.5秒
	MessageBox(str + L"(此弹出框会自动关闭)", L"提示信息", NULL);
}

//定时器函数
void CMFCP1LDDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CWnd* hWnd = FindWindow(NULL, _T("提示信息"));
	if (hWnd)
	{
		hWnd->PostMessage(WM_CLOSE, NULL, NULL);
	}
	KillTimer(1);   //关闭定时器
	GetDlgItem(IDC_SCANLD_EDIT)->SetFocus();
	CDialogEx::OnTimer(nIDEvent);
}


//信息日志模块
//根据值来在信息日志中显示不同的信息
void CMFCP1LDDlg::SetRicheditText(CString strMsg, int No)
{
	m_currentRichedit.SetSel(-1, -1);
	CHARFORMAT   cf{ sizeof(cf) };//设置这个结构来改变你想要的文字的颜色等属性
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD | CFM_SIZE | CFM_FACE;//这个目前还没解析完成，但如果要输出不同颜色的字体一定要使用richedit
	cf.crTextColor = RGB(255, 0, 0);//设置为红色

	switch (No)
	{
	case 0:
		m_currentRichedit.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
		m_currentRichedit.LineScroll(m_currentRichedit.GetLineCount() - 1 - 3);
		break;
	case 1:
		m_currentRichedit.SetSelectionCharFormat(cf);
		m_currentRichedit.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
		m_currentRichedit.LineScroll(m_currentRichedit.GetLineCount() - 1);
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
CString CMFCP1LDDlg::GetTime()
{
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d/%d/%d %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	return strTime;
}

//数据库查询操作模块
//点击查询数据表按钮
void CMFCP1LDDlg::OnBnClickedSelectalldataButton()
{
	// TODO:  在此添加控件通知处理程序代码
	//初始化列表控件
	CRect rect;
	// 获取编程语言列表视图控件的位置和大小   
	m_dbcurtainList.GetClientRect(&rect);

	// 为列表视图控件添加全行选中和栅格风格   
	m_dbcurtainList.SetExtendedStyle(m_dbcurtainList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//删除掉所有的表头
	while (1) {
		if (!m_dbcurtainList.DeleteColumn(0)) {
			break;
		}
	}

	// 为列表视图控件添加列   
	m_dbcurtainList.InsertColumn(0, _T("IMEI号"), LVCFMT_CENTER, rect.Width() / 2, 0);
	m_dbcurtainList.InsertColumn(0, _T("镭雕时间"), LVCFMT_CENTER, rect.Width() / 2, 0);
	ShowAllData();
}

//显示已插入的数据
void CMFCP1LDDlg::ShowAllData()
{
	m_dbcurtainList.DeleteAllItems(); // 全部清空
	CString strimeidata;
	int i = 0;

	ADOManage adomanage;
	adomanage.ConnSQL();
	
	adomanage.ShowInsertImeiByOrderNumber(ZhiDanNO);

	while (!adomanage.m_pRecordSet->adoEOF)
	{
		strimeidata = adomanage.m_pRecordSet->GetCollect("IMEI");
		m_dbcurtainList.InsertItem(i, strimeidata);
		strimeidata = adomanage.m_pRecordSet->GetCollect("InputTime");
		m_dbcurtainList.SetItemText(i, 1, strimeidata);
		adomanage.m_pRecordSet->MoveNext();
		i++;
	}

	adomanage.CloseAll();
}

//点击查询重复表按钮
void CMFCP1LDDlg::OnBnClickedSelectrepetdataButton()
{
	// TODO:  在此添加控件通知处理程序代码
	//初始化列表控件
	CRect rect;
	// 获取编程语言列表视图控件的位置和大小   
	m_dbcurtainList.GetClientRect(&rect);

	// 为列表视图控件添加全行选中和栅格风格   
	m_dbcurtainList.SetExtendedStyle(m_dbcurtainList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//删除掉所有的表头
	while (1) {
		if (!m_dbcurtainList.DeleteColumn(0)) {
			break;
		}
	}

	// 为列表视图控件添加列   
	m_dbcurtainList.InsertColumn(0, _T("IMEI号"), LVCFMT_CENTER, rect.Width() / 5 * 2, 0);
	m_dbcurtainList.InsertColumn(1, _T("最后重镭时间"), LVCFMT_CENTER, rect.Width() / 5 * 2, 0);
	m_dbcurtainList.InsertColumn(2, _T("重复次数"), LVCFMT_CENTER, rect.Width() / 5 * 1, 0);
	ShowRepeatData();
}

//显示重复的数据
void CMFCP1LDDlg::ShowRepeatData()
{
	m_dbcurtainList.DeleteAllItems(); // 全部清空
	CString strimeidata;
	int i = 0;

	ADOManage adomanage;
	adomanage.ConnSQL();
	
	adomanage.ShowRepeatImeiByOrderNumber(ZhiDanNO);

	while (!adomanage.m_pRecordSet->adoEOF)
	{
		strimeidata = adomanage.m_pRecordSet->GetCollect("IMEI");
		m_dbcurtainList.InsertItem(i, strimeidata);
		strimeidata = adomanage.m_pRecordSet->GetCollect("ReLdImeiTime");
		m_dbcurtainList.SetItemText(i, 1, strimeidata);
		strimeidata = adomanage.m_pRecordSet->GetCollect("ReLdImeiNum");
		m_dbcurtainList.SetItemText(i, 2, strimeidata);
		adomanage.m_pRecordSet->MoveNext();
		i++;
	}

	adomanage.CloseAll();
}

//点击查询未镭雕号按钮
void CMFCP1LDDlg::OnBnClickedSelectmissingdataButton()
{
	// TODO:  在此添加控件通知处理程序代码
	//初始化列表控件
	CRect rect;
	// 获取编程语言列表视图控件的位置和大小   
	m_dbcurtainList.GetClientRect(&rect);

	// 为列表视图控件添加全行选中和栅格风格   
	m_dbcurtainList.SetExtendedStyle(m_dbcurtainList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//删除掉所有的表头
	while (1) {
		if (!m_dbcurtainList.DeleteColumn(0)) {
			break;
		}
	}

	// 为列表视图控件添加列   
	m_dbcurtainList.InsertColumn(0, _T("号段内IMEI未镭雕号"), LVCFMT_CENTER, rect.Width(), 0);
	ShowUnData();
}

//显示未镭雕的数据
void CMFCP1LDDlg::ShowUnData()
{
	m_dbcurtainList.DeleteAllItems(); // 全部清空
	CString strimeidata;
	int i = 0, imeicount;
	imeicount = GetDlgItemInt(IDC_IMEINUMBER_EDIT);

	if (imeicount >= 2000)
	{
		MessageBox(L"为保证效率，请将号段数据量控制在2000以内", L"提示信息", NULL);
		return;
	}

	//接着开始在数据库中进行查询，并将数据插入到列表中
	ADOManage adomanage;
	adomanage.ConnSQL();

	adomanage.ShowUnImeiByOrderNumber(ZhiDanNO, IMEIStaCRStart, IMEIStaCREnd);

	while (!adomanage.m_pRecordSet->adoEOF)
	{
		strimeidata = adomanage.m_pRecordSet->GetCollect("UNIMEI");
		m_dbcurtainList.InsertItem(i, strimeidata);
		adomanage.m_pRecordSet->MoveNext();
		i++;
	}

	adomanage.CloseAll();
}


//其它函数
//确定按钮
void CMFCP1LDDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}

//退出按钮
void CMFCP1LDDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	INT_PTR nRes;
	nRes = MessageBox(_T("您确定要退出吗？"), _T("提示"), MB_OKCANCEL | MB_ICONQUESTION);
	// 判断消息对话框返回值。如果为IDCANCEL就return，否则继续向下执行   
	if (IDCANCEL == nRes)
		return;

	CDialogEx::OnCancel();
}

