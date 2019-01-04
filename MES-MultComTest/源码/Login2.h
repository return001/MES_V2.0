#pragma once


// CLogin2 对话框

class CLogin2 : public CDialog
{
	DECLARE_DYNAMIC(CLogin2)

public:
	CLogin2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogin2();

// 对话框数据
	enum { IDD = IDD_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_s_name;
	CString m_s_pwd;
	BOOL LANDING_OK;
	virtual BOOL OnInitDialog();
};
