
// MFCP6ComToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCP6ComTool.h"
#include "MFCP6ComToolDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CMFCP6ComToolDlg 对话框



CMFCP6ComToolDlg::CMFCP6ComToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCP6ComToolDlg::IDD, pParent)
	, m_WithEnterCheckValue(FALSE)
	, m_OutputLocalLogCheckValue(FALSE)
	, m_HexDisplayCheckValue(FALSE)
	, m_HexSendCheckValue(FALSE)
	, m_IsAddTimeCheckValue(FALSE)
	, m_AutoSendTimeValue(0)
	, m_AutoSendCheckValue(FALSE)
	, m_ListControlDelayTimeiValue(0)
	, ListloopsendFlag(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	PortNo = "";
	PortBaud = "";
	m_PortRadioValue = 0;
	PortIsOpenFlag = FALSE;
	SettingSrc = ".\\";
	ListHeaderName[0] = _T("ID");
	ListHeaderName[1] = _T("测试项");
	ListHeaderName[2] = _T("指令");
	ListHeaderName[3] = _T("顺序");
	ListHeaderName[4] = _T("延时");
}

void CMFCP6ComToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Information_RICHEDIT, m_InformationRicheditControl);
	DDX_Control(pDX, IDC_PortOrder_RICHEDIT, m_PortOrderRicheditControl);
	DDX_Control(pDX, IDC_PortNo_COMBO, m_PortNoComboControl);
	DDX_Control(pDX, IDC_PortBaud_COMBO, m_PortBaudComboControl);
	DDX_Radio(pDX, IDC_MtkPort_RADIO, m_PortRadioValue);
	DDX_Check(pDX, IDC_WithEnter_CHECK, m_WithEnterCheckValue);
	DDX_Check(pDX, IDC_OutputLocalLog_CHECK, m_OutputLocalLogCheckValue);
	DDX_Control(pDX, IDC_BLESETTING_LIST, m_BleSettingControl);
	DDX_Control(pDX, IDC_ListControl_EDIT, m_ListControlEdit);
	DDX_Control(pDX, IDC_LISTCONTROL_COMBO, m_ListControlCombo);
	DDX_Control(pDX, IDC_MtkPort_RADIO, m_MtkPortRadioControl);
	DDX_Control(pDX, IDC_RdaPort_RADIO, m_RdaPortRadioControl);
	DDX_Control(pDX, IDC_SendOrder_BUTTON, m_SendOrderControl);
	DDX_Control(pDX, IDC_ListLoopSend_BUTTON, m_ListLoopSendControl);
	DDX_Check(pDX, IDC_HexDisplay_CHECK, m_HexDisplayCheckValue);
	DDX_Check(pDX, IDC_HexSend_CHECK, m_HexSendCheckValue);
	DDX_Check(pDX, IDC_IsAddTime_CHECK, m_IsAddTimeCheckValue);
	DDX_Text(pDX, IDC_AutoSendTime_EDIT, m_AutoSendTimeValue);
	DDV_MinMaxInt(pDX, m_AutoSendTimeValue, 0, 999999);
	DDX_Check(pDX, IDC_AutoSend_CHECK, m_AutoSendCheckValue);
	DDX_Control(pDX, IDC_AutoSend_CHECK, m_AutoSendCheckControl);
	DDX_Text(pDX, IDC_ListControlDelayTime_EDIT, m_ListControlDelayTimeiValue);
	DDX_Control(pDX, IDC_ListControlDelayTime_EDIT, m_ListControlDelayTimeControl);
}

BEGIN_MESSAGE_MAP(CMFCP6ComToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMFCP6ComToolDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMFCP6ComToolDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_ClearInformation_BUTTON, &CMFCP6ComToolDlg::OnBnClickedClearinformationButton)
	ON_BN_CLICKED(IDC_OpenPort_BUTTON, &CMFCP6ComToolDlg::OnBnClickedOpenportButton)
	ON_BN_CLICKED(IDC_ClearSendOrder_BUTTON, &CMFCP6ComToolDlg::OnBnClickedClearsendorderButton)
	ON_BN_CLICKED(IDC_SendOrder_BUTTON, &CMFCP6ComToolDlg::OnBnClickedSendorderButton)
	ON_CBN_DROPDOWN(IDC_PortNo_COMBO, &CMFCP6ComToolDlg::OnCbnDropdownPortnoCombo)
	ON_BN_CLICKED(IDC_MtkPort_RADIO, &CMFCP6ComToolDlg::OnBnClickedMtkportRadio)
	ON_BN_CLICKED(IDC_RdaPort_RADIO, &CMFCP6ComToolDlg::OnBnClickedMtkportRadio)
	ON_BN_CLICKED(IDC_WithEnter_CHECK, &CMFCP6ComToolDlg::OnBnClickedWithenterCheck)
	ON_BN_CLICKED(IDC_OutputLocalLog_CHECK, &CMFCP6ComToolDlg::OnBnClickedOutputlocallogCheck)
	ON_BN_CLICKED(IDC_SETTINGINSERT_BUTTON, &CMFCP6ComToolDlg::OnBnClickedSettinginsertButton)
	ON_BN_CLICKED(IDC_SETTINGUP_BUTTON, &CMFCP6ComToolDlg::OnBnClickedSettingupButton)
	ON_BN_CLICKED(IDC_SETTINGDOWN_BUTTON, &CMFCP6ComToolDlg::OnBnClickedSettingdownButton)
	ON_BN_CLICKED(IDC_SETTINGCOPY_BUTTON, &CMFCP6ComToolDlg::OnBnClickedSettingcopyButton)
	ON_BN_CLICKED(IDC_SETTINGDELETE_BUTTON, &CMFCP6ComToolDlg::OnBnClickedSettingdeleteButton)
	ON_BN_CLICKED(IDC_SETTINGSAVE_BUTTON, &CMFCP6ComToolDlg::OnBnClickedSettingsaveButton)
	ON_NOTIFY(NM_DBLCLK, IDC_BLESETTING_LIST, &CMFCP6ComToolDlg::OnNMDblclkBlesettingList)
	ON_EN_KILLFOCUS(IDC_ListControl_EDIT, &CMFCP6ComToolDlg::OnEnKillfocusListcontrolEdit)
	ON_CBN_KILLFOCUS(IDC_LISTCONTROL_COMBO, &CMFCP6ComToolDlg::OnCbnKillfocusListcontrolCombo)
	ON_BN_CLICKED(IDC_SettingRead_BUTTON, &CMFCP6ComToolDlg::OnBnClickedSettingreadButton)
	ON_NOTIFY(NM_CLICK, IDC_BLESETTING_LIST, &CMFCP6ComToolDlg::OnNMClickBlesettingList)
	ON_BN_CLICKED(IDC_ListLoopSend_BUTTON, &CMFCP6ComToolDlg::OnBnClickedListloopsendButton)
	ON_BN_CLICKED(IDC_HexDisplay_CHECK, &CMFCP6ComToolDlg::OnBnClickedHexdisplayCheck)
	ON_BN_CLICKED(IDC_HexSend_CHECK, &CMFCP6ComToolDlg::OnBnClickedHexsendCheck)
	ON_BN_CLICKED(IDC_IsAddTime_CHECK, &CMFCP6ComToolDlg::OnBnClickedIsaddtimeCheck)
	ON_BN_CLICKED(IDC_AutoSend_CHECK, &CMFCP6ComToolDlg::OnBnClickedAutosendCheck)
	ON_EN_KILLFOCUS(IDC_ListControlDelayTime_EDIT, &CMFCP6ComToolDlg::OnEnKillfocusListcontroldelaytimeEdit)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_BLESETTING_LIST, &CMFCP6ComToolDlg::OnNMCustomdrawBlesettingList)
	ON_EN_KILLFOCUS(IDC_AutoSendTime_EDIT, &CMFCP6ComToolDlg::OnEnKillfocusAutosendtimeEdit)
