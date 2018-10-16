#pragma once

#import "msado15.dll"  no_namespace rename("EOF", "adoEOF")


class ADOManage
{
public:
	ADOManage();
	~ADOManage();

	//自定义参数
public:
	_ConnectionPtr m_pConnection;    //连接参数
	_CommandPtr m_pCommand;          //SQL命令参数
	_RecordsetPtr m_pRecordSet;      //记录集参数

	//自定义函数
public:
	bool ConnSQL();//连接数据库函数
	bool ConndbSQL();//连接数据库函数2,数据库配置那里用来测试连接用
	bool CloseAll();//关闭数据库函数



	int CpImeiByNo(CString Syllablesqlstr, CString noname, CString no, CString strzhidan);//根据其它号段寻找IMEI
	int CpCaiheByImei(CString imei, CString ZhiDan);//根据IMEI寻找彩盒时间
	int InsertCorrectImei(CString zhidan, CString imei1, CString imei2,CString imei3,CString no1, CString no2, CString ip, CString notype, CString errorreason, CString result);//插入正确数据到表中
	int InsertWrongImei(CString zhidan, CString imei1, CString imei2, CString imei3, CString no1, CString no2, CString ip, CString notype, CString errorreason, CString result);//插入错误数据到表中
	int JudgeImei(CString imei);//判断IMEI号是否存在
	int JudgeZhidan(CString imei,CString Zhidan);//判断制单号是否对得上
	void Savesyllable(CString order, int IMEI, int SN, int SIM, int VIP, int ICCID, int BAT, int MAC, int Equipment,int RFID);//保存订单所选择的字段
	void Readsyllable(CString order);//读取订单所选择的字段
	void Savebind(CString order, int SIM, int VIP, int ICCID, int BAT, int MAC, int Equipment, int RFID);//保存订单所绑定的字段
	void Readbind(CString order);//读取订单所绑定的字段
	void SaveConfig(CString order, int imei3Flag);//保存一些配置
	void ReadConfig(CString order);//读取一些配置
	_RecordsetPtr GetOrderNumber();//获取订单号
	_RecordsetPtr GetIMEIByOrderNumber(CString ordernumber);//根据订单号获取IMEI号
	_RecordsetPtr JudgeOrderNumber(CString ordernumber);//判断订单号是否存在

	CString ADOManage::GetTime();//获取当前系统时间


	//目前还未使用的函数
	_RecordsetPtr GetRst();

	//获取数据库信息，当我们连接数据库的时候就先获取这些数据再连接
public:
	CString m_DataSource;  //服务器IP
	CString m_Port;        //服务器端口
	CString m_UserName;    //用户名
	CString m_Password;    //密码
	CString m_Firstdbname;    //镭雕数据库名称
	CString m_Seconddbname;  //订单数据库名称
	CString m_Firstformname;  //镭雕数据表名称
	CString m_Secondformname;//订单数据表名称
	void GetDBINFO();//获取数据库信息函数

	//去掉字符串头尾
	void ADOManage::trim(CString &str);

	CString CreateIMEI15(CString imei);
};

