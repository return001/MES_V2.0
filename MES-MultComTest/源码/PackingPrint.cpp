// PackingPrint.cpp : 实现文件
//

#include "stdafx.h"
#include "WriteIMEI.h"
#include "PackingPrint.h"


// CPackingPrint 对话框

IMPLEMENT_DYNAMIC(CPackingPrint, CDialog)

CPackingPrint::CPackingPrint(CWnd* pParent /*=NULL*/)
	: CDialog(CPackingPrint::IDD, pParent)
	, BtwFile(_T(""))
	, PrintSetting(_T(""))
{

}

CPackingPrint::~CPackingPrint()
{
}

void CPackingPrint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, BtwFile);
	DDX_Text(pDX, IDC_RESULT2, PrintSetting);
}


BEGIN_MESSAGE_MAP(CPackingPrint, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CPackingPrint::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CPackingPrint::OnBnClickedButtonstart1)
	ON_BN_CLICKED(IDC_BUTTON3, &CPackingPrint::OnBnClickedButton3)
END_MESSAGE_MAP()


// CPackingPrint 消息处理程序
BOOL CPackingPrint::OnInitDialog()//初始化程序
{
	CDialog::OnInitDialog();

	LoadConfig();
	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CPackingPrint::OnBnClickedButton2()//重新打印
{
	// TODO: 在此添加控件通知处理程序代码

	/*CoInitialize(NULL);
	DBtApplication		btApp;														//DBtApplication与IBtApplication都有quit函数
	DBtFormat			btFormat;
	DBtDatabase			btdata;
	btApp.CreateDispatch("BarTender.Application",NULL);
	btFormat.CreateDispatch("BatTender.Format",NULL);
	btdata.CreateDispatch("BarTender.Database",NULL);

	btApp.SetVisible(FALSE);

	DBtFormats* btFormats = new DBtFormats(btApp.GetFormats()); 
	btFormat = btFormats->Open(BtwFile,FALSE,"");

	IBtDatabases* btdatas = new IBtDatabases(btFormat.GetDatabases());
	VARIANT varS; 
	varS.vt = VT_I2; 
	varS.iVal = 1; 
	btdata = btdatas->GetDatabase((const VARIANT&)varS);
	
	DBtOLEDB* dbsql = new DBtOLEDB(btdata.GetOledb());
	dbsql->SetSQLStatement("SELECT * FROM MUFE WHERE [品牌] = '" + strbar + "' and [SKU号码] = '" + skunumber + "'   ");
	

	btFormat.SetIdenticalCopiesOfLabel(1);											//设置打印份数
	
	btFormat.PrintOut(0,0);
	
	btApp.Quit(1);
	
	delete btFormats;
	CoUninitialize();
	*/
	UpdateData(TRUE);
	PrintFunIni();
	PrintFun("111111111111111","15811950061");
	PrintFunEnd();
}
void CPackingPrint::PrintFunIni()//打印功能
{
	GetATSetting(PrintSetting);
	CoInitialize(NULL);
	
	btApp.CreateDispatch("BarTender.Application",NULL);
	
	btApp.SetVisibleWindows(FALSE);
	btFormat.CreateDispatch("BatTender.Format",NULL);
	btFormats = new IBtFormats(btApp.GetFormats()); 
	btFormat = btFormats->Open(BtwFile,FALSE,"");
	
	
}
void CPackingPrint::PrintFunEnd()//打印功能
{
	btApp.Quit(1);
	btApp.DetachDispatch();
	//btApp.ReleaseDispatch()
	btFormat.DetachDispatch();
	delete btFormats;
	CoUninitialize();
}
void CPackingPrint::PrintFun(CString IMEIData,CString PhoneNum)//打印功能
{
	for(int i=0;i<MAX_ORDER_NUM;i++)
	{
		if(OrderDataName[i]=="")
			break;
		if(OrderDataName[i]=="IMEINum")
			btFormat.SetNamedSubStringValue(OrderData[i],IMEIData);
		else if(OrderDataName[i]=="手机号")
			btFormat.SetNamedSubStringValue(OrderData[i],PhoneNum);
		else if((OrderDataRerurn[i]!="")&&(OrderDataRerurn[i]!="NULL"))
			btFormat.SetNamedSubStringValue(OrderData[i],OrderDataRerurn[i]);

		//btFormat.GetNamedSubStringValue(OrderData[i]);								//获取模板文件的数据
		//btFormat.GetPrinter();
		//btFormat.SaveAs("",TRUE);
		//btFormat.SetPrinter();		
	}
	btFormat.SetIdenticalCopiesOfLabel(1);											//设置打印份数
	btFormat.PrintOut(0,0);
	//btFormat.Print("lbc",TRUE,5000,NULL);
}
void CPackingPrint::OnBnClickedButtonstart1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	SaveConfig();
	OnCancel();
}


