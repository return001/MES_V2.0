#include "stdafx.h"
#include "CurrentTest.h"
#include "Relay32.h"
#include "Keysight34461A.h"
#include "IT6400.h"
//#pragma comment(lib,"IT6400.lib")
//#pragma comment(lib,"Keysight34461A.lib")
//#pragma comment(lib,"Relay32.lib")

/*
*作者：刘维球
*功能：测试电流平均值
*时间：2019年2月18日16:06:09
返回值 平均电流 double：
输入参数 ：
参数1：第几路产品
参数2：仪表串口号
参数3：继电器串口号
参数4：测试次数
参数5：校准次数（最多测试几遍，如果第一遍测试通过就不再测试，）
参数6：继电器闭合延时多长时间 开始测试电流
参数7：电流的上限
参数8：电流的下限
参数9：电流测试的量程

*/


CurrentTest::CurrentTest()
{
}


CurrentTest::~CurrentTest()
{
}

bool CurrentTest::RelayChange(int proNum, static char InstrName[], static char RelayInstrName[], int testTimes, int  Delaytime)
{
	HINSTANCE bEzdDLL = ::LoadLibraryEx("Relay32.dll", NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	typedef void(*CMDSET)(char * COM, int32_t ReleyNum, LVBoolean on, LVBoolean* relay, LVBoolean* serial);
	CMDSET CmdSet = (CMDSET)GetProcAddress(bEzdDLL, "CmdSet");

	int32_t RelayNum1 = proNum;//继电器1，
	int32_t RelayNum2 = proNum + 1;//继电器2
	const  LVBoolean on = true;
	const  LVBoolean off = false;

	LVBoolean relay1 = false; //继电器指令1
	LVBoolean* relay1_Pt = &relay1;
	LVBoolean serial1 = false;
	LVBoolean* serial1_Pt = &serial1;

	LVBoolean relay2 = false; //继电器指令2
	LVBoolean* relay2_Pt = &relay2;
	LVBoolean serial2 = false;
	LVBoolean* serial2_Pt = &serial2;

	int count = 0;

	do
	{
		CmdSet(RelayInstrName, RelayNum2, on, relay2_Pt, serial2_Pt);//测试电源继电器闭合

		if (relay2 == true)
		{
			count = 0;
			break;
		}
		count++;
		if (count == 3)
		{
			return false;
		}
	} while (1);

	do
	{
		CmdSet(RelayInstrName, RelayNum1, on, relay1_Pt, serial1_Pt);//备用电源继电器断开

		if (relay1 == true)
		{
			count = 0;
			break;
		}
		count++;
		if (count == 3)
		{
			return false;
		}
	} while (1);


	Sleep(Delaytime); //刘维球：继电器开关闭合之后，稍微延迟一点测试，个人觉得如果继电器闭合，电流有一点波动

	do
	{
		CmdSet(RelayInstrName, RelayNum1, off, relay1_Pt, serial1_Pt);//刘维球： 备用电源继电器闭合
		if (relay1 == true)
		{
			count = 0;
			break;
		}
		count++;
		if (count == 3)
		{
			return false;
		}
	} while (1);

	do
	{
		CmdSet(RelayInstrName, RelayNum2, off, relay2_Pt, serial2_Pt);//刘维球： 测试电源继电器断开
		if (relay2 == true)
		{
			count = 0;
			break;
		}
		count++;
		if (count == 3)
		{
			return false;
		}
	} while (1);

	FreeLibrary(bEzdDLL);

	return true;
}

double CurrentTest::currentTest(int proNum, static char InstrName[], static char RelayInstrName[], int testTimes, int calibrationTimes, int  Delaytime, double currLowLimit, double curr_HightLimit, double currRange)
{
	HINSTANCE bEzdDLL = ::LoadLibraryEx("Relay32.dll", NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	typedef void(*CMDSET)(char * COM, int32_t ReleyNum, LVBoolean on, LVBoolean* relay, LVBoolean* serial);
	CMDSET CmdSet = (CMDSET)GetProcAddress(bEzdDLL, "CmdSet");

	double currAverage; //刘维球：最终的测试结果
	int32_t RelayNum1 = proNum;//继电器1，
	int32_t RelayNum2 = proNum + 1;//继电器2
	const  LVBoolean on = true;
	const  LVBoolean off = false;

	LVBoolean relay1 = false; //继电器指令1
	LVBoolean* relay1_Pt = &relay1;
	LVBoolean serial1 = false;
	LVBoolean* serial1_Pt = &serial1;

	LVBoolean relay2 = false; //继电器指令2
	LVBoolean* relay2_Pt = &relay2;
	LVBoolean serial2 = false;
	LVBoolean* serial2_Pt = &serial2;

	int count = 0;

	do
	{
		CmdSet(RelayInstrName, RelayNum2, on, relay2_Pt, serial2_Pt);//测试电源继电器闭合

		if (relay2 == true)
		{
			count = 0;
			break;
		}
		count++;
		if (count == 3)
		{
			return false;
		}
	} while (1);

	do
	{
		CmdSet(RelayInstrName, RelayNum1, on, relay1_Pt, serial1_Pt);//备用电源继电器断开

		if (relay1 == true)
		{
			count = 0;
			break;
		}
		count++;
		if (count == 3)
		{
			return false;
		}
	} while (1);

	Sleep(Delaytime); //刘维球：继电器开关闭合之后，稍微延迟一点测试，个人觉得如果继电器闭合，电流有一点波动

	for (unsigned int i = 0; i< calibrationTimes; i++) //校准的次数
	{
		//锁定仪器
		currAverage = currentAverage(InstrName, testTimes, currRange); //刘维球：电流的量程
		if (currAverage >= currLowLimit && currAverage <= curr_HightLimit); //
		break;
	}

	do
	{
		CmdSet(RelayInstrName, RelayNum1, off, relay1_Pt, serial1_Pt);//刘维球： 备用电源继电器闭合
		if (relay1 == true)
		{
			count = 0;
			break;
		}
		count++;
		if (count == 3)
		{
			return false;
		}
	} while (1);

	do
	{
		CmdSet(RelayInstrName, RelayNum2, off, relay2_Pt, serial2_Pt);//刘维球： 测试电源继电器断开
		if (relay2 == true)
		{
			count = 0;
			break;
		}
		count++;
		if (count == 3)
		{
			return false;
		}
	} while (1);

	FreeLibrary(bEzdDLL);

	return  currAverage;
}


/**
作者：刘维球
功能：求平均值
时间：2019年2月18日14:53:06
*/


double CurrentTest::average(vector<double>::const_iterator begin, vector<double>::const_iterator end, vector<double>::size_type elementsNum)
{
	double currAverage;
	double sum = 0;
	while (begin != end)
	{
		sum = sum + (*begin);
		begin++;
	}
	currAverage = sum / elementsNum;
	return currAverage;
}

/**
作者：刘维球
作用：测试电流
时间：2019年2月18日16:13:18
返回值：电流平均值
输入参数1：仪器的串口号：或者GPIB号
输入参数2：测试次数

*/


double CurrentTest::currentAverage(char InstrName[], int testNum, double currRange)
{
	HINSTANCE hEzdDLL = ::LoadLibraryEx("IT6400.dll", NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

	typedef void(*DEFAULTSET)(char * InstrName, uintptr_t*VisaNameOut, LVBoolean * DefaSetValid);
	DEFAULTSET Defaultset = (DEFAULTSET)GetProcAddress(hEzdDLL, "Defaultset");

	typedef void(*CMEAS)(char * InstrName, double *CMeasValue, LVBoolean * CMeasValid);
	CMEAS CMeas = (CMEAS)GetProcAddress(hEzdDLL, "CMeas");

	HINSTANCE aEzdDLL = ::LoadLibraryEx("Keysight34461A.dll", NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	typedef int32_t(*CONFMEAS)(char * InstrName, double ManuaRange1, uintptr_t * VISAResourceNameOut);
	CONFMEAS ConfMeas = (CONFMEAS)GetProcAddress(aEzdDLL, "ConfMeas");

	typedef int32_t(*CONFMEASB)(char * InstrName, double ManuaRange1, uintptr_t * VISAResourceNameOut);
	CONFMEASB ConfMeasB = (CONFMEASB)GetProcAddress(aEzdDLL, "ConfMeasB");


	uintptr_t* VisaNameOut = 0;//串口号
	LVBoolean DefaSetInit = false;
	LVBoolean * DefaSetValid = &DefaSetInit;//默认初始化
	Defaultset(InstrName, VisaNameOut, DefaSetValid);

	double CInit = 0;
	double *CMeasValue = &CInit;
	LVBoolean CMeasInit = false;
	LVBoolean *CMeasValid = &CMeasInit;//存放电流测量值的变量


	vector<double> cur_Arr; //存放测试电流的值的容器
	double curr_Average = 0;  //存放等待模式下电流的平均

	ConfMeas(InstrName, currRange, VisaNameOut); //测试结束之前，将量程调小

	ConfMeasB(InstrName, currRange, VisaNameOut);

	for (int i = 0; i<testNum; i++)
	{
		ConfMeas(InstrName, currRange, VisaNameOut); //测试结束之前，将量程调小
		ConfMeasB(InstrName, currRange, VisaNameOut);
		CMeas(InstrName, CMeasValue, CMeasValid);
		ConfMeas(InstrName, currRange, VisaNameOut); //测试结束之前，将量程调小
		ConfMeasB(InstrName, currRange, VisaNameOut);
		if (CMeasInit)
		{
			cur_Arr.push_back(CInit);
		}
	}
	ConfMeas(InstrName, 3, VisaNameOut); //测试结束之后，将量程调大，调到3A的测试范围已经足够机子重启
	ConfMeasB(InstrName, 3, VisaNameOut);
	curr_Average = average(cur_Arr.begin(), cur_Arr.end(), cur_Arr.size());

	FreeLibrary(hEzdDLL);
	FreeLibrary(aEzdDLL);

	return curr_Average;
}


/**
作者：刘维球
功能：设置电压值
时间：2019年2月18日15:32:52
返回值：bool 设置电压是否正确
输入参数1：InstrName[] 仪表串口号或者
输入参数2： voltage 预设电压值

*/

bool CurrentTest::PowerOn(char InstrName[], double voltage)
{
	HINSTANCE hEzdDLL = ::LoadLibraryEx("IT6400.dll", NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

	typedef void(*DEFAULTSET)(char * InstrName, uintptr_t*VisaNameOut, LVBoolean * DefaSetValid);
	DEFAULTSET Defaultset = (DEFAULTSET)GetProcAddress(hEzdDLL, "Defaultset");
	typedef void(*VLIMITCONF)(char * InstrName, double VLimit, LVBoolean EnVLimitSet, LVBoolean * VLimitSetValid);
	VLIMITCONF VLimitConf = (VLIMITCONF)GetProcAddress(hEzdDLL, "VLimitConf");
	typedef void(*VSET)(char * InstrName, float VSetValue, LVBoolean * VSetValid);
	VSET VSet = (VSET)GetProcAddress(hEzdDLL, "VSet");
	typedef void(*ENOUTPUT)(char * InstrName, LVBoolean EnOutput, LVBoolean * OutputSetValid);
	ENOUTPUT EnOutput = (ENOUTPUT)GetProcAddress(hEzdDLL, "EnOutput");
	//static char  = "GPIB0::23::INSTR";//设备连接PC的串口名
	uintptr_t* VisaNameOut = 0;//串口号

	LVBoolean DefaSetInit = false;
	LVBoolean * DefaSetValid = &DefaSetInit;//默认初始化
	LVBoolean OutputEnable;

	float VSetValue = voltage;
	LVBoolean VSetInit = false;
	LVBoolean *VsetValid = &VSetInit;//设置电压

	double VLimit = voltage*1.2; //刘维球：电压上限1.2倍
	LVBoolean EnVLimitSet = true;
	LVBoolean VLimitSetInit = false;
	LVBoolean *VLimitSetValid = &VLimitSetInit;//电压上限

	double VInit = 0;
	double *VMeasValue = &VInit;
	LVBoolean VoltMeasInit = false;
	LVBoolean *VMeasValid = &VoltMeasInit;//电压测试值

	float CSetValue = 0.3;
	LVBoolean CSetInit = false;
	LVBoolean* CSetValid = &CSetInit;//电流预设值设置

	LVBoolean EnableOutput = true;
	LVBoolean OutputInit = false;
	LVBoolean *OutputSetValid = &OutputInit;//设置输出
	OutputEnable = (bool)OutputInit;

	Defaultset(InstrName, VisaNameOut, DefaSetValid);
	VLimitConf(InstrName, VLimit, EnVLimitSet, VLimitSetValid);
	VSet(InstrName, VSetValue, VsetValid);
	EnOutput(InstrName, EnableOutput, OutputSetValid);

	FreeLibrary(hEzdDLL);

	return OutputInit;
}




