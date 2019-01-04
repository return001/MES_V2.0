// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "bartender.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// DBtApplication properties

LPDISPATCH DBtApplication::GetActiveFormat()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

void DBtApplication::SetActiveFormat(LPDISPATCH propVal)
{
	SetProperty(0x1, VT_DISPATCH, propVal);
}

long DBtApplication::GetBuildNumber()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void DBtApplication::SetBuildNumber(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

LPDISPATCH DBtApplication::GetFormats()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void DBtApplication::SetFormats(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

BOOL DBtApplication::GetIsPrinting()
{
	BOOL result;
	GetProperty(0x4, VT_BOOL, (void*)&result);
	return result;
}

void DBtApplication::SetIsPrinting(BOOL propVal)
{
	SetProperty(0x4, VT_BOOL, propVal);
}

BOOL DBtApplication::GetIsProcessingCommandLines()
{
	BOOL result;
	GetProperty(0x5, VT_BOOL, (void*)&result);
	return result;
}

void DBtApplication::SetIsProcessingCommandLines(BOOL propVal)
{
	SetProperty(0x5, VT_BOOL, propVal);
}

CString DBtApplication::GetVersion()
{
	CString result;
	GetProperty(0x6, VT_BSTR, (void*)&result);
	return result;
}

void DBtApplication::SetVersion(LPCTSTR propVal)
{
	SetProperty(0x6, VT_BSTR, propVal);
}

BOOL DBtApplication::GetVisible()
{
	BOOL result;
	GetProperty(0x7, VT_BOOL, (void*)&result);
	return result;
}

void DBtApplication::SetVisible(BOOL propVal)
{
	SetProperty(0x7, VT_BOOL, propVal);
}

LPDISPATCH DBtApplication::GetWindow()
{
	LPDISPATCH result;
	GetProperty(0x8, VT_DISPATCH, (void*)&result);
	return result;
}

void DBtApplication::SetWindow(LPDISPATCH propVal)
{
	SetProperty(0x8, VT_DISPATCH, propVal);
}

CString DBtApplication::GetSAPIDocDefinitionFile()
{
	CString result;
	GetProperty(0xc, VT_BSTR, (void*)&result);
	return result;
}

void DBtApplication::SetSAPIDocDefinitionFile(LPCTSTR propVal)
{
	SetProperty(0xc, VT_BSTR, propVal);
}

LPDISPATCH DBtApplication::GetLicenseServer()
{
	LPDISPATCH result;
	GetProperty(0xd, VT_DISPATCH, (void*)&result);
	return result;
}

void DBtApplication::SetLicenseServer(LPDISPATCH propVal)
{
	SetProperty(0xd, VT_DISPATCH, propVal);
}

CString DBtApplication::GetEdition()
{
	CString result;
	GetProperty(0x14, VT_BSTR, (void*)&result);
	return result;
}

void DBtApplication::SetEdition(LPCTSTR propVal)
{
	SetProperty(0x14, VT_BSTR, propVal);
}

long DBtApplication::GetVisibleWindows()
{
	long result;
	GetProperty(0x16, VT_I4, (void*)&result);
	return result;
}

void DBtApplication::SetVisibleWindows(long propVal)
{
	SetProperty(0x16, VT_I4, propVal);
}

BOOL DBtApplication::GetTopMost()
{
	BOOL result;
	GetProperty(0x17, VT_BOOL, (void*)&result);
	return result;
}

void DBtApplication::SetTopMost(BOOL propVal)
{
	SetProperty(0x17, VT_BOOL, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtApplication operations

void DBtApplication::Quit(long SaveChanges)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SaveChanges);
}

void DBtApplication::Save(BOOL prompt)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 prompt);
}

void DBtApplication::CommandLine(LPCTSTR CommandLine)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CommandLine);
}

CString DBtApplication::XMLScript(LPCTSTR XMLScript, long SourceType, LPDISPATCH* Messages)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_PDISPATCH;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		XMLScript, SourceType, Messages);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IBtMessages properties

/////////////////////////////////////////////////////////////////////////////
// IBtMessages operations

