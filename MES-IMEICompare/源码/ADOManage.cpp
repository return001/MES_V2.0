#include "stdafx.h"
#include "ADOManage.h"
#include "iostream"   
#include "vector"
#include <string>
#include "MFCP2CPDlg.h"

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
int ADOManage::CpImeiByNo(CString Syllablesqlstr, CString no, CString strzhidan)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	//_variant_t a;
	CString strSql;

	//根据用户选择的字段去将IMEI号转换出来
	strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[Gps_ManuPrintParam] WHERE ZhiDan ='")+strzhidan+_T("' AND( ");
	strSql += Syllablesqlstr+L")";
	strSql.Replace(L"InputNumber",no);//然后将里面的预设字符串替换掉成具体的数据

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	////查找IMEI是否存在，不存在返回0代表未找到IMEI
	//strSql = _T("SELECT [IMEI] FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [") + noname + _T("] =") + _T("'") + no + _T("'") + _T("AND ZhiDan='") + strzhidan + _T("'");
	//m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	//代表不存在此号段
	if (m_pRecordSet->adoEOF)
	{
		//strSql = _T("SELECT [IMEI] FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [IMEI] =") + _T("'") + no + _T("'");
		//m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

		//if (m_pRecordSet->adoEOF)
		//{
		//	return 1;
		//}
		//else
		//{
		//	return 3;//返回3代表这个是个IMEI号
		//}
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

	return 1;
}

//判断制单号是否对得上
int ADOManage::JudgeZhidan(CString imei,CString Zhidan)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t a;
	CString strSql;

	//查找IMEI是否存在，不存在返回0代表未找到IMEI
	strSql = _T("SELECT [ZhiDan],[IMEI] FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [IMEI] ='") + imei + _T("' AND [ZhiDan]='") + Zhidan+_T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句


	//找不到那就返回1
	if (m_pRecordSet->adoEOF)
	{
		return 1;
	}

	return 0;//返回0就代表对得上
}


