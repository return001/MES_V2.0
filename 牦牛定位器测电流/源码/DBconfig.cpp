// DBconfig.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCP4ECurrentTool.h"
#include "MFCP4ECurrentToolDlg.h"
#include "DBconfig.h"
#include "afxdialogex.h"
#include "ADOManage.h"

IMPLEMENT_DYNAMIC(CDBconfig, CDialogEx)

CDBconfig::CDBconfig(CWnd* pParent /*=NULL*/)
: CDialogEx(CDBconfig::IDD, pParent)
, m_serveripEdit(_T(""))
, m_portEdit(_T(""))
, m_usernameEdit(_T(""))
, m_passwordEdit(_T(""))
, m_seconddbnameEdit(_T(""))
, m_firstdbnameEdit(_T(""))
, m_secondformnameEdit(_T(""))
, m_firstformnameEdit(_T(""))
{

}

CDBconfig::~CDBconfig()
{
}

void CDBconfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SERVERIP_EDIT, m_serveripEdit);
	DDX_Text(pDX, IDC_PORT_EDIT, m_portEdit);
	DDX_Text(pDX, IDC_USERNAME_EDIT, m_usernameEdit);
	DDX_Text(pDX, IDC_PASSWORD_EDIT, m_passwordEdit);
	DDX_Text(pDX, IDC_FIRSTDBNAME_EDIT, m_firstdbnameEdit);
	DDX_Text(pDX, IDC_FIRSTFORMNAME_EDIT, m_firstformnameEdit);
	DDX_Text(pDX, IDC_SECONDDBNAME_EDIT, m_seconddbnameEdit);
	DDX_Text(pDX, IDC_SECONDFORMNAME_EDIT, m_secondformnameEdit);

}


BEGIN_MESSAGE_MAP(CDBconfig, CDialogEx)
	ON_EN_KILLFOCUS(IDC_SERVERIP_EDIT, &CDBconfig::OnEnKillfocusServeripEdit)
	ON_BN_CLICKED(IDOK, &CDBconfig::OnBnClickedOk)
END_MESSAGE_MAP()


// CDBconfig 消息处理程序


