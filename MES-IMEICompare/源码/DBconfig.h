#pragma once
#include "afxwin.h"


// CDBconfig 对话框

class CDBconfig : public CDialogEx
{
	DECLARE_DYNAMIC(CDBconfig)

public:
	CDBconfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDBconfig();

	// 对话框数据
	enum { IDD = IDD_DBCONIFG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_serveripEdit;
	CString m_portEdit;
	CString m_usernameEdit;
	CString m_passwordEdit;
	CString m_firstdbnameEdit;
	CString m_firstformnameEdit;
	CString m_seconddbnameEdit;
	CString m_secondformnameEdit;
	virtual BOOL OnInitDialog();
	afx_msg void OnEnKillfocusServeripEdit();
	afx_msg void OnBnClickedOk();
};