//根据IMEI寻找彩盒时间
int ADOManage::CpCaiheByImei(CString imei, CString ZhiDan)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	
	//参数
	_variant_t a;
	CString aStr;
	CString strSql,strzhidan;

	//查找IMEI是否存在，不存在返回0代表未找到IMEI
	strSql = _T("SELECT [ZhiDan],[IMEI],[CH_PrintTime] FROM [") + m_Seconddbname + _T("].[dbo].[") + m_Secondformname + _T("] WHERE [IMEI] =") + _T("'") + imei + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	if (m_pRecordSet->adoEOF)
	{
		return 0;
	}

	a = m_pRecordSet->GetCollect("CH_PrintTime");

	//返回1代表不存在彩盒时间

	if (a.vt == VT_NULL || a.vt == VT_EMPTY)
	{
		return 1;
	}

	aStr = (LPCSTR)_bstr_t(a);
	if (aStr == "")
	{
		return 1;
	}


	//如果制单号对不上，那就代表制单号错误
	strzhidan = m_pRecordSet->GetCollect("ZhiDan");
	if (ZhiDan!=strzhidan)
	{
		return 3;
	}

	/*打印表的IMEI，SN，SIM，ICCID，MAC，Equipment，VIP，BAT,RFID*/
	/*关联表的IMEI3（SIM），IMEI4（ICCID），IMEI6（MAC），IMEI7（Equipment），IMEI8（VIP），IMEI9（BAT）,IMEI14(IMEI2)*/

	//这里是查绑定的
	//static map<int, CString>::iterator Syllableiter;
	map<int, CString>::iterator BindIter;
	_variant_t Bindtempvt;
	CString Bindtemp;
	strSql = _T("SELECT * FROM [") + m_Firstdbname + _T("].[dbo].[DataRelativeSheet] WHERE [IMEI1] =") + _T("'") + imei + _T("'");
	//strSql = _T("SELECT [ZhiDan],[IMEI],[SIM],[VIP],[ICCID],[BAT],[MAC],[Equipment],[RFID] FROM [") + m_Firstdbname + _T("].[dbo].[Gps_ManuPrintParam] WHERE [IMEI] =") + _T("'") + imei + _T("'") + _T("AND ZhiDan='") + ZhiDan + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	if (m_pRecordSet->adoEOF)
	{
		return d_DataRelativeImeiCheck;
	}

	//根据MAP的长度来决定循环次数，如果对应的值为""，那就代表它缺少绑定，此时返回绑定的字段并报相应的错误
	for (BindIter = CMFCP2CPDlg::BindMap.begin(); BindIter != CMFCP2CPDlg::BindMap.end(); BindIter++)
	{
		switch (BindIter->first)
		{
		case 0:
			//查彩盒表
			//Bindtemp = m_pRecordSet->GetCollect("SIM");
			//Bindtempvt = m_pRecordSet->GetCollect("SIM");
			//if (Bindtemp == "" || Bindtemp == " " || Bindtempvt.vt == VT_NULL)
			//{
			//	return d_SimBindCheck;
			//}

			//查关联表的情况，因为NULL直接转CString会报错，所以要分为NULL和""的两种情况
			Bindtempvt = m_pRecordSet->GetCollect("IMEI3");
			if (Bindtempvt.vt == VT_EMPTY || Bindtempvt.vt == VT_NULL)
			{
				return d_SimBindCheck;
			}
			Bindtemp = (LPCSTR)_bstr_t(Bindtempvt);
			if (Bindtemp == "")
			{
				return d_SimBindCheck;
			}
			break;
		case 1:
			//Bindtemp = m_pRecordSet->GetCollect("VIP");
			//Bindtempvt = m_pRecordSet->GetCollect("VIP");
			//if (Bindtemp == "")
			//{
			//	return d_VipBindCheck;
			//}

			Bindtempvt = m_pRecordSet->GetCollect("IMEI8");
			if (Bindtempvt.vt == VT_EMPTY || Bindtempvt.vt == VT_NULL)
			{
				return d_VipBindCheck;
			}
			Bindtemp = (LPCSTR)_bstr_t(Bindtempvt);
			if (Bindtemp == "")
			{
				return d_VipBindCheck;
			}
			break;
		case 2:
			//Bindtemp = m_pRecordSet->GetCollect("ICCID");
			//Bindtempvt = m_pRecordSet->GetCollect("ICCID");
			//if (Bindtemp == "")
			//{
			//	return d_IccidBindCheck;
			//}

			Bindtempvt = m_pRecordSet->GetCollect("IMEI4");
			if (Bindtempvt.vt == VT_EMPTY || Bindtempvt.vt == VT_NULL)
			{
				return d_IccidBindCheck;
			}
			Bindtemp = (LPCSTR)_bstr_t(Bindtempvt);
			if (Bindtemp == "")
			{
				return d_IccidBindCheck;
			}
			break;
		case 3:
			//Bindtemp = m_pRecordSet->GetCollect("BAT");
			//Bindtempvt = m_pRecordSet->GetCollect("BAT");
			//if (Bindtemp == "")
			//{
			//	return d_BatBindCheck;
			//}

			Bindtempvt = m_pRecordSet->GetCollect("IMEI9");
			if (Bindtempvt.vt == VT_EMPTY || Bindtempvt.vt == VT_NULL)
			{
				return d_BatBindCheck;
			}
			Bindtemp = (LPCSTR)_bstr_t(Bindtempvt);
			if (Bindtemp == "")
			{
				return d_BatBindCheck;
			}
			break;
		case 4:
			//Bindtemp = m_pRecordSet->GetCollect("MAC");
			//Bindtempvt = m_pRecordSet->GetCollect("MAC");
			//if (Bindtemp == "")
			//{
			//	return d_MacBindCheck;
			//}

			Bindtempvt = m_pRecordSet->GetCollect("IMEI6");
			if (Bindtempvt.vt == VT_EMPTY || Bindtempvt.vt == VT_NULL)
			{
				return d_MacBindCheck;
			}
			Bindtemp = (LPCSTR)_bstr_t(Bindtempvt);
			if (Bindtemp == "")
			{
				return d_MacBindCheck;
			}
			break;
		case 5:
			//Bindtemp = m_pRecordSet->GetCollect("Equipment");
			//Bindtempvt = m_pRecordSet->GetCollect("Equipment");
			//if (Bindtemp == "")
			//{
			//	return d_EquipmentBindCheck;
			//}

			Bindtempvt = m_pRecordSet->GetCollect("IMEI7");
			if (Bindtempvt.vt == VT_EMPTY || Bindtempvt.vt == VT_NULL)
			{
				return d_EquipmentBindCheck;
			}
			Bindtemp = (LPCSTR)_bstr_t(Bindtempvt);
			if (Bindtemp == "")
			{
				return d_EquipmentBindCheck;
			}
			break;
		case 6:
			//Bindtemp = m_pRecordSet->GetCollect("RFID");
			//Bindtempvt = m_pRecordSet->GetCollect("RFID");
			//if (Bindtemp == "")
			//{
			//	return d_RfidBindCheck;
			//}

			Bindtempvt = m_pRecordSet->GetCollect("IMEI13");
			if (Bindtempvt.vt == VT_EMPTY || Bindtempvt.vt == VT_NULL)
			{
				return d_RfidBindCheck;
			}
			Bindtemp = (LPCSTR)_bstr_t(Bindtempvt);
			if (Bindtemp == "")
			{
				return d_RfidBindCheck;
			}
			break;
		case 7:

			Bindtempvt = m_pRecordSet->GetCollect("IMEI14");
			if (Bindtempvt.vt == VT_EMPTY || Bindtempvt.vt == VT_NULL)
			{
				return d_DataRelativeImeiBindCheck;
			}
			Bindtemp = (LPCSTR)_bstr_t(Bindtempvt);
			if (Bindtemp == "")
			{
				return d_DataRelativeImeiBindCheck;
			}
			break;
		default:
			break;
		}

	}

	return 2;//上面都没问题就返回2代表成功
}

