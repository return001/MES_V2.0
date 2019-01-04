// Login2.cpp : 实现文件
//

#include "stdafx.h"
#include "WriteIMEI.h"
#include "Login2.h"


// CLogin2 对话框

IMPLEMENT_DYNAMIC(CLogin2, CDialog)

CLogin2::CLogin2(CWnd* pParent /*=NULL*/)
	: CDialog(CLogin2::IDD, pParent)
{

}

CLogin2::~CLogin2()
{
}

void CLogin2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_s_name);
	DDX_Text(pDX, IDC_EDIT2, m_s_pwd);
}


BEGIN_MESSAGE_MAP(CLogin2, CDialog)
	ON_BN_CLICKED(IDOK, &CLogin2::OnBnClickedOk)
END_MESSAGE_MAP()


// CLogin2 消息处理程序


void CLogin2::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;

	if (m_s_name.IsEmpty() || m_s_pwd.IsEmpty())
	{	
		LANDING_OK=FALSE;
		MessageBox("用户名或密码不能为空","用户登录信息",MB_ICONWARNING);	 	
	}
	else if (m_s_name=="xx"&& m_s_pwd=="12")
	{
		   LANDING_OK=TRUE;		   
		   UpdateData(true);      //将控件的值赋给变量
		   EndDialog(0);
		   
	}
	else //if(m_s_name!="desay"|| m_s_pwd!="desay_unlock")
	{	
		   // 不匹配
		LANDING_OK=FALSE;
		MessageBox("用户名或密码不正确","提示", MB_ICONWARNING);
	}
	//OnOK();
}


BOOL CLogin2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//三合一实现自动登陆,自动解锁
	if (g_ADCTFlag == 1)
	{
		SetDlgItemText(IDC_EDIT1, "xx");
		SetDlgItemText(IDC_EDIT2, "12");
		OnBnClickedOk();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
