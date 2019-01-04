#pragma once


// CDeleteDB_Data_Confirm 对话框

class CDeleteDB_Data_Confirm : public CDialog
{
	DECLARE_DYNAMIC(CDeleteDB_Data_Confirm)

public:
	CDeleteDB_Data_Confirm(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDeleteDB_Data_Confirm();

// 对话框数据
	enum { IDD = IDD_Delete_DB_Data };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	bool PassWord_Right;
	CString m_s_pwd;
};