//插入正确数据到表中
int ADOManage::InsertCorrectImei(CString zhidan, CString imei1, CString imei2, CString imei3, CString ip, CString ScanType, CString errorreason, CString result, CString CHResult)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	int AffectLine = 0;
	CString strSql;

	_variant_t IMEIResultTempVT;
	CString IMEIResultTemp;

	//单电脑模式
	if (ScanType == "Single")
	{
		//将数据插入表中
		strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]([CPIP],[ZhiDan],[IMEI1], [IMEI2],[IMEI3], [CPRESULT], [CPTIME], [CPTYPE],[CPERROR],[CHResult],[TesterID1])values('")
			+ ip + _T("', '") + zhidan + _T("', '") + imei1 + _T("', '") + imei2 + _T("', '") + imei3 + _T("', '") + result + _T("', '") + GetTime() + _T("', '") + ScanType + _T("', '") + errorreason + _T("', '") + CHResult + _T("', '") + g_UserNameStr + _T("')");//具体执行的SQL语句
		try{
			m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
		}
		catch (_com_error &e)
		{
		}

		//插入失败那就代表原先有数据了，那就更新一下数据，Affectline是insert操作返回的影响行数，如果为0代表插入失败，也就是说之前已经插入过这条数据
		if (result == "1"&&Affectline.vt == 0)
		{
			strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT = '1',ZhiDan = '") + zhidan + _T("',IMEI2='") + imei2 + _T("',IMEI3='") + imei3 + _T("',CHResult='") + CHResult + _T("',CPTYPE ='") + ScanType + _T("', RECPTIME='") + GetTime() + _T("', TesterID1='") + g_UserNameStr + _T("' where[IMEI1] = '") + imei1 + _T("'");
			try{
				m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
			}
			catch (_com_error &e)
			{
			}
		}


		//更新testresult这张表
		strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[Gps_TestResult]") + _T("SET CPResult = '1'where[IMEI] = '") + imei1 + _T("'") + _T("AND [ZhiDan]='") + zhidan + _T("'");
		try{
			m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
		}
		catch (_com_error &e)
		{
		}

		return AffectLine;
	}
	//多电脑模式
	else
	{
		if (ScanType == "MutiPC1")
		{
			//将数据插入表中
			strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]([CPIP],[ZhiDan],[IMEI1], [IMEI2],[IMEI3], [CPRESULT1], [CPTIME], [CPTYPE],[CPERROR],[CHResult],[TesterID1])values('")
				+ ip + _T("', '") + zhidan + _T("', '") + imei1 + _T("', '") + imei2 + _T("', '") + imei3 + _T("', '") + result + _T("', '") + GetTime() + _T("', '") + ScanType + _T("', '") + errorreason + _T("', '") + CHResult + _T("', '") + g_UserNameStr + _T("')");//具体执行的SQL语句
			try{
				m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
			}
			catch (_com_error &e)
			{
			}

			//插入失败那就代表原先有数据了，那就更新一下数据，Affectline是insert操作返回的影响行数，如果为0代表插入失败，也就是说之前已经插入过这条数据
			if (result == "1"&&Affectline.vt == 0)
			{
				strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT1 = '1',ZhiDan = '") + zhidan + _T("',IMEI2='") + imei2 + _T("',IMEI3='") + imei3 + _T("',CHResult='") + CHResult + _T("',CPTYPE ='") + ScanType + _T("', RECPTIME='") + GetTime() + _T("', TesterID1='") + g_UserNameStr + _T("' where[IMEI1] = '") + imei1 + _T("'");
				try{
					m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
				}
				catch (_com_error &e)
				{
				}
			}

			//先查询对比表另外一个标志位是否为1，为1了才能将关联表的标志位置为1

			try{
				strSql = _T("SELECT [IMEI1],[CPResult2] FROM [") + m_Firstdbname + _T("].[dbo].[Gps_ManuCpParam] WHERE [IMEI1] ='") + imei1 + _T("'");
				m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
			}
			catch (_com_error &e)
			{
			}

			//找不到那就返回1
			if (m_pRecordSet->adoEOF)
			{
				return 2;
			}
			else
			{
				IMEIResultTempVT = m_pRecordSet->GetCollect("CPResult2");
				if (IMEIResultTempVT.vt == VT_EMPTY || IMEIResultTempVT.vt == VT_NULL)
				{
					strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[Gps_TestResult]") + _T("SET CPResult = '0' where[IMEI] = '") + imei1 + _T("'") + _T("AND [ZhiDan]='") + zhidan + _T("'");
					try{
						m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
					}
					catch (...)
					{
					}

					strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT = '0' where[IMEI1] = '") + imei1 + _T("'");
					try{
						m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
					}
					catch (_com_error &e)
					{
					}
					return 1;
				}
				IMEIResultTemp = (LPCSTR)_bstr_t(IMEIResultTempVT);
				if (IMEIResultTemp == "1")
				{
					//更新testresult这张表
					strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[Gps_TestResult]") + _T("SET CPResult = '1' where[IMEI] = '") + imei1 + _T("'") + _T("AND [ZhiDan]='") + zhidan + _T("'");
					try{
						m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
					}
					catch (...)
					{
					}

					strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT = '1' where[IMEI1] = '") + imei1 + _T("'");
					try{
						m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
					}
					catch (...)
					{
					}

					return AffectLine;
				}
				else if (IMEIResultTemp == "0")
				{
					strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[Gps_TestResult]") + _T("SET CPResult = '0' where[IMEI] = '") + imei1 + _T("'") + _T("AND [ZhiDan]='") + zhidan + _T("'");
					try{
						m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
					}
					catch (...)
					{
					}

					strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT = '0' where[IMEI1] = '") + imei1 + _T("'");
					try{
						m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
					}
					catch (_com_error &e)
					{
					}

					return AffectLine;
				}
			}
			return AffectLine;
		}
		else if (ScanType == "MutiPC2")
		{
			//将数据插入表中
			strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]([CPIP],[ZhiDan],[IMEI1], [SECTIONNO1],[SECTIONNO2], [CPRESULT2], [CPTIME], [CPTYPE],[CPERROR],[CHResult],[TesterID2])values('")
				+ ip + _T("', '") + zhidan + _T("', '") + imei1 + _T("', '") + imei2 + _T("', '") + imei3 + _T("', '") + result + _T("', '") + GetTime() + _T("', '") + ScanType + _T("', '") + errorreason + _T("', '") + CHResult + _T("', '") + g_UserNameStr + _T("')");//具体执行的SQL语句
			try{
				m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
			}
			catch (...)
			{
			}

			//插入失败那就代表原先由数据了，那就更新一下数据，Affectline是insert操作返回的影响行数，如果为0代表插入失败，也就是说之前已经插入过这条数据
			if (result == "1"&&Affectline.vt == 0)
			{
				strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT2 = '1',ZhiDan = '") + zhidan + _T("',SECTIONNO1='") + imei2 + _T("',SECTIONNO2='") + imei3 + _T("',CHResult='") + CHResult + _T("',CPTYPE ='") + ScanType + _T("', RECPTIME='") + GetTime() + _T("', TesterID2='") + g_UserNameStr + _T("' where[IMEI1] = '") + imei1 + _T("'");
				try{
					m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
				}
				catch (...)
				{
				}
			}

			//先查询对比表另外一个标志位是否为1，为1了才能将关联表的标志位置为1

			try{
				strSql = _T("SELECT [IMEI1],[CPResult1] FROM [") + m_Firstdbname + _T("].[dbo].[Gps_ManuCpParam] WHERE [IMEI1] ='") + imei1 + _T("'");
				m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
			}
			catch (...)
			{
			}

			//找不到那就返回1
			if (m_pRecordSet->adoEOF)
			{
				return 2;
			}
			else
			{
				IMEIResultTempVT = m_pRecordSet->GetCollect("CPResult1");
				if (IMEIResultTempVT.vt == VT_EMPTY || IMEIResultTempVT.vt == VT_NULL)
				{
					strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[Gps_TestResult]") + _T("SET CPResult = '0' where[IMEI] = '") + imei1 + _T("'") + _T("AND [ZhiDan]='") + zhidan + _T("'");
					try{
						m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
					}
					catch (...)
					{
					}

					strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT = '0' where[IMEI1] = '") + imei1 + _T("'");
					try{
						m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
					}
					catch (_com_error &e)
					{
					}
					return 1;
				}
				IMEIResultTemp = (LPCSTR)_bstr_t(IMEIResultTempVT);
				if (IMEIResultTemp == "1")
				{
					//更新testresult这张表
					strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[Gps_TestResult]") + _T("SET CPResult = '1' where[IMEI] = '") + imei1 + _T("'") + _T("AND [ZhiDan]='") + zhidan + _T("'");
					try{
						m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
					}
					catch (...)
					{
					}

					strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT = '1' where[IMEI1] = '") + imei1 + _T("'");
					try{
						m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
					}
					catch (_com_error &e)
					{
					}
					return AffectLine;
				}
				else if (IMEIResultTemp == "0")
				{
					strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[Gps_TestResult]") + _T("SET CPResult = '0' where[IMEI] = '") + imei1 + _T("'") + _T("AND [ZhiDan]='") + zhidan + _T("'");
					try{
						m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
					}
					catch (...)
					{
					}

					strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT = '0' where[IMEI1] = '") + imei1 + _T("'");
					try{
						m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
					}
					catch (_com_error &e)
					{
					}
					return AffectLine;
				}
			}
			return AffectLine;
		}
	}

}

