// DataBase.cpp : 实现文件
//

#include "stdafx.h"
#include "WriteIMEI.h"
#include "DataBase.h"
#include "WriteIMEIDlg.h"
#include "DeleteDB_Data_Confirm.h"
#include "CSpreadSheet.h"

// CDataBase 对话框

IMPLEMENT_DYNAMIC(CDataBase, CDialog)

CDataBase::CDataBase(CWnd* pParent /*=NULL*/)
	: CDialog(CDataBase::IDD, pParent)
	, m_str_list(_T(""))
	, nSize_Record_Limit_Enable(TRUE)
	, RecordNumber(_T("1"))
	, Time_Limit_Enable(TRUE)
	, Number(_T(""))
	, Start_Time(_T("2013-6-10 00:00:00"))
	, End_Time(_T("2015-10-10 00:00:00"))
	, RECORD_Show(0)
	, Only_IMEI_NC_Machine(TRUE)
	, AllNetCode_Choose(FALSE)
	, Set_WritedFlag(FALSE)
	, NETCODE_Export_FlagValue(_T("Y"))
{

}

CDataBase::~CDataBase()
{
}

void CDataBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list2);
	DDX_LBString(pDX, IDC_LIST1, m_str_list);
	DDX_Control(pDX, IDC_COMBO1, Section_Box_IMEIS);
	DDX_Check(pDX, IDC_CHECK1, nSize_Record_Limit_Enable);
	DDX_Text(pDX, IDC_EDIT1, RecordNumber);
	DDX_Check(pDX, IDC_CHECK30, Time_Limit_Enable);
	DDX_Text(pDX, IDC_EDIT2, Number);
	DDX_Text(pDX, IDC_EDIT3, Start_Time);
	DDX_Text(pDX, IDC_EDIT6, End_Time);
	DDX_Control(pDX, IDC_LIST2, m_list1);
	DDX_Text(pDX, IDC_EDIT4, RECORD_Show);
	DDX_Control(pDX, IDC_BUTTON1, Connect_DB_For_Find_Control);
	DDX_Control(pDX, IDC_BUTTON9, Disconnect_DB_Control);
	DDX_Check(pDX, IDC_CHECK37, Only_IMEI_NC_Machine);
	DDX_Check(pDX, IDC_CHECK3, AllNetCode_Choose);
	DDX_Check(pDX, IDC_CHECK2, Set_WritedFlag);
	DDX_Text(pDX, IDC_EDIT9, NETCODE_Export_FlagValue);
	DDX_Control(pDX, IDC_BUTTON4, Clear_FlagValue_Control);
}


BEGIN_MESSAGE_MAP(CDataBase, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDataBase::OnBnClickedButtonstart1)
	ON_LBN_DBLCLK(IDC_LIST1, &CDataBase::OnDblclkList2)
	ON_BN_CLICKED(IDOK, &CDataBase::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON9, &CDataBase::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON3, &CDataBase::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDataBase::OnBnClickedButton4)
END_MESSAGE_MAP()


// CDataBase 消息处理程序

BOOL CDataBase::OnInitDialog()
{
	CDialog::OnInitDialog();
	Disconnect_DB_Control.EnableWindow(FALSE);
	Wrong=FALSE;
	Clear_FlagValue_Control.EnableWindow(FALSE);
	return TRUE;
}

BOOL CDataBase::Ado(CString strConn)
{
	::CoInitialize(NULL);				// 初始化OLE/COM库环境 
	try
	{
		m_pConn.CreateInstance("ADODB.Connection");	//创建Connection对象
		m_pConn->ConnectionTimeout=5;	//设置超时时间为5秒
		m_pConn->Open((_bstr_t)strConn,"", "", adModeUnknown);//连接数据库	
	}
	
	catch(_com_error e)	
	{
		CATCH_ERROR;//AfxMessageBox("1");
		return false;
	}
	return true;
}
_RecordsetPtr&  CDataBase::GetRS(CString strSQL) //执行strSQL的SQL语句，返回集录集
{
	try
	{
		SQL_Wrong_Excute=false;
		m_pRS.CreateInstance(__uuidof(Recordset));
		m_pRS->Open((_bstr_t)strSQL,m_pConn.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);	// 取得表中的记录
	}
	catch(_com_error e)
	{
		CATCH_ERROR	;
		SQL_Wrong_Excute=true;
		//AfxMessageBox("双击的该表中 不存在选择的字段，或者存在其他异常，请重新确认再查询......");
	}
	return m_pRS;
}
void CDataBase::OnBnClickedButtonstart1()//连接数据库
{
	// TODO: Add your control notification handler code here
	//数据库连接
	UpdateData(true);//更新数据的值
	CWriteIMEIDlg*  pdlg=(CWriteIMEIDlg*)GetParent();
	CString m_strConn;
	_RecordsetPtr pRS;

	m_strConn.Format("driver={SQL Server}; Server=%s; DATABASE=%s; UID=%s; PWD=%s", pdlg->m_server_M, pdlg->m_db_M, pdlg->m_user_M,  pdlg->m_pwd_M);
	
	if(!Ado(m_strConn))
	{
		AfxMessageBox("连接数据库失败");
		return;
	}
	else
	{
		Connect_DB_For_Find_Control.EnableWindow(FALSE);
		Disconnect_DB_Control.EnableWindow(TRUE);
		//AfxMessageBox("连接数据库成功");
	}
	//获取所有表名-SQL SERVER
	pRS=GetRS("select name from sysobjects where xtype='U'");

	CString user;
	int x=0;
	strArry.RemoveAll();
	try
	{
		while (pRS->adoEOF==0)
		{
			strArry.Add((LPCSTR)(_bstr_t)pRS->GetCollect(_variant_t((long)x)));
			pRS->MoveNext();
		}
	}
	catch(_com_error e)
	{
		CATCH_ERROR	;
		Connect_DB_For_Find_Control.EnableWindow(TRUE);
			return;
	}
	//清空list列表
	while(m_list2.GetCount())  
		m_list2.DeleteString(0);

	for(int i=0; i<strArry.GetSize(); i++)
	{
		CString table1,table2,table3;
		m_list2.AddString(strArry.GetAt(i));
		
	}	
}


