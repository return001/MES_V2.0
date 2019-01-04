//以下是数据库Data_UpdatePara
BOOL IMEIWrite_MulAT::Data_UpdatePara(CAdoInterface& myado,int DataUpNum,CEdit* m_Result,CEdit* Final_Result_Control)
{
	_variant_t var;
	CString DB_FAIL;
	if(IMEI_Setdlg.Initial_Connect_DB==TRUE)//必须初始化连接数据库，不然报错”无效指针“
	{
		CString Conn="";
		Conn.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s",IMEI_Setdlg.m_server,IMEI_Setdlg.m_db,IMEI_Setdlg.m_user,IMEI_Setdlg.m_pwd);

		if(myado.m_pCon==NULL)
		{
			CoInitialize(NULL); 
			DB_FAIL=myado.ConnecDataLibrary(Conn,"","",adModeUnknown);   
		}
		if(myado.m_pCon->State==0)  //1表示已经打开,0表示关闭，数据库意外断开，重连
		{
			LogShow_exchange(m_Result,Final_Result_Control,5,"重新连接数据库.......",DataUpNum);
			DB_FAIL=myado.ConnecDataLibrary(Conn,"","",adModeUnknown);//数据库意外断开，重连
		}
		else
		{
			LogShow_exchange(m_Result,Final_Result_Control,0,"数据库连接正常中...",DataUpNum);
		}
		if(DB_FAIL=="FAIL")
		{
			LogShow_exchange(m_Result,Final_Result_Control,128,"连接数据库失败，无法上传数据！！！请检查网络.......",DataUpNum,"功能测试:3-DB");
			return FALSE;
		}

		if((ChipRfID[DataUpNum]=="")||(Software_Version[DataUpNum]==""))
		{
			LogShow_exchange(m_Result,Final_Result_Control,128,"SN或者软件版本为空",DataUpNum);
			return FALSE;
		}
	
		///////////////////////////////////////////////////////////
		//上传功能位
		
		LogShow_exchange(m_Result,Final_Result_Control,0,"查看功能表...",DataUpNum);

		myado.OpenSheet("select SN,Result,SoftModel,Version from dbo.Gps_AutoTest_Result WHERE SN ='"+ChipRfID[DataUpNum]+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'");//变1
		BOOL Barcode_Check=myado.Find("SN='"+ChipRfID[DataUpNum]+"'");
		Barcode_Check&=myado.Find("SoftModel='"+MachineType_CS+"'");
		Barcode_Check&=myado.Find("Version='"+Software_Version[DataUpNum]+"'");
		//Barcode_Check&=myado.Find("Result='1'");
		myado.CloseSheet();
		if(Barcode_Check==TRUE)
		{
			LogShow_exchange(m_Result,Final_Result_Control,0,"数据库已经有功能测试记录，更新...",DataUpNum);
		}
		else
		{
			LogShow_exchange(m_Result,Final_Result_Control,0,"数据库不存在，直接上传...",DataUpNum);
		}
		
		BOOL UP_Barcode=TRUE;
		BOOL Barcode_Check_UP;
		if(Barcode_Check==TRUE)
		{
			
			//更新--被原始系统测试过
			CString strSQL_Write_Barcode;
			myado.OpenSheet("select * from dbo.Gps_AutoTest_Result WHERE SN ='"+ChipRfID[DataUpNum]+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'");//变1

			if(1)
			{
				strSQL_Write_Barcode="UPDATE dbo.Gps_AutoTest_Result SET Result='1' WHERE SN ='"+ChipRfID[DataUpNum]+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'";//变2
				UP_Barcode=myado.Execute(strSQL_Write_Barcode,&var);
			}
			myado.CloseSheet();
			//更新
		}
		else
		{
			//直接上传
			CString strSQL1 = "Insert into dbo.Gps_AutoTest_Result(SN,IMEI,Version,SoftModel,Result,Remark,TesterId)\
							 values('"+ChipRfID[DataUpNum]+"','','"+Software_Version[DataUpNum]+"','"+MachineType_CS+"','1','gps','lbc')";//变3
				
				
			myado.OpenSheet("select * from dbo.Gps_AutoTest_Result");
			UP_Barcode=myado.Execute(strSQL1,&var);
			myado.CloseSheet();
		}
		if(UP_Barcode==TRUE)
		{	
			//再次检查
			Sleep(500);
			myado.OpenSheet("select SN,SoftModel,Version,Result from dbo.Gps_AutoTest_Result WHERE SN ='"+ChipRfID[DataUpNum]+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"' AND Result ='1'");//变3
			Barcode_Check_UP=myado.Find("SN='"+ChipRfID[DataUpNum]+"'");
			Barcode_Check_UP&=myado.Find("SoftModel='"+MachineType_CS+"'");
			Barcode_Check_UP&=myado.Find("Version='"+Software_Version[DataUpNum]+"'");
			Barcode_Check_UP&=myado.Find("Result='1'");//变1
			myado.CloseSheet();
			//if(var.intVal!=1)
				//Barcode_Check_UP=FALSE;
			//再次检查
		}	

		if((UP_Barcode==TRUE&&Barcode_Check_UP==TRUE))
		{
			LogShow_exchange(m_Result,Final_Result_Control,0,"SN:"+ChipRfID[DataUpNum]+"数据库上传功能位成功...",DataUpNum);
		}
		else
		{
			LogShow_exchange(m_Result,Final_Result_Control,128,"SN:"+ChipRfID[DataUpNum]+"数据库上传功能位失败",DataUpNum,"功能测试:3-DB");
			return FALSE;
		}

		//上传功能位
		///////////////////////////////////////////////////////////


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		LogShow_exchange(m_Result,Final_Result_Control,0,"查看数据总表...",DataUpNum);
		////////////////////////////////检查RFID、机型、软件版本是否已经存在//////////////////////////////
		myado.OpenSheet("select SN,SoftModel,Version,AutoTestResult from dbo.Gps_TestResult WHERE SN ='"+ChipRfID[DataUpNum]+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'");
		Barcode_Check=myado.Find("SN='"+ChipRfID[DataUpNum]+"'");
		Barcode_Check&=myado.Find("SoftModel='"+MachineType_CS+"'");
		Barcode_Check&=myado.Find("Version='"+Software_Version[DataUpNum]+"'");
		Barcode_Check&=myado.Find("AutoTestResult='1'");
		myado.CloseSheet();
		if(Barcode_Check==TRUE)
		{
			LogShow_exchange(m_Result,Final_Result_Control,0,"数据库总表已经有，此机已经测试过！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！...",DataUpNum);
			return TRUE;//有2
		}
		else
		{
			myado.OpenSheet("select SN,SoftModel,Version,AutoTestResult from dbo.Gps_TestResult WHERE SN ='"+ChipRfID[DataUpNum]+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'");
			Barcode_Check=myado.Find("SN='"+ChipRfID[DataUpNum]+"'");
			Barcode_Check&=myado.Find("SoftModel='"+MachineType_CS+"'");
			Barcode_Check&=myado.Find("Version='"+Software_Version[DataUpNum]+"'");
			myado.CloseSheet();
			if(Barcode_Check==TRUE)
			{
				LogShow_exchange(m_Result,Final_Result_Control,0,"数据库已经有功能测试记录，更新...",DataUpNum);
			}
			else
			{
				LogShow_exchange(m_Result,Final_Result_Control,0,"数据库不存在，直接上传...",DataUpNum);
			}
		}
		////////////////////////////////检查RFID是否已经存在//////////////////////////////
		UP_Barcode=TRUE;
		if(Barcode_Check==TRUE)
		{
			//更新
			CString strSQL_Write_Barcode;
			myado.OpenSheet("select * from dbo.Gps_TestResult WHERE SN ='"+ChipRfID[DataUpNum]+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'");

			if(1)
			{
				strSQL_Write_Barcode="UPDATE dbo.Gps_TestResult SET AutoTestResult = '1' WHERE SN ='"+ChipRfID[DataUpNum]+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"'";
				UP_Barcode=myado.Execute(strSQL_Write_Barcode,&var);
			}
			myado.CloseSheet();
			//更新
		}
		else
		{
			//直接上传
			CString strSQL1 = "Insert into dbo.Gps_TestResult(SN,IMEI,Version,SoftModel,AutoTestResult)\
							 values('"+ChipRfID[DataUpNum]+"','','"+Software_Version[DataUpNum]+"','"+MachineType_CS+"','1')";	
				
				
			myado.OpenSheet("select * from dbo.Gps_TestResult");
			UP_Barcode=myado.Execute(strSQL1,&var);
			myado.CloseSheet();
		}
		if(UP_Barcode==TRUE)
		{
			//再次检查
			Sleep(500);
			myado.OpenSheet("select SN,SoftModel,Version,CoupleResult,WriteImeiResult,ParamDownloadResult from dbo.Gps_TestResult WHERE SN ='"+ChipRfID[DataUpNum]+"' AND Version='"+Software_Version[DataUpNum]+"'AND SoftModel='"+MachineType_CS+"' ");
			Barcode_Check_UP=myado.Find("SN='"+ChipRfID[DataUpNum]+"'");
			Barcode_Check_UP&=myado.Find("SoftModel='"+MachineType_CS+"'");
			Barcode_Check_UP&=myado.Find("Version='"+Software_Version[DataUpNum]+"'");
			Barcode_Check_UP&=myado.Find("AutoTestResult='1'");
			myado.CloseSheet();
			//再次检查
		}	


		if((UP_Barcode==TRUE&&Barcode_Check_UP==TRUE))
		{
			LogShow_exchange(m_Result,Final_Result_Control,0,"SN:"+ChipRfID[DataUpNum]+"数据库上传总表成功...",DataUpNum);
		}
		else
		{
			LogShow_exchange(m_Result,Final_Result_Control,128,"SN:"+ChipRfID[DataUpNum]+"数据库上传总表失败",DataUpNum,"功能测试:3-DB");
			return FALSE;
		}
		
	}
	else
	{
		LogShow_exchange(m_Result,Final_Result_Control,0,"SN:"+ChipRfID[DataUpNum]+"没有勾选数据库不上传...",DataUpNum);
	}
	return TRUE;//LBCCheckIMEI
}
//以上是数据库Data_UpdatePara