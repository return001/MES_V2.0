#include "StdAfx.h"
#include "CSRBTHostControl.h"
#include "..\uEnergyHost\uEnergyHostAPI.h"

CCSRBTHostControl::CCSRBTHostControl(void)
{
	shutDown = FALSE;
	syncEvent = NULL; 
	SearchDevEvent=NULL;
	ConnectDevEvent=NULL;

	GetInfEvent=NULL;
	WriteDataEvent=NULL;
	DisConnEvent=NULL;

	syncData = 0;
	uEnergyHostMessage = 0;
	
	connectHandle = CSR_BLE_INVALID_HANDLE;

	nDevicesDiscovered = 0;
	Thread_Handle=NULL;
	IniData();
	
}

CCSRBTHostControl::~CCSRBTHostControl(void)
{
}




UINT static __cdecl ThreadProc(LPVOID pParam)
{
	MSG msg;
	BOOL bRet;
	CCSRBTHostControl* HostControl = (CCSRBTHostControl*)pParam;

	while(!HostControl->shutDown)
	{
		//get the next message    (HWND)-1 窗口句柄，如果非零，则GetMessage只检索该窗口（也包括其子窗口）消息，如果为零，则GetMessage检索整个进程内的消息。非零窗口句柄必须属于GetMessage所属的进程。
		//while( (bRet = GetMessage(&msg, (HWND)-1, 0, 0)) != 0)			//如果函数取得WM_QUIT消息，返回值是零
		Sleep(10);
		while( (bRet = GetMessage(&msg, NULL, 0, 0)) != 0)			//如果函数取得WM_QUIT消息，返回值是零
		{ 
			if (bRet == -1)													//如果出现了错误，返回值是-1。例如，当hWnd是无效的窗口句柄或lpMsg是无效的指针时
			{
				Sleep(10);															/* handle the error and possibly exit */
			}
			else if(msg.message == HostControl->uEnergyHostMessage)			//received the message from uEnergy Host library
			{
				switch(msg.wParam)
				{
				case CSR_BLE_HOST_READY:
					{
						HostControl->syncData = (WORD)msg.lParam;			//uEnergy Host library初始化了，检查初始化结果
						
						
						SetEvent(HostControl->syncEvent);					//通知主线程
						if(HostControl->syncData==0)
						{	
							if(CsrBleHostFreeMessageContents(msg.wParam, msg.lParam)==FALSE)							//free uEnergy message
							{
								HostControl->appCurState = APP_ERROR;
							}
							 
							if(CsrBleHostCancel()==FALSE)					
							{
								HostControl->appCurState = APP_ERROR;
							}
							if(CsrBleHostCancel()==FALSE)					
							{
								HostControl->appCurState = APP_ERROR;
							}
							//TerminateThread(HostControl->Thread_Handle, -1);
							//HostControl->EndFunction();
							//break;	
							
							//HostControl->Thread_Handle->SuspendThread();
							//ExitThread(-1);
						}

					}
					break;

				case CSR_BLE_HOST_SEARCH_RESULT:							//device discovery result
					{
						PCSR_BLE_DEVICE_SEARCH_RESULT searchResult = (PCSR_BLE_DEVICE_SEARCH_RESULT)msg.lParam;

						//if(HostControl->nDevicesDiscovered < MIN_DISCOVERED_DEVICES)
						if(atoi(DeviceCountM)>MIN_DISCOVERED_DEVICES)
							DeviceCountM.Format("%d",MIN_DISCOVERED_DEVICES);
						if(atoi(DeviceCountM)<1)
							DeviceCountM.Format("%d",1);
						if(HostControl->nDevicesDiscovered < atoi(DeviceCountM))
						{
							//printf("\t[%d] %04X:%02X:%06X ", HostControl->nDevicesDiscovered, searchResult->deviceAddress.nAp, searchResult->deviceAddress.uAp, searchResult->deviceAddress.lAp);

							/*if(	(searchResult->informationReported & CSR_BLE_DEVICE_SEARCH_SHORT_NAME) ||
								(searchResult->informationReported & CSR_BLE_DEVICE_SEARCH_COMPLETE_NAME))*/
							if(1)
							{
								//printf("%s ", searchResult->deviceName);
								if((searchResult->deviceName=="")||(searchResult->deviceName==NULL))
								{
									break;
								}
								else
									HostControl->DeviceName[HostControl->nDevicesDiscovered]=searchResult->deviceName;
							}
							else
							{
								break;
							}

							//printf("RSSI: %d\n", searchResult->rssi);
							HostControl->RSSI[HostControl->nDevicesDiscovered]=(int)searchResult->rssi;

							/* store discovered device information */
							HostControl->devicesDiscovered[HostControl->nDevicesDiscovered].nAp = searchResult->deviceAddress.nAp;
							HostControl->devicesDiscovered[HostControl->nDevicesDiscovered].uAp = searchResult->deviceAddress.uAp;
							HostControl->devicesDiscovered[HostControl->nDevicesDiscovered].lAp = searchResult->deviceAddress.lAp;

							HostControl->nDevicesDiscovered++;

							HostControl->appCurState = APP_DISCOVERING;
							
							if(HostControl->nDevicesDiscovered ==  atoi(DeviceCountM))
							{
								
								if(CsrBleHostStopDeviceSearch()==FALSE)											//停止查找
								{
									HostControl->appCurState = APP_ERROR;
									//return 0;
								}
							}
						}
					}
					break;

				case CSR_BLE_HOST_SEARCH_STOPPED:								//停止搜索消息
					{
						//printf("*** Device discovery stopped\n");
						HostControl->appCurState = APP_DISSTOP;
						SetEvent(HostControl->SearchDevEvent);	
					}
					break;

				case CSR_BLE_HOST_CONN_PARAM_UPDATE:							//连接参数配置更新
					{
						//connection parameters confirmation 
					}
					break;

				case CSR_BLE_HOST_CONNECT_RESULT:								//连接状态
					{
						PCSR_BLE_CONNECT_RESULT connectResult = (PCSR_BLE_CONNECT_RESULT)msg.lParam;
						if(connectResult->result)								//连接失败
						{
							//printf("*** Connection failed\n");
							HostControl->appCurState = APP_CONNFAILED;
							SetEvent(HostControl->ConnectDevEvent);				
						}
						else
						{
                            HostControl->connectHandle = connectResult->connectHandle;											//连接句柄
							HostControl->appCurState = APP_CONNECTING;
							if(CsrBleHostSetEncryption(HostControl->connectHandle, CSR_BLE_SECURITY_UNAUTHENTICATED)==FALSE)	//初始化配对(不加密)
							{
								HostControl->appCurState = APP_ERROR;
									//return 0;
							}
                        }
                    }
                    break;
					
				case CSR_BLE_HOST_JUSTWORKS_REQUEST:
					{
						PCSR_BLE_JUSTWORKS_REQUEST jwReq = (PCSR_BLE_JUSTWORKS_REQUEST)msg.lParam;		//pairing request
						HostControl->appCurState = APP_CONNECTING;
						if(CsrBleHostJustWorksResult(jwReq->deviceAddress, TRUE, TRUE)==FALSE)
						{
							HostControl->appCurState = APP_ERROR;
								//return 0;
						}
					}
					break;
				case CSR_BLE_HOST_SECURITY_RESULT:
					{
						PCSR_BLE_SECURITY_RESULT secRes = (PCSR_BLE_SECURITY_RESULT)msg.lParam;			//security result 

						if(secRes->result != CSR_BLE_SECURITY_BONDING_ESTABLISHED)						//配对失败
						{
							//printf("*** Paring Failed result: 0x%04X\n",secRes->result);
							/* cancel all ongoing operations */
							if(CsrBleHostCancel()==FALSE)												//Cancel current GATT operation
							{
								HostControl->appCurState = APP_ERROR;
									//return 0;
							}
							if(CsrBleHostDisconnect(HostControl->connectHandle)==FALSE)					//断开连接
							{
								HostControl->appCurState = APP_ERROR;
									//return 0;
							}
							HostControl->appCurState = APP_ERROR;
							SetEvent(HostControl->ConnectDevEvent);
						}
						else
						{
							HostControl->appCurState = APP_CONNECTING;
                            //printf("*** Paired.\n");													//pairing success 
						}
					}					
					break;

				case CSR_BLE_HOST_SET_ENCRYPTION_RESULT:
					{
						PCSR_BLE_SET_ENCRYPTION_RESULT secRes = (PCSR_BLE_SET_ENCRYPTION_RESULT)msg.lParam;

						if(secRes->result == 0x00)
						{
							//printf("*** Encrypted.\n");
							//printf("*** Connected.\n");
                            HostControl->appCurState = APP_CONNECTED;									//switch to the connected state
							SetEvent(HostControl->ConnectDevEvent);
                            /* show available commands */
                            //printf("\t[D]\tDisconnect\n");
                            //printf("\t[G]\tDiscover GATT database\n");
                            //printf("\t[R]\tRead attribute\n");
                            //printf("\t[C]\tWrite client configuration\n\n");
						}
						else
						{
							//printf("*** Encryption Failed result: 0x%04X\n",secRes->result);
							if(CsrBleHostCancel()==FALSE)												//Cancel current GATT operation
							{
								HostControl->appCurState = APP_ERROR;
									//return 0;
							}
							if(CsrBleHostDisconnect(HostControl->connectHandle)==FALSE)					//断开连接
							{
								HostControl->appCurState = APP_ERROR;
									//return 0;
							}
							HostControl->appCurState = APP_ERROR;
						}
					}
					break;

				case CSR_BLE_HOST_RSSI_RESULT:
					{
							PCSR_BLE_RSSI_RESULT stRssi = (PCSR_BLE_RSSI_RESULT) msg.lParam;
							if (stRssi->result == 0x00)
							{						
								HostControl->RSSI_Current=(int)stRssi->rssi; 
								HostControl->InfCounter+=1000;
							}
							else
							{
								HostControl->appCurState = APP_ERROR;
									//return 0;
							}
							if((HostControl->InfCounter==1111)||(HostControl->InfCounter==1000))
								SetEvent(HostControl->GetInfEvent);

					}
					break;
				case CSR_BLE_HOST_DISCONNECTED:															//断开状态								
					{
						/* device disconnected */
						//printf("*** Device disconnected\n");
						HostControl->appCurState = APP_IDLE;
						HostControl->connectHandle = CSR_BLE_INVALID_HANDLE;
						SetEvent(HostControl->DisConnEvent);
					}
					break;

				case CSR_BLE_CLIENT_DATABASE_DISCOVERY_RESULT:
					{
						/* Remote GATT database discovered */
						PCSR_BLE_DATABASE_DISCOVERY_RESULT discoveryResult = (PCSR_BLE_DATABASE_DISCOVERY_RESULT)msg.lParam;

						if(discoveryResult->result)
						{
							//printf("*** GATT database discovery failed\n");
							HostControl->appCurState = APP_ERROR;
						}
						else
						{
							//HostControl->PrintGattDatabase(discoveryResult->nServices, discoveryResult->services);	//print out GATT database
						}
					}
					break;

				case CSR_BLE_CLIENT_CHAR_READ_RESULT:													//读取结果
					{
						PCSR_BLE_CHAR_READ_RESULT readResult = (PCSR_BLE_CHAR_READ_RESULT)msg.lParam;
						if(readResult->result)
						{
							//printf("*** Error reading attribute\n");
							HostControl->appCurState = APP_ERROR;
						}
						else
						{
							//printf("*** Read attribute %04X: %d byte(s): ", readResult->charHandle, readResult->charValueSize);
							//for(int i = 0; i < readResult->charValueSize; i++)
							//{
								//printf("%02X ", ((LPBYTE)readResult->charValue)[i]);					//show the read data 
							//}

							Ble_DataBuf  ble_buf={{128},{0}};													//中间变量
							memcpy(ble_buf.data,(LPBYTE)readResult->charValue,readResult->charValueSize);
							for(int tt=0;tt<H_END;tt++)
							{
								if(ble_handle_table[tt].handle_char[0]==readResult->charHandle) 
								{
									switch(ble_handle_table[tt].permition)
									{
										case H_SERIAL_NUMBER:
											HostControl->Device_Name.Format("%s",ble_buf.data);
											//SetEvent(HostControl->GetInfEvent);
											HostControl->InfCounter+=1;
											break;

										case H_SOFTWARE_REVISION:
											HostControl->Soft_Ver.Format("%s",ble_buf.data);
											//SetEvent(HostControl->GetInfEvent);
											HostControl->InfCounter+=10;
											break;

										case H_BATTERY_LEVEL:
											HostControl->Bat_Level.Format("%d",ble_buf.data[0]);
											HostControl->InfCounter+=100;
											break;
										default:
											break;
									}
									if((HostControl->InfCounter==1111)||(HostControl->InfCounter==1000))
										SetEvent(HostControl->GetInfEvent);
									break;
								}
							}
						}
					}
					break;

				case CSR_BLE_CLIENT_CHAR_WRITE_RESULT:													//写数据结果
					{
						PCSR_BLE_WRITE_RESULT writeResult = (PCSR_BLE_WRITE_RESULT)msg.lParam;
						if(writeResult->result)
						{
							//printf("*** Error writing attribute\n");
							HostControl->appCurState = APP_ERROR;
						}
						else
						{
							HostControl->appCurState = APP_WRITEOK;
							SetEvent(HostControl->WriteDataEvent);
							//printf("*** Successfully wrote client configuration\n");
						}
					}
					break;

				case CSR_BLE_CLIENT_CHAR_NOTIFICATION:													//incoming notification or indication
					{
						PCSR_BLE_CHAR_NOTIFICATION notification = (PCSR_BLE_CHAR_NOTIFICATION)msg.lParam;
						//int i;

						//printf("*** Incoming notification/indication from %04X:%02X:%06X\n",notification->deviceAddress.nAp,notification->deviceAddress.uAp,notification->deviceAddress.lAp);
						//printf("*** Handle: %04X, size: %d bytes: ", notification->charHandle, notification->charValueSize);
						//for(i = 0; i < notification->charValueSize; i++)
						//{
							//printf("%02X ", ((LPBYTE)notification->charValue)[i]);
						//}
						//memcpy(HostControl->ble_buf.data,(LPBYTE)notification->charValue,notification->charValueSize);
					}
					break;

                case CSR_BLE_HOST_CONNECTION_UPDATE_REQUEST:
                   {

                        PCSR_BLE_CONNECTION_UPDATE_REQUEST updRequest = (PCSR_BLE_CONNECTION_UPDATE_REQUEST)msg.lParam;
                        //Accept
                        //CsrBleHostAcceptConnUpdate(updRequest->connectHandle, updRequest->id, TRUE); 
                        //or Reject
                        //CsrBleHostAcceptConnUpdate(updRequest->connectHandle, updRequest->id, FALSE);
				   }

                   break;

                case CSR_BLE_HOST_CONNECTION_UPDATED://重新连接的时候会调用
                   {
                        PCSR_BLE_CONNECTION_UPDATED updRequest = (PCSR_BLE_CONNECTION_UPDATED)msg.lParam;
						HostControl->appCurState = APP_CONNECTING;
                        //Contains the new connection parameters 
                   }

                  break;

				default:
					{
						//unsupported uEnergy Host library message
						//printf("*** Unhandled uEnergy Host library message (%X)\n", msg.wParam);
						Sleep(20);
						break;
					}
				}
				if(CsrBleHostFreeMessageContents(msg.wParam, msg.lParam)==FALSE)							//free uEnergy message
				{
					HostControl->appCurState = APP_ERROR;
						//return 0;
				}
			}
		} 
	}
	return 0;
}