//获取记录条数
int CDataBase::GetNum(CString strc)
{
	int nSize=0;//记录的条数

	CString strSql;
	CString Number_temp;
	Number_temp= "N'"+Number+"'";

	//CONVERT(DATETIME, '"& endday &"', 102)格式为yy.mm.dd       AND ((WritedFlag <> 'Y') OR (WritedFlag IS NULL))

	if(AllNetCode_Choose==TRUE)//不排除已经写了标志位的数据
	{
		if(((Section_Box_IMEIS_CS=="create_time"))  &&  (Start_Time!="")   &&  (End_Time!=""))
			strSql="select count(*) FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND ("+Section_Box_IMEIS_CS+" <= CONVERT(DATETIME, '"+End_Time+"', 102))";
		else if((Time_Limit_Enable==TRUE)  &&  (Start_Time!="")   &&  (End_Time!="")  &&(Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
		{
			strSql="select count(*) FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+") AND (create_time >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND (create_time <= CONVERT(DATETIME, '"+End_Time+"', 102))";
		}
		else if((Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
			strSql="select count(*) FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+")";// WHERE (imei LIKE '[1]%')
		else
		{
			AfxMessageBox("查询的数据不能为空！");//(DEVTYPE = N'11') AND 
			return 0;
		}
	}
	else if(m_str_list=="dse_barcode_imei")//排除了已经写了标志位的数据，且为写号表 AND ((WritedFlag <> 'Y') OR (WritedFlag IS NULL)) ----(WritedFlag <> 'Y') OR 
	{
		if(((Section_Box_IMEIS_CS=="create_time"))  &&  (Start_Time!="")   &&  (End_Time!=""))
			strSql="select count(*) FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND ("+Section_Box_IMEIS_CS+" <= CONVERT(DATETIME, '"+End_Time+"', 102)) AND ((WritedFlag IS NULL))";
		else if((Time_Limit_Enable==TRUE)  &&  (Start_Time!="")   &&  (End_Time!="")  &&(Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
		{
			strSql="select count(*) FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+") AND (create_time >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND (create_time <= CONVERT(DATETIME, '"+End_Time+"', 102)) AND ((WritedFlag IS NULL))";
		}
		else if((Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
			strSql="select count(*) FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+") AND ((WritedFlag IS NULL))";// WHERE (imei LIKE '[1]%')
		else
		{
			AfxMessageBox("查询的数据不能为空！");//(DEVTYPE = N'11') AND 
			return 0;
		}
	}
	else//对其他表，AllNetCode_Choose
	{
		if(((Section_Box_IMEIS_CS=="create_time"))  &&  (Start_Time!="")   &&  (End_Time!=""))
			strSql="select count(*) FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND ("+Section_Box_IMEIS_CS+" <= CONVERT(DATETIME, '"+End_Time+"', 102))";
		else if((Time_Limit_Enable==TRUE)  &&  (Start_Time!="")   &&  (End_Time!="")  &&(Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
		{
			strSql="select count(*) FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+") AND (create_time >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND (create_time <= CONVERT(DATETIME, '"+End_Time+"', 102))";
		}
		else if((Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
			strSql="select count(*) FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+")";// WHERE (imei LIKE '[1]%')
		else
		{
			AfxMessageBox("查询的数据不能为空！");//(DEVTYPE = N'11') AND 
			return 0;
		}
	}
	/*
	 WHERE (DEVTYPE = N'11') AND (create_time >= CONVERT(DATETIME, 
	 '2012-10-17 16:57:17', 102)) OR
	(create_time <= CONVERT(DATETIME, '2012-10-17 16:57:17', 102))*/

	_RecordsetPtr pRS = GetRS(strSql);
	if(SQL_Wrong_Excute==true)
	{
		SQL_Wrong_Excute=false;
		return -1;
		
	}
	
	CString s=(LPCSTR)(_bstr_t)pRS->GetCollect(_variant_t((long)0));
	char *ch=new char[s.GetLength()];
	ch=(LPSTR)(LPCTSTR)s;
	nSize=atoi(ch);
	return nSize;        //记录的条数
	delete []ch;
}
//获取字段名
BOOL CDataBase::GetFieldsName(_RecordsetPtr RcdPtr, int nField, CString & strFieldName)   

{   
	
	if(NULL == RcdPtr || nField >= RcdPtr->GetFields()->Count)
		return FALSE;
	
	_variant_t vt((long)nField);
	
	strFieldName.Format(_T("%s"), (char*)(RcdPtr->GetFields()->Item[vt]->Name)); 
	
	return true;
	
}


//获取字段数量
int CDataBase::GetFieldsCount(_RecordsetPtr RcdPtr)   

{   
	
	int nCount=0;	
	if(NULL != RcdPtr)
		
	{   	
		nCount = RcdPtr->GetFields()->Count;   	
	}
	return nCount;   	
}
void CDataBase::OnDblclkList2() //list控件双击事件
{
	// TODO: Add your control notification handler code here
	m_list1.DeleteAllItems();
	UpdateData(true);
	int q=0;
	//获取字段个数======================================
	

	Section_Box_IMEIS.GetWindowTextA(Section_Box_IMEIS_CS);
	
	//获取记录条数
	if(nSize_Record_Limit_Enable==TRUE)
		RECORD=atoi(RecordNumber);
	else
		RECORD=GetNum(m_str_list);
	if(RECORD<=0)
		return;

	
	CString strSql1;
	strSql1="select * from";
	strSql1+=" ";
	strSql1+=m_str_list;

	_RecordsetPtr m_pRS2 = GetRS(strSql1);
	if(SQL_Wrong_Excute==true)
	{
		SQL_Wrong_Excute=false;
		return;
	}
	//获取字段数
	/*if(Only_IMEI_NC_Machine==TRUE)
	{
		dataSize=3;
	}
	else*/
	{
		dataSize=GetFieldsCount(m_pRS2);
	}
	//==================================================
	if (dataSize==0)
	{
		return;
	}
	//获取字段名strName= /CString *strName=new CString[dataSize];
	strName=new CString[dataSize];
	for (int bb=0;bb<dataSize;bb++)
	{
		GetFieldsName(m_pRS,bb,*(strName+bb));
		if((Section_Box_IMEIS_CS=="")||(m_str_list_Old!=m_str_list))
		{
			if((m_str_list_Old!=m_str_list)&&(bb==0))
			{
				for(int i=64;i>=0;i--)
					Section_Box_IMEIS.DeleteString(i);
			}
			Section_Box_IMEIS.AddString(*(strName+bb));
			Section_Box_IMEIS.SetCurSel(0);
		}
	}
	
	//====================================================
	//清空表头
	while(m_list1.DeleteColumn(0));
	//将字段显示出来
	for (int cc=0;cc<dataSize;cc++)
	{
		m_list1.InsertColumn(cc, *(strName+cc), LVCFMT_LEFT, 150);
	
	}
	/////////////////////数据库字段切换/////////////////////////////
	if((Section_Box_IMEIS_CS=="")||(m_str_list_Old!=m_str_list))
	{
		m_str_list_Old=m_str_list;
		return;
	}
	m_str_list_Old=m_str_list;
	//////////////////////////////////////////////////////////////
	
	CString strSql;
	CString Number_temp;
	
	Number_temp= "N'"+Number+"'";
	if(AllNetCode_Choose==TRUE)//不排除已经写了标志位的数据
	{
		/*if(((Section_Box_IMEIS_CS=="create_time"))  &&  (Start_Time!="")   &&  (End_Time!=""))
			strSql="select imei, NetCode, TYPE FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND ("+Section_Box_IMEIS_CS+" <= CONVERT(DATETIME, '"+End_Time+"', 102))";
		else if((Time_Limit_Enable==TRUE)  &&  (Start_Time!="")   &&  (End_Time!="")  &&(Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
		{		
			strSql="select imei, NetCode, TYPE FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+") AND (create_time >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND (create_time <= CONVERT(DATETIME, '"+End_Time+"', 102))";
		}
		else if((Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
			strSql="select imei, NetCode, TYPE FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+")";// WHERE (imei LIKE '[1]%')
		else
		{
			AfxMessageBox("查询的数据不能为空！");//(DEVTYPE = N'11') AND 
			return;
		}*/
		if(((Section_Box_IMEIS_CS=="create_time"))  &&  (Start_Time!="")   &&  (End_Time!=""))
			strSql="select * FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND ("+Section_Box_IMEIS_CS+" <= CONVERT(DATETIME, '"+End_Time+"', 102))";
		else if((Time_Limit_Enable==TRUE)  &&  (Start_Time!="")   &&  (End_Time!="")  &&(Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
		{		
			strSql="select * FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+") AND (create_time >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND (create_time <= CONVERT(DATETIME, '"+End_Time+"', 102))";
		}
		else if((Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
			strSql="select * FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+")";// WHERE (imei LIKE '[1]%')
		else
		{
			AfxMessageBox("查询的数据不能为空！");//(DEVTYPE = N'11') AND 
			return;
		}
	}
	else if(m_str_list=="dse_barcode_imei")
	{
		if(((Section_Box_IMEIS_CS=="create_time"))  &&  (Start_Time!="")   &&  (End_Time!=""))
			strSql="select * FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND ("+Section_Box_IMEIS_CS+" <= CONVERT(DATETIME, '"+End_Time+"', 102)) AND ((WritedFlag IS NULL))";
		else if((Time_Limit_Enable==TRUE)  &&  (Start_Time!="")   &&  (End_Time!="")  &&(Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
		{		
			strSql="select * FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+") AND (create_time >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND (create_time <= CONVERT(DATETIME, '"+End_Time+"', 102)) AND ((WritedFlag IS NULL))";
		}
		else if((Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
			strSql="select * FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+") AND ((WritedFlag IS NULL))";// WHERE (imei LIKE '[1]%')
		else
		{
			AfxMessageBox("查询的数据不能为空！");//(DEVTYPE = N'11') AND 
			return;
		}
	}
	else
	{
		if(((Section_Box_IMEIS_CS=="create_time"))  &&  (Start_Time!="")   &&  (End_Time!=""))
			strSql="select * FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND ("+Section_Box_IMEIS_CS+" <= CONVERT(DATETIME, '"+End_Time+"', 102))";
		else if((Time_Limit_Enable==TRUE)  &&  (Start_Time!="")   &&  (End_Time!="")  &&(Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
		{		
			strSql="select * FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+") AND (create_time >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND (create_time <= CONVERT(DATETIME, '"+End_Time+"', 102))";
		}
		else if((Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
			strSql="select * FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+")";// WHERE (imei LIKE '[1]%')
		else
		{
			AfxMessageBox("查询的数据不能为空！");//(DEVTYPE = N'11') AND 
			return;
		}
	}
	
	
	int j=0;
	_RecordsetPtr pRS = GetRS(strSql);
	if(SQL_Wrong_Excute==true)
	{
		return;
	}

	//CStringArray *strdataArray=new CStringArray[dataSize];
	strdataArray=new CStringArray[dataSize];
	try
	{
		
		//pRS->MoveFirst();
		//for (int aa=0;aa<dataSize;aa++)
		pRS->MoveLast();
		/*LPCTSTR  IMEI_NAME="imei";
		LPCTSTR  NetCode_NAME="NetCode";
		LPCTSTR  TYPE_NAME="TYPE";
		if(Only_IMEI_NC_Machine==TRUE)
		{
			for(int j=0;j<RECORD;j++)
			{	
				//for(int j=0;j<RECORD;j++)
				for (int aa=0;aa<dataSize;aa++)
				{	
					CString str1;
					_variant_t varTemp;
					//判断数据库中的NULL值
					if(aa==0)
					{
						varTemp=pRS->GetCollect(IMEI_NAME);
						if(varTemp.vt ==VT_NULL)
							str1="<NULL>";
						else
							str1.Format("%s",(LPCSTR)(_bstr_t)pRS->GetCollect(IMEI_NAME));
					}
					if(aa==1)
					{
						varTemp=pRS->GetCollect(NetCode_NAME);
						if(varTemp.vt ==VT_NULL)
							str1="<NULL>";
						else
							str1.Format("%s",(LPCSTR)(_bstr_t)pRS->GetCollect(NetCode_NAME));
					}
					if(aa==2)
					{
						varTemp=pRS->GetCollect(TYPE_NAME);
						if(varTemp.vt ==VT_NULL)
							str1="<NULL>";
						else
							str1.Format("%s",(LPCSTR)(_bstr_t)pRS->GetCollect(TYPE_NAME));
					}
					
					(strdataArray+aa)->Add(str1);
					
					
				}
				//if(pRS->adoEOF ==0)
					//pRS->MoveNext();
				if(pRS->BOF ==0)
					pRS->MovePrevious();
				else
					break;
			}
		}
		else*/
		{
			for(int j=0;j<RECORD;j++)
			{	
				//for(int j=0;j<RECORD;j++)
				for (int aa=0;aa<dataSize;aa++)
				{	
					CString str1;
					_variant_t varTemp;
					//判断数据库中的NULL值
					varTemp=pRS->GetCollect(_variant_t((long)aa));
					if(varTemp.vt ==VT_NULL)
						str1="<NULL>";
					else
						str1.Format("%s",(LPCSTR)(_bstr_t)pRS->GetCollect(_variant_t((long)aa)));
					
					(strdataArray+aa)->Add(str1);
					
					
				}
				//if(pRS->adoEOF ==0)
					//pRS->MoveNext();
				if(pRS->BOF ==0)
					pRS->MovePrevious();
				else
					break;
			}
		}

	}
		
		

	catch(_com_error e)
	{
		//CATCH_ERROR;
		AfxMessageBox("实际记录     小于   设定的预获取数据量！请设小数据量，假如设定的预获取数据量已经==1，则表示数据库中无查询的数据");
			return;
	}
	//===================================================

	CString str;
	int tt=0;
	for (tt=0;tt<RECORD;tt++)
	{
		m_list1.InsertItem(tt,"1",0);//插入行
	}
	
	//for (int tt=0;tt<RECORD;tt++)
	for(int i=0; i<dataSize; i++)
	{	
		//for(int i=0; i<dataSize; i++)
		for (tt=0;tt<RECORD;tt++)
		{	
			str =(strdataArray+i)->GetAt(tt);
			TRACE("   行:%d, 列:%d,数据:%s\n",tt,i,str);
			m_list1.SetItemText(tt,i,str);//插入内容		    
		}			
	}
	RECORD_Show=RECORD;
	UpdateData(false);

	//delete []strName;
	//delete []strdataArray; //释放申请的空间
}

_RecordsetPtr CDataBase::Execute(LPCTSTR lpszSQL, long lOptions)
{

	ASSERT(m_pConn != NULL);
	ASSERT(AfxIsValidString(lpszSQL));
	Wrong=FALSE;//执行
	try
	{
		return m_pConn->Execute(_bstr_t(lpszSQL), NULL, lOptions);
	}
	//catch (...)
	catch(_com_error e)
	{
		//CATCH_ERROR;
		Wrong = TRUE;
		AfxMessageBox("双击的该表中 不存在选择的字段，或者存在其他异常，请重新确认再查询......");
	} 
	Wrong=FALSE;//不会执行
	return TRUE;

}

void CDataBase::OnBnClickedOk()//删除当前记录
{
	// TODO: 在此添加控件通知处理程序代码
	CDeleteDB_Data_Confirm confirm_dlg;
	confirm_dlg.PassWord_Right=false;
	confirm_dlg.DoModal();
	if(confirm_dlg.PassWord_Right!=true)
		return;
	if(RECORD==0)
	{
		AfxMessageBox("没有选择要删除的数据！");
		return;
	}

	CString strSql;
	CString Number_temp;   // AND ((WritedFlag <> 'Y') OR (WritedFlag IS NULL))
	
	Number_temp= "N'"+Number+"'";
	if(AllNetCode_Choose==TRUE)//不排除已经写了标志位的数据
	{
		if(((Section_Box_IMEIS_CS=="create_time"))  &&  (Start_Time!="")   &&  (End_Time!=""))
			strSql="DELETE FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND ("+Section_Box_IMEIS_CS+" <= CONVERT(DATETIME, '"+End_Time+"', 102))";
		else if((Time_Limit_Enable==TRUE)  &&  (Start_Time!="")   &&  (End_Time!="")  &&(Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
		{		
			strSql="DELETE FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+") AND (create_time >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND (create_time <= CONVERT(DATETIME, '"+End_Time+"', 102))";
		}
		else if((Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
			strSql="DELETE FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+")";// WHERE (imei LIKE '[1]%')
		else
		{
			AfxMessageBox("删除的数据不能为空！");//(DEVTYPE = N'11') AND 
			return;
		}
	}
	else if(m_str_list=="dse_barcode_imei")
	{
		if(((Section_Box_IMEIS_CS=="create_time"))  &&  (Start_Time!="")   &&  (End_Time!=""))
			strSql="DELETE FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND ("+Section_Box_IMEIS_CS+" <= CONVERT(DATETIME, '"+End_Time+"', 102)) AND ((WritedFlag IS NULL))";
		else if((Time_Limit_Enable==TRUE)  &&  (Start_Time!="")   &&  (End_Time!="")  &&(Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
		{		
			strSql="DELETE FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+") AND (create_time >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND (create_time <= CONVERT(DATETIME, '"+End_Time+"', 102)) AND ((WritedFlag IS NULL))";
		}
		else if((Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
			strSql="DELETE FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+") AND ((WritedFlag IS NULL))";// WHERE (imei LIKE '[1]%')
		else
		{
			AfxMessageBox("删除的数据不能为空！");//(DEVTYPE = N'11') AND 
			return;
		}
	}
	else
	{
		if(((Section_Box_IMEIS_CS=="create_time"))  &&  (Start_Time!="")   &&  (End_Time!=""))
			strSql="DELETE FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND ("+Section_Box_IMEIS_CS+" <= CONVERT(DATETIME, '"+End_Time+"', 102))";
		else if((Time_Limit_Enable==TRUE)  &&  (Start_Time!="")   &&  (End_Time!="")  &&(Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
		{		
			strSql="DELETE FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+") AND (create_time >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND (create_time <= CONVERT(DATETIME, '"+End_Time+"', 102))";
		}
		else if((Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
			strSql="DELETE FROM "+m_str_list+" WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+")";// WHERE (imei LIKE '[1]%')
		else
		{
			AfxMessageBox("删除的数据不能为空！");//(DEVTYPE = N'11') AND 
			return;
		}
	}

	Execute(strSql,0);
	//清空右侧数据
	m_list1.DeleteAllItems();
	
}

void CDataBase::OnBnClickedButton9()//关闭数据库
{
	// TODO: 在此添加控件通知处理程序代码
	m_pConn->Close();
	m_pConn = NULL;

	m_list1.DeleteAllItems();
	while(m_list2.GetCount())  
		m_list2.DeleteString(0);
	
	Disconnect_DB_Control.EnableWindow(FALSE);
	Connect_DB_For_Find_Control.EnableWindow(TRUE);
	UpdateData(false);
}

void CDataBase::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(RECORD==0)
	{
		AfxMessageBox("没有选择要导出的数据！");
		return;
	}
	CString LOG_Time;
    CTime t = CTime::GetCurrentTime();
	LOG_Time.Format("%02d-%02d-%02d",t.GetHour(),t.GetMinute(),t.GetSecond());

	//system("mkdir \"foldername\"");
	// 打开的路径为本程序所在的路径
    char szName[MAX_PATH] = _T("");
    CString  szPath       = _T("");

    GetModuleFileName(NULL, szName, MAX_PATH);

    szPath = szName;
    szPath = szPath.Mid(0, szPath.ReverseFind('\\')+1);
	CreateDirectory(szPath+m_str_list,NULL);//不会覆盖的，如果目录存在就不会再创建了


	//AfxMessageBox(translate_path+LOG_Time);
	//CSpreadSheet SS(translate_path+LOG_Time+"\\Out.xls", "随机码对应表");

	AfxMessageBox(szPath+m_str_list+"\\"+LOG_Time+"_Out.xls");
	CSpreadSheet SS(szPath+m_str_list+"\\"+LOG_Time+"_Out.xls", "导出数据");
	
	CStringArray *sampleArray, testRow;
	SS.BeginTransaction();
	
	// 加入标题
	//获取字段名strName==================================
	//清空表头
	//while(m_list1.DeleteColumn(0));
	//将字段显示出来
	sampleArray=new CStringArray[RECORD+8];
	sampleArray->RemoveAll();
	for (int cc=0;cc<dataSize;cc++)
	{
		sampleArray->Add(*(strName+cc));
	}
	SS.AddHeaders(*sampleArray);

	////////////////////////////

	int i=0;
	int tt=0; 
	CString str;
	//sampleArray=new CStringArray[RECORD];
	//for(int i=0; i<dataSize; i++)
	for (tt=0;tt<RECORD;tt++)
	{
			//for (tt=0;tt<RECORD;tt++)
			(sampleArray+tt)->RemoveAll();
			for(int i=0; i<dataSize; i++)
			{	
				str =(strdataArray+i)->GetAt(tt);
				(sampleArray+tt)->Add(str);	    
			}	
	}
	for (tt=0;tt<RECORD;tt++)
	{
		SS.AddRow(*(sampleArray+tt));
	}
	/*for (tt=0;tt<RECORD;tt++)
	{
		CString Row_show;
		Row_show=strdataArray->GetAt(tt);
		SS.AddRow(*(strdataArray+tt));           数组strdataArray与数组sampleArray是纵横相反的二维数组
	}*/
	SS.Commit();
	if(tt==RECORD)
	{
		CString Number_temp;
		Number_temp= "N'"+Number+"'";
		AfxMessageBox("导出数据完毕！");
		if((Set_WritedFlag==TRUE)&&(m_str_list=="dse_barcode_imei"))//只支持写号表
		{
			if ( MessageBox ( "导出数据成功完毕\r\n\r\n点击确定--->则继续 写数据库  当前数据的  网标导出标志位\r\n\r\n点击取消--->则数据库中 不 写网标导出标志位！", "温馨确认", MB_OK | MB_OKCANCEL) == IDOK )
			{
				CString strSQL_WritedFlag;
				if((NETCODE_Export_FlagValue!="")&&(_stricmp("NULL",NETCODE_Export_FlagValue)!=0))//不区分大小写，不等于NULL
				{
					if ( MessageBox ( "标志为:"+NETCODE_Export_FlagValue+"\r\n\r\n点击确定--->则继续 \r\n\r\n点击取消--->则取消！", "温馨确认", MB_OK | MB_OKCANCEL) == IDOK )
					{
					
					}
					else
					{
						return;
					}
					if(AllNetCode_Choose==TRUE)//不排除已经写了标志位的数据
					{
						if(((Section_Box_IMEIS_CS=="create_time"))  &&  (Start_Time!="")   &&  (End_Time!=""))
							strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = '"+NETCODE_Export_FlagValue+"' WHERE ("+Section_Box_IMEIS_CS+" >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND ("+Section_Box_IMEIS_CS+" <= CONVERT(DATETIME, '"+End_Time+"', 102))";
						else if((Time_Limit_Enable==TRUE)  &&  (Start_Time!="")   &&  (End_Time!="")  &&(Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
						{		
							strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = '"+NETCODE_Export_FlagValue+"' WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+") AND (create_time >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND (create_time <= CONVERT(DATETIME, '"+End_Time+"', 102))";
						}
						else if((Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
							strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = '"+NETCODE_Export_FlagValue+"' WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+")";// WHERE (imei LIKE '[1]%')
						else
						{
							AfxMessageBox("写网标标志位的数据不能为空！");//(DEVTYPE = N'11') AND 
							return;
						}
					}
					else if(m_str_list=="dse_barcode_imei")
					{
						if(((Section_Box_IMEIS_CS=="create_time"))  &&  (Start_Time!="")   &&  (End_Time!=""))
							strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = '"+NETCODE_Export_FlagValue+"' WHERE ("+Section_Box_IMEIS_CS+" >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND ("+Section_Box_IMEIS_CS+" <= CONVERT(DATETIME, '"+End_Time+"', 102)) AND ((WritedFlag IS NULL))";
						else if((Time_Limit_Enable==TRUE)  &&  (Start_Time!="")   &&  (End_Time!="")  &&(Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
						{		
							strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = '"+NETCODE_Export_FlagValue+"' WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+") AND (create_time >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND (create_time <= CONVERT(DATETIME, '"+End_Time+"', 102)) AND ((WritedFlag IS NULL))";
						}
						else if((Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
							strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = '"+NETCODE_Export_FlagValue+"' WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+") AND ((WritedFlag IS NULL))";// WHERE (imei LIKE '[1]%')
						else
						{
							AfxMessageBox("写网标标志位的数据不能为空！");//(DEVTYPE = N'11') AND 
							return;
						}
					}
					else//没有机会执行的
					{
						if(((Section_Box_IMEIS_CS=="create_time"))  &&  (Start_Time!="")   &&  (End_Time!=""))
							strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = '"+NETCODE_Export_FlagValue+"' WHERE ("+Section_Box_IMEIS_CS+" >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND ("+Section_Box_IMEIS_CS+" <= CONVERT(DATETIME, '"+End_Time+"', 102))";
						else if((Time_Limit_Enable==TRUE)  &&  (Start_Time!="")   &&  (End_Time!="")  &&(Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
						{		
							strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = '"+NETCODE_Export_FlagValue+"' WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+") AND (create_time >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND (create_time <= CONVERT(DATETIME, '"+End_Time+"', 102))";
						}
						else if((Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
							strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = '"+NETCODE_Export_FlagValue+"' WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+")";// WHERE (imei LIKE '[1]%')
						else
						{
							AfxMessageBox("写网标标志位的数据不能为空！");//(DEVTYPE = N'11') AND 
							return;
						}
					}
				}
				else
				{
					if ( MessageBox ( "标志默认为:Y\r\n\r\n点击确定--->则继续 \r\n\r\n点击取消--->则取消！", "温馨确认", MB_OK | MB_OKCANCEL) == IDOK )
					{
					
					}
					else
					{
						return;
					}
					if(AllNetCode_Choose==TRUE)//不排除已经写了标志位的数据
					{
						if(((Section_Box_IMEIS_CS=="create_time"))  &&  (Start_Time!="")   &&  (End_Time!=""))
							strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = 'Y' WHERE ("+Section_Box_IMEIS_CS+" >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND ("+Section_Box_IMEIS_CS+" <= CONVERT(DATETIME, '"+End_Time+"', 102))";
						else if((Time_Limit_Enable==TRUE)  &&  (Start_Time!="")   &&  (End_Time!="")  &&(Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
						{		
							strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = 'Y' WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+") AND (create_time >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND (create_time <= CONVERT(DATETIME, '"+End_Time+"', 102))";
						}
						else if((Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
							strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = 'Y' WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+")";// WHERE (imei LIKE '[1]%')
						else
						{
							AfxMessageBox("写网标标志位的数据不能为空！");//(DEVTYPE = N'11') AND 
							return;
						}
					}
					else if(m_str_list=="dse_barcode_imei")
					{
						if(((Section_Box_IMEIS_CS=="create_time"))  &&  (Start_Time!="")   &&  (End_Time!=""))
							strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = 'Y' WHERE ("+Section_Box_IMEIS_CS+" >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND ("+Section_Box_IMEIS_CS+" <= CONVERT(DATETIME, '"+End_Time+"', 102)) AND ((WritedFlag IS NULL))";
						else if((Time_Limit_Enable==TRUE)  &&  (Start_Time!="")   &&  (End_Time!="")  &&(Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
						{		
							strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = 'Y' WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+") AND (create_time >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND (create_time <= CONVERT(DATETIME, '"+End_Time+"', 102)) AND ((WritedFlag IS NULL))";
						}
						else if((Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
							strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = 'Y' WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+") AND ((WritedFlag IS NULL))";// WHERE (imei LIKE '[1]%')
						else
						{
							AfxMessageBox("写网标标志位的数据不能为空！");//(DEVTYPE = N'11') AND 
							return;
						}
					}
					else//没有机会执行的
					{
						if(((Section_Box_IMEIS_CS=="create_time"))  &&  (Start_Time!="")   &&  (End_Time!=""))
							strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = 'Y' WHERE ("+Section_Box_IMEIS_CS+" >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND ("+Section_Box_IMEIS_CS+" <= CONVERT(DATETIME, '"+End_Time+"', 102))";
						else if((Time_Limit_Enable==TRUE)  &&  (Start_Time!="")   &&  (End_Time!="")  &&(Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
						{		
							strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = 'Y' WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+") AND (create_time >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND (create_time <= CONVERT(DATETIME, '"+End_Time+"', 102))";
						}
						else if((Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
							strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = 'Y' WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+")";// WHERE (imei LIKE '[1]%')
						else
						{
							AfxMessageBox("写网标标志位的数据不能为空！");//(DEVTYPE = N'11') AND 
							return;
						}
					}
				}
				//CString strSQL_WritedFlag="UPDATE dse_imei_sn SET WritedFlag = 'Y' WHERE imei ='"+m_IMEI+"'";
				//Setdlg.myado.OpenSheet("select * from dse_imei_sn WHERE imei ='"+m_IMEI+"'");
				//UP_WritedFlag=Setdlg.myado.Execute(strSQL_WritedFlag);
				//Setdlg.myado.CloseSheet();
				Execute(strSQL_WritedFlag,0);
				if(Wrong==FALSE)
				{
					AfxMessageBox("更新标志成功！");
					Clear_FlagValue_Control.EnableWindow(TRUE);
				}
				else
				{
					AfxMessageBox("更新标志失败！请检查网络是否正常，然后重试！！！（建议：先点击“清除刚写的所有标志位”）");
				}
			}
		}
	}

	//delete sampleArray;//出错
	delete[] sampleArray;
}

void CDataBase::OnBnClickedButton4()
{

	CString Number_temp;
	Number_temp= "N'"+Number+"'";
	if ( MessageBox ( "初始标志默认为:NULL\r\n\r\n点击确定--->则继续 \r\n\r\n点击取消--->则取消！", "温馨确认", MB_OK | MB_OKCANCEL) == IDOK )
	{
	
	}
	else
	{
		return;
	}
	CString strSQL_WritedFlag;
	if(AllNetCode_Choose==TRUE)//不排除已经写了标志位的数据
	{
		if(((Section_Box_IMEIS_CS=="create_time"))  &&  (Start_Time!="")   &&  (End_Time!=""))
			strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = NULL WHERE ("+Section_Box_IMEIS_CS+" >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND ("+Section_Box_IMEIS_CS+" <= CONVERT(DATETIME, '"+End_Time+"', 102))";
		else if((Time_Limit_Enable==TRUE)  &&  (Start_Time!="")   &&  (End_Time!="")  &&(Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
		{		
			strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = NULL WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+") AND (create_time >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND (create_time <= CONVERT(DATETIME, '"+End_Time+"', 102))";
		}
		else if((Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
			strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = NULL WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+")";// WHERE (imei LIKE '[1]%')
		else
		{
			AfxMessageBox("写网标标志位的数据不能为空！");//(DEVTYPE = N'11') AND 
			return;
		}
	}
	else if(m_str_list=="dse_barcode_imei")
	{
		if(((Section_Box_IMEIS_CS=="create_time"))  &&  (Start_Time!="")   &&  (End_Time!=""))
			strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = NULL WHERE ("+Section_Box_IMEIS_CS+" >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND ("+Section_Box_IMEIS_CS+" <= CONVERT(DATETIME, '"+End_Time+"', 102)) AND ((WritedFlag IS NULL))";
		else if((Time_Limit_Enable==TRUE)  &&  (Start_Time!="")   &&  (End_Time!="")  &&(Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
		{		
			strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = NULL WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+") AND (create_time >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND (create_time <= CONVERT(DATETIME, '"+End_Time+"', 102)) AND ((WritedFlag IS NULL))";
		}
		else if((Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
			strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = NULL WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+") AND ((WritedFlag IS NULL))";// WHERE (imei LIKE '[1]%')
		else
		{
			AfxMessageBox("写网标标志位的数据不能为空！");//(DEVTYPE = N'11') AND 
			return;
		}
	}
	else//没有机会执行的
	{
		if(((Section_Box_IMEIS_CS=="create_time"))  &&  (Start_Time!="")   &&  (End_Time!=""))
			strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = NULL WHERE ("+Section_Box_IMEIS_CS+" >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND ("+Section_Box_IMEIS_CS+" <= CONVERT(DATETIME, '"+End_Time+"', 102))";
		else if((Time_Limit_Enable==TRUE)  &&  (Start_Time!="")   &&  (End_Time!="")  &&(Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
		{		
			strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = NULL WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+") AND (create_time >= CONVERT(DATETIME, '"+Start_Time+"', 102)) AND (create_time <= CONVERT(DATETIME, '"+End_Time+"', 102))";
		}
		else if((Number_temp!="N'")  &&(Section_Box_IMEIS_CS!=""))
			strSQL_WritedFlag="UPDATE "+m_str_list+" SET WritedFlag = NULL WHERE ("+Section_Box_IMEIS_CS+" = "+Number_temp+")";// WHERE (imei LIKE '[1]%')
		else
		{
			AfxMessageBox("写网标标志位的数据不能为空！");//(DEVTYPE = N'11') AND 
			return;
		}
	}
	//CString strSQL_WritedFlag="UPDATE dse_imei_sn SET WritedFlag = 'Y' WHERE imei ='"+m_IMEI+"'";
	//Setdlg.myado.OpenSheet("select * from dse_imei_sn WHERE imei ='"+m_IMEI+"'");
	//UP_WritedFlag=Setdlg.myado.Execute(strSQL_WritedFlag);
	//Setdlg.myado.CloseSheet();
	Execute(strSQL_WritedFlag,0);
	if(Wrong==FALSE)
	{
		AfxMessageBox("清除标志成功！重置为“NULL”");
		Clear_FlagValue_Control.EnableWindow(FALSE);
	}
	else
	{
		AfxMessageBox("清除标志失败！请检查网络是否正常，然后重试！！！（建议：先再次点击“清除刚写的所有标志位”）");
	}
}
