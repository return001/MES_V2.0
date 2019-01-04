#include "stdafx.h"
#include "FunctionContext.h"

CFunctionContext::CFunctionContext(Strategy * stg)
{
	stg_Pointer=stg;
}

CFunctionContext::~CFunctionContext(void)
{
	if(!stg_Pointer)
		delete stg_Pointer;
}
int CFunctionContext::DoAction(CWnd *pWnd,char* scanSN1_Accept,char*scanIMEI1_Accept,char*scanWIFI_Accept,int port,char* Software_Version)
{
	return stg_Pointer->AlgrithmInterface(pWnd,scanSN1_Accept,scanIMEI1_Accept,scanWIFI_Accept,port,Software_Version);
}