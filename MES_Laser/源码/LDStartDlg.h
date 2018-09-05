#pragma once


// CLDStartDlg 对话框

class CLDStartDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLDStartDlg)

public:
	CLDStartDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLDStartDlg();

// 对话框数据
	enum { IDD = IDD_LDSTART_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
