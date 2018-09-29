// ModelConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCP4ECurrentTool.h"
#include "ModelConfig.h"
#include "afxdialogex.h"
#include "ADOManage.h"

// CModelConfig 对话框

IMPLEMENT_DYNAMIC(CModelConfig, CDialogEx)

CModelConfig::CModelConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(CModelConfig::IDD, pParent)
	, m_AddModelValue(_T(""))
	, m_TestCommandValue(_T("AT^GT_CM=MODE"))
	, m_TestCommandReplyValue(_T("Mode:1"))
	, m_RidCommandValue(_T("AT^GT_CM=ID,1"))
	, m_RidCommandReplyValue(_T("Chip RID:"))
	, m_StandbyCommandValue(_T("AT^GT_CM=TBT,Shutdown"))
	, m_StandbyCommandReplyValue(_T(""))
	, m_SleepCommandValue(_T("AT^GT_CM=TBT,PWRSAVE"))
	, m_SleepCommandReplyValue(_T("PWRSAVE OK"))
	, m_DBStandbyUpValue(0.15)
	, m_DBStandbyDownValue(0)
	, m_DBSleepUpValue(0.15)
	, m_DBSleepDownValue(0)
	, m_CommandCountValue(3)
	, m_CommandSendIntervalValue(1000)
	, m_CommandReadIntervalValue(150)
{

}

CModelConfig::~CModelConfig()
{
}

void CModelConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADDMODEL_EDIT, m_AddModelControl);
	DDX_Text(pDX, IDC_ADDMODEL_EDIT, m_AddModelValue);
	DDX_Control(pDX, IDC_SELECTMODEL_COMBO, m_SelectModelCombo);
	DDX_Control(pDX, IDC_TESTCOMMAND_EDIT, m_TestCommandControl);
	DDX_Text(pDX, IDC_TESTCOMMAND_EDIT, m_TestCommandValue);
	DDX_Control(pDX, IDC_TESTCOMMANDREPLY_EDIT, m_TestCommandReplyControl);
	DDX_Text(pDX, IDC_TESTCOMMANDREPLY_EDIT, m_TestCommandReplyValue);
	DDX_Control(pDX, IDC_RIDCOMMAND_EDIT, m_RidCommandControl);
	DDX_Text(pDX, IDC_RIDCOMMAND_EDIT, m_RidCommandValue);
	DDX_Control(pDX, IDC_RIDCOMMANDREPLY_EDIT, m_RidCommandReplyControl);
	DDX_Text(pDX, IDC_RIDCOMMANDREPLY_EDIT, m_RidCommandReplyValue);
	DDX_Control(pDX, IDC_STANDBYCOMMAND_EDIT, m_StandbyCommandControl);
	DDX_Text(pDX, IDC_STANDBYCOMMAND_EDIT, m_StandbyCommandValue);
	DDX_Control(pDX, IDC_STANDBYCOMMANDREPLY_EDIT, m_StandbyCommandReplyControl);
	DDX_Text(pDX, IDC_STANDBYCOMMANDREPLY_EDIT, m_StandbyCommandReplyValue);
	DDX_Control(pDX, IDC_SLEEPCOMMAND_EDIT, m_SleepCommandControl);
	DDX_Text(pDX, IDC_SLEEPCOMMAND_EDIT, m_SleepCommandValue);
	DDX_Control(pDX, IDC_SLEEPCOMMANDREPLY_EDIT, m_SleepCommandReplyControl);
	DDX_Text(pDX, IDC_SLEEPCOMMANDREPLY_EDIT, m_SleepCommandReplyValue);
	DDX_Control(pDX, IDC_DBSTANDBYUP_EDIT, m_DBStandbyUpControl);
	DDX_Text(pDX, IDC_DBSTANDBYUP_EDIT, m_DBStandbyUpValue);
	DDV_MinMaxFloat(pDX, m_DBStandbyUpValue, 0, 1);
	DDX_Control(pDX, IDC_DBSTANDBYDOWN_EDIT, m_DBStandbyDownControl);
	DDX_Text(pDX, IDC_DBSTANDBYDOWN_EDIT, m_DBStandbyDownValue);
	DDV_MinMaxFloat(pDX, m_DBStandbyDownValue, 0, 1);
	DDX_Control(pDX, IDC_DBSLEEPUP_EDIT, m_DBSleepUpControl);
	DDX_Text(pDX, IDC_DBSLEEPUP_EDIT, m_DBSleepUpValue);
	DDV_MinMaxFloat(pDX, m_DBSleepUpValue, 0, 1);
	DDX_Control(pDX, IDC_DBSLEEPDOWN_EDIT, m_DBSleepDownControl);
	DDX_Text(pDX, IDC_DBSLEEPDOWN_EDIT, m_DBSleepDownValue);
	DDV_MinMaxFloat(pDX, m_DBSleepDownValue, 0, 1);
	DDX_Control(pDX, IDC_COMMANDCOUNT_EDIT, m_CommandCountControl);
	DDX_Text(pDX, IDC_COMMANDCOUNT_EDIT, m_CommandCountValue);
	DDV_MinMaxInt(pDX, m_CommandCountValue, 0, 10000);
	DDX_Control(pDX, IDC_COMMANDSENDINTERVAL_EDIT, m_CommandSendIntervalControl);
	DDX_Text(pDX, IDC_COMMANDSENDINTERVAL_EDIT, m_CommandSendIntervalValue);
	DDV_MinMaxInt(pDX, m_CommandSendIntervalValue, 0, 10000);
	DDX_Control(pDX, IDC_COMMANDREADINTERVAL_EDIT, m_CommandReadIntervalControl);
	DDX_Text(pDX, IDC_COMMANDREADINTERVAL_EDIT, m_CommandReadIntervalValue);
	DDV_MinMaxInt(pDX, m_CommandReadIntervalValue, 0, 10000);
}