//插入错误数据到表中
int ADOManage::InsertWrongImei(CString zhidan, CString imei1, CString imei2, CString imei3, CString ip, CString ScanType, CString errorreason, CString result, CString CHResult)
{
	//初始化Recordset指针
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	//参数
	_variant_t Affectline;
	int AffectLine = 0;
	CString strSql;

	if (ScanType == "Single")
	{
		//将数据插入表中
		strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]([CPIP],[ZhiDan],[IMEI1], [IMEI2],[IMEI3], [CPRESULT], [CPTIME],[CPFalseCount], [CPTYPE],[CPERROR],[CHResult],[TesterID1])values('")
			+ ip + _T("', '") + zhidan + _T("', '") + imei1 + _T("', '") + imei2 + _T("', '") + imei3 + _T("', '") + result + _T("', '") + GetTime() + _T("', '") + L"1" + _T("', '") + ScanType + _T("', '") + errorreason + _T("', '") + CHResult + _T("', '") + g_UserNameStr + _T("')");//具体执行的SQL语句
		try{
			m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
		}
		catch (_com_error &e)
		{
		}


		//插入失败就代表原先有数据了，那就更新一下失败次数
		if (result == "0"&&Affectline.vt == 0)
		{
			strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT='0',CPFalseCount = CPFalseCount+1,CPERROR=CPERROR+'，") + errorreason + _T("',IMEI2='") + imei2 + _T("',IMEI3='") + imei3 + _T("',CPTYPE ='") + ScanType + _T("', RECPTIME='") + GetTime() + _T("',CHResult='") + CHResult + _T("',TesterID1='") + g_UserNameStr + _T("' where[IMEI1] = '") + imei1 + _T("'") + _T("AND[ZhiDan] = '") + zhidan + _T("'");
			try{
				m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
			}
			catch (_com_error &e)
			{
			}
		}


		//更新testresult这张表
		strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[Gps_TestResult]") + _T("SET CPResult = '0'where[IMEI] = '") + imei1 + _T("'") + _T("AND [ZhiDan]='") + zhidan + _T("'");
		try{
			m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
		}
		catch (_com_error &e)
		{
		}
		return AffectLine;
	}
	else
	{
		if (ScanType == "MutiPC1")
		{
			//将数据插入表中
			strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]([CPIP],[ZhiDan],[IMEI1], [IMEI2],[IMEI3], [CPRESULT1], [CPTIME],[CPFalseCount], [CPTYPE],[CPERROR],[CHResult],[TesterID1])values('")
				+ ip + _T("', '") + zhidan + _T("', '") + imei1 + _T("', '") + imei2 + _T("', '") + imei3 + _T("', '") + result + _T("', '") + GetTime() + _T("', '") + L"1" + _T("', '") + ScanType + _T("', '") + errorreason + _T("', '") + CHResult + _T("', '") + g_UserNameStr + _T("')");//具体执行的SQL语句
			try{
				m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
			}
			catch (_com_error &e)
			{
			}


			//插入失败就代表原先有数据了，那就更新一下失败次数
			if (result == "0"&&Affectline.vt == 0)
			{
				strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT='0',CPRESULT1='0',CPFalseCount = CPFalseCount+1,CPERROR=CPERROR+'，") + errorreason + _T("',IMEI2='") + imei2 + _T("',IMEI3='") + imei3 + _T("',CPTYPE ='") + ScanType + _T("', RECPTIME='") + GetTime() + _T("',CHResult='") + CHResult + _T("',TesterID1='") + g_UserNameStr + _T("' where[IMEI1] = '") + imei1 + _T("'") + _T("AND[ZhiDan] = '") + zhidan + _T("'");
				try{
					m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
				}
				catch (_com_error &e)
				{
				}
			}


			//更新testresult这张表
			strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[Gps_TestResult]") + _T("SET CPResult = '0'where[IMEI] = '") + imei1 + _T("'") + _T("AND [ZhiDan]='") + zhidan + _T("'");
			try{
				m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
			}
			catch (_com_error &e)
			{
			}
			return AffectLine;
		}
		else if (ScanType == "MutiPC2")
		{
						//将数据插入表中
			strSql = _T("insert into[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]([CPIP],[ZhiDan],[IMEI1], [SECTIONNO1],[SECTIONNO2], [CPRESULT2], [CPTIME],[CPFalseCount], [CPTYPE],[CPERROR],[CHResult],[TesterID2])values('")
				+ ip + _T("', '") + zhidan + _T("', '") + imei1 + _T("', '") + imei2 + _T("', '") + imei3 + _T("', '") + result + _T("', '") + GetTime() + _T("', '") + L"1" + _T("', '") + ScanType + _T("', '") + errorreason + _T("', '") + CHResult + _T("', '") + g_UserNameStr + _T("')");//具体执行的SQL语句
			try{
				m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
			}
			catch (_com_error &e)
			{
			}


			//插入失败就代表原先有数据了，那就更新一下失败次数
			if (result == "0"&&Affectline.vt == 0)
			{
				strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[") + m_Firstformname + _T("]") + _T("SET CPRESULT='0',CPRESULT2='0',CPFalseCount = CPFalseCount+1,CPERROR=CPERROR+'，") + errorreason + _T("',SECTIONNO1='") + imei2 + _T("',SECTIONNO2='") + imei3 + _T("',CPTYPE ='") + ScanType + _T("', RECPTIME='") + GetTime() + _T("',CHResult='") + CHResult + _T("',TesterID2='") + g_UserNameStr + _T("' where[IMEI1] = '") + imei1 + _T("'") + _T("AND[ZhiDan] = '") + zhidan + _T("'");
				try{
					m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
				}
				catch (_com_error &e)
				{
				}
			}


			//更新testresult这张表
			strSql = _T("UPDATE[") + m_Firstdbname + _T("].[dbo].[Gps_TestResult]") + _T("SET CPResult = '0'where[IMEI] = '") + imei1 + _T("'") + _T("AND [ZhiDan]='") + zhidan + _T("'");
			try{
				m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
			}
			catch (_com_error &e)
			{
			}
			return AffectLine;
		}

	}
	
}


