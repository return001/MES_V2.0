// CSR_Function_Test.cpp : 实现文件
//

#include "stdafx.h"
#include "WriteIMEI.h"
#include "CSR_Function_Test.h"


// CCSR_Function_Test 对话框
#include "math.h"

//IMPLEMENT_DYNAMIC(CCSR_Function_Test, CDialog)

CCSR_Function_Test::CCSR_Function_Test(CWnd* pParent /*=NULL*/)
	: CDialog(CCSR_Function_Test::IDD, pParent)
	, BLE_m_Result_C(_T(""))
	, BLE_Final_Result(_T(""))
{

}

CCSR_Function_Test::~CCSR_Function_Test()
{
}

void CCSR_Function_Test::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_Port);
	DDX_Control(pDX, IDC_COMBO2, m_Baud);
	DDX_Control(pDX, IDC_RESULT, BLE_m_Result);
	DDX_Text(pDX, IDC_RESULT, BLE_m_Result_C);
	DDX_Text(pDX, IDC_STA_FINAL_RESULT, BLE_Final_Result);
	DDX_Control(pDX, IDC_STA_FINAL_RESULT, BLE_Final_Result_Control);
	DDX_Control(pDX, IDC_BUTTON2, STOP_Test_Control);
	DDX_Control(pDX, IDC_BUTTON1, START_Test_Control);
	DDX_Control(pDX, IDC_STA_FINAL_RESULT2, BLE_Operate_Guide_Control);
	DDX_Control(pDX, IDC_RESULT2, BLE_MAC_RSSI);
}


BEGIN_MESSAGE_MAP(CCSR_Function_Test, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CCSR_Function_Test::OnBnClickedButton1)
	ON_WM_CTLCOLOR()//改变颜色
	ON_WM_TIMER()//手动添加
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, &CCSR_Function_Test::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CCSR_Function_Test::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CCSR_Function_Test::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON11, &CCSR_Function_Test::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON5, &CCSR_Function_Test::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON36, &CCSR_Function_Test::OnBnClickedButton36)
	ON_BN_CLICKED(IDCANCEL, &CCSR_Function_Test::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CCSR_Function_Test::OnBnClickedOk)
END_MESSAGE_MAP()


//CBT_Addr_Setting BLE_Setdlg;//作为全局变量，两个界面线程使用的是同一个 实例  。



// CCSR_Function_Test 消息处理程序

