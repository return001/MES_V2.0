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

	void SimDataOkInsertSql(CString SDIP,CString RID,CString IMEI,CString CID,CString ICCID,CString SDOperator);//SIM卡下载成功后插入函数
	void SimDataErrorInsertSql(CString SDIP, CString RID, CString IMEI, CString SDOperator);//SIM卡下载失败后插入函数
	int SimDataLastStationSql(CString IMEI);//检测上一个工位是否通过函数
	int SimDataIsExitSql(CString RID,CString IMEI);//查询机子是否存在并且下载成功函数
	int SimDataNoIsExitSql(CString CID);//查询种子号是否已经下载过函数

	int SimDataReSql(CString RID, CString IMEI,CString strOKpath);//返工位处理函数

	//通用函数
	CString GetTime();//获取当前系统时间
	_RecordsetPtr GetRst();//干掉某张表数据用的

};

