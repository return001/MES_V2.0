// JIMIDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "JIMI.h"
#include "JIMIDlg.h"
#include "afxdialogex.h"
#include<FSTREAM>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <mmsystem.h>
#include "afxdialogex.h"
#include "math.h"



#pragma comment(lib,"winmm.lib")
#pragma warning(disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

map<int, CString>CJIMIDlg::SyllableMap;
map<int, CString>CJIMIDlg::BindMap;
BOOL Imei3Flag;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CJIMIDlg 对话框



CJIMIDlg::CJIMIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_JIMI_DIALOG, pParent)
	, m_disDownLimit(_T("20"))
	, m_disUpLimit(_T("600"))
	, m_DownLimit(_T("50"))
	, m_UpLimit(_T("500"))
	, LockFlag(FALSE)
	, m_PCIP(_T(""))
	, m_oldWeightValue(_T(""))
	, m_newWeightValue(_T(""))
	, m_WeightValue(_T(""))
	, m_imei3Edit(_T(""))
	, m_imei2Edit(_T(""))
	, m_imei1Edit(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_JIMI);
}

void CJIMIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Run, m_LIST_Run);
	DDX_Control(pDX, IDC_LIST_Warning, m_LIST_Warning);
	DDX_Control(pDX, IDC_COMBO_COM, c_PortCom);
	DDX_Control(pDX, IDC_ZHIDAN_COMBO, m_zhidanCombo);

	DDX_Control(pDX, IDC_IMEISYLLABLE_CHECK, m_imeiSyllableCheck);
	DDX_Control(pDX, IDC_SNSYLLABLE_CHECK, m_snSyllableCheck);
	DDX_Control(pDX, IDC_SIMSYLLABLE_CHECK, m_simSyllableCheck);
	DDX_Control(pDX, IDC_VIPSYLLABLE_CHECK, m_vipSyllableCheck);
	DDX_Control(pDX, IDC_ICCIDSYLLABLE_CHECK, m_iccidSyllableCheck);
	DDX_Control(pDX, IDC_BATSYLLABLE_CHECK, m_batSyllableCheck);
	DDX_Control(pDX, IDC_MACSYLLABLE_CHECK, m_macSyllableCheck);
	DDX_Control(pDX, IDC_EQUIPMENTSYLLABLE_CHECK, m_equipmentSyllableCheck);
	DDX_Control(pDX, IDC_RFIDSYLLABLE_CHECK, m_rfidSyllableCheck);
	DDX_Control(pDX, IDC_IMEI2SYLLABLE_CHECK, m_imei2SyllableCheck);

	//DDX_Control(pDX, IDC_SIMBIND_CHECK, m_simBindCheck);
	//DDX_Control(pDX, IDC_VIPBIND_CHECK, m_vipBindCheck);
	//DDX_Control(pDX, IDC_ICCIDBIND_CHECK, m_iccidBindCheck);
	//DDX_Control(pDX, IDC_BATBIND_CHECK, m_batBindCheck);
	//DDX_Control(pDX, IDC_MACBIND_CHECK, m_macBindCheck);
	//DDX_Control(pDX, IDC_EQUIPMENTBIND_CHECK, m_equipmentBindCheck);
	//DDX_Control(pDX, IDC_RFIDBIND_CHECK, m_rfidBindCheck);

	DDX_Control(pDX, IDC_OPENIMEI3EDIT_CHECK, m_OpenImei3EditCheck);
	//DDX_Control(pDX, IDC_BTN_StateView, m_StateView);
	//DDX_Control(pDX, IDC_BTN_OK_View, m_OKView);
	//DDX_Control(pDX, IDC_BTN_Over_View, m_OverView);
	//DDX_Control(pDX, IDC_BTN_NULL_View, m_NullView);
	//DDX_Control(pDX, IDC_BTN_Down_View, m_DownView);

	DDX_Text(pDX, IDC_EDIT_disDownLmt, m_disDownLimit);
	DDX_Text(pDX, IDC_EDIT_disUpLmt, m_disUpLimit);
	DDX_Text(pDX, IDC_EDIT_DownLimit, m_DownLimit);
	DDX_Text(pDX, IDC_EDIT_UpLimit, m_UpLimit);
	DDX_Text(pDX, IDC_PCIP_EDIT, m_PCIP);
	DDX_Text(pDX, IDC_IMEI3_EDIT, m_imei3Edit);
	DDX_Text(pDX, IDC_IMEI2_EDIT, m_imei2Edit);
	DDX_Text(pDX, IDC_IMEI1_EDIT, m_imei1Edit);

	
}

BEGIN_MESSAGE_MAP(CJIMIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_DBConfig, &CJIMIDlg::OnBnClickedBtnDbconfig)
	ON_BN_CLICKED(IDOK, &CJIMIDlg::OnBnClickedOk)

	ON_MESSAGE(WM_RunLog, &CJIMIDlg::OnRunlog)
	ON_MESSAGE(WM_Voice, &CJIMIDlg::OnVoice)
	ON_MESSAGE(WM_MySetFocus, &CJIMIDlg::OnMySetFocus)
	ON_BN_CLICKED(IDC_BTN_ClearError, &CJIMIDlg::OnBnClickedBtnClearerror)
	ON_MESSAGE(WM_ReadWeighPortRet, &CJIMIDlg::OnReadweighportret)
	ON_CBN_DROPDOWN(IDC_COMBO_COM, &CJIMIDlg::OnDropdownComboCom)
	ON_CBN_SELCHANGE(IDC_ZHIDAN_COMBO, &CJIMIDlg::OnSelchangeZhidanCombo)
//	ON_STN_CLICKED(IDC_HINT_STATIC, &CJIMIDlg::OnStnClickedHintStatic)
    ON_BN_CLICKED(IDC_BUTTON_UpdataOrder, &CJIMIDlg::OnBnClickedButtonUpdataorder)
    ON_BN_CLICKED(IDC_BUTTON_COMOK2, &CJIMIDlg::OnBnClickedButtonComok2)
	ON_CBN_KILLFOCUS(IDC_ZHIDAN_COMBO, &CJIMIDlg::OnKillfocusZhidanCombo)
	ON_CBN_SELENDOK(IDC_ZHIDAN_COMBO, &CJIMIDlg::OnSelendokZhidanCombo)
	ON_BN_CLICKED(IDC_SIMSYLLABLE_CHECK, &CJIMIDlg::OnBnClickedSimsyllableCheck)
	ON_BN_CLICKED(IDC_IMEISYLLABLE_CHECK, &CJIMIDlg::OnBnClickedImeisyllableCheck)
	ON_BN_CLICKED(IDC_SNSYLLABLE_CHECK, &CJIMIDlg::OnBnClickedSnsyllableCheck)
	ON_BN_CLICKED(IDC_VIPSYLLABLE_CHECK, &CJIMIDlg::OnBnClickedVipsyllableCheck)
	ON_BN_CLICKED(IDC_ICCIDSYLLABLE_CHECK, &CJIMIDlg::OnBnClickedIccidsyllableCheck)
	ON_BN_CLICKED(IDC_BATSYLLABLE_CHECK, &CJIMIDlg::OnBnClickedBatsyllableCheck)
	ON_BN_CLICKED(IDC_MACSYLLABLE_CHECK, &CJIMIDlg::OnBnClickedMacsyllableCheck)
	ON_BN_CLICKED(IDC_EQUIPMENTSYLLABLE_CHECK, &CJIMIDlg::OnBnClickedEquipmentsyllableCheck)
	ON_BN_CLICKED(IDC_RFIDSYLLABLE_CHECK, &CJIMIDlg::OnBnClickedRfidsyllableCheck)
	ON_BN_CLICKED(IDC_SAVESYLLABLE_BUTTON, &CJIMIDlg::OnBnClickedSavesyllableButton)
	ON_BN_CLICKED(IDC_READSYLLABLE_BUTTON, &CJIMIDlg::OnBnClickedReadsyllableButton)

	//ON_BN_CLICKED(IDC_SIMBIND_CHECK, &CJIMIDlg::OnBnClickedSimbindCheck)
	//ON_BN_CLICKED(IDC_VIPBIND_CHECK, &CJIMIDlg::OnBnClickedVipbindCheck)
	//ON_BN_CLICKED(IDC_BATBIND_CHECK, &CJIMIDlg::OnBnClickedBatbindCheck)
	//ON_BN_CLICKED(IDC_ICCIDBIND_CHECK, &CJIMIDlg::OnBnClickedIccidbindCheck)
	//ON_BN_CLICKED(IDC_MACBIND_CHECK, &CJIMIDlg::OnBnClickedMacbindCheck)
	//ON_BN_CLICKED(IDC_EQUIPMENTBIND_CHECK, &CJIMIDlg::OnBnClickedEquipmentbindCheck)
	//ON_BN_CLICKED(IDC_RFIDBIND_CHECK, &CJIMIDlg::OnBnClickedRfidbindCheck)

	ON_BN_CLICKED(IDC_OPENIMEI3EDIT_CHECK, &CJIMIDlg::OnBnClickedOpenimei3editCheck)
	ON_BN_CLICKED(IDC_LOCK_BUTTON, &CJIMIDlg::OnBnClickedLockButton)
	ON_BN_CLICKED(IDC_IMEI2SYLLABLE_CHECK, &CJIMIDlg::OnBnClickedImei2syllableCheck)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CJIMIDlg::OnBnClickedButtonLogin)
END_MESSAGE_MAP()



// CJIMIDlg 消息处理程序


