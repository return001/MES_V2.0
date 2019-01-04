#pragma once

#define     MAXPARANUM       100

#include "ReportCtrl.h"
//#include "afxwin.h"
// CPara1 对话框
typedef struct
{
    CString paraID;     //指令唯一的ID号
    CString paraContent;//对应的指令内容

	bool     ifCheck;//是否选中
	CString showName;         //显示在列表中的参数名
    CString Low_Limit_Value;  //期望下限值
	CString High_Limit_Value; //期望上限值
	CString Other_ITEM;       //其他自定义项
}PARAMETER;

class CPara1 : public CDialog
{
	DECLARE_DYNAMIC(CPara1)

public:
	CPara1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPara1();

// 对话框数据
	enum { IDD = IDD_CSR_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRvnItemClick(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
public:
	CReportCtrl m_list_area_para;
//private:
	int numParameter;//参数数量
	CString fSectionName; // ini文件分段名
	PARAMETER parameterArray[MAXPARANUM];//存储参数 的数组
public:
	void ImportListData(int paraNum,PARAMETER paraArray[]);
	int LoadIniFile(CString strLoadFile,PARAMETER paraArray[]);
	void LoadParaList(CString strLoadFile);//装载测试项相关设置


	void SaveBLE_Setting(CString strLoadFile);//保存设置
	int  ExportListData(void);//(PARAMETER exportArray[]);	
	void SaveIniFile(CString strSaveFile,int paraNum,PARAMETER paraArray[]);
};