void CCSRBTHostControl::IniData(void)
{
	
	//g_bleaddress.nAp=_wcstoi64(dev_address.Mid(0,4).AllocSysString(),NULL,16);
	//g_bleaddress.uAp=_wcstoi64(dev_address.Mid(4,2).AllocSysString(),NULL,16);
    //g_bleaddress.lAp=_wcstoi64(dev_address.Mid(6,6).AllocSysString(),NULL,16);
	for(int i=0;i<MIN_DISCOVERED_DEVICES;i++)
	{
		devicesDiscovered[i].nAp = 0X0000;
		devicesDiscovered[i].uAp = 0X00;
		devicesDiscovered[i].lAp = 0X000000;
		DeviceName[i]="";
		RSSI[i]=0;
	}
	devices_Current.nAp = 0X0000;
	devices_Current.uAp = 0X00;
	devices_Current.lAp = 0X000000;
	RSSI_Current=0;
}
int CCSRBTHostControl::IniFunction(void)
{
	//HANDLE thread;
	//DWORD threadId;
	//char keyPressed;

	CSR_BLE_TRANSPORT transport;
	transport.transportType = CSR_BLE_TRANSPORT_USB;						//use default USB driver for transport
	transport.usbDeviceNumber = CSR_BLE_DEFAULT_USB_DEVICE_NUMBER;			//端口0
	/*
	if(NULL == (thread = CreateThread(NULL, 0, ThreadProc, NULL, 0, &threadId)))
	{
		printf("*** Error creating thread (%X)\n", GetLastError());
		return -1;
	}
	*/
	LoadDllApi();

	
    if(NULL == (syncEvent = CreateEventA(NULL, TRUE, FALSE, TEXT("uEnergySyncEvent"))))			//create synchronization event
	{
        return -1;
    }
	if(NULL == (SearchDevEvent = CreateEventA(NULL, TRUE, FALSE, TEXT("SearchEvent"))))				//
	{
        return FALSE;
    }
	if(NULL == (ConnectDevEvent = CreateEventA(NULL, TRUE, FALSE, TEXT("ConnectEvent"))))		//
	{
        return FALSE;
    }
	if(NULL == (GetInfEvent = CreateEventA(NULL, TRUE, FALSE, TEXT("GetInEvent"))))				//
	{
        return FALSE;
    }
	if(NULL == (WriteDataEvent = CreateEventA(NULL, TRUE, FALSE, TEXT("WriteDEvent"))))
	{
        return FALSE;
    }
	if(NULL == (DisConnEvent = CreateEventA(NULL, TRUE, FALSE, TEXT("DisConnEvent"))))
	{
        return FALSE;
    }

	//if(0 == (uEnergyHostMessage = CsrBleHostInit(TRUE, NULL, &transport)))						//initialize uEnergy Host
	if(0 == (uEnergyHostMessage = CsrBleHostInit(FALSE, NULL, &transport)))				//initialize uEnergy Host
	{
		//printf("*** Unable to initialize uEnergy Host.\n");
		return -1;
	}

	
	Thread_Handle=AfxBeginThread(ThreadProc,(LPVOID)this,THREAD_PRIORITY_HIGHEST,0,0,NULL);		//create a thread to listen to uEnergy Host library messages
	if(Thread_Handle==NULL)
		return -1;

	Sleep(100);
	if(!CsrBleHostStart(Thread_Handle->m_nThreadID))//(threadId))								//This function starts uEnergy LE host stack and pass the listener thread id
	{
		//printf("*** Unable to start uEnergy Host.\n");
		return -1;
	}



	WaitForSingleObject(syncEvent,INFINITE);// INFINITE);											//wait for the initialization message from uEnergy Host
	ResetEvent(syncEvent);																		//reset the sync event
	if(syncData)																				//successful initialization, proceed with main application
	{
		appCurState = APP_IDLE;
		//printf("\t[S]\tStart/stop device discovery\n");//printf("\t[0]-[9]\tConnect to the selected device\n");//printf("\t[Q]\tQuit\n\n");
		/*
		while('Q' != (keyPressed = toupper(_getch())))
		{
			ProcessKeyPress(keyPressed);														//process user input---这里则开始执行操作
		}*/
	}
	else
	{
		//printf("*** Failed to start up uEnergy Host stack.\n");
		return -1;
	}

	return 1;
}