void  CPackingPrint::SaveConfig()
{
         char szName[MAX_PATH] = _T("");
         CString  szPath       = _T("");

         GetModuleFileName(NULL, szName, MAX_PATH);

         szPath = szName;
         szPath = szPath.Mid(0, szPath.ReverseFind('\\')+1);

		
    CString strLoadFile = szPath+STRING_PATH_CONFIG;

    // 写出INI文件
    CIniFile m_fIniFile(strLoadFile);
		m_fIniFile.WriteString(STRING_SECTION_CONFIG, "BtwFile", BtwFile);
		m_fIniFile.WriteString(STRING_SECTION_CONFIG, "PrintSetting", PrintSetting);
}

void CPackingPrint::LoadConfig()
{

    int nCommandNum   = 0;

    CString strCommand = _T("");
    CString strValue   = _T("");

    CString strName   = _T("");
    CStringArray strCommandAry;

	char szName[MAX_PATH] = _T("");
    CString  szPath       = _T("");
    GetModuleFileName(NULL, szName, MAX_PATH);
    szPath = szName;
    szPath = szPath.Mid(0, szPath.ReverseFind('\\')+1);

    CString strLoadFile = szPath+STRING_PATH_CONFIG;
    CIniFile m_fIniFile(strLoadFile);

    m_fIniFile.ReadSection(STRING_SECTION_CONFIG, strCommandAry);
    nCommandNum = strCommandAry.GetSize();
	for(int i=0;i<nCommandNum;i++)
	{
        strCommand = strCommandAry[i];//编号
        if(strCommand == "BtwFile")
		{
			BtwFile = m_fIniFile.ReadString(STRING_SECTION_CONFIG, strCommand, "");
		}
		else if(strCommand == "PrintSetting")
		{
			PrintSetting = m_fIniFile.ReadString(STRING_SECTION_CONFIG, strCommand, "");
		}
	}
}

void CPackingPrint::OnOK()
{
	return;
	CDialog::OnOK();
}

BOOL CPackingPrint::GetATSetting(CString SettingChar)
{
	CString strValue;
	SettingChar.Replace(" ","");
	SettingChar.Replace("\r","");
	SettingChar.Replace("\n","");
	strValue=SettingChar;
	for(int i=0;i<MAX_ORDER_NUM;i++)
	{
		int npos=strValue.Find('@');
		if(npos>=0)
		{
			OrderDataName[i]= strValue.Left(npos);
			strValue    = strValue.Mid(npos+1);

			npos=strValue.Find('@');
			if(npos>=0)
			{
				OrderData[i]= strValue.Left(npos);
				strValue    = strValue.Mid(npos+1);

				npos=strValue.Find('}');
				if(npos>=0)
				{
					OrderDataRerurn[i]= strValue.Left(npos);
					strValue    = strValue.Mid(npos+1);
				}
				else
				{
					OrderDataRerurn[i]= "";
				}
			}
			else
			{
				OrderData[i]="";
				OrderDataRerurn[i]= "";
			}
		}
		else
		{
			OrderDataName[i]="";
			OrderData[i]="";
			OrderDataRerurn[i]= "";
		}
	}
	return TRUE;
}
void CPackingPrint::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (_access("C:\\Program Files\\Seagull\\BarTender Suite\\BarTender\\bartend.exe",0)==-1)//文件夹不存在，新建
	{
		AfxMessageBox("C:\\Program Files\\Seagull\\BarTender Suite\\BarTender\\bartend.exe不存在！");
	}
	else
	{
		if(BtwFile.Find(".btw")==-1)
			AfxMessageBox("文件不存在");
		else
			ShellExecute(NULL,"open","C:\\Program Files\\Seagull\\BarTender Suite\\Barterder\\bartend.exe",BtwFile,NULL,SW_SHOWNORMAL);
	}

	//"C:\\Program Files\\Seagull\\BarTender Suite\\bartend.exe"
}
