
// MFCP1LDDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "AutoCombox.h"
#include "afxcmn.h"
#include "MarkEzdDll.h"
#include "LDStartDlg.h"

//读镭雕机串口
UINT ThreadReadLdPort(LPVOID pParam); 

// CMFCP1LDDlg 对话框
class CMFCP1LDDlg : public CDialogEx
{
private:
	CLDStartDlg *m_pCLDStartDlg;

// 构造
public:
	CMFCP1LDDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CMFCP1LDDlg();

// 对话框数据
	enum { IDD = IDD_MFCP1LD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CWinThread* LdReadPortThread;//线程句柄

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	//控制窗体中控件获得使能和失去使能的函数
	void InitInformationwindowtrue();
	void CRLDInformationwindowtrue();
	void LDSystemInformationwindowtrue();
	void InitInformationwindowfalse();
	void CRLDInformationwindowfalse();
	void LDSystemInformationwindowfalse();


	//获取本机信息变量和函数
	CString m_pcnameEdit;
	CString m_pcipEdit;

	int GetLocalHostIPName(CString &sLocalName, CString &sIpAddress);


	//初始化配置模块变量以及函数
	CAutoCombox m_zhidanautoCombo;
	CString ZhiDanNO;//制单号
	CEdit m_imeistartEdit;
	CEdit m_imeiendEdit;
	CEdit m_imeistartshowonlyEdit;
	CEdit m_imeiendshowonlyEdit;

	void InitComboBox();//制单号初始化
	void GetImeiByZhidan();//根据制单号获取IMEI和模版路径
	void ZhiDanControl();//控制选择订单号后的软件行为
	BOOL JudgeZhidan(CString zhidan);//判断订单号是否存在

	afx_msg void OnBnClickedDbconfigButton();
	afx_msg void OnBnClickedSelectmodleButton();
	afx_msg void OnBnClickedOpenmodleButton();
	afx_msg void OnBnClickedUpdatezhidanButton();
	afx_msg void OnCbnKillfocusZhidanautoCombo();
	afx_msg void OnCbnSelchangeZhidanautoCombo();


	//镭雕模式选择模块变量以及函数
	BOOL radioflag;//镭雕模式的标志位

	afx_msg void OnBnClickedImeiCheck();
	afx_msg void OnBnClickedNormolldRadio();
	afx_msg void OnBnClickedReldRadio();


	//当前镭雕信息模块变量以及函数
	CString IMEIStaStart,IMEIStaEnd;//IMEI号段范围的变量
	CString IMEIStaCRStart,IMEIStaCREnd;//镭雕时IMEI的变量

	void GetDataNumber();//获取数据量

	afx_msg void OnBnClickedOpenldsystemButton();
	afx_msg void OnBnClickedReadcurrentimeiButton();
	afx_msg void OnBnClickedReadimeiButton();
	afx_msg void OnEnKillfocusImeistartEdit();
	afx_msg void OnEnKillfocusImeiendEdit();


	//镭雕系统模块变量以及函数
	MarkEzdDll markezd;	//声明镭雕类对象
	CString DllStr;//DLL的返回值转换成CString
	int DllFlag;//DLL的返回值
	int STEPLDFlag;//脚踏或旋转打标模式标志位
	int ScanFlag;//扫描枪模式标志位
	int IMEI15Flag;//校验位标志位
	WORD LdPort;//镭雕串口变量
	CString LdVariableChar[8];//镭雕的字段放这里
	int LdVariableInt[8], LdVariableCount;//镭雕的字段对应的位放这里

	CString CreateIMEI15(CString imei);//生成IMEI校验位
	int FindLdName();//寻找镭雕对象名称
	int ChangeLdName(CString LdVariable, CString strld);//替换指定对象名称的值
	int LdCore();//开始镭雕函数
	void InsertData(CString strimei);//正常\重新镭雕插入更新操作集成
	void ScanInsertData(CString strimei);//扫描枪的数据库插入操作
	void WindowTimeClose(CString str,int strtime);//窗口在N毫秒后自动关闭

	afx_msg void OnBnClickedCloseldsystemButton();
	afx_msg void OnBnClickedLdoneceButton();
	afx_msg void OnBnClickedStepldButton();
	afx_msg void OnBnClickedScanmodeButton();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//信息日志模块变量以及函数
	CRichEditCtrl m_currentRichedit;

	void SetRicheditText(CString strMsg, int No);//根据值来在信息日志中显示不同的信息
	CString GetTime();//获取当前系统时间


	//数据库查询模块变量以及函数
	CListCtrl m_dbcurtainList;

	void ShowAllData();//显示已插入的数据
	void ShowRepeatData();//显示重复的数据
	void ShowUnData();//显示未镭雕的数据

	afx_msg void OnBnClickedSelectalldataButton();
	afx_msg void OnBnClickedSelectrepetdataButton();
	afx_msg void OnBnClickedSelectmissingdataButton();
	afx_msg void OnBnClickedParityCheck();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedSetimeirangeButton();
};
