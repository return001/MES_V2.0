#pragma once
#include "resource.h"

// CHistogramDlg 对话框

class CHistogramDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistogramDlg)

public:
	CHistogramDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHistogramDlg();

// 对话框数据
	enum { IDD = IDD_Histogram };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	int m_NumRect;//用于记录你要作的柱形数量
	int m_MaxValue;//记录你要作的柱形图中最高的高度
	CString *m_RectName;//用于记录柱形图的名称；
	int *m_RectValue;//用于记录柱形图的值；
	int *m_RectValue_draw;//用于备份*m_RectValue中的数据；

	void DrawPrev(int DrawTypeNum, CString *RectName,int *RectValue);
	void ClearPoint();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
};