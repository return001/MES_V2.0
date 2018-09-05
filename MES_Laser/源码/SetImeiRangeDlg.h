#pragma once
#include "afxwin.h"


// CSetImeiRangeDlg 对话框

class CSetImeiRangeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetImeiRangeDlg)

public:
	CSetImeiRangeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetImeiRangeDlg();

// 对话框数据
	enum { IDD = IDD_IMEIRANGE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	int m_imeirangestartEdit;
	int m_imeirangeendEdit;

//自定义变量
public:
	CString m_imeirangestart;
	CString m_imeirangeend;
	int m_imeirangestartNo;
	int m_imeirangeendNo;

	afx_msg void OnEnKillfocusImeirangestartEdit();
	afx_msg void OnEnKillfocusImeirangeendEdit();
};
