// Para1.cpp : 实现文件
//

#include "stdafx.h"
#include "WriteIMEI.h"
#include "Para1.h"
#include "WriteIMEIDlg.h"

// CPara1 对话框

IMPLEMENT_DYNAMIC(CPara1, CDialog)

CPara1::CPara1(CWnd* pParent /*=NULL*/)
	: CDialog(CPara1::IDD, pParent)
{

}

CPara1::~CPara1()
{
}

void CPara1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM2, m_list_area_para);
}


BEGIN_MESSAGE_MAP(CPara1, CDialog)
	ON_NOTIFY(RVN_ENDITEMEDIT, IDC_CUSTOM2, OnRvnEndItemEdit)
	ON_NOTIFY(RVN_ITEMCLICK, IDC_CUSTOM2, OnRvnItemClick)
END_MESSAGE_MAP()


BOOL CPara1::OnInitDialog() 
{
	CDialog::OnInitDialog();
	int i = 0;
	RVITEM rvi;
	// 增加竖线和底色
	m_list_area_para.ModifyStyle(0, 
		RVS_SHOWHGRID|
		RVS_SHOWVGRID|
//		RVS_SHOWCOLORALTERNATE|
		RVS_SINGLESELECT|
		RVS_NOSORT|
		RVS_FOCUSSUBITEMS
		);


	m_list_area_para.InsertColumn(0, _T("No"), 
								RVCF_CENTER|
								RVCF_TEXT|
								RVCF_EX_AUTOWIDTH|
								RVCF_EX_FIXEDWIDTH|
								RVCF_SUBITEM_NOFOCUS
								);

	m_list_area_para.InsertColumn(1, _T("选取"),
								RVCF_CENTER|
								RVCF_TEXT |
								RVCF_EX_AUTOWIDTH);

	m_list_area_para.InsertColumn(2, _T("测试项目"),
								RVCF_LEFT|
								RVCF_TEXT,//|RVCF_SUBITEM_NOFOCUS,
								150);


	m_list_area_para.InsertColumn(3, _T("返回值"),
								RVCF_CENTER|
								RVCF_TEXT
								,160);
	m_list_area_para.InsertColumn(4, _T("可调参数"),
								RVCF_CENTER|
								RVCF_TEXT
								,160);
	m_list_area_para.InsertColumn(5, _T("指令或说明"),
								RVCF_LEFT|
								RVCF_TEXT
								,300);
	LoadParaList("");

	//m_list_area_para.OnRButtonDown()
    UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// CPara1 消息处理程序
void CPara1::OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 如果edit编辑结束，设置通知更改了数据
	//m_pParentWnd->m_bDataChanged = TRUE;
}

void CPara1::OnRvnItemClick(NMHDR* pNMHDR, LRESULT* pResult)//list中check状态的控制
{
	LPNMREPORTVIEW lpnmrv = (LPNMREPORTVIEW)pNMHDR;

	if(lpnmrv->iItem>=0 && lpnmrv->iSubItem>=0)
	{
		RVITEM rvi;
		rvi.iItem = lpnmrv->iItem;
		rvi.iSubItem = lpnmrv->iSubItem;
		m_list_area_para.GetItem(&rvi);

		if(rvi.nMask&RVIM_CHECK && lpnmrv->nFlags&RVHT_ONITEMCHECK)
		{
			rvi.iCheck = rvi.iCheck ? 0:1;

			// 如果Check状态被更改，设置通知更改了数据
		//	m_pParentWnd->m_bDataChanged = TRUE;
		}
		m_list_area_para.SetItem(&rvi);
	}

	*pResult = FALSE;
}

void CPara1::ImportListData(int paraNum,PARAMETER paraArray[])
{
	RVITEM rvi;
	char pszTemp[3];
    m_list_area_para.DeleteAllItems();//清空列表
    for(int i=0;i<paraNum;i++)
    {
       //if(paraArray[i].ifCheck==true)
       {

			 _itoa( i+1, pszTemp, 10 );
			 m_list_area_para.InsertItem(i,pszTemp);//插入行
		
			 rvi.iItem = i;
			 rvi.nMask = RVIM_CHECK;//插入checkbox
			 rvi.iSubItem = 1;
			 if(paraArray[i].ifCheck==true)
				rvi.iCheck = 1;
			 else
				rvi.iCheck = 0;
			 m_list_area_para.SetItem(&rvi);

			 rvi.nMask = RVIM_TEXT;//插入edittext
			 rvi.iSubItem = 2;
			 rvi.lpszText = (LPTSTR)(LPCTSTR)paraArray[i].showName;
			 m_list_area_para.SetItem(&rvi);
		
			 rvi.iSubItem = 3;
			 rvi.lpszText = (LPTSTR)(LPCTSTR)paraArray[i].Low_Limit_Value;
			 m_list_area_para.SetItem(&rvi);

			 rvi.iSubItem = 4;
			 rvi.lpszText = (LPTSTR)(LPCTSTR)paraArray[i].High_Limit_Value;
			 m_list_area_para.SetItem(&rvi);

			 rvi.iSubItem = 5;
			 rvi.lpszText = (LPTSTR)(LPCTSTR)paraArray[i].Other_ITEM;
			 m_list_area_para.SetItem(&rvi);
		}
	} 
}

