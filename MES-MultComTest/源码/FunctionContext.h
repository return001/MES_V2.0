#pragma once
#include "LeadcoreChip.h"
class Strategy;
class CFunctionContext
{
public:
	CFunctionContext(Strategy * stg);
	~CFunctionContext(void);
	int DoAction(CWnd *pWnd,char* scanSN1_Accept,char*scanIMEI1_Accept,char*scanWIFI_Accept,int port,char* Software_Version);
protected:

private:
	Strategy *stg_Pointer;
};