BOOL CDBconfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CFileFind finder;
	CString str;

	BOOL ifFind = finder.FindFile(_T(".\\SystemInfo.ini"));
	if (ifFind)
	{
		GetPrivateProfileString(_T("DatabaseInfo"), _T("DataSource"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
		m_serveripEdit = str;
		str.ReleaseBuffer();
		GetPrivateProfileString(_T("DatabaseInfo"), _T("Port"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
		m_portEdit = str;
		str.ReleaseBuffer();
		GetPrivateProfileString(_T("DatabaseInfo"), _T("UserName"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
		m_usernameEdit = str;
		str.ReleaseBuffer();
		GetPrivateProfileString(_T("DatabaseInfo"), _T("Password"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
		m_passwordEdit = str;
		str.ReleaseBuffer();
		GetPrivateProfileString(_T("DatabaseInfo"), _T("Firstdbname"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
		m_firstdbnameEdit = str;
		str.ReleaseBuffer();
		GetPrivateProfileString(_T("DatabaseInfo"), _T("Firstformname"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
		m_firstformnameEdit = str;
		str.ReleaseBuffer();
		GetPrivateProfileString(_T("DatabaseInfo"), _T("Seconddbname"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
		m_seconddbnameEdit = str;
		str.ReleaseBuffer();
		GetPrivateProfileString(_T("DatabaseInfo"), _T("Secondformname"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
		m_secondformnameEdit = str;
		UpdateData(FALSE);
		str.ReleaseBuffer();
	}
	//初始化配置模块
	SetDlgItemText(IDC_SERVERIP_STATIC, L"服务器IP");
	SetDlgItemText(IDC_SERVERPORT_STATIC, L"端口");
	SetDlgItemText(IDC_SERVERACCOUNT_STATIC, L"账号");
	SetDlgItemText(IDC_SERVERPASSWORD_STATIC, L"密码");
	SetDlgItemText(IDC_DATABASE1_STATIC, L"数据库1名称");
	SetDlgItemText(IDC_DATABASE2_STATIC, L"数据库2名称");
	SetDlgItemText(IDC_DATASHEET1_STATIC, L"数据表1名称");
	SetDlgItemText(IDC_DATASHEET2_STATIC, L"数据表2名称");
	SetDlgItemText(IDOK, L"测试连接并保存");
	SetDlgItemText(IDCANCEL, L"取消");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

//判断用户输入的IP对不对，这是系统自带的消息函数，当编辑框失去焦点的时候就会执行
void CDBconfig::OnEnKillfocusServeripEdit()
{
	// TODO:  在此添加控件通知处理程序代码
	CString IPaddress;
	int a, b, c, d, count1;
	GetDlgItemText(IDC_SERVERIP_EDIT, IPaddress);

	count1 = sscanf_s(CStringA(IPaddress), "%d.%d.%d.%d", &a, &b, &c, &d);

	if (count1 == 4 && a >= 0 && a <= 255 && b >= 0 && b <= 255 && c >= 0 && c <= 255 && d >= 0 && d <= 255)
	{
		return;
	}
	else
	{
		MessageBox(_T("IP输入错误，请根据192.168.1.1这种(x.x.x.x)格式输入!且确保每个值在0~255之间！"), _T("错误信息"), NULL);
		SetDlgItemText(IDC_SERVERIP_EDIT, m_serveripEdit);
	}
}

//测试并连接按钮
void CDBconfig::OnBnClickedOk()
{
	//写INI
	CString str;
	ADOManage adoManage;
	CFileFind finder;
	BOOL ifFind = finder.FindFile(_T(".\\SystemInfo.ini"));
	GetDlgItemText(IDC_SERVERIP_EDIT, m_serveripEdit);
	adoManage.m_DataSource = m_serveripEdit;  //服务器IP
	GetDlgItemText(IDC_PORT_EDIT, m_portEdit);
	adoManage.m_Port = m_portEdit;        //服务器端口
	GetDlgItemText(IDC_USERNAME_EDIT, m_usernameEdit);
	adoManage.m_UserName = m_usernameEdit;    //用户名
	GetDlgItemText(IDC_PASSWORD_EDIT, m_passwordEdit);
	adoManage.m_Password = m_passwordEdit;    //密码
	GetDlgItemText(IDC_FIRSTDBNAME_EDIT, m_firstdbnameEdit);
	adoManage.m_Firstdbname = m_firstdbnameEdit;    //第一个数据库名称
	GetDlgItemText(IDC_FIRSTFORMNAME_EDIT, m_firstformnameEdit);
	adoManage.m_Firstformname = m_firstformnameEdit;  //第一张数据表名称
	GetDlgItemText(IDC_SECONDDBNAME_EDIT, m_seconddbnameEdit);
	adoManage.m_Seconddbname = m_seconddbnameEdit;  //第二个数据库名称
	GetDlgItemText(IDC_SECONDFORMNAME_EDIT, m_secondformnameEdit);
	adoManage.m_Secondformname = m_secondformnameEdit;//第二张数据表名称

	if (!ifFind)
	{
		GetDlgItemText(IDC_SERVERIP_EDIT, str);
		WritePrivateProfileString(_T("DatabaseInfo"), _T("DataSource"), str, _T(".\\SystemInfo.ini"));
		GetDlgItemText(IDC_PORT_EDIT, str);
		WritePrivateProfileString(_T("DatabaseInfo"), _T("Port"), str, _T(".\\SystemInfo.ini"));
		GetDlgItemText(IDC_USERNAME_EDIT, str);
		WritePrivateProfileString(_T("DatabaseInfo"), _T("UserName"), str, _T(".\\SystemInfo.ini"));
		GetDlgItemText(IDC_PASSWORD_EDIT, str);
		WritePrivateProfileString(_T("DatabaseInfo"), _T("Password"), str, _T(".\\SystemInfo.ini"));
		GetDlgItemText(IDC_FIRSTDBNAME_EDIT, str);
		WritePrivateProfileString(_T("DatabaseInfo"), _T("Firstdbname"), str, _T(".\\SystemInfo.ini"));
		GetDlgItemText(IDC_FIRSTFORMNAME_EDIT, str);
		WritePrivateProfileString(_T("DatabaseInfo"), _T("Firstformname"), str, _T(".\\SystemInfo.ini"));
		GetDlgItemText(IDC_SECONDDBNAME_EDIT, str);
		WritePrivateProfileString(_T("DatabaseInfo"), _T("Seconddbname"), str, _T(".\\SystemInfo.ini"));
		GetDlgItemText(IDC_SECONDFORMNAME_EDIT, str);
		WritePrivateProfileString(_T("DatabaseInfo"), _T("Secondformname"), str, _T(".\\SystemInfo.ini"));
	}

	if (adoManage.ConntestdbSQL1())
	{
		if (adoManage.ConntestdbSQL2())
		{
			GetDlgItemText(IDC_SERVERIP_EDIT, str);
			WritePrivateProfileString(_T("DatabaseInfo"), _T("DataSource"), str, _T(".\\SystemInfo.ini"));
			GetDlgItemText(IDC_PORT_EDIT, str);
			WritePrivateProfileString(_T("DatabaseInfo"), _T("Port"), str, _T(".\\SystemInfo.ini"));
			GetDlgItemText(IDC_USERNAME_EDIT, str);
			WritePrivateProfileString(_T("DatabaseInfo"), _T("UserName"), str, _T(".\\SystemInfo.ini"));
			GetDlgItemText(IDC_PASSWORD_EDIT, str);
			WritePrivateProfileString(_T("DatabaseInfo"), _T("Password"), str, _T(".\\SystemInfo.ini"));
			GetDlgItemText(IDC_FIRSTDBNAME_EDIT, str);
			WritePrivateProfileString(_T("DatabaseInfo"), _T("Firstdbname"), str, _T(".\\SystemInfo.ini"));
			GetDlgItemText(IDC_FIRSTFORMNAME_EDIT, str);
			WritePrivateProfileString(_T("DatabaseInfo"), _T("Firstformname"), str, _T(".\\SystemInfo.ini"));
			GetDlgItemText(IDC_SECONDDBNAME_EDIT, str);
			WritePrivateProfileString(_T("DatabaseInfo"), _T("Seconddbname"), str, _T(".\\SystemInfo.ini"));
			GetDlgItemText(IDC_SECONDFORMNAME_EDIT, str);
			WritePrivateProfileString(_T("DatabaseInfo"), _T("Secondformname"), str, _T(".\\SystemInfo.ini"));
			adoManage.CloseDB();
			CDialogEx::OnCancel();
		}
	}
}

