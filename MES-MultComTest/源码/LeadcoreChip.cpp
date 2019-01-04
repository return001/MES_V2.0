#include "stdafx.h"
#include "LeadcoreChip.h"


int CLeadcoreChip::Tstop=0;  

Strategy::Strategy(void)
{
}
Strategy::~Strategy(void)
{
}
int Strategy::AlgrithmInterface(CWnd *pWnd,char* scanSN1_Accept,char*scanIMEI1_Accept,char*scanWIFI_Accept,int port,char* Software_Version)
{
	return -1;
}

//==============================================================================
//
// Title:       AddressTool
// Purpose:     A short description of the application.
//
// Created on:  2013-5-10 at 9:31:03 by .
// Copyright:   Leadcore. All Rights Reserved.
//
//==============================================================================
// Include files
#include <windows.h>
#include "visa.h"

#define WAITTIME   3

ViSession Com_handle=0;    
  



CLeadcoreChip::CLeadcoreChip(void)
{
}

CLeadcoreChip::~CLeadcoreChip(void)
{
}

int CLeadcoreChip::AlgrithmInterface(CWnd *pWnd,char* scanSN1_Accept,char*scanIMEI1_Accept,char*scanWIFI_Accept,int port,char* Software_Version)
{
	//调用下面的函数写号
	return AssociateWnd(pWnd,scanSN1_Accept,scanIMEI1_Accept,scanWIFI_Accept,port,Software_Version);
	
}

int CLeadcoreChip::AssociateWnd(CWnd *pWnd,char* scanSN1_Accept,char*scanIMEI1_Accept,char*scanWIFI_Accept,int port,char* Software_Version)	//将主窗口的句柄,端口等信息传过来
{
	LBC_pWnd=pWnd;
	scanSN1=scanSN1_Accept;
	scanIMEI1=scanIMEI1_Accept;
	scanWIFI=scanWIFI_Accept;
	Com_port=port;
	Software_Ver=Software_Version;

	CString SN_Generate;
	SN_Generate.Format("自动生成SN号:%s\r\n",scanSN1);
	SendMSG((LPSTR)(LPCSTR)SN_Generate);
	return RUN_TEST();
}
void CLeadcoreChip::SendMSG(char* msg)			//向主窗口传输信息
{
	LBC_pWnd->SendMessage(WM_SEND_MSG,0,(LPARAM)msg);
}
/********************************************
//
// 函数名	: Com_init 
// 功能描述	: Com Initialise
// 输入参数	: 
// 输出参数	: 无
// 返回值	: 无 
// 备注	    : 进行串口初始化
//
*********************************************/
EnumErrorCode CLeadcoreChip::Com_init(int Rs232)
{

	ViSession rmsession=0;
	char str[50],strtemp[60];
	int ss;
	int time=10;
	
	memset (str,0,sizeof(str));
	memset (strtemp,0,sizeof(strtemp));
	if(Com_handle!=0) 
		viClose (Com_handle);
	Com_handle=0;
	memset(str,0,sizeof(str));
	sprintf(str,"ASRL%d::INSTR",Rs232);

	viOpenDefaultRM (&rmsession); //open remsession
	do
	{
		if(Tstop==1)  
			break; 
		ss = viOpen (rmsession, str, VI_LOAD_CONFIG, VI_NULL, &Com_handle);//获取handle
		if(ss!=0)
		{
			sprintf(strtemp,"VISA Open COM %s fail--%d!\r\n",str,(10-time+1));
			SendMSG(strtemp);
			Sleep(500);//Delay(2);
		}
	}while((ss!=0)&&(time--));  
	
	if(ss==0)																					  
	{
		viSetAttribute (Com_handle, VI_ATTR_ASRL_BAUD, 115200);
		viSetAttribute (Com_handle, VI_ATTR_ASRL_DATA_BITS, 8);
		viSetAttribute (Com_handle, VI_ATTR_ASRL_PARITY, VI_ASRL_PAR_NONE);
		viSetAttribute (Com_handle, VI_ATTR_ASRL_STOP_BITS, VI_ASRL_STOP_ONE);
		viSetAttribute (Com_handle, VI_ATTR_ASRL_FLOW_CNTRL, VI_ASRL_FLOW_NONE);

		SendMSG("初始化成功\r\n");
		return enNoneErr;
	}
	else
	{
		SendMSG("COMinit失败\r\n");
		return ComNoRsp;
	}
}


void CLeadcoreChip::lowtoup(char *ch)
{
	int i = 0;
    for(i=0;ch[i]!='\0';i++)
    {
        if(ch[i]>='a'&&ch[i]<='z')
        {
            ch[i]+='A'-'a';
        }
    }
}