BOOL CCSR_Function_Test::OnInitDialog()//初始化程序
{
	CDialog::OnInitDialog();
	//SetIcon(m_hIcon,TRUE);
	//SetIcon(m_hIcon,FALSE);
	//CWriteIMEIDlg*  BLE_pdlg=(CWriteIMEIDlg*)GetParent();

	//init baud
	m_Baud.AddString("2400");
	m_Baud.AddString("4800");
	m_Baud.AddString("9600");
	m_Baud.AddString("14400");
	m_Baud.AddString("19200");

	m_Baud.AddString("38400");
	m_Baud.AddString("56000");
	m_Baud.AddString("57600");
	m_Baud.AddString("115200");
	m_Baud.AddString("128000");

	m_Baud.AddString("256000");

	m_Baud.SetCurSel(0);

	//init port
	for( int i=1; i<256; i++ )
	{
		CString sPort;
		sPort.Format(_T("\\\\.\\COM%d"),i);
		BOOL bSuccess=FALSE;
		HANDLE hPort=::CreateFile(sPort, GENERIC_READ|GENERIC_WRITE, 0, 0,
			OPEN_EXISTING, 0, 0);
		if( hPort == INVALID_HANDLE_VALUE)
		{
			DWORD dwError=GetLastError();
			if( dwError == ERROR_ACCESS_DENIED)
				bSuccess=TRUE;
		}
		else
		{
			bSuccess=TRUE;
			CloseHandle(hPort);
		}
		if(  bSuccess )
		{
			CString str;
			str.Format("COM%d",i);
			m_Port.AddString(str);
		}
	}
	m_Port.SetCurSel(0);
	

	CFont* font2;
	font2 = new CFont;
	font2->CreateFont(25,8,0,0,700,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE,
		"arial"); 
	GetDlgItem(IDC_STA_FINAL_RESULT2)->SetFont(font2); 


	HBRUSH mBrush= NULL;//画刷
	CFont* font;
	font = new CFont;
	font->CreateFont(63,0,0,0,700,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE,
		"arial"); 

	GetDlgItem(IDC_STA_FINAL_RESULT)->SetFont(font);
	gColor = RGB(60,190,190);
	
	BLE_UI_LoadConfig();
	if(BLE_Setdlg.Initial_Connect_DB==TRUE)//必须初始化连接数据库，不然报错”无效指针“
	//if(BLE_pdlg->Initial_Connect_DB_M=="true")
	{
		CString Conn="";
		Conn.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s",BLE_Setdlg.m_server,BLE_Setdlg.m_db,BLE_Setdlg.m_user,BLE_Setdlg.m_pwd);
		//Conn.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s",BLE_pdlg->m_server_M,BLE_pdlg->m_db_M,BLE_pdlg->m_user_M,BLE_pdlg->m_pwd_M);
		CString DB_FAIL=BLE_Setdlg.myado.ConnecDataLibrary(Conn,"","",adModeUnknown);
		if(DB_FAIL=="FAIL")
		{
			PrintError("连接数据库失败，无法上传数据！！！请检查网络.......");
			return FALSE;
		}
		else
		{
			LogShow_exchange(250,"数据库连接成功！\r\n",&BLE_Final_Result_Control,&BLE_m_Result,0);
			Log_Show_Global();
			BLE_m_Result.UpdateWindow();
		}
	}
	else
	{
		LogShow_exchange(250,"没勾选连接数据库！！！\r\n",&BLE_Final_Result_Control,&BLE_m_Result,0);
		Log_Show_Global();
		BLE_m_Result.UpdateWindow();
	}

	START_Test_Control.EnableWindow(FALSE);
	STOP_Test_Control.EnableWindow(FALSE);
	Initial_Handle=false;
	BLE_Final_Result_Control.SetWindowTextA("IDLE");
	BLE_Final_Result_Control.UpdateWindow();//改变颜色

	BLE_Operate_Guide_Control.SetWindowTextA("请先插上蓝牙硬件（CSR and TI chip）,\r\n点击开始搜索");
	BLE_Operate_Guide_Control.UpdateWindow();//改变内容与颜色

	//RedrawWindow();
	Watch_Result=false;//初始结果为false

	//ReIniCount=0;
	Test_Handle=NULL;
	SetTimer(1,5000,NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}
HBRUSH CCSR_Function_Test::OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor)//改变颜色
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC,pWnd,nCtlColor);
	switch(pWnd->GetDlgCtrlID())
	{
	case(IDC_STA_FINAL_RESULT):
		pDC->SetTextColor(gColor);
		break;
	case(IDC_STA_FINAL_RESULT2):
		pDC->SetTextColor(RGB(0,155,100));
		break;
	}
	return hbr;

}
BOOL CCSR_Function_Test::OPen_Serial_Port()//打开串口
{
	int debug=0;        //for debug use
	CString sPort,sBaud;//,sTemp;
	int port,baud;
	//get port
	m_Port.GetWindowText(sPort);
	sPort=sPort.Right(sPort.GetLength()-3);
	port=atoi(sPort);
	sPort.Format(_T("\\\\.\\COM%d"),port);

	//get baud
	m_Baud.GetWindowText(sBaud);
	baud=atoi(sBaud);

	//open com port打开端口
	hPort=CreateFile(sPort, GENERIC_READ|GENERIC_WRITE,0, NULL, 
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);
	if(hPort == INVALID_HANDLE_VALUE)
	{
		CString csTemp;
		m_Port.GetWindowText(csTemp);
		//AfxMessageBox("Can't open "+csTemp);
		hPort=NULL;
		return FALSE;
	}

	//config the com port
	DCB dcb;
   	dcb.DCBlength = sizeof(DCB);
	GetCommState( hPort, &dcb ) ;
	SetupComm( hPort, 4096, 4096 ) ;
	PurgeComm(hPort,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
	dcb.Parity = NOPARITY;
	dcb.ByteSize = 8;
	dcb.StopBits = ONESTOPBIT;
	dcb.BaudRate = baud;     //57600(MT6205B), 115200 (MT6218B)			
	dcb.fBinary = TRUE;
	dcb.fParity = FALSE;
	dcb.fOutxCtsFlow = FALSE;
	dcb.fOutxDsrFlow = FALSE;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fDsrSensitivity = FALSE;
	dcb.fTXContinueOnXoff = FALSE;
	dcb.fOutX = FALSE;
	dcb.fInX = FALSE;
	dcb.fErrorChar = FALSE;
	dcb.fNull = FALSE;
	dcb.fRtsControl = RTS_CONTROL_ENABLE;
	dcb.fAbortOnError = FALSE;
	dcb.XonChar = 0;
	dcb.XoffChar = 0;
	dcb.ErrorChar = 0;
	dcb.EofChar = 0;
	dcb.EvtChar = 0;
	SetCommState(hPort, &dcb);

	//set time out struct
	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 2000;
	SetCommTimeouts(hPort, &timeouts);

	//SetCommMask(hPort, EV_RXCHAR|EV_TXEMPTY );//设置事件驱动的类型

	//read and write the com port
	BOOL bReadStatus,bWriteStat;
	DWORD dwBytesWritten,dwBytesRead;
	char *buffer;
	char *p=NULL;
	CString command;
	char buf[1024];
	int timeout=1;
	int i;

	//write "AT"
	for(i=0;i<10;i++)
	{

		command.Empty();
		command.Format("at+act=0\r\n");
		buffer=command.GetBuffer(command.GetLength());
		bWriteStat = WriteFile( hPort, buffer, command.GetLength(), &dwBytesWritten,NULL );
		if( dwBytesWritten != command.GetLength() )
		{
			//PrintError("串口没连接好，请检查串口！");
			CloseHandle(hPort);           //关闭串口
			
			return FALSE;
		}

		//receive response
		Sleep(200);
		memset(buf,0,sizeof(buf));
		buffer=buf;
		bReadStatus = ReadFile( hPort, buffer, 10, &dwBytesRead, NULL);
		if(dwBytesRead != 0)
		{
			p=strstr(buffer,"\r");
			if(p)
			{
				//BLE_m_Result.SetWindowText("==============================手机已经开机！串口通信正常......\r\n"+LOG_Show_Old);
				timeout=0;
				break;
			}
		}
		Sleep(100);
	}
	//check if failed
	if(timeout == 1)
	{
		//PrintError("串口没连接好，请检查串口！");
		CloseHandle(hPort);//
		return FALSE;
	}
	//make sure rx data cleaned
	Sleep(100);
	//bReadStatus = ReadFile( hPort, buffer, 10, &dwBytesRead, NULL);
	bReadStatus = ReadFile( hPort, buffer, 256, &dwBytesRead, NULL);

	return TRUE;//返回TRUE，打开串口正常
}

char*  CCSR_Function_Test::Send_Serial_Order(char** Vaule_Return,CString strCommand_Vaule)//通过串口发送命令
{
	int Vaule_Return_Count=-1;//参数的个数
	BOOL bReadStatus,bWriteStat;
	DWORD dwBytesWritten,dwBytesRead;
	char *buffer;
	char *p=NULL;
	CString command;
	char buf[1024];

	if(strCommand_Vaule=="IDLE")//处理延时
	{
		//BLE_Operate_Guide_Control.SetWindowTextA("计时等待...");
		//BLE_Operate_Guide_Control.UpdateWindow();//改变内容与颜色
		Delay_Clock(800);//paraArray[i].Low_Limit_Value
		*Vaule_Return="DELAY_SUCCESS";
		return "DELAY_SUCCESS";
		
	}
	Sleep(50);
	for(int i=0;i<2;i++)//多次发送
	{
		//send command
		command.Empty();
		//command.Format("AT+CGMR\r\n");
		CString LOG_Time;
		CTime t = CTime::GetCurrentTime();
		LOG_Time.Format("%04d%02d%02d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute());
		if("at+dt="==strCommand_Vaule)
			strCommand_Vaule+=LOG_Time;
		command=strCommand_Vaule+"\r\n";//拼接命令


		buffer=command.GetBuffer(command.GetLength());
		bWriteStat = WriteFile( hPort, buffer, command.GetLength(), &dwBytesWritten,NULL );
		if( dwBytesWritten != command.GetLength() )
		{
			Sleep(300);
			continue;
		}
		if(strCommand_Vaule=="at+off")//处理延时
		{
			Delay_Clock(800);//paraArray[i].Low_Limit_Value
			*Vaule_Return="DELAY_SUCCESS";
			return "DELAY_SUCCESS";
			
		}
		//receive response
		Sleep(atol(paraArray[1].Low_Limit_Value));
		CString Port_Temp="";//串口读取数据 的缓存
		for(int r=0;r<3;r++)
		{	
			memset(buf,0,sizeof(buf));
			buffer=buf;
			bReadStatus = ReadFile( hPort, buffer, 256, &dwBytesRead, NULL);
			if(dwBytesRead != 0)
			{
				p=strstr(buffer,"\r");
				if(p)
				{
					 CString strCommand_Vaule_Return;//串口返回值
					 strCommand_Vaule_Return=Port_Temp+CString(buffer);
					 int   nPosStr = strCommand_Vaule_Return.Find(":");
					 CString  selPort = strCommand_Vaule_Return.Mid(nPosStr+1);
					 nPosStr = selPort.Find("OK");
					 if(nPosStr>=0)
						selPort= selPort.Left(nPosStr);	
					 selPort.Replace("\r","");
					 selPort.Replace("\n","");
					 selPort.Replace(",","_");
					 //获取参数的个数
					 CString string_sn_r;
					 int npos=selPort.Find('"');
					 if(npos>=0)
					 {
						string_sn_r=selPort.Mid(npos+1);
						Vaule_Return_Count_CS=string_sn_r.Left(string_sn_r.Find('"'));
					 }
					 else
					 {
						Vaule_Return_Count_CS=selPort;
					 }
					 //char* Vaule_Return;
					 //*Vaule_Return=(LPTSTR)(LPCTSTR)Vaule_Return_Count_CS;
					 *Vaule_Return="Analysis_SUCCESS";
					 return "Analysis_SUCCESS";
					 //break;
				}
				else
				{
					//CloseHandle(hPort);//串口发送失败则关闭串口
					//return "FAIL";
					Port_Temp+=CString(buffer);
					Sleep(200);
					continue;
				}
			}
			else
			{
				Sleep(200);
				continue;
			}
		}

	}
	//CloseHandle(hPort);//成功后不关闭串口
	*Vaule_Return="FAIL";
	return "FAIL";
}
BOOL CCSR_Function_Test::CSR_Function_Test_Thread()
{
	static char* Serial_Order_Return;//多线程的时候异常
	CWriteIMEIDlg*  Pdlg=(CWriteIMEIDlg*)GetParent();
	if(OPen_Serial_Port()==TRUE)
	{
		//打开串口成功
		LogShow_exchange(0,"打开串口成功！\r\n",&BLE_Final_Result_Control,&BLE_m_Result,0);//第一次显示“测试中”
		Log_Show_Global();
		BLE_m_Result.UpdateWindow();
	}
	else
	{
		//打开串口失败
		PrintError("打开串口失败！！！");
		return FALSE;
	}
	int Try_Time=0;//尝试次数（结果判断）
	int All_Result[30]={1,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0 };
	for(int i=1;i<30;i++)
	{
		int Try_Time_Real=0;
		do{
			if(paraArray[i].ifCheck!=true)//没有勾选
			{
				All_Result[i]=1;
				break; //跳出do while
			}
			if((paraArray[i].showName=="写功能测试标志")&&(All_Result[0]!=1))
				break;//跳出do while//测试项不全部OK，不写功能测试标志

			Send_Serial_Order(&Serial_Order_Return,paraArray[i].Other_ITEM);
			CString Serial_Order_Return_CS_Show;
			if(Serial_Order_Return=="Analysis_SUCCESS")//数据接收正常
				Serial_Order_Return_CS_Show=Vaule_Return_Count_CS;
			else if(((Serial_Order_Return=="FAIL")&&(paraArray[i].showName=="心率")))//\
				||((paraArray[i].showName=="SOS按键_INI_State")||(paraArray[i].showName=="SOS按键")))//心率指令发送后需要等待才会返回值,或是测试SOS按键返回值为AT时
			{
				if(paraArray[i].showName=="心率")
				{
					Vaule_Return_Count_CS="0";
					Serial_Order_Return_CS_Show="NULL";
				}
				Serial_Order_Return="Analysis_SUCCESS";
				
			}

			else
				Serial_Order_Return_CS_Show.Format("%s",Serial_Order_Return);
			LogShow_exchange(5,"串口返回值:"+Serial_Order_Return_CS_Show,&BLE_Final_Result_Control,&BLE_m_Result,0);//继续测试，状态不更新
			Log_Show_Global();
			BLE_m_Result.UpdateWindow();

			bool Judge_Return;//判断结果返回值，false为不正常
			if(Serial_Order_Return!="FAIL")
			{
				//成功后数据处理
				if(Serial_Order_Return=="DELAY_SUCCESS")//延时
				{
					All_Result[i]=1;
					break; //跳出do while 
				}
				if(Serial_Order_Return=="Analysis_SUCCESS")//数据接收正常
				{
					Serial_Order_Return=(LPTSTR)(LPCTSTR)Vaule_Return_Count_CS;
					Judge_Return=BLE_Function_Judge(i,paraArray[i].showName,Serial_Order_Return);
				}
				//BLE_Operate_Guide_Control.SetWindowTextA("......");
				//BLE_Operate_Guide_Control.UpdateWindow();//改变内容与颜色
				if(Judge_Return==true)
				{
					//结果达标
					LogShow_exchange(5,"测试项目:"+paraArray[i].showName+"达标！========该项测试成功",&BLE_Final_Result_Control,&BLE_m_Result,0);//继续测试，状态不更新
					Log_Show_Global();
					BLE_m_Result.UpdateWindow();
					All_Result[i]=1;
					break; //跳出do while
				}
				else
				{
					//失败返回
					Try_Time++;
					if((paraArray[0].ifCheck==true)&&(Try_Time>=atoi(paraArray[0].High_Limit_Value)))//失败立即返回
					{
						
						Send_Serial_Order(&Serial_Order_Return,"at+mfg=7");
						Send_Serial_Order(&Serial_Order_Return,"at+mfg=;");
						Send_Serial_Order(&Serial_Order_Return,"at+mfg==");
						Send_Serial_Order(&Serial_Order_Return,"at+mfg=0");
						PrintError("测试项目:"+paraArray[i].showName+"失败,立即停止！！！");
						OnBnClickedButton3();
						return FALSE; //跳出整个循环for
					}
					else
					{
						LogShow_exchange(5,"测试项目:"+paraArray[i].showName+"失败！重试或者继续下一项目...",&BLE_Final_Result_Control,&BLE_m_Result,0);//继续测试，状态不更新
						Log_Show_Global();
						BLE_m_Result.UpdateWindow();
						continue;//循环do while
					}
				}
			}
			else
			{
				//失败返回
				Try_Time++;
				if((paraArray[0].ifCheck==true)&&(Try_Time>=atoi(paraArray[0].High_Limit_Value)))//失败立即返回
				{
					
					Send_Serial_Order(&Serial_Order_Return,"at+mfg=7");
					Send_Serial_Order(&Serial_Order_Return,"at+mfg=;");
					Send_Serial_Order(&Serial_Order_Return,"at+mfg==");
					Send_Serial_Order(&Serial_Order_Return,"at+mfg=0");
					PrintError("测试项目:"+paraArray[i].showName+"失败,立即停止！！！");
					OnBnClickedButton3();
					return FALSE; //跳出整个循环for
				}
				else
				{
					LogShow_exchange(5,"测试项目:"+paraArray[i].showName+"失败！重试或者继续下一项目...",&BLE_Final_Result_Control,&BLE_m_Result,0);//继续测试，状态不更新
					Log_Show_Global();
					BLE_m_Result.UpdateWindow();
					continue;//循环do while
				}
			}
			
		}while(Try_Time<atoi(paraArray[0].High_Limit_Value));
		Try_Time=0;//将单项尝试次数清零
		BLE_Operate_Guide_Control.SetWindowTextA("......");
		BLE_Operate_Guide_Control.UpdateWindow();//改变内容与颜色

		All_Result[0]&=All_Result[i];
	}
	//delete SENSOR_Gravity;
	CString DB_FAIL="";
	if(All_Result[0]==1)
	{
		if(BLE_Setdlg.Initial_Connect_DB==TRUE)
		{
			CString Conn="";
			
			Conn.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s",BLE_Setdlg.m_server,BLE_Setdlg.m_db,BLE_Setdlg.m_user,BLE_Setdlg.m_pwd);//BLE
			if(BLE_Setdlg.myado.m_pCon==NULL)
			{
				DB_FAIL=BLE_Setdlg.myado.ConnecDataLibrary(Conn,"","",adModeUnknown);
			}
			if(BLE_Setdlg.myado.m_pCon->State==0)  //1表示已经打开,0表示关闭，数据库断开，重连
			{
				LogShow_exchange(0,"=================================数据库断开状态，进行连接........\r\n",&BLE_Final_Result_Control,&BLE_m_Result,0);
				Log_Show_Global();
				BLE_m_Result.UpdateWindow();
				DB_FAIL=BLE_Setdlg.myado.ConnecDataLibrary(Conn,"","",adModeUnknown);//数据库意外断开，重连
			}
			else
			{
				LogShow_exchange(0,"=================================数据库连接正常中........\r\n",&BLE_Final_Result_Control,&BLE_m_Result,0);
				Log_Show_Global();
				BLE_m_Result.UpdateWindow();
			}
			if(DB_FAIL=="FAIL")
			{
				PrintError("连接数据库失败，无法上传数据！！！请检查网络.......");
				return FALSE;
			}

			//检查数据库是否存在该BT-MAC地址,除去相同板号那一项
			if(BLE_MAC_Addr!="")
			{
				BLE_Setdlg.myado.OpenSheet("select BT_Addr from odm_check_test_BoardNum WHERE (BT_Addr ='"+BLE_MAC_Addr+"') AND (BoardNumber <> '"+SN_BLE+"')");
				BOOL BTMAC_Check_UP=BLE_Setdlg.myado.Find("BT_Addr='"+BLE_MAC_Addr+"'");
				BLE_Setdlg.myado.CloseSheet();
				if(BTMAC_Check_UP==TRUE)
				{
					PrintError("蓝牙地址："+BLE_MAC_Addr+"数据库重复！！！");
					return FALSE;
				}
				else
				{
					LogShow_exchange(0,"蓝牙地址："+BLE_MAC_Addr+"数据库正常...",&BLE_Final_Result_Control,&BLE_m_Result,0);
					Log_Show_Global();
					BLE_m_Result.UpdateWindow();
				}
			}
			else
			{
				PrintError("蓝牙地址获取失败！！！");
				return FALSE;
			}
			//////////////////////////////////////////////////
			//检查数据库是否存在该SN
			if(SN_BLE!="000000000000")
			{
				BLE_Setdlg.myado.OpenSheet("select BoardNumber from odm_check_test_BoardNum WHERE BoardNumber ='"+SN_BLE+"'");
				BOOL SN_Check_UP=BLE_Setdlg.myado.Find("BoardNumber='"+SN_BLE+"'");
				BLE_Setdlg.myado.CloseSheet();
			
				if(Cal_Sign!="1111")
				{
					PrintError("蓝牙没有校准！！！只写了SN号。");
					return FALSE;
				}
				else
				{
					LogShow_exchange(0,"蓝牙射频已校准...\r\n",&BLE_Final_Result_Control,&BLE_m_Result,0);
					Log_Show_Global();
					BLE_m_Result.UpdateWindow();
				}

				CString Coupling_Sign_CS;
				Coupling_Sign_CS.Format("%d",Coupling_Sign);
				if(SN_Check_UP==TRUE)
				{
					if(paraArray[32].ifCheck==true)
					{
						LogShow_exchange(0,"数据库存在SN号："+SN_BLE+"更新该SN号对应的数据库信息...",&BLE_Final_Result_Control,&BLE_m_Result,0);
						Log_Show_Global();
						BLE_m_Result.UpdateWindow();
						
						CString strSQL = "UPDATE odm_check_test_BoardNum SET BT_Addr = '"+BLE_MAC_Addr+"',CalibrMark = '"+Cal_Sign+"',CompreMark = '"+Coupling_Sign_CS+"'\
							 ,ordernum = '"+Function_Test_Order_Number+"',bversion = '"+Software_Version+"'\
							 ,Power_INI = '"+Power_INI+"',Power_Diff = '"+Power_Diff+"'\
							 ,Temperature = '"+Temperature+"',HeartRate = '"+HeartRate+"'\
							 ,Gravity = '"+SENSOR_Gravity_ALL+"' \
							 WHERE (BoardNumber = '"+SN_BLE+"') ";
						BLE_Setdlg.myado.OpenSheet("select * from odm_check_test_BoardNum");
						BOOL UP_FAIL=BLE_Setdlg.myado.Execute(strSQL);
						BLE_Setdlg.myado.CloseSheet();
						Sleep(500);
						BLE_Setdlg.myado.OpenSheet("select BoardNumber from odm_check_test_BoardNum WHERE BoardNumber ='"+SN_BLE+"'");
						BOOL ALL_Check_UP=BLE_Setdlg.myado.Find("BoardNumber='"+SN_BLE+"'");
						BLE_Setdlg.myado.CloseSheet();
						
						if((UP_FAIL==TRUE&&ALL_Check_UP==TRUE))
						{
							//m_Result.SetWindowText("成功更新数据库！！！\r\n\r\n");
							PrintOK("成功更新数据库！！！");
						}
						else
						{
							PrintError("更新数据库失败！！！！！！");
							return FALSE;
						}
					}
					else
					{
						PrintError("数据库SN重复！！！！！！");
						return FALSE;
					}
				}
				else
				{
					LogShow_exchange(0,"数据库不存在SN号："+SN_BLE+"直接上传SN号对应的数据库信息...",&BLE_Final_Result_Control,&BLE_m_Result,0);
					Log_Show_Global();
					BLE_m_Result.UpdateWindow();

					CString strSQL = "Insert into odm_check_test_BoardNum(BoardNumber,BT_Addr,CalibrMark,CompreMark,ordernum,bversion,Power_INI,Power_Diff,Temperature,HeartRate,Gravity)\
						 values('"+SN_BLE+"','"+BLE_MAC_Addr+"','"+Cal_Sign+"','"+Coupling_Sign_CS+"','"+Function_Test_Order_Number+"','"+Software_Version+"','"+Power_INI+"','"+Power_Diff+"','"+Temperature+"','"+HeartRate+"','"+SENSOR_Gravity_ALL+"')";		
					BLE_Setdlg.myado.OpenSheet("select * from odm_check_test_BoardNum");
					BOOL UP_FAIL=BLE_Setdlg.myado.Execute(strSQL);
					BLE_Setdlg.myado.CloseSheet();
					Sleep(500);
					BLE_Setdlg.myado.OpenSheet("select BoardNumber from odm_check_test_BoardNum WHERE BoardNumber ='"+SN_BLE+"'");
					BOOL ALL_Check_UP=BLE_Setdlg.myado.Find("BoardNumber='"+SN_BLE+"'");
					BLE_Setdlg.myado.CloseSheet();
					
					if((UP_FAIL==TRUE&&ALL_Check_UP==TRUE))
					{
						PrintOK("成功上传数据库！！！");
					}
					else
					{
						PrintError("上传数据库失败！！！！！！");
						return FALSE;
					}
				}
			}
			else
			{
				PrintError("CSR芯片没有SN号，可能是因为升级丢失参数！！！");
				return FALSE;
			}

			//////////////////////////////////////////////////
			//////////////////////////////////////////////////
		}
		else
			PrintOK("腕带功能性测试OK！！");
	}
	else
	{
		PrintError("腕带功能性测试FAIL！！！");
	}
	return TRUE;

}

void CCSR_Function_Test::LogShow_exchange(int State,CString Msg_Log,CEdit* Final_Result_Control,CEdit* m_Result,int WndChoose)     //传递  线程变量 的函数
{
	BLE_State=State;
	m_Result->GetWindowText(LOG_Show_Old);
	
	CTime cTime;
	cTime = CTime::GetCurrentTime();                                                         //创建以当日日期的文件夹来保存数据
	CString LogTime;
	LogTime.Format("    (%04d-%02d-%02d--%02d:%02d:%02d)",cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(),cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());

	if(WndChoose==0)
	{
		BLE_Msg_Log=LOG_Show_Old+"\r\n"+Msg_Log+LogTime;
		m_Result->SetWindowText(BLE_Msg_Log.Right(8092));
		BLE_m_Result.LineScroll(BLE_m_Result.GetLineCount());
	}
	else if(WndChoose==1)
	{
		if(Msg_Log.Find("搜索蓝牙设备成功")!=-1)
			BLE_Msg_Log=LOG_Show_Old+"\r\n"+Msg_Log+"\r\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\r\n\r\n";
		else
			BLE_Msg_Log=LOG_Show_Old+"\r\n"+Msg_Log+"\r\n"+LogTime;
		m_Result->SetWindowText(BLE_Msg_Log.Right(1024));
		BLE_MAC_RSSI.LineScroll(BLE_MAC_RSSI.GetLineCount());
	}
	//m_Result->UpdateWindow();	
	if(State==0)
	{
		gColor = RGB(0,155,100);
		Final_Result_Control->SetWindowTextA("测试中");
		Final_Result_Control->UpdateWindow();
	}
	else if(State==128)
	{
		gColor = RGB(255,0,0);
		Final_Result_Control->SetWindowTextA("功能FAIL");
		Final_Result_Control->UpdateWindow(); 
	}
	else if(State==255)
	{
		gColor = RGB(0,255,0);
		Final_Result_Control->SetWindowTextA("功能_OK");
		Final_Result_Control->UpdateWindow();
	}
	else if(State==250)
	{
		gColor = RGB(60,190,190);
		Final_Result_Control->SetWindowTextA("IDLE");
		Final_Result_Control->UpdateWindow();
	}
	else if(State==251)//校准成功后继续综测
	{
		gColor = RGB(0,155,100);
		Final_Result_Control->SetWindowTextA("测试中");
		Final_Result_Control->UpdateWindow();
	}
	else if(State==5)//继续测试，状态不更新
	{
		//gColor = RGB(0,155,100);
		//BLE_Final_Result_Control.SetWindowTextA("测试中");
		//BLE_Final_Result_Control.UpdateWindow();
		//RedrawWindow();
	}
	else
	{
		gColor = RGB(0,155,100);
		Final_Result_Control->SetWindowTextA("测试中");
		Final_Result_Control->UpdateWindow();
	}
	
}

void CCSR_Function_Test::Log_Show_Global(void)                          //创建BLE  log信息线程
{
	//LogShow_Thread_Handle=AfxBeginThread(LogShow_Thread_Static_Global,(LPVOID)this,THREAD_PRIORITY_NORMAL,0,0,NULL);//错误1
}

UINT static __cdecl Measure_Function_BLE(LPVOID pParam)
{
	CCSR_Function_Test* Mead_Main_Win = (CCSR_Function_Test*)pParam;
	Mead_Main_Win->CSR_Function_Test_Thread();
	return 0;
}

void CCSR_Function_Test::OnBnClickedButton1()//开始测试
{
	// TODO: 在此添加控件通知处理程序代码
	Initial_Handle=true;
	Watch_Result=false;//每次开始置为false
	//SENSOR_Gravity = new CStringArray[2];
	//SENSOR_Gravity->RemoveAll();
	SENSOR_Gravity[0][0]="";
	SENSOR_Gravity[1][0]="";
	SENSOR_Gravity[2][0]="";
	SENSOR_Gravity[0][1]="";
	SENSOR_Gravity[1][1]="";
	SENSOR_Gravity[2][1]="";
	SENSOR_Gravity_ALL="";
	Function_Test_Order_Number="";
	Software_Version="";
	BLE_MAC_Addr="";

	SN_BLE="";
	Cal_Sign="";
	Coupling_Sign=0;

	Power_INI=""; //初始电量
	Power_Diff="";//充电电量
	//SENSOR_Gravity[3][2];
	Temperature="";
	HeartRate="";
	HeartRate_RL[0][0]="";
	HeartRate_RL[1][0]="";
	HeartRate_RL[0][1]="";
	HeartRate_RL[1][1]="";

	HeartRate_IL[0][0]="";
	HeartRate_IL[1][0]="";
	HeartRate_IL[0][1]="";
	HeartRate_IL[1][1]="";

	BLE_m_Result.SetWindowText("");
	BLE_m_Result.UpdateWindow();

	Measure_Thread_Handle=AfxBeginThread(Measure_Function_BLE,(LPVOID)this,THREAD_PRIORITY_BELOW_NORMAL,0,0,NULL);
	gColor = RGB(0,155,100);
	BLE_Final_Result_Control.SetWindowTextA("测试中");
	BLE_Final_Result_Control.UpdateWindow();
	
	BLE_Operate_Guide_Control.SetWindowTextA("正常运行状态...");
	BLE_Operate_Guide_Control.UpdateWindow();//改变内容与颜色

	STOP_Test_Control.EnableWindow(TRUE);
	START_Test_Control.EnableWindow(FALSE);
	
	//RedrawWindow();
}

void CCSR_Function_Test::OnBnClickedButton2()//停止测试
{
	// TODO: 在此添加控件通知处理程序代码
	if(Initial_Handle==true)
	{
		//BLE
		if(LogShow_Thread_Handle)			LogShow_Thread_Handle->SuspendThread();     //挂起LOG线程
		if(Measure_Thread_Handle)			Measure_Thread_Handle->SuspendThread();     //挂起测试线程 （后）

		if(LogShow_Thread_Handle)			TerminateThread(LogShow_Thread_Handle,2);
		if(Measure_Thread_Handle)			TerminateThread(Measure_Thread_Handle,3);
		if(LogShow_Thread_Handle)			TerminateThread(LogShow_Thread_Handle,4);
		if(Measure_Thread_Handle)			TerminateThread(Measure_Thread_Handle,5);
	}
	char* Serial_Order_Return;
	
	Send_Serial_Order(&Serial_Order_Return,"at+mfg=7");
	Send_Serial_Order(&Serial_Order_Return,"at+mfg=;");
	Send_Serial_Order(&Serial_Order_Return,"at+mfg==");
	Send_Serial_Order(&Serial_Order_Return,"at+mfg=0");
	PrintError("结束测试！！！");
	BLE_Operate_Guide_Control.SetWindowTextA("Idle...");
	BLE_Operate_Guide_Control.UpdateWindow();//改变内容与颜色

	STOP_Test_Control.EnableWindow(FALSE);
	START_Test_Control.EnableWindow(TRUE);
}

void CCSR_Function_Test::BLE_UI_LoadConfig()
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

		 //CopyFile(szPath+"\\config\\limit.vbs","C:\\limit.vbs",FALSE);

		 CString strLoadFile=szPath+STRING_PATH_BLE;
		 CIniFile tmpread(strLoadFile);
		 CString fSectionName=tmpread.GetParaName();
 		 if(fSectionName=="BLE")
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
				return;
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
			}
		 }


         strLoadFile = szPath+STRING_PATH_CONFIG;
         CIniFile m_fIniFile(strLoadFile);

         m_fIniFile.ReadSection(STRING_SECTION_CONFIG, strCommandAry);
         nCommandNum = strCommandAry.GetSize();
		 CString INI_MSG_TEMP;
		 for(int i=0;i<nCommandNum;i++)
		 {
             strCommand = strCommandAry[i];//编号
             if(strCommand == "Initial_Connect_DB_M")
             {
				 INI_MSG_TEMP= m_fIniFile.ReadString(STRING_SECTION_CONFIG, strCommand, "");
				 if(INI_MSG_TEMP=="true")
					 BLE_Setdlg.Initial_Connect_DB=TRUE;
				 else
					 BLE_Setdlg.Initial_Connect_DB=FALSE;
             }
			 else if(strCommand == "m_server_M")
			 {
					   BLE_Setdlg.m_server = m_fIniFile.ReadString(STRING_SECTION_CONFIG, strCommand, "");
			 }
			 else if(strCommand == "m_db_M")
			 {
					   BLE_Setdlg.m_db = m_fIniFile.ReadString(STRING_SECTION_CONFIG, strCommand, "");
			 }
			 else if(strCommand == "m_user_M")
			 {
					   BLE_Setdlg.m_user = m_fIniFile.ReadString(STRING_SECTION_CONFIG, strCommand, "");
			 }
			 else if(strCommand == "m_pwd_M")
			 {
			 		   BLE_Setdlg.m_pwd = m_fIniFile.ReadString(STRING_SECTION_CONFIG, strCommand, "");
			 }

		 }
		 UpdateData(false);
	}