BOOL CJIMIDlg::OnInitDialog()
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

	UserLogin userDlg;
	userDlg.DoModal();
	if (userDlg.sUSER == _T(""))
	{
		CDialog::OnCancel();
		return FALSE;
	}

	//m_StateView.Blank();
	//m_OKView.Green();
	//m_OverView.Red();
	//m_DownView.Yellow();
	//m_NullView.Blank();

	ShowWindow(SW_MAXIMIZE);//对话框默认最大化弹出
	pComThread = NULL;
	//初始化串口列表
	GetLocalHostIPName(m_PCName,m_PCIP);
	SetDlgItemText(IDC_PCIP_EDIT, m_PCIP);
	GetDlgItem(IDC_ZHIDAN_COMBO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UpdataOrder)->EnableWindow(FALSE);
	GetDlgItem(IDC_IMEI3_EDIT)->EnableWindow(FALSE);

	FindCommPort((CComboBox*)GetDlgItem(IDC_COMBO_COM), sComPort, 1);
	FontInit();
	AddToRunList(_T("登录人员：")+ userDlg.sUSER);
	m_UserName = userDlg.sUSER;
	SetDlgItemText(IDC_USERNAME_EDIT, m_UserName);
	m_UserRight = userDlg.sUSERRight;
	SetDlgItemText(IDC_UserRight_EDIT, m_UserRight);

	IniVoice();
	AddToRunList(_T("初始化完成！！"));
	bThreadEnd = true;
	OnBnClickedButtonUpdataorder();
	if ((m_UserRight.Find(_T("&0000")) == -1) && (m_UserRight.Find(_T("&1001")) == -1))
	{
		WidgetStatue(FALSE);
	}
	else
	{
		//WidgetStatue(TRUE);
	}	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CJIMIDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CJIMIDlg::OnPaint()
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
HCURSOR CJIMIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//初始化串口
HANDLE CJIMIDlg::InitCom(CString comName)
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
		MessageBox(L"串口初始化失败！", L"提示信息", NULL);
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
	dcb.BaudRate = 9600; //波特率为9600
	dcb.ByteSize = 8; //每个字节有8位
	dcb.Parity = NOPARITY; //无奇偶校验位
	dcb.StopBits = ONESTOPBIT; //1个停止位
	SetCommState(hCom, &dcb);
	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
	
	//SetRicheditText(L"串口初始化成功！", 0);
	return hCom;
}

//关闭串口
int CJIMIDlg::CloseCom(HANDLE handler)
{
	return CloseHandle(handler);
}

//获取当前串口号
void CJIMIDlg::GetCommPort(CComboBox *pComboBox, CString &ComNo)
{
	int nSel;
	nSel = pComboBox->GetCurSel();
	pComboBox->GetLBText(nSel, ComNo);

	if (ComNo == "")
	{
		MessageBox(L"请先选择串口号！", L"提示信息", NULL);
	}
}

/*串口通用操作*/
//从注册表中寻找已经注册串口号然后展示出来
void CJIMIDlg::FindCommPort(CComboBox *pComboBox, CString &ComNo, int PortNO)
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

//获取本机IP和地址
int CJIMIDlg::GetLocalHostIPName(CString &sLocalName, CString &sIpAddress)
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

UINT CJIMIDlg::ComRxThread(LPVOID pParam)
{
	CJIMIDlg *pThisThreadDlg = (CJIMIDlg *)pParam;
	int res;
	DWORD dwError = 0;
	DWORD dwEvtMask = 0;
	DWORD dwReadSize = 0;
	char *RxBuffer, buf[1024];;
	BOOL bRet;
	CString str, temp;
	str = _T("");
	temp = _T("");
	////设置接收时间掩码
	//if (SetCommMask(pThisThreadDlg->port1handler, EV_RXCHAR) == 0) {
	//	pThisThreadDlg->SetDlgItemText(IDC_WeightView, _T("电子秤未连接"));
	//	AfxMessageBox(_T("设置接收时间掩码出错"));
	//	return 1;
	//}
	while (TRUE)
	{
		memset(buf,0, sizeof(buf));
		RxBuffer = buf;
		bRet = ReadFile(pThisThreadDlg->port1handler, RxBuffer, 512, &dwReadSize, NULL);
		if (bRet)
		{
			if (dwReadSize != 0)
			{
				str = RxBuffer;
				str.Trim();//清除空格等
				str.Replace(_T("\r"), _T(""));//清除换行符号等
				str.Replace(_T("\n"), _T(""));//清除换行符号等
				temp.Format(_T("收到串口消息:  %s"), str);
				pThisThreadDlg->DealtSerialData(str);

				str = _T("");
			}
			else
			{
				pThisThreadDlg->m_newWeightValue = _T("");
				pThisThreadDlg->SetDlgItemText(IDC_WeightView, _T("电子秤未连接"));
			}
		}
		else
		{
			pThisThreadDlg->m_newWeightValue = _T("");
			pThisThreadDlg->SetDlgItemText(IDC_WeightView, _T("电子秤未连接"));
			OutputDebugString(_T("ReadFile 读文件出错/r/n"));
		}
		PurgeComm(pThisThreadDlg->port1handler, PURGE_RXABORT | PURGE_RXCLEAR);
		Sleep(140);
		str = _T("");
	}
	return 0;
}

void CJIMIDlg::GetWeightDEAL()
{
	m_OKValue = _T("");
	m_WeightValue = _T("");
	GetDlgItem(IDC_IMEI3_EDIT)->EnableWindow(FALSE);
	if (!GetWeightValue())
	{
		AddToErrorList(_T("获取重量值超时，请检查问题后,重新扫描此产品!"));
		RecordResult(_T("error501、获取重量值超时，请检查问题后,重新扫描此产品!"));
		SetDlgItemText(IDC_HINT_STATIC, L"获取重量超时");
//		SetEditEmpty();//清空编辑框    	
	}
	else
	{
		//if (iValResult == 0)
		{
            RecordResult(_T("结果：重量值为:") + m_WeightValue + _T(";详情为：") + sResult);
			ToResultSheet();//记录数据至总表,目前总表的IMEI号为空
		}
		AfxBeginThread(VoiceThread, this);//开启与语音读数据的线程																	//重量不正常则不需要上传数据，需求说明有提到
		AddToRunList(_T("IMEI:") + m_IMEIValue + _T("重量值为:") + m_WeightValue + _T(";详情为：") + sResult);
	//	SetDlgItemText(IDC_HINT_STATIC, L"就绪");
		SetDlgItemText(IDC_IMEI2_EDIT, m_WeightValue);
	//	SetEditEmpty();//清空编辑框   
	}
	return;
}
BOOL CJIMIDlg::Process()
{
	return TRUE;
}

UINT CJIMIDlg::GetWeightThread(LPVOID pParam)
{
	CJIMIDlg *pThisThreadDlg = (CJIMIDlg *)pParam;
	//pThisThreadDlg->bThreadEnd = false;
	pThisThreadDlg->m_OKValue = _T("");
    pThisThreadDlg->m_WeightValue = _T("");
	//pThisThreadDlg->GetDlgItem(IDC_IMEI3_EDIT)->EnableWindow(FALSE);
	if (!pThisThreadDlg->GetWeightValue())
	{
		pThisThreadDlg->AddToErrorList(_T("获取重量值超时，请检查问题后,重新扫描此产品!"));
		pThisThreadDlg->RecordResult(_T("error501、获取重量值超时，请检查问题后,重新扫描此产品!"));
		pThisThreadDlg->SetDlgItemText(IDC_HINT_STATIC, L"获取重量超时");
		//pThisThreadDlg->GetDlgItem(IDC_IMEI3_EDIT)->EnableWindow(TRUE);
		//::PostMessage(pThisThreadDlg->m_hWnd, WM_MySetFocus, 0, 0);
		::SendMessage(pThisThreadDlg->m_hWnd, WM_MySetFocus, 0, 0);
		//pThisThreadDlg->SetEditEmpty();//清空编辑框    	
	}
	else
	{
		//if (pThisThreadDlg->iValResult == 0)
		{
			pThisThreadDlg->RecordResult(_T("结果：重量值为:") + pThisThreadDlg->m_WeightValue + _T(";详情为：") + pThisThreadDlg->sResult);
			Sleep(10);
			pThisThreadDlg->ToResultSheet();//记录数据至总表,目前总表的IMEI号为空
		}
		//AfxBeginThread(pThisThreadDlg->VoiceThread, pThisThreadDlg);//开启与语音读数据的线程
		//重量不正常则不需要上传数据，需求说明有提到
		pThisThreadDlg->AddToRunList(_T("IMEI:") + pThisThreadDlg->m_IMEIValue + _T("重量值为:") + pThisThreadDlg->m_WeightValue + _T(";详情为：") + pThisThreadDlg->sResult);		
	//	pThisThreadDlg->SetDlgItemText(IDC_HINT_STATIC, L"就绪");
		pThisThreadDlg->SetDlgItemText(IDC_IMEI2_EDIT, pThisThreadDlg->m_WeightValue);	
		//pThisThreadDlg->SetEditEmpty();//清空编辑框   
	
		//pThisThreadDlg->GetDlgItem(IDC_IMEI3_EDIT)->EnableWindow(TRUE);
		//::PostMessage(pThisThreadDlg->m_hWnd, WM_MySetFocus, 0, 0);
		::SendMessage(pThisThreadDlg->m_hWnd, WM_MySetFocus, 0, 0);
		//pThisThreadDlg->SetDlgItemText(IDC_IMEI3_EDIT, L"");
		//HWND hd = pThisThreadDlg->GetDlgItem(IDC_IMEI3_EDIT)->GetSafeHwnd();
		//pThisThreadDlg->SetForegroundWindow();		
	}	
	//pThisThreadDlg->bThreadEnd = true;
	return TRUE;
}

UINT CJIMIDlg::VoiceThread(LPVOID pParam)
{
	CJIMIDlg *pThisThreadDlg = (CJIMIDlg *)pParam;
	CString strText;
	switch (pThisThreadDlg->iValResult)
	{
	case 0:
		strText = _T("正常");
		pThisThreadDlg->PlayVoice(strText);
		break;
	case 1:
		strText = _T("偏轻");
		pThisThreadDlg->PlayVoice(strText);
		break;
	case 2:
		strText = _T("偏重");
		pThisThreadDlg->PlayVoice(strText);
		break;
	default:
		break;
	}	
	return TRUE;
}
void CJIMIDlg::OnBnClickedBtnDbconfig()
{
	// TODO: 在此添加控件通知处理程序代码
	DBConfig dlg;
	dlg.DoModal();
}