int CPara1::LoadIniFile(CString strLoadFile,PARAMETER paraArray[])
{

  // 读入INI文件
    ASSERT(!strLoadFile.IsEmpty());
    CIniFile m_fIniFile(strLoadFile);
    int i             = 0;
    char aText[140]={0};
	
    int nCommand      = 0;
    int nCommandNum   = 0;
	RVITEM rvi;

    CString strCommand = _T("");
    CString strValue        = _T("");//测试上下限值
	CString Check_item      = _T("");//是否选中
	CString strValue_low    = _T("");//测试下限值
	CString strValue_high   = _T("");//测试上限值
	CString strValue_OtherITEM   = _T("");//其他自定义


    CStringArray strCommandAry;

    m_fIniFile.ReadSection(STRING_SECTION_BLE, strCommandAry);
    nCommandNum = strCommandAry.GetSize();

    if (nCommandNum == 0 )
    {
		CString strInfo;
        strInfo = _T("Imported script file don't have setting data!");
        AfxMessageBox(strInfo);
	    return 0;
    }
    else
    {		
        for (i= 0; i< nCommandNum; i++)         //对象数组的数量
        {
			strCommand = strCommandAry[i];      //编号
			paraArray[i].showName=strCommand;   //具体测试项

            strValue   = m_fIniFile.ReadString(STRING_SECTION_BLE, strCommand, "");//编号对应的内容
			
			int npos=strValue.Find("@@");
			if(npos>=0)
			{
				Check_item  = strValue.Left(npos);
				strValue    = strValue.Mid(npos+2);
			}
			npos=strValue.Find("@@");
			if(npos>=0)
			{
				strValue_low  = strValue.Left(npos);
				strValue      = strValue.Mid(npos+2);
			}
			npos=strValue.Find("@@");
			if(npos>=0)
			{
				strValue_high  = strValue.Left(npos);
				strValue_OtherITEM = strValue.Mid(npos+2);
			}
			else
			{
				strValue_high  = strValue;
				strValue_OtherITEM="null";
			}

			paraArray[i].Low_Limit_Value = strValue_low;	//期望上下限值
			paraArray[i].High_Limit_Value =strValue_high;	//期望上下限值
			paraArray[i].Other_ITEM =strValue_OtherITEM;	//其他自定义
			if(Check_item=="true")
				paraArray[i].ifCheck=true;           //选中       
			else
				paraArray[i].ifCheck=false;          //没选中       
				
        }
		return nCommandNum;
    }
}
//装载设置
void CPara1::LoadParaList(CString strLoadFile)
{
	UpdateData(TRUE) ;
    char szName[MAX_PATH] = _T("");
    CString  szPath       = _T("");
    GetModuleFileName(NULL, szName, MAX_PATH);
    szPath = szName;
    szPath = szPath.Mid(0, szPath.ReverseFind('\\')+1);

	strLoadFile=szPath+STRING_PATH_BLE;

	CIniFile tmpread(strLoadFile);
	
	//int i, j;
	//CString a, b;
	//i=strLoadFile.Find(".");
	//j=strLoadFile.GetLength();
	//a = strLoadFile.Left(i);
	//b = strLoadFile.Right(j-i-2);
	//strLoadFile = a + b;

	fSectionName=tmpread.GetParaName();
 	if(fSectionName=="BLE")
 	{
		numParameter=LoadIniFile(strLoadFile,parameterArray); 
	}
 	ImportListData(numParameter,parameterArray);
	
}


//保存设置
void CPara1::SaveBLE_Setting(CString strLoadFile)
{
	

	//UpdateData(TRUE) ;//必须屏蔽
    char szName[MAX_PATH] = _T("");
    CString  szPath       = _T("");
    GetModuleFileName(NULL, szName, MAX_PATH);
    szPath = szName;
    szPath = szPath.Mid(0, szPath.ReverseFind('\\')+1);

	strLoadFile=szPath+STRING_PATH_BLE;
	DeleteFile(strLoadFile);
	int num =  ExportListData();

	SaveIniFile(strLoadFile, num,parameterArray);
}

int CPara1::ExportListData(void)//(PARAMETER exportArray[])	//导出(其在列表中位置一定不能发生改变)
{
	//int num=0;
	//int j=0;
	numParameter=m_list_area_para.GetItemCount();
    for(int i=0;i<numParameter;i++)
    {
		   parameterArray[i].ifCheck= m_list_area_para.GetItemCheck(i, 1)?true:false;
           parameterArray[i].showName=m_list_area_para.GetItemText(i, 2);
		   parameterArray[i].Low_Limit_Value=m_list_area_para.GetItemText(i, 3);
		   parameterArray[i].High_Limit_Value=m_list_area_para.GetItemText(i, 4);
		   parameterArray[i].Other_ITEM=m_list_area_para.GetItemText(i, 5);
	}
    return numParameter;
}

void CPara1::SaveIniFile(CString strSaveFile,int paraNum,PARAMETER paraArray[])
{
	ASSERT(!strSaveFile.IsEmpty());
    int  nIndex = 0;
    int  nItem  = 0;
    // 写出INI文件
    CIniFile m_fIniFile(strSaveFile);
	CString para_temp;
    for(int i=0;i<paraNum ;i++)
    {
		if(paraArray[i].ifCheck==true)
			para_temp="true@@"+paraArray[i].Low_Limit_Value+ "@@"+ paraArray[i].High_Limit_Value+ "@@"+ paraArray[i].Other_ITEM;			//选中       
		else
			para_temp="false@@"+paraArray[i].Low_Limit_Value+ "@@"+ paraArray[i].High_Limit_Value+ "@@"+ paraArray[i].Other_ITEM;			//没选中       
		m_fIniFile.WriteString(STRING_SECTION_BLE, paraArray[i].showName, para_temp);
	}
}