BEGIN_MESSAGE_MAP(CModelConfig, CDialogEx)
	ON_BN_CLICKED(IDOK, &CModelConfig::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CModelConfig::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_ADDMODEL_BUTTON, &CModelConfig::OnBnClickedAddmodelButton)
	ON_BN_CLICKED(IDC_DELETEMODEL_BUTTON, &CModelConfig::OnBnClickedDeletemodelButton)
	ON_BN_CLICKED(IDC_SAVEMODELINFO_BUTTON, &CModelConfig::OnBnClickedSavemodelinfoButton)
	ON_CBN_KILLFOCUS(IDC_SELECTMODEL_COMBO, &CModelConfig::OnCbnKillfocusSelectmodelCombo)
	ON_CBN_SELENDOK(IDC_SELECTMODEL_COMBO, &CModelConfig::OnCbnSelendokSelectmodelCombo)
	ON_CBN_SELCHANGE(IDC_SELECTMODEL_COMBO, &CModelConfig::OnCbnSelchangeSelectmodelCombo)
END_MESSAGE_MAP()

BOOL CModelConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ConfigVauleEnableWindows(FALSE);//初始化控件使能
	InitModelDBOperation();//初始化下拉框

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

// CModelConfig 消息处理程序

/*控件使能函数*/
//机型配置的编辑框使能函数
void CModelConfig::ConfigVauleEnableWindows(BOOL choose)
{
	GetDlgItem(IDC_TESTCOMMAND_EDIT)->EnableWindow(choose);
	GetDlgItem(IDC_TESTCOMMANDREPLY_EDIT)->EnableWindow(choose);
	GetDlgItem(IDC_RIDCOMMAND_EDIT)->EnableWindow(choose);
	GetDlgItem(IDC_RIDCOMMANDREPLY_EDIT)->EnableWindow(choose);
	GetDlgItem(IDC_STANDBYCOMMAND_EDIT)->EnableWindow(choose);
	GetDlgItem(IDC_STANDBYCOMMANDREPLY_EDIT)->EnableWindow(choose);
	GetDlgItem(IDC_SLEEPCOMMAND_EDIT)->EnableWindow(choose);
	GetDlgItem(IDC_SLEEPCOMMANDREPLY_EDIT)->EnableWindow(choose);
	GetDlgItem(IDC_DBSTANDBYUP_EDIT)->EnableWindow(choose);
	GetDlgItem(IDC_DBSTANDBYDOWN_EDIT)->EnableWindow(choose);
	GetDlgItem(IDC_DBSLEEPUP_EDIT)->EnableWindow(choose);
	GetDlgItem(IDC_DBSLEEPDOWN_EDIT)->EnableWindow(choose);
	GetDlgItem(IDC_COMMANDCOUNT_EDIT)->EnableWindow(choose);
	GetDlgItem(IDC_COMMANDSENDINTERVAL_EDIT)->EnableWindow(choose);
	GetDlgItem(IDC_COMMANDREADINTERVAL_EDIT)->EnableWindow(choose);
	GetDlgItem(IDC_SAVEMODELINFO_BUTTON)->EnableWindow(choose);
	GetDlgItem(IDC_DELETEMODEL_BUTTON)->EnableWindow(choose);
}

