
// MFCP2CPDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCP2CP.h"
#include "MFCP2CPDlg.h"
#include "afxdialogex.h"
#include <mmsystem.h>
#include "DBconfig.h"
#include "ADOManage.h"

#pragma comment(lib,"winmm.lib")

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


// CMFCP2CPDlg 对话框



CMFCP2CPDlg::CMFCP2CPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCP2CPDlg::IDD, pParent)
	, m_imei1Edit(_T(""))
	, m_imei2Edit(_T(""))
	, strpcname(_T(""))
	, strpcip(_T(""))
	, notype(0)
	, strno1(L"NULL")
    , strno2(L"NULL")
	, strzhidan(L"")
	, chjudgeflag(0)
	, strimeistart(L"")
	, strimeiend(L"")
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	notypename[0] = L"IMEI";
	notypename[1] = L"SN";
	notypename[2] = L"SIM";
	notypename[3] = L"VIP";
	notypename[4] = L"ICCID";
	notypename[5] = L"BAT";
	notypename[6] = L"MAC";
	notypename[7] = L"Equipment";
}

void CMFCP2CPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HINT_STATIC, m_hintStatic);
	DDX_Control(pDX, IDC_IMEI2_STATIC, m_imei2Static);
	DDX_Control(pDX, IDC_IMEI1_STATIC, m_imei1Static);
	DDX_Text(pDX, IDC_IMEI1_EDIT, m_imei1Edit);
	DDX_Text(pDX, IDC_IMEI2_EDIT, m_imei2Edit);
	DDX_Control(pDX, IDC_COUPLING_CHECK, m_couplingCheck);
	DDX_Control(pDX, IDC_WRITENUM_CHECK, m_writenumCheck);
	DDX_Control(pDX, IDC_DOWNLOAD_CHECK, m_downloadCheck);
	DDX_Control(pDX, IDC_FUNTEST_CHECK, m_funtestCheck);
	DDX_Control(pDX, IDC_ZHIDAN_COMBO, m_zhidanCombo);
	DDX_Control(pDX, IDC_CHJUDGE_CHECK, m_caihejudgeCheck);
}

BEGIN_MESSAGE_MAP(CMFCP2CPDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMFCP2CPDlg::OnBnClickedOk)
	ON_EN_SETFOCUS(IDC_IMEI2_EDIT, &CMFCP2CPDlg::OnEnSetfocusImei2Edit)
	ON_BN_CLICKED(IDC_DBCONFIG_BUTTON, &CMFCP2CPDlg::OnBnClickedDbconfigButton)
	ON_BN_CLICKED(IDC_IMEI_RADIO, &CMFCP2CPDlg::OnBnClickedImeiRadio)
	ON_BN_CLICKED(IDC_SN_RADIO, &CMFCP2CPDlg::OnBnClickedSnRadio)
	ON_BN_CLICKED(IDC_SIM_RADIO, &CMFCP2CPDlg::OnBnClickedSimRadio)
	ON_BN_CLICKED(IDC_VIP_RADIO, &CMFCP2CPDlg::OnBnClickedVipRadio)
	ON_BN_CLICKED(IDC_ICCID_RADIO, &CMFCP2CPDlg::OnBnClickedIccidRadio)
	ON_BN_CLICKED(IDC_BAT_RADIO, &CMFCP2CPDlg::OnBnClickedBatRadio)
	ON_BN_CLICKED(IDC_MAC_RADIO, &CMFCP2CPDlg::OnBnClickedMacRadio)
	ON_BN_CLICKED(IDC_EQUIPMENT_RADIO, &CMFCP2CPDlg::OnBnClickedEquipmentRadio)
	ON_BN_CLICKED(IDC_UPDATEORDERNUMBER_BUTTON, &CMFCP2CPDlg::OnBnClickedUpdateordernumberButton)
	ON_CBN_SELCHANGE(IDC_ZHIDAN_COMBO, &CMFCP2CPDlg::OnCbnSelchangeZhidanCombo)
	ON_CBN_SELENDOK(IDC_ZHIDAN_COMBO, &CMFCP2CPDlg::OnCbnSelendokZhidanCombo)
	ON_CBN_KILLFOCUS(IDC_ZHIDAN_COMBO, &CMFCP2CPDlg::OnCbnKillfocusZhidanCombo)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHJUDGE_CHECK, &CMFCP2CPDlg::OnBnClickedChjudgeCheck)
END_MESSAGE_MAP()


// CMFCP2CPDlg 消息处理程序