bool CJIMIDlg::IniVoice()
{
	pVoice = NULL; //初始化COM 

	if (FAILED(CoInitialize(NULL)))
	{
		AfxMessageBox(_T("Error to intiliaze COM"));
		return false;
	}
	//初始化SAPI HRESULT
	hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
	ISpObjectToken * pSpObjectToken = NULL;
	if (SUCCEEDED(SpFindBestToken(SPCAT_VOICES, L"language=804", NULL, &pSpObjectToken)))//804代表中文 
	{
		pVoice->SetVoice(pSpObjectToken);//声音大小
		pVoice->SetRate(3);//语速
		pSpObjectToken->Release();
	}
	else {
		AfxMessageBox(_T("没有安装微软语音库!"));
		return false;
	}
	//朗读编辑框中的文字 
	//if (SUCCEEDED(hr))
	//{
	//	CString strText = _T("语音测试");//引号是朗读的话 可以是中文  
	//	PlayVoice(strText);
	//  hr = pVoice->Speak(strText.AllocSysString(), 0, NULL);
	//	pVoice->Release();
	//	pVoice = NULL;
	//}
	return true;
}

void CJIMIDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	
//	CDialogEx::OnOK();
}

void CJIMIDlg::AddToRunList(CString str)
{
	//添加运行日志
	/**/
	SYSTEMTIME time;
	GetLocalTime(&time);
	CString strTime;
	strTime.Format(_T("%02d:%02d:%02d  %s"), time.wHour, time.wMinute, time.wSecond, str);

	if (m_LIST_Run.GetCount() > 100)
	{
		for (int i = m_LIST_Run.GetCount(); i >= 50; i--)
		{
			m_LIST_Run.DeleteString(i - 1);
		}
	}
	m_LIST_Run.InsertString(0, strTime);

	char * buffer = new char[1024];
	memset(buffer, 0, 1024);

	va_list list;
	va_start(list, str);
	
	const char* cstr;
	char temp[100];
	::wsprintfA(temp, "%ls", (LPCTSTR)str);
	cstr = temp;
	vsprintf_s(buffer, 1024, cstr, list);
	va_end(list);

	::PostMessage(this->m_hWnd, WM_RunLog, (WPARAM)buffer, 0);

}
void CJIMIDlg::AddToErrorList(CString str)
{
	//添加运行日志
	/**/
	SYSTEMTIME time;
	GetLocalTime(&time);
	CString strTime;
	strTime.Format(_T("%02d:%02d:%02d  %s"), time.wHour, time.wMinute, time.wSecond, str);

	m_LIST_Warning.InsertString(0, strTime);
	if (m_LIST_Warning.GetCount())
	{
		GetDlgItem(IDC_BTN_ClearError)->EnableWindow(TRUE); 
	}
		
	char * buffer = new char[1024];
	memset(buffer, 0, 1024);

	va_list list;
	va_start(list, str);

	const char* cstr;
	char temp[100];
	::wsprintfA(temp, "%ls", (LPCTSTR)str);
	cstr = temp;
	vsprintf_s(buffer, 1024, cstr, list);
	va_end(list);

	::PostMessage(this->m_hWnd, WM_RunLog, (WPARAM)buffer, 1);
}
void CJIMIDlg::PlayVoice(CString str)
{
	char * buffer = new char[1024];
	memset(buffer, 0, 1024);
	va_list list;
	va_start(list, str);

	const char* cstr;
	char temp[100];
	::wsprintfA(temp, "%ls", (LPCTSTR)str);
	cstr = temp;
	vsprintf_s(buffer, 1024, cstr, list);
	va_end(list);

	::PostMessage(this->m_hWnd, WM_Voice, (WPARAM)buffer, NULL);
}
afx_msg LRESULT CJIMIDlg::OnVoice(WPARAM wParam, LPARAM lParam)
{
	char * pBuf = (char *)wParam;
	CString sVoice = CString(pBuf);
	hr = pVoice->Speak(sVoice.AllocSysString(), 0, NULL);
	return 0;
}
afx_msg LRESULT CJIMIDlg::OnMySetFocus(WPARAM wParam, LPARAM lParam)
{
//	SetDlgItemText(IDC_IMEI3_EDIT, L"");
//	GetDlgItem(IDC_IMEI3_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();	
	bThreadEnd = true;
	return 0;
}
afx_msg LRESULT CJIMIDlg::OnRunlog(WPARAM wParam, LPARAM lParam)
{
	//写日志
	char * pBuf = (char *)wParam;
	SYSTEMTIME time;
	GetLocalTime(&time);	
	try
	{
		CFile file;
		FILE* m_pStream = NULL;
		//CLineRead file;
		CStdioFile m_SFile;
		CFileFind m_FileFind;
		CString strLogMessage = CString(pBuf);
		CString m_sFileName;
		if (lParam == 0)
		{
			m_sFileName.Format(_T("%04d-%02d-%02d  Run.log"), time.wYear, time.wMonth, time.wDay);
		}
		else if (lParam == 1)
		{
			m_sFileName.Format(_T("%04d-%02d-%02d  Error.log"), time.wYear, time.wMonth, time.wDay );
		}

		CString m_sFilePath = _T(".\\LOG");
		//SYSTEMTIME time;
		//GetLocalTime(&time);
		CString strCurrentTime;
		strCurrentTime.Format(_T("%04d-%02d-%02d  %02d:%02d:%02d:%03d  "), time.wYear, time.wMonth, time.wDay,
			time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

		strLogMessage = strCurrentTime + strLogMessage + _T("\r\n");// + "\r\n";\r

		if (!m_FileFind.FindFile(m_sFilePath))
		{
			CreateDirectory(m_sFilePath, NULL);
		}

		if (!m_SFile.Open(m_sFilePath + _T("\\") + m_sFileName, CFile::modeReadWrite | CFile::modeNoTruncate))
		{
			if (!m_SFile.Open(m_sFilePath + _T("\\") + m_sFileName,CFile::modeCreate | CFile::modeReadWrite | CFile::typeText | CFile::modeNoTruncate))
			{
				//AfxMessageBox("打开日志文件 %s 失败", m_sFilePath + "\\" + m_sFileName);
				return FALSE;
			}
		}
		m_SFile.SeekToEnd();

#ifdef UNICODE 
		CString strTmp = strLogMessage;
		int len = 0;
		char *buf = NULL;
		len = WideCharToMultiByte(CP_ACP, 0, strTmp, strTmp.GetLength(), NULL, 0, NULL, NULL) + 1;
		buf = new char[len];
		memset(buf, 0, len);
		WideCharToMultiByte(CP_ACP, 0, strTmp, strTmp.GetLength(), buf, len - 1, NULL, NULL);
#else
		CString strTmp = strLog;
		char *buf = NULL;
		int len = 0;
		len = strTmp.GetLength() + 1;//多分配一个存放结束符号
		buf = new char[len];
		memset(buf, 0, len);
		buf = strTmp.GetBuffer();
#endif

		m_SFile.Write(buf, strlen(buf));
		m_SFile.Close();
	}
	catch (CFileException *fileException)
	{
		if (pBuf) {
			delete[] pBuf;
		}
		return false;
	}
	if (pBuf) {
		delete[] pBuf;
	}
	return 0;
}


void CJIMIDlg::OnBnClickedBtnClearerror()
{
	// TODO: 在此添加控件通知处理程序代码
	m_LIST_Warning.ResetContent();
}


afx_msg LRESULT CJIMIDlg::OnReadweighportret(WPARAM wParam, LPARAM lParam)
{
	int res;
	DWORD dwError = 0;
	DWORD dwEvtMask = 0;
	DWORD dwReadSize = 0;
	unsigned char RxBuffer[1024];
	BOOL bRet;
	CString str, temp;
	str = _T("");
	temp = _T("");
	if (SetCommMask(port1handler, EV_RXCHAR) == 0) {
		AfxMessageBox(_T("设置接收时间掩码出错"));
		return 1;
	}
	if (port1handler == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	PurgeComm(port1handler, PURGE_RXABORT | PURGE_RXCLEAR);//清除缓冲区
	if (WaitCommEvent(port1handler, &dwEvtMask, &m_osRead))
	{
		if (dwEvtMask & EV_RXCHAR)
		{ //接收事件
				//TODO：进行读取及其他应用操作 (ReadFile)
				memset(RxBuffer, 0, 1024);
				bRet = ReadFile(port1handler, RxBuffer, sizeof(RxBuffer), &dwReadSize, NULL);
				if (bRet)
				{

					OutputDebugString(_T("ReadFile 读文件成功/r/n"));
					for (int i = 0; i < sizeof(RxBuffer) / sizeof(RxBuffer[0]); i++)
					{
						temp.Format(_T("%c"), RxBuffer[i]);
						str = str + temp;
					}
					temp.Format(_T("收到串口消息:  %s"), str);
					AddToRunList(temp);
					str = _T("");
				}
				else
				{
					OutputDebugString(_T("ReadFile 读文件出错/r/n"));
				}
			PurgeComm(port1handler, PURGE_RXABORT | PURGE_RXCLEAR);
		}
	}
	return 0;
}


void CJIMIDlg::OnDropdownComboCom()
{
	// TODO: 在此添加控件通知处理程序代码
	FindCommPort((CComboBox*)GetDlgItem(IDC_COMBO_COM), sComPort, 1);
}



void CJIMIDlg::OnSelchangeZhidanCombo()
{
	if ((m_UserRight.Find(_T("&0000")) == -1) && (m_UserRight.Find(_T("&1001")) == -1))
	{
		AfxMessageBox(_T("您没有权限，如需更改，请切换账户"));
		//SetDlgItemText(IDC_ZHIDAN_COMBO,L"");
		m_zhidanCombo.SetCurSel(-1);
		return;
	}
	if (!readimei())
	{
		return;
	}
	SetEditEmpty();//清空编辑框
//	AddToRunList(_T("选择订单号:") + strzhidan);
	SetDlgItemText(IDC_HINT_STATIC, L"等待就绪");

	RelationEnableWindow(TRUE);

	//m_StateView.Blank();
	ChImei3EnableWindow(TRUE);
	GetDlgItem(IDC_LOCK_BUTTON)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON_COMOK2)->EnableWindow(FALSE);
	CleanSyllableCheck();
	CleanBindCheck();
	CleanImei3Check();
	Readsyllable(FALSE);
	//Readconfig();

	GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
	GetDlgItem(IDC_IMEI3_EDIT)->EnableWindow(FALSE);
	AddToRunList(_T("选择订单号为") + strzhidan);
	//OnBnClickedImeisyllableCheck();
	m_imeiSyllableCheck.SetCheck(1);
	UpdateData(FALSE);
}

//根据订单号获取IMEI起始号和结束号
bool CJIMIDlg::readimei()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strWea;
	int nSel;
	nSel = m_zhidanCombo.GetCurSel();
	m_zhidanCombo.GetLBText(nSel, strWea);

	SetDlgItemText(IDC_ZHIDAN_COMBO, strWea);//选中的那一瞬间是没有及时更新到编辑框的，所以得先获取临时数据后更新到编辑框中
	_RecordsetPtr GetIMEI;
	GetDlgItemText(IDC_ZHIDAN_COMBO, strzhidan);
	
	ADOManage adomanage;
	if (!adomanage.ConnSQL())
	{
		return false;
	}
	GetIMEI = adomanage.GetIMEIByOrderNumber(strzhidan);
	if (GetIMEI->adoEOF)
	{
		SetDlgItemText(IDC_IMEISTART_EDIT, L"");
		SetDlgItemText(IDC_IMEIOVER_EDIT, L"");
		adomanage.CloseAll();
		return false;
	}

	//让IMEI等全局号段获得值，以后记得添加进其它的全局号段，然后其它号段的值的设定在绑定那边用checkbox触发
	strimeistart = GetIMEI->GetCollect("IMEIStart");
	strimeiend = GetIMEI->GetCollect("IMEIEnd");
	SetDlgItemText(IDC_IMEISTART_EDIT, strimeistart);
	SetDlgItemText(IDC_IMEIOVER_EDIT, strimeiend);
	adomanage.CloseAll();
	return true;
}



