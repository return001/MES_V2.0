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

	//获取数据库信息，当我们连接数据库的时候就先获取这些数据再连接,如果没有从外部获取那就在类初始化那里自定义一下
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


	//以下都是自定义函数
public:
	//数据库连接和资源释放函数
	bool ConnSQL();//连接数据库函数
	bool ConntestdbSQL();//数据库配置那里用来测试连接用
	bool ConntestdbSQL2();//测试第二个数据库用的
	bool CloseDB();//关闭数据库
	bool CloseAll();//释放数据库资源

	//非通用函数
	_RecordsetPtr MOBAN(CString str1, CString str2);//模版函数

	//初始化配置模块中所用的函数
	_RecordsetPtr GetZhiDan();//获取订单号
	_RecordsetPtr GetIMEIByZhiDan(CString strzhidan);//根据订单号获取IMEI号
	_RecordsetPtr JudgeZhiDan(CString strzhidan);//判断订单号是否存在
    CString GetIMEICurrent(CString strzhidan);//获取当前镭雕的imei号和模版路径
	int GetImeicount(CString strzhidan, CString imeistart, CString imeiend);//获取IMEI计数
	

	//镭雕系统模块所用函数
	_RecordsetPtr UpdateIMEICurrent(CString strzhidan, CString imeicurrent);//存储当前镭雕的imei号
	_RecordsetPtr AddNew(CString strpicp, CString strzhidan, CString strimei);//插入镭雕输出数据到镭雕数据表
	_RecordsetPtr UpdateReld(CString strimei);//重新镭雕的更新操作
	int JudgeIMEIExit(CString strImei);//判断IMEI是否存在


	//数据库查询模块所用的函数
	_RecordsetPtr ShowInsertImeiByOrderNumber(CString ordernumber);//将对应的订单号中已经插入的IMEI展示出来
	_RecordsetPtr ShowRepeatImeiByOrderNumber(CString ordernumber);//将对应的订单号中已经重复的IMEI展示出来
	_RecordsetPtr ShowUnImeiByOrderNumber(CString ordernumber, CString imeistrat, CString imeiend);//将对应的订单号中未插入的IMEI展示出来

	//通用函数
	_RecordsetPtr GetRst();//干掉某张表数据用的
	CString GetTime();//获取当前系统时间
	CString CreateIMEI15(CString imei);//用来算IMEI校验位用的
};

