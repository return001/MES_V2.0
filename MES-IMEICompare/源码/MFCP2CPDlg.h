
// MFCP2CPDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "AutoCombox.h"

// CMFCP2CPDlg 对话框
class CMFCP2CPDlg : public CDialogEx
{
	// 构造
public:
	CMFCP2CPDlg(CWnd* pParent = NULL);	// 标准构造函数

	// 对话框数据
	enum { IDD = IDD_MFCP2CP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	//控件变量
public:
	CStatic m_hintStatic;
	CStatic m_imei2Static;
	CStatic m_imei1Static;
	CString m_imei1Edit;
	CString m_imei2Edit;
	CButton m_couplingCheck;
	CButton m_writenumCheck;
	CButton m_downloadCheck;
	CButton m_funtestCheck;


	//字体变量与函数操作
public:
	CFont editfont;
	CFont staticfont1;
	CFont staticfont2;
	CFont checkfont;
	COLORREF clrcolor;
	void fontinit();//初始化字体



	//编辑框操作
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL IsNumber(const CString& strTest);//判断数字和字母
	BOOL IsNumber2(const CString& strTest);//只判断数字
	afx_msg void OnEnSetfocusImei2Edit();

	//其它变量
	CString strpcip, strpcname;//主机名称和ip
	int notype;//代表目前要扫的类型
	CString notypename[8];//里面放着IMEI、VIP等字符串
	CString strno1,strno2;//存放其它扫描类型的值，比如选中SN时，里面放着的就是SN号
	CString strzhidan;//存放当前选中的制单号
	int chjudgeflag;//彩盒复选框的标志
	CString strimeistart, strimeiend;


	//数据库操作函数
	void InitComboBox();
	BOOL JudgeZhidan(CString zhidan);

    //其它函数
	CString GetTime();//获取当前时间
	int GetLocalHostIPName(CString &sLocalName, CString &sIpAddress);//获取IP地址
	afx_msg void OnBnClickedOk();
	void readimei();
	BOOL judgeimeirang(CString str, CString strimeistart, CString strimeiend);

	//以下是控件按钮消息函数

    //数据库配置按钮
	afx_msg void OnBnClickedDbconfigButton();

	//更新按钮
	afx_msg void OnBnClickedUpdateordernumberButton();

	//单选框按钮
	afx_msg void OnBnClickedImeiRadio();
	afx_msg void OnBnClickedSnRadio();
	afx_msg void OnBnClickedSimRadio();
	afx_msg void OnBnClickedVipRadio();
	afx_msg void OnBnClickedIccidRadio();
	afx_msg void OnBnClickedBatRadio();
	afx_msg void OnBnClickedMacRadio();
	afx_msg void OnBnClickedEquipmentRadio();

	//自定义下拉框以及相关的消息函数
	CAutoCombox m_zhidanCombo;
	afx_msg void OnCbnSelchangeZhidanCombo();
	afx_msg void OnCbnSelendokZhidanCombo();
	afx_msg void OnCbnKillfocusZhidanCombo();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CButton m_caihejudgeCheck;
	afx_msg void OnBnClickedChjudgeCheck();
};