END_MESSAGE_MAP()


// CMFCP6ComToolDlg 消息处理程序

BOOL CMFCP6ComToolDlg::OnInitDialog()
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
	InitToolSetting();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCP6ComToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCP6ComToolDlg::OnPaint()
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
HCURSOR CMFCP6ComToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/*初始化相关函数*/

//初始化软件
void  CMFCP6ComToolDlg::InitToolSetting()
{
	BOOL RdaFlag = RdaHostInterface.RDADllInit();
	if (RdaFlag == FALSE)
	{
		SetRicheditText("RDA初始化失败", 1);
	}
	LogName = GetLogTime() + "PortLog";
	SettingSrc = "";
	OnCbnDropdownPortnoCombo();
	InitBleSetting(&m_BleSettingControl);
	IniLoadConig();
	if (PortBaud == "")
	   m_PortBaudComboControl.SetWindowTextA("115200");
	UpdateData(FALSE);
	EnbleWindowIsOpenPort(PortIsOpenFlag);
}

//读取INI文件自动保存的信息
void  CMFCP6ComToolDlg::IniLoadConig()
{
	CFileFind finder;
	CString IniFileName = ".\\ComConfig.ini";
	CString ValueStr, SettringStr,SettringTitle = "Config";
	int ValueInt;

	BOOL ifFind = finder.FindFile(IniFileName);//先检测文件存不存在
	if (ifFind)
	{
		ValueInt = GetPrivateProfileInt(SettringTitle, _T("ComType"), 0, IniFileName);
		m_PortRadioValue = ValueInt;

		ValueInt = GetPrivateProfileInt(SettringTitle, _T("IsLogCheck"), 0, IniFileName);
		m_OutputLocalLogCheckValue = ValueInt;

		ValueInt = GetPrivateProfileInt(SettringTitle, _T("IsHexDisplayCheck"), 0, IniFileName);
		m_HexDisplayCheckValue = ValueInt;

		ValueInt = GetPrivateProfileInt(SettringTitle, _T("IsHexSendCheck"), 0, IniFileName);
		m_HexSendCheckValue = ValueInt;

		ValueInt = GetPrivateProfileInt(SettringTitle, _T("AutoSendTime"), 0, IniFileName);
		m_AutoSendTimeValue = ValueInt;

		ValueInt = GetPrivateProfileInt(SettringTitle, _T("IsEnterCheck"), 0, IniFileName);
		m_WithEnterCheckValue = ValueInt;

		GetPrivateProfileString(SettringTitle, _T("PortBaud"), _T(""), ValueStr.GetBuffer(50), 50, IniFileName);
		PortBaud = ValueStr;
		ValueStr.ReleaseBuffer();

		GetDlgItem(IDC_PortBaud_COMBO)->SetWindowTextA(PortBaud);


		GetPrivateProfileString(SettringTitle, _T("PortOrder"), _T(""), ValueStr.GetBuffer(50), 50, IniFileName);
		GetDlgItem(IDC_PortOrder_RICHEDIT)->SetWindowTextA(ValueStr);

		GetPrivateProfileString(SettringTitle, _T("PortNo"), _T(""), ValueStr.GetBuffer(50), 50, IniFileName);
		PortNo = ValueStr;
		ValueStr.ReleaseBuffer();
		int FindInt=m_PortNoComboControl.FindString(0, PortNo);
		if (FindInt >= 0)
		{
			m_PortNoComboControl.SetCurSel(FindInt);
		}

		GetPrivateProfileString(SettringTitle, _T("SettingSrc"), _T(""), ValueStr.GetBuffer(50), 50, IniFileName);
		SettingSrc = ValueStr;
		ValueStr.ReleaseBuffer();
		ListSettingLoadFun(SettingSrc);
	}

}

//INI文件自动保存
void  CMFCP6ComToolDlg::IniSaveConig()
{
	UpdateData(TRUE);
	CFileFind finder;
	CString IniFileName = ".\\ComConfig.ini";
	CString ValueStr, SettringStr, SettringTitle="Config";
	int ValueInt;

	int nSel1;
	nSel1 = m_PortNoComboControl.GetCurSel();
	if (nSel1 >= 0)
	{
		m_PortNoComboControl.GetLBText(nSel1, PortNo);
	}
	m_PortBaudComboControl.GetWindowTextA(PortBaud);

	ValueStr.Format("%d", m_PortRadioValue);
	WritePrivateProfileString(SettringTitle, _T("ComType"), ValueStr, IniFileName);

	GetDlgItem(IDC_PortOrder_RICHEDIT)->GetWindowTextA(ValueStr);
	WritePrivateProfileString(SettringTitle, _T("PortOrder"), ValueStr, IniFileName);

	ValueStr = PortNo;
	WritePrivateProfileString(SettringTitle, _T("PortNo"), ValueStr, IniFileName);

	ValueStr = PortBaud;
	WritePrivateProfileString(SettringTitle, _T("PortBaud"), ValueStr, IniFileName);

	ValueStr.Format("%d", m_OutputLocalLogCheckValue);
	WritePrivateProfileString(SettringTitle, _T("IsLogCheck"), ValueStr, IniFileName);

	ValueStr.Format("%d", m_WithEnterCheckValue);
	WritePrivateProfileString(SettringTitle, _T("IsEnterCheck"), ValueStr, IniFileName);

	ValueStr.Format("%d", m_HexDisplayCheckValue);
	WritePrivateProfileString(SettringTitle, _T("IsHexDisplayCheck"), ValueStr, IniFileName);

	ValueStr.Format("%d", m_HexSendCheckValue);
	WritePrivateProfileString(SettringTitle, _T("IsHexSendCheck"), ValueStr, IniFileName);

	ValueStr = SettingSrc;
	WritePrivateProfileString(SettringTitle, _T("SettingSrc"), ValueStr, IniFileName);

	ValueStr.Format("%d", m_AutoSendTimeValue);
	WritePrivateProfileString(SettringTitle, _T("AutoSendTime"), ValueStr, IniFileName);
}



/*界面控件函数*/
//触发清除当前的消息窗口
void CMFCP6ComToolDlg::OnBnClickedClearinformationButton()
{
	// TODO:  在此添加控件通知处理程序代码
	m_InformationRicheditControl.SetReadOnly(FALSE);
	m_InformationRicheditControl.SetSel(0,-1);
	m_InformationRicheditControl.Clear();
	m_InformationRicheditControl.SetReadOnly(TRUE);
}