/********************************************
//
// 函数名	: RUN_TEST 
// 功能描述	: Test start 
// 输入参数	: void
// 输出参数	: 无
// 返回值	: 无 
// 备注	    :  
//
*********************************************/
int CLeadcoreChip::RUN_TEST(void)
{
	int AMTstatus;														
	int Wait_t;															//连接超时设置
	char* modestatus;													//芯片模式状态
	char mode;															//芯片模式

	int tempIMEI1,tempSN1,tempWIFI,tempCKVER,tempMODE,tempONOFFAMT;
	EnumErrorCode  InitCOMstatus,err;					//进入模式返回的状态
	EnumErrorCode  status;
	char  *buf,*buf2;													//读取数据的缓存
	int   data[10];														//写入的标识位信息
	char str_info[50];													//写入芯片的标识位信息
	//unsigned char *rul;
	//unsigned char Resp1[16]={0xAA,0xBB,0xCC,0xDD,0x17,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x0F,0x00,0xBB,0x28};//用于AMT1模式
	int len,i;
	char Resp[2000]; 

	char str[100],str1[100],str2[100];
	char str3[100];														//输入的软件版本


	tempMODE=1;
	switch(tempMODE)
	{
		case 0: {
					mode = 0xFF;
					//sprintf(modestatus,"%s\0","TDAMT1");
					break;
				}	//0:TDAMT1          
		case 1: {
					mode = 0xDF;
					//sprintf(modestatus,"%s\0","TDAMT4");
					break;
				}   //1:TDAMT4
		case 2: {
					mode = 0xEF;
					//sprintf(modestatus,"%s\0","GSMAMT1");
					break;
				} 	//2:GSMAMT1
		case 3: {
					mode = 0xCF;
					//sprintf(modestatus,"%s\0","GSMAMT4");
					break;
				} 	//3:GSMAMT4
		case 4: {
					mode = 0xBF;
					//sprintf(modestatus,"%s\0","LTEAMT1");
					break;
				} 	//4:LTEAMT1
		case 5: {
					mode = 0xAF;
					//sprintf(modestatus,"%s\0","LTEAMT4");
					break;
				} 	//5:LTEAMT4
		default:break;
	}
	SendMSG("选择的模式为TDAMT4\r\n");
	
	if(Com_handle!=0) 
		viClose (Com_handle);
	//Wait_t=6; 
	Wait_t=0;
	err=enNoneErr;
	do
	{
		if(Tstop==1)  goto stop_end;
		InitCOMstatus = Com_init(Com_port);
		Wait_t++;
		AMTstatus=0;
	}while((InitCOMstatus!=enNoneErr)&&(Wait_t<WAITTIME));
	if(InitCOMstatus!=enNoneErr)  
	{
		goto fail_end;									//"初始化超时失败!\n"
	}

	
	if(Tstop==1)  goto stop_end;
	
	//Wait_t=2;
	Wait_t=0;
	status = AT_rd(Resp);
	status = enUnknownCmdErr;
	memset(Resp,0,sizeof(Resp));
	do
	{
		if(Tstop==1)  goto stop_end; 
		viPrintf (Com_handle, "AT\r\n");
		Sleep(100);
		status = AT_rd(Resp);
		Wait_t++;
	}while((Wait_t<WAITTIME)&&(status!=0));
	
	
	if(status==0)
	{
		SendMSG("进入生产测试模式(TDAMT4)成功!\r\n");
		err=enNoneErr;
		//AMT4模式下操作
		{
			tempIMEI1=1;
			if(tempIMEI1)																				//AMT4下写入IMEI1号
			{
				//////////////////////////
				//验证原始IMEI///////////
				memset(str2,0,sizeof(str2)); 
				sprintf(str2,"AT^DGSN?\r\n\0");															//AT读IMEI      
				SendMSG(str2);
				for(i=0;i<3;i++)
				{
					Wait_t=0;
					viWrite (Com_handle, (ViBuf)str2, strlen(str2), VI_NULL);
					memset(str,0,sizeof(str));               
					do
					{	
						if(Tstop) goto stop_end;
						Sleep(500);																		//Delay(0.5);
						err = AT_rd(str);
						Wait_t++;
					}while(((Wait_t<WAITTIME)&&strstr(str,"OK")==NULL));
					if(strstr(str,"OK")!=NULL) break;
				}
				if (strstr(str,"OK")!=NULL)
				{
			        memset(str1,0,sizeof(str1));
			        buf = strchr(str,':')+1;															//没空格 +1，有空格 +2
					for (i=0;i<10;i++)																	//如果有空格
					{
						strncpy(str1,buf,1);
						if(strcmp(str1," ")!=0) 
							break;
						else 
							buf=buf+1;
					}
			        strncpy(str1,buf,15);
					sprintf(str,"成功!\r\n读取原始的IMEI是：%s\r\n",str1);
					SendMSG(str);
				}
				else
				{
					SendMSG("读取原始IMEI失败！");
					goto fail_end;
				}
				//验证原始IMEI///////////
				/////////////////////////
				memset(str,0,sizeof(str));
				//Wait_t=10;
				Wait_t=0;
				sprintf(str,"AT^DCGSNW=%s\r\n",scanIMEI1);													//AT写IMEI         
				SendMSG(str);
				for(i=0;i<3;i++)
				{
					viWrite (Com_handle, (ViBuf)str, strlen(str), VI_NULL);
					memset(str2,0,sizeof(str2));               
					do
					{	
						if(Tstop) goto stop_end;
						Sleep(500);																			//Delay(0.5);
						err = AT_rd(str2);
						Wait_t++;
					}while((Wait_t<WAITTIME)&&strstr(str2,"OK")==NULL);
					if(strstr(str2,"OK")!=NULL) break;
				}
				if (strstr(str2,"OK")!=NULL)																//检查写入的IMEI号
				{
					SendMSG("写入IMEI成功,再次读取验证..\r\n");
					Sleep(100);																				//Delay(0.1);
					sprintf(str2,"AT^DGSN?\r\n\0");															//AT读IMEI       
					SendMSG(str2);
					for(i=0;i<3;i++)
					{
						Wait_t=0;
						viWrite (Com_handle, (ViBuf)str2, strlen(str2), VI_NULL);
						memset(str,0,sizeof(str));               
						do
						{	
							if(Tstop) goto stop_end;
							Sleep(500);																		//Delay(0.5);
							err = AT_rd(str);
							Wait_t++;
						}while(((Wait_t<WAITTIME)&&strstr(str,"OK")==NULL));
						if(strstr(str,"OK")!=NULL) break;
					}
					if (strstr(str,"OK")!=NULL)
					{
			            memset(str1,0,sizeof(str1));
			            buf = strchr(str,':')+1;															//没空格 +1，有空格 +2
						for (i=0;i<10;i++)																	//如果有空格
						{
							strncpy(str1,buf,1);
							if(strcmp(str1," ")!=0) 
								break;
							else 
								buf=buf+1;
						}
			            strncpy(str1,buf,15);
						sprintf(str,"成功!再次读取的IMEI是：%s\r\n",str1);
						SendMSG(str);
						if(strcmp(str1,scanIMEI1)==0)  
						{
							SendMSG("检查IMEI成功!\r\n");
							/**************写入IMEI的标识位信息开始****************/
							memset(data,0,sizeof(data));
							memset(str,0,sizeof(str));
							memset(str_info,0,sizeof(str_info));											//初始化   
							sprintf(str_info,"IMEI-PASS");													//decription信息为"IMEI-PASS"
							data[0]=19;																		//位置为19
							data[1]=1;																		//pass为1
							sprintf(str,"AT^DATOMW=%d,%d,%s\r\n\0",data[0],data[1],str_info);
							SendMSG(str);
							for(i=0;i<3;i++)
							{
								Wait_t=0;
								viWrite (Com_handle,(ViBuf)str,strlen(str),VI_NULL);
								memset(str,0,sizeof(str));               
								do
								{	
									if(Tstop) goto stop_end;
									Sleep(500);																//Delay(0.5);
									err = AT_rd(str);
									Wait_t++;
								}while((Wait_t<WAITTIME)&&strstr(str,"OK")==NULL);
								if(strstr(str,"OK")!=NULL) break;
							}
							if(strstr(str,"OK")!=NULL)
							{
								SendMSG("写入IMEI-PASS标志位成功\r\n");
							}
							else
							{
								SendMSG("写入IMEI-PASS标志位失败！\r\n");
								goto fail_end;
							}
							/**************写入IMEI的标识位信息结束****************/
						}
						else
						{
							SendMSG("写入和读出的IMEI不相符，检查IMEI失败！\r\n");
							goto fail_end;
						}
					} 	
					else
					{
						SendMSG("写入IMEI成功,再次读取IMEI失败！\r\n");
						goto fail_end;
					}					
				} 	
				else
				{
					SendMSG("写入IMEI失败！\r\n");
					goto fail_end;
				}	
			}

			tempSN1=1;
			if(tempSN1)		 //AMT4下写入SN1号
			{
				//Wait_t=5;
				sprintf(str,"AT^DMSN=%s\r\n\0",scanSN1);													//AT写MSN1            
				SendMSG(str);
				for(i=0;i<3;i++)
				{
					viWrite (Com_handle,(ViBuf)str, strlen(str), VI_NULL);
					memset(str2,0,sizeof(str2));               
					Wait_t=0;
					do
					{	
						if(Tstop) goto stop_end;
						Sleep(500);																			//Delay(0.5);
						err = AT_rd(str2);
						Wait_t++;
					}while((Wait_t<WAITTIME)&&strstr(str2,"OK")==NULL);
					if(strstr(str2,"OK")!=NULL) break;
				}
				if (strstr(str2,"OK")!=NULL)																//检查SN号写入
				{
					SendMSG("写入MSN成功,再次读取验证..\r\n");
					Sleep(200);																				//Delay(0.2);
					sprintf(str2,"AT^DMSN\r\n\0");															//AT读MSN      
					SendMSG(str2);
					for(i=0;i<3;i++)
					{
						viWrite (Com_handle, (ViBuf)str2, strlen(str2), VI_NULL);
						memset(str,0,sizeof(str));               
						Wait_t=0;
						do
						{	
							if(Tstop) goto stop_end;
							Sleep(500);																		//Delay(0.5);
							err = AT_rd(str);
							Wait_t++;
						}while((Wait_t<WAITTIME)&&strstr(str,"OK")==NULL);
						if(strstr(str,"OK")!=NULL) break;
					}
					if (strstr(str,"OK")!=NULL)
					{
						memset(str1,0,sizeof(str1));
						memset(str2,0,sizeof(str2));
						buf = strstr(str,"MSN:")+4;															//没空格 +1，有空格 +2
						for (i=0;i<10;i++)																	//如果有空格，忽略
						{
							strncpy(str1,buf,1);
							if(strcmp(str1," ")!=0) break;
							else buf=buf+1;
						}
						buf2 = strchr(buf,'\r'); 
						strncpy(str2,buf,buf2-buf);
						sprintf(str,"成功!再次读取的MSN是：%s\r\n",str2);
						SendMSG(str);

						if(strcmp(str2,scanSN1)==0)  
						{
							SendMSG("检查MSN成功!\r\n");
							/**************写入SN的标识位信息开始****************/
							memset(data,0,sizeof(data));
							memset(str,0,sizeof(str));
							memset(str_info,0,sizeof(str_info));											//初始化   
							sprintf(str_info,"SN-PASS");													//decription信息为"SN-PASS"
							data[0]=0;																		//位置为0
							data[1]=1;																		//pass为1
							sprintf(str,"AT^DATOMW=%d,%d,%s\r\n\0",data[0],data[1],str_info);
							SendMSG(str);
							for(i=0;i<3;i++)
							{
								viWrite (Com_handle,(ViBuf)str,strlen(str),VI_NULL);
								memset(str,0,sizeof(str));               
								Wait_t=0;
								do
								{	
									if(Tstop) goto stop_end;
									Sleep(500);																//Delay(0.5);
									err = AT_rd(str);
									Wait_t++;
								}while((Wait_t<WAITTIME)&&strstr(str,"OK")==NULL);
								if(strstr(str,"OK")!=NULL) break;
							}
							if(strstr(str,"OK")!=NULL)
							{
								SendMSG("写入SN-PASS标志位成功\r\n");
							}
							else
							{
								SendMSG("写入SN-PASS标志位失败！\r\n");
								goto fail_end;
							}
							/**************写入SN的标识位信息结束****************/
						}
						else
						{	SendMSG("写入和读出的MSN不相符，检查MSN失败！\r\n");
							goto fail_end;
						}
					} 	
					else
					{
						SendMSG("写入MSN成功,再次读取MSN失败！\r\n");
						goto fail_end;
					}					
				} 	
				else
				{
					SendMSG("写入MSN失败！\r\n");
					goto fail_end;
				}	
			}
			tempWIFI=1;
			if(tempWIFI)	 //AMT4下写入wifi地址
			{
				char scanWIFI_Temp[12];
				strcpy(scanWIFI_Temp,scanWIFI);
				sprintf(str,"AT^DWIFIADDR=\"%c%c:%c%c:%c%c:%c%c:%c%c:%c%c\"\r\n\0",scanWIFI_Temp[0],scanWIFI_Temp[1],scanWIFI_Temp[2],scanWIFI_Temp[3],scanWIFI_Temp[4],scanWIFI_Temp[5],scanWIFI_Temp[6],scanWIFI_Temp[7],scanWIFI_Temp[8],scanWIFI_Temp[9],scanWIFI_Temp[10],scanWIFI_Temp[11]);		
				char scanWIFI_tem[12];
				sprintf(scanWIFI_tem,"%c%c:%c%c:%c%c:%c%c:%c%c:%c%c",scanWIFI_Temp[0],scanWIFI_Temp[1],scanWIFI_Temp[2],scanWIFI_Temp[3],scanWIFI_Temp[4],scanWIFI_Temp[5],scanWIFI_Temp[6],scanWIFI_Temp[7],scanWIFI_Temp[8],scanWIFI_Temp[9],scanWIFI_Temp[10],scanWIFI_Temp[11]);
													//AT写WiFi地址            
				SendMSG(str);
				for(i=0;i<3;i++)
				{
					if(Tstop) goto stop_end;
					viWrite (Com_handle, (ViBuf)str, strlen(str), VI_NULL);
					memset(str2,0,sizeof(str2));               
					Wait_t=0;
					do
					{	
						Sleep(500);																			//Delay(0.5);
						err = AT_rd(str2);
						Wait_t++;
					}while((Wait_t<WAITTIME)&&strstr(str2,"OK")==NULL);
					if(strstr(str2,"OK")!=NULL) break;
				}
				if (strstr(str2,"OK")!=NULL)																//检查WIFI地址的写入
				{
					SendMSG("写入WIFI-MAC成功,再次读取验证..\r\n");
					Sleep(1000);																			//Delay(1);
					sprintf(str2,"AT^DWIFIADDR?\r\n\0");													//AT读WiFi地址            
					for(i=0;i<3;i++)
					{
						if(Tstop) goto stop_end;
						viWrite (Com_handle, (ViBuf)str2, strlen(str2), VI_NULL);
						memset(str,0,sizeof(str));               
						Wait_t=0;
						do
						{	
							Sleep(500);																		//Delay(0.5);
							err = AT_rd(str);
							Wait_t++;
						}while((Wait_t<WAITTIME)&&strstr(str,"OK")==NULL);
						if(strstr(str,"OK")!=NULL) break;
					}

					if (strstr(str,"OK")!=NULL)
					{
						memset(str1,0,sizeof(str1));
						memset(str2,0,sizeof(str2));
						buf = strstr(str,"WIFIADDR:")+9;   //没空格 +1，有空格 +2
						for (i=0;i<10;i++)						  
						{
							strncpy(str1,buf,1);
							if(strcmp(str1," ")!=0) 
								break;
							else 
								buf=buf+1;
						}
						buf2 = strchr(buf,'\r'); 
						strncpy(str2,buf+1,buf2-buf-2);

						sprintf(str,"成功!再次读取的WiFi地址是：%s\r\n",str2);
						SendMSG(str);
						if(strcmp(str2,scanWIFI_tem)==0)  
						{
							SendMSG("检查WIFI-MAC成功!\r\n");
						}
						else
						{
							SendMSG("写入和读出的MSN不相符，检查WIFI-MAC失败！\r\n");
							goto fail_end;
						}
					} 	
					else
					{
						SendMSG("写入WIFI-MAC成功,再次读取WIFI-MAC失败！\r\n");
						goto fail_end;
					}					
				} 	
				else
				{
					SendMSG("写入WIFI-MAC失败！\r\n");
					goto fail_end;
				}
			}
			tempCKVER=1;
			if(tempCKVER)	 ////AMT4下检查软件版本号
			{
				//Wait_t=5;
				Wait_t=0;
				sprintf(str,"%s\r\n\0","AT^DSVER");																//AT查询软件版本
				SendMSG(str);
				viWrite (Com_handle, (ViBuf)str, strlen(str), VI_NULL);
				memset(Resp,0,sizeof(Resp));               
				do
				{	
					if(Tstop) goto stop_end;
					Sleep(500);																					//Delay(0.5);
					err = AT_rd(Resp);
					Wait_t++;
				}while((Wait_t<WAITTIME)&&strstr(Resp,"OK")==NULL);
				if (strstr(Resp,"OK")!=NULL)																	//检查版本号的正确与否
				{
					memset(str1,0,sizeof(str1));
					memset(str2,0,sizeof(str2));
					buf = strstr(Resp,"^DSVER:")+8;																//没空格 +1，有空格 +2
					for (i=0;i<10;i++)						
					{
						strncpy(str1,buf,1);
						if(strcmp(str1," ")!=0) 
							break;
						else 
							buf=buf+1;
					}
					buf2 = strchr(buf,'\r'); 
					strncpy(str2,buf,buf2-buf);

					CString Software_Version_CS;
					Software_Version_CS.Format("%s",str2);
					Software_Version_CS.Replace("\r","");
					Software_Version_CS.Replace("\n","");
					Software_Version_CS.Replace(" ","");
					sprintf(str,"成功!读取的软件版本号为：\r\n%s\r\n",str2);
					SendMSG(str);
					
					//SendMSG((LPSTR)(LPCSTR)Software_Version_CS)


					sprintf(str,"目标版本为：%s\r\n",Software_Ver);
					SendMSG(str);
					
					//if(strcmp(str2,str3)==0)  
					if(Software_Version_CS.Find(Software_Ver)!=-1)
					{
						SendMSG("检查软件版本成功!\r\n");
					}
					else
					{
						SendMSG("检查软件版本失败！\r\n");
						goto fail_end;
					}
				} 
				else
				{
					SendMSG("读取软件版本失败！\r\n");
					goto fail_end;
				}
			}
			tempONOFFAMT=1;
			if(tempONOFFAMT)
			{
				len=0;
				err=enNoneErr;
				memset(str3,0,sizeof(str3));
				sprintf(str3,"%s\r\n\0","AT^DAMT1=0");		//0 not enter into AMT1 mode during next power-on
				//sprintf(str3,"%s\r\n\0","AT^DAMT1=1");	//用于测试指令AT^DAMT1=0 的效果
				SendMSG(str3);
				//Wait_t=1;     
				for(i=0;i<2;i++)
				{
					if(Tstop) goto stop_end;     
					viWrite (Com_handle, (ViBuf)str3, sizeof(str3), VI_NULL);
					memset(Resp,0,sizeof(Resp));               
					Wait_t=0;
					do
					{	
						if(Tstop==1) goto fail_end;
						err = AT_rd(Resp);
						Sleep(500);																					//Delay(0.5);
						Wait_t++;
					}while((Wait_t<WAITTIME)&&(strstr(Resp,"OK")==NULL));
					if(strstr(Resp,"OK")!=NULL) break; 
				}
				if(strstr(Resp,"OK")!=NULL)
				{
					SendMSG("close AMT1 mode when next power-on.指令成功,再次确认..\r\n");
		 		    memset(str3,0,sizeof(str3));
				    sprintf(str3,"%s\r\n\0","AT^DAMT1?");    
					SendMSG(str3);
					//Wait_t=1;     
					for(i=0;i<2;i++)
					{
						if(Tstop) goto stop_end;     
						viWrite (Com_handle, (ViBuf)str3, sizeof(str3), VI_NULL);
						memset(Resp,0,sizeof(Resp));               
						Wait_t=0;
						do
						{	
							if(Tstop) goto stop_end;
							err = AT_rd(Resp);
							Sleep(500);																				//Delay(0.5);
						}while((Wait_t<WAITTIME)&&strstr(Resp,"OK")==NULL);
						if(strstr(Resp,"OK")!=NULL) break; 
					}
					if(strstr(Resp,"OK")!=NULL)	 
					{
						//if(strstr(Resp,"^DAMT1: 1")!=NULL)  
						if(strstr(Resp,"^DAMT1: 0")!=NULL) 
						{
							SendMSG("close AMT1 mode when next power-on.成功\r\n");
						}
						else
						{
							SendMSG("close AMT1 mode when next power-on.失败！\r\n");
							goto fail_end;
						}
					}
					else
					{
						SendMSG("close AMT1 mode when next power-on.指令执行状态失败！\r\n");
						goto fail_end;
					}
					
				}
				else
				{
					SendMSG("close AMT1 mode when next power-on.指令失败！\r\n");
					goto fail_end;
				}
				
			}
		}
		SendMSG("写号全部成功！！！！！！！！！！！！！！！！！！\r\n\r\n");
		goto pass_end;
		
	}
	else 
	{
		SendMSG("无法生产测试模式ATM4！\r\n");
		goto fail_end;
	}




pass_end:

	sprintf(str,"AT^DSOFF\r\n\0");		 //AT关机   
	SendMSG(str);
	//sprintf(str,"AT^DSSRST=1\r\n\0");  //AT重启    
	for(i=0;i<2;i++)
	{   
		viWrite (Com_handle, (ViBuf)str, strlen(str), VI_NULL);
		memset(Resp,0,sizeof(Resp));               
		Wait_t=0;
		do
		{	
			if(Tstop==1) goto fail_end;
			err = AT_rd(Resp);
			Sleep(500);																					//Delay(0.5);
			Wait_t++;
		}while((Wait_t<WAITTIME)&&strstr(Resp,"OK")==NULL);
		if(strstr(Resp,"OK")!=NULL) break; 
	}
	if(strstr(Resp,"OK")!=NULL)	 
	{
		SendMSG("成功关机！\r\n");
	}
	else
	{
		SendMSG("关机失败！\r\n");
		goto fail_end;
	}
	//////////////////////////////////////////////////////////////////////////////
	///////////贴纸信息///////////////////////////////////////////////////////////////////
	if (tempWIFI!=0)
	{
		char scanWIFI_Temp[12];
		strcpy(scanWIFI_Temp,scanWIFI);
		memset(str,0,sizeof(str));
		sprintf (str,"网络名: TDL1261-%c%c%c%c%c%c\r\n",scanWIFI_Temp[6],scanWIFI_Temp[7],scanWIFI_Temp[8],scanWIFI_Temp[9],scanWIFI_Temp[10],scanWIFI_Temp[11]);
		lowtoup(str);
		SendMSG(str);

		char scanIMEI1_Temp[15];
		strcpy(scanIMEI1_Temp,scanIMEI1);
		memset(str,0,sizeof(str));
		sprintf (str,"TDL1261-%c%c%c%c%c%c%c%c\r\n",scanWIFI_Temp[8],scanWIFI_Temp[9],scanWIFI_Temp[10],scanWIFI_Temp[11],scanWIFI_Temp[12],scanWIFI_Temp[13],scanWIFI_Temp[14],scanWIFI_Temp[15]);
		lowtoup(str);
		SendMSG(str);
		
	}
	
	if (tempIMEI1!=0)
	{
		//密码 从第8位开始
		memset(str,0,sizeof(str));
		sprintf (str,"WIFI密码: %s\r\n",scanIMEI1+7);
		SendMSG(str);
		
		memset(str,0,sizeof(str));
		sprintf (str,"%s\r\n",scanIMEI1+7);
		SendMSG(str);

	}
	

	///////////////贴纸信息///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////			  
	return 0;
	
stop_end:

	sprintf(str,"AT^DSOFF\r\n\0");		 //AT关机   
	//sprintf(str,"AT^DSSRST=1\r\n\0");  //AT重启    
	SendMSG(str);
	for(i=0;i<1;i++)
	{   
		viWrite (Com_handle, (ViBuf)str, strlen(str), VI_NULL);
		memset(Resp,0,sizeof(Resp));               
		Wait_t=0;
		do
		{	
			if(Tstop==1) goto fail_end;
			err = AT_rd(Resp);
			Sleep(200);																					//Delay(0.5);
			Wait_t++;
		}while((Wait_t<WAITTIME)&&(strstr(Resp,"OK")==NULL));
		if(strstr(Resp,"OK")!=NULL) break; 
	}
	if(strstr(Resp,"OK")!=NULL)	 
	{
		SendMSG("成功关机！\r\n");
	}
	else
	{
		SendMSG("关机失败！\r\n");
		//goto fail_end;
	}
	return 1;
	
fail_end:
	sprintf(str,"AT^DSOFF\r\n\0");		 //AT关机   
	//sprintf(str,"AT^DSSRST=1\r\n\0");  //AT重启    
	SendMSG(str);
	for(i=0;i<1;i++)
	{   
		viWrite (Com_handle, (ViBuf)str, strlen(str), VI_NULL);
		memset(Resp,0,sizeof(Resp));               
		Wait_t=0;
		do
		{	
			//if(Tstop==1) goto fail_end;																//不屏蔽会导致死循环
			err = AT_rd(Resp);
			Sleep(200);																					//Delay(0.5);
			Wait_t++;
		}while((Wait_t<WAITTIME)&&(strstr(Resp,"OK")==NULL));
		if(strstr(Resp,"OK")!=NULL) break; 
	}
	if(strstr(Resp,"OK")!=NULL)	 
	{
		SendMSG("成功关机！\r\n");
	}
	else
	{
		SendMSG("关机失败！\r\n");
	}
	return 1;
}
EnumErrorCode CLeadcoreChip::Turn_on(char mode)
{
	int len,i,j;
	EnumErrorCode status;
	unsigned char Resp[8]={0xAA,0xBB,0xCC,0xDD,0xEF,0xFF,0xFF,0xFF};
	char  str[200];
	
	unsigned char rb;
	double Wait_t;

	status=ComRspErr;
	//Wait_t=10;

	Resp[4]=mode;
	Resp[5]=0xFF;
	Resp[6]=0xFF;
	Resp[7]=0xFF;
	//if(Com_handle!=0) viClose (Com_handle); 
	//开机时最多送3次开机查询信号，故最多循环4次
	for(j=0;j<20;j++)
	{
		if(Tstop==1)  break;
		rb=0;
		len=0;
		do//DD
		{
			if(Tstop==1)  break; 
			do//CC
			{
				if(Tstop==1)  break; 
				do//BB
				{
					if(Tstop==1)  break; 
					do//AA
					{
						if(Tstop==1)  break; 
						if(rb==0xAA)  break;
						Wait_t=0;
						do
						{
							if(Tstop==1) break;
							viGetAttribute (Com_handle, VI_ATTR_ASRL_AVAIL_NUM, &len);
							//Sleep(200);
							Wait_t++;
						}while((len==0) && (Wait_t<WAITTIME+100));
						if(len==0) break;
						viRead (Com_handle, &rb, 1, VI_NULL);
						sprintf(str,"\nUE->PC:\n%02x ",rb);
					}while(rb!=0xAA);
					///////////////////////////////////////
					if(len==0) break;
					Wait_t=0;
					do
					{
						if(Tstop==1) break;
						viGetAttribute (Com_handle, VI_ATTR_ASRL_AVAIL_NUM, &len);
						//Sleep(200);
						Wait_t++;
					}while((len==0) && (Wait_t<WAITTIME+100));
					if(len==0) break;
					viRead (Com_handle, &rb, 1, VI_NULL);
					sprintf(str,"%02x ",rb);
				}while(rb!=0xBB);
				////////////////////////////////////////////
				if(len==0) break;
				Wait_t=0;
				do
				{
					if(Tstop==1) break;
					viGetAttribute (Com_handle, VI_ATTR_ASRL_AVAIL_NUM, &len);
					//Sleep(200);
					Wait_t++;
				}while((len==0) && (Wait_t<WAITTIME+100));
				if(len==0) break;
				viRead (Com_handle, &rb, 1, VI_NULL);
				sprintf(str,"%02x ",rb);
			}while(rb!=0xCC);
			if(len==0) break;
			Wait_t=0;
			do
			{
				if(Tstop==1) break;
				viGetAttribute (Com_handle, VI_ATTR_ASRL_AVAIL_NUM, &len);
				//Sleep(200);
				Wait_t++;
			}while((len==0) && (Wait_t<WAITTIME+100));
			if(len==0) break;
			viRead (Com_handle, &rb, 1, VI_NULL);
			sprintf(str,"%02x ",rb);

		}while(rb!=0xDD);


		Wait_t=0;
		do
		{
			if(Tstop==1) break;
			viGetAttribute (Com_handle, VI_ATTR_ASRL_AVAIL_NUM, &len);
			//Sleep(200);
			Wait_t++;
		}while((len==0) && (Wait_t<WAITTIME+100));
		//if(len==0) break;
		viRead (Com_handle, &rb, 1, VI_NULL);
		sprintf(str,"%02x ",rb);
		if(Tstop==1) break;
		if(rb==0x7F)
		{
																								//"\nPC->UE:\n"
			viWrite (Com_handle, Resp, 8, VI_NULL);
			for(i=0;i<8;i++)
			{
				sprintf(str,"%02x ", Resp[i]);
			}
		}
		else if((rb==0x6F)&&(j>0)) 																//检测是否收到AA，BB，CC，DD，6F，判断进入到测试模式
		{
			status=enNoneErr;																	//成功进入测试模式，状态位置0
			break;
		}
		if(Tstop==1) break;
	}
	if(Tstop==0) 
		return status;   //输出执行结果
	else 
		return enUnknownCmdErr;
}
void CLeadcoreChip::STOP (void)
{
	if(Com_handle) viClose (Com_handle);
	return;
}


