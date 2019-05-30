#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDataBase 对话框

class CDataBase : public CDialog
{
	DECLARE_DYNAMIC(CDataBase)

public:
	CDataBase(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDataBase();

// 对话框数据
	enum { IDD = IDD_DATABASE };

private:
	_ConnectionPtr m_pConn;
	_RecordsetPtr  m_pRS;
	CStringArray strArry;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	bool    SQL_Wrong_Excute;

	afx_msg void OnBnClickedButtonstart1();
	BOOL Ado(CString strConn);
	_RecordsetPtr&  GetRS(CString strSQL); //执行strSQL的SQL语句，返回集录集
	CListBox m_list2;


	afx_msg void OnDblclkList2(); //list控件双击事件
	BOOL GetFieldsName(_RecordsetPtr RcdPtr, int nField, CString & strFieldName);   
	int  GetNum(CString strc);
	int  GetFieldsCount(_RecordsetPtr RcdPtr);
	CString m_str_list;
	CString m_str_list_Old;
	// 写号表等字段
	CComboBox Section_Box_IMEIS;
	CString   Section_Box_IMEIS_CS;
	BOOL nSize_Record_Limit_Enable;
	long RECORD;
	CString RecordNumber;
	BOOL Time_Limit_Enable;
	// 具体数据
	CString Number;
	CString Start_Time;
	CString End_Time;
	// 数据显示表格
	CListCtrl m_list1;
	long RECORD_Show;

	//获取字段数
	int dataSize;
	CStringArray *strdataArray;//数据字符串二维数组
	CString *strName;          //字段名字符串
	// 数据库查询
	CButton Connect_DB_For_Find_Control;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton9();
	CButton Disconnect_DB_Control;

	_RecordsetPtr Execute(LPCTSTR lpszSQL, long lOptions);//执行SQL语句
	afx_msg void OnBnClickedButton3();
	// 仅导出IMEI、网标、机型
	BOOL Only_IMEI_NC_Machine;
	// 所有网标数据，不管标志位
	BOOL AllNetCode_Choose;
	// 写网标已经用的标志位
	BOOL Set_WritedFlag;

	BOOL Wrong;
	// 网标导出的标志位值
	CString NETCODE_Export_FlagValue;
	afx_msg void OnBnClickedButton4();
	CButton Clear_FlagValue_Control;
};
