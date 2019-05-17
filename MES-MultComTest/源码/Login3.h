#pragma once

//用户管理界面
#include "UserSetting.h"
// CLogin3 对话框

class CLogin3 : public CDialog
{
	DECLARE_DYNAMIC(CLogin3)

public:
	CLogin3(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogin3();

// 对话框数据
	enum { IDD = IDD_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	CString m_s_name;
	CString m_s_pwd;
public:
	CAdoInterface myado;
public:
	BOOL ConnecDB(CString m_server, CString m_db, CString m_user, CString m_pwd);
	BOOL GetUserType(CAdoInterface& myado, CString Name, CString Password, CString *Limits);
	BOOL CheckUser(CString Name, CString Password, CString* Limits);
	virtual BOOL OnInitDialog();
};
