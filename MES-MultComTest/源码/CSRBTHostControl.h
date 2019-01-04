#ifndef __UENERGYHOSTCONTROL_H__
#define __UENERGYHOSTCONTROL_H__


#include ".\uEnergyHost\uEnergyHost.h"

/* link with uEnergy Host library */
//#pragma comment( lib, "uEnergyHost" )

/* maximum discovered devices */
#define MIN_DISCOVERED_DEVICES		(32)

#define H_SERIAL_NUMBER          1   
#define H_FIRMWARE_REVISION      2    
#define H_HARDWARE_REVISION      3    
#define H_SOFTWARE_REVISION      4
#define H_MANUFACTURER_NAME      5
#define H_PNP_ID                 6
#define H_BATTERY_LEVEL          7
#define H_LED_BRG                8
#define H_LED_PWM                9
#define H_INVALID                0
#define H_END                    11

#define H_LED_TIME               10

#define  BLE_BUFFLEN             128

typedef struct  _Ble_Handle
{
 unsigned int        permition;   
 WORD                handle_char[2];
}Ble_Handle;

static const Ble_Handle ble_handle_table[]=
{
	{H_INVALID,              { NULL,NULL}},
	{H_SERIAL_NUMBER,        { 0x1B,0x00}},     
	{H_FIRMWARE_REVISION,    { 0x1F,0x00}},    
	{H_HARDWARE_REVISION,    { 0x1D,0x00}},    
	{H_SOFTWARE_REVISION,    { 0x21,0x00}}, 
	{H_MANUFACTURER_NAME,    { 0x23,0x00}}, 
	{H_PNP_ID,               { 0x25,0x00}}, 
	{H_BATTERY_LEVEL,        { 0x17,0x00}},  
	//{H_LED_BRG,              { 0x12,0x00}}, 
	//{H_LED_PWM,              { 0x10,0x00}},
	//{H_LED_TIME,             { 0x0E,0x00}},
	{H_LED_BRG,              { 0x1A,0x00}}, 
	{H_LED_PWM,              { 0x18,0x00}},
	{H_LED_TIME,             { 0x16,0x00}},
	{H_END,                  { NULL,NULL}},
};

typedef enum
{
	APP_IDLE						= 0,		//------库文件初始化成功，可以第一次搜索设备了(或者，断开前一个设备建立新连接)
	APP_DISCOVERING,							//正常运行状态(搜索中)
	APP_CONNECTING,								//正常运行状态(连接中)
	APP_CONNECTED,								//------可以通信了

	APP_ERROR,									//------错误
	APP_DISSTOP,								//------停止搜索状态，可以开始连接了
	APP_CONNFAILED,								//连接失败状态
	APP_WRITEOK,								//写数据成功

	APP_STATE_MAX
} appState;


typedef struct 
{
 WORD                len;   
 unsigned char data[BLE_BUFFLEN];
}Ble_DataBuf;


class CCSRBTHostControl
{
public:
	CCSRBTHostControl(void);
	~CCSRBTHostControl(void);
protected:
	
public:
	BOOL shutDown;															//shutdown flag
	HANDLE syncEvent;														//synchronization event object
	HANDLE SearchDevEvent;
	HANDLE ConnectDevEvent;

	int InfCounter;															//信息计数
	HANDLE GetInfEvent;
	HANDLE WriteDataEvent;
	HANDLE DisConnEvent;

	WORD syncData;															//synchronization word

	UINT uEnergyHostMessage;												//message id for uEnergy Host library messages
	DWORD connectHandle;													//connection handle

	int nDevicesDiscovered;
	/////////////////////////////////
	//变量初始化
	CSR_BLE_BLUETOOTH_ADDRESS devicesDiscovered[MIN_DISCOVERED_DEVICES];	//discovered LE devices
	CSR_BLE_BLUETOOTH_ADDRESS devices_Current;

	CString DeviceName[MIN_DISCOVERED_DEVICES];

	int RSSI[MIN_DISCOVERED_DEVICES];
	int RSSI_Current;
	//变量初始化
	/////////////////////////////////
	appState appCurState;													//当前状态

public:
	//BOOL ProcessOrder(char OrderInput);
	//void PrintGattDatabase(WORD nServices, PCSR_BLE_SERVICE services);

	void IniData(void);
	int IniFunction(void);
	
		
	BOOL DiscovertDevice(void);
	BOOL ConnectDevice(void);
	BOOL GetDataFunction(void);
	BOOL SendPWM(int data[4]);
	BOOL DisConn(void);
	
	int EndFunction(void);

public:
	BOOL LoadDllApi(void);
public:
	HINSTANCE hinstLib_uEnergyHost;
public:
	CWinThread* Thread_Handle;
	
	CString Device_Name;													//当前名词
	CString Soft_Ver;
	CString Bat_Level;
};

#endif
/*
Device Time Switch  [write]
Mode          1 Byte
Time          2 Byte
State         1 Byte

Device Timer Switch  [read]
Mode          1 Byte
Time          2 Byte
State         1 Byte

typedef enum
{
    led_normal = 0,//Application Initial State
    led_open,
    led_close
} led_state;

typedef struct
{
    uint8      mode;
    uint16     timer;
    uint8      led_state;
}SYSTEM_CONTROL_T;
*/