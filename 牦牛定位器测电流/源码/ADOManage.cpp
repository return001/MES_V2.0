#include "stdafx.h"
#include "ADOManage.h"
#include "iostream"   
#include "MFCP4ECurrentToolDlg.h"

//全局变量需在类外进行初始化

ADOManage::ADOManage() :
m_DataSource(_T("")),
m_Port(_T("")),
m_UserName(_T("")),
m_Password(_T("")),
m_Firstdbname(_T("")),
m_Seconddbname(_T("")),
m_Firstformname(_T("")),
m_Secondformname(_T(""))
{
}


ADOManage::~ADOManage()
{
}


//以下是数据库连接和释放资源函数

//连接数据库
bool ADOManage::ConnSQL()
{
	CoInitialize(NULL); //初始化COM环境           
	m_pConnection.CreateInstance(_T("ADODB.Connection"));//初始化Connection指针 
	GetDBINFO();//获取数据库信息
	try
	{
		/*打开数据库“SQLServer”，这里需要根据数据库的情况 */
		CString m_ConnectStr;//连接字符串

		//要注意连接字符串有三种模式，这里应该选择网络模式
		m_ConnectStr = _T("Provider=SQLOLEDB.1;Data Source=") + m_DataSource + _T(",") + m_Port + _T(";Network Library=DBMSSOCN;Initial Catalog=") + m_Firstdbname;
		m_pConnection->Open(_bstr_t(m_ConnectStr), _bstr_t(m_UserName), _bstr_t(m_Password), adConnectUnspecified);
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
		return 0;
	}
	return 1;
}

//测试连接数据库
bool ADOManage::ConntestdbSQL1()
{
	CoInitialize(NULL); //初始化COM环境           
	m_pConnection.CreateInstance(_T("ADODB.Connection"));//初始化Connection指针 
	try
	{
		/*打开数据库“SQLServer”，这里需要根据数据库的情况 */
		CString m_ConnectStr;//连接字符串

		//要注意连接字符串有三种模式，这里应该选择网络模式
		m_ConnectStr = _T("Provider=SQLOLEDB.1;Data Source=") + m_DataSource + _T(",") + m_Port + _T(";Network Library=DBMSSOCN;Initial Catalog=") + m_Firstdbname;
		m_pConnection->Open(_bstr_t(m_ConnectStr), _bstr_t(m_UserName), _bstr_t(m_Password), adConnectUnspecified);
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
		return 0;
	}

	MessageBox(NULL, L"数据库连接成功", L"提示信息", NULL);

	return 1;
}

//测试连接数据库
bool ADOManage::ConntestdbSQL2()
{
	CoInitialize(NULL); //初始化COM环境           
	m_pConnection.CreateInstance(_T("ADODB.Connection"));//初始化Connection指针 
	try
	{
		/*打开数据库“SQLServer”，这里需要根据数据库的情况 */
		CString m_ConnectStr;//连接字符串

		//要注意连接字符串有三种模式，这里应该选择网络模式
		m_ConnectStr = _T("Provider=SQLOLEDB.1;Data Source=") + m_DataSource + _T(",") + m_Port + _T(";Network Library=DBMSSOCN;Initial Catalog=") + m_Seconddbname;
		m_pConnection->Open(_bstr_t(m_ConnectStr), _bstr_t(m_UserName), _bstr_t(m_Password), adConnectUnspecified);
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
		return 0;
	}
	return 1;
}


//关闭数据库
bool ADOManage::CloseDB()
{
	if (m_pConnection->GetState() == adStateOpen)
	{
		m_pConnection->Close();
	}
	if (m_pConnection)
	{
		m_pConnection.Release();
		m_pConnection = NULL;
	}
	return 1;
}

//释放全部的数据库资源
bool ADOManage::CloseAll()
{
	if (m_pRecordSet->GetState() == adStateOpen)
	{
		m_pRecordSet->Close();
	}
	if (m_pRecordSet)
	{
		m_pRecordSet.Release();
		m_pRecordSet = NULL;
	}
	if (m_pConnection->GetState() == adStateOpen)
	{
		m_pConnection->Close();
	}
	if (m_pConnection)
	{
		m_pConnection.Release();
		m_pConnection = NULL;
	}
	return 1;
}

