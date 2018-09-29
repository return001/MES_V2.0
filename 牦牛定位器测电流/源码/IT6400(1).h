#include "extcode.h"
#pragma pack(push)
#pragma pack(1)

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * CLimitConf
 */
void __cdecl CLimitConf(char InstrName[], double CLimit, 
	LVBoolean EnCLimitSet, LVBoolean *CLimitSetValid);
/*!
 * Performs an instrument error query before terminating the software 
 * connection to the instrument.
 */
void __cdecl Close(char InstrName[], LVBoolean *CloseValid);
/*!
 * CMeas
 */
void __cdecl CMeas(char Instr[], double *CMeasValue, LVBoolean *CMeasValid);
/*!
 * CSet
 */
void __cdecl CSet(char InstrName[], float CSetValue, LVBoolean *CSetValid);
/*!
 * Defaultset
 */
void __cdecl Defaultset(char InstrName[], uintptr_t *VISAResourceNameOut, 
	LVBoolean *DefaSetValid);
/*!
 * EnOutput
 */
void __cdecl EnOutput(char InstrName[], LVBoolean EnOutput2, 
	LVBoolean *OutputSetValid);
/*!
 * Establishes communication with the instrument and optionally performs an 
 * instrument identification query and/or an instrument reset.  It also places 
 * the instrument in a default state needed for other instrument driver 
 * operations.  Therefore, call this VI before calling other instrument driver 
 * VIs for this instrument.  Generally, you need to call the <b>Initialize</b> 
 * VI only once at the beginning of an application.
 */
void __cdecl Init(char InstrName[], LVBoolean ResetTrue, uint32_t BaudRate, 
	uint16_t DataBits, uint16_t ParityBits, uint16_t StopBits, 
	LVBoolean *InitValid);
/*!
 * This command to set the source output delay time, can set the source output 
 * delay time range from 0-999.999 s
 */
void __cdecl OutputDelayConfig(char InstrName[], double Delay, 
	LVBoolean *OutputDelayValid);
/*!
 * OCPConf
 */
void __cdecl OCPConf(char InstrName[], LVBoolean EnOCP, 
	LVBoolean *OCPSetValid);
/*!
 * Configures over voltage protection (OVP) of the power supply.  Specify the 
 * voltage limit and whether to enable or disable OVP.
 */
void __cdecl OVPConf(char InstrName[], double VRange, LVBoolean EnOVP, 
	LVBoolean *OVPSetValid);
/*!
 * ProClear
 */
void __cdecl ProClear(char InstrName[], LVBoolean *ProClearValid);
/*!
 * Query
 */
int32_t __cdecl Query(char InstrName[], uintptr_t *VISAResourceNameOut, 
	int32_t ErrorCode[], char ErrorMessage[], int32_t len, int32_t len2);
/*!
 * QueryError
 */
int32_t __cdecl QueryError(int32_t errorCode0, char errorMessage[], 
	LVBoolean showCallChainFalse, LVBoolean isWarningFalse);
/*!
 * VLimitConf
 */
void __cdecl VLimitConf(char InstrName[], double VLimit, 
	LVBoolean EnVLimitSet, LVBoolean *VLimitSetValid);
/*!
 * VMeas
 */
void __cdecl VMeas(char InstrName[], double *VMeasValue, 
	LVBoolean *VMeasValid);
/*!
 * VSet
 */
void __cdecl VSet(char InstrName[], float VSetValue, LVBoolean *VSetValid);

MgErr __cdecl LVDLLStatus(char *errStr, int errStrLen, void *module);

#ifdef __cplusplus
} // extern "C"
#endif

#pragma pack(pop)

