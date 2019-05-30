// BT_Addr_Setting.cpp : 实现文件
//

#include "stdafx.h"
#include "WriteIMEI.h"
#include "BT_Addr_Setting.h"
#include "WriteIMEIDlg.h"

CPara1 m_para1;
// CBT_Addr_Setting 对话框

IMPLEMENT_DYNAMIC(CBT_Addr_Setting, CDialog)

CBT_Addr_Setting::CBT_Addr_Setting(CWnd* pParent /*=NULL*/)
	: CDialog(CBT_Addr_Setting::IDD, pParent)
	, BtAddrStart(_T(""))
	, BtAddrEnd(_T(""))
	, DB_CONNECT(FALSE)
	, m_server(_T(""))
	, m_db(_T(""))
	, m_user(_T(""))
	, m_pwd(_T(""))
	, Order_Num_up(_T(""))
	, Machine_Type(_T(""))
	, Modulation_Tppe(_T(""))
	, Line_Number(_T(""))
	, Floor(_T(""))
	, Product_Address(_T(""))
	, Is_Return(_T(""))
	, Software_Version(_T(""))
	, Operator(_T(""))
	, BT_Addr_EndShow(_T(""))
	, Hardware_Version(_T(""))
	, IMEI1_Choose(FALSE)
	, IMEI2_Choose(FALSE)
	, IMEI3_Choose(FALSE)
	, IMEI4_Choose(FALSE)
	, BT_MAC_ADDR_Choose(FALSE)
	, WIFI_MAC_ADDR_Choose(FALSE)
	, IMEI_CheckValid_Choose(FALSE)
	, AutoIMEI_Choose(FALSE)
	, Software_Version_Check_Choose(FALSE)
	, WatchDog_Time(_T(""))
	, AutoIMEI_Start_Num(_T(""))
	, UseSameIMEI_Choose(FALSE)
	, AutoBTAddr_Choose(FALSE)
	, AutoWIFIAd_Start_Num(_T(""))
	, AutoWIFIAddr_Choose(FALSE)
	, TestMode_S(_T(""))
	, WIFIAddrStart(_T(""))
	, WIFIAddrEnd(_T(""))
	, IMEIStart(_T(""))
	, IMEIEnd(_T(""))
	, IMEI_DB_Choose(FALSE)
	, MAC_DB_Choose(FALSE)
	, MSN_Upload_Choose(FALSE)
	, Check_repeat_Choose(FALSE)
	, Initial_Connect_DB(FALSE)
	, UP_DB_Data(FALSE)
	, IMEI_MAC_Check_Choose(FALSE)
	, Scan_Choose(FALSE)
	, ServerIP_MAC(_T(""))
	, Repeat_AutoStart(FALSE)
	, ORACLE_Connect_Choose(FALSE)
	, ORACLE_m_server(_T(""))
	, ORACLE_m_user(_T(""))
	, ORACLE_m_pwd(_T(""))
	, BARCODE_Choose(FALSE)
	, ChangeEN_Choose(FALSE)
	, Unlock_Code_Choose(FALSE)
	, Unlock_code_S(_T(""))
	, NETCODE_Choose(FALSE)
	, NETCODE_Only_Num_Choose(FALSE)
	, JUST_UpDate_DB_Choose(FALSE)
	, UpdateIMEI_MSN_Choose(FALSE)
	, NetCodeStart(_T(""))
	, NetCodeEnd(_T(""))
	, NetCode_SwiftNumber(_T(""))
	, IMEI_Exist_Update_NC_Choose(FALSE)
	, Machine_Type_NC_CompareChoose(FALSE)
	, DestFile(_T(""))
{

}

CBT_Addr_Setting::~CBT_Addr_Setting()
{
}