void CJIMIDlg::OnBnClickedButtonUpdataorder()
{

	InitComboBox();//读取数据库文件
	SetDlgItemText(IDC_IMEISTART_EDIT, L"");
	SetDlgItemText(IDC_IMEIOVER_EDIT, L"");
	//SetEditEmpty();//清空编辑框
	SetDlgItemText(IDC_HINT_STATIC, L"等待就绪");
	RelationEnableWindow(FALSE);
	ChImei3EnableWindow(FALSE);
	ImeiInputEnableWindow(FALSE);
	GetDlgItem(IDC_LOCK_BUTTON)->EnableWindow(FALSE);
	CleanSyllableCheck();
	CleanBindCheck();
	CleanImei3Check();
	AddToRunList(_T("读取订单号"));
}

//给combox添加数据库中的订单号，更新按钮和开启后自动初始化都用这个函数
void CJIMIDlg::InitComboBox()
{
	m_zhidanCombo.ResetContent();
	ADOManage adomanageon;
	if (!adomanageon.ConnSQL())
	{
		return;
	}
	adomanageon.m_pRecordSet = adomanageon.GetOrderNumber();
	while (!adomanageon.m_pRecordSet->adoEOF)
	{
		m_zhidanCombo.AddString(adomanageon.m_pRecordSet->GetCollect("ZhiDan").bstrVal);
		adomanageon.m_pRecordSet->MoveNext();
	}
	adomanageon.CloseAll();
}
//将编辑框置为空
void CJIMIDlg::SetEditEmpty()
{
//	SetDlgItemText(IDC_IMEI1_EDIT, L"");
//	SetDlgItemText(IDC_IMEI2_EDIT, L"");
	//m_StateView.Blank();
	//SetDlgItemText(IDC_IMEI3_EDIT, L"");
	//GetDlgItem(IDC_IMEI3_EDIT)->EnableWindow(TRUE);
	//GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
	bThreadEnd = true;
	strno1 = L"NULL";
	strno2 = L"NULL";
	strno3 = L"NULL";
}
//使字段和绑定控件使能
void CJIMIDlg::RelationEnableWindow(BOOL chose)
{
	//GetDlgItem(IDC_SIMBIND_CHECK)->EnableWindow(chose);
	//GetDlgItem(IDC_VIPBIND_CHECK)->EnableWindow(chose);
	//GetDlgItem(IDC_ICCIDBIND_CHECK)->EnableWindow(chose);
	//GetDlgItem(IDC_BATBIND_CHECK)->EnableWindow(chose);
	//GetDlgItem(IDC_MACBIND_CHECK)->EnableWindow(chose);
	//GetDlgItem(IDC_EQUIPMENTBIND_CHECK)->EnableWindow(chose);
	//GetDlgItem(IDC_RFIDBIND_CHECK)->EnableWindow(chose);

	GetDlgItem(IDC_IMEISYLLABLE_CHECK)->EnableWindow(chose);
	GetDlgItem(IDC_IMEI2SYLLABLE_CHECK)->EnableWindow(chose);

	GetDlgItem(IDC_SNSYLLABLE_CHECK)->EnableWindow(chose);
	GetDlgItem(IDC_SIMSYLLABLE_CHECK)->EnableWindow(chose);
	GetDlgItem(IDC_VIPSYLLABLE_CHECK)->EnableWindow(chose);
	GetDlgItem(IDC_ICCIDSYLLABLE_CHECK)->EnableWindow(chose);
	GetDlgItem(IDC_BATSYLLABLE_CHECK)->EnableWindow(chose);
	GetDlgItem(IDC_MACSYLLABLE_CHECK)->EnableWindow(chose);
	GetDlgItem(IDC_EQUIPMENTSYLLABLE_CHECK)->EnableWindow(chose);
	GetDlgItem(IDC_RFIDSYLLABLE_CHECK)->EnableWindow(chose);
	GetDlgItem(IDC_SAVESYLLABLE_BUTTON)->EnableWindow(chose);
	GetDlgItem(IDC_READSYLLABLE_BUTTON)->EnableWindow(chose);

	GetDlgItem(IDC_EDIT_UpLimit)->EnableWindow(chose);
	GetDlgItem(IDC_EDIT_DownLimit)->EnableWindow(chose);
	GetDlgItem(IDC_EDIT_disDownLmt)->EnableWindow(chose);
	GetDlgItem(IDC_EDIT_disUpLmt)->EnableWindow(chose);


}

//使不判断开启IMEI编辑框控件使能
void CJIMIDlg::ChImei3EnableWindow(BOOL chose)
{
	GetDlgItem(IDC_OPENIMEI3EDIT_CHECK)->EnableWindow(chose);
}

//使IMEI输入框控件使能
void CJIMIDlg::ImeiInputEnableWindow(BOOL chose)
{
//	GetDlgItem(IDC_IMEI1_EDIT)->EnableWindow(chose);
//	GetDlgItem(IDC_IMEI2_EDIT)->EnableWindow(chose);

	if (m_OpenImei3EditCheck.GetCheck())
	{
		Imei3Flag = TRUE;
	}
	else
	{
		Imei3Flag = FALSE;
	}
	//当IMEI复选框被选中的时候IMEI扫描框才会被开启
	//if (Imei3Flag == TRUE)
	//{
	//	GetDlgItem(IDC_IMEI3_EDIT)->EnableWindow(FALSE);
	//}
	//else
	//{
	//	GetDlgItem(IDC_IMEI3_EDIT)->EnableWindow(TRUE);
	//}
}
//清除字段选择复选框
void CJIMIDlg::CleanSyllableCheck()
{
	m_snSyllableCheck.SetCheck(0);
	OnBnClickedSnsyllableCheck();
	m_simSyllableCheck.SetCheck(0);
	OnBnClickedSimsyllableCheck();
	m_vipSyllableCheck.SetCheck(0);
	OnBnClickedVipsyllableCheck();
	m_iccidSyllableCheck.SetCheck(0);
	OnBnClickedIccidsyllableCheck();
	m_batSyllableCheck.SetCheck(0);
	OnBnClickedBatsyllableCheck();
	m_macSyllableCheck.SetCheck(0);
	OnBnClickedMacsyllableCheck();
	m_equipmentSyllableCheck.SetCheck(0);
	OnBnClickedEquipmentsyllableCheck();
	m_rfidSyllableCheck.SetCheck(0);
	OnBnClickedRfidsyllableCheck();
	m_imei2SyllableCheck.SetCheck(0);
	OnBnClickedImei2syllableCheck();
	m_UpLimit = _T("500");
	m_DownLimit = _T("50");
	m_disUpLimit = _T("600");
	m_disDownLimit = _T("20");	
	UpdateData(FALSE);
}

//清除绑定复选框
void CJIMIDlg::CleanBindCheck()
{

}

//清除IMEI复选框
void CJIMIDlg::CleanImei3Check()
{
	m_OpenImei3EditCheck.SetCheck(0);
	OnBnClickedOpenimei3editCheck();
}

void CJIMIDlg::OnBnClickedButtonComok2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItemText(IDC_BUTTON_COMOK2,str);
	if (_T("连接") == str)
	{
		int  nSel = c_PortCom.GetCurSel();
		if (nSel < 0)
		{
			AfxMessageBox(_T("未搜寻到电脑串口！！"));
			return;
		}
		c_PortCom.GetLBText(nSel, sComPort);
		port1handler = InitCom(sComPort);
		if (SetCommMask(port1handler, EV_RXCHAR) == 0) 
		{
		//	SetDlgItemText(IDC_WeightView, _T("电子秤未连接"));
			AfxMessageBox(_T("串口打开失败"));
			return ;
		}
		pComThread = AfxBeginThread(ComRxThread, this);//开启读数据的线程

		SetDlgItemText(IDC_BUTTON_COMOK2, _T("断开"));
		GetDlgItem(IDC_COMBO_COM)->EnableWindow(FALSE);
		GetDlgItem(IDC_ZHIDAN_COMBO)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_UpdataOrder)->EnableWindow(TRUE);
	}
	else if (_T("断开") == str)
	{
	//	port1handler = INVALID_HANDLE_VALUE;
		if (pComThread != NULL)
		{
			if (pComThread != NULL)
			{
				TerminateThread(pComThread, 0);
				pComThread->SuspendThread();
				pComThread->ExitInstance();
				pComThread = NULL;
			}
		}
		try
		{
			int ret = CloseCom(port1handler);
		}
		catch (const std::exception&)
		{
		}		
		//m_StateView.Blank();
		SetDlgItemText(IDC_BUTTON_COMOK2, _T("连接"));
		SetDlgItemText(IDC_WeightView, _T("电子秤未连接"));		
		GetDlgItem(IDC_COMBO_COM)->EnableWindow(TRUE);
		GetDlgItem(IDC_ZHIDAN_COMBO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UpdataOrder)->EnableWindow(FALSE);
	}
	
}

