// PowerControlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PowerControlDlg.h"
#include "afxdialogex.h"

// CPowerControlDlg 对话框

IMPLEMENT_DYNAMIC(CPowerControlDlg, CDialogEx)

CPowerControlDlg::CPowerControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPowerControlDlg::IDD, pParent)
	, m_RelayCheckVaule(TRUE)
	, m_PowerControlNumber1Vaule(0)
	, m_PowerControlNumber2Vaule(0)
	, m_BackUpPowerNumberVaule(0)
	, m_PowerControlComList1Vaule(_T(""))
	, m_RelayListVaule(_T(""))
	, m_Relayport1Vaule(_T(""))
	, m_VoltageVaule(0)
	, m_RangeVaule(0)
	, m_BackuppowerCheckVaule(FALSE)
{

}

CPowerControlDlg::~CPowerControlDlg()
{
}

void CPowerControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RELAYPORT1_COMBO, m_Relayport1Control);
	DDX_Control(pDX, IDC_RELAYPORT2_COMBO, m_Relayport2Control);
	DDX_Control(pDX, IDC_RELAYPORT3_COMBO, m_Relayport3Control);
	DDX_Control(pDX, IDC_RELAYPORT4_COMBO, m_Relayport4Control);
	DDX_Control(pDX, IDC_RELAYPORT5_COMBO, m_Relayport5Control);
	DDX_Control(pDX, IDC_RELAYPORT6_COMBO, m_Relayport6Control);
	DDX_Control(pDX, IDC_RELAYPORT7_COMBO, m_Relayport7Control);
	DDX_Control(pDX, IDC_RELAYPORT8_COMBO, m_Relayport8Control);
	DDX_Control(pDX, IDC_RELAYTEST1_BUTTON, m_RelayTest1Control);
	DDX_Control(pDX, IDC_RELAYTEST2_BUTTON, m_RelayTest2Control);
	DDX_Control(pDX, IDC_RELAYTEST3_BUTTON, m_RelayTest3Control);
	DDX_Control(pDX, IDC_RELAYTEST4_BUTTON, m_RelayTest4Control);
	DDX_Control(pDX, IDC_RELAYTEST5_BUTTON, m_RelayTest5Control);
	DDX_Control(pDX, IDC_RELAYTEST6_BUTTON, m_RelayTest6Control);
	DDX_Control(pDX, IDC_RELAYTEST7_BUTTON, m_RelayTest7Control);
	DDX_Control(pDX, IDC_RELAYTEST8_BUTTON, m_RelayTest8Control);
	DDX_Control(pDX, IDC_RELAYRESULT1_EDIT, m_RelayResult1Control);
	DDX_Control(pDX, IDC_RELAYRESULT2_EDIT, m_RelayResult2Control);
	DDX_Control(pDX, IDC_RELAYRESULT3_EDIT, m_RelayResult3Control);
	DDX_Control(pDX, IDC_RELAYRESULT4_EDIT, m_RelayResult4Control);
	DDX_Control(pDX, IDC_RELAYRESULT5_EDIT, m_RelayResult5Control);
	DDX_Control(pDX, IDC_RELAYRESULT6_EDIT, m_RelayResult6Control);
	DDX_Control(pDX, IDC_RELAYRESULT7_EDIT, m_RelayResult7Control);
	DDX_Control(pDX, IDC_RELAYRESULT8_EDIT, m_RelayResult8Control);
	DDX_Check(pDX, IDC_RELAY_CHECK, m_RelayCheckVaule);
	DDX_Control(pDX, IDC_RELAYLIST_COMBO, m_RelayListControl);
	DDX_Control(pDX, IDC_POWERCONTROLCOMLIST1_COMBO, m_PowerControlComList1Control);
	DDX_Control(pDX, IDC_POWERCONTROLCOMLIST2_COMBO, m_PowerControlComList2Control);
	DDX_Control(pDX, IDC_POWERCONTROLNUMBER1_EDIT, m_PowerControlNumber1Control);
	DDX_Control(pDX, IDC_POWERCONTROLNUMBER2_EDIT, m_PowerControlNumber2Control);
	DDX_Control(pDX, IDC_POWERCONTROLRESULT1_EDIT, m_PowerControlResult1Control);
	DDX_Control(pDX, IDC_POWERCONTROLRESULT2_EDIT, m_PowerControlResult2Control);
	DDX_Control(pDX, IDC_POWERCONTROLTEST1_BUTTON, m_PowerControlTest1Control);
	DDX_Control(pDX, IDC_POWERCONTROLTEST2_BUTTON, m_PowerControlTest2Control);
	DDX_Control(pDX, IDC_BACKUPPOWERCOMLIST_COMBO, m_BackUpPowerComListControl);
	DDX_Control(pDX, IDC_BACKUPPOWERNUMBER_EDIT, m_BackUpPowerNumberControl);
	DDX_Text(pDX, IDC_POWERCONTROLNUMBER1_EDIT, m_PowerControlNumber1Vaule);
	DDV_MinMaxInt(pDX, m_PowerControlNumber1Vaule, 0, 99);
	DDX_Text(pDX, IDC_POWERCONTROLNUMBER2_EDIT, m_PowerControlNumber2Vaule);
	DDV_MinMaxInt(pDX, m_PowerControlNumber2Vaule, 0, 99);
	DDX_Text(pDX, IDC_BACKUPPOWERNUMBER_EDIT, m_BackUpPowerNumberVaule);
	DDV_MinMaxInt(pDX, m_BackUpPowerNumberVaule, 0, 99);
	DDX_CBString(pDX, IDC_POWERCONTROLCOMLIST1_COMBO, m_PowerControlComList1Vaule);
	DDX_CBString(pDX, IDC_POWERCONTROLCOMLIST2_COMBO, m_PowerControlComList2Vaule);
	DDX_CBString(pDX, IDC_POWERCONTROLRESULT1_EDIT, m_PowerControlResult1Vaule);
	DDX_CBString(pDX, IDC_POWERCONTROLRESULT2_EDIT, m_PowerControlResult2Vaule);
	DDX_CBString(pDX, IDC_POWERCONTROLTEST1_BUTTON, m_PowerControlTest1Vaule);
	DDX_CBString(pDX, IDC_POWERCONTROLTEST2_BUTTON, m_PowerControlTest2Vaule);
	DDX_CBString(pDX, IDC_BACKUPPOWERCOMLIST_COMBO, m_BackUpPowerComListVaule);
	DDX_CBString(pDX, IDC_RELAYLIST_COMBO, m_RelayListVaule);
	DDX_CBString(pDX, IDC_RELAYPORT1_COMBO, m_Relayport1Vaule);
	DDX_CBString(pDX, IDC_RELAYPORT2_COMBO, m_Relayport2Vaule);
	DDX_CBString(pDX, IDC_RELAYPORT3_COMBO, m_Relayport3Vaule);
	DDX_CBString(pDX, IDC_RELAYPORT4_COMBO, m_Relayport4Vaule);
	DDX_CBString(pDX, IDC_RELAYPORT5_COMBO, m_Relayport5Vaule);
	DDX_CBString(pDX, IDC_RELAYPORT6_COMBO, m_Relayport6Vaule);
	DDX_CBString(pDX, IDC_RELAYPORT7_COMBO, m_Relayport7Vaule);
	DDX_CBString(pDX, IDC_RELAYPORT8_COMBO, m_Relayport8Vaule);
	DDX_Control(pDX, IDC_BACKUPPOWERTESTRESULT_EDIT, m_BackUpPowerTestResultControl);
	DDX_Text(pDX, IDC_VOLTAGE_EDIT, m_VoltageVaule);
	DDV_MinMaxDouble(pDX, m_VoltageVaule, 0, 20.0);
	DDX_Text(pDX, IDC_RANGE_EDIT, m_RangeVaule);
	DDV_MinMaxDouble(pDX, m_RangeVaule, 0, 10);
	DDX_Check(pDX, IDC_BACKUPPOWER_CHECK, m_BackuppowerCheckVaule);
}