void CBT_Addr_Setting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, BtAddrStart);
	DDX_Text(pDX, IDC_EDIT2, BtAddrEnd);
	DDX_Check(pDX, IDC_CHECK1, DB_CONNECT);
	DDX_Text(pDX, IDC_EDIT4, m_server);
	DDX_Text(pDX, IDC_EDIT5, m_db);
	DDX_Text(pDX, IDC_EDIT6, m_user);
	DDX_Text(pDX, IDC_EDIT7, m_pwd);
	DDX_Text(pDX, IDC_EDIT8, Order_Num_up);
	DDX_Text(pDX, IDC_EDIT9, Machine_Type);
	DDX_Text(pDX, IDC_EDIT12, Modulation_Tppe);
	DDX_Text(pDX, IDC_EDIT10, Line_Number);
	DDX_Text(pDX, IDC_EDIT13, Floor);
	DDX_Text(pDX, IDC_EDIT14, Product_Address);
	DDX_Text(pDX, IDC_EDIT11, Is_Return);
	DDX_Text(pDX, IDC_EDIT16, Software_Version);
	DDX_Text(pDX, IDC_EDIT15, Operator);
	DDX_Text(pDX, IDC_EDIT17, BT_Addr_EndShow);
	DDX_Text(pDX, IDC_EDIT18, Hardware_Version);
	DDX_Check(pDX, IDC_CHECK6, IMEI1_Choose);
	DDX_Check(pDX, IDC_CHECK7, IMEI2_Choose);
	DDX_Check(pDX, IDC_CHECK8, IMEI3_Choose);
	DDX_Check(pDX, IDC_CHECK9, IMEI4_Choose);
	DDX_Check(pDX, IDC_CHECK10, BT_MAC_ADDR_Choose);
	DDX_Check(pDX, IDC_CHECK11, WIFI_MAC_ADDR_Choose);
	DDX_Check(pDX, IDC_CHECK12, IMEI_CheckValid_Choose);
	DDX_Check(pDX, IDC_CHECK13, AutoIMEI_Choose);
	DDX_Check(pDX, IDC_CHECK14, Software_Version_Check_Choose);
	DDX_Text(pDX, IDC_EDIT3, WatchDog_Time);
	DDX_Text(pDX, IDC_EDIT19, AutoIMEI_Start_Num);
	DDX_Check(pDX, IDC_CHECK15, UseSameIMEI_Choose);
	DDX_Check(pDX, IDC_CHECK16, AutoBTAddr_Choose);
	DDX_Text(pDX, IDC_EDIT20, AutoWIFIAd_Start_Num);
	DDX_Check(pDX, IDC_CHECK17, AutoWIFIAddr_Choose);
	DDX_Control(pDX, IDC_COMBO1, TestMode_Control);
	DDX_CBString(pDX, IDC_COMBO1, TestMode_S);
	DDX_Text(pDX, IDC_EDIT22, WIFIAddrStart);
	DDX_Text(pDX, IDC_EDIT23, WIFIAddrEnd);
	DDX_Text(pDX, IDC_EDIT24, IMEIStart);
	DDX_Text(pDX, IDC_EDIT25, IMEIEnd);
	DDX_Check(pDX, IDC_CHECK18, IMEI_DB_Choose);
	DDX_Check(pDX, IDC_CHECK19, MAC_DB_Choose);
	DDX_Check(pDX, IDC_CHECK20, MSN_Upload_Choose);
	DDX_Check(pDX, IDC_CHECK21, Check_repeat_Choose);
	DDX_Check(pDX, IDC_CHECK22, Initial_Connect_DB);
	DDX_Check(pDX, IDC_CHECK23, UP_DB_Data);
	DDX_Check(pDX, IDC_CHECK24, IMEI_MAC_Check_Choose);
	DDX_Check(pDX, IDC_CHECK25, Scan_Choose);
	DDX_Control(pDX, IDC_CHECK14, Software_Version_Check_Choose_Control);
	DDX_Control(pDX, IDC_CHECK24, IMEI_MAC_Check_Choose_Control);
	DDX_Control(pDX, IDC_CHECK12, IMEI_CheckValid_Choose_Control);
	DDX_Text(pDX, IDC_EDIT27, ServerIP_MAC);
	DDX_Check(pDX, IDC_CHECK26, Repeat_AutoStart);
	DDX_Check(pDX, IDC_CHECK27, ORACLE_Connect_Choose);
	DDX_Text(pDX, IDC_EDIT28, ORACLE_m_server);
	DDX_Text(pDX, IDC_EDIT30, ORACLE_m_user);
	DDX_Text(pDX, IDC_EDIT31, ORACLE_m_pwd);
	DDX_Check(pDX, IDC_CHECK28, BARCODE_Choose);
	DDX_Check(pDX, IDC_CHECK29, ChangeEN_Choose);
	DDX_Control(pDX, IDC_TAB1, m_tab);
	//DDX_Control(pDX, IDC_CHECK31, Unlock_Code_Choose);
	DDX_Check(pDX, IDC_CHECK31, Unlock_Code_Choose);
	DDX_Control(pDX, IDC_COMBO3, Unlock_code_Control);
	DDX_CBString(pDX, IDC_COMBO3, Unlock_code_S);
	DDX_Check(pDX, IDC_CHECK32, NETCODE_Choose);
	DDX_Check(pDX, IDC_CHECK33, NETCODE_Only_Num_Choose);
	DDX_Check(pDX, IDC_CHECK34, JUST_UpDate_DB_Choose);
	DDX_Check(pDX, IDC_CHECK35, UpdateIMEI_MSN_Choose);
	DDX_Text(pDX, IDC_EDIT36, NetCodeStart);
	DDX_Text(pDX, IDC_EDIT37, NetCodeEnd);
	DDX_Text(pDX, IDC_EDIT38, NetCode_SwiftNumber);
	DDX_Check(pDX, IDC_CHECK36, IMEI_Exist_Update_NC_Choose);
	DDX_Check(pDX, IDC_CHECK38, Machine_Type_NC_CompareChoose);
	DDX_Text(pDX, IDC_EDIT_DEST_FILE, DestFile);
}