//获取数据库信息
void ADOManage::GetDBINFO()
{
	CString str;

	GetPrivateProfileString(_T("DatabaseInfo"), _T("DataSource"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
	ADOManage::m_DataSource = str;
	str.ReleaseBuffer();
	GetPrivateProfileString(_T("DatabaseInfo"), _T("Port"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
	ADOManage::m_Port = str;
	str.ReleaseBuffer();
	GetPrivateProfileString(_T("DatabaseInfo"), _T("UserName"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
	ADOManage::m_UserName = str;
	str.ReleaseBuffer();
	GetPrivateProfileString(_T("DatabaseInfo"), _T("Password"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
	ADOManage::m_Password = str;
	str.ReleaseBuffer();
	GetPrivateProfileString(_T("DatabaseInfo"), _T("Firstdbname"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
	ADOManage::m_Firstdbname = str;
	str.ReleaseBuffer();
	GetPrivateProfileString(_T("DatabaseInfo"), _T("Firstformname"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
	ADOManage::m_Firstformname = str;
	str.ReleaseBuffer();
	GetPrivateProfileString(_T("DatabaseInfo"), _T("Seconddbname"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
	ADOManage::m_Seconddbname = str;
	str.ReleaseBuffer();
	GetPrivateProfileString(_T("DatabaseInfo"), _T("Secondformname"), _T(""), str.GetBuffer(50), 50, _T(".\\SystemInfo.ini"));
	ADOManage::m_Secondformname = str;
	str.ReleaseBuffer();
}


//以下是非通用函数

//这个是模版函数，总结一下用法
_RecordsetPtr ADOManage::MOBAN(CString str1, CString str2)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t affectline, strnull;//这个是用来获取影响行数用的
	CString strSql;//这个是用来存放执行语句用的

	//查找语句
	strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [") + str1 + _T("] =") + _T("'") + str2 + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	//为真就代表没查出任何数据
	if (m_pRecordSet->adoEOF)
	{
		return m_pRecordSet;
	}

	//可以从数据集中拿数据然后放到某个变量
	m_pRecordSet->GetCollect("CH_PrintTime");

	//如果是判断NULL也要用_variant_t类型
	strnull = m_pRecordSet->GetCollect("CH_PrintTime");

	//这样就可以判断取出来的数据是否为NULL
	if (strnull.vt == VT_NULL)
	{
		return m_pRecordSet;
	}

	//插入语句
	strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]([CPIP],[ZhiDan])values('") + str1 + _T("', '") + str2 + _T("')");//具体执行的SQL语句

	//如果插入的记录存在唯一值，我们就得用try，catch来保证不会炸掉，并通过affectline来使得我们可以进行其它操作，比如下面的update
	try{
		m_pConnection->Execute(_bstr_t(strSql), &affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
		e.Description();
		//AfxMessageBox(e.Description());/*打印出异常原因*/
	}


	//更新语句
	//这里是假设插入失败后才进行
	if (affectline.vt == 0)
	{
		strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT = '1',IMEI2='") + str1 + _T("',SECTIONNO1='") + str2 + _T("'");
		m_pConnection->Execute(_bstr_t(strSql), &affectline, adCmdText);//直接执行语句
	}

	return m_pRecordSet;//上面都没问题就返回2代表成功

}


//测试后将结果以及RID等插入到数据库,若已测试过但失败的更新结果为1
void ADOManage::TestResultInsertSql(CString ECIP, CString Rid, CString StandbyFiveCurrent, CString StandbyAverage, CString SleepFiveCurrent1, CString SleepAverage1, CString SleepFiveCurrent2, CString SleepAverage2, CString TestResult)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//参数
	_variant_t Affectline;
	int AffectLine = 0;
	CString strSql;
	int TResult;

	//判断该Rid是否测试通过
	strSql = _T("SELECT *FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [RID]='") + Rid + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
	//为真就代表没查出任何数据
	if (m_pRecordSet->adoEOF)
	{
		strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("](ECIP,Rid,StandbyFiveCurrent,StandbyAverage,SleepFiveCurrent1,SleepAverage1,SleepFiveCurrent2,SleepAverage2,TestResult,TestTime,ReTestNum)values('") + ECIP + _T("','") + Rid + _T("','")
			+ StandbyFiveCurrent + _T("', '") + StandbyAverage + _T("', '") + SleepFiveCurrent1 + _T("', '") + SleepAverage1 + _T("','") + SleepFiveCurrent2 + _T("', '") + SleepAverage2 + _T("','") + TestResult + _T("','") + GetTime() + _T("','0')");//具体执行的SQL语句
		try
		{
			m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
		}
		catch (_com_error &e)
		{
			e.Description();
			//AfxMessageBox(e.Description());/*打印出异常原因*/
		}
	}
	//查出了数据
	else if (!m_pRecordSet->adoEOF) {
		//判断之前的测试结果是否为0
		TResult = m_pRecordSet->GetCollect("TestResult");
		if (TResult == 0)
		{
			strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET TestResult = '") + TestResult + _T("',ECIP ='") + ECIP + _T("',StandbyFiveCurrent = '") + StandbyFiveCurrent + _T("',StandbyAverage = '") + StandbyAverage + _T("',SleepFiveCurrent1 ='") + SleepFiveCurrent1 + _T("',SleepAverage1='") + SleepAverage1 + _T("',SleepFiveCurrent2 ='") + SleepFiveCurrent2 + _T("',SleepAverage2 ='") + SleepAverage2 + _T("' Where Rid ='") + Rid + _T("'");
			m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
		}
		else if (TResult == 1) 
		{
			strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET ECIP ='") + ECIP + _T("',StandbyFiveCurrent = '") + StandbyFiveCurrent + _T("',StandbyAverage = '") + StandbyAverage + _T("',SleepFiveCurrent1 ='") + SleepFiveCurrent1 + _T("',SleepAverage1='") + SleepAverage1 + _T("',SleepFiveCurrent2 ='") + SleepFiveCurrent2 + _T("',SleepAverage2 ='") + SleepAverage2 + _T("',ReTestNum = ReTestNum + 1,ReTestTime ='") + GetTime() + _T("'Where Rid ='") + Rid + _T("'");
			m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
		}
	}
}

//根据RID查找数据，找到并且测试结果为通过的则返回0，前端需提示已测试通过；查不到数据或者查到但是测试结果为不通过的则返回1，前端往下继续测试
int ADOManage::CheckTestResultByRid(CString Rid)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//参数
	_variant_t Affectline;
	int AffectLine = 0;
	CString strSql;
	int TResult;

	//根据Rid查找数据库
	strSql = _T("SELECT *FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [RID]='") + Rid + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
	//为真就代表没查出任何数据
	if (m_pRecordSet->adoEOF)
	{
		return 1;
	}
	//查出了数据
	else if (!m_pRecordSet->adoEOF) {
		//判断之前的测试结果是否为0
		TResult = m_pRecordSet->GetCollect("TestResult");
		if (TResult == 0)
		{
			return 1;
		}
		else if (TResult == 1)
		{
			return 0;
		}
	}
}

//添加配置信息
void ADOManage::ConfigInsertSql(CString ModelName, float StandbyUp, float StandbyDown, float SleepUP, float SleepDown, CString TestCommand, CString TestCommandReply, CString RidCommand, CString RidCommandReply, CString StandbyCommand, CString StandbyCommandReply, CString SleepCommand, CString SleepCommandReply, int Count, int ReadTime, int WriteTime)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//参数
	_variant_t Affectline;
	int AffectLine = 0;
	CString strSql;

	//根据机型名称查找数据库
	strSql = _T("SELECT *FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [ModelName]='") + ModelName + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
	//为真就代表没查出任何数据
	if (m_pRecordSet->adoEOF)
	{
		strSql.Format(_T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Secondformname + _T("](ModelName,StandbyUp,StandbyDown,SleepUP,SleepDown,TestCommand,TestCommandReply,RidCommand,RidCommandReply,StandbyCommand,StandbyCommandReply,SleepCommand,SleepCommandReply,Count,ReadTime,WriteTime)values('")
			+ ModelName + _T("', %g,%g,%g,%g,'") + TestCommand + _T("','") + TestCommandReply + _T("','") + RidCommand + _T("','") + RidCommandReply + _T("','") + StandbyCommand + _T("','") + StandbyCommandReply + _T("','") + SleepCommand + _T("','") + SleepCommandReply + _T("',%d,%d,%d)"), StandbyUp, StandbyDown, SleepUP, SleepDown, Count, ReadTime, WriteTime);
		try
		{
			m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
			MessageBox(NULL, L"添加成功", L"提示信息", NULL);
		}
		catch (_com_error &e)
		{
			e.Description();
			//AfxMessageBox(e.Description());/*打印出异常原因*/
		}
	}
	else {
		MessageBox(NULL, L"该机型已存在", L"提示信息", NULL);
	}
}

//根据机型查找该机型配置
_RecordsetPtr ADOManage::CheckConfigByModelNameSql(CString ModelName)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT * FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [ModelName] =") + _T("'") + ModelName + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	return m_pRecordSet;
}

//查找所有机型
_RecordsetPtr ADOManage::CheckAllInConfigSql()
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT [ModelName] FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Secondformname + _T("]");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
	return m_pRecordSet;
}

//更新配置信息
void ADOManage::ConfigUpdataSql(CString ModelName, float StandbyUp, float StandbyDown, float SleepUP, float SleepDown, CString TestCommand, CString TestCommandReply, CString RidCommand, CString RidCommandReply, CString StandbyCommand, CString StandbyCommandReply, CString SleepCommand, CString SleepCommandReply, int Count, int ReadTime, int WriteTime) 
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));//初始化Recordset指针
	//参数
	_variant_t Affectline;
	CString strSql;

	strSql.Format(_T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Secondformname + _T("] SET StandbyUp = %g,StandbyDown =%g,SleepUP = %g,SleepDown = %g,TestCommand ='") + TestCommand + _T("',TestCommandReply = '") + TestCommandReply + _T("',RidCommand = '") + RidCommand
		+ _T("',RidCommandReply ='") + RidCommandReply + _T("',StandbyCommand = '") + StandbyCommand + _T("',StandbyCommandReply='") + StandbyCommandReply + _T("',SleepCommand='") + SleepCommand + _T("',SleepCommandReply='") + SleepCommandReply + _T("',Count=%d,ReadTime=%d,WriteTime=%d WHERE ModelName='") + ModelName + _T("'"), StandbyUp, StandbyDown, SleepUP, SleepDown, Count, ReadTime, WriteTime);
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	if (Affectline.bVal == 1)
	{
		MessageBox(NULL, L"更新配置成功", L"提示信息", NULL);
	}
	if (Affectline.bVal == 0)
	{
		MessageBox(NULL, L"更新配置失败，请检查数据库信息", L"提示信息", NULL);
	}

}

//根据机型删除整条数据
void ADOManage::DeleteConfigByModelNameSql(CString ModelName) 
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));//初始化Recordset指针
	//参数
	_variant_t Affectline;
	CString strSql;

	strSql = _T("DELETE FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [ModelName]= '") + ModelName + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
}

/*以下是通用函数*/
//清除数据库某张表
_RecordsetPtr ADOManage::GetRst()
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针 
	CString strSql;
	strSql = "Truncate table [testLD].[dbo].[testld]";
	//strSql = "select * from BLEStatus";//具体执行的SQL语句

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//将查询数据导入m_pRecordset数据容器
	return m_pRecordSet;
}

//获取当前时间函数
CString ADOManage::GetTime(){
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d/%d/%d %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	return strTime;
}