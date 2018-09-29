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
	CString m_Firstdbname;    //第一个数据库名称
	CString m_Seconddbname;  //第二个数据库名称
	CString m_Firstformname;  //第一个数据表名称
	CString m_Secondformname;//第二个数据表名称
	void GetDBINFO();//获取数据库信息函数


	//以下都是自定义函数
public:
	//数据库连接和资源释放函数
	bool ConnSQL();//连接数据库函数
	bool ConntestdbSQL1();//数据库配置那里用来测试连接用
	bool ConntestdbSQL2();//数据库配置那里用来测试连接用
	bool CloseDB();//关闭数据库
	bool CloseAll();//释放数据库资源

	//非通用函数
	_RecordsetPtr MOBAN(CString str1, CString str2);//模版函数

	//测试结果表：
	//测试后将结果以及RID等插入到数据库,若已测试过但失败的更新结果为1
	void TestResultInsertSql(CString Rid, CString StandbyCurrent, CString StandbyAverage, CString SleepCurrent, CString SleepAverage, CString TestResult);
	//根据RID查找数据，找到并且测试结果为通过的则返回0，前端需提示已测试通过；查不到数据或者查到但是测试结果为不通过的则返回1，前端往下继续测试
	int CheckTestResultByRid(CString Rid);

	//配置表：
	//添加配置信息
	void ConfigInsertSql(CString ModelName, float StandbyUp, float StandbyDown, float SleepUP, float SleepDown, CString TestCommand, CString TestCommandReply, CString RidCommand, CString RidCommandReply, CString StandbyCommand, CString StandbyCommandReply, CString SleepCommand, CString SleepCommandReply, int Count, int ReadTime, int WriteTime);
	//根据机型查找该机型配置
	_RecordsetPtr CheckConfigByModelNameSql(CString ModelName);
	//查找所有机型
	_RecordsetPtr CheckAllInConfigSql();
	//更新配置信息
	void ConfigUpdataSql(CString ModelName, float StandbyUp, float StandbyDown, float SleepUP, float SleepDown, CString TestCommand, CString TestCommandReply, CString RidCommand, CString RidCommandReply, CString StandbyCommand, CString StandbyCommandReply, CString SleepCommand, CString SleepCommandReply, int Count, int ReadTime, int WriteTime);
	//根据机型删除整条数据
	void DeleteConfigByModelNameSql(CString ModelName);

	//通用函数
	CString GetTime();//获取当前系统时间
	_RecordsetPtr GetRst();//干掉某张表数据用的

};