BEGIN_MESSAGE_MAP(CPowerControlDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RELAY_CHECK, &CPowerControlDlg::OnBnClickedRelayCheck)
	ON_BN_CLICKED(IDC_RELAYTEST1_BUTTON, &CPowerControlDlg::OnBnClickedRelaytest1Button)
	ON_BN_CLICKED(IDC_RELAYTEST2_BUTTON, &CPowerControlDlg::OnBnClickedRelaytest2Button)
	ON_BN_CLICKED(IDC_RELAYTEST3_BUTTON, &CPowerControlDlg::OnBnClickedRelaytest3Button)
	ON_BN_CLICKED(IDC_RELAYTEST4_BUTTON, &CPowerControlDlg::OnBnClickedRelaytest4Button)
	ON_BN_CLICKED(IDC_RELAYTEST5_BUTTON, &CPowerControlDlg::OnBnClickedRelaytest5Button)
	ON_BN_CLICKED(IDC_RELAYTEST6_BUTTON, &CPowerControlDlg::OnBnClickedRelaytest6Button)
	ON_BN_CLICKED(IDC_RELAYTEST7_BUTTON, &CPowerControlDlg::OnBnClickedRelaytest7Button)
	ON_BN_CLICKED(IDC_RELAYTEST8_BUTTON, &CPowerControlDlg::OnBnClickedRelaytest8Button)
	ON_BN_CLICKED(IDC_RELAYALLTEST1_BUTTON, &CPowerControlDlg::OnBnClickedRelayalltest1Button)
	ON_BN_CLICKED(IDC_RELAYALLTEST2_BUTTON, &CPowerControlDlg::OnBnClickedRelayalltest2Button)
	ON_BN_CLICKED(IDC_POWERCONTROLTEST1_BUTTON, &CPowerControlDlg::OnBnClickedPowercontroltest1Button)
	ON_BN_CLICKED(IDC_POWERCONTROLTEST2_BUTTON, &CPowerControlDlg::OnBnClickedPowercontroltest2Button)
	ON_BN_CLICKED(IDOK, &CPowerControlDlg::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_POWERCONTROLNUMBER1_EDIT, &CPowerControlDlg::OnEnKillfocusPowercontrolnumber1Edit)
	ON_EN_KILLFOCUS(IDC_POWERCONTROLNUMBER2_EDIT, &CPowerControlDlg::OnEnKillfocusPowercontrolnumber2Edit)
	ON_EN_KILLFOCUS(IDC_BACKUPPOWERNUMBER_EDIT, &CPowerControlDlg::OnEnKillfocusBackuppowernumberEdit)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_POWERCONTROLCOMLIST1_COMBO, &CPowerControlDlg::OnCbnSelchangePowercontrolcomlist1Combo)
	ON_CBN_SELCHANGE(IDC_POWERCONTROLCOMLIST2_COMBO, &CPowerControlDlg::OnCbnSelchangePowercontrolcomlist2Combo)
	ON_CBN_SELCHANGE(IDC_RELAYPORT1_COMBO, &CPowerControlDlg::OnCbnSelchangeRelayport1Combo)
	ON_CBN_SELCHANGE(IDC_RELAYPORT2_COMBO, &CPowerControlDlg::OnCbnSelchangeRelayport2Combo)
	ON_CBN_SELCHANGE(IDC_RELAYPORT3_COMBO, &CPowerControlDlg::OnCbnSelchangeRelayport3Combo)
	ON_CBN_SELCHANGE(IDC_RELAYPORT4_COMBO, &CPowerControlDlg::OnCbnSelchangeRelayport4Combo)
	ON_CBN_SELCHANGE(IDC_RELAYPORT5_COMBO, &CPowerControlDlg::OnCbnSelchangeRelayport5Combo)
	ON_CBN_SELCHANGE(IDC_RELAYPORT6_COMBO, &CPowerControlDlg::OnCbnSelchangeRelayport6Combo)
	ON_CBN_SELCHANGE(IDC_RELAYPORT7_COMBO, &CPowerControlDlg::OnCbnSelchangeRelayport7Combo)
	ON_CBN_SELCHANGE(IDC_RELAYPORT8_COMBO, &CPowerControlDlg::OnCbnSelchangeRelayport8Combo)
	ON_BN_CLICKED(IDC_BACKUPPOWERTEST_BUTTON, &CPowerControlDlg::OnBnClickedBackuppowertestButton)
	ON_CBN_SELCHANGE(IDC_BACKUPPOWERCOMLIST_COMBO, &CPowerControlDlg::OnCbnSelchangeBackuppowercomlistCombo)
	ON_EN_KILLFOCUS(IDC_VOLTAGE_EDIT, &CPowerControlDlg::OnEnKillfocusVoltageEdit)
	ON_EN_KILLFOCUS(IDC_RANGE_EDIT, &CPowerControlDlg::OnEnKillfocusRangeEdit)
	ON_BN_CLICKED(IDC_BACKUPPOWER_CHECK, &CPowerControlDlg::OnBnClickedBackuppowerCheck)