//串口接收线程
UINT static __cdecl OpenComReceive_Thread(LPVOID pParam)
{
	CMFCP6ComToolDlg* Mead_Main_Win = (CMFCP6ComToolDlg*)pParam;
	Mead_Main_Win->ReceiveComOrder(Mead_Main_Win->Porthandler);
	return 0;
}

//打开当前选择的串口号函数
void CMFCP6ComToolDlg::OnBnClickedOpenportButton()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_OpenPort_BUTTON)->EnableWindow(FALSE);
	BOOL IsOpenFlag = TRUE;
	if (PortIsOpenFlag == FALSE)
	{
		IsOpenFlag=InitPort();
		if (IsOpenFlag == FALSE)
		{
			GetDlgItem(IDC_OpenPort_BUTTON)->EnableWindow(TRUE);
			return;
		}
		PortIsOpenFlag = TRUE;
		GetDlgItem(IDC_OpenPort_BUTTON)->SetWindowTextA("关闭端口");
		AfxBeginThread(OpenComReceive_Thread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
	else if (PortIsOpenFlag == TRUE)
	{
		IsOpenFlag = CloseCom(Porthandler);
		if (IsOpenFlag == FALSE)
		{
			GetDlgItem(IDC_OpenPort_BUTTON)->EnableWindow(TRUE);
			SetRicheditText("关闭端口失败", 1);
			return;
		}
		PortIsOpenFlag = FALSE;
		GetDlgItem(IDC_OpenPort_BUTTON)->SetWindowTextA("打开端口");
		SetRicheditText("关闭端口成功", 0);
	}

	GetDlgItem(IDC_OpenPort_BUTTON)->EnableWindow(TRUE);
	EnbleWindowIsOpenPort(PortIsOpenFlag);
}

//清除当前指令窗口
void CMFCP6ComToolDlg::OnBnClickedClearsendorderButton()
{
	// TODO:  在此添加控件通知处理程序代码
	m_PortOrderRicheditControl.SetSel(0, -1);
	m_PortOrderRicheditControl.Clear();
}

//点击发送指令按钮
void CMFCP6ComToolDlg::OnBnClickedSendorderButton()
{
	// TODO:  在此添加控件通知处理程序代码
	CString PortOrderStr;
	m_PortOrderRicheditControl.GetWindowTextA(PortOrderStr);

	SendComOrderHandleFun(PortOrderStr);
}

//串口发送指令的综合处理函数
void CMFCP6ComToolDlg::SendComOrderHandleFun(CString PortOrder)
{
	//如果没有勾上不带回车换行就加上\r\n
	if (m_WithEnterCheckValue == 0)
	{
		PortOrder += "\r\n";
	}

	SetRicheditText("发->□" + PortOrder, 0);

	if (m_PortRadioValue == 0)
	{
		SendComOrder(Porthandler, PortOrder);
	}
	else if (m_PortRadioValue == 1)
	{
		RdaHostInterface.RDAComWriteData(0, PortOrder);
	}
}

//串口号下拉框
void CMFCP6ComToolDlg::OnCbnDropdownPortnoCombo()
{
	// TODO:  在此添加控件通知处理程序代码
	FindCommPort(&m_PortNoComboControl, PortNo, 0);
}

void CMFCP6ComToolDlg::OnBnClickedMtkportRadio()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
}

void CMFCP6ComToolDlg::OnBnClickedWithenterCheck()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
}

void CMFCP6ComToolDlg::OnBnClickedOutputlocallogCheck()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
}

void CMFCP6ComToolDlg::OnBnClickedHexdisplayCheck()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
}

void CMFCP6ComToolDlg::OnBnClickedHexsendCheck()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
}

void CMFCP6ComToolDlg::OnBnClickedIsaddtimeCheck()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
}


UINT static __cdecl AutoSend_Thread(LPVOID pParam)
{
	BOOL flag;

	CMFCP6ComToolDlg* Mead_Main_Win = (CMFCP6ComToolDlg*)pParam;
	Mead_Main_Win->AutoSendComOrder();
	return 0;
}

//定时发送
void CMFCP6ComToolDlg::OnBnClickedAutosendCheck()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_AutoSendCheckValue == TRUE)
	{
		AfxBeginThread(AutoSend_Thread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
}

void CMFCP6ComToolDlg::AutoSendComOrder()
{
	while (m_AutoSendCheckValue)
	{
		CString PortOrderStr;
		m_PortOrderRicheditControl.GetWindowTextA(PortOrderStr);
		UpdateData(TRUE);
		Sleep(m_AutoSendTimeValue);
		SendComOrderHandleFun(PortOrderStr);
	}
}

//视串口开启与否，禁用与开启相关控件
void CMFCP6ComToolDlg::EnbleWindowIsOpenPort(BOOL Chooes)
{
	m_PortNoComboControl.EnableWindow(!Chooes);
	m_PortBaudComboControl.EnableWindow(!Chooes);
	m_MtkPortRadioControl.EnableWindow(!Chooes);
	m_RdaPortRadioControl.EnableWindow(!Chooes);
	m_SendOrderControl.EnableWindow(Chooes);
	m_ListLoopSendControl.EnableWindow(Chooes);
	m_AutoSendCheckControl.EnableWindow(Chooes);
}


/*串口通用操作*/
//从注册表中寻找已经注册串口号然后展示出来
void CMFCP6ComToolDlg::FindCommPort(CComboBox *pComboBox, CString &ComNo, int PortNO)
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
	if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Hardware\\DeviceMap\\SerialComm", NULL, KEY_READ, &hKey) == ERROR_SUCCESS) // 打开串口注册表
	{
		int i = 0;
		char portName[256], commName[256];
		DWORD dwLong, dwSize;
		while (1)
		{
			dwLong = dwSize = sizeof(portName);
			if (::RegEnumValue(hKey, i, LPSTR(portName), &dwLong, NULL, NULL, (PUCHAR)commName, &dwSize) == ERROR_NO_MORE_ITEMS) // 枚举串口
			{
				break;
			}
			if (nSel >= 0 && ComNo == LPSTR(commName))//如果跟上次选择的相等那就不让当前选择变动
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
			pComboBox->AddString(LPSTR(commName));
			i++;
		}
		if (pComboBox->GetCount() == 0)
		{
			pComboBox->ResetContent();
			RegCloseKey(hKey);
			return;
		}
		pComboBox->SetCurSel(cur);
		RegCloseKey(hKey);
	}
}

//初始化串口集成函数
BOOL CMFCP6ComToolDlg::InitPort()
{
	BOOL FindFlag=GetCommPort(&m_PortNoComboControl, &m_PortBaudComboControl),InitFlag=TRUE;
	
	if (FindFlag == FALSE)
	{
		return FALSE;
	}

	if (m_PortRadioValue == 0)
	{
		Porthandler = InitCom(PortNo);
	}
	else if (m_PortRadioValue == 1)
	{
		InitFlag=RdaHostInterface.RDAComInit(0, PortNo);
		if (InitFlag == TRUE)
		{
			SetRicheditText("RDA串口初始化成功！", 0);
		}
	}

	if (Porthandler == NULL || InitFlag == FALSE)
	{
		SetRicheditText("串口初始化失败!请检查串口是否被占用！", 1);
		return FALSE;
	}

	return TRUE;
}

