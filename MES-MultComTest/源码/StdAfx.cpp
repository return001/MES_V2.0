// stdafx.cpp : source file that includes just the standard includes
//	WriteIMEI.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
CString DeviceCountM="5";

CString UserNameDB;


//三合一新增功能
int g_ADCTFlag = 1;//因为要实现三合一的缘故，所以这里做一个标志位来实现自动登陆和自动获取数据，为1表示三合一，为0表示作为单个正常使用（三合一时会去掉所有弹出框

CString g_BackDoorIP = "192.168.0.240";
CString g_BackDoorDatabase = "GPSTest";
CString g_BackDoorUser = "sa";
CString g_BackDoorPassword = "sa123abc";
CString g_BackDoorSocket = "192.168.0.243";
//CString g_BackDoorIP = "10.10.11.110,1456";
//CString g_BackDoorDatabase = "GPSTest";
//CString g_BackDoorUser = "sa";
//CString g_BackDoorPassword = "jiminewpower";
//CString g_BackDoorSocket = "10.10.11.191";