void CCSR_Function_Test::PrintError(CString cs)
{
	OnBnClickedButton3();
	LogShow_exchange(128,cs+"\r\n",&BLE_Final_Result_Control,&BLE_m_Result,0);
	Log_Show_Global();
	BLE_m_Result.UpdateWindow();
	STOP_Test_Control.EnableWindow(FALSE);
	START_Test_Control.EnableWindow(TRUE);
	BLE_Operate_Guide_Control.SetWindowTextA("Idle...");
	BLE_Operate_Guide_Control.UpdateWindow();//改变内容与颜色
}

void CCSR_Function_Test::PrintOK(CString cs)
{
	OnBnClickedButton3();
	LogShow_exchange(255,cs+"\r\n",&BLE_Final_Result_Control,&BLE_m_Result,0);
	Log_Show_Global();
	BLE_m_Result.UpdateWindow();
	STOP_Test_Control.EnableWindow(FALSE);
	START_Test_Control.EnableWindow(TRUE);
	BLE_Operate_Guide_Control.SetWindowTextA("Idle...");
	BLE_Operate_Guide_Control.UpdateWindow();//改变内容与颜色
}


UINT static __cdecl TIMER_DELAY(LPVOID pParam)     //延时线程
{
	CCSR_Function_Test* TIMER_DELAY_Function=(CCSR_Function_Test*)pParam;

	TIMER_DELAY_Function->TimerID=TIMER_DELAY_Function->SetTimer(25,TIMER_DELAY_Function->Sleep_Idle_Time,NULL);
	return 0;
}
void CCSR_Function_Test::Delay_Clock(UINT mSecond)//定时器延时
{
	if(TimerID!=NULL)	
		KillTimer(TimerID);
	
	RunCommandList.ResetEvent();//等待
	//AfxBeginThread(TIMER_DELAY,(LPVOID)this,THREAD_PRIORITY_HIGHEST,0,0,NULL);
	WaitForSingleObject(RunCommandList.m_hObject,mSecond+500);
	//WaitForSingleObject(RunCommandList.m_hObject,INFINITE);
	Sleep(0);
	return;
}

