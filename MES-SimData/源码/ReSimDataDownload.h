#pragma once
#include "afxwin.h"
#include "afxcmn.h"

UINT ReDownloadMainThread(LPVOID lpParam);//串口主控线程

UINT ReDownloadWirtePortThread(LPVOID lpParam);//串口写线程
UINT ReDownloadReadPortThread(LPVOID lpParam);//串口读线程

//extern CString LastReRID;
//extern CString LastReIMEI;
extern BOOL RePortAbnomal;
extern CString strFolderpath, strOKFolderpath, strFolderFile,m_resimdatafolderPath;//各种全局路径
extern HWND MainReFormHWND;//主线程句柄
extern HANDLE reporthandler;//串口句柄
extern CString reComNo;//串口号
extern int simrestratflag;//开始下载标志位
extern CString ReLogName;//本地日志文件名

// CReSimDataDownload 对话框

class CReSimDataDownload : public CDialogEx
{
	DECLARE_DYNAMIC(CReSimDataDownload)

public:
	CReSimDataDownload(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CReSimDataDownload();

// 对话框数据
	enum { IDD = IDD_RESIMDATA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CMFCP3SIMPORTDlg *parentdlg = (CMFCP3SIMPORTDlg *)GetParent();//获取父窗口指针

	//初始化模块函数以及变量
	CComboBox m_report1list1Combo;

	afx_msg void OnBnClickedReopensimdatafolderpathButton();
	afx_msg void OnCbnDropdownReportlist1Combo();
	afx_msg void OnBnClickedCancel();

	
	//返工模块函数以及变量
	int simreconnectflag;
	int CommandNo;
	CWinThread* DWThread;//下载写线程句柄
	CWinThread* DRThread;//下载写线程句柄
	CWinThread* MainThread;//下载主控线程句柄

	CString CommandWriteUnit(int strcommandNo);//写指令
	CString CommandReadUnit(int strcommandNo);//读指令
	void ReDownloadRestPortThread();//重置线程变量
	void ReDownloadClosePortThread();//关闭线程变量
	void SetRePortEditEmpty();//清除窗口

	afx_msg void OnBnClickedReport1connectButton();
	afx_msg void OnBnClickedRestart1Button();


	//本地日志以及信息日志相关的函数以及变量
	CRichEditCtrl m_recurrentinformationRichedit;

	void SetRicheditText(CString strMsg, int No);//信息日志
	void PrintReLog(CString strMsg);//本地日志
	CString GetTime();
	CString GetLogTime();


	//字体函数以及变量
	CFont staticReHintfont;
	COLORREF clrcolor;

	void fontinit();//初始化字体

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	
	//afx_msg LRESULT MainRePortThreadControl(WPARAM wParam, LPARAM lParam);
	//void ReDownloadWrite1PortThread(LPVOID lpParam);
	//void ReDownloadWrite2PortThread(LPVOID lpParam);
	//void ReDownloadRead1PortThread(LPVOID lpParam);
	//void ReDownloadRead2PortThread(LPVOID lpParam);
	//CTask ThreadControl;//线程池的东西
	//CThreadPool* m_lpThreadPool;//线程池的东西

	//void OpenThreadPoolTask(int Command);


};
