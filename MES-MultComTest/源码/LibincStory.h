#pragma once


// CLibincStory 对话框
#include "PictureEx.h"//显示gif
class CLibincStory : public CDialog
{
	DECLARE_DYNAMIC(CLibincStory)

public:
	CLibincStory(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLibincStory();

// 对话框数据
	enum { IDD = IDD_DIALOGSTORY };

protected:
	HICON m_hIcon_Story;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg HBRUSH OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor); 

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	CPictureEx Story_Show;
};