void CCSR_Function_Test::OnTimer(UINT nIDEvent)//后续手动增加，定时器
{
	switch(nIDEvent)
	{
	case 25:
		KillTimer(TimerID);
		AfxMessageBox("延时OK");
		RunCommandList.SetEvent();//无用
		break;
	case 1:
			if(CSRBTHostApp.appCurState == APP_ERROR)
			{
				m_bStopFlag = TRUE;
				SetEvent(CSRBTHostApp.syncEvent);
				SetEvent(CSRBTHostApp.SearchDevEvent);
				SetEvent(CSRBTHostApp.ConnectDevEvent);
				SetEvent(CSRBTHostApp.GetInfEvent);
				SetEvent(CSRBTHostApp.WriteDataEvent);
				SetEvent(CSRBTHostApp.DisConnEvent);
			}
			break;
	}
	CDialog::OnTimer(nIDEvent);
}

int CCSR_Function_Test::CharToHexChar(char ch)
{
   if((ch>='0')&&(ch<='9'))
   {
       return ch-0x30;

   }
   else if((ch>='A')&&(ch<='F'))
   {
       return ch-'A'+10;
   }
   else if((ch>='a')&&(ch<='f'))
   {
       return ch-'a'+10;  
   }      
   else
   {
       return -1;
   }
}