END_MESSAGE_MAP()

BOOL CPowerControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitPowerSetting();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


// CPowerControlDlg 消息处理程序

//以下都是控件的事件处理函数
void CPowerControlDlg::OnBnClickedRelayCheck()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	EnableWindowFun(m_RelayCheckVaule);
}

void CPowerControlDlg::OnBnClickedBackuppowerCheck()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	EnableWindowFun(m_RelayCheckVaule);
}

void CPowerControlDlg::OnEnKillfocusPowercontrolnumber1Edit()
{
	// TODO:  在此添加控件通知处理程序代码
	PowerControlColorResetFun(0);
}

void CPowerControlDlg::OnEnKillfocusPowercontrolnumber2Edit()
{
	// TODO:  在此添加控件通知处理程序代码
	PowerControlColorResetFun(1);
}

void CPowerControlDlg::OnCbnSelchangePowercontrolcomlist1Combo()
{
	// TODO:  在此添加控件通知处理程序代码
	PowerControlColorResetFun(0);
}

void CPowerControlDlg::OnCbnSelchangePowercontrolcomlist2Combo()
{
	// TODO:  在此添加控件通知处理程序代码
	PowerControlColorResetFun(1);
}

void CPowerControlDlg::OnCbnSelchangeBackuppowercomlistCombo()
{
	// TODO:  在此添加控件通知处理程序代码
	BackUpPowerResetFun();
}

void CPowerControlDlg::OnEnKillfocusBackuppowernumberEdit()
{
	// TODO:  在此添加控件通知处理程序代码
	BackUpPowerResetFun();
}

void CPowerControlDlg::OnEnKillfocusVoltageEdit()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
}

void CPowerControlDlg::OnEnKillfocusRangeEdit()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
}


void CPowerControlDlg::OnCbnSelchangeRelayport1Combo()
{
	// TODO:  在此添加控件通知处理程序代码
	RelayColorResetFun(0);
}

void CPowerControlDlg::OnCbnSelchangeRelayport2Combo()
{
	// TODO:  在此添加控件通知处理程序代码
	RelayColorResetFun(1);
}