void CJIMIDlg::OnKillfocusZhidanCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	//CString str1;
	//GetDlgItemText(IDC_ZHIDAN_COMBO, str1);

	//if (JudgeZhidan(str1))
	//{
	//	if (str1 != "")
	//	{
	//		MessageBox(L"订单号不存在！", L"提示信息", NULL);
	//	}
	//	SetDlgItemText(IDC_ZHIDAN_COMBO, L"");
	//	SetDlgItemText(IDC_IMEISTART_EDIT, L"");
	//	SetDlgItemText(IDC_IMEIOVER_EDIT, L"");
	//	SetEditEmpty();//清空编辑框
	//	InitComboBox();
	//	RelationEnableWindow(FALSE);
	//	ChImei3EnableWindow(FALSE);
	//	GetDlgItem(IDC_LOCK_BUTTON)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_BUTTON_COMOK2)->EnableWindow(FALSE);
	//	CleanSyllableCheck();
	//	CleanBindCheck();
	//	CleanImei3Check();
	//	return;
	//}
	//GetDlgItem(IDC_LOCK_BUTTON)->EnableWindow(TRUE);
}


void CJIMIDlg::OnSelendokZhidanCombo()
{
	////这个是按回车后触发操作
	//if (!readimei())
	//{
	//	return;
	//}
	//SetEditEmpty();//清空编辑框
 //  //	AddToRunList(_T("选择订单号:") + strzhidan);
	//SetDlgItemText(IDC_HINT_STATIC, L"就绪");
	//RelationEnableWindow(TRUE);
	//m_StateView.Blank();
	//ChImei3EnableWindow(TRUE);
	//GetDlgItem(IDC_LOCK_BUTTON)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON_COMOK2)->EnableWindow(FALSE);
	//CleanSyllableCheck();
	//CleanBindCheck();
	//CleanImei3Check();
	//Readbind(FALSE);
	//Readsyllable(FALSE);
	//Readconfig();
	//GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
	//GetDlgItem(IDC_IMEI3_EDIT)->EnableWindow(FALSE);

}


void CJIMIDlg::OnBnClickedSimsyllableCheck()
{
	if (m_simSyllableCheck.GetCheck())
	{
		SyllableMap[2] = L"SIM";
	}
	else
	{
		SyllableMap.erase(2);
	}
}


void CJIMIDlg::OnBnClickedImeisyllableCheck()
{
	//如果被选中，则添加进MAP，如果不是则清除掉它
	if (m_imeiSyllableCheck.GetCheck() == 1)
	{
		SyllableMap[0] = L"IMEI";
	}
	else
	{
		SyllableMap.erase(0);
	}
}


void CJIMIDlg::OnBnClickedSnsyllableCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_snSyllableCheck.GetCheck() == 1)
	{
		SyllableMap[1] = L"SN";
	}
	else
	{
		SyllableMap.erase(1);
	}
}


void CJIMIDlg::OnBnClickedVipsyllableCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_vipSyllableCheck.GetCheck())
	{
		SyllableMap[3] = L"VIP";
	}
	else
	{
		SyllableMap.erase(3);
	}
}


void CJIMIDlg::OnBnClickedIccidsyllableCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_iccidSyllableCheck.GetCheck())
	{
		SyllableMap[4] = L"ICCID";
	}
	else
	{
		SyllableMap.erase(4);
	}
}


void CJIMIDlg::OnBnClickedBatsyllableCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_batSyllableCheck.GetCheck())
	{
		SyllableMap[5] = L"BAT";
	}
	else
	{
		SyllableMap.erase(5);
	}
}


void CJIMIDlg::OnBnClickedMacsyllableCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_macSyllableCheck.GetCheck())
	{
		SyllableMap[6] = L"MAC";
	}
	else
	{
		SyllableMap.erase(6);
	}
}


void CJIMIDlg::OnBnClickedEquipmentsyllableCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_equipmentSyllableCheck.GetCheck())
	{
		SyllableMap[7] = L"Equipment";
	}
	else
	{
		SyllableMap.erase(7);
	}
}


void CJIMIDlg::OnBnClickedRfidsyllableCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_rfidSyllableCheck.GetCheck())
	{
		SyllableMap[8] = L"RFID";
	}
	else
	{
		SyllableMap.erase(8);
	}
}


void CJIMIDlg::OnBnClickedSavesyllableButton()
{
	// TODO: 在此添加控件通知处理程序代码

	if ((m_UserRight.Find(_T("&0000")) == -1) && (m_UserRight.Find(_T("&1001")) == -1))
	{
		AfxMessageBox(_T("您没有权限，如需更改，请切换账户"));
		OnBnClickedReadsyllableButton();
		return;
	}
	if (false == CheckParam())
	{
		AddToRunList(_T("保存参数失败"));
		return;
	}

	Savesyllable();
	//Savebind();
	Saveconfig();//保存IMEI3DE 的配置信息
	AddToRunList(_T("保存参数成功"));
}


void CJIMIDlg::OnBnClickedReadsyllableButton()
{
	// TODO: 在此添加控件通知处理程序代码
	Readsyllable(TRUE);
	AddToRunList(_T("刷新参数成功"));
	//Readbind(TRUE);
}


//保存字段选择
void CJIMIDlg::Savesyllable()
{
	UpdateData(TRUE);
	BOOL IMEI, SN, SIM, VIP, ICCID, BAT, MAC, Equipment, RFID,IMEI2;
	IMEI = m_imeiSyllableCheck.GetCheck();
	SN = m_snSyllableCheck.GetCheck();
	SIM = m_simSyllableCheck.GetCheck();
	VIP = m_vipSyllableCheck.GetCheck();
	ICCID = m_iccidSyllableCheck.GetCheck();
	BAT = m_batSyllableCheck.GetCheck();
	MAC = m_macSyllableCheck.GetCheck();
	Equipment = m_equipmentSyllableCheck.GetCheck();
	RFID = m_rfidSyllableCheck.GetCheck();
	IMEI2 = m_imei2SyllableCheck.GetCheck();

	ADOManage adomanage;
	if (!adomanage.ConnSQL())
	{
		return;
	}
	adomanage.Savesyllable(strzhidan, IMEI, SN, SIM, VIP, ICCID, BAT, MAC, Equipment, RFID,IMEI2, m_UpLimit, m_DownLimit, m_disUpLimit, m_disDownLimit);
	adomanage.CloseAll();
}

//读取字段选择和上下限的值
void CJIMIDlg::Readsyllable(BOOL CheckEx)
{
	_variant_t imeitemp;//用来放getcollect变量的

	ADOManage adomanage;
	if (!adomanage.ConnSQL())
	{
		return;
	}
	adomanage.Readsyllable(strzhidan);
	//先判断有无记录，有记录，就直接判断要不要报错，没记录，就先判断该字段是否为NULL，再判断是否要报错
	if (adomanage.m_pRecordSet->adoEOF)
	{
		if (CheckEx)
		{
			MessageBox(L"无此订单号字段选择记录！", L"提示", NULL);
		}
		return;
	}
	else if (!adomanage.m_pRecordSet->adoEOF)
	{
		imeitemp = adomanage.m_pRecordSet->GetCollect("IMEISyllable");
		if (imeitemp.vt == VT_NULL)
		{
			if (CheckEx)
			{
				MessageBox(L"无此订单号字段选择记录！", L"提示", NULL);
				return;
			}
			else if (!CheckEx)
			{
				return;
			}
		}
	}
	//设置对应控件状态
	m_imeiSyllableCheck.SetCheck(TRUE);//无论如何，本身必须匹配
	OnBnClickedImeisyllableCheck();
	m_snSyllableCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("SNSyllable"));
	OnBnClickedSnsyllableCheck();
	m_simSyllableCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("SIMSyllable"));
	OnBnClickedSimsyllableCheck();
	m_vipSyllableCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("VIPSyllable"));
	OnBnClickedVipsyllableCheck();
	m_iccidSyllableCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("ICCIDSyllable"));
	OnBnClickedIccidsyllableCheck();
	m_batSyllableCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("BATSyllable"));
	OnBnClickedBatsyllableCheck();
	m_macSyllableCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("MACSyllable"));
	OnBnClickedMacsyllableCheck();
	m_equipmentSyllableCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("EquipmentSyllable"));
	OnBnClickedEquipmentsyllableCheck();
	m_rfidSyllableCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("RFIDSyllable"));
	OnBnClickedRfidsyllableCheck();
	m_imei2SyllableCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("IMEI2Syllable"));
	OnBnClickedImei2syllableCheck();
    m_OpenImei3EditCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("IMEI3Flag"));//彩盒工位判断
    OnBnClickedOpenimei3editCheck();

	m_UpLimit = adomanage.m_pRecordSet->GetCollect("UpperLimit");
	m_DownLimit = adomanage.m_pRecordSet->GetCollect("DownLimit");
	m_disUpLimit = adomanage.m_pRecordSet->GetCollect("DisUpperLimit");
	m_disDownLimit = adomanage.m_pRecordSet->GetCollect("DisDownLimit");

	UpdateData(FALSE);

	adomanage.CloseAll();
}

//保存绑定选择
void CJIMIDlg::Savebind()
{
	BOOL SIM, VIP, ICCID, BAT, MAC, Equipment, RFID;
	//SIM = m_simBindCheck.GetCheck();
	//VIP = m_vipBindCheck.GetCheck();
	//ICCID = m_iccidBindCheck.GetCheck();
	//BAT = m_batBindCheck.GetCheck();
	//MAC = m_macBindCheck.GetCheck();
	//Equipment = m_equipmentBindCheck.GetCheck();
	//RFID = m_rfidBindCheck.GetCheck();

	//ADOManage adomanage;
	//adomanage.ConnSQL();
	//adomanage.Savebind(strzhidan, SIM, VIP, ICCID, BAT, MAC, Equipment, RFID);
	//adomanage.CloseAll();
}

