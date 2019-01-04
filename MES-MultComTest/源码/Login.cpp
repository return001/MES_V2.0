// Login.cpp : 实现文件
//

#include "stdafx.h"
#include "WriteIMEI.h"
#include "Login.h"

#include "BT_Addr_Setting.h"
#include "WriteIMEIDlg.h"
#include "UserSetting.h"

// CLogin 对话框

IMPLEMENT_DYNAMIC(CLogin, CDialog)

CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CLogin::IDD, pParent)
	, m_s_name(_T(""))
	, m_s_pwd(_T(""))
{

}

CLogin::~CLogin()
{
}

void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_s_name);
	DDX_Text(pDX, IDC_EDIT2, m_s_pwd);
}


BEGIN_MESSAGE_MAP(CLogin, CDialog)
	ON_BN_CLICKED(IDOK, &CLogin::OnBnClickedOk)
END_MESSAGE_MAP()


// CLogin 消息处理程序
CBT_Addr_Setting Setdlg_login;

void CLogin::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CBT_Addr_Setting Setdlg_login;
	Setdlg_login.DoModal();
	UpdateData(TRUE) ;
    CWriteIMEIDlg*  pdlg=(CWriteIMEIDlg*)GetParent();


	CString LimitsTemp="";

	if (m_s_name.IsEmpty() || m_s_pwd.IsEmpty())
	{	
		MessageBox("用户名或密码不能为空","用户登录信息",MB_ICONWARNING);	 	
	}
	//else if (m_s_name=="desay"&& m_s_pwd=="desay_unlock")
	else if (m_s_name=="xx"&& m_s_pwd=="12")
	{
		   pdlg->m_Auto.EnableWindow(true);
		   //pdlg->Plat_Form.EnableWindow(true);
		   pdlg->Modem_Database_Control.EnableWindow(true);
		   pdlg->AP_Database_Control.EnableWindow(true);

		   pdlg->Manul_Print_Control.EnableWindow(true);

		   pdlg->DB_Find_Control.EnableWindow(true);

		   pdlg->Wait_time_Control.EnableWindow(true);
		   pdlg->SaveCurrentSetting_Control.EnableWindow(true);

		   pdlg->IMEI_PRE_Control.EnableWindow(true);
		   pdlg->SN_PRE_Control.EnableWindow(true);

		   pdlg->BT_PRE_Control.EnableWindow(true);
		   pdlg->WIFI_PRE_Control.EnableWindow(true);

		   pdlg->PrinterScript_Control.EnableWindow(true);

		   pdlg->NETCODE_PRE_Control.EnableWindow(true);  //网标前缀  
		   pdlg->NETCODE_Count_Control.EnableWindow(true);//网标位数

		   pdlg->BtAddrCount_Control.EnableWindow(true);
		   pdlg->IMEI_Count_Control.EnableWindow(true);

		   pdlg->SN_Count_Control.EnableWindow(true);
		   pdlg->m_Type.EnableWindow(true);
		   pdlg->Plat_Form.EnableWindow(true);//平台选择

		   pdlg->MSN_Relative_IMEI_Control.EnableWindow(true);
		   pdlg->BT_Write_Enable_Control.EnableWindow(true);
		   pdlg->Recover_Status_Control.EnableWindow(true);

		   pdlg->CHECK_GSM_TEST_Enable_Control.EnableWindow(true);
		   pdlg->CHECK_BT_TEST_Enable_Control.EnableWindow(true);
		   //BT
		   Setdlg_login.BtAddrStart=pdlg->BtAddrStart_M;
		   Setdlg_login.BtAddrEnd=pdlg->BtAddrEnd_M;
		   //WIFI
		   Setdlg_login.WIFIAddrStart=pdlg->WIFIAddrStart_M;
		   Setdlg_login.WIFIAddrEnd=pdlg->WIFIAddrEnd_M;
		   //网标 
		   Setdlg_login.NetCodeStart=pdlg->NetCodeStart_M;
		   Setdlg_login.NetCodeEnd=pdlg->NetCodeEnd_M;
		   Setdlg_login.NetCode_SwiftNumber=pdlg->NetCode_SwiftNumber_M;
		   //IMEI
		   Setdlg_login.IMEIStart=pdlg->IMEIStart_M;
		   Setdlg_login.IMEIEnd=pdlg->IMEIEnd_M;

		   if(pdlg->UP_DB_Data_M=="true")//9
				Setdlg_login.UP_DB_Data=TRUE;
		   else
				Setdlg_login.UP_DB_Data=FALSE;

		   if(pdlg->Initial_Connect_DB_M=="true")//10
				Setdlg_login.Initial_Connect_DB=TRUE;
		   else
				Setdlg_login.Initial_Connect_DB=FALSE;

		   if(pdlg->DB_CONNECT_M=="true")//11
				Setdlg_login.DB_CONNECT=TRUE;
		   else
				Setdlg_login.DB_CONNECT=FALSE;

		   if(pdlg->IMEI1_Choose_M=="true")//12
				Setdlg_login.IMEI1_Choose=TRUE;
		   else
				Setdlg_login.IMEI1_Choose=FALSE;

		   if(pdlg->IMEI2_Choose_M=="true")//13
				Setdlg_login.IMEI2_Choose=TRUE;
		   else
				Setdlg_login.IMEI2_Choose=FALSE;
		   if(pdlg->IMEI3_Choose_M=="true")//14
				Setdlg_login.IMEI3_Choose=TRUE;
		   else
				Setdlg_login.IMEI3_Choose=FALSE;
		   if(pdlg->IMEI4_Choose_M=="true")//15
				Setdlg_login.IMEI4_Choose=TRUE;
		   else
				Setdlg_login.IMEI4_Choose=FALSE;

		   if(pdlg->IMEI_CheckValid_Choose_M=="true")//18检查IMEI是否合法
				Setdlg_login.IMEI_CheckValid_Choose=TRUE;
		   else
				Setdlg_login.IMEI_CheckValid_Choose=FALSE;
		   if(pdlg->AutoIMEI_Choose_M=="true")//19自动生成IMEI
				Setdlg_login.AutoIMEI_Choose=TRUE;
		   else
				Setdlg_login.AutoIMEI_Choose=FALSE;

		   if(pdlg->UseSameIMEI_Choose_M=="true")//20
				Setdlg_login.UseSameIMEI_Choose=TRUE;
		   else
				Setdlg_login.UseSameIMEI_Choose=FALSE;

		   if(pdlg->IMEI_DB_Choose_M=="true")//23
				Setdlg_login.IMEI_DB_Choose=TRUE;
		   else
				Setdlg_login.IMEI_DB_Choose=FALSE;//使用数据库关联
		   if(pdlg->MAC_DB_Choose_M=="true")//24
				Setdlg_login.MAC_DB_Choose=TRUE;
		   else
				Setdlg_login.MAC_DB_Choose=FALSE;

		   if(pdlg->MSN_Upload_Choose_M=="true")//25
				Setdlg_login.MSN_Upload_Choose=TRUE;
		   else
				Setdlg_login.MSN_Upload_Choose=FALSE;

		   if(pdlg->ORACLE_Connect_Choose_M=="true")//27
				Setdlg_login.ORACLE_Connect_Choose=TRUE;
		   else
				Setdlg_login.ORACLE_Connect_Choose=FALSE;

		   if(pdlg->Check_repeat_Choose_M=="true")//26
				Setdlg_login.Check_repeat_Choose=TRUE;
		   else
				Setdlg_login.Check_repeat_Choose=FALSE;

		   if(pdlg->AutoBTAddr_Choose_M=="true")//21
				Setdlg_login.AutoBTAddr_Choose=TRUE;
		   else
				Setdlg_login.AutoBTAddr_Choose=FALSE;

		   if(pdlg->AutoWIFIAddr_Choose_M=="true")//22
				Setdlg_login.AutoWIFIAddr_Choose=TRUE;
		   else
				Setdlg_login.AutoWIFIAddr_Choose=FALSE;


		   if(pdlg->BT_MAC_ADDR_Choose_M=="true")//16
				Setdlg_login.BT_MAC_ADDR_Choose=TRUE;
		   else
				Setdlg_login.BT_MAC_ADDR_Choose=FALSE;

		   if(pdlg->WIFI_MAC_ADDR_Choose_M=="true")//17
				Setdlg_login.WIFI_MAC_ADDR_Choose=TRUE;
		   else
				Setdlg_login.WIFI_MAC_ADDR_Choose=FALSE;

		   if(pdlg->BARCODE_Choose_M=="true")//22
				Setdlg_login.BARCODE_Choose=TRUE;
		   else
				Setdlg_login.BARCODE_Choose=FALSE; 

		   if(pdlg->ChangeEN_Choose_M=="true")//23
				Setdlg_login.ChangeEN_Choose=TRUE;
		   else
				Setdlg_login.ChangeEN_Choose=FALSE;

		   if(pdlg->Unlock_Code_Choose_M=="true")//24
				Setdlg_login.Unlock_Code_Choose=TRUE;
		   else
				Setdlg_login.Unlock_Code_Choose=FALSE;

		   if(pdlg->NETCODE_Choose_M=="true")//25
				Setdlg_login.NETCODE_Choose=TRUE;
		   else
				Setdlg_login.NETCODE_Choose=FALSE;

		   if(pdlg->NETCODE_Only_Num_Choose_M=="true")//26
				Setdlg_login.NETCODE_Only_Num_Choose=TRUE;
		   else
				Setdlg_login.NETCODE_Only_Num_Choose=FALSE;

		   if(pdlg->JUST_UpDate_DB_Choose_M=="true")//27
				Setdlg_login.JUST_UpDate_DB_Choose=TRUE;
		   else
				Setdlg_login.JUST_UpDate_DB_Choose=FALSE;

		   if(pdlg->UpdateIMEI_MSN_Choose_M=="true")//28
				Setdlg_login.UpdateIMEI_MSN_Choose=TRUE;
		   else
				Setdlg_login.UpdateIMEI_MSN_Choose=FALSE;

		   if(pdlg->IMEI_Exist_Update_NC_Choose_M=="true")//29
				Setdlg_login.IMEI_Exist_Update_NC_Choose=TRUE;
		   else
				Setdlg_login.IMEI_Exist_Update_NC_Choose=FALSE;

		   if(pdlg->Machine_Type_NC_CompareChoose_M=="true")//30
				Setdlg_login.Machine_Type_NC_CompareChoose=TRUE;
		   else
				Setdlg_login.Machine_Type_NC_CompareChoose=FALSE;


		   if(pdlg->Software_Version_Check_Choose_M=="true")//18
				Setdlg_login.Software_Version_Check_Choose=TRUE;
		   else
				Setdlg_login.Software_Version_Check_Choose=FALSE;
		   if(pdlg->IMEI_MAC_Check_Choose_M=="true")//19
				Setdlg_login.IMEI_MAC_Check_Choose=TRUE;
		   else
				Setdlg_login.IMEI_MAC_Check_Choose=FALSE;

		   if(pdlg->Scan_Choose_M=="true")//20
				Setdlg_login.Scan_Choose=TRUE;
		   else
				Setdlg_login.Scan_Choose=FALSE;

		   if(pdlg->Repeat_AutoStart_M=="true")//21
				Setdlg_login.Repeat_AutoStart=TRUE;
		   else
				Setdlg_login.Repeat_AutoStart=FALSE;
	  
		   //上传信息更新
		   Setdlg_login.Order_Num_up=pdlg->Order_Num_up_M;
		   Setdlg_login.Machine_Type=pdlg->Machine_Type_M;
		   Setdlg_login.Modulation_Tppe=pdlg->Modulation_Tppe_M;
		   Setdlg_login.Line_Number=pdlg->Line_Number_M;
		   Setdlg_login.Floor=pdlg->Floor_M;
		   Setdlg_login.Product_Address=pdlg->Product_Address_M;
		   Setdlg_login.Is_Return=pdlg->Is_Return_M;
		   Setdlg_login.Software_Version=pdlg->Software_Version_M;
		   Setdlg_login.Hardware_Version=pdlg->Hardware_Version_M;

		   Setdlg_login.ServerIP_MAC=pdlg->ServerIP_MAC_M;
		   Setdlg_login.WatchDog_Time=pdlg->WatchDog_Time_M;//超时时间设置

		   Setdlg_login.AutoIMEI_Start_Num=pdlg->AutoIMEI_Start_Num_M;
		   Setdlg_login.AutoWIFIAd_Start_Num=pdlg->AutoWIFIAd_Start_Num_M;

		   Setdlg_login.DestFile =pdlg->DestFile_M;

		   Setdlg_login.Operator=pdlg->Operator_M;

		   Setdlg_login.m_server=pdlg->m_server_M;
		   Setdlg_login.m_db=pdlg->m_db_M;
		   Setdlg_login.m_user=pdlg->m_user_M;
		   Setdlg_login.m_pwd=pdlg->m_pwd_M;

		   Setdlg_login.ORACLE_m_server=pdlg->ORACLE_m_server_M;
		   Setdlg_login.ORACLE_m_user=pdlg->ORACLE_m_user_M;
		   Setdlg_login.ORACLE_m_pwd=pdlg->ORACLE_m_pwd_M;

		   Setdlg_login.BT_Addr_EndShow=pdlg->BT_Addr_EndShow_M;
		
		   Setdlg_login.TestMode_S=pdlg->TestMode_M;
		   Setdlg_login.Unlock_code_S=pdlg->Unlock_code_M;
		   ShowWindow(SW_HIDE);		   
		   Setdlg_login.DoModal();   
		   UpdateData(true);      //将控件的值赋给变量
		   
		   pdlg->BtAddrStart_M=Setdlg_login.BtAddrStart;
		   pdlg->BtAddrEnd_M =Setdlg_login.BtAddrEnd;
		   pdlg->BtAddrTotal=pdlg->BtAddrEnd_M;

		   pdlg->WIFIAddrStart_M=Setdlg_login.WIFIAddrStart;
		   pdlg->WIFIAddrEnd_M=Setdlg_login.WIFIAddrEnd;

		   pdlg->NetCodeStart_M=Setdlg_login.NetCodeStart;
		   pdlg->NetCodeEnd_M=Setdlg_login.NetCodeEnd;

		   pdlg->NetCode_SwiftNumber_M=Setdlg_login.NetCode_SwiftNumber;

		   pdlg->IMEIStart_M=Setdlg_login.IMEIStart;
		   pdlg->IMEIEnd_M=Setdlg_login.IMEIEnd;

		   if(Setdlg_login.UP_DB_Data==TRUE)//9
				pdlg->UP_DB_Data_M="true";
		   else
				pdlg->UP_DB_Data_M="false";

		   if(Setdlg_login.Initial_Connect_DB==TRUE)//10
				pdlg->Initial_Connect_DB_M="true";
		   else
				pdlg->Initial_Connect_DB_M="false";
		   if(Setdlg_login.DB_CONNECT==TRUE)//11
				pdlg->DB_CONNECT_M="true";
		   else
				pdlg->DB_CONNECT_M="false";

		   if(Setdlg_login.IMEI1_Choose==TRUE)//12
				pdlg->IMEI1_Choose_M="true";
		   else
				pdlg->IMEI1_Choose_M="false";

		   if(Setdlg_login.IMEI2_Choose==TRUE)//13
				pdlg->IMEI2_Choose_M="true";
		   else
				pdlg->IMEI2_Choose_M="false";
		   if(Setdlg_login.IMEI3_Choose==TRUE)//14
				pdlg->IMEI3_Choose_M="true";
		   else
				pdlg->IMEI3_Choose_M="false";
		   if(Setdlg_login.IMEI4_Choose==TRUE)//15
				pdlg->IMEI4_Choose_M="true";
		   else
				pdlg->IMEI4_Choose_M="false";

		   if(Setdlg_login.IMEI_CheckValid_Choose==TRUE)//18
				pdlg->IMEI_CheckValid_Choose_M="true";
		   else
				pdlg->IMEI_CheckValid_Choose_M="false";

		   if(Setdlg_login.AutoIMEI_Choose==TRUE)//19
				pdlg->AutoIMEI_Choose_M="true";
		   else
				pdlg->AutoIMEI_Choose_M="false";
		   if(Setdlg_login.UseSameIMEI_Choose==TRUE)//20
				pdlg->UseSameIMEI_Choose_M="true";
		   else
				pdlg->UseSameIMEI_Choose_M="false";

		   if(Setdlg_login.IMEI_DB_Choose==TRUE)//23
				pdlg->IMEI_DB_Choose_M="true";
		   else
				pdlg->IMEI_DB_Choose_M="false";

		   if(Setdlg_login.MAC_DB_Choose==TRUE)//24
				pdlg->MAC_DB_Choose_M="true";
		   else
				pdlg->MAC_DB_Choose_M="false";
		   if(Setdlg_login.MSN_Upload_Choose==TRUE)//25
				pdlg->MSN_Upload_Choose_M="true";
		   else
				pdlg->MSN_Upload_Choose_M="false";

		   if(Setdlg_login.ORACLE_Connect_Choose==TRUE)//27
				pdlg->ORACLE_Connect_Choose_M="true";
		   else
				pdlg->ORACLE_Connect_Choose_M="false";

		   if(Setdlg_login.Check_repeat_Choose==TRUE)//26
				pdlg->Check_repeat_Choose_M="true";
		   else
				pdlg->Check_repeat_Choose_M="false";

		   if(Setdlg_login.AutoBTAddr_Choose==TRUE)//21
				pdlg->AutoBTAddr_Choose_M="true";
		   else
				pdlg->AutoBTAddr_Choose_M="false";

		   if(Setdlg_login.AutoWIFIAddr_Choose==TRUE)//22
				pdlg->AutoWIFIAddr_Choose_M="true";
		   else
				pdlg->AutoWIFIAddr_Choose_M="false";

		   if(Setdlg_login.BT_MAC_ADDR_Choose==TRUE)//16 
				pdlg->BT_MAC_ADDR_Choose_M="true";
		   else
				pdlg->BT_MAC_ADDR_Choose_M="false";

		   if(Setdlg_login.WIFI_MAC_ADDR_Choose==TRUE)//17
				pdlg->WIFI_MAC_ADDR_Choose_M="true";
		   else
				pdlg->WIFI_MAC_ADDR_Choose_M="false";

		   if(Setdlg_login.BARCODE_Choose==TRUE)//22 
				pdlg->BARCODE_Choose_M="true";
		   else
				pdlg->BARCODE_Choose_M="false";

		   if(Setdlg_login.ChangeEN_Choose==TRUE)//23
				pdlg->ChangeEN_Choose_M="true";
		   else
				pdlg->ChangeEN_Choose_M="false";

		   if(Setdlg_login.Unlock_Code_Choose==TRUE)//24
				pdlg->Unlock_Code_Choose_M="true";
		   else
				pdlg->Unlock_Code_Choose_M="false";

		   if(Setdlg_login.NETCODE_Choose==TRUE)//25
				pdlg->NETCODE_Choose_M="true";
		   else
				pdlg->NETCODE_Choose_M="false";

		   if(Setdlg_login.NETCODE_Only_Num_Choose==TRUE)//26
				pdlg->NETCODE_Only_Num_Choose_M="true";
		   else
				pdlg->NETCODE_Only_Num_Choose_M="false";

		   if(Setdlg_login.JUST_UpDate_DB_Choose==TRUE)//27
				pdlg->JUST_UpDate_DB_Choose_M="true";
		   else
				pdlg->JUST_UpDate_DB_Choose_M="false";

		   if(Setdlg_login.UpdateIMEI_MSN_Choose==TRUE)//28
				pdlg->UpdateIMEI_MSN_Choose_M="true";
		   else
				pdlg->UpdateIMEI_MSN_Choose_M="false";

		   if(Setdlg_login.IMEI_Exist_Update_NC_Choose==TRUE)//29
				pdlg->IMEI_Exist_Update_NC_Choose_M="true";
		   else
				pdlg->IMEI_Exist_Update_NC_Choose_M="false";

		   if(Setdlg_login.Machine_Type_NC_CompareChoose==TRUE)//30
				pdlg->Machine_Type_NC_CompareChoose_M="true";
		   else
				pdlg->Machine_Type_NC_CompareChoose_M="false";

		   if(Setdlg_login.Software_Version_Check_Choose==TRUE)//18
				pdlg->Software_Version_Check_Choose_M="true";
		   else
				pdlg->Software_Version_Check_Choose_M="false";

		   if(Setdlg_login.IMEI_MAC_Check_Choose==TRUE)//19
				pdlg->IMEI_MAC_Check_Choose_M="true";
		   else
				pdlg->IMEI_MAC_Check_Choose_M="false";
		   if(Setdlg_login.Scan_Choose==TRUE)//20
				pdlg->Scan_Choose_M="true";
		   else
				pdlg->Scan_Choose_M="false";

		   if(Setdlg_login.Repeat_AutoStart==TRUE)//21
				pdlg->Repeat_AutoStart_M="true";
		   else
				pdlg->Repeat_AutoStart_M="false";

		   pdlg->Order_Num_up_M=Setdlg_login.Order_Num_up;
		   pdlg->Machine_Type_M=Setdlg_login.Machine_Type;
		   pdlg->Machine_Type=pdlg->Machine_Type_M;//Machine_Type为界面显示机型
		   pdlg->Modulation_Tppe_M=Setdlg_login.Modulation_Tppe;
		   pdlg->Line_Number_M=Setdlg_login.Line_Number;
		   pdlg->Floor_M=Setdlg_login.Floor;
		   pdlg->Product_Address_M=Setdlg_login.Product_Address;
		   pdlg->Is_Return_M=Setdlg_login.Is_Return;
		   pdlg->Software_Version_M=Setdlg_login.Software_Version;
		   pdlg->Hardware_Version_M=Setdlg_login.Hardware_Version;

		   pdlg->ServerIP_MAC_M=Setdlg_login.ServerIP_MAC;

		   pdlg->WatchDog_Time_M=Setdlg_login.WatchDog_Time;
		   pdlg->AutoIMEI_Start_Num_M=Setdlg_login.AutoIMEI_Start_Num;
		   pdlg->AutoWIFIAd_Start_Num_M=Setdlg_login.AutoWIFIAd_Start_Num;

		   pdlg->DestFile_M=Setdlg_login.DestFile;

		   pdlg->Operator_M=Setdlg_login.Operator;

		   pdlg->m_server_M=Setdlg_login.m_server;
		   pdlg->m_db_M=Setdlg_login.m_db;
		   pdlg->m_user_M=Setdlg_login.m_user;
		   pdlg->m_pwd_M=Setdlg_login.m_pwd;

		   pdlg->ORACLE_m_server_M=Setdlg_login.ORACLE_m_server;
		   pdlg->ORACLE_m_user_M=Setdlg_login.ORACLE_m_user;
		   pdlg->ORACLE_m_pwd_M=Setdlg_login.ORACLE_m_pwd;

		   pdlg->BT_Addr_EndShow_M=Setdlg_login.BT_Addr_EndShow;

		   pdlg->TestMode_M=Setdlg_login.TestMode_S;
		   pdlg->Unlock_code_M=Setdlg_login.Unlock_code_S;

		   if((pdlg->Initial_Connect_DB_M=="false"))//不使用数据库的情况下：不能参数检查、检查合法性，就是一个普通的工具
		   {
				pdlg->Software_Version_Check_Choose_M="false";
				pdlg->IMEI_MAC_Check_Choose_M="false";
				pdlg->IMEI_CheckValid_Choose_M="false";
				pdlg->AutoIMEI_Choose_M="false";
				pdlg->AutoBTAddr_Choose_M="false";
				pdlg->AutoWIFIAddr_Choose_M="false";
				//AfxMessageBox("不使用数据库的情况下：部分功能失效！！！");
		   }
		   else
		   {
				//pdlg->OnTimerFunction(25);//校验服务器信息
		   }
		   pdlg->SaveConfig();

		   pdlg->OnBnClickedButton4();

		   extern CPara1 m_para1;
		   for(int i=0;i<MAXPARANUM;i++)
		   {
				pdlg->paraArray[i].High_Limit_Value=m_para1.parameterArray[i].High_Limit_Value;
				pdlg->paraArray[i].ifCheck=m_para1.parameterArray[i].ifCheck;
				pdlg->paraArray[i].Low_Limit_Value=m_para1.parameterArray[i].Low_Limit_Value;
				pdlg->paraArray[i].Other_ITEM=m_para1.parameterArray[i].Other_ITEM;
				pdlg->paraArray[i].paraContent=m_para1.parameterArray[i].paraContent;
				pdlg->paraArray[i].paraID=m_para1.parameterArray[i].paraID;
				pdlg->paraArray[i].showName=m_para1.parameterArray[i].showName;
		   }
		   EndDialog(0);
	}
	else 
	{	
		// 不匹配
		MessageBox("用户名或密码不正确","提示", MB_ICONWARNING);
	}
	//OnOK();
}

