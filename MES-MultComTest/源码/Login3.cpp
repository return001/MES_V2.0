// CLogin3.cpp : 实现文件
//

#include "stdafx.h"
#include "WriteIMEI.h"
#include "Login3.h"


// CLogin3 对话框

IMPLEMENT_DYNAMIC(CLogin3, CDialog)

CLogin3::CLogin3(CWnd* pParent /*=NULL*/)
	: CDialog(CLogin3::IDD, pParent)
{

}

CLogin3::~CLogin3()
{
}

void CLogin3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_s_name);
	DDX_Text(pDX, IDC_EDIT2, m_s_pwd);
}


BEGIN_MESSAGE_MAP(CLogin3, CDialog)
	ON_BN_CLICKED(IDOK, &CLogin3::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CLogin3::OnBnClickedButton1)
END_MESSAGE_MAP()


// CLogin3 消息处理程序

void CLogin3::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE) ;

	CString LimitsTemp="";

	if (m_s_name.IsEmpty() || m_s_pwd.IsEmpty())
	{	
		MessageBox("用户名或密码不能为空","用户登录信息",MB_ICONWARNING);	 	
	}
	else if (ConnecDB(g_BackDoorIP, g_BackDoorDatabase, g_BackDoorUser, g_BackDoorPassword) == TRUE)
	//else if (ConnecDB("192.168.0.240", "GPSTest", "sa", "sa123abc") == TRUE)
	{
		if((m_s_name!="")&& (CheckUser(m_s_name, m_s_pwd, &LimitsTemp) == TRUE))//账号1不为空
		{
			if (LimitsTemp == "")
			{
				AfxMessageBox("账号/密码不正确！");
				return;
			}
			else
			{
				UserNameDB = m_s_name;
				OnOK();
			}
		}
	}
	else
	{
		if (g_ADCTFlag != 1)
		{
			// 不匹配
			MessageBox("用户名或密码不正确", "提示", MB_ICONWARNING);
		}
	}
}


BOOL CLogin3::ConnecDB(CString m_server, CString m_db, CString m_user, CString m_pwd)
{
	if (1)//数据库地址不为空，就上传数据
	{
		CString Conn = "";
		CString DB_FAIL = "";
		Conn.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s", m_server, m_db, m_user, m_pwd);
		if (myado.m_pCon == NULL)
		{
			CoInitialize(NULL);
			DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);
		}
		if (myado.m_pCon->State == 0)  //1表示已经打开,0表示关闭，数据库意外断开，重连
		{
			if (g_ADCTFlag == 1)
			{
				HWND ADCTHwnd = NULL;
				ADCTHwnd = ::FindWindow("ADCT", "AutoDownloadATETest");
				MessageBox(_T("连接数据库失败，请检查数据库，当前程序立即关闭！"), _T("Warning"), MB_SYSTEMMODAL | MB_ICONEXCLAMATION | MB_OK);
				::PostMessage(ADCTHwnd, WM_USER + 1001, 3, 99);
				OnCancel();
				return FALSE;
			}

			if (MessageBox("数据库处于断开状态，重连？", "温馨确认", MB_OK | MB_OKCANCEL) == IDOK)
				DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);//数据库意外断开，重连
			else
				return FALSE;
		}

		if (DB_FAIL == "FAIL")
		{
			AfxMessageBox("连接数据库失败，请检查网络.......");
			return FALSE;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//MD5 mb;
		//CString CpuOrderTemp = "AT01a#^GT_CM=TEST";
		//mb.update(CpuOrderTemp, strlen(CpuOrderTemp));
		//CString CpuOrderMD5 = (mb.toString()).c_str();

	}
	return TRUE;
}
BOOL CLogin3::GetUserType(CAdoInterface& myado, CString Name, CString Password, CString *Limits)
{
	_variant_t var;
	CString FieldName, FieldValue;
	try
	{
		myado.OpenSheet("select * from LUserAccount WHERE Name='" + Name + "' AND Password='" + Password  + "'");
		//var= myado.m_pRec->GetCollect(_variant_t((long)5));
		FieldName.Format("UserType");
		var = myado.m_pRec->GetCollect((LPCTSTR)FieldName);
		if (var.vt != VT_NULL)
		{
			FieldValue.Format("%s", (LPCSTR)(_bstr_t)var);
			*Limits = FieldValue;
		}
		else
		{
			*Limits = "";
		}

	}
	catch (_com_error e)
	{
		*Limits = "";
		myado.CloseSheet();
		return FALSE;
	}
	myado.CloseSheet();
	return TRUE;
}