//初始化串口
HANDLE CMFCP6ComToolDlg::InitCom(CString comName)
{
	HANDLE hCom;
	hCom = CreateFile("\\\\.\\" + comName,//COM口
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
	dcb.BaudRate = atoi(PortBaud);
	dcb.ByteSize = 8; //每个字节有8位
	dcb.Parity = NOPARITY; //无奇偶校验位
	dcb.StopBits = ONESTOPBIT; //1个停止位
	SetCommState(hCom, &dcb);
	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
	SetRicheditText("MTK串口初始化成功！", 0);
	return hCom;
}

//关闭串口
BOOL CMFCP6ComToolDlg::CloseCom(HANDLE handler)
{
	if (m_PortRadioValue == 0)
	{
		return CloseHandle(handler);
	}
	else if (m_PortRadioValue == 1)
	{
		return RdaHostInterface.RDAComShutdown(0);
	}
	return TRUE;
}

//获取当前串口号
BOOL CMFCP6ComToolDlg::GetCommPort(CComboBox *pComboBox, CComboBox *pBaudBox)
{
	int nSel1;
	nSel1 = pComboBox->GetCurSel();
	pComboBox->GetLBText(nSel1, PortNo);

	if (PortNo == "")
	{
		MessageBox("请先选择串口号！", "提示信息", NULL);
		return FALSE;
	}

	pBaudBox->GetWindowTextA(PortBaud);

	if (PortBaud == "")
	{
		MessageBox("请先选择波特率！", "提示信息", NULL);
		return FALSE;
	}
	return TRUE;
}

//CString转Byte
BYTE * CMFCP6ComToolDlg::CStrToByte(int len, CString str)
{
	BYTE *Buf = new BYTE[len];
	CString StrSub;
	for (int iSrc = 0, iDst = 0; iDst<len; iSrc += 2, iDst++)
	{
		StrSub = str.Mid(iSrc, 2);
		Buf[iDst] = (BYTE)_tcstoul(StrSub, NULL, 16);//16是十六进制的意思
	}
	return Buf;
}

//串口发指令
void  CMFCP6ComToolDlg::SendComOrder(HANDLE PortHandler, CString SendOrder)
{
	PurgeComm(PortHandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat, bReadStatus;
	char *buffer;

	ClearCommError(PortHandler, &dwErrorFlags, &ComStat);

	if (m_HexSendCheckValue == FALSE)
	{
		buffer = SendOrder.GetBuffer(SendOrder.GetLength());
		bWriteStat = WriteFile(PortHandler, buffer, SendOrder.GetLength(), &dwBytesWrite, NULL);
	}
	else if (m_HexSendCheckValue == TRUE)
	{
		//先转十六进制
		SendOrder.Replace(" ", "");
		SendOrder.Replace("\r\n", "");
		int len = SendOrder.GetLength() / 2-1;
		BYTE *pBData = new BYTE[len];
		pBData = CStrToByte(len, SendOrder);

		//发送十六进制数据
		bWriteStat = WriteFile(PortHandler, (LPVOID)pBData, len, &dwBytesWrite, NULL);
	}
	Sleep(1);
}

//串口收到回复
void  CMFCP6ComToolDlg::ReceiveComOrder(HANDLE PortHandler)
{
	CString ReceiveData="";
	DWORD dwBytesRead;
	BOOL bReadStatus;
	char *buffer,buf[1024];
	while (PortIsOpenFlag)
	{
		if (m_PortRadioValue == 0)
		{
			memset(buf, 0, sizeof(buf));
			buffer = buf;
			bReadStatus = ReadFile(PortHandler, buffer, 512, &dwBytesRead, NULL);
			if (dwBytesRead != 0)
			{
				ReceiveData = buffer;
			}
		}
		else if (m_PortRadioValue == 1)
		{
			ReceiveData = RDAComReceive[0];
		}

		if (m_HexDisplayCheckValue == FALSE)
		{
			if (ReceiveData != "")
			{
				SetRicheditText("收<-■" + ReceiveData, 0);
			}
			ReceiveData = "";
			RDAComReceive[0] = "";
		}
		else if (m_HexDisplayCheckValue == TRUE)
		{
			if (ReceiveData != "")
			{
				CString charVtemp1, charVtemp2, charVtemp3;
				unsigned char *str = (unsigned char*)ReceiveData.GetBuffer(0);
				for (int i = 0; i < ReceiveData.GetLength(); i++)
				{
					charVtemp1.Format(_T("%02X "), (unsigned char)str[i]);
					charVtemp3 += charVtemp1;
				}
				SetRicheditText("收<-■" + charVtemp3, 0);
			}
			RDAComReceive[0] = "";
			ReceiveData = "";
		}
		Sleep(50);
	}	
}

//串口指令收发
void CMFCP6ComToolDlg::SendAndReceiveComOrder(HANDLE PortHandler, CString SendOrder, CString &ReceiveData, CString &IsSuccessFlag)
{
	PurgeComm(PortHandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags, dwBytesRead;
	BOOL bWriteStat, bReadStatus;
	char *buffer, buf[1024];

	ClearCommError(PortHandler, &dwErrorFlags, &ComStat);

	buffer = SendOrder.GetBuffer(SendOrder.GetLength());
	bWriteStat = WriteFile(PortHandler, buffer, SendOrder.GetLength(), &dwBytesWrite, NULL);

	for (int i = 0; i < 6; i++)
	{

		memset(buf, 0, sizeof(buf));
		buffer = buf;
		bReadStatus = ReadFile(PortHandler, buffer, 512, &dwBytesRead, NULL);
		if (dwBytesRead != 0)
		{
			ReceiveData = buffer;
			IsSuccessFlag = "Success";
			return;

		}
		else
		{
			Sleep(100 * i);
			continue;
		}
	}
	IsSuccessFlag = "Falied";
	return;
}


/*信息日志模块函数*/
//信息日志核心函数
void CMFCP6ComToolDlg::SetRicheditText(CString strMsg, int No)
{
	m_InformationRicheditControl.SetSel(-1, -1);
	CHARFORMAT   cf{ sizeof(cf) };//设置这个结构来改变你想要的文字的颜色等属性
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD | CFM_SIZE | CFM_FACE;//这个目前还没解析完成，但如果要输出不同颜色的字体一定要使用richedit
	cf.crTextColor = RGB(255, 0, 0);//设置为红色

	CString a = "收<-■";

	if (m_OutputLocalLogCheckValue == TRUE)
	{
		PrintLog(strMsg, 0);
	}

	if (m_IsAddTimeCheckValue == FALSE)
	{
		strMsg = GetTime() + _T("\r\n  ") + strMsg + _T("\r\n");
	}
	else if (m_IsAddTimeCheckValue == TRUE)
	{
		strMsg = strMsg.Right(strMsg.GetLength()-a.GetLength());
	}

	switch (No)
	{
	case 0:
		m_InformationRicheditControl.ReplaceSel(strMsg);
		m_InformationRicheditControl.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
		break;
	case 1:
		m_InformationRicheditControl.SetSelectionCharFormat(cf);
		m_InformationRicheditControl.ReplaceSel(strMsg);
		m_InformationRicheditControl.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
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
CString CMFCP6ComToolDlg::GetTime()
{
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d/%d/%d %02d:%02d:%02d.%03d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	return strTime;
}

//获取日志时间
CString CMFCP6ComToolDlg::GetLogTime()
{
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d%02d%02d"), time.wYear, time.wMonth, time.wDay);
	return strTime;
}

//日志打印函数
void CMFCP6ComToolDlg::PrintLog(CString strMsg, int No)
{
	CLog Relog0;//日志文件

	switch (No)
	{
	case 0:
		Relog0.WriteLog(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"), LogName);
		break;
	default:
		break;
	}
}


/*配置工具模块*/
//初始化配置列表
void CMFCP6ComToolDlg::InitBleSetting(CListCtrl *m_ListControl)
{
	//初始化列表控件
	CRect rect;
	// 获取编程语言列表视图控件的位置和大小   
	m_ListControl->GetClientRect(&rect);

	// 为列表视图控件添加全行选中和栅格风格   
	m_ListControl->SetExtendedStyle(m_ListControl->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//删除掉所有的表头
	while (1) {
		if (!m_ListControl->DeleteColumn(0)) {
			break;
		}
	}

	// 为列表视图控件添加列
	m_ListControl->InsertColumn(0, ListHeaderName[0], LVCFMT_CENTER, rect.Width() / 12 * 1.2, 0);
	m_ListControl->InsertColumn(1, ListHeaderName[1], LVCFMT_CENTER, rect.Width() / 12 * 3, 0);
	m_ListControl->InsertColumn(2, ListHeaderName[2], LVCFMT_CENTER, rect.Width() / 12 * 5.3, 0);
	m_ListControl->InsertColumn(3, ListHeaderName[3], LVCFMT_CENTER, rect.Width() / 12 * 1.2, 0);
	m_ListControl->InsertColumn(4, ListHeaderName[4], LVCFMT_CENTER, rect.Width() / 12 * 1.5, 0);
}

//点击插入新行按钮
void CMFCP6ComToolDlg::OnBnClickedSettinginsertButton()
{
	// TODO:  在此添加控件通知处理程序代码
	ListSettingInsert(&m_BleSettingControl);
}

//点击向上移动按钮
void CMFCP6ComToolDlg::OnBnClickedSettingupButton()
{
	// TODO:  在此添加控件通知处理程序代码
	ListSettingUp(&m_BleSettingControl);
}

//点击向下移动按钮
void CMFCP6ComToolDlg::OnBnClickedSettingdownButton()
{
	// TODO:  在此添加控件通知处理程序代码
	ListSettingDown(&m_BleSettingControl);
}

//点击复制按钮
void CMFCP6ComToolDlg::OnBnClickedSettingcopyButton()
{
	// TODO:  在此添加控件通知处理程序代码
	ListSettingCopy(&m_BleSettingControl);
}

//点击删除按钮
void CMFCP6ComToolDlg::OnBnClickedSettingdeleteButton()
{
	// TODO:  在此添加控件通知处理程序代码
	ListSettingDelete(&m_BleSettingControl);
}

//点击保存按钮
void CMFCP6ComToolDlg::OnBnClickedSettingsaveButton()
{
	// TODO:  在此添加控件通知处理程序代码
	ListSettingSave(&m_BleSettingControl);
}

//点击读取按钮
void CMFCP6ComToolDlg::OnBnClickedSettingreadButton()
{
	// TODO:  在此添加控件通知处理程序代码
	ListSettingLoad(&m_BleSettingControl);
}

UINT static __cdecl ListLoop_Thread(LPVOID pParam)
{
	BOOL flag;

	CMFCP6ComToolDlg* Mead_Main_Win = (CMFCP6ComToolDlg*)pParam;
	Mead_Main_Win->ListLoopSendComOrder();
	return 0;
}

void CMFCP6ComToolDlg::ListLoopSendComOrder()
{
	CString SendCountStr,SendNo;
	int SendCountint = 0, SendCountTemp;
	while (ListloopsendFlag)
	{
		int SendCountTempArray[100];
		int ListItemCountInt = m_BleSettingControl.GetItemCount();
		SendCountTemp = 0;
		for (int i = 0; i < ListItemCountInt; i++)
		{
			SendCountStr = m_BleSettingControl.GetItemText(i, 3);
			if (SendCountStr == "0")
			{
				continue;
			}
			else
			{
				SendCountTempArray[SendCountTemp] = atoi(SendCountStr);
				SendCountTemp++;
			}
		}

		int SendCountCurrentTempInt=0;
		for (int i = 0; i < SendCountTemp; i++)
		{
			if (SendCountTempArray[i] > SendCountint&&i > 0 && SendCountTempArray[i] != 0 && SendCountTempArray[i] != SendCountint)
			{
				if (SendCountCurrentTempInt>SendCountTempArray[i])
					SendCountCurrentTempInt = SendCountTempArray[i];
				else if (SendCountCurrentTempInt==0)
					SendCountCurrentTempInt = SendCountTempArray[i];
			}
			else if (SendCountTempArray[i] > SendCountint&&i == 0 && SendCountTempArray[i] != 0 && SendCountTempArray[i] != SendCountint)
			{
				SendCountCurrentTempInt = SendCountTempArray[i];
			}
		}
		
		if (SendCountCurrentTempInt == SendCountint || SendCountCurrentTempInt==0)
		{
			SendCountint = 0;
			continue;
		}
		SendCountint=SendCountCurrentTempInt;

		CString PortOrderStr, PortDelayStr;
		int ColorInt;
		for (int i = 0; i < ListItemCountInt; i++)
		{
			SendNo = m_BleSettingControl.GetItemText(i, 3);
			m_BleSettingControl.SetItemData(ColorInt, 0);
			if (SendCountint == atoi(SendNo))
			{
				PortOrderStr = m_BleSettingControl.GetItemText(i, 2);
				PortDelayStr = m_BleSettingControl.GetItemText(i, 4);
				m_BleSettingControl.SetItemData(i, 1);
				ColorInt = i;
				SendComOrderHandleFun(PortOrderStr);
				Sleep(atoi(PortDelayStr));
			}
		}
		Sleep(10);
	}
}

//点击顺序发送按钮，里面是个线程
void CMFCP6ComToolDlg::OnBnClickedListloopsendButton()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_ListLoopSend_BUTTON)->EnableWindow(FALSE);
	if (ListloopsendFlag==FALSE)
	{
		ListloopsendFlag = TRUE;
		AfxBeginThread(ListLoop_Thread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		GetDlgItem(IDC_ListLoopSend_BUTTON)->SetWindowTextA("停止发送");
	}
	else if (ListloopsendFlag == TRUE)
	{
		ListloopsendFlag = FALSE;
		GetDlgItem(IDC_ListLoopSend_BUTTON)->SetWindowTextA("顺序发送");
	}
	GetDlgItem(IDC_ListLoopSend_BUTTON)->EnableWindow(TRUE);
}

//配置列表插入新行
void CMFCP6ComToolDlg::ListSettingInsert(CListCtrl *m_ListControl)
{
	int SelectCount = 0;
	int ListItemCountInt = m_ListControl->GetItemCount();
	CString ListItemCountStr;
	ListItemCountStr.Format("%d", ListItemCountInt);

	int SelectMark = m_ListControl->GetSelectionMark();

	//判断当前列表的被选中的行数
	for (int i = ListItemCountInt - 1; i >= 0; i--)
	{
		if (LVIS_SELECTED == m_ListControl->GetItemState(i, LVIS_SELECTED))
		{
			SelectCount++;
		}
	}

	//没选中和选中多行默认插入到末尾，选中一行则默认插入到那一行之后
	if (SelectMark == -1 || SelectCount>1 || SelectCount == 0)
	{
		m_ListControl->InsertItem(ListItemCountInt, ListItemCountStr);

		//让插入行高亮
		for (int i = 0; i < ListItemCountInt; i++)
		{
			m_ListControl->SetItemState(i, 0, -1);
		}
		m_ListControl->SetItemState(ListItemCountInt, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_ListControl->SetFocus();
		m_ListControl->SetSelectionMark(ListItemCountInt);
	}
	else if (SelectMark != -1 && SelectCount == 1)
	{
		m_ListControl->InsertItem(SelectMark + 1, ListItemCountStr);

		CString TempSelectMark;
		for (int i = SelectMark, j = 0; i <= ListItemCountInt; i++, j++)
		{
			TempSelectMark.Format("%d", SelectMark + j);
			m_ListControl->SetItemText(i, 0, TempSelectMark);
		}

		//让插入行高亮
		m_ListControl->SetItemState(SelectMark, 0, -1);
		m_ListControl->SetItemState(SelectMark + 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_ListControl->SetFocus();
		m_ListControl->SetSelectionMark(SelectMark + 1);
	}
	m_ListControl->SetItemText(SelectMark + 1, 3, "0");
	m_ListControl->SetItemText(SelectMark + 1, 4, "0");
}

//配置列表选中行向上移动
void CMFCP6ComToolDlg::ListSettingUp(CListCtrl *m_ListControl)
{
	m_ListControl->SetFocus();

	int Columns = m_ListControl->GetHeaderCtrl()->GetItemCount();
	CString TempA, TempB;
	int ListItemCountInt = m_ListControl->GetItemCount();
	CString ListItemCountStr;
	ListItemCountStr.Format("%d", ListItemCountInt);
	int TopMark = 0;


	//判断当前顶端可移动的位置，假设0为高亮，那么可移动位置为1，假设0和1都是高亮，那么可移动位置就是2
	for (int i = 0; i < ListItemCountInt; i++)
	{
		if (LVIS_SELECTED == m_ListControl->GetItemState(i, LVIS_SELECTED))
		{
			if (i == TopMark)
			{
				TopMark++;
				continue;
			}
			else if (TopMark == ListItemCountInt)
			{
				return;
			}
			//上下两行进行互换
			for (int j = 1; j < Columns; j++)
			{
				TempA = m_ListControl->GetItemText(i - 1, j);
				TempB = m_ListControl->GetItemText(i, j);
				m_ListControl->SetItemText(i, j, TempA);
				m_ListControl->SetItemText(i - 1, j, TempB);
			}
			//让移动行高亮
			m_ListControl->SetItemState(i, 0, -1);
			m_ListControl->SetItemState(i - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			m_ListControl->SetSelectionMark(i - 1);
		}
	}
	m_ListControl->SetFocus();
}

//配置列表选中行向下移动
void CMFCP6ComToolDlg::ListSettingDown(CListCtrl *m_ListControl)
{
	m_ListControl->SetFocus();

	int Columns = m_ListControl->GetHeaderCtrl()->GetItemCount();
	CString TempA, TempB;
	int ListItemCountInt = m_ListControl->GetItemCount();
	CString ListItemCountStr;
	ListItemCountStr.Format("%d", ListItemCountInt);
	int BottomMark = ListItemCountInt - 1;

	//判断当前底端可移动的位置，原理参考顶端
	for (int i = ListItemCountInt - 1; i >= 0; i--)
	{
		if (LVIS_SELECTED == m_ListControl->GetItemState(i, LVIS_SELECTED))
		{
			if (i == BottomMark)
			{
				BottomMark--;
				continue;
			}
			else if (BottomMark == 0)
			{
				return;
			}
			//上下两行进行互换
			for (int j = 1; j < Columns; j++)
			{
				TempA = m_ListControl->GetItemText(i, j);
				TempB = m_ListControl->GetItemText(i + 1, j);
				m_ListControl->SetItemText(i + 1, j, TempA);
				m_ListControl->SetItemText(i, j, TempB);
			}
			//让移动行高亮
			m_ListControl->SetItemState(i, 0, -1);
			m_ListControl->SetItemState(i + 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			m_ListControl->SetSelectionMark(i + 1);
		}
	}
	m_ListControl->SetFocus();
}

//配置列表复制选中行
void CMFCP6ComToolDlg::ListSettingCopy(CListCtrl *m_ListControl)
{
	m_ListControl->SetFocus();

	int SelectCount = 0;
	int Columns = m_ListControl->GetHeaderCtrl()->GetItemCount();
	int ListItemCountInt = m_ListControl->GetItemCount();
	CString ListItemCountStr;
	ListItemCountStr.Format("%d", ListItemCountInt);

	//判断当前列表的被选中的行数，加亮行数+原始行数才为最终列表数据数量
	for (int i = ListItemCountInt - 1; i >= 0; i--)
	{
		if (LVIS_SELECTED == m_ListControl->GetItemState(i, LVIS_SELECTED))
		{
			SelectCount++;
		}
	}

	//对高亮行进行复制
	for (int i = 0; i < ListItemCountInt + SelectCount; i++)
	{
		if (LVIS_SELECTED == m_ListControl->GetItemState(i, LVIS_SELECTED))
		{
			m_ListControl->InsertItem(i + 1, m_ListControl->GetItemText(i, 0));

			for (int j = 1; j < Columns; j++)
			{
				m_ListControl->SetItemText(i + 1, j, m_ListControl->GetItemText(i, j));
			}
			//让复制出来的那一行高亮
			m_ListControl->SetItemState(i, 0, -1);
			m_ListControl->SetItemState(i + 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			i++;//因为多了一行，所以这时候i要加1
			m_ListControl->SetSelectionMark(i);
		}
	}
	m_ListControl->SetFocus();

	//重新排下序
	CString TempSelectMark;
	for (int i = 0; i <= m_ListControl->GetItemCount(); i++)
	{
		TempSelectMark.Format("%d", i);
		m_ListControl->SetItemText(i, 0, TempSelectMark);
	}
}

//配置列表删除选中行
void CMFCP6ComToolDlg::ListSettingDelete(CListCtrl *m_ListControl)
{
	////先询问是否需要删除
	//if (MessageBox(TEXT("您确定要删除数据吗？"), TEXT("提示"), MB_OKCANCEL) != IDOK)
	//{
	//	return;
	//}

	m_ListControl->SetFocus();

	int DeleteCount = 0;
	int ListItemCountInt = m_ListControl->GetItemCount();

	//判断当前列表的被选中行然后进行删除
	for (int i = ListItemCountInt - 1; i >= 0; i--)
	{
		if (LVIS_SELECTED == m_ListControl->GetItemState(i, LVIS_SELECTED))
		{
			m_ListControl->DeleteItem(i);
			DeleteCount++;
		}
	}

	//如果没删过数据就不要重新排序了
	if (DeleteCount == 0)
	{
		return;
	}

	//重新排下序
	CString TempSelectMark;
	for (int i = 0; i <= m_ListControl->GetItemCount(); i++)
	{
		TempSelectMark.Format("%d", i);
		m_ListControl->SetItemText(i, 0, TempSelectMark);
	}
	m_ListControl->SetSelectionMark(-1);//然后重新设置一下列表焦点
}

//配置列表保存
void CMFCP6ComToolDlg::ListSettingSave(CListCtrl *m_ListControl)
{
	//设置过滤器
	TCHAR szFilter[] = _T("保存指令配置文件(*.ini)|*.ini|");
	//构造打开文件对话框
	CFileDialog fileDlg(FALSE, SettingSrc, NULL, 0, szFilter, this);
	if (IDOK == fileDlg.DoModal())
	{
		// 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里
		SettingSrc = fileDlg.GetPathName();
	}
	else
	{
		return;
	}

	int Columns = m_ListControl->GetHeaderCtrl()->GetItemCount();
	LVCOLUMN col;//获取标题名称用的
	col.mask = LVCF_TEXT;

	int ListItemCountInt = m_ListControl->GetItemCount();
	CString ListItemCountStr;
	ListItemCountStr.Format("%d", ListItemCountInt);

	CString ListControlSingleData = "";

	CFileFind finder;
	CString ValueStr, StrSetting = "ListSetting", StrValue;
	int ValueInt;

	BOOL ifFind = finder.FindFile(SettingSrc);
	if (ifFind)
	{
		DeleteFile(SettingSrc);
	}

	//取得数据总条数写入INI文件，然后再将指令逐条写入INI文件中
	WritePrivateProfileString("Config", "SettingCount", ListItemCountStr, SettingSrc);

	for (int i = 0; i < ListItemCountInt; i++)
	{
		StrSetting.Format("ListSetting%d", i);
		for (int j = 0; j < Columns; j++)
		{
			//ListControlSingleData += ListHeaderName[j] + ":";
			ListControlSingleData += m_ListControl->GetItemText(i, j) + "#$&";
		}
		WritePrivateProfileString("Config", StrSetting, ListControlSingleData, SettingSrc);
		ListControlSingleData = "";
	}

	MessageBox("保存完毕！", "提示");
}

//配置列表读取
void CMFCP6ComToolDlg::ListSettingLoad(CListCtrl *m_ListControl)
{

	int ISSaveFlag = m_ListControl->GetItemCount();

	if (ISSaveFlag != 0)
	{
		//先询问是否需要保存
		if (MessageBox(TEXT("需要先要保存配置吗？"), TEXT("提示"), MB_OKCANCEL) != IDOK)
		{

		}
		else
		{
			ListSettingSave(m_ListControl);
		}
	}

	//设置过滤器
	TCHAR szFilter[] = _T("读取指令配置文件(*.ini)|*.ini|");
	//构造打开文件对话框
	CFileDialog fileDlg(TRUE, SettingSrc, NULL, 0, szFilter, this);
	fileDlg.m_ofn.lpstrInitialDir = SettingSrc;
	if (IDOK == fileDlg.DoModal())
	{
		// 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里
		SettingSrc = fileDlg.GetPathName();
	}
	else
	{
		return;
	}

	CString ListControlSingleData = "";
	int ListSettingCount=0;

	CFileFind finder;
	CString ValueStr, StrSetting = "ListSetting", StrValue;
	int ValueInt;

	BOOL ifFind = finder.FindFile(SettingSrc);
	if (ifFind)
	{
		ListSettingLoadFun(SettingSrc);
	}
}

BOOL CMFCP6ComToolDlg::ListSettingLoadFun(CString SettingSrc)
{
	CString ListControlSingleData = "";
	int ListSettingCount = 0;
	CString ValueStr, StrSetting = "ListSetting", StrValue;
	int ValueInt;
	ListSettingCount = GetPrivateProfileInt("Config", "SettingCount", -1, SettingSrc);

	if (SettingSrc == "")
	{
		return FALSE;
	}

	if (ListSettingCount == -1)
	{
		MessageBox("配置文件已损坏！请选择其它配置文件！", "警告！");
		return FALSE;
	}
	m_BleSettingControl.DeleteAllItems();

	for (int i = 0; i < ListSettingCount; i++)
	{
		CString SettingCut;
		StrSetting.Format("ListSetting%d", i);
		GetPrivateProfileString("Config", StrSetting, _T(""), ListControlSingleData.GetBuffer(128), 128, SettingSrc);
		ListControlSingleData.ReleaseBuffer();

		SettingCut = ListControlSingleData.Left(ListControlSingleData.Find("#$&"));
		ListControlSingleData = ListControlSingleData.Right(ListControlSingleData.GetLength() - ListControlSingleData.Find("#$&") - 3);
		m_BleSettingControl.InsertItem(i, SettingCut);

		for (int j = 1; j < 5; j++)
		{
			SettingCut = ListControlSingleData.Left(ListControlSingleData.Find("#$&"));
			ListControlSingleData = ListControlSingleData.Right(ListControlSingleData.GetLength() - ListControlSingleData.Find("#$&") - 3);
			m_BleSettingControl.SetItemText(i, j, SettingCut);
		}
		ListControlSingleData = "";
	}
	return TRUE;
}

//双击列表时触发的事件
void CMFCP6ComToolDlg::OnNMDblclkBlesettingList(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString PortOrderStr;
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CRect rc;
	m_Row = pNMListView->iItem;//被选中行的行号

	if (m_Row == -1)
		return;
	
	m_Colum = pNMListView->iSubItem;//被选中行的列号

	switch (pNMListView->iSubItem)
	{
	case 0:
		//if (PortIsOpenFlag)
		//{
		//	PortOrderStr = m_BleSettingControl.GetItemText(m_Row, 2);
		//	SendComOrderHandleFun(PortOrderStr);
		//}
		break;
	case 3:
	case 4:
		m_BleSettingControl.GetSubItemRect(m_Row, m_Colum, LVIR_LABEL, rc);//获取子项在RECT的位置
		m_ListControlDelayTimeControl.SetParent(&m_BleSettingControl);//转换坐标为列表框中的坐标
		m_ListControlDelayTimeControl.MoveWindow(rc);//移动Eidt编辑框到RECT所在的位置
		m_ListControlDelayTimeControl.SetWindowText(m_BleSettingControl.GetItemText(m_Row, m_Colum));//将该子项的数据显示到编辑框中
		m_ListControlDelayTimeControl.ShowWindow(SW_SHOW);//显示Edit控件
		m_ListControlDelayTimeControl.SetFocus();//设置Edit焦点
		m_ListControlDelayTimeControl.ShowCaret();//显示光标
		m_ListControlDelayTimeControl.SetSel(-1);//将光标移动到最后
		break;
	default:
		m_BleSettingControl.GetSubItemRect(m_Row, m_Colum, LVIR_LABEL, rc);//获取子项在RECT的位置
		m_ListControlEdit.SetParent(&m_BleSettingControl);//转换坐标为列表框中的坐标
		m_ListControlEdit.MoveWindow(rc);//移动Eidt编辑框到RECT所在的位置
		m_ListControlEdit.SetWindowText(m_BleSettingControl.GetItemText(m_Row, m_Colum));//将该子项的数据显示到编辑框中
		m_ListControlEdit.ShowWindow(SW_SHOW);//显示Edit控件
		m_ListControlEdit.SetFocus();//设置Edit焦点
		m_ListControlEdit.ShowCaret();//显示光标
		m_ListControlEdit.SetSel(-1);//将光标移动到最后
		break;
	}

	//if (pNMListView->iSubItem != 0)
	//{
	//	m_BleSettingControl.GetSubItemRect(m_Row, m_Colum, LVIR_LABEL, rc);//获取子项在RECT的位置
	//	m_ListControlEdit.SetParent(&m_BleSettingControl);//转换坐标为列表框中的坐标
	//	m_ListControlEdit.MoveWindow(rc);//移动Eidt编辑框到RECT所在的位置
	//	m_ListControlEdit.SetWindowText(m_BleSettingControl.GetItemText(m_Row,m_Colum));//将该子项的数据显示到编辑框中
	//	m_ListControlEdit.ShowWindow(SW_SHOW);//显示Edit控件
	//	m_ListControlEdit.SetFocus();//设置Edit焦点
	//	m_ListControlEdit.ShowCaret();//显示光标
	//	m_ListControlEdit.SetSel(-1);//将光标移动到最后
	//}

	*pResult = 0;
}

//单击鼠标左键
void CMFCP6ComToolDlg::OnNMClickBlesettingList(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString PortOrderStr;

	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CRect rc;
	m_Row = pNMListView->iItem;//被选中行的行号

	if (m_Row == -1)
		return;


	m_Colum = pNMListView->iSubItem;//被选中行的列号

	if (m_Colum == 5)
		return;

	switch (pNMListView->iSubItem)
	{
	case 0:
		if (PortIsOpenFlag)
		{
			PortOrderStr = m_BleSettingControl.GetItemText(m_Row, 2);
			SendComOrderHandleFun(PortOrderStr);
		}
		break;
	default:
		break;
	}
	*pResult = 0;
}

void CMFCP6ComToolDlg::OnNMCustomdrawBlesettingList(NMHDR *pNMHDR, LRESULT *pResult)
{

	LPNMTVCUSTOMDRAW pNMCD = reinterpret_cast<LPNMTVCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NMCUSTOMDRAW nmCustomDraw = pNMCD->nmcd;

	switch (nmCustomDraw.dwDrawStage)
	{
	case CDDS_ITEMPREPAINT:
	{
		if (0 == nmCustomDraw.lItemlParam)
		{
			pNMCD->clrTextBk = RGB(255, 255, 255);
			pNMCD->clrText = RGB(0, 0, 0);
		}
		else if (1 == nmCustomDraw.lItemlParam)
		{
			pNMCD->clrTextBk = RGB(255, 0, 0);		//背景颜色
			pNMCD->clrText = RGB(255, 255, 255);		//文字颜色
		}
		else if (2 == nmCustomDraw.lItemlParam)
		{
			pNMCD->clrTextBk = RGB(51, 153, 255);
			pNMCD->clrText = RGB(255, 255, 255);
		}
		else
		{
			//
		}
		break;
	}
	default:
	{
		break;
	}
	}

	*pResult = 0;
	*pResult |= CDRF_NOTIFYPOSTPAINT;		//必须有，不然就没有效果
	*pResult |= CDRF_NOTIFYITEMDRAW;		//必须有，不然就没有效果
	return;
}

void CMFCP6ComToolDlg::OnEnKillfocusAutosendtimeEdit()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str, judge;
	GetDlgItem(IDC_AutoSendTime_EDIT)->GetWindowText(str);//取得编辑框的内容
	judge = str;
	judge.Trim("0123546789");
	if (judge != "")
	{
		UpdateData(FALSE);
		MessageBox("请输入有效数字", NULL);
		return;
	}
	UpdateData(TRUE);
}

//移动鼠标焦点后
void CMFCP6ComToolDlg::OnEnKillfocusListcontrolEdit()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str;
	m_ListControlEdit.GetWindowText(str);//取得编辑框的内容
	m_BleSettingControl.SetItemText(m_Row, m_Colum, str);//将该内容更新到列表对应的项中
	m_ListControlEdit.ShowWindow(SW_HIDE);//隐藏编辑框
}

//移动鼠标焦点后
void CMFCP6ComToolDlg::OnCbnKillfocusListcontrolCombo()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str;
	m_ListControlCombo.GetWindowText(str);//取得编辑框的内容
	m_BleSettingControl.SetItemText(m_Row, m_Colum, str);//将该内容更新到列表对应的项中
	m_ListControlCombo.ShowWindow(SW_HIDE);//隐藏编辑框
}

//移动鼠标焦点后
void CMFCP6ComToolDlg::OnEnKillfocusListcontroldelaytimeEdit()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str,judge;
	m_ListControlDelayTimeControl.GetWindowText(str);//取得编辑框的内容
	judge = str;
	judge.Trim("0123546789");
	if (judge != "")
	{
		m_ListControlDelayTimeControl.ShowWindow(SW_HIDE);//隐藏编辑框
		MessageBox("请输入有效数字",NULL);
		return;
	}

	m_BleSettingControl.SetItemText(m_Row, m_Colum, str);//将该内容更新到列表对应的项中
	m_ListControlDelayTimeControl.ShowWindow(SW_HIDE);//隐藏编辑框
}


/*其它函数*/
void CMFCP6ComToolDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}

void CMFCP6ComToolDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	int ISSaveFlag = m_BleSettingControl.GetItemCount();

	if (ISSaveFlag != 0)
	{
		//先询问是否需要保存
		if (MessageBox(TEXT("需要先要保存配置吗？"), TEXT("提示"), MB_OKCANCEL) != IDOK)
		{

		}
		else
		{
			ListSettingSave(&m_BleSettingControl);
		}
	}

	INT_PTR nRes;
	nRes = MessageBox(_T("您确定要退出吗？"), _T("提示"), MB_OKCANCEL | MB_ICONQUESTION);
	
	// 判断消息对话框返回值。如果为IDCANCEL就return，否则继续向下执行   
	if (IDCANCEL == nRes)
		return;

	IniSaveConig();

	CDialogEx::OnCancel();
}


