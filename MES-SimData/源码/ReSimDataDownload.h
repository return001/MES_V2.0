#pragma once
#include "afxwin.h"
#include "afxcmn.h"

extern CString strFolderpath, strOKFolderpath, strFolderFile,m_resimdatafolderPath;
extern HWND MainReFormHWND;
extern HANDLE reporthandler;
extern CString reComNo;
extern int simrestratflag;

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
	afx_msg void OnBnClickedReopensimdatafolderpathButton();
	afx_msg void OnBnClickedReport1connectButton();
	afx_msg void OnBnClickedRestart1Button();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnDropdownReportlist1Combo();
	
	CComboBox m_report1list1Combo;
	CMFCP3SIMPORTDlg *parentdlg = (CMFCP3SIMPORTDlg *)GetParent();

	int simreconnectflag;

	void SetRePortEditEmpty();

	CFont staticReHintfont;
	COLORREF clrcolor;
	void fontinit();//初始化字体
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	//返工位线程
	afx_msg LRESULT MainRePortThreadControl(WPARAM wParam, LPARAM lParam);

	void ReDownloadMainThread(LPVOID lpParam);
	void ReDownloadWrite1PortThread(LPVOID lpParam);
	void ReDownloadWrite2PortThread(LPVOID lpParam);
	void ReDownloadRead1PortThread(LPVOID lpParam);
	void ReDownloadRead2PortThread(LPVOID lpParam);

	void ReDownloadRestPortThread();
	void ReDownloadClosePortThread();

	CTask ThreadControl;//线程池的东西
	CThreadPool* m_lpThreadPool;//线程池的东西

	void OpenThreadPoolTask(int Command);

	//数据库函数
	int SimDataReFun();

	afx_msg LRESULT MainReDataInsertControl(WPARAM wParam, LPARAM lParam);//数据库消息循环函数

	void SetRicheditText(CString strMsg, int No);
	CString GetTime();

	CRichEditCtrl m_recurrentinformationRichedit;
};
