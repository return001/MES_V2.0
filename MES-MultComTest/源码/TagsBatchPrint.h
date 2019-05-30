#pragma once

//任务管理器
#include "IMEIWrite_MulAT.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ComboListCtrl.h"

#include "winspool.h"

#define PRINTONCE_MAX  20000
// TagsBatchPrint 对话框
class TagsBatchPrint : public CDialog
{
	DECLARE_DYNAMIC(TagsBatchPrint)

public:
	TagsBatchPrint(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TagsBatchPrint();

// 对话框数据
	enum { IDD = IDD_BATCH_PRINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()


public:
	void AutoCreateTagsFunc();    //自动生成IMEI地址
	void AutoCreateTags(LPTSTR lpszTags);
	void AutoCreateTags14(LPTSTR lpszTags);
	__int64 ComputeCD(__int64 nImei);

	BOOL CheckIMEI(CString	m_IMEI);
public:
	// 贴纸起始号
	CString TagsStartNum;
	__int64 m_int64StartTags;
	__int64 m_int64DefaultTags;

	// 贴纸计数
	CString Tags_Count;
	int Tags_Count_int;

	// 当前号
	CString TagsCurrNum;
	CString Tags_ALL;

	// 结束号
	CString TagsEndNum;
	__int64 m_int64EndTags;

public:
	CArray<wininfo,wininfo&> m_wininfoarray;
	wininfo m_currentwin;

	CString Tags_Input[PRINTONCE_MAX];											//一次打印的数据
	byte buf[32];																//当前数据
	CString Tags_LOG_CS;


	CWinThread* Print_Handle;
	
	JOB_INFO_2 *ppJobInfo;														// Pointer to be filled. 
	int pcJobs;																	// Count of jobs filled. 
    DWORD pStatus;																// Print Queue status. 

	HANDLE Current_handle;
	CString PrinterNameCS;

public:
	void Get_App_Running();//获取正在运行的应用 
	void OnWndFocus(BOOL ShowWin,BOOL Print,CString IMEI_Print[PRINTONCE_MAX]);
	void Input_Data(CString IMEI_Print[PRINTONCE_MAX]);
	void Pause_Print();
	CString LoadConfig_Change();

	void LogShow_exchange(CString Msg_Log);

	void GetPrinters();
	BOOL GetJobs(HANDLE hPrinter,			// Handle to the printer.
                JOB_INFO_2 **ppJobInfo,		// Pointer to be filled. 
                int *pcJobs,				// Count of jobs filled. 
                DWORD *pStatus);			// Print Queue status.   

	BOOL TimerFunction();//定时功能
	BOOL PrintSpace(CString Count);//打印空白贴纸
public:
	// 打印空白纸间隔
	CString PrintBlankInterval;
	CEdit TagsCurrNumControl;
	CEdit Tags_CountControl;
	afx_msg void OnBnClickedButtonstart17();

public:
	//优化数据存储、指针调用
	CString *strContents;
	afx_msg void OnBnClickedButtonstart19();
	//CListCtrl Excell_show;
	CComboListCtrl Excell_show;
	CString		m_strExchangeFileName;//规则文件交换路径
	long count_Tags;
	afx_msg void OnBnClickedButton2();
	CEdit Tags_ALL_Control;
	// 贴纸数据的位数
	int TagsBits;
	// 打印速度
	int PrintSpeed;
	afx_msg void OnBnClickedButtonstart15();
	//停止标志
	BOOL StopSign;
	BOOL Sequence;
	afx_msg void OnBnClickedButton21();
	CComboBox PrinterName;
	CEdit PrintStateShow;
	// 选择是否生成IMEI校验位
	BOOL IMEI_CAL;
	afx_msg void OnBnClickedButton22();
};