BEGIN_MESSAGE_MAP(CBT_Addr_Setting, CDialog)
	//ON_NOTIFY(RVN_ENDITEMEDIT, IDC_CUSTOM1, OnRvnEndItemEdit)   //自定义类
	//ON_NOTIFY(RVN_ITEMCLICK, IDC_CUSTOM1, OnRvnItemClick)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CBT_Addr_Setting::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDOK, &CBT_Addr_Setting::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON12, &CBT_Addr_Setting::OnBnClickedButtonstart12)
END_MESSAGE_MAP()


// CBT_Addr_Setting 消息处理程序
BOOL CBT_Addr_Setting::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateData(true);
	//CPara1 m_para1;

	m_tab.InsertItem(0,"串口AT指令设置");   //添加参数一选项卡
	m_tab.InsertItem(1,"其他设置");				//添加参数一选项卡
	GetLastError();
	m_para1.Create(IDD_CSR_SETTING,GetDlgItem(IDC_TAB1));

	CRect rs;
	m_tab.GetClientRect(rs);
	rs.top+=22;
	rs.bottom+=20;
	rs.left+=5;
	rs.right+=6;
	m_para1.MoveWindow(rs);

	m_para1.ShowWindow(TRUE);
	//设置默认的选项卡
	m_tab.SetCurSel(0);
	m_para1.LoadParaList("");//m_para1.SaveBLE_Setting("");


	//TestMode_Control.AddString("PC_MC_CALIBRATION_MODE");
	//TestMode_Control.AddString("PC_MC_RM_CALIBR_NV_WRITE_MODE");
    //RM_NORMAL_MODE;RM_CALIBRATION_MODE;RM_CALIBR_POST_MODE;RM_CALIBR_POST_NO_LCM_MODE;RM_TD_CALITRATION_MODE;RM_TD_CALIBR_POST_MODE;RM_TD_CALIBR_POST_NO_LCM_MODE;RM_CALIBR_NV_ACCESS_MODE

	//TestMode_Control.AddString("RM_NORMAL_MODE");
	//TestMode_Control.AddString("RM_CALIBRATION_MODE");
	//TestMode_Control.AddString("RM_CALIBR_POST_MODE");
	//TestMode_Control.AddString("RM_CALIBR_POST_NO_LCM_MODE");

	//TestMode_Control.AddString("RM_TD_CALITRATION_MODE");
	//TestMode_Control.AddString("RM_TD_CALIBR_POST_MODE");
	//TestMode_Control.AddString("RM_TD_CALIBR_POST_NO_LCM_MODE");

	//TestMode_Control.AddString("RM_CALIBR_NV_ACCESS_MODE");

	if(TestMode_S=="RM_NORMAL_MODE")
		TestMode_N=4;
	else if(TestMode_S=="RM_CALIBRATION_MODE")
		TestMode_N=3;
	else if(TestMode_S=="RM_CALIBR_POST_MODE")
		TestMode_N=1;
	else if(TestMode_S=="RM_CALIBR_POST_NO_LCM_MODE")
		TestMode_N=2;
	else if(TestMode_S=="RM_TD_CALITRATION_MODE")
		TestMode_N=7;
	else if(TestMode_S=="RM_TD_CALIBR_POST_MODE")
		TestMode_N=5;
	else if(TestMode_S=="RM_TD_CALIBR_POST_NO_LCM_MODE")
		TestMode_N=6;
	else if(TestMode_S=="RM_CALIBR_NV_ACCESS_MODE")
		TestMode_N=0;
	else
		TestMode_N=3;
	TestMode_Control.SetCurSel(TestMode_N);
	TestMode_Control.GetWindowTextA(TestMode_S);

	/*/自定义类
	int i = 0;
	RVITEM rvi;
	// 增加竖线和底色
	m_list_area_para.ModifyStyle(0, 
		RVS_SHOWHGRID|
		RVS_SHOWVGRID|
//		RVS_SHOWCOLORALTERNATE|
		RVS_SINGLESELECT|
		RVS_NOSORT|
		RVS_FOCUSSUBITEMS
		);


	m_list_area_para.InsertColumn(0, _T("No"), 
								RVCF_CENTER|
								RVCF_TEXT|
								RVCF_EX_AUTOWIDTH|
								RVCF_EX_FIXEDWIDTH|
								RVCF_SUBITEM_NOFOCUS
								);

	m_list_area_para.InsertColumn(1, _T("选取"),
								RVCF_CENTER|
								RVCF_TEXT |
								RVCF_EX_AUTOWIDTH);

	m_list_area_para.InsertColumn(2, _T("测试项目"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_SUBITEM_NOFOCUS,
								150);


	m_list_area_para.InsertColumn(3, _T("指标下限"),
								RVCF_CENTER|
								RVCF_TEXT
								,60);
	m_list_area_para.InsertColumn(4, _T("指标上限"),
								RVCF_CENTER|
								RVCF_TEXT
								,60);
	m_list_area_para.InsertColumn(5, _T("自定义参数"),
								RVCF_CENTER|
								RVCF_TEXT
								,120);
	LoadParaList("");
    UpdateData(FALSE);*/
	if(Unlock_code_S=="12")
		Unlock_code_N=1;
	else if(Unlock_code_S=="8")
		Unlock_code_N=2;
	else if(Unlock_code_S=="0")
		Unlock_code_N=0;
	else
		Unlock_code_N=1;

	Unlock_code_Control.SetCurSel(Unlock_code_N);
	Unlock_code_Control.GetWindowTextA(Unlock_code_S);

	CEdit *pEdit1;
	pEdit1=(CEdit*)GetDlgItem(IDC_EDIT24);
	pEdit1->SetLimitText(14);
	CEdit *pEdit2;
	pEdit2=(CEdit*)GetDlgItem(IDC_EDIT25);
	pEdit2->SetLimitText(14);

	return TRUE;
}

