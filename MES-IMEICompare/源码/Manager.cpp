// Manager.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCP2CP.h"
#include "MFCP2CPDlg.h"
#include "Manager.h"
#include "afxdialogex.h"
#include "ADOManage.h"

// CManager 对话框

IMPLEMENT_DYNAMIC(CManager, CDialogEx)

CManager::CManager(CWnd* pParent /*=NULL*/)
: CDialogEx(CManager::IDD, pParent)
{

}

CManager::~CManager()
{
}

void CManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CManager, CDialogEx)
	ON_BN_CLICKED(IDOK, &CManager::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CManager::OnBnClickedCancel)
END_MESSAGE_MAP()


// CManager 消息处理程序

BOOL CManager::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//初始化配置模块
	SetDlgItemText(IDC_PASSWORD_STATIC, L"密码:");
	SetDlgItemText(IDOK, L"确定");
	SetDlgItemText(IDCANCEL, L"取消");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CManager::OnBnClickedOk()
{
	ADOManage adomanageCheckUser;
	CString name,password,ChcekResult;
	GetDlgItemText(IDC_USERNAME_EDIT, name);
	GetDlgItemText(IDC_MANAGERPASSWORD_EDIT, password);
	
	if (name == "" || password == "")
	{
		MessageBox(L"账号或密码不能为空！", L"提示信息", NULL);
		return;
	}
	adomanageCheckUser.ConnSQL();
	ChcekResult = adomanageCheckUser.CheckUserAuthority(name, password);
	adomanageCheckUser.CloseAll();
	if (ChcekResult == L"CheckFail")
	{
		MessageBox(L"账号或密码错误！", L"提示信息", NULL);
		return;
	}
	else if (ChcekResult.Find(g_UserTypeNeed) != -1)
	{
		g_UserNameStr = name;
		g_CheckUserType = ChcekResult; 
		CDialogEx::OnOK();
	}
	else
	{
		MessageBox(L"当前账号没有权限！", L"提示信息", NULL);
		return;
	}


	// TODO:  在此添加控件通知处理程序代码

}


void CManager::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