void CPowerControlDlg::OnCbnSelchangeRelayport3Combo()
{
	// TODO:  在此添加控件通知处理程序代码
	RelayColorResetFun(2);
}

void CPowerControlDlg::OnCbnSelchangeRelayport4Combo()
{
	// TODO:  在此添加控件通知处理程序代码
	RelayColorResetFun(3);
}

void CPowerControlDlg::OnCbnSelchangeRelayport5Combo()
{
	// TODO:  在此添加控件通知处理程序代码
	RelayColorResetFun(4);
}

void CPowerControlDlg::OnCbnSelchangeRelayport6Combo()
{
	// TODO:  在此添加控件通知处理程序代码
	RelayColorResetFun(5);
}

void CPowerControlDlg::OnCbnSelchangeRelayport7Combo()
{
	// TODO:  在此添加控件通知处理程序代码
	RelayColorResetFun(6);
}

void CPowerControlDlg::OnCbnSelchangeRelayport8Combo()
{
	// TODO:  在此添加控件通知处理程序代码
	RelayColorResetFun(7);
}

void CPowerControlDlg::OnBnClickedPowercontroltest1Button()
{
	// TODO:  在此添加控件通知处理程序代码
	PowerControlTestFun(0);
}

void CPowerControlDlg::OnBnClickedPowercontroltest2Button()
{
	// TODO:  在此添加控件通知处理程序代码
	PowerControlTestFun(1);
}

void CPowerControlDlg::OnBnClickedRelaytest1Button()
{
	// TODO:  在此添加控件通知处理程序代码
	RelayTestFun(0);
}

void CPowerControlDlg::OnBnClickedRelaytest2Button()
{
	// TODO:  在此添加控件通知处理程序代码
	RelayTestFun(1);
}

void CPowerControlDlg::OnBnClickedRelaytest3Button()
{
	// TODO:  在此添加控件通知处理程序代码
	RelayTestFun(2);
}

void CPowerControlDlg::OnBnClickedRelaytest4Button()
{
	// TODO:  在此添加控件通知处理程序代码
	RelayTestFun(3);
}

void CPowerControlDlg::OnBnClickedRelaytest5Button()
{
	// TODO:  在此添加控件通知处理程序代码
	RelayTestFun(4);
}

void CPowerControlDlg::OnBnClickedRelaytest6Button()
{
	// TODO:  在此添加控件通知处理程序代码
	RelayTestFun(5);
}

void CPowerControlDlg::OnBnClickedRelaytest7Button()
{
	// TODO:  在此添加控件通知处理程序代码
	RelayTestFun(6);
}

void CPowerControlDlg::OnBnClickedRelaytest8Button()
{
	// TODO:  在此添加控件通知处理程序代码
	RelayTestFun(7);
}

void CPowerControlDlg::OnBnClickedRelayalltest1Button()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 0; i < 4; i++)
	{
		RelayTestFun(i);
		UpdateWindow();
	}
}

void CPowerControlDlg::OnBnClickedRelayalltest2Button()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 4; i < 8; i++)
	{
		RelayTestFun(i);
		UpdateWindow();
	}
}

void CPowerControlDlg::OnBnClickedBackuppowertestButton()
{
	// TODO:  在此添加控件通知处理程序代码
	BackUpPowerTestFun();
}

