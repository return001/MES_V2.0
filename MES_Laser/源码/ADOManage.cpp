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
bool ADOManage::ConntestdbSQL()
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


//下面五个函数是初始化配置模块所用的函数
//获取订单号码
_RecordsetPtr ADOManage::GetZhiDan()
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT [ZhiDan] FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] order by ZhiDan");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
	return m_pRecordSet;
}

//根据订单号获取IMEI号和镭雕模版
_RecordsetPtr ADOManage::GetIMEIByZhiDan(CString strzhidan)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [ZhiDan] =") + _T("'") + strzhidan + _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
	return m_pRecordSet;
}

//获取IMEI计数量
int ADOManage::GetImeicount(CString strzhidan, CString imeistart, CString imeiend)
{
	int imeicount;

	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	unsigned long long b = 0;
	b = _atoi64(CStringA(imeiend)) + 1;
	imeiend = _ui64toa(b, CT2A(imeiend), 10);
	//初始化Recordset指针
	CString strSql = _T("SELECT count(*) FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [ZhiDan] =") + _T("'") + strzhidan + _T("'") + _T("AND IMEI >=") + _T("'") + imeistart + _T("'") + _T("AND IMEI <=") + _T("'") + imeiend + _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	imeicount = m_pRecordSet->GetCollect("");

	return imeicount;
}

//获取当前镭雕的imei号
CString ADOManage::GetIMEICurrent(CString strzhidan)
{
	_variant_t a;
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT [LDIMEICurrent] FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [ZhiDan] =") + _T("'") + strzhidan + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	a = m_pRecordSet->GetCollect("LDIMEICurrent");

	if (a.vt == VT_NULL)
	{
		return L"";
	}

	return m_pRecordSet->GetCollect("LDIMEICurrent");
}

//判断订单号是否存在
_RecordsetPtr ADOManage::JudgeZhiDan(CString strzhidan)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针

	CString strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [ZhiDan] =") + _T("'") + strzhidan + _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	return m_pRecordSet;
}


//镭雕系统模块所用的函数
//插入镭雕输出数据到镭雕数据表
_RecordsetPtr ADOManage::AddNew(CString strpicp, CString strzhidan, CString strimei)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("](PCIP, InputTime, ZhiDan,IMEI)values('") + strpicp + _T("', '") + GetTime() + _T("', '") + strzhidan + _T("', '") + strimei + _T("')");

	//IMEI号存在唯一值，一般来说在外面就已经做足了防呆防重复的判断，这里是最后一道保险，如果出现了重复的IMEI就报异常
	try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}

	return m_pRecordSet;
}

//重新镭雕的更新操作
_RecordsetPtr ADOManage::UpdateReld(CString strimei)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET ReLdImeiNum = ReLdImeiNum+1,ReLdImeiTime='") + GetTime() + _T("' where [IMEI] = '") + strimei + _T("'");

	//如果执行错误就会报异常原因
	try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}

	return m_pRecordSet;
}

//判断IMEI是否存在
int ADOManage::JudgeIMEIExit(CString strImei)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT * FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [IMEI] =") + _T("'") + strImei + _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	//如果查出来的数据为空，则m_pRecordSet->adoEOF返回的是ture，此时函数返回0代表此IMEI不存在，否则返回1代表IMEI存在
	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}

	return 1;
}

//存储当前镭雕的imei号
_RecordsetPtr ADOManage::UpdateIMEICurrent(CString strzhidan, CString imeicurrent)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	_variant_t Affectline;

	//把IMEI号+1
	unsigned long long imeiint = 0;
	imeiint = _atoi64(CStringA(imeicurrent)) + 1;
	imeicurrent = _ui64toa(imeiint, CT2A(imeicurrent), 10);

	//更新一下最后一个扫入的IMEI号，一般来说不会更新失败，要是失败了，那肯定是订单号被干掉了
	CString strSql = _T("UPDATE[") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("]") + _T("SET LDIMEICurrent = '") + imeicurrent + _T("' where ZhiDan = '") + strzhidan + _T("'");

	try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
		AfxMessageBox(e.Description());/*打印出异常原因*/
	}
	return m_pRecordSet;
}


//下面三个数据库查询模块所用的函数
//将对应的订单号中已经插入的IMEI展示出来
_RecordsetPtr ADOManage::ShowInsertImeiByOrderNumber(CString ordernumber)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT [IMEI],[InputTime] FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [ZhiDan] =") + _T("'") + ordernumber + _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
	return m_pRecordSet;
}

//将对应的订单号中已经重复的IMEI展示出来
_RecordsetPtr ADOManage::ShowRepeatImeiByOrderNumber(CString ordernumber)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT [IMEI],[ReLdImeiNum],[ReLdImeiTime] FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [ZhiDan] =") + _T("'") + ordernumber + _T("'") + _T("AND [ReLdImeiNum]>0");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
	return m_pRecordSet;
}

//将对应的订单号中未插入的IMEI展示出来
_RecordsetPtr ADOManage::ShowUnImeiByOrderNumber(CString ordernumber, CString imeistart, CString imeiend)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针

	CString strSql1, strSql2, strSql3, imeitemp;

	//先清除一遍临时表
	strSql3 = _T("truncate table[testLD].[dbo].[Gps_ManuUnLdParam]");
	m_pConnection->Execute(_bstr_t(strSql3), NULL, adCmdText);//直接执行语句

	//然后将数据插入到一张临时表中
	unsigned long long a, b;
	a = _atoi64(CStringA(imeistart));
	b = _atoi64(CStringA(imeiend));
	for (; a <= b; a++)
	{
		imeistart = _ui64toa(a, CT2A(imeistart), 10);
		imeitemp = CreateIMEI15(imeistart);
		strSql1 = _T("insert into[testLD].[dbo].[Gps_ManuUnLdParam](UNIMEI)values('") + imeitemp + _T("')");
		m_pConnection->Execute(_bstr_t(strSql1), NULL, adCmdText);
	}

	//接着获取范围内没有的数据
	strSql2 = _T("select[UNIMEI] FROM[testLD].[dbo].[Gps_ManuUnLdParam] where[UNIMEI] not in(select[IMEI] FROM[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] where[zhidan] = '") + ordernumber + _T("')");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql2), NULL, adCmdText);//直接执行语句

	//最后清除临时表
	m_pConnection->Execute(_bstr_t(strSql3), NULL, adCmdText);//直接执行语句

	return m_pRecordSet;
}


//以下是通用函数
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

//用来算IMEI校验位用的
CString ADOManage::CreateIMEI15(CString imei)
{
	long long a = 0;
	int j = 0;
	char imeiChar[14];
	strncpy(imeiChar, CStringA(imei), sizeof(imeiChar));
	//	std::string sz2 = CT2A(imei.GetBuffer());

	int resultInt = 0;
	for (int i = 0; i < 14; i++)
	{
		int a = (int)imeiChar[i] - '0';
		i++;
		int temp = ((int)imeiChar[i] - '0') * 2;
		int b = temp < 10 ? temp : temp - 9;
		resultInt += a + b;
	}
	resultInt %= 10;
	GetLastError();
	resultInt = (resultInt == 0 ? 0 : 10 - resultInt);
	CString resultStr, abc;
	resultStr.Format(_T("%d"), resultInt);
	abc = imei + resultStr;
	imei.ReleaseBuffer();
	return abc;
}