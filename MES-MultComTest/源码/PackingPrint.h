#pragma once

#include "bartender.h"
#include "WriteIMEIDlg.h"

//判断是否存在文件夹
#include "io.h"
// CPackingPrint 对话框
#define MAX_ORDER_NUM 64

class CPackingPrint : public CDialog
{
	DECLARE_DYNAMIC(CPackingPrint)

public:
	CPackingPrint(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPackingPrint();

// 对话框数据
	enum { IDD = IDD_PACKING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

public:
	CString OrderDataName[MAX_ORDER_NUM];
	CString OrderData[MAX_ORDER_NUM];
	CString OrderDataRerurn[MAX_ORDER_NUM];
public:
	IBtApplication		btApp;														//DBtApplication与IBtApplication都有quit函数
	IBtFormat			btFormat;
	IBtFormats*			btFormats;
public:
	void  SaveConfig();
	void  LoadConfig();
	BOOL GetATSetting(CString SettingChar);
	void PrintFun(CString IMEIData,CString PhoneNum);//打印功能
	void PrintFunIni();//打印功能
	void PrintFunEnd();//打印功能
public:
	afx_msg void OnBnClickedButton2();//重新打印
	afx_msg void OnBnClickedButtonstart1();
	// Bartender模板文件路径
	CString BtwFile;
	// 打印数据设置
	CString PrintSetting;
	afx_msg void OnBnClickedButton3();
};
