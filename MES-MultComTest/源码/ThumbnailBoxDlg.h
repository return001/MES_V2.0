
// ThumbnailBoxDlg.h : 头文件
//

#pragma once
#include "thumbnailbox.h"
#include "resource.h"		

// CThumbnailBoxDlg 对话框
class CThumbnailBoxDlg : public CDialog
{
// 构造
public:
	CThumbnailBoxDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_THUMBNAILBOX_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	afx_msg void OnTimer(UINT nIDEvent);
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString UnionPicPath;
	CString PicPath0;
	CString PicPath1;
	int ShowPicTime;
	int ShowHandleHun; 
};