//读取绑定选择
void CJIMIDlg::Readbind(BOOL CheckEx)
{

}

//保存一些配置
void CJIMIDlg::Saveconfig()
{
	BOOL imei3flag;
	imei3flag = m_OpenImei3EditCheck.GetCheck();
	ADOManage adomanage;
	if (!adomanage.ConnSQL())
	{
		return;
	}
	adomanage.SaveConfig(strzhidan, imei3flag);
	adomanage.CloseAll();
}

//读取一些配置
void CJIMIDlg::Readconfig()
{
	_variant_t imeitemp;//用来放getcollect变量的

	ADOManage adomanage;
	if (!adomanage.ConnSQL())
	{
		return;
	}
	adomanage.Readsyllable(strzhidan);
	//先判断有无记录，有记录，就直接判断要不要报错，没记录，就先判断该字段是否为NULL，再判断是否要报错
	if (adomanage.m_pRecordSet->adoEOF)
	{
		return;
	}
	else if (!adomanage.m_pRecordSet->adoEOF)
	{
		imeitemp = adomanage.m_pRecordSet->GetCollect("IMEI3Flag");
		if (imeitemp.vt == VT_NULL)
		{
			return;
		}
	}

	//设置对应控件状态
	m_OpenImei3EditCheck.SetCheck(adomanage.m_pRecordSet->GetCollect("IMEI3Flag"));
	OnBnClickedOpenimei3editCheck();
	adomanage.CloseAll();
}

//判断订单号操作集成
BOOL CJIMIDlg::JudgeZhidan(CString zhidan)
{
	_RecordsetPtr recordzhidan;
	int flag = 1;
	ADOManage adomanage;
	if (!adomanage.ConnSQL())
	{
		return FALSE;
	}
	recordzhidan = adomanage.JudgeOrderNumber(zhidan);
	if (!recordzhidan->adoEOF)
	{
		flag = 0;
	}
	adomanage.CloseAll();
	return flag;
}

void CJIMIDlg::OnBnClickedOpenimei3editCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_OpenImei3EditCheck.GetCheck())
	{
		Imei3Flag = TRUE;
	}
	else
	{
		Imei3Flag = FALSE;
	}
}

//使订单配置、不判断彩盒、数据库配置控件使能
void CJIMIDlg::OtherEnableWindow(BOOL chose)
{
	GetDlgItem(IDC_ZHIDAN_COMBO)->EnableWindow(chose);
	//GetDlgItem(IDC_UPDATEORDERNUMBER_BUTTON)->EnableWindow(chose);
	//GetDlgItem(IDC_CHJUDGE_CHECK)->EnableWindow(chose);
	//GetDlgItem(IDC_DBCONFIG_BUTTON)->EnableWindow(chose);
	GetDlgItem(IDC_OPENIMEI3EDIT_CHECK)->EnableWindow(chose);
}

void CJIMIDlg::OnBnClickedLockButton()
{
	if ((m_UserRight.Find(_T("&0000")) == -1) && (m_UserRight.Find(_T("&1002")) == -1))
	{
		AfxMessageBox(_T("您没有权限，如需更改，请切换账户"));
		return;
	}
	if (LockFlag == TRUE)
	{
		INT_PTR nRes;
		//UserLogin userlogin = new UserLogin;
		//nRes = userlogin.DoModal();

		//if (IDCANCEL == nRes)
		//{
		//	return;
		//}

		//else if (IDOK == nRes)
		{
			//AddToRunList(_T("解锁登录人员：") + userlogin.sUSER);
			//m_UserName = userlogin.sUSER;
			OtherEnableWindow(TRUE);

			RelationEnableWindow(TRUE);
			ChImei3EnableWindow(TRUE);
			//ImeiInputEnableWindow(FALSE);
			SetDlgItemText(IDC_IMEI3_EDIT, L"");
			GetDlgItem(IDC_IMEI3_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_COMOK2)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_UpdataOrder)->EnableWindow(TRUE);
			SetDlgItemText(IDC_LOCK_BUTTON, L"开始");
		//	m_StateView.Blank();
			AddToRunList(m_UserName + _T(",停止称重!!!!!!!!"));
			GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(TRUE);
			LockFlag = FALSE;
			if ((m_UserRight.Find(_T("&0000")) == -1) && (m_UserRight.Find(_T("&1001")) == -1))
			{
				WidgetStatue(FALSE);
			}
			else
			{
				WidgetStatue(TRUE);
			}
		}		
		SetDlgItemText(IDC_HINT_STATIC, L"等待就绪");
	}
	else if (LockFlag == FALSE)
	{
		//先保存配置
	//	OnBnClickedSavesyllableButton();
		if (false == CheckParam())
		{
			AddToErrorList(_T("称重参数异常，无法开始称重"));
			return;
		}
		Saveconfig();

		OtherEnableWindow(FALSE);
		RelationEnableWindow(FALSE);
		ChImei3EnableWindow(FALSE);
		GetDlgItem(IDC_IMEI3_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_COMOK2)->EnableWindow(FALSE);
	//	ImeiInputEnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_UpdataOrder)->EnableWindow(FALSE);
		SetDlgItemText(IDC_LOCK_BUTTON, L"停止");
		SetDlgItemText(IDC_HINT_STATIC, L"就绪");
		AddToRunList(m_UserName + _T(",开始称重!!!!!!!!"));
		GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(FALSE);
		//将查询字段的后半段sql语句提前生成出来，提高效率
		//这里是查字段的
		map<int, CString>::iterator SyllableIter;
		//map<int, CString>::iterator BindIter;
		_variant_t Syllabletempvt;
		CString Syllabletemp;
		SyllablestrSql = L"";//先重置一下
		notype = SyllableMap.size();

		LockFlag = TRUE;;
		//根据MAP的长度来决定循环次数
		for (SyllableIter = CJIMIDlg::SyllableMap.begin(); SyllableIter != CJIMIDlg::SyllableMap.end(); SyllableIter++)
		{
			switch (SyllableIter->first)
			{
			case 0:
				SyllablestrSql += _T("IMEI= 'InputNumber' ");
				break;
			case 1:
				SyllablestrSql += _T("OR SN= 'InputNumber' ");
				break;
			case 2:
				SyllablestrSql += _T("OR SIM= 'InputNumber' ");
				break;
			case 3:
				SyllablestrSql += _T("OR VIP= 'InputNumber' ");
				break;
			case 4:
				SyllablestrSql += _T("OR ICCID= 'InputNumber' ");
				break;
			case 5:
				SyllablestrSql += _T("OR BAT= 'InputNumber' ");
				break;
			case 6:
				SyllablestrSql += _T("OR MAC= 'InputNumber' ");
				break;
			case 7:
				SyllablestrSql += _T("OR Equipment= 'InputNumber' ");
				break;
			case 8:
				SyllablestrSql += _T("OR RFID= 'InputNumber' ");
				break;
			case 9:
				SyllablestrSql += _T("OR IMEI2= 'InputNumber' ");
				break;
			default:
				break;
			}
		}
		GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
		//if (Imei3Flag == TRUE)
		//{
		//	GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
		//}
		//else if (Imei3Flag == FALSE)
		//{
		//	GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
		//}
	}
}


BOOL CJIMIDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	CString str1 = L"", str2 = L"", str3 = L"";//分别装载IMEI1扫描框，IMEI2扫描框，IMEI3扫描框的字符串
	int mm;
	int resultflag1, resultflag2, resultflag3;//分别装载IMEI1扫描框，IMEI2扫描框，IMEI3扫描框的一些查询结果，根据这些结果来判定成功与否
	

	//如果为空就不要继续往下面执行了