/********************************************
//
// 函数名	: AT_rd 
// 功能描述	: AT指令接收    
// 输入参数	: char *Resp
// 输出参数	: 无
// 返回值	:  
// 备注	    : 
//
*********************************************/
EnumErrorCode CLeadcoreChip::AT_rd(char *Resp)
{
	int len,i,j,k;
	char buf[6000];
	len=0;
	memset(buf,0,sizeof(buf));
	viGetAttribute (Com_handle, VI_ATTR_ASRL_AVAIL_NUM, &len);			//VI_ATTR_ASRL_AVAIL_NUM 接收缓冲区  字节个数
	int try_time=0;
	if(len>0)
	{
		k=0;
		do
		{
			j=0;
			viGetAttribute (Com_handle, VI_ATTR_ASRL_AVAIL_NUM, &len);
			if(len>0) 
			{	
				for (i=0;i<len;i++)
				{
					if(Tstop==1) break;
					viRead (Com_handle, (ViBuf)&buf[k], 1, VI_NULL);
					k++;
				}
			}
			try_time++;
		}while((len>0)&&(try_time<10));   				
		sprintf(Resp,"%s",buf);
		SendMSG(Resp);
		return enNoneErr; 
	}
	else 
		return ComNoRsp;
}		
/********************************************
//
// 函数名	: TestRead_AT 
// 功能描述	: AT方式读取测试信息内部函数    
// 输入参数	: void													  
// 输出参数	: 无
// 返回值	: Limit_check(err) 
// 备注	    : 
//的格式为  m,n，其中，m表示Station No.
//					   n：1 PASS/2 Fail
//
*********************************************/
EnumErrorCode CLeadcoreChip::TestRead_AT(int *data,char *Resp) 
{
	EnumErrorCode err;
	double Wait_t;
	char strtemp[50];
	
	//Wait_t=5;
	Wait_t=0;
	memset(strtemp,0,sizeof(strtemp));
	viPrintf (Com_handle, "AT^DATOMR=1,%d\r\n",data[0]);								//Read the result of AMT.
	do
	{	
		if(Tstop==1) break;
		err = AT_rd(Resp);
		Sleep(100);																		//Delay(0.1);
	}while((Wait_t<WAITTIME) && strstr(Resp,"OK")==NULL);
	if(strstr(Resp,"OK")!=NULL)  return enNoneErr;
	else  return ComNoRsp;
}