//初始化电流测试参数
void CPowerControlDlg::InitPowerSetting()
{
	CString tempStr;

	m_RelayportControlArray[0] = &m_Relayport1Control;
	m_RelayportControlArray[1] = &m_Relayport2Control;
	m_RelayportControlArray[2] = &m_Relayport3Control;
	m_RelayportControlArray[3] = &m_Relayport4Control;
	m_RelayportControlArray[4] = &m_Relayport5Control;
	m_RelayportControlArray[5] = &m_Relayport6Control;
	m_RelayportControlArray[6] = &m_Relayport7Control;
	m_RelayportControlArray[7] = &m_Relayport8Control;

	m_RelayportVauleArray[0] = &m_Relayport1Vaule;
	m_RelayportVauleArray[1] = &m_Relayport2Vaule;
	m_RelayportVauleArray[2] = &m_Relayport3Vaule;
	m_RelayportVauleArray[3] = &m_Relayport4Vaule;
	m_RelayportVauleArray[4] = &m_Relayport5Vaule;
	m_RelayportVauleArray[5] = &m_Relayport6Vaule;
	m_RelayportVauleArray[6] = &m_Relayport7Vaule;
	m_RelayportVauleArray[7] = &m_Relayport8Vaule;

	m_RelayTestControlArray[0] = &m_RelayTest1Control;
	m_RelayTestControlArray[1] = &m_RelayTest2Control;
	m_RelayTestControlArray[2] = &m_RelayTest3Control;
	m_RelayTestControlArray[3] = &m_RelayTest4Control;
	m_RelayTestControlArray[4] = &m_RelayTest5Control;
	m_RelayTestControlArray[5] = &m_RelayTest6Control;
	m_RelayTestControlArray[6] = &m_RelayTest7Control;
	m_RelayTestControlArray[7] = &m_RelayTest8Control;

	m_RelayResultControlArray[0] = &m_RelayResult1Control;
	m_RelayResultControlArray[1] = &m_RelayResult2Control;
	m_RelayResultControlArray[2] = &m_RelayResult3Control;
	m_RelayResultControlArray[3] = &m_RelayResult4Control;
	m_RelayResultControlArray[4] = &m_RelayResult5Control;
	m_RelayResultControlArray[5] = &m_RelayResult6Control;
	m_RelayResultControlArray[6] = &m_RelayResult7Control;
	m_RelayResultControlArray[7] = &m_RelayResult8Control;

	m_PowerControlTestControlArray[0] = &m_PowerControlTest1Control;
	m_PowerControlTestControlArray[1] = &m_PowerControlTest2Control;

	m_PowerControlComListControlArray[0] = &m_PowerControlComList1Control;
	m_PowerControlComListControlArray[1] = &m_PowerControlComList2Control;

	m_PowerControlNumberControlArray[0] = &m_PowerControlNumber1Control;
	m_PowerControlNumberControlArray[1] = &m_PowerControlNumber2Control;

	m_PowerControlTestResultControlArray[0] = &m_PowerControlResult1Control;
	m_PowerControlTestResultControlArray[1] = &m_PowerControlResult2Control;

	m_PowerControlTestVauleArray[0] = &m_PowerControlTest1Vaule;
	m_PowerControlTestVauleArray[1] = &m_PowerControlTest2Vaule;

	m_PowerControlComListVauleArray[0] = &m_PowerControlComList1Vaule;
	m_PowerControlComListVauleArray[1] = &m_PowerControlComList2Vaule;

	m_PowerControlNumberVauleArray[0] = &m_PowerControlNumber1Vaule;
	m_PowerControlNumberVauleArray[1] = &m_PowerControlNumber2Vaule;

	PowerControlTestResultColor[0] = WhiteColor;
	PowerControlTestResultColor[1] = WhiteColor;
	BackUpPowerTestResultColor = WhiteColor;
	for (int i = 0; i < 8; i++)
	{
		PortTestResultColor[i] = WhiteColor;
	}

	FindCommPort(&m_RelayListControl);


	//初始化仪器以及备用电源下拉框
	for (int i = 0; i < 2; i++)
	{
		m_PowerControlComListControlArray[i]->ResetContent();
		for (int j = 0; j < 6; j++)
		{
			tempStr.Format("GPIB%d::X::INSTR", j);
			m_PowerControlComListControlArray[i]->AddString(tempStr);
		}
		m_PowerControlComListControlArray[i]->AddString("COMX");
		m_PowerControlComListControlArray[i]->SetCurSel(0);
	}

	m_BackUpPowerComListControl.ResetContent();
	for (int i = 0; i < 6; i++)
	{
		tempStr.Format("GPIB%d::X::INSTR", i);
		m_BackUpPowerComListControl.AddString(tempStr);
	}
	m_BackUpPowerComListControl.AddString("COMX");
	m_BackUpPowerComListControl.SetCurSel(0);

	//初始化程控电源端口
	for (int i = 0; i < 8; i++)
	{
		m_RelayportControlArray[i]->ResetContent();
		for (int j = 0; j < 32; j++)
		{
			CString PortStr;
			PortStr.Format("%d", j + 1);
			m_RelayportControlArray[i]->AddString(PortStr);
		}
		m_RelayportControlArray[i]->SetCurSel(i*2);
	}
	
	InitPowerReadIniSettingFun();

	EnableWindowFun(m_RelayCheckVaule);
}

//写INI文件
void CPowerControlDlg::InitPowerWriteIniSettingFun()
{
	CFileFind finder;
	CString IniFileName = ".\\PowerTestSetting.ini";
	CString ValueStr, SettringStr;
	CString SettringTitle1 = "MachineAddress", SettringTitle2 = "RelaySetting";
	int ValueInt;
	BOOL ifFind = finder.FindFile(IniFileName);

	for (int i = 0; i < 2; i++)
	{
		SettringStr.Format("Machine%dType", i);
		m_PowerControlComListControlArray[i]->GetWindowTextA(ValueStr);
		WritePrivateProfileString(SettringTitle1, SettringStr, ValueStr, IniFileName);
		SettringStr.Format("Machine%dNum", i);
		m_PowerControlNumberControlArray[i]->GetWindowTextA(ValueStr);
		WritePrivateProfileString(SettringTitle1, SettringStr, ValueStr, IniFileName);
	}

	ValueStr.Format("%d", m_RelayCheckVaule);
	WritePrivateProfileString(SettringTitle2, _T("RelayFlag"), ValueStr, IniFileName);

	ValueStr.Format("%d", m_BackuppowerCheckVaule);
	WritePrivateProfileString(SettringTitle2, _T("BackuppowerFlag"), ValueStr, IniFileName);

	ValueStr.Format("%f", m_VoltageVaule);
	WritePrivateProfileString(SettringTitle2, _T("Voltage"), ValueStr, IniFileName);

	ValueStr.Format("%f", m_RangeVaule);
	WritePrivateProfileString(SettringTitle2, _T("Range"), ValueStr, IniFileName);

	m_RelayListControl.GetWindowTextA(ValueStr);
	WritePrivateProfileString(SettringTitle2, _T("RelayAddress"), ValueStr, IniFileName);

	m_BackUpPowerComListControl.GetWindowTextA(ValueStr);
	WritePrivateProfileString(SettringTitle2, _T("BackUpPowerType"), ValueStr, IniFileName);
	m_BackUpPowerNumberControl.GetWindowTextA(ValueStr);
	WritePrivateProfileString(SettringTitle2, _T("BackUpPowerNum"), ValueStr, IniFileName);
	
	for (int i = 0; i < 8; i++)
	{
		SettringStr.Format("PortSetting%d", i);
		m_RelayportControlArray[i]->GetWindowTextA(ValueStr);
		WritePrivateProfileString(SettringTitle2, SettringStr, ValueStr, IniFileName);
	}

}

