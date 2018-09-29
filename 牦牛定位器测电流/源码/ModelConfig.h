#pragma once
#include "afxwin.h"
#include "AutoCombox.h"

// CModelConfig 对话框

class CModelConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CModelConfig)

public:
	CModelConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CModelConfig();

// 对话框数据
	enum { IDD = IDD_MODELCONFIG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	/*控件使能变量和函数*/
	void ConfigVauleEnableWindows(BOOL choose);//机型配置的编辑框使能函数

	/*模块控件变量和函数*/
	CAutoCombox m_SelectModelCombo;

	CEdit m_AddModelControl;
	CString m_AddModelValue;

	CEdit m_TestCommandControl;//测试指令
	CString m_TestCommandValue;
	CEdit m_TestCommandReplyControl;
	CString m_TestCommandReplyValue;

	CEdit m_RidCommandControl;//读RID指令
	CString m_RidCommandValue;
	CEdit m_RidCommandReplyControl;
	CString m_RidCommandReplyValue;

	CEdit m_StandbyCommandControl;//待机指令
	CString m_StandbyCommandValue;
	CEdit m_StandbyCommandReplyControl;
	CString m_StandbyCommandReplyValue;

	CEdit m_SleepCommandControl;//睡眠指令
	CString m_SleepCommandValue;
	CEdit m_SleepCommandReplyControl;
	CString m_SleepCommandReplyValue;

	CEdit m_DBStandbyUpControl;//待机上限
	float m_DBStandbyUpValue;
	CEdit m_DBStandbyDownControl;//待机下限
	float m_DBStandbyDownValue;

	CEdit m_DBSleepUpControl;//睡眠上限
	float m_DBSleepUpValue;
	CEdit m_DBSleepDownControl;//睡眠下限
	float m_DBSleepDownValue;

	CEdit m_CommandCountControl;//指令发送次数
	int m_CommandCountValue;
	CEdit m_CommandSendIntervalControl;//指令发送间隔
	int m_CommandSendIntervalValue;
	CEdit m_CommandReadIntervalControl;//指令读取间隔
	int m_CommandReadIntervalValue;

	void AddModelDBOperation(CString ModelName);//添加机型数据库操作
	void DeleteModelDBOperation(CString ModelName);//删除机型数据库操作
	void SaveModelDBOperation();//保存机型数据库操作
	void FindModelDBOperation();//查找机型数据库操作
	void InitModelDBOperation();//初始化机型下拉框数据库操作

	afx_msg void OnBnClickedAddmodelButton();
	afx_msg void OnBnClickedDeletemodelButton();
	afx_msg void OnBnClickedSavemodelinfoButton();
	afx_msg void OnCbnKillfocusSelectmodelCombo();
	afx_msg void OnCbnSelendokSelectmodelCombo();
	afx_msg void OnCbnSelchangeSelectmodelCombo();


	/*其它变量和函数*/
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