//	if (Imei3Flag == TRUE)
	{
		GetDlgItemText(IDC_IMEI3_EDIT, str3);
		if (str3 == "")			
		{
			return CDialogEx::PreTranslateMessage(pMsg);
		}
	}
	//不断的读键盘输入的东西
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		if (false == bThreadEnd)
		{
			SetDlgItemText(IDC_IMEI3_EDIT, L"");
			return CDialogEx::PreTranslateMessage(pMsg);
		}
		bThreadEnd = false;
		ADOManage adomanage;
		//m_StateView.Blank();
	//	int nVirtKey = (int)pMsg->wParam;
	//	if (nVirtKey == VK_RETURN)
		{//回车
			//if (pMsg->hwnd == GetDlgItem(IDC_IMEI3_EDIT)->GetSafeHwnd())
			{
				SetDlgItemText(IDC_HINT_STATIC, L"");
				SetDlgItemText(IDC_IMEI1_EDIT, _T(""));
				SetDlgItemText(IDC_IMEI2_EDIT, _T(""));
				//m_StateView.Blank();
				GetDlgItemText(IDC_IMEI3_EDIT, str3);
				SetDlgItemText(IDC_IMEI3_EDIT, L"");
				//GetDlgItem(IDC_IMEI3_EDIT)->EnableWindow(FALSE);
				SetDlgItemText(IDC_STATIC_LASTInput, str3);
				AddToRunList(_T("扫码输入：")+ str3);
				//-------1、判断输入是否合格 ----------------------//
				//通过字段关系匹配，查找扫错其他码，通过关系查找正确的IMEI号
				mm = IsNumber(str3);
				if (mm == FALSE)
				{			
					SetDlgItemText(IDC_HINT_STATIC, L"输入格式错误");
					
					AddToErrorList(_T("输入内容格式不合法"));
					PlayVoice(_T("格式错误"));
					SetEditEmpty();//清空编辑框
					GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
					return FALSE;
				//	return CDialogEx::PreTranslateMessage(pMsg);
				}
				//当用户选择的不是IMEI的时候才需要根据用户输入的数据去将它对应的IMEI找出来，因为改了需求，所以notype目前已经没用了
				if (notype > 0)
				{
					if (!adomanage.ConnSQL())
					{
						return FALSE;
					}
					resultflag3 = adomanage.CpImeiByNo(SyllablestrSql, notypename[notype], str3, strzhidan);//根据用户输入的数据去找IMEI
 					if (resultflag3 == 2)//返回2代表成功找到
					{
						str3 = adomanage.m_pRecordSet->GetCollect("IMEI");
						adomanage.CloseAll();
						SetDlgItemText(IDC_IMEI1_EDIT, str3);
						//再次查询匹配以后的
						mm = IsNumber(str3);
						if (mm == FALSE)
						{
							SetEditEmpty();//清空编辑框
							GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
							return FALSE;
						}
						AddToRunList(_T("当前匹配正确的IMEI号为：") + str3);
					}
					else
					{
						SetDlgItemText(IDC_HINT_STATIC, L"没匹配到IMEI号");
						AddToErrorList(_T("此订单没匹配到相应的IMEI号码！"));
						PlayVoice(_T("订单无此号"));
						SetEditEmpty();//清空编辑框
						GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();						
						return FALSE;
					}					
				}
				else
				{
					SetDlgItemText(IDC_IMEI1_EDIT, str3);
				}
				//-------1、判断输入是否合格 ----------------------//
				//----------2、再判断是否在订单IMEI区间内----------------------//				
				GetDlgItemText(IDC_IMEI1_EDIT, str3);
				if (!judgeimeirang(str3, strimeistart, strimeiend))
				{			
				//	RecordResult(_T("error201、号段在范围外"));//把错误日志写进数据库
					SetDlgItemText(IDC_HINT_STATIC, L"号段在范围外");
					PlayVoice(_T("号段异常"));
					SetEditEmpty();//清空编辑框
					GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
					AddToErrorList(L"号段在范围外");
					
					return FALSE;
				//	return CDialogEx::PreTranslateMessage(pMsg);
				}
				SetDlgItemText(IDC_IMEI1_EDIT, str3);//把IMEI号显示在显示栏目
				GetDlgItemText(IDC_IMEI1_EDIT, m_IMEIValue);
				//----------3、通过IMEI号，判断是否为对比工位的标志位，绑定关系是否为NULL，可检查出之前的加工是否正常----------------------//
				if (!adomanage.ConnSQL())
				{
					return FALSE;
				}
				resultflag2 = adomanage.CpCaiheByImei(str3, strzhidan);
				if (resultflag2 != 1)
				{					
					RecordResult(L"error302、对比标志位不正常");
					SetDlgItemText(IDC_HINT_STATIC, L"对比位异常");
					AddToErrorList(L"对比工位标志位不正常：" + m_IMEIValue);
					PlayVoice(_T("对比位异常"));
					SetDlgItemText(IDC_IMEI1_EDIT, _T(""));
					SetEditEmpty();//清空编辑框
				    GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
					
					return FALSE;
				}								
			    //-------------4、 查询对比工位彩盒判断字段CHRESULT号是否在订单中----------------------//前面很多地方判断过了，这里就不用了
				if (m_OpenImei3EditCheck.GetCheck())
				{
					ADOManage adomanage;
					if (!adomanage.ConnSQL())
					{
						return FALSE;
					}
					if (false == adomanage.CheckCHRESULT(str3))
					{
						SetDlgItemText(IDC_HINT_STATIC, L"彩盒判断位异常");
						AddToErrorList(L"查询彩盒判断标志位不正常：" + str3);
						PlayVoice(_T("彩盒检异常"));
						SetDlgItemText(IDC_IMEI1_EDIT, _T(""));
						SetEditEmpty();//清空编辑框
						GetDlgItem(IDC_IMEI3_EDIT)->SetFocus();
						
						return FALSE;
					}
					adomanage.CloseAll();
				}
				
			    //-------------5、 此产品的重量是否合格----------------------//
				pWeightThread = AfxBeginThread(GetWeightThread, this);//开启读数据的线程
				return TRUE;
			}
			//pMsg->wParam = VK_TAB;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

//判断扫入的数据是否为数字或者字母
BOOL CJIMIDlg::IsNumber(const CString& strTest)
{
	CString str = strTest;

	// 根据需要，决定是否要去掉字串两端空格、TAB或换行符
	str.TrimLeft();
	str.TrimRight();

	if (str.IsEmpty())
		return FALSE;

	str.TrimLeft(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	str.TrimRight(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	// 如果去掉数字和字母后，字串为空，说明字串中全部是字母或数字，否则就是非法字符
	// 如果不允许第一个字符为0，前面再加一个判断if(str.GetAt(0) != '0') return FALSE;
	//if ((str.GetAt(0) < '0'||str.GetAt(0)>'9')&& str!="") 
	//return FALSE;
	if (str.IsEmpty())
		return TRUE;

	return FALSE;
}

//判断扫入的数据是否为数字
BOOL CJIMIDlg::IsNumber2(const CString& strTest)
{
	CString str = strTest;

	// 根据需要，决定是否要去掉字串两端空格、TAB或换行符
	str.TrimLeft();
	str.TrimRight();

	if (str.IsEmpty())
		return FALSE;

	str.TrimLeft(L"0123456789.-");
	str.TrimRight(L"0123456789.-");
	// 如果去掉数字，字串为空，说明字串中全部是数字
	// 如果不允许第一个字符为0，前面再加一个判断if(str.GetAt(0) != '0') return FALSE;
	//if ((str.GetAt(0) < '0'||str.GetAt(0)>'9')&& str!="") 
	//return FALSE;
	if (str.IsEmpty())
		return TRUE;

	return FALSE;
}

//判断IMEI是否在范围内
BOOL CJIMIDlg::judgeimeirang(CString str, CString strimeistart, CString strimeiend)
{
	//设置无符号长整型变量来放IMEI起始和终止，还有传进来的要对比的IMEI号
	unsigned long long a = 0, b = 0, c = 0;

	//如果传入的IMEI号比IMEI起始号多一位，那就是有校验位的
	if (strimeistart.GetLength() == 14 && str.GetLength() == 15)
	{
		a = _atoi64(CStringA(strimeistart));
		b = _atoi64(CStringA(strimeiend));
		str = str.Left(str.GetLength() - 1);//从左到右读，str.GetLength() - 1就是将整个字符串最右边一位删掉
		c = _atoi64(CStringA(str));
		if (c >= a && c <= b)
		{
			return TRUE;
		}
		return FALSE;
	}
	//如果不是那就是没有校验位的
	else
	{
		if (str >= strimeistart && str <= strimeiend&&str.GetLength() == strimeistart.GetLength())
		{
			return TRUE;
		}
		return FALSE;
	}
}
//记录结果进数据库，此处只打印remark日志
void CJIMIDlg::RecordResult(CString sRemark)
{
	ADOManage adomanage;
	if (!adomanage.ConnSQL())
	{
		return;
	}
	adomanage.InsertResultLog(strzhidan, m_IMEIValue,m_WeightValue,m_UpLimit,m_DownLimit, m_OKValue,m_PCIP,m_UserName,sRemark);
	adomanage.CloseAll();
}

//记录结果进数据库总表，每个工位
void CJIMIDlg::ToResultSheet()
{
	ADOManage adomanage;
	if (!adomanage.ConnSQL())
	{
		return;
	}
	adomanage.adoToResultSheet(m_OKValue, m_IMEIValue);
	adomanage.CloseAll();
	
}
//获取当前的串口返回的有效的重量值
bool CJIMIDlg::GetWeightValue()
{
	int num = 40;//获取串口返回值的最大次数，次数*sleep（）时间即为超时时间200*130 =30000,即300s没获取到值，则获取稳定值超时
	int isafe = 0;//稳定值，共秤4次
	double dbnewVAL, dboldVALdb,dbUplimit,dbDownlimit;//最新重量值，上次重量值，上下限值
	dboldVALdb = 0;//初始化为0
	CString str, strText;
	for(int i = 0; i< num ; i++)
	{
		if (_T("")== m_newWeightValue)
		{
			Sleep(60);
			continue;
		}		
		dbnewVAL = _wtof(m_newWeightValue.GetBuffer());
		dbUplimit = _wtof(m_UpLimit.GetBuffer());
		dbDownlimit = _wtof(m_DownLimit.GetBuffer());

		//秤的精度为0.2，三次称重后，前后两次重量差在0.3
		if (fabs(dbnewVAL - dboldVALdb) <= 0.3)
		{
			if (isafe == 2)
			{//如果称量到第三次ok，则给m_WeightValue赋值
				m_WeightValue = m_newWeightValue;
				if (dbnewVAL> dbUplimit)
				{
					str.Format(_T("%.1f"), dbnewVAL- dbUplimit);
					sResult = _T("偏重:")+ str +_T("g");				
					m_OKValue = _T("0");
					iValResult = 2;
				//	m_StateView.Red();
					SetDlgItemText(IDC_HINT_STATIC, L"偏重");
					strText = _T("偏重");
					PlayVoice(strText);
					//AfxBeginThread(VoiceThread, this);//开启读数据的线程
				}
				else if (dbnewVAL < dbDownlimit)
				{
					str.Format(_T("%.1f"), dbDownlimit- dbnewVAL);
					sResult = _T("偏轻:") + str + _T("g");					
					m_OKValue = _T("0");
					iValResult = 1;
				//	m_StateView.Yellow();
					SetDlgItemText(IDC_HINT_STATIC, L"偏轻");
					strText = _T("偏轻");
					PlayVoice(strText);
				//	AfxBeginThread(VoiceThread, this);//开启读数据的线程
				}
				else 
				{
					str.Format(_T("%.1f"), dbnewVAL);
					sResult = _T("重量正常:") + str + _T("g");					
					m_OKValue = _T("1");
					iValResult = 0;
				//	m_StateView.Green();
					SetDlgItemText(IDC_HINT_STATIC, L"正常");
					strText = _T("正常");
					PlayVoice(strText);
				//	AfxBeginThread(VoiceThread, this);//开启读数据的线程
				}
				m_newWeightValue = _T("");
				return true;
				break;
			}
			else
			{//新值赋值给旧值，继续读
				dboldVALdb = dbnewVAL;
				isafe++;
				Sleep(60);
				continue;
			}		    	
		}
		else
		{//如果两者重量值差很多，则继续获取新的值
			iValResult = 3;
			Sleep(80);
			dboldVALdb = dbnewVAL;
		}
	}
	return false;
}

//获取当前的串口返回的有效的重量值
void CJIMIDlg::DealtSerialData(CString sSerialData)
{
	//"ST,NT,+ 4544.7 g"    串口数据案例
	
	int len = sSerialData.GetLength();
	if (len <16)
	{	//收到字符串太短，不合规矩
		return;
	}
	int pos = sSerialData.Find(_T("ST,"));
	if ((pos + 16)> len)
	{//当大于16时，尾部数据不全也不行
		return;
	}
	////////////-------UI上显示秤的读数-----////////////
	CString sview = sSerialData.Mid(pos + 6, 10);
	sview.Replace(_T(" "), _T(""));
	SetDlgItemText(IDC_WeightView, sview);//显示当前重量
	////////////-------UI上显示秤的读数-----////////////

	CString unit = sSerialData.Mid(pos + 14, 2);//单位的提取，Kg还是g
	unit.Trim();//去掉空格
	CString sign = sSerialData.Mid(pos + 6, 1);//+-提取
	CString sweight = sSerialData.Mid(pos + 7, 8);//带加减的字符串
	sweight.Trim();
	double dbvalue = _wtof(sweight);//重量转double型方便计算
	///////////单位换算//////////
	if (_T("g") == unit)
	{//为g则不做处理
	}
	else if (_T("kg") == unit)
	{
		dbvalue = dbvalue * 1000;//单位转换为g
	}
	else
	{
		return;
	}
	///////////单位换算//////////
	///////////+-换算并赋值给m_newWeightValue，然后等待数据库书的提取//////////
	if (_T("+") == sign)
	{
		if( dbvalue <= _wtof(m_disUpLimit) && dbvalue >= _wtof(m_disDownLimit))
		{
			m_newWeightValue.Format(_T("%.2f"), dbvalue); 
		}
		else
		{
			m_newWeightValue = _T("");
			return;//无效值
		}
		
	}
	else if (_T("-") == sign)
	{
		if (dbvalue <= _wtof(m_disUpLimit) && dbvalue >= _wtof(m_disDownLimit))
		{
			m_newWeightValue.Format(_T("-%.2f"), dbvalue);
		}
		else
		{
			m_newWeightValue = _T("");
			return;//无效值
		}
	}
	else
	{
		return;
	}
	return;
}

void CJIMIDlg::FontInit()
{
	Font1.CreatePointFont(400, L"宋体");
	Font2.CreatePointFont(300, L"隶书");
	Font3.CreatePointFont(200, L"隶书");
	Font4.CreatePointFont(200, L"宋体");
	Font5.CreatePointFont(600, L"黑体");

	GetDlgItem(IDC_WeightView)->SetFont(&Font1);

	GetDlgItem(IDC_IMEI2_EDIT)->SetFont(&Font2);
	GetDlgItem(IDC_IMEI1_EDIT)->SetFont(&Font2);
	GetDlgItem(IDC_IMEI3_EDIT)->SetFont(&Font3);
	GetDlgItem(IDC_STATIC_IMEISHOW)->SetFont(&Font4);
	GetDlgItem(IDC_STATIC_WEIGHTSHOW)->SetFont(&Font4);
	GetDlgItem(IDC_STATIC_g)->SetFont(&Font4);
	GetDlgItem(IDC_LOCK_BUTTON)->SetFont(&Font1);
	GetDlgItem(IDC_BUTTON_COMOK2)->SetFont(&Font4);
	GetDlgItem(IDC_BUTTON_UpdataOrder)->SetFont(&Font4);
	GetDlgItem(IDC_HINT_STATIC)->SetFont(&Font5);
	
}

void CJIMIDlg::OnBnClickedImei2syllableCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_imei2SyllableCheck.GetCheck())
	{
		SyllableMap[9] = L"IMEI2";
	}
	else
	{
		SyllableMap.erase(9);
	}
}

bool CJIMIDlg::CheckParam()
{
	UpdateData(TRUE);
	BOOL IMEI, SN, SIM, VIP, ICCID, BAT, MAC, Equipment, RFID, IMEI2;
	IMEI = m_imeiSyllableCheck.GetCheck();
	SN = m_snSyllableCheck.GetCheck();
	SIM = m_simSyllableCheck.GetCheck();
	VIP = m_vipSyllableCheck.GetCheck();
	ICCID = m_iccidSyllableCheck.GetCheck();
	BAT = m_batSyllableCheck.GetCheck();
	MAC = m_macSyllableCheck.GetCheck();
	Equipment = m_equipmentSyllableCheck.GetCheck();
	RFID = m_rfidSyllableCheck.GetCheck();
	IMEI2 = m_imei2SyllableCheck.GetCheck();
	if (FALSE == IMEI && FALSE == SN && FALSE == SIM && FALSE == VIP && FALSE == ICCID && FALSE == BAT && FALSE == MAC && FALSE == Equipment && FALSE == RFID && FALSE == IMEI2)
	{
		AfxMessageBox(_T("字段选项不能全为空"));
		return false;
	}
	if (FALSE == IMEI)
	{
		AfxMessageBox(_T("IMEI字段选项不能为空"));
		return false;
	}


	CString str1, str2, str3, str4;
	GetDlgItemText(IDC_EDIT_UpLimit, str1);
	GetDlgItemText(IDC_EDIT_DownLimit, str2);
	GetDlgItemText(IDC_EDIT_disUpLmt, str3);
	GetDlgItemText(IDC_EDIT_disDownLmt, str4);
	if (FALSE==IsNumber2(str1) || FALSE == IsNumber2(str2) || FALSE == IsNumber2(str3) || FALSE == IsNumber2(str4))
	{
		AfxMessageBox(_T("重量设置值不得含有字符!"));
		return false;
	}
	if (_T("") == str1 || _T("") == str2 || _T("") == str3 || _T("") == str4 )
	{
		AfxMessageBox(_T("重量设置值不得为空!"));
		return false;
	}
	if (_wtof(str1.GetBuffer()) <= _wtof(str2.GetBuffer()))
	{
		AfxMessageBox(_T("重量上限不得小于下限!"));
		return false;
	}
	if (_wtof(str3.GetBuffer()) <= _wtof(str4.GetBuffer()))
	{
		AfxMessageBox(_T("有效值上限不得小于下限!"));
		return false;;
	}
	if (_wtof(str1.GetBuffer()) >= _wtof(str3.GetBuffer()))
	{
		AfxMessageBox(_T("重量上限不得超过有效值上限!"));
		return false;;
	}
	if (_wtof(str2.GetBuffer()) <= _wtof(str4.GetBuffer()))
	{
		AfxMessageBox(_T("重量下限不得小于有效值下限!"));
		return false;;
	}
	return true;
}



void CJIMIDlg::OnBnClickedButtonLogin()
{
	//CString str;
	//GetDlgItemText(IDC_BUTTON_LOGIN, str);
	//if (_T("注销") == str)
	//{
	//	AddToRunList(_T("登录人员注销：") + m_UserName);
	//	m_UserName = _T("");
	//	SetDlgItemText(IDC_USERNAME_EDIT, m_UserName);
	//	m_UserRight = _T("");
	//	SetDlgItemText(IDC_UserRight_EDIT, m_UserRight);
	//	SetDlgItemText(IDC_BUTTON_LOGIN, _T("登录"));
	//	return;
	//}
	//else if (_T("登录") == str)
	{
		UserLogin userDlg;
		userDlg.DoModal();
		if (userDlg.sUSER == _T(""))
		{
			return ;
		}
		AddToRunList(_T("登录人员：") + userDlg.sUSER);
		m_UserName = userDlg.sUSER;
		SetDlgItemText(IDC_USERNAME_EDIT, m_UserName);
		m_UserRight = userDlg.sUSERRight;
		SetDlgItemText(IDC_UserRight_EDIT, m_UserRight);
		//SetDlgItemText(IDC_BUTTON_LOGIN, _T("注销"));
		if ((m_UserRight.Find(_T("&0000")) == -1) && (m_UserRight.Find(_T("&1001")) == -1))
		{
			WidgetStatue(FALSE);
		}
		else
		{
			WidgetStatue(TRUE);
		}
		return;
	}
}

void CJIMIDlg::WidgetStatue(BOOL Show)
{
	GetDlgItem(IDC_COMBO_COM)->EnableWindow(Show);
	GetDlgItem(IDC_BUTTON_COMOK2)->EnableWindow(Show);
	GetDlgItem(IDC_ZHIDAN_COMBO)->EnableWindow(Show);
	GetDlgItem(IDC_BUTTON_UpdataOrder)->EnableWindow(Show);
	GetDlgItem(IDC_IMEISYLLABLE_CHECK)->EnableWindow(Show);
	GetDlgItem(IDC_SNSYLLABLE_CHECK)->EnableWindow(Show);
	GetDlgItem(IDC_SIMSYLLABLE_CHECK)->EnableWindow(Show);
	GetDlgItem(IDC_VIPSYLLABLE_CHECK)->EnableWindow(Show);
	GetDlgItem(IDC_ICCIDSYLLABLE_CHECK)->EnableWindow(Show);
	GetDlgItem(IDC_BATSYLLABLE_CHECK)->EnableWindow(Show);
	GetDlgItem(IDC_MACSYLLABLE_CHECK)->EnableWindow(Show);
	GetDlgItem(IDC_EQUIPMENTSYLLABLE_CHECK)->EnableWindow(Show);
	GetDlgItem(IDC_RFIDSYLLABLE_CHECK)->EnableWindow(Show);
	GetDlgItem(IDC_IMEI2SYLLABLE_CHECK)->EnableWindow(Show);
	GetDlgItem(IDC_EDIT_UpLimit)->EnableWindow(Show);
	GetDlgItem(IDC_EDIT_disUpLmt)->EnableWindow(Show);
	GetDlgItem(IDC_EDIT_DownLimit)->EnableWindow(Show);
	GetDlgItem(IDC_EDIT_disDownLmt)->EnableWindow(Show);
	GetDlgItem(IDC_OPENIMEI3EDIT_CHECK)->EnableWindow(Show);
	GetDlgItem(IDC_SAVESYLLABLE_BUTTON)->EnableWindow(Show);
	GetDlgItem(IDC_READSYLLABLE_BUTTON)->EnableWindow(Show);

}