BOOL CCSRBTHostControl::LoadDllApi(void)
{
	if(hinstLib_uEnergyHost != NULL)
	{
		FreeLibrary(hinstLib_uEnergyHost);
		hinstLib_uEnergyHost = NULL;
	}
	hinstLib_uEnergyHost = LoadLibrary("uEnergyHost.dll");//("BROM_DLL");	   
    if (hinstLib_uEnergyHost != NULL)
	{
		CsrBleHostInit=(UINT (CALLSTYTLE *) (BOOL useLogging, LPCSTR logFilename,PCSR_BLE_TRANSPORT transport))GetProcAddress(hinstLib_uEnergyHost, "CsrBleHostInit");
		//CsrBleHostInit=(UINT (*) (BOOL useLogging, LPCSTR logFilename,PCSR_BLE_TRANSPORT transport))GetProcAddress(hinstLib_uEnergyHost, "CsrBleHostInit");
		CsrBleHostStart=(BOOL(CALLSTYTLE *) (DWORD threadId))GetProcAddress(hinstLib_uEnergyHost, "CsrBleHostStart");

		CsrBleHostStartWnd=(BOOL (CALLSTYTLE *) (HWND hWnd))GetProcAddress(hinstLib_uEnergyHost, "CsrBleHostStartWnd");
		CsrBleHostDeinit=(BOOL (CALLSTYTLE *) (void))GetProcAddress(hinstLib_uEnergyHost, "CsrBleHostDeinit");
		CsrBleHostStartDeviceSearch=(BOOL (CALLSTYTLE *) (DWORD searchTime))GetProcAddress(hinstLib_uEnergyHost, "CsrBleHostStartDeviceSearch");
		CsrBleHostStopDeviceSearch=(BOOL (CALLSTYTLE *) (void))GetProcAddress(hinstLib_uEnergyHost, "CsrBleHostStopDeviceSearch");
		CsrBleHostConnect=(BOOL (CALLSTYTLE *) (CSR_BLE_BLUETOOTH_ADDRESS address))GetProcAddress(hinstLib_uEnergyHost, "CsrBleHostConnect");
		CsrBleHostCancelConnect=(BOOL (CALLSTYTLE *) (void))GetProcAddress(hinstLib_uEnergyHost, "CsrBleHostCancelConnect");

		CsrBleHostSetConnectionParams=(BOOL (CALLSTYTLE *) (WORD scanInterval,WORD scanWindow, WORD connMin, WORD connMax, WORD latency, WORD timeout))GetProcAddress(hinstLib_uEnergyHost, "CsrBleHostSetConnectionParams");
		CsrBleHostUpdateConnectionParams=(BOOL (CALLSTYTLE *) (CSR_BLE_BLUETOOTH_ADDRESS address, WORD connMin, WORD connMax,WORD latency, WORD timeout, WORD minCeLength, WORD maxCeLength))GetProcAddress(hinstLib_uEnergyHost, "CsrBleHostUpdateConnectionParams");
		CsrBleHostDisconnect=(BOOL (CALLSTYTLE *) (DWORD connectHandle))GetProcAddress(hinstLib_uEnergyHost, "CsrBleHostDisconnect");
		CsrBleHostGetPairedDeviceList=(BOOL (CALLSTYTLE *) (LPWORD nPairedDevices, PCSR_BLE_PAIRED_DEVICE* pairedDevices))GetProcAddress(hinstLib_uEnergyHost, "CsrBleHostGetPairedDeviceList");
		CsrBleHostFreePairedDeviceList=(BOOL (CALLSTYTLE *) (PCSR_BLE_PAIRED_DEVICE pairedDevices))GetProcAddress(hinstLib_uEnergyHost, "CsrBleHostFreePairedDeviceList");
		CsrBleHostDebond=(BOOL (CALLSTYTLE *) (CSR_BLE_BLUETOOTH_ADDRESS device))GetProcAddress(hinstLib_uEnergyHost, "CsrBleHostDebond");
		CsrBleHostJustWorksResult=(BOOL (CALLSTYTLE *) (CSR_BLE_BLUETOOTH_ADDRESS device, BOOL accept, BOOL bond))GetProcAddress(hinstLib_uEnergyHost, "CsrBleHostJustWorksResult");
		CsrBleHostSetEncryption=(BOOL (CALLSTYTLE *) (DWORD connectHandle, BYTE security))GetProcAddress(hinstLib_uEnergyHost, "CsrBleHostSetEncryption");
		CsrBleHostFreeMessageContents=(BOOL (CALLSTYTLE *) (WPARAM wParam, LPARAM lParam))GetProcAddress(hinstLib_uEnergyHost, "CsrBleHostFreeMessageContents");
		CsrBleHostCancel=(BOOL (CALLSTYTLE *) ())GetProcAddress(hinstLib_uEnergyHost, "CsrBleHostCancel");
		CsrBleHostGetRssi=(BOOL (CALLSTYTLE *) (CSR_BLE_BLUETOOTH_ADDRESS address))GetProcAddress(hinstLib_uEnergyHost, "CsrBleHostGetRssi");
		CsrBleClientDiscoverServices=(BOOL (CALLSTYTLE *) (DWORD connectHandle))GetProcAddress(hinstLib_uEnergyHost, "CsrBleClientDiscoverServices");
		CsrBleClientDiscoverCharacteristics=(BOOL (CALLSTYTLE *) (DWORD connectHandle, WORD startHandle, WORD endHandle))GetProcAddress(hinstLib_uEnergyHost, "CsrBleClientDiscoverCharacteristics");

		CsrBleClientDiscoverCharacteristicsByUuid=(BOOL (CALLSTYTLE *) (DWORD connectHandle, CSR_BLE_UUID uuid, WORD startHandle, WORD endHandle))GetProcAddress(hinstLib_uEnergyHost, "CsrBleClientDiscoverCharacteristicsByUuid");
		CsrBleClientDiscoverCharacteristicDsc=(BOOL (CALLSTYTLE *) (DWORD connectHandle, WORD startHandle, WORD endHandle))GetProcAddress(hinstLib_uEnergyHost, "CsrBleClientDiscoverCharacteristicDsc");
		CsrBleClientDiscoverDatabase=(BOOL (CALLSTYTLE *) (DWORD connectHandle))GetProcAddress(hinstLib_uEnergyHost, "CsrBleClientDiscoverDatabase");
		CsrBleClientReadCharByHandle=(BOOL (CALLSTYTLE *) (DWORD connectHandle, WORD charHandle))GetProcAddress(hinstLib_uEnergyHost, "CsrBleClientReadCharByHandle");
		CsrBleClientReadCharsByHandles=(BOOL (CALLSTYTLE *) (DWORD connectHandle, WORD userTag, int nHandles, WORD* charHandle))GetProcAddress(hinstLib_uEnergyHost, "CsrBleClientReadCharsByHandles");
		CsrBleClientReadCharByUuid=(BOOL (CALLSTYTLE *) (DWORD connectHandle, CSR_BLE_UUID uuid, WORD startHandle, WORD endHandle))GetProcAddress(hinstLib_uEnergyHost, "CsrBleClientReadCharByUuid");
		CsrBleClientWriteCharByHandle=(BOOL (CALLSTYTLE *) (DWORD connectHandle, BOOL isSigned, WORD charHandle, WORD valueSize, LPVOID value))GetProcAddress(hinstLib_uEnergyHost, "CsrBleClientWriteCharByHandle");
		CsrBleClientWriteCfmCharByHandle=(BOOL (CALLSTYTLE *) (DWORD connectHandle, WORD charHandle, WORD offset, WORD valueSize, LPVOID value))GetProcAddress(hinstLib_uEnergyHost, "CsrBleClientWriteCfmCharByHandle");
		CsrBleClientWriteConfiguration=(BOOL (CALLSTYTLE *) (DWORD connectHandle, WORD charDscHandle, WORD charHandle, BYTE config))GetProcAddress(hinstLib_uEnergyHost, "CsrBleClientWriteConfiguration");

		CsrBleServerAllocDatabase=(BOOL (CALLSTYTLE *) (WORD nHandles))GetProcAddress(hinstLib_uEnergyHost, "CsrBleServerAllocDatabase");
		CsrBleServerAddDatabase=(BOOL (CALLSTYTLE *) (WORD startHandle, WORD nServices, PCSR_BLE_DB_SERVICE services))GetProcAddress(hinstLib_uEnergyHost, "CsrBleServerAddDatabase");
		CsrBleServerReadResponse=(BOOL (CALLSTYTLE *) (DWORD connectHandle, WORD handle, WORD response, WORD length, LPVOID value))GetProcAddress(hinstLib_uEnergyHost, "CsrBleServerReadResponse");
		CsrBleServerWriteResponse=(BOOL (CALLSTYTLE *) (DWORD connectHandle, WORD handle, WORD response))GetProcAddress(hinstLib_uEnergyHost, "CsrBleServerWriteResponse");

	}
	else
		return FALSE;

	return TRUE;
}