//获取订单号码
_RecordsetPtr ADOManage::GetOrderNumber()
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT [ZhiDan] FROM [") + m_Seconddbname + _T("].[dbo].[Gps_ManuOrderParam] WHERE Status='0' OR Status='1' order by ZhiDan" );
	//CString strSql = _T("SELECT [ZhiDan] FROM [") + m_Seconddbname + _T("].[dbo].[orderld]");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
	return m_pRecordSet;
}

//根据订单号获取IMEI号
_RecordsetPtr ADOManage::GetIMEIByOrderNumber(CString ordernumber)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[Gps_ManuOrderParam] WHERE (Status='0' OR Status='1') AND [ZhiDan] =") + _T("'") + ordernumber + _T("'");
	//	CString strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[orderld] WHERE [ZhiDan] =") + _T("'") + ordernumber + _T("'");
	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
	return m_pRecordSet;
}

//判断订单号是否存在
_RecordsetPtr ADOManage::JudgeOrderNumber(CString ordernumber)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针

	CString strSql = _T("SELECT [ZhiDan] FROM [") + m_Seconddbname + _T("].[dbo].[Gps_ManuOrderParam] WHERE [ZhiDan] =") + _T("'") + ordernumber + _T("'");
	//	CString strSql = _T("SELECT * FROM [") + m_Seconddbname + _T("].[dbo].[orderld] WHERE [ZhiDan] =") + _T("'") + ordernumber + _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

	return m_pRecordSet;

}


