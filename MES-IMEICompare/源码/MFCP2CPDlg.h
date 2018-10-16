
// MFCP2CPDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "AutoCombox.h"
#include <map>
#include "Resource.h"

using namespace std;


/*全局变量*/
extern BOOL Imei3Flag;

//extern int SyllableArray[8];
//extern int BindArray[6];

//extern map<int, CString>SyllableMap;
//extern map<int, CString>BindMap;

// CMFCP2CPDlg 对话框
class CMFCP2CPDlg : public CDialogEx
{
	// 构造
public:
	CMFCP2CPDlg(CWnd* pParent = NULL);	// 标准构造函数

	// 对话框数据
	enum { IDD = IDD_MFCP2CP_DIALOG };

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

	//控件变量
public:
	CStatic m_hintStatic;
	CStatic m_imei2Static;
	CStatic m_imei1Static;
	CString m_imei1Edit;
	CString m_imei2Edit;
	CString m_imei3Edit;
	CButton m_OpenImei3EditCheck;
	CButton m_couplingCheck;
	CButton m_writenumCheck;
	CButton m_downloadCheck;
	CButton m_funtestCheck;
	BOOL LockFlag;//锁定标志位

	void RelationEnableWindow(BOOL chose);//使字段和绑定控件使能
	void ChImei3EnableWindow(BOOL chose);//使不判断彩盒和开启IMEI编辑框控件使能
	void OtherEnableWindow(BOOL chose);//使订单配置、不判断彩盒、数据库配置控件使能
	void ImeiInputEnableWindow(BOOL chose);//使IMEI输入框控件使能
	void SetEditEmpty();//将编辑框置为空
	void CleanImei3Check();//清除IMEI复选框

	//字体变量与函数操作
public:
	CFont editfont;
	CFont staticfont1;
	CFont staticfont2;
	CFont checkfont;
	COLORREF clrcolor;
	void fontinit();//初始化字体

	//字段选择模块
	CButton m_imeiSyllableCheck;
	CButton m_snSyllableCheck;
	CButton m_simSyllableCheck;
	CButton m_vipSyllableCheck;
	CButton m_iccidSyllableCheck;
	CButton m_batSyllableCheck;
	CButton m_macSyllableCheck;
	CButton m_equipmentSyllableCheck;
	CButton m_rfidSyllableCheck;

	afx_msg void OnBnClickedSavesyllableButton();
	afx_msg void OnBnClickedReadsyllableButton();
	afx_msg void OnBnClickedImeisyllableCheck();
	afx_msg void OnBnClickedSnsyllableCheck();
	afx_msg void OnBnClickedVipsyllableCheck();
	afx_msg void OnBnClickedSimsyllableCheck();
	afx_msg void OnBnClickedBatsyllableCheck();
	afx_msg void OnBnClickedIccidsyllableCheck();
	afx_msg void OnBnClickedEquipmentsyllableCheck();
	afx_msg void OnBnClickedMacsyllableCheck();
	afx_msg void OnBnClickedRfidsyllableCheck();

	void CleanSyllableCheck();//清除字段选择复选框

	//绑定选择模块
	CButton m_simBindCheck;
	CButton m_vipBindCheck;
	CButton m_iccidBindCheck;
	CButton m_batBindCheck;
	CButton m_macBindCheck;
	CButton m_equipmentBindCheck;
	CButton m_rfidBindCheck;

	afx_msg void OnBnClickedSavebindButton();
	afx_msg void OnBnClickedReadbindButton();
	afx_msg void OnBnClickedSimbindCheck();
	afx_msg void OnBnClickedVipbindCheck();
	afx_msg void OnBnClickedBatbindCheck();
	afx_msg void OnBnClickedIccidbindCheck();
	afx_msg void OnBnClickedMacbindCheck();
	afx_msg void OnBnClickedEquipmentbindCheck();
	afx_msg void OnBnClickedRfidbindCheck();

	void CleanBindCheck();//清除绑定复选框

	static map<int, CString>SyllableMap;//存放用户所选字段
	static map<int, CString>BindMap;//存放用户所选绑定

	//编辑框操作
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL IsNumber(const CString& strTest);//判断数字和字母
	BOOL IsNumber2(const CString& strTest);//只判断数字
	afx_msg void OnEnSetfocusImei2Edit();

	//其它变量
	CString strpcip, strpcname;//主机名称和ip
	int notype;//代表目前要扫的类型
	CString notypename[8];//里面放着IMEI、VIP等字符串 注意，notypename目前因为需求更改而被废弃掉，但因为它属于核心逻辑的东西，不好随意改，目前不会对程序有影响，所以暂时保留。
	CString strno1,strno2,strno3;//存放其它扫描类型的值，比如选中SN时，里面放着的就是SN号
	CString strzhidan;//存放当前选中的制单号
	int chjudgeflag;//彩盒复选框的标志
	CString strimeistart, strimeiend;//IMEI起始和结束


	//数据库操作模块
	CString SyllablestrSql;

	void InitComboBox();//初始化制单号下拉框
	BOOL JudgeZhidan(CString zhidan);//判断制单号
	void Savesyllable();//保存字段选择
	void Readsyllable(BOOL CheckEx);//读取字段选择
	void Savebind();//保存绑定选择
	void Readbind(BOOL CheckEx);//读取绑定选择
	void Saveconfig();//保存一些配置
	void Readconfig();//读取一些配置


    //其它函数
	CString GetTime();//获取当前时间
	int GetLocalHostIPName(CString &sLocalName, CString &sIpAddress);//获取IP地址
	afx_msg void OnBnClickedOk();
	void readimei();
	BOOL judgeimeirang(CString str, CString strimeistart, CString strimeiend);

	//以下是控件按钮消息函数

    //数据库配置按钮
	afx_msg void OnBnClickedDbconfigButton();

	//更新按钮
	afx_msg void OnBnClickedUpdateordernumberButton();

	//单选框按钮
	afx_msg void OnBnClickedImeiRadio();
	afx_msg void OnBnClickedSnRadio();
	afx_msg void OnBnClickedSimRadio();
	afx_msg void OnBnClickedVipRadio();
	afx_msg void OnBnClickedIccidRadio();
	afx_msg void OnBnClickedBatRadio();
	afx_msg void OnBnClickedMacRadio();
	afx_msg void OnBnClickedEquipmentRadio();

	//自定义下拉框以及相关的消息函数
	CAutoCombox m_zhidanCombo;
	afx_msg void OnCbnSelchangeZhidanCombo();
	afx_msg void OnCbnSelendokZhidanCombo();
	afx_msg void OnCbnKillfocusZhidanCombo();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CButton m_caihejudgeCheck;
	afx_msg void OnBnClickedChjudgeCheck();

	afx_msg void OnBnClickedLockButton();
	afx_msg void OnBnClickedOpenimei3editCheck();

	afx_msg void OnEnSetfocusImei1Edit();
};
