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


//SIM卡下载成功后插入函数
void ADOManage::SimDataOkInsertSql(CString SDIP, CString RID, CString IMEI, CString CID, CString ICCID, CString SDOperator)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	int AffectLine = 0;
	CString strSql;


	//先判断这条数据是不是返工的
	strSql = _T("SELECT [RID],[IMEI],[SDRESULT] FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [RID]='") + RID + _T("' AND [IMEI] = '") + IMEI + _T("' AND SDRESULT = 2");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	//如果查得到那就代表这条数据是返工的，那接下来是返工数据更新，同时关联表也要更新
	if (!m_pRecordSet->adoEOF)
	{
		strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[DataRelativeSheet]") + _T("SET IMEI3 = '") + CID + _T("',IMEI4='") + ICCID + _T("'where[IMEI1] = '") + IMEI + _T("'");
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句

		strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET SDRESULT = '1',SDIP = '") + SDIP + _T("',CID='") + CID+_T("',RID='") + RID + _T("',ICCID='") + ICCID + _T("',SDOperator ='") + SDOperator + _T("',ReSDTime ='") + GetTime() + _T("',ReSDCount=ReSDCount+1 where[RID] = '") + RID + _T("'");
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
		return ;
	}

	strSql = _T("SELECT [IMEI1] FROM [") + m_Firstdbname + _T("].[dbo].[DataRelativeSheet] WHERE [IMEI1] = '") + IMEI + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	if (m_pRecordSet->adoEOF)
	{
		//如果不是返工就将数据插入表中，同时关联表也要插入数据
		strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[DataRelativeSheet](SN, IMEI1, IMEI3, IMEI4, TestTime, _MASK_FROM_V2)values(NULL,'")
			+ IMEI + _T("', '") + CID + _T("', '") + ICCID + _T("', '") + GetTime() + _T("', NULL)");//具体执行的SQL语句
		try{
			m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
		}
		catch (_com_error &e)
		{
		}
	}
	else if (!m_pRecordSet->adoEOF)
	{
		strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[DataRelativeSheet]") + _T("SET IMEI3 = '") + CID + _T("',IMEI4='") + ICCID + _T("'where[IMEI1] = '") + IMEI + _T("'");
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}


	//如果不是返工就将数据插入表中
	strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]([SDIP],[RID],[IMEI], [CID], [ICCID], [SDOperator], [SDTime], [SDRESULT],[ReSDCount])values('")
		+ SDIP + _T("', '") + RID + _T("', '") + IMEI + _T("', '") + CID + _T("', '") + ICCID + _T("', '") + SDOperator + _T("', '")  + GetTime() + _T("', 1,0)");//具体执行的SQL语句
	try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
	}

	//查找RID是否存在，然后判断标志位是否为0，如果存在表示下载过
	strSql = _T("SELECT [RID],[IMEI],[SDRESULT] FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [RID]='") + RID + _T("' AND [IMEI] = '") + IMEI + _T("' AND SDRESULT = 0");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	//查得到就更新,Affectline是insert操作返回的影响行数，如果为0代表插入失败，也就是说之前已经插入过这条数据但是是失败，现在机子重新下载成功需要更新数据
	if (!m_pRecordSet->adoEOF)
	{
		strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET SDRESULT = 1,SDIP = '") + SDIP + _T("',CID='") + CID + _T("',ICCID='") + ICCID + _T("',SDOperator ='") + SDOperator + _T("',SDTime ='") + GetTime() + _T("' where[IMEI] = '") + IMEI + _T("'");
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句

	}

}

//SIM卡下载失败后插入函数
void ADOManage::SimDataErrorInsertSql(CString SDIP, CString RID, CString IMEI, CString SDOperator)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	int AffectLine = 0;
	CString strSql;

	//查找RID是否存在
	strSql = _T("SELECT [RID],[IMEI],[SDRESULT] FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [RID]='") + RID + _T("' AND [IMEI] = '") + IMEI + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	if (m_pRecordSet->adoEOF)
	{
		//将数据插入表中
		strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]([SDIP],[RID],[IMEI],[SDOperator], [SDTime], [SDRESULT],[ReSDCount])values('")
			+ SDIP + _T("', '") + RID + _T("', '") + IMEI + _T("', '") + SDOperator + _T("', '") + GetTime() + _T("', 0,0)");//具体执行的SQL语句
		try{
			m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
		}
		catch (_com_error &e)
		{
		}
	}
}