void CBT_Addr_Setting::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//CPara1 m_para1;

	//获得IDC_TABTEST客户区大小
	CRect rs;
	m_tab.GetClientRect(&rs);
	//调整子对话框在父窗口中的位置
	ClientToScreen(&rs);//移动窗口，控件跟着移动

	int CurSel = m_tab.GetCurSel();
    switch(CurSel)
    {
    case 0:
        m_para1.ShowWindow(true);
        break;
    case 1:
        m_para1.ShowWindow(false);
        break;
    default:
		m_para1.ShowWindow(true);
        break;
    }  
	*pResult = 0;
}

void CBT_Addr_Setting::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_para1.SaveBLE_Setting("");
	OnOK();
}

void CBT_Addr_Setting::OnBnClickedButtonstart12()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR pszDir[MAX_PATH];
	FileSelectWizard(pszDir);
	SetDlgItemText(IDC_EDIT_DEST_FILE, pszDir);
}

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    RECT rect;
    POINT dlgPT;
	int cxScreen = 0, cyScreen = 0;
    if(uMsg == BFFM_INITIALIZED)// set pop window in the center of screen
    {
		cxScreen = GetSystemMetrics(SM_CXSCREEN);
		cyScreen = GetSystemMetrics(SM_CYSCREEN);
        GetWindowRect(hwnd, &rect);
        dlgPT.x = cxScreen / 2 - (rect.right - rect.left) / 2;
		dlgPT.y = cyScreen / 2 - (rect.bottom - rect.top) / 2;
        MoveWindow(hwnd, dlgPT.x, dlgPT.y, rect.right - rect.left, rect.bottom - rect.top, true);
        //SetWindowPos(hwnd, HWND_TOP, dlgPT.x, dlgPT.y, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
    }
    
    return 1;
}

void CBT_Addr_Setting::FileSelectWizard(TCHAR *pszDir)
{
    BROWSEINFO info;
    LPCITEMIDLIST itemid;
	ITEMIDLIST *ppidl = NULL;

	SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &ppidl);

	info.hwndOwner = this->GetSafeHwnd();//NULL
	info.pidlRoot = ppidl;//NULL
	info.pszDisplayName = NULL;
    info.lpszTitle = _T("Please Select a File Path!");
    info.ulFlags = BIF_BROWSEINCLUDEURLS | BIF_SHAREABLE;
    info.lpfn = BrowseCallbackProc;//NULL
    info.lParam = 0;
    info.iImage = 0;
    itemid = SHBrowseForFolder(&info);
    
    if (NULL != itemid)
    {
        SHGetPathFromIDList(itemid, pszDir);
        if (*(strrchr(pszDir, _T('\0')) - 1) != _T('\\'))
        {
            strcat(pszDir, _T("\\"));
        }
    }
}