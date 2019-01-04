#pragma once

class CZebraPrinter
{
public:
	CZebraPrinter(void);
	~CZebraPrinter(void);
public:
	//²¢¿Ú
	HANDLE hPLPort;
public:
	bool  OnPrintFunction(int high,int width,int x,int y,char* SSID,char* PassWord,char* MSN,char* IMEI,char* Font,int X_Pos[10],int Y_Pos[10],char* CH_Font) ;
	CString PrintChinese(char *strSource,char* pCHN_ID,char* CH_Font,int high,int width,int x,int y);
	CString PrintContent(int high,int width,int x,int y,char* SSID,char* PassWord,char* MSN,char* IMEI,char* Font,int X_Pos[10],int Y_Pos[10],char* CH_Font)	;
};
