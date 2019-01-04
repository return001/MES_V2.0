#ifndef	__COMMAND_H__
#define __COMMAND_H__


#define	RX1_Lench			128

 

bit RX1_End=0;	//-------------------------------接收完标志
bit Data_checkout=0;	//-----------------------数据校验成败标志，0为失败，错误数据，1为校验通过。
bit SuperAdministrator=0;//----------------------超级管理员标志

bit Admin_Mode_C=0;		//-----------------------管理员功能标志
bit LED_State_C=0; 	//---------------------------改状态LED标志
bit	BuzzerState_C=0;	//-----------------------改蜂鸣器状态标志
bit keyState_C=0;	//---------------------------改按键（继电器）状态标志
bit SVState_C=0; //------------------------------改电磁阀状态标志
bit SensorState_C=0;   //------------------------查询光感请求
bit Anyalyze_End;	//---------------------------转换完成标志位
bit PC_Request=0;	//---------------------------PC控制申请

bit Workpiece=0;	//---------------------------工件到位标志
bit Mechanical_Reset=0;	//-----------------------机械复位请求标志

char Fifty_ms=0;	 //--------------------------5毫秒定时
char Second=0;	//-------------------------------1S定时
char Five_ms=0;	 //------------------------------5毫秒定时
char CAdmin_Mode=0;//----------------------------管理员功能号
char CLEDState=0;//------------------------------LED通断
char CBuzzerState=0; //--------------------------蜂鸣器通断
char CKeyState=0;//------------------------------继电器通断
char CSVState=0; //------------------------------电磁阀通断
char CSensorState=0; //--------------------------查询位置光感



char CPosition=0;	


unsigned char   RX1_Cnt=0;	//-------------------串口1接收计数
unsigned char   String_Len=0;	//---------------字符操作命令长度


char idata RX1_Buffer[RX1_Lench];	//---串口1收缓冲
char idata MD5buffer[16];

char String_Function_Command[20];	//---截取字符操作命令缓冲
char String_Position_Command[3];	//---截取字符操作命令缓冲
char Rec_MD5_Hex[4];	//---------------截取MD5方式加密码缓冲
char Update_MD5_Hex[16];	//-----------通过截取字符操作命令转换成MD5加密码缓冲


void Cut_String() ;	//---------------------------截取字符串函数
void Rec_Command_to_MD5HEX();	//---------------字符串加密
void Command_check();	//-----------------------校验数据

void Function_Analyze();	//------------------（仅）命令功能解析
void Position_Analyze();	//------------------（仅）位置解析
void Command_Analyze();	//-----------------------命令+位置处理
void Data_Processing();	//-----------------------数据处理

#define TypeMAX      10		//行为功能分类
#define OrderMAX	 24		//列为功能细分

/*
extern char strcmp (char *s1, char *s2);
extern char strncmp (char *s1, char *s2, int n);
*/

unsigned char  *Command_Function[TypeMAX][OrderMAX]=
{
	 //------------------高级功能----------------------------------------------
	{"Debug",	"Exit",		"Hello",	"CpuRest",	"CodeUpte",		"AT^GT_CM=TEST"},   
	//进入调试---退出调试----呼叫----------复位------代码更新---------呼叫----------

	
	{"TNRL:",	"TFRL:",	"TNGL:",	"TFGL:"},
	//-开红灯----关红灯------开绿灯------关绿灯

	{"TNB:",	"TFB:"},
	//开蜂鸣器---关蜂鸣器	

		
	{"TNK:"},
	//开SOS按键

	
	{"TNASV:",	
	//开A位置气缸
	"TFASV:",		"TNBSV:",		"TFBSV:",		"TNCSV:",		"TFCSV:",
//关A位置气缸------开B位置气缸------关B位置气缸------开C位置气缸-----关C位置气缸	
	"TNDSV:",		"TFDSV:",		"TNESV:",		"TFESV:",		"TNFSV:",	
//开D位置气缸------关D位置气缸------开E位置气缸------关E位置气缸-----开F位置气缸
	"TFFSV:",		"TNGSV:", 	  "TFGSV:",			"TNHSV:",		"TFHSV:",	
//关F位置气缸------开G位置气缸------关G位置气缸------开H位置气缸-----关H位置气缸
	"TNISV:",		"TFISV:"},	   
//开I位置气缸------关I位置气缸	



	
	{"Cstj:",		"Cstk:",		"Cstl:",		"Cstm:",		"Cstn:"}
//查j位置光感-----查k位置光感----查l位置光感------查m位置光感----查n位置光感														   
};

unsigned char   *Command_Position[16]=
{
	//-----------------------------------------------------------1-8号工位-------------------
	"01a",		"02b",		"03c",		"04d",		"05e",		"06f",		"07g",		"08h",
	//-----------------------------------------------------------9-16号工位------------------
	"09i",		"10j",		"11k",		"12l",		"13m",		"14n",		"15o",		"16p"
};
 



#endif