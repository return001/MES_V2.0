
// MFCP6ComToolDlg.h : 头文件
//

#pragma once
#include "RDAHostInterface.h"
#include "Log.h"
#include "afxwin.h"
#include "afxbutton.h"
#include "HistogramDlg.h"
#include "afxcmn.h"

// CMFCP6ComToolDlg 对话框
class CMFCP6ComToolDlg : public CDialogEx
{
// 构造
public:
	CMFCP6ComToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCP6COMTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton m_MtkPortRadioControl;
	CButton m_SendOrderControl;
	CButton m_ListLoopSendControl;
	CButton m_RdaPortRadioControl;
	BOOL m_HexDisplayCheckValue;
	BOOL m_HexSendCheckValue;
	BOOL m_IsAddTimeCheckValue;
	int m_AutoSendTimeValue;
	BOOL m_AutoSendCheckValue;
	CButton m_AutoSendCheckControl;
	//int m_ListControlDelayTimeiValue;
	CEdit m_ListControlDelayTimeControl;
	CComboBox m_PortNoComboControl;
	CComboBox m_PortBaudComboControl;
	int m_PortRadioValue;
	BOOL m_WithEnterCheckValue;
	BOOL m_OutputLocalLogCheckValue;
	CFont fn;
	RDAHostInterface  RdaHostInterface;

	BOOL ListloopsendFlag;//顺序发送标志
	BOOL PortIsOpenFlag;//串口开关按钮标志
	CString PortNo;//串口名称变量
	CString PortBaud;//串口波特率
	HANDLE Porthandler;//串口句柄

	BOOL InitPort();//初始化串口集成函数
	HANDLE InitCom(CString comName);//初始化串口
	BOOL CloseCom(HANDLE handler);//关闭串口
	BOOL GetCommPort(CComboBox *pComboBox, CComboBox *pBaudBox);//获取当前串口号和波特率
	void FindCommPort(CComboBox *pComboBox, CString &ComNo, int PortNO);//从注册表中寻找串口号
	void SendAndReceiveComOrder(HANDLE PortHandler, CString SendOrder, CString &ReceiveData, CString &IsSuccessFlag);//串口指令收发
	void SendComOrder(HANDLE PortHandler, CString SendOrder);//串口发指令
	void AutoSendComOrder();//定时发送指令
	void ReceiveComOrder(HANDLE PortHandler);//串口收指令
	void EnbleWindowIsOpenPort(BOOL Chooes);//视串口开启与否，禁用与开启相关控件
	void ListLoopSendComOrder();

	void SendComOrderHandleFun(CString PortOrder);//串口发送指令的综合处理函数
	void InitToolSetting();//初始化软件
	void IniLoadConig();//读取INI文件自动保存的信息
	void IniSaveConig();//INI文件自动保存
	BYTE *CStrToByte(int len, CString str);



	//信息日志模块函数以及变量
	CRichEditCtrl m_InformationRicheditControl;
	CRichEditCtrl m_PortOrderRicheditControl;
	void PrintLog(CString strMsg, int No);//本地日志
	void SetRicheditText(CString strMsg, int No);//即时日志
	CString GetTime();
	CString GetLogTime();

	CString LogName;

	/*配置工具模块变量和函数*/
	CListCtrl m_ListSettingControl;
	CString ListHeaderName[8];
	CEdit m_ListControlEdit;
	CComboBox m_ListControlCombo;
	int m_Row, m_Colum;
	CString SettingSrc;

	void InitListSetting(CListCtrl *m_ListControl);//初始化配置列表
	void ListSettingInsert(CListCtrl *m_ListControl);//配置列表插入新行
	void ListSettingUp(CListCtrl *m_ListControl);//配置列表选中行向上移动
	void ListSettingDown(CListCtrl *m_ListControl);//配置列表选中行向下移动
	void ListSettingCopy(CListCtrl *m_ListControl);//配置列表复制选中行
	void ListSettingDelete(CListCtrl *m_ListControl);//配置列表删除选中行
	void ListSettingSave(CListCtrl *m_ListControl);//配置列表保存
	void ListSettingLoad(CListCtrl *m_ListControl);//配置列表读取
	BOOL ListSettingLoadFun(CString SettingSrc);//配置列表读取函数

	afx_msg void OnBnClickedSettinginsertButton();
	afx_msg void OnBnClickedSettingupButton();
	afx_msg void OnBnClickedSettingdownButton();
	afx_msg void OnBnClickedSettingcopyButton();
	afx_msg void OnBnClickedSettingdeleteButton();
	afx_msg void OnBnClickedSettingsaveButton();
	afx_msg void OnNMDblclkBlesettingList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusListcontrolEdit();
	afx_msg void OnCbnKillfocusListcontrolCombo();

	/*GPS模块变量和函数*/
	int m_CurSelTab;
	BOOL m_GPSIsOpenFlag;
	CListCtrl m_GPSDisplayListControl;
	CTabCtrl m_PageControlTabControl;
	CHistogramDlg m_HistogramPage;
	CStatic m_GPSHintControl;

	CString GetData(char* Serial_Order_Return, CString Start, CString End, int Count);
	void InitGPSList(CListCtrl *m_ListControl);//初始化GPS列表
	void InsertGPSList(CListCtrl *m_ListControl, CString GPSIDList[20], CString GPSSNRList[20], int GPSCount);//插入数据到GPS列表中，同时处理柱形图的数据

	void InitTabControl();//初始化Tab模块
	afx_msg void OnTcnSelchangePagecontrolTab(NMHDR *pNMHDR, LRESULT *pResult);//切换选项卡

	/*其它函数*/
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedClearinformationButton();
	afx_msg void OnBnClickedOpenportButton();
	afx_msg void OnBnClickedClearsendorderButton();
	afx_msg void OnBnClickedSendorderButton();
	afx_msg void OnCbnDropdownPortnoCombo();
	afx_msg void OnBnClickedMtkportRadio();
	afx_msg void OnBnClickedWithenterCheck();
	afx_msg void OnBnClickedOutputlocallogCheck();
	afx_msg void OnBnClickedSettingreadButton();
	afx_msg void OnNMClickBlesettingList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedListloopsendButton();
	afx_msg void OnBnClickedAutosendCheck();
	afx_msg void OnBnClickedHexdisplayCheck();
	afx_msg void OnBnClickedHexsendCheck();
	afx_msg void OnBnClickedIsaddtimeCheck();
	afx_msg void OnEnKillfocusListcontroldelaytimeEdit();
	afx_msg void OnNMCustomdrawBlesettingList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusAutosendtimeEdit();
	afx_msg void OnBnClickedIstaidouCheck();
	afx_msg void OnBnClickedSendgpsorderButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	BOOL m_IsTaiDouCheckValue;
	afx_msg void OnBnClickedCleargpslistButton();
	CButton m_SendGpsOrderControl;
};