//保存订单所选择的字段
void ADOManage::Savesyllable(CString order, int IMEI, int SN, int SIM, int VIP, int ICCID, int BAT, int MAC, int Equipment, int RFID,int IMEI2)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	//CString a;
	//a = CMFCP2CPDlg::SyllableMap[0];
	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	strSql.Format(_T("insert into[" + m_Firstdbname + "].[dbo].[Gps_ManuCpOrderRelationParam]([ZhiDan],[IMEISyllable], [SNSyllable], [SIMSyllable], [VIPSyllable], [ICCIDSyllable], [BATSyllable], [MACSyllable], [EquipmentSyllable],[RFIDSyllable],[IMEI2Syllable])values('")
		+ order + _T("', %d,%d,%d,%d,%d,%d,%d,%d,%d,%d)"), IMEI, SN, SIM, VIP, ICCID, BAT, MAC, Equipment,RFID,IMEI2);

	try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
	}

	//Affectline是insert操作返回的影响行数，如果为0代表插入失败，也就是说之前已经插入过这条数据
	if (Affectline.vt == 0)
	{
		strSql.Format(_T("UPDATE[" + m_Firstdbname + "].[dbo].[Gps_ManuCpOrderRelationParam] SET [IMEISyllable]=%d, [SNSyllable]=%d, [SIMSyllable]=%d, [VIPSyllable]=%d, [ICCIDSyllable]=%d, [BATSyllable]=%d, [MACSyllable]=%d, [EquipmentSyllable]=%d,[RFIDSyllable]=%d,[IMEI2Syllable]=%d WHERE [ZhiDan]='") + order + _T("'"), IMEI, SN, SIM, VIP, ICCID, BAT, MAC, Equipment,RFID,IMEI2);
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
}

