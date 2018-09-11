// Manager.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCP3SIMPORT.h"
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