BOOL CLogin3::CheckUser(CString Name, CString Password, CString* Limits)
{
	if (1)//数据库地址不为空，就上传数据
	{

		BOOL UP_Barcode = TRUE;
		BOOL Barcode_Check_UP = TRUE;

		myado.OpenSheet("select * from LUserAccount WHERE Name='" + Name + "' AND Password='" + Password + "'");
		BOOL Barcode_Check = myado.Find("Name='" + Name + "'");
		myado.CloseSheet();
		if (Barcode_Check == TRUE)//存在账号
		{
			if (GetUserType(myado, Name, Password, Limits) == TRUE)
			{

			}
			else
			{
				return FALSE;
			}
				
		}
	}
	return TRUE;
}

void CLogin3::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);

	CString LimitsTemp = "";
	if (ConnecDB(g_BackDoorIP, g_BackDoorDatabase, g_BackDoorUser, g_BackDoorPassword) == TRUE)
		//if (ConnecDB("192.168.0.240", "GPSTest", "sa", "sa123abc") != TRUE)
	{
		if (CheckUser(m_s_name, m_s_pwd, &LimitsTemp) == TRUE)
		{
			if (LimitsTemp.Find("&00") == -1)
			{
				AfxMessageBox("该账号没有管理权限");
				return;
			}
		}
		else
		{
			return;
		}
	}
	else
	{
		return;
	}
	CUserSetting UserS;
	UserS.DoModal();
}

BOOL CLogin3::OnInitDialog()
{
	CDialog::OnInitDialog();

    //后门开启。如果存在Systembackdoor这个文件，ip配置就按配置文件来，否则就默认工厂IP
	CFileFind finder;
	CString str;
	int ValueInt;

	BOOL ifFind = finder.FindFile(_T(".\\SystemSetting.ini"));
	if (ifFind)
	{
		ValueInt = GetPrivateProfileInt(_T("SimpleSetting"), _T("IsHideFlag"), 0, _T(".\\SystemSetting.ini"));
		g_IsHideFlag = ValueInt;
		ValueInt = GetPrivateProfileInt(_T("SimpleSetting"), _T("ADCTFlag"), 0, _T(".\\SystemSetting.ini"));
		g_ADCTFlag = ValueInt;
	}

	ifFind = finder.FindFile(_T(".\\Systembackdoor.ini"));
	if (ifFind)
	{

		GetPrivateProfileString(_T("DatabaseInfo"), _T("IP"), _T(""), str.GetBuffer(50), 50, _T(".\\Systembackdoor.ini"));
		g_BackDoorIP = str;
		str.ReleaseBuffer();
		GetPrivateProfileString(_T("DatabaseInfo"), _T("Database"), _T(""), str.GetBuffer(50), 50, _T(".\\Systembackdoor.ini"));
		g_BackDoorDatabase = str;
		str.ReleaseBuffer();
		GetPrivateProfileString(_T("DatabaseInfo"), _T("User"), _T(""), str.GetBuffer(50), 50, _T(".\\Systembackdoor.ini"));
		g_BackDoorUser = str;
		str.ReleaseBuffer();
		GetPrivateProfileString(_T("DatabaseInfo"), _T("Password"), _T(""), str.GetBuffer(50), 50, _T(".\\Systembackdoor.ini"));
		g_BackDoorPassword = str;
		str.ReleaseBuffer();
		GetPrivateProfileString(_T("DatabaseInfo"), _T("Socket"), _T(""), str.GetBuffer(50), 50, _T(".\\Systembackdoor.ini"));
		g_BackDoorSocket = str;
		str.ReleaseBuffer();
	}

	// TODO:  在此添加额外的初始化
	//三合一实现自动登陆
	if (g_ADCTFlag == 1)
	{
		SetDlgItemText(IDC_EDIT1, "1");
		SetDlgItemText(IDC_EDIT2, "1");
		OnBnClickedOk();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