BOOL CMFCP2CPDlg::OnInitDialog()
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
	//radiobutton变量设置
	CButton* imeiradio = (CButton*)GetDlgItem(IDC_IMEI_RADIO);
	CButton* snradio = (CButton*)GetDlgItem(IDC_SN_RADIO);
	CButton* vpiradio = (CButton*)GetDlgItem(IDC_VIP_RADIO);
	CButton* iccidradio = (CButton*)GetDlgItem(IDC_ICCID_RADIO);
	CButton* batradio = (CButton*)GetDlgItem(IDC_BAT_RADIO);
	CButton* macradio = (CButton*)GetDlgItem(IDC_MAC_RADIO);
	CButton* equipmentradio = (CButton*)GetDlgItem(IDC_EQUIPMENT_RADIO);
	CButton* simradio = (CButton*)GetDlgItem(IDC_SIM_RADIO);

	//字体大小
	fontinit();

	//初始化控件选项
	imeiradio->SetCheck(1);

	//初始化主机IP和名称
	GetLocalHostIPName(strpcname,strpcip);

	SetDlgItemText(IDC_PCNAME_EDIT, strpcname);
	SetDlgItemText(IDC_PCIP_EDIT, strpcip);

	//初始化订单号
	InitComboBox();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCP2CPDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCP2CPDlg::OnPaint()
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
HCURSOR CMFCP2CPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//编辑框转移焦点，核心函数,这是个系统自带的消息函数，用于检测键盘输入的东西，只要键盘有任何的输入都会触发这个函数
BOOL CMFCP2CPDlg::PreTranslateMessage(MSG* pMsg)
{
	CString str1, str2,str3;
	int mm;
	int resultflag1, resultflag2;
	ADOManage adomanage;
	
	GetDlgItemText(IDC_IMEI1_EDIT, str1);
	if (str1 == "")
	{
		return CDialogEx::PreTranslateMessage(pMsg);
	}
    
	// TODO:  在此添加专用代码和/或调用基类
	//不断的读键盘输入的东西
	if (pMsg->message == WM_KEYDOWN)
	{
		int nVirtKey = (int)pMsg->wParam;

		if (nVirtKey == VK_RETURN)
		{
			//如果是回车在这里做你要做的事情

			//下面这里是勾选不判断彩盒贴工位做的事情
			if (chjudgeflag == 1)
			{
				if (pMsg->hwnd == GetDlgItem(IDC_IMEI1_EDIT)->GetSafeHwnd())
				{
					GetDlgItemText(IDC_IMEI1_EDIT, str1);
					mm = IsNumber(str1);//判断str1是否带有非法字符

					//如果带有非法字符则直接提示号码错误
					if (mm == FALSE)
					{
						PlaySound(L"失败_对比失败.wav", NULL, SND_FILENAME | SND_ASYNC);
						SetDlgItemText(IDC_HINT_STATIC, L"号码错误");
						SetDlgItemText(IDC_IMEI1_EDIT, L"");
						SetDlgItemText(IDC_IMEI2_EDIT, L"");
						GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
						strno1 = L"NULL";
						return CDialogEx::PreTranslateMessage(pMsg);
					}

					SetDlgItemText(IDC_HINT_STATIC, L"等待");
				}
				else
				{
					GetDlgItemText(IDC_IMEI1_EDIT, str1);
					GetDlgItemText(IDC_IMEI2_EDIT, str2);

					//先判断两个框是否都是纯数字，不是的话就直接报错并不上传数据库
					mm = IsNumber(str1);
					if (mm == FALSE)
					{
						PlaySound(L"失败_对比失败.wav", NULL, SND_FILENAME | SND_ASYNC);
						SetDlgItemText(IDC_HINT_STATIC, L"号码错误");
						SetDlgItemText(IDC_IMEI1_EDIT, L"");
						SetDlgItemText(IDC_IMEI2_EDIT, L"");
						GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
						strno1 = L"NULL";
						return CDialogEx::PreTranslateMessage(pMsg);
					}
					mm = IsNumber(str2);
					if (mm == FALSE)
					{
						PlaySound(L"失败_对比失败.wav", NULL, SND_FILENAME | SND_ASYNC);
						SetDlgItemText(IDC_HINT_STATIC, L"号码错误");
						SetDlgItemText(IDC_IMEI1_EDIT, L"");
						SetDlgItemText(IDC_IMEI2_EDIT, L"");
						GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
						strno1 = L"NULL";
						return CDialogEx::PreTranslateMessage(pMsg);
					}

					//再判断是否相等
					if (str1 == str2&&judgeimeirang(str2, strimeistart, strimeiend) && judgeimeirang(str1, strimeistart, strimeiend))
					{
						adomanage.ConnSQL();

						if (adomanage.JudgeZhidan(str1,strzhidan))
						{
						PlaySound(L"失败_对比失败.wav", NULL, SND_FILENAME | SND_ASYNC);
						adomanage.InsertWrongImei(strzhidan, str1, str2, strno1, strno2, strpcip, notypename[notype], L"制单号错误", L"0");
						adomanage.CloseAll();
						SetDlgItemText(IDC_HINT_STATIC, L"制单号错误");
						SetDlgItemText(IDC_IMEI1_EDIT, L"");
						SetDlgItemText(IDC_IMEI2_EDIT, L"");
						GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
						strno1 = L"NULL";
						strno2 = L"NULL";
						return CDialogEx::PreTranslateMessage(pMsg);
						}

						PlaySound(L"通过.wav", NULL, SND_FILENAME | SND_ASYNC);
						adomanage.InsertCorrectImei(strzhidan, str1, str2, strno1, strno2, strpcip, L"IMEI", L"NULL", L"1");
						SetDlgItemText(IDC_HINT_STATIC, L"通过");
						adomanage.CloseAll();
						SetDlgItemText(IDC_IMEI1_EDIT, L"");
						SetDlgItemText(IDC_IMEI2_EDIT, L"");
						GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
						strno1 = L"NULL";
						strno2 = L"NULL";
						return CDialogEx::PreTranslateMessage(pMsg);
					}
					//不等就上传失败数据
					else
					{
						adomanage.ConnSQL();
						if (judgeimeirang(str2, strimeistart, strimeiend))
						{
							PlaySound(L"失败_对比失败.wav", NULL, SND_FILENAME | SND_ASYNC);
							adomanage.InsertWrongImei(strzhidan, str1, str2, strno1, strno2, strpcip, L"IMEI", L"IMEI号不匹配", L"0");
							adomanage.CloseAll();
							SetDlgItemText(IDC_HINT_STATIC, L"失败");
						}
						else
						{
							PlaySound(L"失败_对比失败.wav", NULL, SND_FILENAME | SND_ASYNC);
							adomanage.InsertWrongImei(strzhidan, str1, str2, strno1, strno2, strpcip, L"IMEI", L"号段在范围外", L"0");
							adomanage.CloseAll();
							SetDlgItemText(IDC_HINT_STATIC, L"号段在范围外");
						}
						SetDlgItemText(IDC_IMEI1_EDIT, L"");
						SetDlgItemText(IDC_IMEI2_EDIT, L"");
						GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
						strno1 = L"NULL";
						strno2 = L"NULL";
						return CDialogEx::PreTranslateMessage(pMsg);
					}
				}
			}

			//下面这里是没勾选不判断彩盒贴工位做的事情
			if (pMsg->hwnd == GetDlgItem(IDC_IMEI1_EDIT)->GetSafeHwnd())
			{
				adomanage.ConnSQL();//连接数据库

				//当用户选择的不是IMEI的时候才需要根据用户输入的数据去将它对应的IMEI找出来
				if (notype > 0)
				{
					GetDlgItemText(IDC_IMEI1_EDIT, strno1);
					resultflag1 = adomanage.CpImeiByNo(notypename[notype], strno1,strzhidan);//根据用户输入的数据去找IMEI
					//返回2代表成功找到
					if (resultflag1 == 2)
					{
						str1 = adomanage.m_pRecordSet->GetCollect("IMEI");
						SetDlgItemText(IDC_IMEI1_EDIT, str1);
						adomanage.CloseAll();
					}
					//返回3代表这是个IMEI，返回0和返回1都代表IMEI不存在或者用户输入的数据不存在
					else if (resultflag1 == 3)
					{
						adomanage.CloseAll();
					}
				}

				GetDlgItemText(IDC_IMEI1_EDIT, str1);
				mm = IsNumber(str1);//判断str1是否带有非法字符

				//resultflag = adomanage.JudgeImei(str1);

				//如果带有非法字符则直接提示号码错误
				if (mm == FALSE)
				{
					PlaySound(L"失败_对比失败.wav", NULL, SND_FILENAME | SND_ASYNC);
					SetDlgItemText(IDC_HINT_STATIC, L"号码错误");
					SetDlgItemText(IDC_IMEI1_EDIT, L"");
					SetDlgItemText(IDC_IMEI2_EDIT, L"");
					GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
					strno1 = L"NULL";
					return CDialogEx::PreTranslateMessage(pMsg);
				}

				SetDlgItemText(IDC_HINT_STATIC, L"等待");
			}
			else
			{
				GetDlgItemText(IDC_IMEI1_EDIT, str1);//先将第一个IMEI框中的数据放到str1中
				adomanage.ConnSQL();

				if (notype > 0)
				{
					GetDlgItemText(IDC_IMEI2_EDIT, strno2);

					//根据输入的数据去查找IMEI，返回0代表不存在此号段，返回1代表不存在IMEI，返回2代表成功，返回3代表这个是个IMEI号
					resultflag2 = adomanage.CpImeiByNo(notypename[notype], strno2, strzhidan);
					if (resultflag2 == 2)
					{
						str2 = adomanage.m_pRecordSet->GetCollect("IMEI");
						SetDlgItemText(IDC_IMEI2_EDIT, str2);
					}
					else if (resultflag2 == 0 || resultflag2 == 1)
					{
						PlaySound(L"失败_对比失败.wav", NULL, SND_FILENAME | SND_ASYNC);
						SetDlgItemText(IDC_HINT_STATIC, L"号段不存在或错误");
						adomanage.InsertWrongImei(strzhidan, str1, str2, strno1, strno2, strpcip, notypename[notype], L"所输号段不存在或错误", L"0");
						SetDlgItemText(IDC_IMEI1_EDIT, L"");
						SetDlgItemText(IDC_IMEI2_EDIT, L"");
						GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
						adomanage.CloseAll();
						strno1 = L"NULL";
						strno2 = L"NULL";
						return CDialogEx::PreTranslateMessage(pMsg);
					}
				}

				GetDlgItemText(IDC_IMEI2_EDIT, str2);

				//如果此时是IMEI号，则判断它是否是数字，不是的话就直接报错并不上传数据库
				if (notype == 0)
				{
					mm = IsNumber(str1);
					if (mm == FALSE)
					{
						PlaySound(L"失败_对比失败.wav", NULL, SND_FILENAME | SND_ASYNC);
						SetDlgItemText(IDC_HINT_STATIC, L"号码错误");
						SetDlgItemText(IDC_IMEI1_EDIT, L"");
						SetDlgItemText(IDC_IMEI2_EDIT, L"");
						GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
						strno1 = L"NULL";
						return CDialogEx::PreTranslateMessage(pMsg);
					}
					mm = IsNumber(str2);
					if (mm == FALSE)
					{
						PlaySound(L"失败_对比失败.wav", NULL, SND_FILENAME | SND_ASYNC);
						SetDlgItemText(IDC_HINT_STATIC, L"号码错误");
						SetDlgItemText(IDC_IMEI1_EDIT, L"");
						SetDlgItemText(IDC_IMEI2_EDIT, L"");
						GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
						strno1 = L"NULL";
						return CDialogEx::PreTranslateMessage(pMsg);
					}
				}
				mm = IsNumber(str2);

				resultflag1 = adomanage.JudgeImei(str1);

				if (mm == TRUE&&resultflag1 == 1)
				{
					if (str1 == str2&&judgeimeirang(str2, strimeistart, strimeiend) && judgeimeirang(str1, strimeistart, strimeiend))
					{
						resultflag2 = adomanage.CpCaiheByImei(str2, strzhidan);
						if (resultflag2 == 2)
						{
							PlaySound(L"通过.wav", NULL, SND_FILENAME | SND_ASYNC);
							adomanage.InsertCorrectImei(strzhidan, str1, str2, strno1, strno2, strpcip, notypename[notype], L"NULL", L"1");
							SetDlgItemText(IDC_HINT_STATIC, L"通过");
						}
						else if (resultflag2 == 1)
						{
							PlaySound(L"失败_对比失败.wav", NULL, SND_FILENAME | SND_ASYNC);
							adomanage.InsertWrongImei(strzhidan, str1, str2, strno1, strno2, strpcip, notypename[notype], L"漏打彩盒贴", L"0");
							strno1 = L"NULL";
							strno2 = L"NULL";
							SetDlgItemText(IDC_HINT_STATIC, L"漏打彩盒贴");
						}
						else if (resultflag2 == 3)
						{
							PlaySound(L"失败_对比失败.wav", NULL, SND_FILENAME | SND_ASYNC);
							adomanage.InsertWrongImei(strzhidan, str1, str2, strno1, strno2, strpcip, notypename[notype], L"制单号错误", L"0");
							strno1 = L"NULL";
							strno2 = L"NULL";
							SetDlgItemText(IDC_HINT_STATIC, L"制单号错误");
						}
						else
						{
							PlaySound(L"失败_对比失败.wav", NULL, SND_FILENAME | SND_ASYNC);
							adomanage.InsertWrongImei(strzhidan, str1, str2, strno1, strno2, strpcip, notypename[notype], L"所输号段不存在或错误", L"0");
							strno1 = L"NULL";
							strno2 = L"NULL";
							SetDlgItemText(IDC_HINT_STATIC, L"号段不存在或错误");
						}

						adomanage.CloseAll();
						SetDlgItemText(IDC_IMEI1_EDIT, L"");
						SetDlgItemText(IDC_IMEI2_EDIT, L"");
						GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
						strno1 = L"NULL";
						strno2 = L"NULL";
						return CDialogEx::PreTranslateMessage(pMsg);
					}
					else
					{
						if (judgeimeirang(str2, strimeistart, strimeiend))
						{
							PlaySound(L"失败_对比失败.wav", NULL, SND_FILENAME | SND_ASYNC);
							adomanage.InsertWrongImei(strzhidan, str1, str2, strno1, strno2, strpcip, notypename[notype], L"IMEI号不匹配", L"0");
							SetDlgItemText(IDC_HINT_STATIC, L"失败");
						}
						else
						{
							PlaySound(L"失败_对比失败.wav", NULL, SND_FILENAME | SND_ASYNC);
							adomanage.InsertWrongImei(strzhidan, str1, str2, strno1, strno2, strpcip, notypename[notype], L"号段在范围外", L"0");
							SetDlgItemText(IDC_HINT_STATIC, L"号段在范围外");
						}
						SetDlgItemText(IDC_IMEI1_EDIT, L"");
						SetDlgItemText(IDC_IMEI2_EDIT, L"");
						GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
						strno1 = L"NULL";
						strno2 = L"NULL";
						return CDialogEx::PreTranslateMessage(pMsg);
					}
				}
				else if (mm == FALSE)
				{
					PlaySound(L"失败_对比失败.wav", NULL, SND_FILENAME | SND_ASYNC);
					SetDlgItemText(IDC_HINT_STATIC, L"号码错误");
					SetDlgItemText(IDC_IMEI1_EDIT, L"");
					SetDlgItemText(IDC_IMEI2_EDIT, L"");
					GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
					strno1 = L"NULL";
					return CDialogEx::PreTranslateMessage(pMsg);
				}
				else
				{
					PlaySound(L"失败_对比失败.wav", NULL, SND_FILENAME | SND_ASYNC);
					adomanage.InsertWrongImei(strzhidan, str1, str2, strno1, strno2, strpcip, notypename[notype], L"所输号段不存在或错误", L"0");
					SetDlgItemText(IDC_HINT_STATIC, L"号段不存在或错误");
					SetDlgItemText(IDC_IMEI1_EDIT, L"");
					SetDlgItemText(IDC_IMEI2_EDIT, L"");
					GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
					strno1 = L"NULL";
					strno2 = L"NULL";
					return CDialogEx::PreTranslateMessage(pMsg);
				}
			}
			pMsg->wParam = VK_TAB;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


//判断扫入的数据是否为数字或者字母
BOOL CMFCP2CPDlg::IsNumber(const CString& strTest)
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
BOOL CMFCP2CPDlg::IsNumber2(const CString& strTest)
{
	CString str = strTest;

	// 根据需要，决定是否要去掉字串两端空格、TAB或换行符
	str.TrimLeft();
	str.TrimRight();

	if (str.IsEmpty())
		return FALSE;

	str.TrimLeft(L"0123456789");
	str.TrimRight(L"0123456789");
	// 如果去掉数字，字串为空，说明字串中全部是数字
	// 如果不允许第一个字符为0，前面再加一个判断if(str.GetAt(0) != '0') return FALSE;
	//if ((str.GetAt(0) < '0'||str.GetAt(0)>'9')&& str!="") 
	//return FALSE;
	if (str.IsEmpty())
		return TRUE;

	return FALSE;
}

//当IEMI1编辑框没数据的时候固定死在IMEI1编辑框
void CMFCP2CPDlg::OnEnSetfocusImei2Edit()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str1;
	GetDlgItemText(IDC_IMEI1_EDIT, str1);
	if (str1 == "")
	{
		GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
	}

}

//以下全是单选框点击事件
void CMFCP2CPDlg::OnBnClickedImeiRadio()
{
	// TODO:  在此添加控件通知处理程序代码
	notype = 0;
	SetDlgItemText(IDC_IMEI1_STATIC, L"机身IMEI");
	SetDlgItemText(IDC_IMEI2_STATIC, L"彩盒IMEI");
	SetDlgItemText(IDC_IMEI1_EDIT, L"");
	SetDlgItemText(IDC_IMEI2_EDIT, L"");
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
	strno1 = L"NULL";
	strno2 = L"NULL";
	SetDlgItemText(IDC_HINT_STATIC, L"就绪");
}

void CMFCP2CPDlg::OnBnClickedSnRadio()
{
	// TODO:  在此添加控件通知处理程序代码
	notype = 1;
	SetDlgItemText(IDC_IMEI1_STATIC, L"机身IMEI/SN");
	SetDlgItemText(IDC_IMEI2_STATIC, L"彩盒IMEI/SN");
	SetDlgItemText(IDC_IMEI1_EDIT, L"");
	SetDlgItemText(IDC_IMEI2_EDIT, L"");
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
	strno1 = L"NULL";
	strno2 = L"NULL";
	SetDlgItemText(IDC_HINT_STATIC, L"就绪");
}

void CMFCP2CPDlg::OnBnClickedSimRadio()
{
	// TODO:  在此添加控件通知处理程序代码
	notype = 2;
	SetDlgItemText(IDC_IMEI1_STATIC, L"机身IMEI/SIM");
	SetDlgItemText(IDC_IMEI2_STATIC, L"彩盒IMEI/SIM");
	SetDlgItemText(IDC_IMEI1_EDIT, L"");
	SetDlgItemText(IDC_IMEI2_EDIT, L"");
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
	strno1 = L"NULL";
	strno2 = L"NULL";
	SetDlgItemText(IDC_HINT_STATIC, L"就绪");
}

void CMFCP2CPDlg::OnBnClickedVipRadio()
{
	// TODO:  在此添加控件通知处理程序代码
	notype = 3;
	SetDlgItemText(IDC_IMEI1_STATIC, L"机身IMEI/VIP");
	SetDlgItemText(IDC_IMEI2_STATIC, L"彩盒IMEI/VIP");
	SetDlgItemText(IDC_IMEI1_EDIT, L"");
	SetDlgItemText(IDC_IMEI2_EDIT, L"");
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
	strno1 = L"NULL";
	strno2 = L"NULL";
	SetDlgItemText(IDC_HINT_STATIC, L"就绪");
}

void CMFCP2CPDlg::OnBnClickedIccidRadio()
{
	// TODO:  在此添加控件通知处理程序代码
	notype = 4;
	SetDlgItemText(IDC_IMEI1_STATIC, L"机身IMEI/ICC");
	SetDlgItemText(IDC_IMEI2_STATIC, L"彩盒IMEI/ICC");
	SetDlgItemText(IDC_IMEI1_EDIT, L"");
	SetDlgItemText(IDC_IMEI2_EDIT, L"");
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
	strno1 = L"NULL";
	strno2 = L"NULL";
	SetDlgItemText(IDC_HINT_STATIC, L"就绪");
}

void CMFCP2CPDlg::OnBnClickedBatRadio()
{
	// TODO:  在此添加控件通知处理程序代码
	notype = 5;
	SetDlgItemText(IDC_IMEI1_STATIC, L"机身IMEI/BAT");
	SetDlgItemText(IDC_IMEI2_STATIC, L"彩盒IMEI/BAT");
	SetDlgItemText(IDC_IMEI1_EDIT, L"");
	SetDlgItemText(IDC_IMEI2_EDIT, L"");
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
	strno1 = L"NULL";
	strno2 = L"NULL";
	SetDlgItemText(IDC_HINT_STATIC, L"就绪");
}

void CMFCP2CPDlg::OnBnClickedMacRadio()
{
	// TODO:  在此添加控件通知处理程序代码
	notype = 6;
	SetDlgItemText(IDC_IMEI1_STATIC, L"机身IMEI/MAC");
	SetDlgItemText(IDC_IMEI2_STATIC, L"彩盒IMEI/MAC");
	SetDlgItemText(IDC_IMEI1_EDIT, L"");
	SetDlgItemText(IDC_IMEI2_EDIT, L"");
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
	strno1 = L"NULL";
	strno2 = L"NULL";
	SetDlgItemText(IDC_HINT_STATIC, L"就绪");
}

void CMFCP2CPDlg::OnBnClickedEquipmentRadio()
{
	// TODO:  在此添加控件通知处理程序代码
	notype = 7;
	SetDlgItemText(IDC_IMEI1_STATIC, L"机身IMEI/EQU");
	SetDlgItemText(IDC_IMEI2_STATIC, L"彩盒IMEI/EQU");
	SetDlgItemText(IDC_IMEI1_EDIT, L"");
	SetDlgItemText(IDC_IMEI2_EDIT, L"");
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
	strno1 = L"NULL";
	strno2 = L"NULL";
	SetDlgItemText(IDC_HINT_STATIC, L"就绪");
}

//字体初始化函数
void CMFCP2CPDlg::fontinit()
{
	editfont.CreatePointFont(400, L"黑体");
	staticfont1.CreatePointFont(400, L"黑体");
	staticfont2.CreatePointFont(1400, L"黑体");
	checkfont.CreatePointFont(150, L"黑体");

	GetDlgItem(IDC_HINT_STATIC)->SetFont(&staticfont2);
	GetDlgItem(IDC_IMEI1_EDIT)->SetFont(&editfont);
	GetDlgItem(IDC_IMEI2_EDIT)->SetFont(&editfont);
	GetDlgItem(IDC_IMEI1_STATIC)->SetFont(&staticfont1);
	GetDlgItem(IDC_IMEI2_STATIC)->SetFont(&staticfont1);

	GetDlgItem(IDC_GROUP1_STATIC)->SetFont(&checkfont);
	GetDlgItem(IDC_DOWNLOAD_CHECK)->SetFont(&checkfont);
	GetDlgItem(IDC_WRITENUM_CHECK)->SetFont(&checkfont);
	GetDlgItem(IDC_FUNTEST_CHECK)->SetFont(&checkfont);
	GetDlgItem(IDC_COUPLING_CHECK)->SetFont(&checkfont);

	GetDlgItem(IDC_GROUP2_STATIC)->SetFont(&checkfont);
	GetDlgItem(IDC_SN_RADIO)->SetFont(&checkfont);
	GetDlgItem(IDC_VIP_RADIO)->SetFont(&checkfont);
	GetDlgItem(IDC_BAT_RADIO)->SetFont(&checkfont);
	GetDlgItem(IDC_ICCID_RADIO)->SetFont(&checkfont);
	GetDlgItem(IDC_MAC_RADIO)->SetFont(&checkfont);
	GetDlgItem(IDC_EQUIPMENT_RADIO)->SetFont(&checkfont);
	GetDlgItem(IDC_BAT_RADIO)->SetFont(&checkfont);
	GetDlgItem(IDC_SIM_RADIO)->SetFont(&checkfont);
	GetDlgItem(IDC_IMEI_RADIO)->SetFont(&checkfont);

	GetDlgItem(IDC_GROUP3_STATIC)->SetFont(&checkfont);
	GetDlgItem(IDC_PCNAME_STATIC)->SetFont(&checkfont);
	GetDlgItem(IDC_PCIP_STATIC)->SetFont(&checkfont);
	GetDlgItem(IDC_PCNAME_EDIT)->SetFont(&checkfont);
	GetDlgItem(IDC_PCIP_EDIT)->SetFont(&checkfont);

	GetDlgItem(IDC_GROUP3_STATIC)->SetFont(&checkfont);
	GetDlgItem(IDC_PCNAME_STATIC)->SetFont(&checkfont);
	GetDlgItem(IDC_PCIP_STATIC)->SetFont(&checkfont);
	GetDlgItem(IDC_PCNAME_EDIT)->SetFont(&checkfont);
	GetDlgItem(IDC_PCIP_EDIT)->SetFont(&checkfont);

	GetDlgItem(IDC_GROUP4_STATIC)->SetFont(&checkfont);
	GetDlgItem(IDC_ZHIDAN1_STATIC)->SetFont(&checkfont);
	GetDlgItem(IDC_IMEISTART_STATIC)->SetFont(&checkfont);
	GetDlgItem(IDC_IMEIEND_STATIC)->SetFont(&checkfont);
	GetDlgItem(IDC_ZHIDAN_COMBO)->SetFont(&checkfont);
	GetDlgItem(IDC_IMEISTART_EDIT)->SetFont(&checkfont);
	GetDlgItem(IDC_IMEIOVER_EDIT)->SetFont(&checkfont);
}

//改变颜色用的函数
HBRUSH CMFCP2CPDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	CString str;
	// TODO:  在此更改 DC 的任何特性
	GetDlgItemText(IDC_HINT_STATIC, str);
	if (pWnd->GetDlgCtrlID() == IDC_HINT_STATIC)
	{
		if (str == "号码错误" || str == "失败" || str == "漏打彩盒贴"||str=="号段不存在或错误"||str=="号段在范围外"||str=="制单号错误")
		{
			pDC->SetTextColor(RGB(255, 0, 0));//用RGB宏改变颜色 
			pDC->SelectObject(&staticfont2);
		}
		else if (str == "通过")
		{
			pDC->SetTextColor(RGB(0, 255, 0));//用RGB宏改变颜色 
			pDC->SelectObject(&staticfont2);
		}
		else
		{
			pDC->SetTextColor(RGB(0, 0, 0));//用RGB宏改变颜色 
			pDC->SelectObject(&staticfont2);
		}
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

//获取本机IP和地址
int CMFCP2CPDlg::GetLocalHostIPName(CString &sLocalName, CString &sIpAddress)
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

//数据库配置按钮
void CMFCP2CPDlg::OnBnClickedDbconfigButton()
{
	// TODO:  在此添加控件通知处理程序代码
	INT_PTR nRes;             // 用于保存DoModal函数的返回值   

	CDBconfig dbconfigDlg;           // 构造对话框类CTipDlg的实例   
	nRes = dbconfigDlg.DoModal();  // 弹出对话框

	if (IDCANCEL == nRes)
		return;
}

//给combox添加数据库中的订单号，更新按钮和开启后自动初始化都用这个函数
void CMFCP2CPDlg::InitComboBox()
{
	m_zhidanCombo.ResetContent();
	ADOManage adomanageon;
	adomanageon.ConnSQL();
	adomanageon.m_pRecordSet = adomanageon.GetOrderNumber();
	while (!adomanageon.m_pRecordSet->adoEOF)
	{
		m_zhidanCombo.AddString(adomanageon.m_pRecordSet->GetCollect("ZhiDan").bstrVal);
		adomanageon.m_pRecordSet->MoveNext();
	}
	adomanageon.CloseAll();
}

//更新订单按钮
void CMFCP2CPDlg::OnBnClickedUpdateordernumberButton()
{
	// TODO:  在此添加控件通知处理程序代码
	InitComboBox();
	SetDlgItemText(IDC_IMEISTART_EDIT, L"");
	SetDlgItemText(IDC_IMEIOVER_EDIT, L"");
	SetDlgItemText(IDC_IMEI1_EDIT, L"");
	SetDlgItemText(IDC_IMEI2_EDIT, L"");
	strno1 = L"NULL";
	strno2 = L"NULL";
	SetDlgItemText(IDC_HINT_STATIC, L"就绪");
	GetDlgItem(IDC_IMEI1_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_IMEI2_EDIT)->EnableWindow(FALSE);
}

//这个是选择后触发操作
void CMFCP2CPDlg::OnCbnSelchangeZhidanCombo()
{
	// TODO:  在此添加控件通知处理程序代码
	readimei();
	SetDlgItemText(IDC_IMEI1_EDIT, L"");
	SetDlgItemText(IDC_IMEI2_EDIT, L"");
	strno1 = L"NULL";
	strno2 = L"NULL";
	SetDlgItemText(IDC_HINT_STATIC, L"就绪");
	GetDlgItem(IDC_IMEI1_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_IMEI2_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
}

//这个是按回车后触发操作
void CMFCP2CPDlg::OnCbnSelendokZhidanCombo()
{
	// TODO:  在此添加控件通知处理程序代码
	readimei();
	SetDlgItemText(IDC_IMEI1_EDIT, L"");
	SetDlgItemText(IDC_IMEI2_EDIT, L"");
	strno1 = L"NULL";
	strno2 = L"NULL";
	SetDlgItemText(IDC_HINT_STATIC, L"就绪");
	GetDlgItem(IDC_IMEI1_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_IMEI2_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_IMEI1_EDIT)->SetFocus();
}

//根据订单号获取IMEI起始号和结束号
void CMFCP2CPDlg::readimei()
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
	adomanage.ConnSQL();
	GetIMEI = adomanage.GetIMEIByOrderNumber(strzhidan);
	if (GetIMEI->adoEOF)
	{
		SetDlgItemText(IDC_IMEISTART_EDIT, L"");
		SetDlgItemText(IDC_IMEIOVER_EDIT, L"");
		adomanage.CloseAll();
		return;
	}

	//让IMEI等全局号段获得值，以后记得添加进其它的全局号段，然后其它号段的值的设定在绑定那边用checkbox触发
	strimeistart = GetIMEI->GetCollect("IMEIStart");
	strimeiend = GetIMEI->GetCollect("IMEIEnd");
	SetDlgItemText(IDC_IMEISTART_EDIT, strimeistart);
	SetDlgItemText(IDC_IMEIOVER_EDIT, strimeiend);
	adomanage.CloseAll();
}

//镭雕判断订单号操作集成
BOOL CMFCP2CPDlg::JudgeZhidan(CString zhidan)
{
	_RecordsetPtr recordzhidan;
	int flag = 1;
	ADOManage adomanage;
	adomanage.ConnSQL();
	recordzhidan = adomanage.JudgeOrderNumber(zhidan);
	if (!recordzhidan->adoEOF)
	{
		flag = 0;
	}
	adomanage.CloseAll();
	return flag;
}

//订单号下拉框失去焦点后会做的事情
void CMFCP2CPDlg::OnCbnKillfocusZhidanCombo()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str1;
	GetDlgItemText(IDC_ZHIDAN_COMBO, str1);

	if (JudgeZhidan(str1))
	{
		MessageBox(L"订单号不存在！", L"提示信息", NULL);
		SetDlgItemText(IDC_ZHIDAN_COMBO, L"");
		SetDlgItemText(IDC_IMEISTART_EDIT, L"");
		SetDlgItemText(IDC_IMEIOVER_EDIT, L"");
		SetDlgItemText(IDC_IMEI1_EDIT, L"");
		SetDlgItemText(IDC_IMEI2_EDIT, L"");
		strno1 = L"NULL";
		strno2 = L"NULL";
		InitComboBox();
		GetDlgItem(IDC_IMEI1_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_IMEI2_EDIT)->EnableWindow(FALSE);
		return;
	}
	GetDlgItem(IDC_IMEI1_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_IMEI2_EDIT)->EnableWindow(TRUE);
}

//不判断彩盒贴工位复选框操作
void CMFCP2CPDlg::OnBnClickedChjudgeCheck()
{
	// TODO:  在此添加控件通知处理程序代码
	if (chjudgeflag == 0)
	{
		GetDlgItem(IDC_IMEI_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_VIP_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_SN_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_SIM_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_EQUIPMENT_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_MAC_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_ICCID_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BAT_RADIO)->EnableWindow(FALSE);
		CButton* imeiradio = (CButton*)GetDlgItem(IDC_IMEI_RADIO);
		CButton* snradio = (CButton*)GetDlgItem(IDC_SN_RADIO);
		CButton* vipradio = (CButton*)GetDlgItem(IDC_VIP_RADIO);
		CButton* iccidradio = (CButton*)GetDlgItem(IDC_ICCID_RADIO);
		CButton* batradio = (CButton*)GetDlgItem(IDC_BAT_RADIO);
		CButton* macradio = (CButton*)GetDlgItem(IDC_MAC_RADIO);
		CButton* equipmentradio = (CButton*)GetDlgItem(IDC_EQUIPMENT_RADIO);
		CButton* simradio = (CButton*)GetDlgItem(IDC_SIM_RADIO);
		imeiradio->SetCheck(1);
		snradio->SetCheck(0);
		vipradio->SetCheck(0);
		iccidradio->SetCheck(0);
		batradio->SetCheck(0);
		macradio->SetCheck(0);
		equipmentradio->SetCheck(0);
		simradio->SetCheck(0);
		OnBnClickedImeiRadio();
		chjudgeflag = 1;
	}
	else if (chjudgeflag == 1)
	{
		GetDlgItem(IDC_IMEI_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_VIP_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_SN_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_SIM_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_EQUIPMENT_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_MAC_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_ICCID_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_BAT_RADIO)->EnableWindow(TRUE);
		chjudgeflag = 0;
	}
}

//判断IMEI是否在范围内
BOOL CMFCP2CPDlg::judgeimeirang(CString str, CString strimeistart, CString strimeiend)
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
	    //strimeistart = strimeistart.TrimLeft(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	    //strimeiend = strimeiend.TrimLeft(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
		//str = str.TrimLeft(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
		//a = _atoi64(CStringA(strimeistart));
		//b = _atoi64(CStringA(strimeiend));
		//c = _atoi64(CStringA(str));
		if (str >= strimeistart && str <= strimeiend&&str.GetLength()==strimeistart.GetLength())
		{
			return TRUE;
		}
		return FALSE;
	}
}


//其它函数
void CMFCP2CPDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}