//读INI文件
void CPowerControlDlg::InitPowerReadIniSettingFun()
{
	CFileFind finder;
	CString IniFileName = ".\\PowerTestSetting.ini";
	CString ValueStr,SettringStr;
	CString SettringTitle1 = "MachineAddress", SettringTitle2 = "RelaySetting";
	int ValueInt;

	BOOL ifFind = finder.FindFile(IniFileName);//先检测文件存不存在
	
	if (ifFind)
	{
		for (int i = 0; i < 2; i++)
		{
			SettringStr.Format("Machine%dType", i);
			GetPrivateProfileString(SettringTitle1, SettringStr, _T(""), ValueStr.GetBuffer(50), 50, IniFileName);
			*m_PowerControlComListVauleArray[i] = ValueStr;
			ValueStr.ReleaseBuffer();
			SettringStr.Format("Machine%dNum", i);
			ValueInt = GetPrivateProfileInt(SettringTitle1, SettringStr, i+4, IniFileName);
			*m_PowerControlNumberVauleArray[i]=ValueInt;
		}

		ValueInt = GetPrivateProfileInt(SettringTitle2, _T("RelayFlag"), 0, IniFileName);
		m_RelayCheckVaule = ValueInt;

		ValueInt = GetPrivateProfileInt(SettringTitle2, _T("BackuppowerFlag"), 0, IniFileName);
		m_BackuppowerCheckVaule = ValueInt;

		ValueInt = GetPrivateProfileInt(SettringTitle2, _T("Voltage"), 4.0, IniFileName);
		m_VoltageVaule = ValueInt;

		ValueInt = GetPrivateProfileInt(SettringTitle2, _T("Range"), 0.001, IniFileName);
		m_RangeVaule = ValueInt;

		GetPrivateProfileString(SettringTitle2, _T("RelayAddress"), _T(""), ValueStr.GetBuffer(50), 50, IniFileName);
		m_RelayListVaule=ValueStr;
		ValueStr.ReleaseBuffer();

		GetPrivateProfileString(SettringTitle2, _T("BackUpPowerType"), _T(""), ValueStr.GetBuffer(50), 50, IniFileName);
		m_BackUpPowerComListVaule=ValueStr;
		ValueStr.ReleaseBuffer();

		ValueInt = GetPrivateProfileInt(SettringTitle2, _T("BackUpPowerNum"), 6, IniFileName);
		m_BackUpPowerNumberVaule = ValueInt;

		for (int i = 0; i < 8; i++)
		{
			SettringStr.Format("PortSetting%d", i);
			GetPrivateProfileString(SettringTitle2, SettringStr, _T(""), ValueStr.GetBuffer(50), 50, IniFileName);
			*m_RelayportVauleArray[i]=ValueStr;
			ValueStr.ReleaseBuffer();
		}
	}
	else
	{
		for (int i = 0; i < 2; i++)
		{
			m_PowerControlComListControlArray[i]->SetCurSel(0);
			*m_PowerControlNumberVauleArray[i] = i + 4;
		}

		m_BackUpPowerComListControl.SetCurSel(0);
		m_BackUpPowerNumberVaule = 6;
		m_VoltageVaule = 3.0;
		m_RelayCheckVaule = 0;
		m_RangeVaule = 0.001;
		m_BackuppowerCheckVaule = 0;

		for (int i = 0; i < 8; i++)
		{
			CString PortStr;
			PortStr.Format("%d", (i+1)*2-1);
			*m_RelayportVauleArray[i] = PortStr;
		}
	}
	UpdateData(FALSE);
}