/*机型配置模块函数*/
//添加机型配置按钮
void CModelConfig::OnBnClickedAddmodelButton()
{
	// TODO:  在此添加控件通知处理程序代码
	CString ModelName;
	GetDlgItemText(IDC_ADDMODEL_EDIT, ModelName);
	AddModelDBOperation(ModelName);
	InitModelDBOperation();//初始化下拉框
}

//删除机型配置按钮
void CModelConfig::OnBnClickedDeletemodelButton()
{
	// TODO:  在此添加控件通知处理程序代码
	CString ModelName;
	GetDlgItemText(IDC_SELECTMODEL_COMBO, ModelName);
	DeleteModelDBOperation(ModelName);
	InitModelDBOperation();//初始化下拉框
}

//保存机型配置按钮
void CModelConfig::OnBnClickedSavemodelinfoButton()
{
	// TODO:  在此添加控件通知处理程序代码
	SaveModelDBOperation();
}

//添加机型数据库操作
void CModelConfig::AddModelDBOperation(CString ModelName)
{
	ADOManage adomanageInsert;
	adomanageInsert.ConnSQL();
	adomanageInsert.ConfigInsertSql(ModelName, 0.15, 0, 0.1, 0, _T("AT^GT_CM=MODE"), _T("Mode:1"), _T("AT^GT_CM=ID,1"), _T("Chip RID:"), _T("AT^GT_CM=TBT,Shutdown"), _T(""), _T("AT^GT_CM=TBT,PWRSAVE"), _T("PWRSAVE OK"), 3, 150, 1000);
	adomanageInsert.CloseAll();
	ConfigVauleEnableWindows(FALSE);//初始化控件使能
}

//删除机型数据库操作
void CModelConfig::DeleteModelDBOperation(CString ModelName)
{
	ADOManage adomanageDelete;
	adomanageDelete.ConnSQL();
	adomanageDelete.DeleteConfigByModelNameSql(ModelName);
	adomanageDelete.CloseAll();

	//清空一下界面
	m_TestCommandValue = L"";
	m_TestCommandReplyValue = L"";
	m_RidCommandValue = L"";
	m_RidCommandReplyValue = L"";
	m_StandbyCommandValue = L"";
	m_StandbyCommandReplyValue = L"";
	m_SleepCommandValue = L"";
	m_SleepCommandReplyValue = L"";
	m_DBStandbyUpValue = 0;
	m_DBStandbyDownValue = 0;
	m_DBSleepUpValue = 0;
	m_DBSleepDownValue = 0;
	m_CommandCountValue = 0;
	m_CommandReadIntervalValue = 0;
	m_CommandSendIntervalValue = 0;
	UpdateData(FALSE);
	ConfigVauleEnableWindows(FALSE);//初始化控件使能
}

//保存机型数据库操作
void CModelConfig::SaveModelDBOperation()
{
	UpdateData(TRUE);
	ADOManage adomanageUpdate;
	CString ModelName;
	int nSel;
	nSel = m_SelectModelCombo.GetCurSel();
	m_SelectModelCombo.GetLBText(nSel, ModelName);
	adomanageUpdate.ConnSQL();
	adomanageUpdate.ConfigUpdataSql(ModelName, m_DBStandbyUpValue, m_DBStandbyDownValue, m_DBSleepUpValue, m_DBSleepDownValue, m_TestCommandValue, m_TestCommandReplyValue, m_RidCommandValue, m_RidCommandReplyValue, m_StandbyCommandValue, m_StandbyCommandReplyValue, m_SleepCommandValue, m_SleepCommandReplyValue, m_CommandCountValue, m_CommandReadIntervalValue, m_CommandSendIntervalValue);
	adomanageUpdate.CloseAll();
}