BOOL CCSRBTHostControl::DiscovertDevice(void)
{
	appCurState = APP_DISCOVERING;
	nDevicesDiscovered = 0;																			//蓝牙设备计数清零
	
	if(CsrBleHostStartDeviceSearch(25000)==FALSE)													//start device discovery
	{
		appCurState = APP_ERROR;
		return FALSE;
	}
	WaitForSingleObject(SearchDevEvent,25000);// INFINITE);
	ResetEvent(SearchDevEvent);
	if(CsrBleHostStopDeviceSearch()==FALSE)															//查找超时，停止查找（确保）
	{
		appCurState = APP_ERROR;
		return FALSE;
	}
	if((appCurState==APP_DISSTOP)&&(nDevicesDiscovered>0))											//处于停止查找状态，而且查找到有设备--成功
		return TRUE;
	else
		return FALSE;
	//下面是信息显示
}
BOOL CCSRBTHostControl::ConnectDevice(void)
{
	appCurState = APP_CONNECTING;
	
	/*if(CsrBleHostCancel()==FALSE)																//Cancel current GATT operation
	{
		appCurState = APP_ERROR;
		return FALSE;
	}*/
	if(CsrBleHostCancelConnect()==FALSE)
	{
		appCurState = APP_ERROR;
		//return FALSE;//没有连接的时候返回错误
	}
	Sleep(500);
	/*
	The default connection parameters are as follows:\n
	 *				<b>scan interval</b>: 4\n
	 *				<b>scan window</b>: 4\n
	 *				<b>connection interval minimum</b>: 40\n
	 *				<b>connection interval maximum</b>: 40\n
	 *				<b>slave latency</b>: 12\n
	 *				<b>supervision timeout</b>: 360
	*/
	//if(CsrBleHostSetConnectionParams(4100, 4096, 10, 10, 3, 400)==FALSE)
	if(CsrBleHostSetConnectionParams(4, 4, 9, 12, 0, 400)==FALSE)
	{
		appCurState = APP_ERROR;
		return FALSE;
	}

	/*if(CsrBleHostDebond(devices_Current)==FALSE)												//很重要
	{
		appCurState = APP_ERROR;
		return FALSE;
	}*/
	/*if(CsrBleHostJustWorksResult(devices_Current,TRUE,TRUE)==FALSE)							
	{
		appCurState = APP_ERROR;
		return FALSE;
	}*/
	
	if(CsrBleHostConnect(devices_Current)==FALSE)
	{
		appCurState = APP_ERROR;
		return FALSE;
	}
	WaitForSingleObject(ConnectDevEvent,25000);// INFINITE);
	ResetEvent(ConnectDevEvent);
	if(appCurState==APP_CONNECTED)																//成功建立连接
		return TRUE;
	else
		return FALSE;
}
BOOL CCSRBTHostControl::GetDataFunction(void)
{
	

	if(CsrBleHostGetRssi(devices_Current)==FALSE)
		return FALSE;
	//WaitForSingleObject(GetInfEvent,6000);
	//ResetEvent(GetInfEvent);

	/*
	if(CsrBleClientReadCharByHandle(connectHandle, ble_handle_table[H_SERIAL_NUMBER].handle_char[0])==FALSE)
		return FALSE;
	//WaitForSingleObject(GetInfEvent,6000);
	//ResetEvent(GetInfEvent);

	if(CsrBleClientReadCharByHandle(connectHandle, ble_handle_table[H_SOFTWARE_REVISION].handle_char[0])==FALSE)
		return FALSE;
	//WaitForSingleObject(GetInfEvent,6000);
	//ResetEvent(GetInfEvent);

	if(CsrBleClientReadCharByHandle(connectHandle, ble_handle_table[H_BATTERY_LEVEL].handle_char[0])==FALSE)
		return FALSE;
	*/
	WaitForSingleObject(GetInfEvent,6000);
	ResetEvent(GetInfEvent);

	//00015B00011C  DFC8E5087973 case CSR_BLE_HOST_CONNECTION_UPDATED:
	//CsrBleClientReadCharByHandle(connectHandle, ble_handle_table[H_FIRMWARE_REVISION].handle_char[0]);
	//CsrBleClientReadCharByHandle(connectHandle, ble_handle_table[H_HARDWARE_REVISION].handle_char[0]);
	//CsrBleClientReadCharByHandle(connectHandle, ble_handle_table[H_MANUFACTURER_NAME].handle_char[0]);
    //CsrBleClientReadCharByHandle(connectHandle, ble_handle_table[H_PNP_ID].handle_char[0]);
	//CsrBleClientReadCharByHandle(connectHandle, ble_handle_table[H_SYSTEM_TIME].handle_char[0]);
    //CsrBleClientReadCharByHandle(connectHandle, ble_handle_table[H_SYSTEM_PARA].handle_char[0]);
	if(appCurState != APP_ERROR)	
		return TRUE;
	else
		return FALSE;
}
BOOL CCSRBTHostControl::SendPWM(int data[4])
{
	unsigned char pwm_data[5];
	
    pwm_data[0]=0x0F;
    pwm_data[1]=data[0];
    pwm_data[2]=data[1];
	pwm_data[3]=data[2];
	pwm_data[4]=data[3];

	CSR_BLE_UUID uuid1;
	uuid1.uuid16=0x2A92;
	uuid1.uuid128[0]=0x00;
	uuid1.uuid128[1]=0x00;
	uuid1.uuid128[2]=0x2A;
	uuid1.uuid128[3]=0x92;
	uuid1.uuid128[4]=0x00;
	uuid1.uuid128[5]=0x00;
	uuid1.uuid128[6]=0x10;
	uuid1.uuid128[7]=0x00;
	uuid1.uuid128[8]=0x80;
	uuid1.uuid128[9]=0x00;

	uuid1.uuid128[10]=0x00;
	uuid1.uuid128[11]=0x80;
	uuid1.uuid128[12]=0x5F;
	uuid1.uuid128[13]=0x9B;
	uuid1.uuid128[14]=0x34;
	uuid1.uuid128[15]=0xFB;
	//CsrBleClientReadCharByUuid(connectHandle,uuid1,0x14,0x1E);

    if(CsrBleClientWriteCfmCharByHandle(connectHandle, ble_handle_table[H_LED_PWM].handle_char[0],0,sizeof(pwm_data), (LPVOID)&pwm_data)==FALSE)
		return FALSE;
	WaitForSingleObject(WriteDataEvent,6000);
	ResetEvent(WriteDataEvent);
	if(appCurState==APP_WRITEOK)
		return TRUE;
	else
		return FALSE;
}