//检测上一个工位是否通过函数
int ADOManage::SimDataLastStationSql(CString IMEI)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t a;
	CString strSql;

	//查找IMEI是否存在，不存在返回0代表未找到IMEI
	strSql = _T("SELECT [IMEI],[CoupleResult] FROM [") + m_Seconddbname + _T("].[dbo].[Gps_TestResult] WHERE [IMEI] ='") + IMEI + _T("'AND CoupleResult = 1");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	//查不到就返回0表示失败
	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}

	return 1;//上面没问题就返回1代表成功
}

//查询机子是否存在并且下载成功函数
int ADOManage::SimDataIsExitSql(CString RID,CString IMEI)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t a;
	CString strSql;
	//查找RID是否存在，存在就返回0表示下载过
	strSql = _T("SELECT [RID],[IMEI],[SDRESULT] FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [RID]='") + RID + _T("' AND [IMEI] = '") + IMEI + _T("' AND SDRESULT = 1");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	//查得到就返回0表示已经下载过
	if (!m_pRecordSet->adoEOF)
	{
		return 0;
	}

	return 1;//上面没问题就返回1代表找不到
}

//查询种子号是否已经下载过函数
int ADOManage::SimDataNoIsExitSql(CString CID)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t a;
	CString strSql;

	//查找CID是否存在，存在就返回0表示下载过
	strSql = _T("SELECT [CID] FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [CID] ='") + CID + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	//查得到就返回0表示已经下载过
	if (!m_pRecordSet->adoEOF)
	{
		return 0;
	}

	return 1;//上面没问题就返回1代表找不到
}

//返工位处理函数
int ADOManage::SimDataReSql(CString RID, CString IMEI,CString strOKpath)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t a;
	CString strSql,strCID;
	int flag;
	bool moveflag;
	//查找RID是否存在
	strSql = _T("SELECT [RID],[IMEI],[CID],[SDRESULT] FROM [") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("] WHERE [RID]='") + RID + _T("' AND [IMEI] = '") + IMEI + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	//查得到就先将数据转移到SIM数据下载记录表
	if (!m_pRecordSet->adoEOF)
	{
		flag = m_pRecordSet->GetCollect("SDRESULT");
		if (flag == 1)
		{
			strCID = m_pRecordSet->GetCollect("CID");

			//这里需要写移动文件夹的代码
			
			moveflag = MoveFile(strOKpath + strCID + L"\\", strOKpath.Left(strOKpath.GetLength() - 3) + L"\\" + strCID + L"\\");
			if (moveflag == FALSE)
			{
				MoveFile(strOKpath.Left(strOKpath.GetLength() - 3) + L"\\" + strCID + L"\\",strOKpath + strCID + L"\\");
				return 4;
			}
			//然后再将数据插入到另一张表
			strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[Gps_ManuReSimDataParam] select [SDIP], [RID], [IMEI], [CID], [ICCID], [SDOperator], [SDTime], [SDRESULT], [ReSDTime], [ReSDCount] from[") + m_Firstdbname + _T("].[dbo].[Gps_ManuSimDataParam] where[RID] = '") + RID + _T("' AND[IMEI] = '") + IMEI + _T("'");
			m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句


			//然后更新一下原数据
			strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET SDRESULT = '2',SDIP = NULL,CID=NULL,ICCID=NULL,SDOperator =NULL where[IMEI] = '") + IMEI + _T("'");
			m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

			strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[DataRelativeSheet]") + _T("SET IMEI3 = NULL, IMEI4 = NULL where[IMEI1] = '") + IMEI + _T("'");
			m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

			return 1;
		}
		else if (flag == 2)//等于2就表示它已经返工了，现在无须返工
		{
			return 2;
		}
		else if (flag == 0)//等于0表示它本来就没下载成功过，也无须返工
		{
			return 3;
		}

	}

	return 0;//上面有问题就返回0代表这台机器本来就没有数据
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