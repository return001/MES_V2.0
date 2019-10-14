
// JIMIDlg.h : 头文件
//

#pragma once
#include "ADOManage.h"
#include "DBConfig.h"
#include "UserLogin.h"
#include "AutoCombox.h"
#include "NewButton.h"
#include <map>

#include "afxwin.h"

#include <sapi.h>//语音添加
#include <sphelper.h>//语音添加
#pragma comment(lib,"sapi.lib") //语音添加

using namespace std;


/*全局变量*/
extern BOOL Imei3Flag;

// CJIMIDlg 对话框
class CJIMIDlg : public CDialogEx
{
	// 构造
public:
	CJIMIDlg(CWnd* pParent = NULL);	// 标准构造函数

									// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JIMI_DIALOG };
#endif

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

	//定义要使用类的对象
public:
	ADOManage ado_SQL;

	//串口模块
	HANDLE port1handler;
	HANDLE InitCom(CString comName);//初始化串口
	CString sComPort;
	int CloseCom(HANDLE handler);//关闭串口
	void GetCommPort(CComboBox *pComboBox, CString &ComNo);//获取当前串口号
	void FindCommPort(CComboBox *pComboBox, CString &ComNo, int PortNO);//从注册表中寻找串口号
	int GetLocalHostIPName(CString &sLocalName, CString &sIpAddress);//获取IP地址

	CWinThread * pComThread;	
	CWinThread * pWeightThread;//称重线程
	CWinThread * pVoiceThread;//称重线程
	static UINT ComRxThread(LPVOID pParam);//串口接收数据线程
	static UINT GetWeightThread(LPVOID pParam);//重量计算线程
	static UINT VoiceThread(LPVOID pParam);//重量计算线程
	BOOL Process();
	void GetWeightDEAL();//重量结果处理
	bool bThreadEnd;//判断线城结束的变量
	OVERLAPPED m_osRead; //用于读取控制
	OVERLAPPED m_osWrite; //用于发送控制

	void FontInit();//初始化字体
	CFont Font1;
	CFont Font2;
	CFont Font3; 
	CFont Font4;
	CFont Font5;

	//系统语音库;
	ISpVoice * pVoice;
	bool IniVoice();
	HRESULT hr;
	//运行日志
	void AddToRunList(CString str);
	void AddToErrorList(CString str);
	void PlayVoice(CString str);