//读取订单所选择的字段
void ADOManage::Readsyllable(CString order)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT  * FROM [" + m_Firstdbname + "].[dbo].[Gps_ManuCpOrderRelationParam] WHERE [ZhiDan]='") + order + _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句

}

//保存订单所绑定的字段
void ADOManage::Savebind(CString order, int SIM, int VIP, int ICCID, int BAT, int MAC, int Equipment, int RFID, int IMEI2)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针

	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	strSql.Format(_T("insert into[" + m_Firstdbname + "].[dbo].[Gps_ManuCpOrderRelationParam]([ZhiDan],[SIMBind], [VIPBind], [ICCIDBind], [BATBind], [MACBind], [EquipmentBind],[RFIDBind],[IMEI2Bind])values('")
		+ order + _T("', %d,%d,%d,%d,%d,%d,%d,%d)"), SIM, VIP, ICCID, BAT, MAC, Equipment,RFID,IMEI2);

	try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
	}

	//Affectline是insert操作返回的影响行数，如果为0代表插入失败，也就是说之前已经插入过这条数据
	if (Affectline.vt == 0)
	{
		strSql.Format(_T("UPDATE[" + m_Firstdbname + "].[dbo].[Gps_ManuCpOrderRelationParam] SET [SIMBind]=%d, [VIPBind]=%d, [ICCIDBind]=%d, [BATBind]=%d, [MACBind]=%d, [EquipmentBind]=%d,[RFIDBind]=%d,[IMEI2Bind]=%d  WHERE [ZhiDan]='") + order + _T("'"), SIM, VIP, ICCID, BAT, MAC, Equipment,RFID,IMEI2);
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
}

