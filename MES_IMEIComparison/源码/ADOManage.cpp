#include "stdafx.h"
#include "ADOManage.h"
#include "iostream"   
#include "vector"
#include <string>

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
		::MessageBox(NULL, _T("数据库连接错误"), _T("异常信息"), NULL); /*打印出异常原因*/
		return 0;
	}
	return 1;
}

//连接数据库
bool ADOManage::ConndbSQL()
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
		::MessageBox(NULL, _T("数据库连接错误"), _T("异常信息"), NULL); /*打印出异常原因*/
		return 0;
	}
	return 1;
}

//关闭数据库连接
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

//根据其它号段寻找IMEI
int ADOManage::CpImeiByNo(CString noname,CString no)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	//_variant_t a;
	CString strSql;

	//查找IMEI是否存在，不存在返回0代表未找到IMEI
	strSql = _T("SELECT [IMEI] FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [")+noname+_T("] =") + _T("'") + no + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	//代表不存在此号段
	if (m_pRecordSet->adoEOF)
	{
		strSql = _T("SELECT [IMEI] FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [IMEI] =") + _T("'") + no + _T("'");
		m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

		if (m_pRecordSet->adoEOF)
		{
			return 1;
		}
		else
		{
			return 3;//返回3代表这个是个IMEI号
		}
		return 0;
	}


	return 2;//上面都没问题就返回2代表成功

}

//判断IMEI号是否存在
int ADOManage::JudgeImei(CString imei)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t a;
	CString strSql;

	//查找IMEI是否存在，不存在返回0代表未找到IMEI
	strSql = _T("SELECT [IMEI] FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [IMEI] =") + _T("'") + imei + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}

	return 1;//上面都没问题就返回2代表成功
}

//根据IMEI寻找彩盒时间
int ADOManage::CpCaiheByImei(CString imei)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	
	//参数
	_variant_t a;
	CString strSql;

	//查找IMEI是否存在，不存在返回0代表未找到IMEI
	strSql = _T("SELECT [IMEI],[CH_PrintTime] FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [IMEI] =") + _T("'") + imei + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}

	a = m_pRecordSet->GetCollect("CH_PrintTime");

	//返回1代表不存在彩盒时间
	if (a.vt == VT_NULL)
	{
		return 1;
	}

	return 2;//上面都没问题就返回2代表成功
}

//插入正确数据到表中
int ADOManage::InsertCorrectImei(CString zhidan, CString imei1, CString imei2, CString no1, CString no2, CString ip, CString notype, CString errorreason, CString result)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	int AffectLine = 0;
	CString strSql;

	//将数据插入表中
	strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]([CPIP],[ZhiDan],[IMEI1], [IMEI2], [SECTIONNO1], [SECTIONNO2], [CPRESULT], [CPTIME], [CPTYPE],[CPERROR])values('")
		+ ip + _T("', '") + zhidan + _T("', '") + imei1 + _T("', '") + imei2 + _T("', '") + no1 + _T("', '") + no2 + _T("', '") + result + _T("', '") + GetTime() + _T("', '") + notype + _T("', '") +errorreason +_T("')");//具体执行的SQL语句
	try{
	m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{}

	//如果对比成功那就更新一下数据，Affectline是insert操作返回的影响行数，如果为0代表插入失败，也就是说之前已经插入过这条数据
	if (result=="1"&&Affectline.vt == 0)
	{
		strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT = '1',IMEI2='") + imei2 + _T("',SECTIONNO1='") + no1 + _T("',SECTIONNO2 ='") + no2 + _T("',CPTYPE ='") + notype+_T("', RECPTIME='") + GetTime() + _T("' where[IMEI1] = '") + imei1 + _T("'") + _T("AND[ZhiDan] = '") + zhidan + _T("'");
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}

	return AffectLine;
}

//插入错误数据到表中
int ADOManage::InsertWrongImei(CString zhidan, CString imei1, CString imei2, CString no1, CString no2, CString ip, CString notype, CString errorreason, CString result)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	int AffectLine = 0;
	CString strSql;

	//将数据插入表中
	strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]([CPIP],[ZhiDan],[IMEI1], [IMEI2], [SECTIONNO1], [SECTIONNO2], [CPRESULT], [CPTIME],[CPFalseCount], [CPTYPE],[CPERROR])values('")
		+ ip + _T("', '") + zhidan + _T("', '") + imei1 + _T("', '") + imei2 + _T("', '") + no1 + _T("', '") + no2 + _T("', '") + result + _T("', '") + GetTime() + _T("', '")+L"1" + _T("', '") + notype + _T("', '") + errorreason + _T("')");//具体执行的SQL语句
	try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
	}

	//如果对比是失败的，那就更新一下失败次数
	if (result == "0"&&Affectline.vt == 0)
	{
		strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT='0',CPFalseCount = CPFalseCount+1,CPERROR=CPERROR+'，") + errorreason + _T("',IMEI2='") + imei2 + _T("',SECTIONNO1='") + no1 + _T("',SECTIONNO2 ='") + no2 + _T("',CPTYPE ='") + notype + _T("', RECPTIME='") + GetTime() + _T("' where[IMEI1] = '") + imei1 + _T("'") + _T("AND[ZhiDan] = '") + zhidan + _T("'");
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}

	return AffectLine;
}


//获取订单号码
_RecordsetPtr ADOManage::GetOrderNumber()
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT [ZhiDan] FROM [") + m_Seconddbname + _T("].[dbo].[Gps_ManuOrderParam]");
	//CString strSql = _T("SELECT [ZhiDan] FROM [") + m_Seconddbname + _T("].[dbo].[orderld]");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
	return m_pRecordSet;
}

//根据订单号获取IMEI号
_RecordsetPtr ADOManage::GetIMEIByOrderNumber(CString ordernumber)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[Gps_ManuOrderParam] WHERE [ZhiDan] =") + _T("'") + ordernumber + _T("'");
	//	CString strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[orderld] WHERE [ZhiDan] =") + _T("'") + ordernumber + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
	return m_pRecordSet;
}

//判断订单号是否存在
_RecordsetPtr ADOManage::JudgeOrderNumber(CString ordernumber)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针

	CString strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[Gps_ManuOrderParam] WHERE [ZhiDan] =") + _T("'") + ordernumber + _T("'");
	//	CString strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[orderld] WHERE [ZhiDan] =") + _T("'") + ordernumber + _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

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