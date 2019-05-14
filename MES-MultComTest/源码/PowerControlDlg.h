#pragma once
#include "Resource.h"
#include "afxwin.h"
#include "CurrentTest.h"

// CPowerControlDlg 对话框

class CPowerControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPowerControlDlg)

public:
	CPowerControlDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPowerControlDlg();

// 对话框数据
	enum { IDD = IDD_PowerControl};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_Relayport1Control;
	CComboBox m_Relayport2Control;
	CComboBox m_Relayport3Control;
	CComboBox m_Relayport4Control;
	CComboBox m_Relayport5Control;
	CComboBox m_Relayport6Control;
	CComboBox m_Relayport7Control;
	CComboBox m_Relayport8Control;
	CButton m_RelayTest1Control;
	CButton m_RelayTest2Control;
	CButton m_RelayTest3Control;
	CButton m_RelayTest4Control;
	CButton m_RelayTest5Control;
	CButton m_RelayTest6Control;
	CButton m_RelayTest7Control;
	CButton m_RelayTest8Control;
	CEdit m_RelayResult1Control;
	CEdit m_RelayResult2Control;
	CEdit m_RelayResult3Control;
	CEdit m_RelayResult4Control;
	CEdit m_RelayResult5Control;
	CEdit m_RelayResult6Control;
	CEdit m_RelayResult7Control;
	CEdit m_RelayResult8Control;
	BOOL m_RelayCheckVaule;
	BOOL m_BackuppowerCheckVaule;
	CComboBox m_RelayListControl;
	CComboBox m_BackUpPowerComListControl;
	CEdit m_BackUpPowerNumberControl;
	CEdit m_BackUpPowerTestResultControl;
	CComboBox m_PowerControlComList1Control;
	CComboBox m_PowerControlComList2Control;
	CEdit m_PowerControlNumber1Control;
	CEdit m_PowerControlNumber2Control;
	CEdit m_PowerControlResult1Control;
	CEdit m_PowerControlResult2Control;
	CButton m_PowerControlTest1Control;
	CButton m_PowerControlTest2Control;
	
	CString m_RelayListVaule;
	CString m_Relayport1Vaule;
	CString m_Relayport2Vaule;
	CString m_Relayport3Vaule;
	CString m_Relayport4Vaule;
	CString m_Relayport5Vaule;
	CString m_Relayport6Vaule;
	CString m_Relayport7Vaule;
	CString m_Relayport8Vaule;
	CString m_PowerControlComList1Vaule;
	CString m_PowerControlComList2Vaule;
	CString m_PowerControlResult1Vaule;
	CString m_PowerControlResult2Vaule;
	CString m_PowerControlTest1Vaule;
	CString m_PowerControlTest2Vaule;
	CString m_BackUpPowerComListVaule;
	int m_PowerControlNumber1Vaule;
	int m_PowerControlNumber2Vaule;
	int m_BackUpPowerNumberVaule;
	double m_VoltageVaule;
	double m_RangeVaule;


	CComboBox *m_RelayportControlArray[8];
	CButton *m_RelayTestControlArray[8];
	CEdit *m_RelayResultControlArray[8];
	CComboBox *m_PowerControlComListControlArray[2];
	CEdit *m_PowerControlNumberControlArray[2];
	CEdit *m_PowerControlTestResultControlArray[2];
	CButton *m_PowerControlTestControlArray[2];

	CString *m_RelayportVauleArray[8];
	CString *m_PowerControlComListVauleArray[2];
	CString *m_PowerControlTestVauleArray[2];
	int *m_PowerControlNumberVauleArray[2];

	afx_msg void OnBnClickedRelayCheck();
	afx_msg void OnBnClickedRelaytest1Button();
	afx_msg void OnBnClickedRelaytest2Button();
	afx_msg void OnBnClickedRelaytest3Button();
	afx_msg void OnBnClickedRelaytest4Button();
	afx_msg void OnBnClickedRelaytest5Button();
	afx_msg void OnBnClickedRelaytest6Button();
	afx_msg void OnBnClickedRelaytest7Button();
	afx_msg void OnBnClickedRelaytest8Button();
	afx_msg void OnBnClickedRelayalltest1Button();
	afx_msg void OnBnClickedRelayalltest2Button();
	afx_msg void OnBnClickedPowercontroltest1Button();
	afx_msg void OnBnClickedPowercontroltest2Button();
	afx_msg void OnEnKillfocusPowercontrolnumber1Edit();
	afx_msg void OnEnKillfocusPowercontrolnumber2Edit();
	afx_msg void OnEnKillfocusBackuppowernumberEdit();
	afx_msg void OnCbnSelchangePowercontrolcomlist1Combo();
	afx_msg void OnCbnSelchangePowercontrolcomlist2Combo();
	afx_msg void OnCbnSelchangeRelayport1Combo();
	afx_msg void OnCbnSelchangeRelayport2Combo();
	afx_msg void OnCbnSelchangeRelayport3Combo();
	afx_msg void OnCbnSelchangeRelayport4Combo();
	afx_msg void OnCbnSelchangeRelayport5Combo();
	afx_msg void OnCbnSelchangeRelayport6Combo();
	afx_msg void OnCbnSelchangeRelayport7Combo();
	afx_msg void OnCbnSelchangeRelayport8Combo();
	afx_msg void OnCbnSelchangeBackuppowercomlistCombo();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBackuppowertestButton();
	afx_msg void OnBnClickedBackuppowerCheck();
	afx_msg void OnEnKillfocusRangeEdit();
	afx_msg void OnEnKillfocusVoltageEdit();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CBrush m_brush;
	COLORREF PowerControlTestResultColor[2], PortTestResultColor[8], BackUpPowerTestResultColor, RedColor = RGB(255, 0, 0), GreenColor = RGB(0, 128, 0), WhiteColor = RGB(245, 245, 245);
	CurrentTest QCurrentTest;


	void FindCommPort(CComboBox *pComboBox);//从注册表中寻找已经注册串口号然后展示出来
	void InitPowerSetting();//初始化电流测试参数
	void InitPowerReadIniSettingFun();//写INI文件
	void InitPowerWriteIniSettingFun();//读INI文件
	void EnableWindowFun(BOOL Choose);//禁用与启用控件
	void PowerControlTestFun(int Num);//仪器初始化测试函数
	void RelayTestFun(int Num);//继电器电流测试函数
	void BackUpPowerTestFun();//备用电源测试函数
	void PowerControlColorResetFun(int Num);//电流测试结果重置
	void RelayColorResetFun(int Num);//继电器测试结果重置
	void BackUpPowerResetFun();//备用电源测试结果重置


};