//读取订单所绑定的字段
void ADOManage::Readbind(CString order)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT  * FROM [" + m_Firstdbname + "].[dbo].[Gps_ManuCpOrderRelationParam] WHERE [ZhiDan]='") + order + _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
}

//保存一些配置
void ADOManage::SaveConfig(CString order, int imei3Flag, int IsSinglePcFlag)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针

	//参数
	_variant_t Affectline;
	CString strSql;

	//将数据插入表中
	strSql.Format(_T("insert into[" + m_Firstdbname + "].[dbo].[Gps_ManuCpOrderRelationParam]([ZhiDan],[IMEI3Flag],[PCModeFlag])values('") + order + _T("', %d,%d)"), imei3Flag, IsSinglePcFlag);

	try{
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
	catch (_com_error &e)
	{
	}

	//Affectline是insert操作返回的影响行数，如果为0代表插入失败，也就是说之前已经插入过这条数据
	if (Affectline.vt == 0)
	{
		strSql.Format(_T("UPDATE[" + m_Firstdbname + "].[dbo].[Gps_ManuCpOrderRelationParam] SET [IMEI3Flag]=%d,[PCModeFlag]=%d WHERE [ZhiDan]='") + order + _T("'"), imei3Flag, IsSinglePcFlag);
		m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句
	}
}

//读取一些配置
void ADOManage::ReadConfig(CString order)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针
	CString strSql = _T("SELECT  * FROM [" + m_Firstdbname + "].[dbo].[Gps_ManuCpOrderRelationParam] WHERE [ZhiDan]='") + order + _T("'");

	m_pRecordSet = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//直接执行语句
}

//通常返回值是此用户权限，返回CheckFail表示不存在此用户
CString ADOManage::CheckUserAuthority(CString UserName, CString UserPassword)
{
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	//初始化Recordset指针

	//参数
	_variant_t Affectline;
	CString strSql;

	_variant_t AuthorityTempVT;
	CString AuthorityTemp;

	//将数据插入表中
	strSql.Format(L"SELECT  * FROM [GPSTest].[dbo].[LUserAccount] WHERE [Name]='%s' AND [Password]='%s'",UserName,UserPassword);

	try{
		m_pRecordSet=m_pConnection->Execute(_bstr_t(strSql), &Affectline, adCmdText);//直接执行语句

		//找不到那就返回1
		if (m_pRecordSet->adoEOF)
		{
			return L"CheckFail";
		}
		else
		{
			AuthorityTempVT = m_pRecordSet->GetCollect("UserType");
			if (AuthorityTempVT.vt == VT_EMPTY || AuthorityTempVT.vt == VT_NULL)
			{
				return L"";
			}
			AuthorityTemp = (LPCSTR)_bstr_t(AuthorityTempVT);

			return AuthorityTemp;
		}
	}
	catch (...)
	{
	}
}

//获取当前时间函数
CString ADOManage::GetTime(){
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d/%d/%d %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	return strTime;
}