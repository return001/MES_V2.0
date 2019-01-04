// DeleteDB_Data_Confirm.cpp : 实现文件
//

#include "stdafx.h"
#include "WriteIMEI.h"
#include "DeleteDB_Data_Confirm.h"


// CDeleteDB_Data_Confirm 对话框

IMPLEMENT_DYNAMIC(CDeleteDB_Data_Confirm, CDialog)

CDeleteDB_Data_Confirm::CDeleteDB_Data_Confirm(CWnd* pParent /*=NULL*/)
	: CDialog(CDeleteDB_Data_Confirm::IDD, pParent)
	, m_s_pwd(_T(""))
{

}

CDeleteDB_Data_Confirm::~CDeleteDB_Data_Confirm()
{
}

void CDeleteDB_Data_Confirm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_s_pwd);
}


BEGIN_MESSAGE_MAP(CDeleteDB_Data_Confirm, CDialog)
	ON_BN_CLICKED(IDOK, &CDeleteDB_Data_Confirm::OnBnClickedOk)
END_MESSAGE_MAP()


// CDeleteDB_Data_Confirm 消息处理程序

void CDeleteDB_Data_Confirm::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	PassWord_Right=false;
	UpdateData(TRUE) ;
	if (m_s_pwd.IsEmpty())
	{	
		MessageBox("口令不能为空","权限信息",MB_ICONWARNING);	 	
	}
	else if (m_s_pwd=="88888666")
	{
		PassWord_Right=true;
		EndDialog(0);
	}
	else //if(m_s_name!="desay"|| m_s_pwd!="desay_unlock")
	{	
		   // 不匹配
		MessageBox("口令不正确","提示", MB_ICONWARNING);
	}
	//OnOK();
}
