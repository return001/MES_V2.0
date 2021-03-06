// IT6400.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "IT6400.h"
#include <ctime>
#include <string>
#pragma comment(lib,"IT6400.lib")
using  namespace std;
int main()
{   
	char InstrName[] = "USB0::0x2EC7::0x6432::602563013727830005::INSTR";//设备连接PC的串口名
	uintptr_t* VisaNameOut = 0;//串口号

	LVBoolean CloseInit = false;//
	LVBoolean* CloseOK = &CloseInit;//关闭是否成功
	
	LVBoolean DefaSetInit = false;
    LVBoolean * DefaSetValid= &DefaSetInit;//默认初始化
 
 	float VSetValue = 3.6;
 	LVBoolean VSetInit = false;
 	LVBoolean *VsetValid = &VSetInit;//设置电压
 
  	LVBoolean EnableOutput = true;
 	LVBoolean OutputInit = false;
  	LVBoolean *OutputSetValid = &OutputInit;//设置输出
 	
 
 	double VLimit = 3.8;
  	LVBoolean EnVLimitSet = true;
 	LVBoolean VLimitSetInit = false;
 	LVBoolean *VLimitSetValid = &VLimitSetInit;//电压上限
 	

 	double VInit = 0;
 	double *VMeasValue = &VInit;
 	LVBoolean VoltMeasInit = false;
 	LVBoolean *VMeasValid = &VoltMeasInit;//电压测试值
 

 
  	double CInit = 0;
  	double *CMeasValue = &CInit;
  	LVBoolean CMeasInit = false;
  	LVBoolean *CMeasValid= &CMeasInit;//电流测量值
 	


	Defaultset(InstrName, VisaNameOut, DefaSetValid);
	VSet(InstrName,VSetValue,VsetValid);
	VLimitConf(InstrName, VLimit, EnVLimitSet, VLimitSetValid);
	EnOutput(InstrName, EnableOutput, OutputSetValid);
	VMeas(InstrName, VMeasValue, VMeasValid);
	cout << *VMeasValue << endl;
    CMeas(InstrName,CMeasValue, CMeasValid );
	cout << *CMeasValue << endl;
	Close(InstrName, CloseOK);

	system("pause");
    return 0;
}

