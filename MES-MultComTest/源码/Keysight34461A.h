#include "extcode.h"
#pragma pack(push)
#pragma pack(1)

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Configures the measurement type, resolution, and range on the instrument.
 */
int32_t __cdecl ConfMeas(char InstrName[], double ManualRange1, 
	uintptr_t *VISAResourceNameOut);
/*!
 * ConfMeasB
 */
int32_t __cdecl ConfMeasB(char InstrName[], double ManualRange1, 
	uintptr_t *VISAResourceNameOut);
/*!
 * 使错误或警告代码转换为错误簇。 
 * 收到共享库调用的返回值或返回用户定义错误代码时，可使用该VI。
 */
int32_t __cdecl ErrCode(int32_t errorCode0, char errorMessage[], 
	LVBoolean showCallChainFalse, LVBoolean isWarningFalse);
/*!
 * Queries the instrument for any errors in the instrument's error queue.  It 
 * will continue to read errors until all errors have been read from the 
 * instrument.  Any errors will be returned in the <B>error out</B> control.
 * 
 * <B>Note:</B>
 * This VI is called automatically by most driver VIs so it is not usually 
 * necessary to call this in an application.
 */
int32_t __cdecl ErrQuery(uintptr_t *VISAResourceName, 
	uintptr_t *VISAResourceNameOut, int32_t ErrorCode[], char ErrorMessage[], 
	int32_t len, int32_t len2);

MgErr __cdecl LVDLLStatus(char *errStr, int errStrLen, void *module);

#ifdef __cplusplus
} // extern "C"
#endif

#pragma pack(pop)