	afx_msg void OnBnClickedBtnDbconfig();
	afx_msg void OnBnClickedOk();
	CListBox m_LIST_Run;
	CListBox m_LIST_Warning;

protected:
	afx_msg LRESULT OnRunlog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnVoice(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMySetFocus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReadweighportret(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedBtnClearerror();
	CComboBox c_PortCom;
	afx_msg void OnDropdownComboCom();

	afx_msg void OnSelchangeZhidanCombo();

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
	CButton m_imei2SyllableCheck;

	//绑定选择模块
	CButton m_simBindCheck;
	CButton m_vipBindCheck;
	CButton m_iccidBindCheck;
	CButton m_batBindCheck;
	CButton m_macBindCheck;
	CButton m_equipmentBindCheck;
	CButton m_rfidBindCheck;

public:
	bool readimei();////根据订单号获取IMEI起始号和结束号
	CAutoCombox m_zhidanCombo;

	static map<int, CString>SyllableMap;//存放用户所选字段
	static map<int, CString>BindMap;//存放用户所选绑定

									//								//编辑框操作
									//virtual BOOL PreTranslateMessage(MSG* pMsg);
									//BOOL IsNumber(const CString& strTest);//判断数字和字母
									//BOOL IsNumber2(const CString& strTest);//只判断数字
									//afx_msg void OnEnSetfocusImei2Edit();

									//其它变量
	CString strpcip, strpcname;//主机名称和ip
	int notype;//代表目前要扫的类型
	CString notypename[9];//里面放着IMEI、VIP等字符串 注意，notypename目前因为需求更改而被废弃掉，但因为它属于核心逻辑的东西，不好随意改，目前不会对程序有影响，所以暂时保留。
	CString strno1, strno2, strno3;//存放其它扫描类型的值，比如选中SN时，里面放着的就是SN号
	CString strzhidan;//存放当前选中的制单号

	CString strimeistart, strimeiend;//IMEI起始和结束

	afx_msg void OnBnClickedButtonUpdataorder();
	void InitComboBox();//给combox添加数据库中的订单号，更新按钮和开启后自动初始化都用这个函数
	void SetEditEmpty();//将编辑框置为空
	void RelationEnableWindow(BOOL chose);//使字段和绑定控件使能
	void ChImei3EnableWindow(BOOL chose);//使不判断彩盒和开启IMEI编辑框控件使能
	void ImeiInputEnableWindow(BOOL chose);//使IMEI输入框控件使能
	void CleanSyllableCheck();//清除字段选择复选框
	void CleanBindCheck();//清除绑定复选框
	void CleanImei3Check();//清除IMEI复选框

						   //数据库操作模块
	CString SyllablestrSql;

	BOOL JudgeZhidan(CString zhidan);//判断制单号
	void Savesyllable();//保存字段选择
	void Readsyllable(BOOL CheckEx);//读取字段选择
	void Savebind();//保存绑定选择
	void Readbind(BOOL CheckEx);//读取绑定选择
	void Saveconfig();//保存一些配置
	void Readconfig();//读取一些配置

	BOOL LockFlag;//锁定标志位
	void OtherEnableWindow(BOOL chose);//使订单配置、不判断彩盒、数据库配置控件使能

	afx_msg void OnBnClickedButtonComok2();
	afx_msg void OnKillfocusZhidanCombo();
	afx_msg void OnSelendokZhidanCombo();
	afx_msg void OnBnClickedSimsyllableCheck();
	afx_msg void OnBnClickedImeisyllableCheck();
	afx_msg void OnBnClickedSnsyllableCheck();
	afx_msg void OnBnClickedVipsyllableCheck();
	afx_msg void OnBnClickedIccidsyllableCheck();
	afx_msg void OnBnClickedBatsyllableCheck();
	afx_msg void OnBnClickedMacsyllableCheck();
	afx_msg void OnBnClickedEquipmentsyllableCheck();
	afx_msg void OnBnClickedRfidsyllableCheck();
	afx_msg void OnBnClickedSavesyllableButton();
	afx_msg void OnBnClickedReadsyllableButton();

	afx_msg void OnBnClickedSimbindCheck();
	afx_msg void OnBnClickedVipbindCheck();
	afx_msg void OnBnClickedBatbindCheck();
	afx_msg void OnBnClickedIccidbindCheck();
	afx_msg void OnBnClickedMacbindCheck();
	afx_msg void OnBnClickedEquipmentbindCheck();
	afx_msg void OnBnClickedRfidbindCheck();
	afx_msg void OnBnClickedSavebindButton();
	afx_msg void OnBnClickedReadbindButton();
	afx_msg void OnBnClickedOpenimei3editCheck();
	CButton m_OpenImei3EditCheck;
	//CNewButton m_StateView;
	//CNewButton m_OKView;
	//CNewButton m_OverView;
	//CNewButton m_NullView;
	//CNewButton m_DownView;
	//////////////////要传输给数据的数据////////////////
	CString m_disDownLimit;
	CString m_disUpLimit;
	CString m_DownLimit;
	CString m_UpLimit;
	CString m_PCIP;
	CString m_PCName;
	CString m_UserName;
	CString m_UserRight;//用户权限

	CString m_WeightValue;//最终称重的结果值
	CString m_IMEIValue;//最终称重的IMEI结果值
	CString m_OKValue;//最终称重的判断结果值
	CString sResult;//做转换用
	int iValResult;//0正常，1、偏轻；2、偏重；3、其它

				   //////////////////要传输给数据的数据////////////////

	CString m_oldWeightValue;//过程用来计算的
	CString m_newWeightValue;//过程用来计算的

	afx_msg void OnBnClickedLockButton();
	CString m_imei3Edit;
	CString m_imei2Edit;
	CString m_imei1Edit;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL IsNumber(const CString& strTest);//判断数字和字母
	BOOL IsNumber2(const CString& strTest);//只判断数字
	BOOL judgeimeirang(CString str, CString strimeistart, CString strimeiend);
	void RecordResult(CString sRemark);//记录结果进数据库
	void ToResultSheet();
	bool GetWeightValue();//计算处理有效的重量值，赋值相应的值结果给数据库
	void DealtSerialData(CString sSerialData);//获取当前的串口返回的有效的重量值
	bool CheckParam();//检查设置的参数
	void WidgetStatue(BOOL Show);//根据权限问题的显示状态

	afx_msg void OnBnClickedImei2syllableCheck();
	afx_msg void OnBnClickedButtonLogin();
};

