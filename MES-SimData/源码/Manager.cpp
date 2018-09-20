// Manager.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCP3SIMPORT.h"
#include "MFCP3SIMPORTDlg.h"
#include "Manager.h"
#include "afxdialogex.h"


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

	//设置为中文
	if (!LanguageFlag)
	{
		//初始化配置模块
		SetDlgItemText(IDC_PASSWORD_STATIC, L"请输入密码:");
		SetDlgItemText(IDOK, L"确定");
		SetDlgItemText(IDCANCEL, L"取消");
	}
	//设置为英文
	else if (LanguageFlag)
	{
		SetDlgItemText(IDC_PASSWORD_STATIC, L"Password:");
		SetDlgItemText(IDOK, L"OK");
		SetDlgItemText(IDCANCEL, L"CANCEL");
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CManager::OnBnClickedOk()
{
	CString password;
	GetDlgItemText(IDC_MANAGERPASSWORD_EDIT, password);

	if (password != L"jimi")
	{
		MessageBox(L"密码错误！",L"提示信息",NULL);
		return;
	}

	CDialogEx::OnOK();
	// TODO:  在此添加控件通知处理程序代码

}


void CManager::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