bool CCSR_Function_Test::BLE_Function_Judge(int i,CString BLE_FT_Item,char* Serial_Order_Return)//判断数据是否正常paraArray[i].showName
{
	int BLE_FT_Item_Int=0;
	CString Serial_Order_Return_CS;
	Serial_Order_Return_CS.Format("%s",Serial_Order_Return);

	int  WIFI_addr1[6];
	long WIFI_addr2=0;
	long WIFI_addr3=0;
	long WIFI_addr4=0;
	CString m_WIFI_Addr_Temp1,m_WIFI_Addr_Temp2;
	char m_WIFI_Addr_Temp3[5];

	

	if(BLE_FT_Item=="读功能测试标志")
	{
		BLE_FT_Item_Int=1;
		Function_Test_Order_Number=paraArray[i].High_Limit_Value;
	}
	else if(BLE_FT_Item=="软件版本")
	{
		BLE_FT_Item_Int=2;
		//Software_Version=paraArray[i].Low_Limit_Value;
		Software_Version=Serial_Order_Return_CS;
	}
	else if(BLE_FT_Item=="BT_MAC")
	{
		BLE_FT_Item_Int=3;
		BLE_MAC_Addr=Serial_Order_Return_CS.Mid(4);
		Serial_Order_Return_CS=Serial_Order_Return_CS.Mid(4);
		for(int a=1;a<7;a++)                                   //BT MAC地址的结束值
		{
			m_WIFI_Addr_Temp1=paraArray[i].High_Limit_Value.Right(a);
			m_WIFI_Addr_Temp2=m_WIFI_Addr_Temp1.Left(1);
			//strncpy(m_WIFI_Addr_Temp3,(LPCTSTR)m_WIFI_Addr_Temp2,sizeof(m_WIFI_Addr_Temp2));		
			strcpy(m_WIFI_Addr_Temp3,(LPCTSTR)m_WIFI_Addr_Temp2);	
			WIFI_addr1[a-1]=CharToHexChar(m_WIFI_Addr_Temp3[0]);
			//WIFI_addr1[a-1]=atoi(m_WIFI_Addr_Temp3);
			WIFI_addr4+=WIFI_addr1[a-1]*(pow((double)16,(a-1)));//BT结束地址后6位
		}

		for(int a=1;a<7;a++)                                  //BT MAC地址的起始值
		{
			m_WIFI_Addr_Temp1=paraArray[i].Low_Limit_Value.Right(a);
			m_WIFI_Addr_Temp2=m_WIFI_Addr_Temp1.Left(1);
			//strncpy(m_WIFI_Addr_Temp3,(LPCTSTR)m_WIFI_Addr_Temp2,sizeof(m_WIFI_Addr_Temp2));		
			strcpy(m_WIFI_Addr_Temp3,(LPCTSTR)m_WIFI_Addr_Temp2);	
			WIFI_addr1[a-1]=CharToHexChar(m_WIFI_Addr_Temp3[0]);
			//WIFI_addr1[a-1]=atoi(m_WIFI_Addr_Temp3);
			WIFI_addr3+=WIFI_addr1[a-1]*(pow((double)16,(a-1)));
		}

		for(int a=1;a<7;a++)
		{
			m_WIFI_Addr_Temp1=Serial_Order_Return_CS.Right(a);
			m_WIFI_Addr_Temp2=m_WIFI_Addr_Temp1.Left(1);
			//strncpy(m_WIFI_Addr_Temp3,(LPCTSTR)m_WIFI_Addr_Temp2,sizeof(m_WIFI_Addr_Temp2));		
			strcpy(m_WIFI_Addr_Temp3,(LPCTSTR)m_WIFI_Addr_Temp2);	
			WIFI_addr1[a-1]=CharToHexChar(m_WIFI_Addr_Temp3[0]);
			//WIFI_addr1[a-1]=atoi(m_WIFI_Addr_Temp3);
			WIFI_addr2+=WIFI_addr1[a-1]*(pow((double)16,(a-1)));
		}
	}
	else if(BLE_FT_Item=="SN_标志位")
	{
		BLE_FT_Item_Int=4;
		CString Serial_Order_Return_Temp;

		LimitOfTool=Serial_Order_Return_CS.Right(11);

		SN_BLE=Serial_Order_Return_CS.Left(12);
		Serial_Order_Return_Temp=Serial_Order_Return_CS.Mid(12);
		Cal_Sign=Serial_Order_Return_Temp.Left(4);

		Serial_Order_Return_Temp=Serial_Order_Return_Temp.Mid(4);
		Coupling_Sign=atoi(Serial_Order_Return_Temp.Left(4));
	}
	else if(BLE_FT_Item=="电池电量1")
	{
		BLE_FT_Item_Int=5;
	}
	else if(BLE_FT_Item=="重力传感器开")
		BLE_FT_Item_Int=21;
	else if(BLE_FT_Item=="重力")
	{
		BLE_FT_Item_Int=6;
		SENSOR_Gravity_ALL=Serial_Order_Return_CS;
		int npos=Serial_Order_Return_CS.Find("gsensor_");
		Serial_Order_Return_CS=Serial_Order_Return_CS.Mid(npos+8);
		for(int j=0;j<3;j++)
		{
			SENSOR_Gravity[j][1]=SENSOR_Gravity[j][0];
		}

		CString Serial_Order_Return_Temp;
		npos=Serial_Order_Return_CS.Find("_");
		if(npos>=0)
		{
			SENSOR_Gravity[0][0]=Serial_Order_Return_CS.Left(npos);
			Serial_Order_Return_Temp=Serial_Order_Return_CS.Mid(npos+1);

			npos=Serial_Order_Return_Temp.Find("_");
			if(npos>=0)
			{
				SENSOR_Gravity[1][0]=Serial_Order_Return_Temp.Left(npos);
				Serial_Order_Return_Temp=Serial_Order_Return_Temp.Mid(npos+1,3);
				SENSOR_Gravity[2][0]=Serial_Order_Return_Temp;

				LogShow_exchange(5,"重力值:"+SENSOR_Gravity[0][1]+"，"+SENSOR_Gravity[1][1]+"，"+SENSOR_Gravity[2][1]+"；"+SENSOR_Gravity[0][0]+"，"+SENSOR_Gravity[1][0]+"，"+SENSOR_Gravity[2][0],&BLE_Final_Result_Control,&BLE_m_Result,0);//继续测试，状态不更新
				Log_Show_Global();
				BLE_m_Result.UpdateWindow();
			}
			else
			{
				//SENSOR_Gravity[1][0]="";
				//SENSOR_Gravity[2][0]="";
			}
		}
		else
		{
			//SENSOR_Gravity[0][0]="";//值保持不变
			//SENSOR_Gravity[1][0]="";
			//SENSOR_Gravity[2][0]="";
		}

		BLE_Operate_Guide_Control.SetWindowTextA("测试重力，摇晃腕带..并准备心率...");
		BLE_Operate_Guide_Control.UpdateWindow();				//改变内容与颜色
		
		Delay_Clock(atol(paraArray[i-1].High_Limit_Value));		//等待重力测试 的时间
	}
	else if(BLE_FT_Item=="重力中断开")
	{
		BLE_FT_Item_Int=19;
		BLE_Operate_Guide_Control.SetWindowTextA("测试重力中断，摇晃腕带（断开外电）");
		BLE_Operate_Guide_Control.UpdateWindow();//改变内容与颜色
		//Delay_Clock(atol(paraArray[i].High_Limit_Value));//等待人眼判断 的时间
	}
	else if(BLE_FT_Item=="重力中断关")
	{
		BLE_FT_Item_Int=20;
		BLE_Operate_Guide_Control.SetWindowTextA("关闭重力中断，准备心率...");
		BLE_Operate_Guide_Control.UpdateWindow();//改变内容与颜色
		//Delay_Clock(atol(paraArray[i].High_Limit_Value));//等待人眼判断 的时间
	}
	else if(BLE_FT_Item=="重力传感器关")
		BLE_FT_Item_Int=22;
	else if(BLE_FT_Item=="温度")
	{
		BLE_FT_Item_Int=7;
		Temperature=Serial_Order_Return_CS;
	}
	else if(BLE_FT_Item=="心率直流校准")
	{
		BLE_FT_Item_Int=23;
		HR_DC=Serial_Order_Return_CS;
		BLE_Operate_Guide_Control.SetWindowTextA("心率直流校准，心率灯不要有任何遮挡！！！");
		BLE_Operate_Guide_Control.UpdateWindow();//改变内容与颜色
	}
	else if(BLE_FT_Item=="心率红灯")
	{
		BLE_FT_Item_Int=17;
		HeartRate_RL[0][1]=HeartRate_RL[0][0];
		HeartRate_RL[1][1]=HeartRate_RL[1][0];

		int npos=Serial_Order_Return_CS.Find("adc_");
		Serial_Order_Return_CS=Serial_Order_Return_CS.Mid(npos+4);
		npos=Serial_Order_Return_CS.Find("_");
		if(npos>=0)
		{
			HeartRate_RL[0][0]=Serial_Order_Return_CS.Left(npos);
			HeartRate_RL[1][0]=Serial_Order_Return_CS.Mid(npos+1,4);

			LogShow_exchange(5,"心率值:"+HeartRate_RL[0][1]+"，"+HeartRate_RL[1][1]+"；"+HeartRate_RL[0][0]+"，"+HeartRate_RL[1][0],&BLE_Final_Result_Control,&BLE_m_Result,0);//继续测试，状态不更新
			Log_Show_Global();
			BLE_m_Result.UpdateWindow();
		}
		else
		{
			HeartRate_RL[0][0]=Serial_Order_Return_CS;//第一个0标示为交流，第二个0标示第1个交流。换了指令过后，只有交流  at+aio=0
			//HeartRate_RL[0][0]="";//值保持不变
			//HeartRate_RL[1][0]="";
		}
		BLE_Operate_Guide_Control.SetWindowTextA("测试心率红灯，请用  手指  遮挡红色灯...");
		BLE_Operate_Guide_Control.UpdateWindow();//改变内容与颜色
	}
	else if(BLE_FT_Item=="心率红外")
	{
		BLE_FT_Item_Int=18;
		//HeartRate_IL[1]=HeartRate_IL[0];
		HeartRate_IL[0][1]=HeartRate_IL[0][0];
		HeartRate_IL[1][1]=HeartRate_IL[1][0];

		int npos=Serial_Order_Return_CS.Find("_");
		if(npos>=0)
		{
			HeartRate_IL[0][0]=Serial_Order_Return_CS.Left(npos);
			HeartRate_IL[1][0]=Serial_Order_Return_CS.Mid(npos+1);
		}
		else
		{
			HeartRate_IL[0][0]=Serial_Order_Return_CS;//第一个0标示为交流《当前存的是直流》，第二个0标示第1个交流。换了指令过后，只有直流  at+aio=1
			//HeartRate_IL[0][0]="";//值保持不变
			//HeartRate_IL[1][0]="";
		}
		//BLE_Operate_Guide_Control.SetWindowTextA("测试心率红外灯，请继续遮挡红外灯...");
		//BLE_Operate_Guide_Control.UpdateWindow();//改变内容与颜色
	}
	else if(BLE_FT_Item=="心率")
	{
		BLE_FT_Item_Int=8;
		HeartRate=Serial_Order_Return_CS;
		BLE_Operate_Guide_Control.SetWindowTextA("测试心率传感器，请...");
		BLE_Operate_Guide_Control.UpdateWindow();//改变内容与颜色
	}
	else if(BLE_FT_Item=="电池电量2")
	{
		BLE_FT_Item_Int=9;
		Power_Diff.Format("%f",atof(Serial_Order_Return_CS)-atof(Power_INI));
	}
	else if(BLE_FT_Item=="SOS按键_INI_State")
	{
		BLE_FT_Item_Int=10;
		BLE_Operate_Guide_Control.SetWindowTextA("长按SOS按键..并确认断开 外接电源！");
		BLE_Operate_Guide_Control.UpdateWindow();//改变内容与颜色
		Delay_Clock(atol(paraArray[i].Low_Limit_Value));

	}
	else if(BLE_FT_Item=="SOS按键")
	{
		BLE_FT_Item_Int=16;
		BLE_Operate_Guide_Control.SetWindowTextA("按下 按键，然后松开..");
		BLE_Operate_Guide_Control.UpdateWindow();//改变内容与颜色
		Delay_Clock(atol(paraArray[i].Low_Limit_Value));
	}
	else if(BLE_FT_Item=="马达")
	{
		BLE_FT_Item_Int=11;
		BLE_Operate_Guide_Control.SetWindowTextA("腕带振动则点击PASS.");
		BLE_Operate_Guide_Control.UpdateWindow();//改变内容与颜色
		Delay_Clock(atol(paraArray[i].High_Limit_Value));//等待人眼判断 的时间
	}
	else if(BLE_FT_Item=="LED灯ON")
	{
		BLE_FT_Item_Int=12;
		BLE_Operate_Guide_Control.SetWindowTextA("屏幕全白，请观察是否屏幕全白？");
		BLE_Operate_Guide_Control.UpdateWindow();//改变内容与颜色
		//Delay_Clock(atol(paraArray[i].High_Limit_Value));//等待人眼判断 的时间
	}
	else if(BLE_FT_Item=="LED灯OFF")
	{
		BLE_FT_Item_Int=13;
		BLE_Operate_Guide_Control.SetWindowTextA("屏幕全黑，请观察是否屏幕全黑？");
		BLE_Operate_Guide_Control.UpdateWindow();//改变内容与颜色
		//Delay_Clock(atol(paraArray[i].High_Limit_Value));//等待人眼判断 的时间
	}
	else if(BLE_FT_Item=="写功能测试标志")
		BLE_FT_Item_Int=14;
	else if(BLE_FT_Item=="恢复出厂设置")
		BLE_FT_Item_Int=15;

	else 
		BLE_FT_Item_Int=32;

	float Confirm=0.0;
	switch(BLE_FT_Item_Int)
	{
		case 1://读功能测试标志
			if(atoi(Serial_Order_Return_CS)<(atoi(paraArray[i].High_Limit_Value)-1))//工序不正常
				return false;
			break;
		case 2:
			if((Serial_Order_Return_CS.Find(paraArray[i].Low_Limit_Value))==-1)      //软件版本
				return false;
			break;
		case 3:
			if((WIFI_addr2>WIFI_addr4)||(WIFI_addr2<WIFI_addr3))                     //蓝牙地址
				return false;
			if(Serial_Order_Return_CS.GetLength() == 12)
			{
				for(int a=0;a<Serial_Order_Return_CS.GetLength();a++)
				{
					if((Serial_Order_Return_CS.GetAt(a) >='0' && Serial_Order_Return_CS.GetAt(a) <='9')  ||  (Serial_Order_Return_CS.GetAt(a) >='a' && Serial_Order_Return_CS.GetAt(a) <='f')||  (Serial_Order_Return_CS.GetAt(a) >='A' && Serial_Order_Return_CS.GetAt(a) <='F'))
					{					
					}
					else
					{
						return false;
					}
					
				}
			}
			else
				return false;
			break;
		case 4:                                                                    //SN标志位
			if((SN_BLE=="000000000000")||(Cal_Sign=="0000")||(Coupling_Sign<(atoi(paraArray[i].High_Limit_Value)-1))||(LimitOfTool!=paraArray[i].Low_Limit_Value.Right(11)) )
				return false;
			break;
		case 5:                                                                    //电池电量1
			//if()
				//return false;
			Power_INI=Serial_Order_Return_CS;
			break;
		case 21: 
			if((Serial_Order_Return_CS.Find(paraArray[i].High_Limit_Value))!=-1)//正确
			{
				//重力传感器开
			}
			else
				return false;
			break;
		case 22: 
			if((Serial_Order_Return_CS.Find(paraArray[i].High_Limit_Value))!=-1)//正确
			{
				//重力传感器关
			}
			else
				return false;
			break;
		case 6:                                                                    //重力
			//Confirm=atof((SENSOR_Gravity[0][0]));
			//Confirm=atof((SENSOR_Gravity[1][0]));
			//Confirm=atof((SENSOR_Gravity[2][0]));
			BLE_Operate_Guide_Control.SetWindowTextA("重力传感检测中...");
			BLE_Operate_Guide_Control.UpdateWindow();
			if((SENSOR_Gravity[0][0]=="")||(SENSOR_Gravity[1][0]=="")||(SENSOR_Gravity[2][0]=="")\
				||(SENSOR_Gravity[0][1]=="")||(SENSOR_Gravity[1][1]=="")||(SENSOR_Gravity[2][1]=="")\
				||(abs(atof((SENSOR_Gravity[0][1]))-atof((SENSOR_Gravity[0][0])))<atof(paraArray[i].Low_Limit_Value))\
				||(abs(atof((SENSOR_Gravity[1][1]))-atof((SENSOR_Gravity[1][0])))<atof(paraArray[i].Low_Limit_Value))\
				||(abs(atof((SENSOR_Gravity[2][1]))-atof((SENSOR_Gravity[2][0])))<atof(paraArray[i].Low_Limit_Value)) )
				return false;
			break;
		case 19:
			if((Serial_Order_Return_CS.Find(paraArray[i].High_Limit_Value))!=-1)//正确
			{
			}
			else
			{
				BLE_Operate_Guide_Control.SetWindowTextA("重力中断开测试，请断开USB电源！");
				BLE_Operate_Guide_Control.UpdateWindow();//改变内容与颜色
				Delay_Clock(atol(paraArray[i].Low_Limit_Value));//等待移动中断
				return false;
			}
			break;
		case 20:
			if((Serial_Order_Return_CS.Find(paraArray[i].High_Limit_Value))!=-1)//正确
			{
				//BLE_Operate_Guide_Control.SetWindowTextA("..");
				//BLE_Operate_Guide_Control.UpdateWindow();//改变内容与颜色
				//Delay_Clock(atol(paraArray[i].Low_Limit_Value));//等待移动中断
			}
			else
				return false;
			break;
		case 7:                                                                    //温度
			if((atof(Serial_Order_Return_CS)>atof(paraArray[i].High_Limit_Value))||(atof(Serial_Order_Return_CS)<atof(paraArray[i].Low_Limit_Value)))
				return false;
			break;
		case 8:                                                                    //心率
			BLE_Operate_Guide_Control.SetWindowTextA("心率检测中...");
			BLE_Operate_Guide_Control.UpdateWindow();
			
			if((atoi(Serial_Order_Return_CS)>atoi(paraArray[i].High_Limit_Value))||(atoi(Serial_Order_Return_CS)<atoi(paraArray[i].Low_Limit_Value)))
			{
				Delay_Clock(atol(paraArray[i+1].High_Limit_Value));//等待心率测试 的时间
				return false;
			}
			break;
		case 23:
			if((atof(Serial_Order_Return_CS)>atof(paraArray[i].High_Limit_Value))||(atof(Serial_Order_Return_CS)<atof(paraArray[i].Low_Limit_Value)))
				return false;
			break;
		case 17:                                                                    //心率红灯
			/*if(((atol(HeartRate_RL[0][0])<atol(paraArray[i].Low_Limit_Value))||(atol(HeartRate_RL[1][0])>atol(paraArray[i].High_Limit_Value))\
				||(atol(HeartRate_RL[0][1])>atol(paraArray[i+1].Low_Limit_Value))||(atol(HeartRate_RL[1][1])<atol(paraArray[i+1].High_Limit_Value)))\
				&&\
				((atol(HeartRate_RL[0][0])>atol(paraArray[i+1].Low_Limit_Value))||(atol(HeartRate_RL[1][0])<atol(paraArray[i+1].High_Limit_Value))\
				||(atol(HeartRate_RL[0][1])<atol(paraArray[i].Low_Limit_Value))||(atol(HeartRate_RL[1][1])>atol(paraArray[i].High_Limit_Value)))\
				)*/
			  //先有遮挡，然后没有遮挡||
			  //先没有挡遮，然后没有遮挡
			//if( ((atol(HeartRate_RL[0][0])>atol(paraArray[i].Low_Limit_Value))  &&(atol(HeartRate_RL[1][0])<atol(paraArray[i].High_Limit_Value))) \
				&&((atol(HeartRate_RL[0][1])<atol(paraArray[i+1].Low_Limit_Value))&&(atol(HeartRate_RL[1][1])>atol(paraArray[i+1].High_Limit_Value))) \
				|| \
				((atol(HeartRate_RL[0][1])>atol(paraArray[i].Low_Limit_Value))  &&(atol(HeartRate_RL[1][1])<atol(paraArray[i].High_Limit_Value))) \
				&&((atol(HeartRate_RL[0][0])<atol(paraArray[i+1].Low_Limit_Value))&&(atol(HeartRate_RL[1][0])>atol(paraArray[i+1].High_Limit_Value))) \
				)
			long test1,test2,T1,T2;
			test1=abs(atol(HeartRate_RL[0][0]) -atol(HeartRate_RL[0][1]));
			T1=atol(paraArray[i].Low_Limit_Value);

			test2=abs(atol(HeartRate_RL[1][0]) -atol(HeartRate_RL[1][1]));
			T2=atol(paraArray[i].High_Limit_Value);
			if( ( ( abs(atol(HeartRate_RL[0][0]) -atol(HeartRate_RL[0][1])) >=atol(paraArray[i].Low_Limit_Value) )  \
				&& ((HeartRate_RL[0][0]!="")&& (HeartRate_RL[0][1]!="")) )				 &&              
				( abs(atol(HeartRate_RL[1][0]) -atol(HeartRate_RL[1][1])) >=atol(paraArray[i].High_Limit_Value) ) \
				&& ((HeartRate_RL[1][0]!="")&& (HeartRate_RL[1][1]!=""))    //交流、直流变化值
			  )
			{

				BLE_Operate_Guide_Control.SetWindowTextA("心率红色灯OK，松开遮挡");
				BLE_Operate_Guide_Control.UpdateWindow();
				Delay_Clock(500);//

			}
			else
			{
				Delay_Clock(atol(paraArray[i-1].High_Limit_Value));//等待心率红灯测试时间
				return false;
			}
			break;
		case 18:                                                                    //心率红外
			/*if(((atol(HeartRate_IL[0][0])<atol(paraArray[i].Low_Limit_Value))||(atol(HeartRate_IL[1][0])>atol(paraArray[i].High_Limit_Value))\
				||(atol(HeartRate_IL[0][1])>atol(paraArray[i+1].Low_Limit_Value))||(atol(HeartRate_IL[1][1])<atol(paraArray[i+1].High_Limit_Value)))\
				&&\
				((atol(HeartRate_IL[0][0])>atol(paraArray[i+1].Low_Limit_Value))||(atol(HeartRate_IL[1][0])<atol(paraArray[i+1].High_Limit_Value))\
				||(atol(HeartRate_IL[0][1])<atol(paraArray[i].Low_Limit_Value))||(atol(HeartRate_IL[1][1])>atol(paraArray[i].High_Limit_Value)))\
				)
			{
				BLE_Operate_Guide_Control.SetWindowTextA("松开遮挡.");
				BLE_Operate_Guide_Control.UpdateWindow();

				Delay_Clock(atol(paraArray[i-2].High_Limit_Value));//等待心率红灯测试时间
				return false;
			}
			else
			{
				BLE_Operate_Guide_Control.SetWindowTextA("心率红外灯OK");
				BLE_Operate_Guide_Control.UpdateWindow();
				Delay_Clock(500);//
			}*/
			//先有遮挡，然后没有遮挡||
			//先没有挡遮，然后没有遮挡
			//if( ((atol(HeartRate_IL[0][0])>atol(paraArray[i-1].Low_Limit_Value))  &&(atol(HeartRate_IL[1][0])<atol(paraArray[i-1].High_Limit_Value))) \
				&&((atol(HeartRate_IL[0][1])<atol(paraArray[i].Low_Limit_Value))&&(atol(HeartRate_IL[1][1])>atol(paraArray[i].High_Limit_Value))) \
				|| \
				((atol(HeartRate_IL[0][1])>atol(paraArray[i-1].Low_Limit_Value))  &&(atol(HeartRate_IL[1][1])<atol(paraArray[i-1].High_Limit_Value))) \
				&&((atol(HeartRate_IL[0][0])<atol(paraArray[i].Low_Limit_Value))&&(atol(HeartRate_IL[1][0])>atol(paraArray[i].High_Limit_Value))) \
			  )
			/*if( ( abs(atol(HeartRate_IL[0][0]) -atol(HeartRate_IL[0][1])) >=atol(paraArray[i-1].High_Limit_Value) ) \
				&& ((HeartRate_IL[0][0]!="")&& (HeartRate_IL[0][1]!=""))) //直流变化值大于
			{

				BLE_Operate_Guide_Control.SetWindowTextA("心率红外灯OK.");
				BLE_Operate_Guide_Control.UpdateWindow();
				Delay_Clock(500);//
				Send_Serial_Order(&Serial_Order_Return,"at+hrpower=0");
				Send_Serial_Order(&Serial_Order_Return,"at+hrpower=0");

				
			}
			else
			{
				Delay_Clock(atol(paraArray[i-2].High_Limit_Value));//等待心率红灯测试时间
				BLE_Operate_Guide_Control.SetWindowTextA("松开遮挡.");
				BLE_Operate_Guide_Control.UpdateWindow();
				return false;
			}*/
			if((Serial_Order_Return_CS.Find(paraArray[i].High_Limit_Value))!=-1)
			{
				Delay_Clock(500);//
			}
			else
				return false;
			break;
		case 9:                                                                    //充电电量
			BLE_Operate_Guide_Control.SetWindowTextA("充电测试完成，拔掉外部电源，测试电池...");
			BLE_Operate_Guide_Control.UpdateWindow();
			Delay_Clock(atol(paraArray[i+1].High_Limit_Value));//延时显示测试电池
			if((atof(Serial_Order_Return_CS)-atof(Power_INI))<atof(paraArray[i].Low_Limit_Value))
				return false;
			break;
		case 10:                                                                   //SOS按键初始状态
			if((Serial_Order_Return_CS.Find(paraArray[i].High_Limit_Value))!=-1)
			{
				//BLE_Operate_Guide_Control.SetWindowTextA("..");
				//BLE_Operate_Guide_Control.UpdateWindow();//改变内容与颜色
				//Delay_Clock(2000);//等待显示松开SOS按键
			}
			else
				return false;
			break;

		case 16:                                                                   //SOS按键SOS_LONG_PRESS
			if(((Serial_Order_Return_CS.Find(paraArray[i].High_Limit_Value))!=-1)||((Serial_Order_Return_CS.Find("key_2"))!=-1))
			{
				BLE_Operate_Guide_Control.SetWindowTextA("SOS按键测试完成");
				BLE_Operate_Guide_Control.UpdateWindow();//改变内容与颜色
				Delay_Clock(100);//等待显示松开SOS按键
			}
			else
				return false;
			break;

		case 11:                                                                   //马达--靠重力传感器来感应判断
			if((Watch_Result==false)&&(paraArray[i].Low_Limit_Value=="true"))
				return false;
			else
				Watch_Result=false;//该项测试OK，将标志清零
			break;
		case 12:                                                                   //LED灯ON
			if(Watch_Result==false)
			{
				BLE_Operate_Guide_Control.SetWindowTextA("屏幕全白则点击PASS!");
				BLE_Operate_Guide_Control.UpdateWindow();//改变内容与颜色
				Delay_Clock(atol(paraArray[i].High_Limit_Value));//等待人眼判断 的时间
				return false;
			}
			else
				Watch_Result=false;//该项测试OK，将标志清零
			break;
		case 13:                                                                   //LED灯OFF
			if(Watch_Result==false)
			{
				BLE_Operate_Guide_Control.SetWindowTextA("屏幕全黑则点击PASS!");
				BLE_Operate_Guide_Control.UpdateWindow();//改变内容与颜色
				Delay_Clock(atol(paraArray[i].High_Limit_Value));//等待人眼判断 的时间
				return false;
			}
			else
				Watch_Result=false;//该项测试OK，将标志清零
			break;
		case 14:                                                                   //写功能测试标志
		
			break;
		case 15:                                                                   //恢复出厂设置
			if(Serial_Order_Return_CS==paraArray[i].High_Limit_Value)
			{
			}
			else
				return false;
			break;
		default :
				return true;
			break;
	}
	return true;
}
void CCSR_Function_Test::OnBnClickedButton3()//关闭串口
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL CloseHandle_Return;
	if((hPort != INVALID_HANDLE_VALUE)&&(hPort != NULL))
	{
		CloseHandle_Return=CloseHandle(hPort);
		if(CloseHandle_Return==TRUE)
			//AfxMessageBox("关闭该串口成功！");
			;
		else
			;
			//AfxMessageBox("关闭该串口失败！！");

	}
	else
	{
		AfxMessageBox("该串口没有打开。");
	}
}