//禁用与启用控件
void CPowerControlDlg::EnableWindowFun(BOOL Choose)
{
	for (int i = 0; i < 8; i++)
	{
		m_RelayportControlArray[i]->EnableWindow(Choose);
		m_RelayTestControlArray[i]->EnableWindow(Choose);
		m_RelayResultControlArray[i]->EnableWindow(Choose);
	}

	m_RelayListControl.EnableWindow(Choose);
	GetDlgItem(IDC_RELAYALLTEST1_BUTTON)->EnableWindow(Choose);
	GetDlgItem(IDC_RELAYALLTEST2_BUTTON)->EnableWindow(Choose);

	GetDlgItem(IDC_BACKUPPOWER_CHECK)->EnableWindow(Choose);
	GetDlgItem(IDC_BACKUPPOWERTEST_BUTTON)->EnableWindow(Choose&m_BackuppowerCheckVaule);
	m_BackUpPowerComListControl.EnableWindow(Choose&m_BackuppowerCheckVaule);
	m_BackUpPowerNumberControl.EnableWindow(Choose&m_BackuppowerCheckVaule);
	m_BackUpPowerTestResultControl.EnableWindow(Choose&m_BackuppowerCheckVaule);


	m_PowerControlComListControlArray[1]->EnableWindow(Choose);
	m_PowerControlNumberControlArray[1]->EnableWindow(Choose);
	m_PowerControlTestResultControlArray[1]->EnableWindow(Choose);
	m_PowerControlTestControlArray[1]->EnableWindow(Choose);
	m_BackUpPowerTestResultControl.EnableWindow(Choose);
}

//仪器初始化测试函数
void CPowerControlDlg::PowerControlTestFun(int Num)
{
	CString tempStr1,tempStr2;
	tempStr1 = *m_PowerControlComListVauleArray[Num];
	tempStr2.Format("%d",*m_PowerControlNumberVauleArray[Num]);
	tempStr1.Replace("X", tempStr2);
	char *p = tempStr1.GetBuffer();
	BOOL TestFlag = QCurrentTest.PowerOn(p, m_VoltageVaule);
	if (TestFlag == TRUE)
	{
		PowerControlTestResultColor[Num] = GreenColor;
		m_PowerControlTestResultControlArray[Num]->SetWindowTextA("");
	}
	else if (TestFlag == FALSE)
	{
		PowerControlTestResultColor[Num] = RedColor;
		m_PowerControlTestResultControlArray[Num]->SetWindowTextA("");
	}
}

//继电器电流测试函数
void CPowerControlDlg::RelayTestFun(int Num)
{
	CString RelayAddress, BackUpPowerAddress,tempStr1, tempStr2;
	m_RelayListControl.GetWindowText(RelayAddress);
	if (Num >= 0 || Num <= 3)
	{
		tempStr1 = *m_PowerControlComListVauleArray[0];
		tempStr2.Format("%d", *m_PowerControlNumberVauleArray[0]);
		tempStr1.Replace("X", tempStr2);
	}
	else if (Num >= 4 || Num <= 7)
	{
		tempStr1 = *m_PowerControlComListVauleArray[1];
		tempStr2.Format("%d", *m_PowerControlNumberVauleArray[1]);
		tempStr1.Replace("X", tempStr2);
	}
	char *p1 = tempStr1.GetBuffer(), *p2 = RelayAddress.GetBuffer();

	BOOL TestFlag = QCurrentTest.RelayChange(_ttoi(*m_RelayportVauleArray[Num]), p1, p2, 3, 100);
	if (TestFlag == TRUE)
	{
		PortTestResultColor[Num] = GreenColor;
		m_RelayResultControlArray[Num]->SetWindowTextA("");
	}
	else if (TestFlag == FALSE)
	{
		PortTestResultColor[Num] = RedColor;
		m_RelayResultControlArray[Num]->SetWindowTextA("");
	}
}

//备用电源测试函数
void CPowerControlDlg::BackUpPowerTestFun()
{
	CString tempStr1, tempStr2;
	tempStr1 = m_BackUpPowerComListVaule;
	tempStr2.Format("%d", m_BackUpPowerNumberVaule);
	tempStr1.Replace("X", tempStr2);
	char *p = tempStr1.GetBuffer();
	BOOL TestFlag = QCurrentTest.PowerOn(p, m_VoltageVaule);
	if (TestFlag == TRUE)
	{
		BackUpPowerTestResultColor = GreenColor;
		m_BackUpPowerTestResultControl.SetWindowTextA("");
	}
	else if (TestFlag == FALSE)
	{
		BackUpPowerTestResultColor = RedColor;
		m_BackUpPowerTestResultControl.SetWindowTextA("");
	}
}

//电流测试结果重置
void CPowerControlDlg::PowerControlColorResetFun(int Num)
{
	CString tempStr1;
	int tempStr2;

	tempStr1 = *m_PowerControlComListVauleArray[Num];
	tempStr2 = *m_PowerControlNumberVauleArray[Num]; 
	UpdateData(TRUE);
	if (tempStr1 != *m_PowerControlComListVauleArray[Num] || tempStr2 != *m_PowerControlNumberVauleArray[Num])
	{
		PowerControlTestResultColor[Num] = WhiteColor;
		m_PowerControlTestResultControlArray[Num]->SetWindowTextA("");
	}
}

