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



	int CpImeiByNo(CString noname, CString no);//根据其它号段寻找IMEI
	int CpCaiheByImei(CString imei);//根据IMEI寻找彩盒时间
	int InsertCorrectImei(CString zhidan, CString imei1, CString imei2, CString no1, CString no2, CString ip, CString notype, CString errorreason, CString result);//插入正确数据到表中
	int InsertWrongImei(CString zhidan, CString imei1, CString imei2, CString no1, CString no2, CString ip, CString notype, CString errorreason, CString result);//插入错误数据到表中
	int JudgeImei(CString imei);//判断IMEI号是否存在
	_RecordsetPtr GetOrderNumber();//获取订单号
	_RecordsetPtr GetIMEIByOrderNumber(CString ordernumber);//根据订单号获取IMEI号
	_RecordsetPtr JudgeOrderNumber(CString ordernumber);//判断订单号是否存在

	CString ADOManage::GetTime();//获取当前系统时间



	/*
	//数据库查询模块所用的函数
	_RecordsetPtr ShowInsertImeiByOrderNumber(CString ordernumber);//将对应的订单号中已经插入的IMEI展示出来
	_RecordsetPtr ShowRepeatImeiByOrderNumber(CString ordernumber);//将对应的订单号中已经重复的IMEI展示出来
	_RecordsetPtr ShowUnImeiByOrderNumber(CString ordernumber, CString imeistrat, CString imeiend);//将对应的订单号中未插入的IMEI展示出来

	//更新订单模块所用的函数
	int GetImeicount(CString zhidan, CString imeistart, CString imeiend);//获取IMEI计数

	CString GetIMEICurrent(CString strordernumber);//获取当前镭雕的imei号


	//通讯模块所用的函数
	_RecordsetPtr UpdateIMEICurrent(CString strordernumber, CString imeicurrent);//存储当前镭雕的imei号
	_RecordsetPtr JudgeRepetition(CString strImei);//判断IMEI是否重复
	_RecordsetPtr JudgeIMEIExit(CString strImei);//判断重新镭雕时候IMEI是否存在
	int AddNew(CString picp, CString zhidan);//插入镭雕输出数据到镭雕数据表
	int updatereld(CString zhidan);//重新镭雕的更新操作
    */
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