void CCSR_Function_Test::OnBnClickedButton4()//PASS
{
	// TODO: 在此添加控件通知处理程序代码
	Watch_Result=true;//成功为true
	RunCommandList.SetEvent();
}

void CCSR_Function_Test::OnBnClickedButton11()//FAIL
{
	// TODO: 在此添加控件通知处理程序代码
	Watch_Result=false;//失败为false
	RunCommandList.SetEvent();
}



UINT static __cdecl ThreadTestCSR(LPVOID pParam)
{
	CCSR_Function_Test* HostControl = (CCSR_Function_Test*)pParam;
	HostControl->StartSearch();
	return 0;
}
void CCSR_Function_Test::OnBnClickedButton5()
{
	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	CSRBTHostApp.IniData();
	m_bStopFlag=FALSE;
	DeviceCountM=paraArray[3].High_Limit_Value;
	Test_Handle=AfxBeginThread(ThreadTestCSR,(LPVOID)this,THREAD_PRIORITY_NORMAL,0,0,NULL);	
}
static int ReIniCount;
BOOL CCSR_Function_Test::StartSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	if(ReIniCount==0)
	{
		LogShow_exchange(0,"初始化CSR-USB蓝牙设备...",&BLE_Final_Result_Control,&BLE_m_Result,0);
		Log_Show_Global();
		BLE_m_Result.UpdateWindow();
		if(CSRBTHostApp.IniFunction()==-1)				//初始化蓝牙库
		{
			CSRBTHostApp.shutDown = TRUE;
			TerminateThread(CSRBTHostApp.Thread_Handle, -1);
			CSRBTHostApp.EndFunction();
			LogShow_exchange(128,"初始化USB蓝牙设备失败！",&BLE_Final_Result_Control,&BLE_m_Result,0);
			Log_Show_Global();
			BLE_m_Result.UpdateWindow();
			GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
			return FALSE;
		}
		else
		{
			ReIniCount=1;
			LogShow_exchange(5,"初始化USB蓝牙设备成功,开始搜索蓝牙设备...",&BLE_Final_Result_Control,&BLE_m_Result,0);
			Log_Show_Global();
			BLE_m_Result.UpdateWindow();
		}	
	}
	else
	{
		ReIniCount=2;
		LogShow_exchange(5,"USB蓝牙设备已经正常初始化过了,开始搜索蓝牙设备...",&BLE_Final_Result_Control,&BLE_m_Result,0);
		Log_Show_Global();
		BLE_m_Result.UpdateWindow();
	}
	START_Test_Control.EnableWindow(TRUE);

	while(m_bStopFlag==FALSE)
	{
		ResetEvent(CSRBTHostApp.syncEvent);
		ResetEvent(CSRBTHostApp.SearchDevEvent);
		ResetEvent(CSRBTHostApp.ConnectDevEvent);
		ResetEvent(CSRBTHostApp.GetInfEvent);
		ResetEvent(CSRBTHostApp.WriteDataEvent);
		ResetEvent(CSRBTHostApp.DisConnEvent);
		LogShow_exchange(5,"搜索蓝牙设备...",&BLE_Final_Result_Control,&BLE_m_Result,0);
		if(CSRBTHostApp.DiscovertDevice()==FALSE)
		{
			LogShow_exchange(5,"搜索蓝牙设备失败！",&BLE_Final_Result_Control,&BLE_m_Result,0);
			Log_Show_Global();
			BLE_m_Result.UpdateWindow();
			Sleep(1000);
			continue;
		}
		else
		{
			//显示搜索到的信息
			CString RSSI_CS[MIN_DISCOVERED_DEVICES];
			CString BTMacALL[MIN_DISCOVERED_DEVICES];
			CString BTDeviceName[MIN_DISCOVERED_DEVICES];

			CString RSSI_CSTemp[MIN_DISCOVERED_DEVICES];
			CString BTMacALLTemp[MIN_DISCOVERED_DEVICES];
			CString BTDeviceNameTemp[MIN_DISCOVERED_DEVICES];
			
			for(int j=0;j<CSRBTHostApp.nDevicesDiscovered;j++)
			{
				RSSI_CS[j].Format("%d",CSRBTHostApp.RSSI[j]);
				BTMacALL[j].Format("%04X:%02X:%06X",CSRBTHostApp.devicesDiscovered[j].nAp,CSRBTHostApp.devicesDiscovered[j].uAp,CSRBTHostApp.devicesDiscovered[j].lAp);
				BTDeviceName[j].Format("%-10s",CSRBTHostApp.DeviceName[j]);
			}
			LogShow_exchange(5,"由强到弱排序...",&BLE_Final_Result_Control,&BLE_MAC_RSSI,1);
			for(int j=0;j<CSRBTHostApp.nDevicesDiscovered;j++)
			{
				for(int i=j+1;i<CSRBTHostApp.nDevicesDiscovered;i++)
				{
					if(atoi(RSSI_CS[j])<atoi(RSSI_CS[i]))
					{
						RSSI_CSTemp[j]=RSSI_CS[j];
						BTMacALLTemp[j]=BTMacALL[j];
						BTDeviceNameTemp[j]=BTDeviceName[j];

						RSSI_CS[j]=RSSI_CS[i];
						BTMacALL[j]=BTMacALL[i];
						BTDeviceName[j]=BTDeviceName[i];

						RSSI_CS[i]     =RSSI_CSTemp[j];
						BTMacALL[i]    =BTMacALLTemp[j];
						BTDeviceName[i]=BTDeviceNameTemp[j];
					}
				}
			}
			BOOL OnceExe[2];
			OnceExe[0]=FALSE;
			OnceExe[1]=FALSE;
			for(int j=0;j<CSRBTHostApp.nDevicesDiscovered;j++)
			{
				if((atoi(paraArray[4].Low_Limit_Value)<=atoi(RSSI_CS[j]))&&(atoi(paraArray[4].High_Limit_Value)>=atoi(RSSI_CS[j])))
				{
					if(OnceExe[0]==FALSE)
					{
						OnceExe[0]=TRUE;
						LogShow_exchange(5,"=================><RSSI达标设备>OKOK",&BLE_Final_Result_Control,&BLE_MAC_RSSI,1);
					}
					LogShow_exchange(5,"Name:"+BTDeviceName[j]+"   MACAddr:"+BTMacALL[j]+"   rssi:"+RSSI_CS[j],&BLE_Final_Result_Control,&BLE_MAC_RSSI,1);
				}
				else
				{
					if(OnceExe[1]==FALSE)
					{
						OnceExe[1]=TRUE;
						LogShow_exchange(5,"=================><RSSI异常设备>！！！",&BLE_Final_Result_Control,&BLE_MAC_RSSI,1);
					}
					LogShow_exchange(5,"Name:"+BTDeviceName[j]+"   MACAddr:"+BTMacALL[j]+"   rssi:"+RSSI_CS[j],&BLE_Final_Result_Control,&BLE_MAC_RSSI,1);
				}
				Log_Show_Global();
				BLE_m_Result.UpdateWindow();
			}
			//Sleep(2000);
			LogShow_exchange(5,"搜索蓝牙设备成功...",&BLE_Final_Result_Control,&BLE_MAC_RSSI,1);
			Log_Show_Global();
			BLE_m_Result.UpdateWindow();
		}
	}
	GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);	
	GetDlgItem(IDOK)->EnableWindow(TRUE);
	START_Test_Control.EnableWindow(FALSE);
	LogShow_exchange(5,"蓝牙搜索已经正常结束！",&BLE_Final_Result_Control,&BLE_m_Result,0);
	Log_Show_Global();
	BLE_m_Result.UpdateWindow();
	return TRUE;
}
void CCSR_Function_Test::OnBnClickedButton36()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bStopFlag=TRUE;
	LogShow_exchange(5,"请等待上次搜索进程...",&BLE_Final_Result_Control,&BLE_m_Result,0);
	Log_Show_Global();
	BLE_m_Result.UpdateWindow();
}

void CCSR_Function_Test::OnDestroy()
{
	CDialog::OnDestroy();	
}
void CCSR_Function_Test::OnClose()
{
	/*if(Test_Handle!=NULL)
	{
		if(WaitForSingleObject(Test_Handle->m_hThread, 5000) == WAIT_TIMEOUT)				//错误
		{
			TerminateThread(Test_Handle->m_hThread, -1);
		}
		Test_Handle=NULL;
	}
	OnCancel();*/
}
void CCSR_Function_Test::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	/*OnClose();
	OnCancel();*/
}

void CCSR_Function_Test::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	BLE_UI_LoadConfig();
	LogShow_exchange(5,"获取成功",&BLE_Final_Result_Control,&BLE_m_Result,0);
	//OnOK();
}