//查找机型数据库操作
void CModelConfig::FindModelDBOperation()
{
	ADOManage adomanageFind;
	CString ModelName,strtemp;
	float ftemp;
	int itemp,nSel;
	nSel = m_SelectModelCombo.GetCurSel();
	m_SelectModelCombo.GetLBText(nSel, ModelName);
	adomanageFind.ConnSQL();
	adomanageFind.CheckConfigByModelNameSql(ModelName);

	//获取待机上下限的值（注意，由于没有浮点型的setdlgitem，所以只能用updatadata的方式来更新到界面）
	ftemp = adomanageFind.m_pRecordSet->GetCollect("StandbyUp");
	m_DBStandbyUpValue = ftemp;
	//SetDlgItemInt(IDC_DBSTANDBYUP_EDIT, ftemp);

	ftemp = adomanageFind.m_pRecordSet->GetCollect("StandbyDown");
	m_DBStandbyDownValue = ftemp;
	//SetDlgItemInt(IDC_DBSTANDBYDOWN_EDIT, ftemp);

	ftemp = adomanageFind.m_pRecordSet->GetCollect("SleepUP");
	m_DBSleepUpValue = ftemp;
	//SetDlgItemInt(IDC_DBSLEEPUP_EDIT, ftemp);

	ftemp = adomanageFind.m_pRecordSet->GetCollect("SleepDown");
	m_DBSleepDownValue = ftemp;
	//SetDlgItemInt(IDC_DBSLEEPDOWN_EDIT, ftemp);
	UpdateData(FALSE);


	//获取各种指令和回复
	strtemp = adomanageFind.m_pRecordSet->GetCollect("TestCommand").bstrVal;
	SetDlgItemText(IDC_TESTCOMMAND_EDIT, strtemp);

	strtemp = adomanageFind.m_pRecordSet->GetCollect("TestCommandReply").bstrVal;
	SetDlgItemText(IDC_TESTCOMMANDREPLY_EDIT, strtemp);

	strtemp = adomanageFind.m_pRecordSet->GetCollect("RidCommand").bstrVal;
	SetDlgItemText(IDC_RIDCOMMAND_EDIT, strtemp);

	strtemp = adomanageFind.m_pRecordSet->GetCollect("RidCommandReply").bstrVal;
	SetDlgItemText(IDC_RIDCOMMANDREPLY_EDIT, strtemp);

	strtemp = adomanageFind.m_pRecordSet->GetCollect("StandbyCommand").bstrVal;
	SetDlgItemText(IDC_STANDBYCOMMAND_EDIT, strtemp);

	strtemp = adomanageFind.m_pRecordSet->GetCollect("StandbyCommandReply").bstrVal;
	SetDlgItemText(IDC_STANDBYCOMMANDREPLY_EDIT, strtemp);

	strtemp = adomanageFind.m_pRecordSet->GetCollect("SleepCommand").bstrVal;
	SetDlgItemText(IDC_SLEEPCOMMAND_EDIT, strtemp);

	strtemp = adomanageFind.m_pRecordSet->GetCollect("SleepCommandReply").bstrVal;
	SetDlgItemText(IDC_SLEEPCOMMANDREPLY_EDIT, strtemp);

	//获取指令发送次数和间隔
	itemp = adomanageFind.m_pRecordSet->GetCollect("Count");
	SetDlgItemInt(IDC_COMMANDCOUNT_EDIT, itemp);

	itemp = adomanageFind.m_pRecordSet->GetCollect("WriteTime");
	SetDlgItemInt(IDC_COMMANDSENDINTERVAL_EDIT, itemp);

	itemp = adomanageFind.m_pRecordSet->GetCollect("ReadTime");
	SetDlgItemInt(IDC_COMMANDREADINTERVAL_EDIT, itemp);

	adomanageFind.CloseAll();
}

//初始化机型下拉框数据库操作
void CModelConfig::InitModelDBOperation()
{
	m_SelectModelCombo.ResetContent();
	ADOManage adomanageCombo;
	adomanageCombo.ConnSQL();
	adomanageCombo.m_pRecordSet = adomanageCombo.CheckAllInConfigSql();
	while (!adomanageCombo.m_pRecordSet->adoEOF)
	{
		m_SelectModelCombo.AddString(adomanageCombo.m_pRecordSet->GetCollect("ModelName").bstrVal);
		adomanageCombo.m_pRecordSet->MoveNext();
	}
	adomanageCombo.CloseAll();
}


//鼠标焦点离开机型下拉框后的操作
void CModelConfig::OnCbnKillfocusSelectmodelCombo()
{
	// TODO:  在此添加控件通知处理程序代码

	//ConfigVauleEnableWindows(TRUE);
}

//输入机型按回车之后的操作
void CModelConfig::OnCbnSelendokSelectmodelCombo()
{
	// TODO:  在此添加控件通知处理程序代码

	//ConfigVauleEnableWindows(TRUE);
}

//选择机型之后的操作
void CModelConfig::OnCbnSelchangeSelectmodelCombo()
{
	// TODO:  在此添加控件通知处理程序代码
	FindModelDBOperation();
	ConfigVauleEnableWindows(TRUE);
}


/*其它函数*/
//点击确定按钮
void CModelConfig::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}

//点击退出按钮
void CModelConfig::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	INT_PTR nRes;
	nRes = MessageBox(_T("您确定要退出吗？"), _T("提示"), MB_OKCANCEL | MB_ICONQUESTION);
	// 判断消息对话框返回值。如果为IDCANCEL就return，否则继续向下执行   
	if (IDCANCEL == nRes)
		return;

	CDialogEx::OnCancel();
}