BOOL CCSRBTHostControl::DisConn(void)
{
	// TODO: Add your control notification handler code here
	
	if((appCurState == APP_CONNECTED)||(appCurState == APP_WRITEOK))
	{
		if(CsrBleHostCancel()==FALSE)																//Cancel current GATT operation
		{
			appCurState = APP_ERROR;
			return FALSE;
		}
		Sleep(500);
		if(CsrBleHostDisconnect(connectHandle)==FALSE)
		{
			appCurState = APP_ERROR;
			return FALSE;
		}
	}
	WaitForSingleObject(DisConnEvent,6000);
	ResetEvent(DisConnEvent);
	if(appCurState == APP_IDLE)
		return TRUE;
	else
		return FALSE;
}

int CCSRBTHostControl::EndFunction(void)
{
	int i=0;
	shutDown = TRUE;

	if(syncData)
	{
		for(i=0;i<5;i++)
		{
			//if((CsrBleHostCancel()==TRUE)&&(CsrBleHostDeinit()==TRUE))																			//deinitialize uEnergy Host stack
				break;
		}
		syncData=0;
		if(i==5)
			return -1;
	}
																					//indicate shutdown to the thread and wait for it
	//if(WaitForSingleObject(thread, 5000) == WAIT_TIMEOUT)
	/*if(Thread_Handle!=NULL)
	{
		if(WaitForSingleObject(Thread_Handle->m_hThread, 15000) == WAIT_TIMEOUT)					//错误
		//if(WaitForSingleObject(Thread_Handle, 5000) == WAIT_TIMEOUT)
		{
			TerminateThread(Thread_Handle->m_hThread, -1);
		}
		Thread_Handle=NULL;
	}*/
	if(syncEvent!=NULL)
	{
		CloseHandle(syncEvent);																		//close event handle
	}
	if(SearchDevEvent!=NULL)
		CloseHandle(SearchDevEvent);		
	if(ConnectDevEvent!=NULL)
		CloseHandle(ConnectDevEvent);
	if(GetInfEvent!=NULL)
		CloseHandle(GetInfEvent);
	if(WriteDataEvent!=NULL)
		CloseHandle(WriteDataEvent);
	if(DisConnEvent!=NULL)
		CloseHandle(DisConnEvent);

	//if(Thread_Handle!=NULL)
		//CloseHandle(Thread_Handle->m_hThread);														//close thread handle
	return 1;
}