//继电器测试结果重置
void CPowerControlDlg::RelayColorResetFun(int Num)
{
	CString tempStr1;

	tempStr1 = *m_RelayportVauleArray[Num];
	UpdateData(TRUE);
	if (tempStr1 != *m_RelayportVauleArray[Num])
	{
		PortTestResultColor[Num] = WhiteColor;
		m_RelayResultControlArray[Num]->SetWindowTextA("");
	}
}

//备用电源测试结果重置
void CPowerControlDlg::BackUpPowerResetFun()
{
	CString tempStr1;
	int tempStr2;

	tempStr1 = m_BackUpPowerComListVaule;
	tempStr2 = m_BackUpPowerNumberVaule;
	UpdateData(TRUE);
	if (tempStr1 != m_BackUpPowerComListVaule || tempStr2 != m_BackUpPowerNumberVaule)
	{
		BackUpPowerTestResultColor = WhiteColor;
		m_BackUpPowerTestResultControl.SetWindowTextA("");
	}
}

//笔刷，改变颜色消息循环函数
HBRUSH CPowerControlDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_RELAYRESULT1_EDIT:
		//m_brush.CreateSolidBrush(PortTestResultColor[0]);
		pDC->SetBkColor(PortTestResultColor[0]);//设置背景颜色
		break;
	case IDC_RELAYRESULT2_EDIT:
		//m_brush.CreateSolidBrush(PortTestResultColor[1]);
		pDC->SetBkColor(PortTestResultColor[1]);//设置背景颜色
		break;
	case IDC_RELAYRESULT3_EDIT:
		//m_brush.CreateSolidBrush(PortTestResultColor[2]);
		pDC->SetBkColor(PortTestResultColor[2]);//设置背景颜色
		break;
	case IDC_RELAYRESULT4_EDIT:
		//m_brush.CreateSolidBrush(PortTestResultColor[3]);
		pDC->SetBkColor(PortTestResultColor[3]);//设置背景颜色
		break;
	case IDC_RELAYRESULT5_EDIT:
		//m_brush.CreateSolidBrush(PortTestResultColor[4]);
		pDC->SetBkColor(PortTestResultColor[4]);//设置背景颜色
		break;
	case IDC_RELAYRESULT6_EDIT:
		//m_brush.CreateSolidBrush(PortTestResultColor[5]);
		pDC->SetBkColor(PortTestResultColor[5]);//设置背景颜色
		break;
	case IDC_RELAYRESULT7_EDIT:
		//m_brush.CreateSolidBrush(PortTestResultColor[6]);
		pDC->SetBkColor(PortTestResultColor[6]);//设置背景颜色
		break;
	case IDC_RELAYRESULT8_EDIT:
		//m_brush.CreateSolidBrush(PortTestResultColor[7]);
		pDC->SetBkColor(PortTestResultColor[7]);//设置背景颜色
		break;
	case IDC_POWERCONTROLRESULT1_EDIT:
		//m_brush.CreateSolidBrush(PowerTestResultColor[0]);
		pDC->SetBkColor(PowerControlTestResultColor[0]);//设置背景颜色
		break;
	case IDC_POWERCONTROLRESULT2_EDIT:
		//m_brush.CreateSolidBrush(PowerTestResultColor[1]);
		pDC->SetBkColor(PowerControlTestResultColor[1]);//设置背景颜色
		break;
	case IDC_BACKUPPOWERTESTRESULT_EDIT:
		//m_brush.CreateSolidBrush(PowerTestResultColor[1]);
		pDC->SetBkColor(BackUpPowerTestResultColor);//设置背景颜色
		break;
	default:
		break;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

//从注册表中寻找已经注册串口号然后展示出来
void CPowerControlDlg::FindCommPort(CComboBox *pComboBox)
{
	//先清空一下列表
	pComboBox->ResetContent();

	HKEY hKey;
#ifdef _DEBUG
	ASSERT(pComboBox != NULL);
	pComboBox->AssertValid();
#endif
	if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Hardware\\DeviceMap\\SerialComm"), NULL, KEY_READ, &hKey) == ERROR_SUCCESS) // 打开串口注册表
	{
		int i = 0;
		char portName[256], commName[256];
		DWORD dwLong, dwSize;
		while (1)
		{
			dwLong = dwSize = sizeof(portName);
			if (::RegEnumValue(hKey, i, LPSTR(portName), &dwLong, NULL, NULL, (PUCHAR)commName, &dwSize) == ERROR_NO_MORE_ITEMS) // 枚举串口
			{
				break;
			}
			pComboBox->AddString(LPSTR(commName));
			i++;
		}
		if (pComboBox->GetCount() == 0)
		{
			pComboBox->ResetContent();
			//SetRicheditText(L"找不到串口！请检测串口设备是否存在问题！", 1);
			//MessageBox(L"找不到串口！请检测串口设备是否存在问题！", L"提示信息", NULL);
			RegCloseKey(hKey);
			return;
		}
		pComboBox->SetCurSel(0);
		RegCloseKey(hKey);
	}
}

/*其它函数*/
void CPowerControlDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	InitPowerWriteIniSettingFun();
	CDialogEx::OnOK();
}