long IBtMessages::GetCount()
{
	long result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBtMessages::Item(VARIANT* Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

LPDISPATCH IBtMessages::GetMessage(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IBtMessage properties

/////////////////////////////////////////////////////////////////////////////
// IBtMessage operations

long IBtMessage::GetNumber()
{
	long result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long IBtMessage::GetType()
{
	long result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long IBtMessage::GetSeverity()
{
	long result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString IBtMessage::GetMessage()
{
	CString result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBtMessage::GetVerification()
{
	LPDISPATCH result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString IBtMessage::GetTypeText()
{
	CString result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IBtVerification properties

/////////////////////////////////////////////////////////////////////////////
// IBtVerification operations

CString IBtVerification::GetObject()
{
	CString result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IBtVerification::GetFields()
{
	CString result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IBtVerification::GetProblem()
{
	CString result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IBtVerification::GetResult()
{
	CString result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IBtVerification::GetAutoFix()
{
	CString result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// DBtVerification properties

CString DBtVerification::GetObject()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void DBtVerification::SetObject(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

CString DBtVerification::GetFields()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}

void DBtVerification::SetFields(LPCTSTR propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}

CString DBtVerification::GetProblem()
{
	CString result;
	GetProperty(0x3, VT_BSTR, (void*)&result);
	return result;
}

void DBtVerification::SetProblem(LPCTSTR propVal)
{
	SetProperty(0x3, VT_BSTR, propVal);
}

CString DBtVerification::GetResult()
{
	CString result;
	GetProperty(0x4, VT_BSTR, (void*)&result);
	return result;
}

void DBtVerification::SetResult(LPCTSTR propVal)
{
	SetProperty(0x4, VT_BSTR, propVal);
}

CString DBtVerification::GetAutoFix()
{
	CString result;
	GetProperty(0x5, VT_BSTR, (void*)&result);
	return result;
}

void DBtVerification::SetAutoFix(LPCTSTR propVal)
{
	SetProperty(0x5, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtVerification operations


/////////////////////////////////////////////////////////////////////////////
// DBtMessage properties

long DBtMessage::GetNumNumber()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void DBtMessage::SetNumNumber(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

long DBtMessage::GetType()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void DBtMessage::SetType(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

long DBtMessage::GetSeverity()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void DBtMessage::SetSeverity(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

CString DBtMessage::GetMessage()
{
	CString result;
	GetProperty(0x4, VT_BSTR, (void*)&result);
	return result;
}

void DBtMessage::SetMessage(LPCTSTR propVal)
{
	SetProperty(0x4, VT_BSTR, propVal);
}

LPDISPATCH DBtMessage::GetVerification()
{
	LPDISPATCH result;
	GetProperty(0x5, VT_DISPATCH, (void*)&result);
	return result;
}

void DBtMessage::SetVerification(LPDISPATCH propVal)
{
	SetProperty(0x5, VT_DISPATCH, propVal);
}

CString DBtMessage::GetTypeText()
{
	CString result;
	GetProperty(0x6, VT_BSTR, (void*)&result);
	return result;
}

void DBtMessage::SetTypeText(LPCTSTR propVal)
{
	SetProperty(0x6, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtMessage operations


/////////////////////////////////////////////////////////////////////////////
// DBtMessages properties

long DBtMessages::GetCount()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void DBtMessages::SetCount(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtMessages operations

LPDISPATCH DBtMessages::Item(VARIANT* Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

LPDISPATCH DBtMessages::GetMessage(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IBtApplication properties

/////////////////////////////////////////////////////////////////////////////
// IBtApplication operations

LPDISPATCH IBtApplication::GetActiveFormat()
{
	LPDISPATCH result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long IBtApplication::GetBuildNumber()
{
	long result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBtApplication::GetFormats()
{
	LPDISPATCH result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL IBtApplication::GetIsPrinting()
{
	BOOL result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL IBtApplication::GetIsProcessingCommandLines()
{
	BOOL result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString IBtApplication::GetVersion()
{
	CString result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBtApplication::GetWindow()
{
	LPDISPATCH result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void IBtApplication::Quit(long SaveChanges)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SaveChanges);
}

void IBtApplication::Save(BOOL DoPrompt)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 DoPrompt);
}

void IBtApplication::SetSAPIDocDefinitionFile(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtApplication::GetSAPIDocDefinitionFile()
{
	CString result;
	InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtApplication::CommandLine(LPCTSTR CommandLine)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CommandLine);
}

LPDISPATCH IBtApplication::GetLicenseServer()
{
	LPDISPATCH result;
	InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString IBtApplication::XMLScript(LPCTSTR XMLScript, long SourceType, LPDISPATCH* Messages)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_PDISPATCH;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		XMLScript, SourceType, Messages);
	return result;
}

CString IBtApplication::GetEdition()
{
	CString result;
	InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtApplication::SetVisibleWindows(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x16, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtApplication::GetVisibleWindows()
{
	long result;
	InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtApplication::SetTopMost(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtApplication::GetTopMost()
{
	BOOL result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IBtFormat properties

/////////////////////////////////////////////////////////////////////////////
// IBtFormat operations

void IBtFormat::SetAutoPrintAgain(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtFormat::GetAutoPrintAgain()
{
	BOOL result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString IBtFormat::GetBaseName()
{
	CString result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IBtFormat::GetDirectory()
{
	CString result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IBtFormat::GetFileName()
{
	CString result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtFormat::SetIdenticalCopiesOfLabel(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtFormat::GetIdenticalCopiesOfLabel()
{
	long result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBtFormat::GetInputDataFileSetup()
{
	LPDISPATCH result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void IBtFormat::SetNumberSerializedLabels(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtFormat::GetNumberSerializedLabels()
{
	long result;
	InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString IBtFormat::GetPrinter()
{
	CString result;
	InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtFormat::SetPrintToFile(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtFormat::GetPrintToFile()
{
	BOOL result;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString IBtFormat::GetTitle()
{
	CString result;
	InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtFormat::SetUseInputDataFile(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtFormat::GetUseInputDataFile()
{
	BOOL result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IBtFormat::Activate()
{
	InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void IBtFormat::Close(long SaveChanges)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SaveChanges);
}

long IBtFormat::PrintOut(BOOL ShowStatusWindow, BOOL ShowPrintDialog)
{
	long result;
	static BYTE parms[] =
		VTS_BOOL VTS_BOOL;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		ShowStatusWindow, ShowPrintDialog);
	return result;
}

void IBtFormat::SetNamedSubStringValue(LPCTSTR SubStringName, LPCTSTR Value)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SubStringName, Value);
}

CString IBtFormat::GetNamedSubStringValue(LPCTSTR SubStringName)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		SubStringName);
	return result;
}

void IBtFormat::Save()
{
	InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void IBtFormat::SaveAs(LPCTSTR FileName, BOOL OverwriteIfExists)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL;
	InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName, OverwriteIfExists);
}

void IBtFormat::SetSaveAsVersion(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x19, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtFormat::GetSaveAsVersion()
{
	long result;
	InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBtFormat::GetNamedSubStrings()
{
	LPDISPATCH result;
	InvokeHelper(0x1c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void IBtFormat::ExportToClipboard(long Color, long Resolution)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Color, Resolution);
}

void IBtFormat::ExportToFile(LPCTSTR FileName, LPCTSTR FileType, long Color, long Resolution, long SaveOption)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName, FileType, Color, Resolution, SaveOption);
}

LPDISPATCH IBtFormat::GetDatabases()
{
	LPDISPATCH result;
	InvokeHelper(0x1d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void IBtFormat::SetEnablePrompting(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x1e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtFormat::GetEnablePrompting()
{
	BOOL result;
	InvokeHelper(0x1e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IBtFormat::SetRecordRange(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtFormat::GetRecordRange()
{
	CString result;
	InvokeHelper(0x1f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtFormat::SetSelectRecordsAtPrint(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x20, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtFormat::GetSelectRecordsAtPrint()
{
	BOOL result;
	InvokeHelper(0x20, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IBtFormat::SetUseDatabase(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x21, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtFormat::GetUseDatabase()
{
	BOOL result;
	InvokeHelper(0x21, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IBtFormat::SetPrinter(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x10, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void IBtFormat::SetPrinterFile(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x22, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtFormat::GetPrinterFile()
{
	CString result;
	InvokeHelper(0x22, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBtFormat::GetPageSetup()
{
	LPDISPATCH result;
	InvokeHelper(0x24, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBtFormat::GetPrintSetup()
{
	LPDISPATCH result;
	InvokeHelper(0x26, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBtFormat::GetPrinterCodeTemplate()
{
	LPDISPATCH result;
	InvokeHelper(0x27, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString IBtFormat::GetUsedNames(long FieldType, LPCTSTR delimiter)
{
	CString result;
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x28, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms,
		FieldType, delimiter);
	return result;
}

void IBtFormat::SetAvailableTemplateFieldNames(LPCTSTR FieldnameSampleDataList, LPCTSTR delimiter)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x2a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FieldnameSampleDataList, delimiter);
}

CString IBtFormat::ExportPromptToXML(LPCTSTR XSLFileName)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2c, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		XSLFileName);
	return result;
}

void IBtFormat::SetPrompt(LPCTSTR PromptName, LPCTSTR PromptData)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x2d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 PromptName, PromptData);
}

void IBtFormat::SetJobName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtFormat::GetJobName()
{
	CString result;
	InvokeHelper(0x2e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtFormat::SetPrintToFileLicense(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtFormat::GetPrintToFileLicense()
{
	CString result;
	InvokeHelper(0x2f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long IBtFormat::Print(LPCTSTR PrintJobName, BOOL WaitForSpoolJobToComplete, long TimeoutMs, LPDISPATCH* Messages)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL VTS_I4 VTS_PDISPATCH;
	InvokeHelper(0x30, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		PrintJobName, WaitForSpoolJobToComplete, TimeoutMs, Messages);
	return result;
}

void IBtFormat::SetPaperTray(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x32, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtFormat::GetPaperTray()
{
	CString result;
	InvokeHelper(0x32, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtFormat::SetComment(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x33, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtFormat::GetComment()
{
	CString result;
	InvokeHelper(0x33, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long IBtFormat::ExportPrintPreviewToImage(LPCTSTR Directory, LPCTSTR FileNameTemplate, LPCTSTR FileType, long Colors, long dpi, long backgroundColor, long SaveOpts, BOOL IncludeMargins, BOOL IncludeBorder, LPDISPATCH* Messages)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BOOL VTS_BOOL VTS_PDISPATCH;
	InvokeHelper(0x35, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Directory, FileNameTemplate, FileType, Colors, dpi, backgroundColor, SaveOpts, IncludeMargins, IncludeBorder, Messages);
	return result;
}

void IBtFormat::SetEncryption(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x36, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtFormat::GetEncryption()
{
	long result;
	InvokeHelper(0x36, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL IBtFormat::GetSupportsSetSerializedCopies()
{
	BOOL result;
	InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL IBtFormat::GetSupportsSetIdenticalCopies()
{
	BOOL result;
	InvokeHelper(0x38, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IBtInputDataFileSetup properties

/////////////////////////////////////////////////////////////////////////////
// IBtInputDataFileSetup operations

void IBtInputDataFileSetup::SetAutoViewSelectedRecordsBeforePrint(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtInputDataFileSetup::GetAutoViewSelectedRecordsBeforePrint()
{
	BOOL result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBtInputDataFileSetup::GetTextFile()
{
	LPDISPATCH result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void IBtInputDataFileSetup::SetType(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtInputDataFileSetup::GetType()
{
	long result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBtInputDataFileSetup::GetOdbc()
{
	LPDISPATCH result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBtInputDataFileSetup::GetOledb()
{
	LPDISPATCH result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IBtTextFile properties

/////////////////////////////////////////////////////////////////////////////
// IBtTextFile operations

void IBtTextFile::SetFileName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtTextFile::GetFileName()
{
	CString result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtTextFile::SetDelimitation(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtTextFile::GetDelimitation()
{
	long result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtTextFile::SetFieldDelimiter(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtTextFile::GetFieldDelimiter()
{
	CString result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtTextFile::SetNumberOfFields(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtTextFile::GetNumberOfFields()
{
	long result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtTextFile::SetUseFieldNamesFromFirstRecord(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtTextFile::GetUseFieldNamesFromFirstRecord()
{
	BOOL result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// DBtTextFile properties

CString DBtTextFile::GetFileName()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}

void DBtTextFile::SetFileName(LPCTSTR propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}

long DBtTextFile::GetDelimitation()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void DBtTextFile::SetDelimitation(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

CString DBtTextFile::GetFieldDelimiter()
{
	CString result;
	GetProperty(0x3, VT_BSTR, (void*)&result);
	return result;
}

void DBtTextFile::SetFieldDelimiter(LPCTSTR propVal)
{
	SetProperty(0x3, VT_BSTR, propVal);
}

long DBtTextFile::GetNumberOfFields()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}

void DBtTextFile::SetNumberOfFields(long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}

BOOL DBtTextFile::GetUseFieldNamesFromFirstRecord()
{
	BOOL result;
	GetProperty(0x5, VT_BOOL, (void*)&result);
	return result;
}

void DBtTextFile::SetUseFieldNamesFromFirstRecord(BOOL propVal)
{
	SetProperty(0x5, VT_BOOL, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtTextFile operations


/////////////////////////////////////////////////////////////////////////////
// IBtODBC properties

/////////////////////////////////////////////////////////////////////////////
// IBtODBC operations

void IBtODBC::SetWhere(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtODBC::GetWhere()
{
	CString result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtODBC::SetPassword(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtODBC::GetPassword()
{
	CString result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtODBC::SetUserId(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtODBC::GetUserId()
{
	CString result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtODBC::SetSQLStatement(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtODBC::GetSQLStatement()
{
	CString result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// DBtODBC properties

CString DBtODBC::GetWhere()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void DBtODBC::SetWhere(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

CString DBtODBC::GetPassword()
{
	CString result;
	GetProperty(0x3, VT_BSTR, (void*)&result);
	return result;
}

void DBtODBC::SetPassword(LPCTSTR propVal)
{
	SetProperty(0x3, VT_BSTR, propVal);
}

CString DBtODBC::GetUserId()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}

void DBtODBC::SetUserId(LPCTSTR propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}

CString DBtODBC::GetSQLStatement()
{
	CString result;
	GetProperty(0x4, VT_BSTR, (void*)&result);
	return result;
}

void DBtODBC::SetSQLStatement(LPCTSTR propVal)
{
	SetProperty(0x4, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtODBC operations


/////////////////////////////////////////////////////////////////////////////
// IBtOLEDB properties

/////////////////////////////////////////////////////////////////////////////
// IBtOLEDB operations

void IBtOLEDB::SetPassword(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtOLEDB::GetPassword()
{
	CString result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtOLEDB::SetUserId(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtOLEDB::GetUserId()
{
	CString result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtOLEDB::SetSQLStatement(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtOLEDB::GetSQLStatement()
{
	CString result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// DBtOLEDB properties

CString DBtOLEDB::GetPassword()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}

void DBtOLEDB::SetPassword(LPCTSTR propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}

CString DBtOLEDB::GetUserId()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void DBtOLEDB::SetUserId(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

CString DBtOLEDB::GetSQLStatement()
{
	CString result;
	GetProperty(0x3, VT_BSTR, (void*)&result);
	return result;
}

void DBtOLEDB::SetSQLStatement(LPCTSTR propVal)
{
	SetProperty(0x3, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtOLEDB operations


/////////////////////////////////////////////////////////////////////////////
// DBtInputDataFileSetup properties

BOOL DBtInputDataFileSetup::GetAutoViewSelectedRecordsBeforePrint()
{
	BOOL result;
	GetProperty(0x4, VT_BOOL, (void*)&result);
	return result;
}

void DBtInputDataFileSetup::SetAutoViewSelectedRecordsBeforePrint(BOOL propVal)
{
	SetProperty(0x4, VT_BOOL, propVal);
}

LPDISPATCH DBtInputDataFileSetup::GetTextFile()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void DBtInputDataFileSetup::SetTextFile(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

long DBtInputDataFileSetup::GetType()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void DBtInputDataFileSetup::SetType(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

LPDISPATCH DBtInputDataFileSetup::GetOdbc()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void DBtInputDataFileSetup::SetOdbc(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

LPDISPATCH DBtInputDataFileSetup::GetOledb()
{
	LPDISPATCH result;
	GetProperty(0x5, VT_DISPATCH, (void*)&result);
	return result;
}

void DBtInputDataFileSetup::SetOledb(LPDISPATCH propVal)
{
	SetProperty(0x5, VT_DISPATCH, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtInputDataFileSetup operations


/////////////////////////////////////////////////////////////////////////////
// IBtNamedSubStrings properties

/////////////////////////////////////////////////////////////////////////////
// IBtNamedSubStrings operations

long IBtNamedSubStrings::GetCount()
{
	long result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBtNamedSubStrings::Item(VARIANT* Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

CString IBtNamedSubStrings::GetAll(LPCTSTR nameValueDelimiter, LPCTSTR recordDelimter)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		nameValueDelimiter, recordDelimter);
	return result;
}

void IBtNamedSubStrings::SetAll(LPCTSTR setStr, LPCTSTR delimter)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 setStr, delimter);
}

LPDISPATCH IBtNamedSubStrings::GetSubString(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IBtSubString properties

/////////////////////////////////////////////////////////////////////////////
// IBtSubString operations

CString IBtSubString::GetName()
{
	CString result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IBtSubString::GetValue()
{
	CString result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtSubString::SetValue(BSTR* newValue)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long IBtSubString::GetType()
{
	long result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// DBtSubString properties

CString DBtSubString::GetName()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void DBtSubString::SetName(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

CString DBtSubString::GetValue()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}

void DBtSubString::SetValue(LPCTSTR propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}

long DBtSubString::GetType()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void DBtSubString::SetType(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtSubString operations


/////////////////////////////////////////////////////////////////////////////
// DBtNamedSubStrings properties

long DBtNamedSubStrings::GetCount()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void DBtNamedSubStrings::SetCount(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtNamedSubStrings operations

LPDISPATCH DBtNamedSubStrings::Item(VARIANT* Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

CString DBtNamedSubStrings::GetAll(LPCTSTR nameValueDelimiter, LPCTSTR recordDelimiter)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		nameValueDelimiter, recordDelimiter);
	return result;
}

void DBtNamedSubStrings::SetAll(LPCTSTR subStringsAndValues, LPCTSTR delimiter)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 subStringsAndValues, delimiter);
}

LPDISPATCH DBtNamedSubStrings::GetSubString(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IBtDatabases properties

/////////////////////////////////////////////////////////////////////////////
// IBtDatabases operations

long IBtDatabases::GetCount()
{
	long result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBtDatabases::GetQueryPrompts()
{
	LPDISPATCH result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBtDatabases::Item(VARIANT* Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

LPDISPATCH IBtDatabases::GetDatabase(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IBtQueryPrompts properties

/////////////////////////////////////////////////////////////////////////////
// IBtQueryPrompts operations

long IBtQueryPrompts::GetCount()
{
	long result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBtQueryPrompts::Item(VARIANT* Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

LPDISPATCH IBtQueryPrompts::GetQueryPrompt(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IBtQueryPrompt properties

/////////////////////////////////////////////////////////////////////////////
// IBtQueryPrompt operations

void IBtQueryPrompt::SetDefaultReply(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtQueryPrompt::GetDefaultReply()
{
	CString result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IBtQueryPrompt::GetName()
{
	CString result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtQueryPrompt::SetUserPrompt(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtQueryPrompt::GetUserPrompt()
{
	CString result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtQueryPrompt::SetValue(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtQueryPrompt::GetValue()
{
	CString result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// DBtQueryPrompt properties

CString DBtQueryPrompt::GetDefaultReply()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void DBtQueryPrompt::SetDefaultReply(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

CString DBtQueryPrompt::GetName()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}

void DBtQueryPrompt::SetName(LPCTSTR propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}

CString DBtQueryPrompt::GetUserPrompt()
{
	CString result;
	GetProperty(0x3, VT_BSTR, (void*)&result);
	return result;
}

void DBtQueryPrompt::SetUserPrompt(LPCTSTR propVal)
{
	SetProperty(0x3, VT_BSTR, propVal);
}

CString DBtQueryPrompt::GetValue()
{
	CString result;
	GetProperty(0x4, VT_BSTR, (void*)&result);
	return result;
}

void DBtQueryPrompt::SetValue(LPCTSTR propVal)
{
	SetProperty(0x4, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtQueryPrompt operations


/////////////////////////////////////////////////////////////////////////////
// DBtQueryPrompts properties

long DBtQueryPrompts::GetCount()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void DBtQueryPrompts::SetCount(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtQueryPrompts operations

LPDISPATCH DBtQueryPrompts::Item(VARIANT* Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

LPDISPATCH DBtQueryPrompts::GetQueryPrompt(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IBtDatabase properties

/////////////////////////////////////////////////////////////////////////////
// IBtDatabase operations

void IBtDatabase::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtDatabase::GetName()
{
	CString result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBtDatabase::GetOdbc()
{
	LPDISPATCH result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBtDatabase::GetTextFile()
{
	LPDISPATCH result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long IBtDatabase::GetType()
{
	long result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBtDatabase::GetSAPIDoc()
{
	LPDISPATCH result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBtDatabase::GetOledb()
{
	LPDISPATCH result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IBtSAPIDoc properties

/////////////////////////////////////////////////////////////////////////////
// IBtSAPIDoc operations

void IBtSAPIDoc::SetIDocFileName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtSAPIDoc::GetIDocFileName()
{
	CString result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// DBtSAPIDoc properties

CString DBtSAPIDoc::GetIDocFileName()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void DBtSAPIDoc::SetIDocFileName(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtSAPIDoc operations


/////////////////////////////////////////////////////////////////////////////
// DBtDatabase properties

CString DBtDatabase::GetName()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void DBtDatabase::SetName(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

LPDISPATCH DBtDatabase::GetOdbc()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void DBtDatabase::SetOdbc(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

LPDISPATCH DBtDatabase::GetTextFile()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void DBtDatabase::SetTextFile(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

long DBtDatabase::GetType()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}

void DBtDatabase::SetType(long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}

LPDISPATCH DBtDatabase::GetSAPIDoc()
{
	LPDISPATCH result;
	GetProperty(0x5, VT_DISPATCH, (void*)&result);
	return result;
}

void DBtDatabase::SetSAPIDoc(LPDISPATCH propVal)
{
	SetProperty(0x5, VT_DISPATCH, propVal);
}

LPDISPATCH DBtDatabase::GetOledb()
{
	LPDISPATCH result;
	GetProperty(0x6, VT_DISPATCH, (void*)&result);
	return result;
}

void DBtDatabase::SetOledb(LPDISPATCH propVal)
{
	SetProperty(0x6, VT_DISPATCH, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtDatabase operations


/////////////////////////////////////////////////////////////////////////////
// DBtDatabases properties

long DBtDatabases::GetCount()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void DBtDatabases::SetCount(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

LPDISPATCH DBtDatabases::GetQueryPrompts()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void DBtDatabases::SetQueryPrompts(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtDatabases operations

LPDISPATCH DBtDatabases::Item(VARIANT* Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

LPDISPATCH DBtDatabases::GetDatabase(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IBtPageSetup properties

/////////////////////////////////////////////////////////////////////////////
// IBtPageSetup operations

CString IBtPageSetup::GetStockCategory()
{
	CString result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IBtPageSetup::GetStockType()
{
	CString result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtPageSetup::SetPaperName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtPageSetup::GetPaperName()
{
	CString result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtPageSetup::SetPaperID(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtPageSetup::GetPaperID()
{
	long result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtPageSetup::SetOrientation(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtPageSetup::GetOrientation()
{
	long result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtPageSetup::SetLabelColumns(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtPageSetup::GetLabelColumns()
{
	long result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtPageSetup::SetLabelRows(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtPageSetup::GetLabelRows()
{
	long result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtPageSetup::SetLabelSizeManual(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtPageSetup::GetLabelSizeManual()
{
	BOOL result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IBtPageSetup::SetLabelShape(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtPageSetup::GetLabelShape()
{
	long result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtPageSetup::SetLabelCorners(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtPageSetup::GetLabelCorners()
{
	long result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtPageSetup::SetLabelHole(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtPageSetup::GetLabelHole()
{
	BOOL result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IBtPageSetup::SetPaperWidth(long units, float newValue)
{
	static BYTE parms[] =
		VTS_I4 VTS_R4;
	InvokeHelper(0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 units, newValue);
}

float IBtPageSetup::GetPaperWidth(long units)
{
	float result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, parms,
		units);
	return result;
}

void IBtPageSetup::SetPaperHeight(long units, float newValue)
{
	static BYTE parms[] =
		VTS_I4 VTS_R4;
	InvokeHelper(0xd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 units, newValue);
}

float IBtPageSetup::GetPaperHeight(long units)
{
	float result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, parms,
		units);
	return result;
}

void IBtPageSetup::SetMarginLeft(long units, float newValue)
{
	static BYTE parms[] =
		VTS_I4 VTS_R4;
	InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 units, newValue);
}

float IBtPageSetup::GetMarginLeft(long units)
{
	float result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, parms,
		units);
	return result;
}

void IBtPageSetup::SetMarginRight(long units, float newValue)
{
	static BYTE parms[] =
		VTS_I4 VTS_R4;
	InvokeHelper(0xf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 units, newValue);
}

float IBtPageSetup::GetMarginRight(long units)
{
	float result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, parms,
		units);
	return result;
}

void IBtPageSetup::SetMarginTop(long units, float newValue)
{
	static BYTE parms[] =
		VTS_I4 VTS_R4;
	InvokeHelper(0x10, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 units, newValue);
}

float IBtPageSetup::GetMarginTop(long units)
{
	float result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, parms,
		units);
	return result;
}

void IBtPageSetup::SetMarginBottom(long units, float newValue)
{
	static BYTE parms[] =
		VTS_I4 VTS_R4;
	InvokeHelper(0x11, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 units, newValue);
}

float IBtPageSetup::GetMarginBottom(long units)
{
	float result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, parms,
		units);
	return result;
}

void IBtPageSetup::SetLabelWidth(long units, float newValue)
{
	static BYTE parms[] =
		VTS_I4 VTS_R4;
	InvokeHelper(0x12, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 units, newValue);
}

float IBtPageSetup::GetLabelWidth(long units)
{
	float result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, parms,
		units);
	return result;
}

void IBtPageSetup::SetLabelHeight(long units, float newValue)
{
	static BYTE parms[] =
		VTS_I4 VTS_R4;
	InvokeHelper(0x13, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 units, newValue);
}

float IBtPageSetup::GetLabelHeight(long units)
{
	float result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, parms,
		units);
	return result;
}

void IBtPageSetup::SetLabelHoleDiameter(long units, float newValue)
{
	static BYTE parms[] =
		VTS_I4 VTS_R4;
	InvokeHelper(0x14, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 units, newValue);
}

float IBtPageSetup::GetLabelHoleDiameter(long units)
{
	float result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, parms,
		units);
	return result;
}

BOOL IBtPageSetup::GetValid()
{
	BOOL result;
	InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IBtPageSetup::SetStock(LPCTSTR StockCategory, LPCTSTR StockType)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x17, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 StockCategory, StockType);
}

void IBtPageSetup::SetLabelGapManual(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x18, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtPageSetup::GetLabelGapManual()
{
	BOOL result;
	InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IBtPageSetup::SetLabelGapHorizontal(long units, float newValue)
{
	static BYTE parms[] =
		VTS_I4 VTS_R4;
	InvokeHelper(0x19, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 units, newValue);
}

float IBtPageSetup::GetLabelGapHorizontal(long units)
{
	float result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, parms,
		units);
	return result;
}

void IBtPageSetup::SetLabelGapVertical(long units, float newValue)
{
	static BYTE parms[] =
		VTS_I4 VTS_R4;
	InvokeHelper(0x1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 units, newValue);
}

float IBtPageSetup::GetLabelGapVertical(long units)
{
	float result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, parms,
		units);
	return result;
}

void IBtPageSetup::SetMirror(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x1b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtPageSetup::GetMirror()
{
	BOOL result;
	InvokeHelper(0x1b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IBtPageSetup::SetInverse(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x1c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtPageSetup::GetInverse()
{
	BOOL result;
	InvokeHelper(0x1c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString IBtPageSetup::GetTray()
{
	CString result;
	InvokeHelper(0x1d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtPageSetup::SetTray(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH IBtPageSetup::GetMediaHandling()
{
	LPDISPATCH result;
	InvokeHelper(0x1e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IBtMediaHandling properties

/////////////////////////////////////////////////////////////////////////////
// IBtMediaHandling operations

void IBtMediaHandling::SetAction(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtMediaHandling::GetAction()
{
	long result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtMediaHandling::SetOccurrence(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtMediaHandling::GetOccurrence()
{
	long result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtMediaHandling::SetDataItemType(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtMediaHandling::GetDataItemType()
{
	long result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtMediaHandling::SetDataItemName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtMediaHandling::GetDataItemName()
{
	CString result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtMediaHandling::SetNumberOfLabels(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtMediaHandling::GetNumberOfLabels()
{
	long result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtMediaHandling::SetIgnoreCase(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtMediaHandling::GetIgnoreCase()
{
	BOOL result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// DBtMediaHandling properties

long DBtMediaHandling::GetAction()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void DBtMediaHandling::SetAction(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

long DBtMediaHandling::GetOccurrence()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void DBtMediaHandling::SetOccurrence(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

long DBtMediaHandling::GetDataItemType()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void DBtMediaHandling::SetDataItemType(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

CString DBtMediaHandling::GetDataItemName()
{
	CString result;
	GetProperty(0x4, VT_BSTR, (void*)&result);
	return result;
}

void DBtMediaHandling::SetDataItemName(LPCTSTR propVal)
{
	SetProperty(0x4, VT_BSTR, propVal);
}

long DBtMediaHandling::GetNumberOfLabels()
{
	long result;
	GetProperty(0x5, VT_I4, (void*)&result);
	return result;
}

void DBtMediaHandling::SetNumberOfLabels(long propVal)
{
	SetProperty(0x5, VT_I4, propVal);
}

BOOL DBtMediaHandling::GetIgnoreCase()
{
	BOOL result;
	GetProperty(0x6, VT_BOOL, (void*)&result);
	return result;
}

void DBtMediaHandling::SetIgnoreCase(BOOL propVal)
{
	SetProperty(0x6, VT_BOOL, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtMediaHandling operations


/////////////////////////////////////////////////////////////////////////////
// DBtPageSetup properties

CString DBtPageSetup::GetStockCategory()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void DBtPageSetup::SetStockCategory(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

CString DBtPageSetup::GetStockType()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}

void DBtPageSetup::SetStockType(LPCTSTR propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}

CString DBtPageSetup::GetPaperName()
{
	CString result;
	GetProperty(0x3, VT_BSTR, (void*)&result);
	return result;
}

void DBtPageSetup::SetPaperName(LPCTSTR propVal)
{
	SetProperty(0x3, VT_BSTR, propVal);
}

long DBtPageSetup::GetPaperID()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}

void DBtPageSetup::SetPaperID(long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}

long DBtPageSetup::GetOrientation()
{
	long result;
	GetProperty(0x5, VT_I4, (void*)&result);
	return result;
}

void DBtPageSetup::SetOrientation(long propVal)
{
	SetProperty(0x5, VT_I4, propVal);
}

long DBtPageSetup::GetLabelColumns()
{
	long result;
	GetProperty(0x6, VT_I4, (void*)&result);
	return result;
}

void DBtPageSetup::SetLabelColumns(long propVal)
{
	SetProperty(0x6, VT_I4, propVal);
}

long DBtPageSetup::GetLabelRows()
{
	long result;
	GetProperty(0x7, VT_I4, (void*)&result);
	return result;
}

void DBtPageSetup::SetLabelRows(long propVal)
{
	SetProperty(0x7, VT_I4, propVal);
}

BOOL DBtPageSetup::GetLabelSizeManual()
{
	BOOL result;
	GetProperty(0x8, VT_BOOL, (void*)&result);
	return result;
}

void DBtPageSetup::SetLabelSizeManual(BOOL propVal)
{
	SetProperty(0x8, VT_BOOL, propVal);
}

long DBtPageSetup::GetLabelShape()
{
	long result;
	GetProperty(0x9, VT_I4, (void*)&result);
	return result;
}

void DBtPageSetup::SetLabelShape(long propVal)
{
	SetProperty(0x9, VT_I4, propVal);
}

long DBtPageSetup::GetLabelCorners()
{
	long result;
	GetProperty(0xa, VT_I4, (void*)&result);
	return result;
}

void DBtPageSetup::SetLabelCorners(long propVal)
{
	SetProperty(0xa, VT_I4, propVal);
}

BOOL DBtPageSetup::GetLabelHole()
{
	BOOL result;
	GetProperty(0xb, VT_BOOL, (void*)&result);
	return result;
}

void DBtPageSetup::SetLabelHole(BOOL propVal)
{
	SetProperty(0xb, VT_BOOL, propVal);
}

BOOL DBtPageSetup::GetValid()
{
	BOOL result;
	GetProperty(0x15, VT_BOOL, (void*)&result);
	return result;
}

void DBtPageSetup::SetValid(BOOL propVal)
{
	SetProperty(0x15, VT_BOOL, propVal);
}

BOOL DBtPageSetup::GetLabelGapManual()
{
	BOOL result;
	GetProperty(0x18, VT_BOOL, (void*)&result);
	return result;
}

void DBtPageSetup::SetLabelGapManual(BOOL propVal)
{
	SetProperty(0x18, VT_BOOL, propVal);
}

BOOL DBtPageSetup::GetMirror()
{
	BOOL result;
	GetProperty(0x1b, VT_BOOL, (void*)&result);
	return result;
}

void DBtPageSetup::SetMirror(BOOL propVal)
{
	SetProperty(0x1b, VT_BOOL, propVal);
}

BOOL DBtPageSetup::GetInverse()
{
	BOOL result;
	GetProperty(0x1c, VT_BOOL, (void*)&result);
	return result;
}

void DBtPageSetup::SetInverse(BOOL propVal)
{
	SetProperty(0x1c, VT_BOOL, propVal);
}

CString DBtPageSetup::GetTray()
{
	CString result;
	GetProperty(0x1d, VT_BSTR, (void*)&result);
	return result;
}

void DBtPageSetup::SetTray(LPCTSTR propVal)
{
	SetProperty(0x1d, VT_BSTR, propVal);
}

LPDISPATCH DBtPageSetup::GetMediaHandling()
{
	LPDISPATCH result;
	GetProperty(0x1e, VT_DISPATCH, (void*)&result);
	return result;
}

void DBtPageSetup::SetMediaHandling(LPDISPATCH propVal)
{
	SetProperty(0x1e, VT_DISPATCH, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtPageSetup operations

float DBtPageSetup::GetPaperWidth(long units)
{
	float result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, parms,
		units);
	return result;
}

float DBtPageSetup::GetPaperHeight(long units)
{
	float result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, parms,
		units);
	return result;
}

float DBtPageSetup::GetMarginLeft(long units)
{
	float result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, parms,
		units);
	return result;
}

float DBtPageSetup::GetMarginRight(long units)
{
	float result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, parms,
		units);
	return result;
}

float DBtPageSetup::GetMarginTop(long units)
{
	float result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, parms,
		units);
	return result;
}

float DBtPageSetup::GetMarginBottom(long units)
{
	float result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, parms,
		units);
	return result;
}

float DBtPageSetup::GetLabelWidth(long units)
{
	float result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, parms,
		units);
	return result;
}

float DBtPageSetup::GetLabelHeight(long units)
{
	float result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, parms,
		units);
	return result;
}

float DBtPageSetup::GetLabelHoleDiameter(long units)
{
	float result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, parms,
		units);
	return result;
}

float DBtPageSetup::GetLabelGapHorizontal(long units)
{
	float result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, parms,
		units);
	return result;
}

float DBtPageSetup::GetLabelGapVertical(long units)
{
	float result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, parms,
		units);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IBtPrintSetup properties

/////////////////////////////////////////////////////////////////////////////
// IBtPrintSetup operations

LPDISPATCH IBtPrintSetup::GetPerformance()
{
	LPDISPATCH result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBtPrintSetup::GetCache()
{
	LPDISPATCH result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBtPrintSetup::GetPrintMethod()
{
	LPDISPATCH result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void IBtPrintSetup::SetAutoPrintAgain(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtPrintSetup::GetAutoPrintAgain()
{
	BOOL result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IBtPrintSetup::SetIdenticalCopiesOfLabel(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtPrintSetup::GetIdenticalCopiesOfLabel()
{
	long result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtPrintSetup::SetNumberSerializedLabels(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtPrintSetup::GetNumberSerializedLabels()
{
	long result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtPrintSetup::SetRefreshDatabases(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtPrintSetup::GetRefreshDatabases()
{
	BOOL result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IBtPrintSetup::SetLogPrintJob(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtPrintSetup::GetLogPrintJob()
{
	BOOL result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString IBtPrintSetup::GetPrinter()
{
	CString result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtPrintSetup::SetPrintToFile(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtPrintSetup::GetPrintToFile()
{
	BOOL result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IBtPrintSetup::SetUseDatabase(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtPrintSetup::GetUseDatabase()
{
	BOOL result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IBtPrintSetup::SetPrinter(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// IBtPerformance properties

/////////////////////////////////////////////////////////////////////////////
// IBtPerformance operations

void IBtPerformance::SetAllowStaticGraphics(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtPerformance::GetAllowStaticGraphics()
{
	BOOL result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IBtPerformance::SetAllowStaticObjects(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtPerformance::GetAllowStaticObjects()
{
	BOOL result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IBtPerformance::SetAllowSerialization(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtPerformance::GetAllowSerialization()
{
	BOOL result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IBtPerformance::SetAllowVariableDataOptimization(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtPerformance::GetAllowVariableDataOptimization()
{
	BOOL result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IBtPerformance::SetAllowGraphicsCaching(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtPerformance::GetAllowGraphicsCaching()
{
	BOOL result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IBtPerformance::SetAllowFormatCaching(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtPerformance::GetAllowFormatCaching()
{
	BOOL result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IBtPerformance::SetWarnWhenUsingTrueTypeFonts(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IBtPerformance::GetWarnWhenUsingTrueTypeFonts()
{
	BOOL result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// DBtPerformance properties

BOOL DBtPerformance::GetAllowStaticGraphics()
{
	BOOL result;
	GetProperty(0x1, VT_BOOL, (void*)&result);
	return result;
}

void DBtPerformance::SetAllowStaticGraphics(BOOL propVal)
{
	SetProperty(0x1, VT_BOOL, propVal);
}

BOOL DBtPerformance::GetAllowStaticObjects()
{
	BOOL result;
	GetProperty(0x2, VT_BOOL, (void*)&result);
	return result;
}

void DBtPerformance::SetAllowStaticObjects(BOOL propVal)
{
	SetProperty(0x2, VT_BOOL, propVal);
}

BOOL DBtPerformance::GetAllowSerialization()
{
	BOOL result;
	GetProperty(0x3, VT_BOOL, (void*)&result);
	return result;
}

void DBtPerformance::SetAllowSerialization(BOOL propVal)
{
	SetProperty(0x3, VT_BOOL, propVal);
}

BOOL DBtPerformance::GetAllowVariableDataOptimization()
{
	BOOL result;
	GetProperty(0x4, VT_BOOL, (void*)&result);
	return result;
}

void DBtPerformance::SetAllowVariableDataOptimization(BOOL propVal)
{
	SetProperty(0x4, VT_BOOL, propVal);
}

BOOL DBtPerformance::GetAllowGraphicsCaching()
{
	BOOL result;
	GetProperty(0x5, VT_BOOL, (void*)&result);
	return result;
}

void DBtPerformance::SetAllowGraphicsCaching(BOOL propVal)
{
	SetProperty(0x5, VT_BOOL, propVal);
}

BOOL DBtPerformance::GetAllowFormatCaching()
{
	BOOL result;
	GetProperty(0x6, VT_BOOL, (void*)&result);
	return result;
}

void DBtPerformance::SetAllowFormatCaching(BOOL propVal)
{
	SetProperty(0x6, VT_BOOL, propVal);
}

BOOL DBtPerformance::GetWarnWhenUsingTrueTypeFonts()
{
	BOOL result;
	GetProperty(0x7, VT_BOOL, (void*)&result);
	return result;
}

void DBtPerformance::SetWarnWhenUsingTrueTypeFonts(BOOL propVal)
{
	SetProperty(0x7, VT_BOOL, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtPerformance operations


/////////////////////////////////////////////////////////////////////////////
// IBtCache properties

/////////////////////////////////////////////////////////////////////////////
// IBtCache operations

void IBtCache::SetFlushInterval(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtCache::GetFlushInterval()
{
	long result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtCache::Flush()
{
	InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// DBtCache properties

long DBtCache::GetFlushInterval()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void DBtCache::SetFlushInterval(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtCache operations

void DBtCache::Flush()
{
	InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// IBtPrintMethod properties

/////////////////////////////////////////////////////////////////////////////
// IBtPrintMethod operations

void IBtPrintMethod::SetTrueTypeText(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtPrintMethod::GetTrueTypeText()
{
	long result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtPrintMethod::SetUnsupported1DBarCodes(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtPrintMethod::GetUnsupported1DBarCodes()
{
	long result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtPrintMethod::SetUnsupported2DBarCodes(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtPrintMethod::GetUnsupported2DBarCodes()
{
	long result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtPrintMethod::SetLines(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtPrintMethod::GetLines()
{
	long result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtPrintMethod::SetBoxes(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtPrintMethod::GetBoxes()
{
	long result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtPrintMethod::SetEllipses(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtPrintMethod::GetEllipses()
{
	long result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// DBtPrintMethod properties

long DBtPrintMethod::GetTrueTypeText()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void DBtPrintMethod::SetTrueTypeText(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

long DBtPrintMethod::GetUnsupported1DBarCodes()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void DBtPrintMethod::SetUnsupported1DBarCodes(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

long DBtPrintMethod::GetUnsupported2DBarCodes()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void DBtPrintMethod::SetUnsupported2DBarCodes(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

long DBtPrintMethod::GetLines()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}

void DBtPrintMethod::SetLines(long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}

long DBtPrintMethod::GetBoxes()
{
	long result;
	GetProperty(0x5, VT_I4, (void*)&result);
	return result;
}

void DBtPrintMethod::SetBoxes(long propVal)
{
	SetProperty(0x5, VT_I4, propVal);
}

long DBtPrintMethod::GetEllipses()
{
	long result;
	GetProperty(0x6, VT_I4, (void*)&result);
	return result;
}

void DBtPrintMethod::SetEllipses(long propVal)
{
	SetProperty(0x6, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtPrintMethod operations


/////////////////////////////////////////////////////////////////////////////
// DBtPrintSetup properties

LPDISPATCH DBtPrintSetup::GetPerformance()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

void DBtPrintSetup::SetPerformance(LPDISPATCH propVal)
{
	SetProperty(0x1, VT_DISPATCH, propVal);
}

LPDISPATCH DBtPrintSetup::GetCache()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void DBtPrintSetup::SetCache(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

LPDISPATCH DBtPrintSetup::GetPrintMethod()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void DBtPrintSetup::SetPrintMethod(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

BOOL DBtPrintSetup::GetAutoPrintAgain()
{
	BOOL result;
	GetProperty(0x4, VT_BOOL, (void*)&result);
	return result;
}

void DBtPrintSetup::SetAutoPrintAgain(BOOL propVal)
{
	SetProperty(0x4, VT_BOOL, propVal);
}

long DBtPrintSetup::GetIdenticalCopiesOfLabel()
{
	long result;
	GetProperty(0x5, VT_I4, (void*)&result);
	return result;
}

void DBtPrintSetup::SetIdenticalCopiesOfLabel(long propVal)
{
	SetProperty(0x5, VT_I4, propVal);
}

long DBtPrintSetup::GetNumberSerializedLabels()
{
	long result;
	GetProperty(0x6, VT_I4, (void*)&result);
	return result;
}

void DBtPrintSetup::SetNumberSerializedLabels(long propVal)
{
	SetProperty(0x6, VT_I4, propVal);
}

CString DBtPrintSetup::GetPrinter()
{
	CString result;
	GetProperty(0x9, VT_BSTR, (void*)&result);
	return result;
}

void DBtPrintSetup::SetPrinter(LPCTSTR propVal)
{
	SetProperty(0x9, VT_BSTR, propVal);
}

BOOL DBtPrintSetup::GetPrintToFile()
{
	BOOL result;
	GetProperty(0xa, VT_BOOL, (void*)&result);
	return result;
}

void DBtPrintSetup::SetPrintToFile(BOOL propVal)
{
	SetProperty(0xa, VT_BOOL, propVal);
}

BOOL DBtPrintSetup::GetUseDatabase()
{
	BOOL result;
	GetProperty(0xb, VT_BOOL, (void*)&result);
	return result;
}

void DBtPrintSetup::SetUseDatabase(BOOL propVal)
{
	SetProperty(0xb, VT_BOOL, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtPrintSetup operations


/////////////////////////////////////////////////////////////////////////////
// IBtPrinterCodeTemplate properties

/////////////////////////////////////////////////////////////////////////////
// IBtPrinterCodeTemplate operations

LPDISPATCH IBtPrinterCodeTemplate::GetPrintMethod()
{
	LPDISPATCH result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBtPrinterCodeTemplate::GetPerformance()
{
	LPDISPATCH result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBtPrinterCodeTemplate::GetJob()
{
	LPDISPATCH result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL IBtPrinterCodeTemplate::Export(LPCTSTR PrintSystemName, long ExportType, LPCTSTR FormatOrCombinedExportPath, LPCTSTR DataExportPath, LPDISPATCH* Messages)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_BSTR VTS_BSTR VTS_PDISPATCH;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		PrintSystemName, ExportType, FormatOrCombinedExportPath, DataExportPath, Messages);
	return result;
}

void IBtPrinterCodeTemplate::SetExportAsVersion(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtPrinterCodeTemplate::GetExportAsVersion()
{
	long result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IBtJob properties

/////////////////////////////////////////////////////////////////////////////
// IBtJob operations

void IBtJob::SetFormatIdType(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtJob::GetFormatIdType()
{
	long result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtJob::SetFormatId(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtJob::GetFormatId()
{
	CString result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtJob::SetPrinterCopiesType(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtJob::GetPrinterCopiesType()
{
	long result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtJob::SetPrinterCopies(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtJob::GetPrinterCopies()
{
	CString result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtJob::SetPrinterSerializedCopiesType(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtJob::GetPrinterSerializedCopiesType()
{
	long result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtJob::SetPrinterSerializedCopies(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtJob::GetPrinterSerializedCopies()
{
	CString result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtJob::SetPrinterBatchCopiesType(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtJob::GetPrinterBatchCopiesType()
{
	long result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtJob::SetPrinterBatchCopies(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtJob::GetPrinterBatchCopies()
{
	CString result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// DBtJob properties

long DBtJob::GetFormatIdValueType()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void DBtJob::SetFormatIdValueType(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

CString DBtJob::GetFormatIdValue()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}

void DBtJob::SetFormatIdValue(LPCTSTR propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}

long DBtJob::GetPrinterCopiesValueType()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void DBtJob::SetPrinterCopiesValueType(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

CString DBtJob::GetPrinterCopiesValue()
{
	CString result;
	GetProperty(0x4, VT_BSTR, (void*)&result);
	return result;
}

void DBtJob::SetPrinterCopiesValue(LPCTSTR propVal)
{
	SetProperty(0x4, VT_BSTR, propVal);
}

long DBtJob::GetPrinterSerializedCopiesValueType()
{
	long result;
	GetProperty(0x5, VT_I4, (void*)&result);
	return result;
}

void DBtJob::SetPrinterSerializedCopiesValueType(long propVal)
{
	SetProperty(0x5, VT_I4, propVal);
}

CString DBtJob::GetPrinterSerializedCopiesValue()
{
	CString result;
	GetProperty(0x6, VT_BSTR, (void*)&result);
	return result;
}

void DBtJob::SetPrinterSerializedCopiesValue(LPCTSTR propVal)
{
	SetProperty(0x6, VT_BSTR, propVal);
}

long DBtJob::GetPrinterBatchCopiesValueType()
{
	long result;
	GetProperty(0x7, VT_I4, (void*)&result);
	return result;
}

void DBtJob::SetPrinterBatchCopiesValueType(long propVal)
{
	SetProperty(0x7, VT_I4, propVal);
}

CString DBtJob::GetPrinterBatchCopiesValue()
{
	CString result;
	GetProperty(0x8, VT_BSTR, (void*)&result);
	return result;
}

void DBtJob::SetPrinterBatchCopiesValue(LPCTSTR propVal)
{
	SetProperty(0x8, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtJob operations


/////////////////////////////////////////////////////////////////////////////
// DBtPrinterCodeTemplate properties

LPDISPATCH DBtPrinterCodeTemplate::GetPrintMethod()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

void DBtPrinterCodeTemplate::SetPrintMethod(LPDISPATCH propVal)
{
	SetProperty(0x1, VT_DISPATCH, propVal);
}

LPDISPATCH DBtPrinterCodeTemplate::GetPerformance()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void DBtPrinterCodeTemplate::SetPerformance(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

LPDISPATCH DBtPrinterCodeTemplate::GetJob()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void DBtPrinterCodeTemplate::SetJob(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

long DBtPrinterCodeTemplate::GetExportAsVersion()
{
	long result;
	GetProperty(0x5, VT_I4, (void*)&result);
	return result;
}

void DBtPrinterCodeTemplate::SetExportAsVersion(long propVal)
{
	SetProperty(0x5, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtPrinterCodeTemplate operations

BOOL DBtPrinterCodeTemplate::Export(LPCTSTR PrintSystemName, long ExportType, LPCTSTR FormatOrCombinedExportPath, LPCTSTR DataExportPath, LPDISPATCH* Messages)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_BSTR VTS_BSTR VTS_PDISPATCH;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		PrintSystemName, ExportType, FormatOrCombinedExportPath, DataExportPath, Messages);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// DBtFormat properties

BOOL DBtFormat::GetAutoPrintAgain()
{
	BOOL result;
	GetProperty(0x2, VT_BOOL, (void*)&result);
	return result;
}

void DBtFormat::SetAutoPrintAgain(BOOL propVal)
{
	SetProperty(0x2, VT_BOOL, propVal);
}

CString DBtFormat::GetBaseName()
{
	CString result;
	GetProperty(0x3, VT_BSTR, (void*)&result);
	return result;
}

void DBtFormat::SetBaseName(LPCTSTR propVal)
{
	SetProperty(0x3, VT_BSTR, propVal);
}

CString DBtFormat::GetDirectory()
{
	CString result;
	GetProperty(0x6, VT_BSTR, (void*)&result);
	return result;
}

void DBtFormat::SetDirectory(LPCTSTR propVal)
{
	SetProperty(0x6, VT_BSTR, propVal);
}

CString DBtFormat::GetFileName()
{
	CString result;
	GetProperty(0x8, VT_BSTR, (void*)&result);
	return result;
}

void DBtFormat::SetFileName(LPCTSTR propVal)
{
	SetProperty(0x8, VT_BSTR, propVal);
}

long DBtFormat::GetIdenticalCopiesOfLabel()
{
	long result;
	GetProperty(0x9, VT_I4, (void*)&result);
	return result;
}

void DBtFormat::SetIdenticalCopiesOfLabel(long propVal)
{
	SetProperty(0x9, VT_I4, propVal);
}

LPDISPATCH DBtFormat::GetInputDataFileSetup()
{
	LPDISPATCH result;
	GetProperty(0xa, VT_DISPATCH, (void*)&result);
	return result;
}

void DBtFormat::SetInputDataFileSetup(LPDISPATCH propVal)
{
	SetProperty(0xa, VT_DISPATCH, propVal);
}

long DBtFormat::GetNumberSerializedLabels()
{
	long result;
	GetProperty(0xe, VT_I4, (void*)&result);
	return result;
}

void DBtFormat::SetNumberSerializedLabels(long propVal)
{
	SetProperty(0xe, VT_I4, propVal);
}

CString DBtFormat::GetPrinter()
{
	CString result;
	GetProperty(0x10, VT_BSTR, (void*)&result);
	return result;
}

void DBtFormat::SetPrinter(LPCTSTR propVal)
{
	SetProperty(0x10, VT_BSTR, propVal);
}

BOOL DBtFormat::GetPrintToFile()
{
	BOOL result;
	GetProperty(0xf, VT_BOOL, (void*)&result);
	return result;
}

void DBtFormat::SetPrintToFile(BOOL propVal)
{
	SetProperty(0xf, VT_BOOL, propVal);
}

CString DBtFormat::GetTitle()
{
	CString result;
	GetProperty(0x16, VT_BSTR, (void*)&result);
	return result;
}

void DBtFormat::SetTitle(LPCTSTR propVal)
{
	SetProperty(0x16, VT_BSTR, propVal);
}

BOOL DBtFormat::GetUseInputDataFile()
{
	BOOL result;
	GetProperty(0x17, VT_BOOL, (void*)&result);
	return result;
}

void DBtFormat::SetUseInputDataFile(BOOL propVal)
{
	SetProperty(0x17, VT_BOOL, propVal);
}

long DBtFormat::GetSaveAsVersion()
{
	long result;
	GetProperty(0x19, VT_I4, (void*)&result);
	return result;
}

void DBtFormat::SetSaveAsVersion(long propVal)
{
	SetProperty(0x19, VT_I4, propVal);
}

LPDISPATCH DBtFormat::GetNamedSubStrings()
{
	LPDISPATCH result;
	GetProperty(0x1c, VT_DISPATCH, (void*)&result);
	return result;
}

void DBtFormat::SetNamedSubStrings(LPDISPATCH propVal)
{
	SetProperty(0x1c, VT_DISPATCH, propVal);
}

LPDISPATCH DBtFormat::GetDatabases()
{
	LPDISPATCH result;
	GetProperty(0x1d, VT_DISPATCH, (void*)&result);
	return result;
}

void DBtFormat::SetDatabases(LPDISPATCH propVal)
{
	SetProperty(0x1d, VT_DISPATCH, propVal);
}

BOOL DBtFormat::GetEnablePrompting()
{
	BOOL result;
	GetProperty(0x1e, VT_BOOL, (void*)&result);
	return result;
}

void DBtFormat::SetEnablePrompting(BOOL propVal)
{
	SetProperty(0x1e, VT_BOOL, propVal);
}

CString DBtFormat::GetRecordRange()
{
	CString result;
	GetProperty(0x1f, VT_BSTR, (void*)&result);
	return result;
}

void DBtFormat::SetRecordRange(LPCTSTR propVal)
{
	SetProperty(0x1f, VT_BSTR, propVal);
}

BOOL DBtFormat::GetSelectRecordsAtPrint()
{
	BOOL result;
	GetProperty(0x20, VT_BOOL, (void*)&result);
	return result;
}

void DBtFormat::SetSelectRecordsAtPrint(BOOL propVal)
{
	SetProperty(0x20, VT_BOOL, propVal);
}

BOOL DBtFormat::GetUseDatabase()
{
	BOOL result;
	GetProperty(0x21, VT_BOOL, (void*)&result);
	return result;
}

void DBtFormat::SetUseDatabase(BOOL propVal)
{
	SetProperty(0x21, VT_BOOL, propVal);
}

CString DBtFormat::GetPrinterFile()
{
	CString result;
	GetProperty(0x22, VT_BSTR, (void*)&result);
	return result;
}

void DBtFormat::SetPrinterFile(LPCTSTR propVal)
{
	SetProperty(0x22, VT_BSTR, propVal);
}

LPDISPATCH DBtFormat::GetPageSetup()
{
	LPDISPATCH result;
	GetProperty(0x24, VT_DISPATCH, (void*)&result);
	return result;
}

void DBtFormat::SetPageSetup(LPDISPATCH propVal)
{
	SetProperty(0x24, VT_DISPATCH, propVal);
}

LPDISPATCH DBtFormat::GetPrintSetup()
{
	LPDISPATCH result;
	GetProperty(0x26, VT_DISPATCH, (void*)&result);
	return result;
}

void DBtFormat::SetPrintSetup(LPDISPATCH propVal)
{
	SetProperty(0x26, VT_DISPATCH, propVal);
}

LPDISPATCH DBtFormat::GetPrinterCodeTemplate()
{
	LPDISPATCH result;
	GetProperty(0x27, VT_DISPATCH, (void*)&result);
	return result;
}

void DBtFormat::SetPrinterCodeTemplate(LPDISPATCH propVal)
{
	SetProperty(0x27, VT_DISPATCH, propVal);
}

CString DBtFormat::GetUsedNames()
{
	CString result;
	GetProperty(0x28, VT_BSTR, (void*)&result);
	return result;
}

void DBtFormat::SetUsedNames(LPCTSTR propVal)
{
	SetProperty(0x28, VT_BSTR, propVal);
}

CString DBtFormat::GetUsedNamesSampleData()
{
	CString result;
	GetProperty(0x29, VT_BSTR, (void*)&result);
	return result;
}

void DBtFormat::SetUsedNamesSampleData(LPCTSTR propVal)
{
	SetProperty(0x29, VT_BSTR, propVal);
}

CString DBtFormat::GetJobName()
{
	CString result;
	GetProperty(0x2e, VT_BSTR, (void*)&result);
	return result;
}

void DBtFormat::SetJobName(LPCTSTR propVal)
{
	SetProperty(0x2e, VT_BSTR, propVal);
}

CString DBtFormat::GetPrintToFileLicense()
{
	CString result;
	GetProperty(0x2f, VT_BSTR, (void*)&result);
	return result;
}

void DBtFormat::SetPrintToFileLicense(LPCTSTR propVal)
{
	SetProperty(0x2f, VT_BSTR, propVal);
}

CString DBtFormat::GetPaperTray()
{
	CString result;
	GetProperty(0x32, VT_BSTR, (void*)&result);
	return result;
}

void DBtFormat::SetPaperTray(LPCTSTR propVal)
{
	SetProperty(0x32, VT_BSTR, propVal);
}

CString DBtFormat::GetComment()
{
	CString result;
	GetProperty(0x33, VT_BSTR, (void*)&result);
	return result;
}

void DBtFormat::SetComment(LPCTSTR propVal)
{
	SetProperty(0x33, VT_BSTR, propVal);
}

long DBtFormat::GetEncryption()
{
	long result;
	GetProperty(0x36, VT_I4, (void*)&result);
	return result;
}

void DBtFormat::SetEncryption(long propVal)
{
	SetProperty(0x36, VT_I4, propVal);
}

BOOL DBtFormat::GetSupportsSetSerializedCopies()
{
	BOOL result;
	GetProperty(0x37, VT_BOOL, (void*)&result);
	return result;
}

void DBtFormat::SetSupportsSetSerializedCopies(BOOL propVal)
{
	SetProperty(0x37, VT_BOOL, propVal);
}

BOOL DBtFormat::GetSupportsSetIdenticalCopies()
{
	BOOL result;
	GetProperty(0x38, VT_BOOL, (void*)&result);
	return result;
}

void DBtFormat::SetSupportsSetIdenticalCopies(BOOL propVal)
{
	SetProperty(0x38, VT_BOOL, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtFormat operations

void DBtFormat::Activate()
{
	InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void DBtFormat::Close(long SaveChanges)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SaveChanges);
}

void DBtFormat::PrintOut(BOOL ShowStatusWindow, BOOL ShowPrintDialog)
{
	static BYTE parms[] =
		VTS_BOOL VTS_BOOL;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ShowStatusWindow, ShowPrintDialog);
}

void DBtFormat::Save()
{
	InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void DBtFormat::SaveAs(LPCTSTR FileName, BOOL OverwriteIfExists)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL;
	InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName, OverwriteIfExists);
}

void DBtFormat::ExportToClipboard(long Colors, long Resolution)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Colors, Resolution);
}

void DBtFormat::ExportToFile(LPCTSTR FileName, LPCTSTR FileType, long Colors, long Resolution, long SaveOption)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName, FileType, Colors, Resolution, SaveOption);
}

void DBtFormat::SetAvailableTemplateFieldNames(LPCTSTR FieldnameSampleDataList, LPCTSTR delimiter)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x2a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FieldnameSampleDataList, delimiter);
}

CString DBtFormat::ExportPromptToXML(LPCTSTR XSLFileName)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2c, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		XSLFileName);
	return result;
}

void DBtFormat::SetPrompt(LPCTSTR PromptName, LPCTSTR PromptData)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x2d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 PromptName, PromptData);
}

long DBtFormat::Print(LPCTSTR PrintJobName, BOOL WaitForSpoolJobToComplete, long TimeoutMs, LPDISPATCH* Messages)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL VTS_I4 VTS_PDISPATCH;
	InvokeHelper(0x30, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		PrintJobName, WaitForSpoolJobToComplete, TimeoutMs, Messages);
	return result;
}

long DBtFormat::ExportPrintPreviewToImage(LPCTSTR Directory, LPCTSTR FileNameTemplate, LPCTSTR FileType, long Colors, long dpi, long backgroundColor, long SaveOpts, BOOL IncludeMargins, BOOL IncludeBorders, LPDISPATCH* Messages)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BOOL VTS_BOOL VTS_PDISPATCH;
	InvokeHelper(0x35, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Directory, FileNameTemplate, FileType, Colors, dpi, backgroundColor, SaveOpts, IncludeMargins, IncludeBorders, Messages);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IFormatEvents properties

/////////////////////////////////////////////////////////////////////////////
// IFormatEvents operations

void IFormatEvents::OnNewPrintJob(LPCTSTR PrinterName, long JobID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0x34, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 PrinterName, JobID);
}


/////////////////////////////////////////////////////////////////////////////
// IBtFormats properties

/////////////////////////////////////////////////////////////////////////////
// IBtFormats operations

long IBtFormats::GetCount()
{
	long result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBtFormats::Add()
{
	LPDISPATCH result;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBtFormats::Item(VARIANT* Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

LPDISPATCH IBtFormats::Open(LPCTSTR FileName, BOOL CloseOutFirstFormat, LPCTSTR UsePrinter)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL VTS_BSTR;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		FileName, CloseOutFirstFormat, UsePrinter);
	return result;
}

LPDISPATCH IBtFormats::GetFormat(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// DBtFormats properties

long DBtFormats::GetCount()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void DBtFormats::SetCount(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtFormats operations

LPDISPATCH DBtFormats::Add()
{
	LPDISPATCH result;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH DBtFormats::Open(LPCTSTR FileName, BOOL CloseOutFirstFormat, LPCTSTR UsePrinter)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL VTS_BSTR;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		FileName, CloseOutFirstFormat, UsePrinter);
	return result;
}

LPDISPATCH DBtFormats::Item(VARIANT* Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

LPDISPATCH DBtFormats::GetFormat(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IBtWindow properties

/////////////////////////////////////////////////////////////////////////////
// IBtWindow operations

void IBtWindow::SetLeft(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtWindow::GetLeft()
{
	long result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtWindow::SetTop(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtWindow::GetTop()
{
	long result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtWindow::SetWidth(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtWindow::GetWidth()
{
	long result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtWindow::SetHeight(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtWindow::GetHeight()
{
	long result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtWindow::SetWindowState(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtWindow::GetWindowState()
{
	long result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// DBtWindow properties

long DBtWindow::GetLeft()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void DBtWindow::SetLeft(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

long DBtWindow::GetTop()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void DBtWindow::SetTop(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

long DBtWindow::GetWidth()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void DBtWindow::SetWidth(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

long DBtWindow::GetHeight()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}

void DBtWindow::SetHeight(long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}

long DBtWindow::GetWindowState()
{
	long result;
	GetProperty(0x5, VT_I4, (void*)&result);
	return result;
}

void DBtWindow::SetWindowState(long propVal)
{
	SetProperty(0x5, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtWindow operations


/////////////////////////////////////////////////////////////////////////////
// IBtLicenseServer properties

/////////////////////////////////////////////////////////////////////////////
// IBtLicenseServer operations

void IBtLicenseServer::SetAddress(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtLicenseServer::GetAddress()
{
	CString result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtLicenseServer::SetPort(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtLicenseServer::GetPort()
{
	long result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtLicenseServer::SetTimeout(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtLicenseServer::GetTimeout()
{
	long result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL IBtLicenseServer::GetIsConnected()
{
	BOOL result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL IBtLicenseServer::Connect()
{
	BOOL result;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IBtLicenseServer::SetAlternateAddress(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IBtLicenseServer::GetAlternateAddress()
{
	CString result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IBtLicenseServer::SetAlternatePort(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtLicenseServer::GetAlternatePort()
{
	long result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtLicenseServer::SetAlternateTimeout(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtLicenseServer::GetAlternateTimeout()
{
	long result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long IBtLicenseServer::GetMaxAllowedPrinters()
{
	long result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long IBtLicenseServer::GetMaxAllowedUsers()
{
	long result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtLicenseServer::SetRetries(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtLicenseServer::GetRetries()
{
	long result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IBtLicenseServer::SetAlternateRetries(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IBtLicenseServer::GetAlternateRetries()
{
	long result;
	InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// DBtLicenseServer properties

CString DBtLicenseServer::GetAddress()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void DBtLicenseServer::SetAddress(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

CString DBtLicenseServer::GetAlternateAddress()
{
	CString result;
	GetProperty(0x6, VT_BSTR, (void*)&result);
	return result;
}

void DBtLicenseServer::SetAlternateAddress(LPCTSTR propVal)
{
	SetProperty(0x6, VT_BSTR, propVal);
}

long DBtLicenseServer::GetPort()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void DBtLicenseServer::SetPort(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

long DBtLicenseServer::GetAlternatePort()
{
	long result;
	GetProperty(0x7, VT_I4, (void*)&result);
	return result;
}

void DBtLicenseServer::SetAlternatePort(long propVal)
{
	SetProperty(0x7, VT_I4, propVal);
}

long DBtLicenseServer::GetTimeout()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void DBtLicenseServer::SetTimeout(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

long DBtLicenseServer::GetAlternateTimeout()
{
	long result;
	GetProperty(0x8, VT_I4, (void*)&result);
	return result;
}

void DBtLicenseServer::SetAlternateTimeout(long propVal)
{
	SetProperty(0x8, VT_I4, propVal);
}

BOOL DBtLicenseServer::GetIsConnected()
{
	BOOL result;
	GetProperty(0x4, VT_BOOL, (void*)&result);
	return result;
}

void DBtLicenseServer::SetIsConnected(BOOL propVal)
{
	SetProperty(0x4, VT_BOOL, propVal);
}

long DBtLicenseServer::GetMaxAllowedPrinters()
{
	long result;
	GetProperty(0x9, VT_I4, (void*)&result);
	return result;
}

void DBtLicenseServer::SetMaxAllowedPrinters(long propVal)
{
	SetProperty(0x9, VT_I4, propVal);
}

long DBtLicenseServer::GetMaxAllowedUsers()
{
	long result;
	GetProperty(0xa, VT_I4, (void*)&result);
	return result;
}

void DBtLicenseServer::SetMaxAllowedUsers(long propVal)
{
	SetProperty(0xa, VT_I4, propVal);
}

long DBtLicenseServer::GetRetries()
{
	long result;
	GetProperty(0xb, VT_I4, (void*)&result);
	return result;
}

void DBtLicenseServer::SetRetries(long propVal)
{
	SetProperty(0xb, VT_I4, propVal);
}

long DBtLicenseServer::GetAlternateRetries()
{
	long result;
	GetProperty(0xc, VT_I4, (void*)&result);
	return result;
}

void DBtLicenseServer::SetAlternateRetries(long propVal)
{
	SetProperty(0xc, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DBtLicenseServer operations

BOOL DBtLicenseServer::Connect()
{
	BOOL result;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IApplicationEvents properties

/////////////////////////////////////////////////////////////////////////////
// IApplicationEvents operations

void IApplicationEvents::OnCommandLineCompleted(LPCTSTR ResponseString, LPDISPATCH* Messages, LPCTSTR CommandLine)
{
	static BYTE parms[] =
		VTS_BSTR VTS_PDISPATCH VTS_BSTR;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ResponseString, Messages, CommandLine);
}

void IApplicationEvents::OnNewPrintJob(LPCTSTR PrinterName, long JobID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 PrinterName, JobID);
}


/////////////////////////////////////////////////////////////////////////////
// IScreenDs properties

CString IScreenDs::GetName()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void IScreenDs::SetName(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

CString IScreenDs::GetValue()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}

void IScreenDs::SetValue(LPCTSTR propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// IScreenDs operations


/////////////////////////////////////////////////////////////////////////////
// ITemplateFieldDs properties

CString ITemplateFieldDs::GetName()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void ITemplateFieldDs::SetName(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

CString ITemplateFieldDs::GetValue()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}

void ITemplateFieldDs::SetValue(LPCTSTR propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}

CString ITemplateFieldDs::GetFieldName()
{
	CString result;
	GetProperty(0x3, VT_BSTR, (void*)&result);
	return result;
}

void ITemplateFieldDs::SetFieldName(LPCTSTR propVal)
{
	SetProperty(0x3, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// ITemplateFieldDs operations


/////////////////////////////////////////////////////////////////////////////
// ILabelObjectDs properties

CString ILabelObjectDs::GetObjectName()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}

void ILabelObjectDs::SetObjectName(LPCTSTR propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}

CString ILabelObjectDs::GetValue()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void ILabelObjectDs::SetValue(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// ILabelObjectDs operations
