// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// DBtApplication wrapper class

class DBtApplication : public COleDispatchDriver
{
public:
	DBtApplication() {}		// Calls COleDispatchDriver default constructor
	DBtApplication(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtApplication(const DBtApplication& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetActiveFormat();
	void SetActiveFormat(LPDISPATCH);
	long GetBuildNumber();
	void SetBuildNumber(long);
	LPDISPATCH GetFormats();
	void SetFormats(LPDISPATCH);
	BOOL GetIsPrinting();
	void SetIsPrinting(BOOL);
	BOOL GetIsProcessingCommandLines();
	void SetIsProcessingCommandLines(BOOL);
	CString GetVersion();
	void SetVersion(LPCTSTR);
	BOOL GetVisible();
	void SetVisible(BOOL);
	LPDISPATCH GetWindow();
	void SetWindow(LPDISPATCH);
	CString GetSAPIDocDefinitionFile();
	void SetSAPIDocDefinitionFile(LPCTSTR);
	LPDISPATCH GetLicenseServer();
	void SetLicenseServer(LPDISPATCH);
	CString GetEdition();
	void SetEdition(LPCTSTR);
	long GetVisibleWindows();
	void SetVisibleWindows(long);
	BOOL GetTopMost();
	void SetTopMost(BOOL);
	// property 'ParentWindow' not emitted because of invalid type

// Operations
public:
	void Quit(long SaveChanges);
	void Save(BOOL prompt);
	void CommandLine(LPCTSTR CommandLine);
	CString XMLScript(LPCTSTR XMLScript, long SourceType, LPDISPATCH* Messages);
};
/////////////////////////////////////////////////////////////////////////////
// IBtMessages wrapper class

class IBtMessages : public COleDispatchDriver
{
public:
	IBtMessages() {}		// Calls COleDispatchDriver default constructor
	IBtMessages(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtMessages(const IBtMessages& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetCount();
	LPDISPATCH Item(VARIANT* Index);
	LPDISPATCH GetMessage(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// IBtMessage wrapper class

class IBtMessage : public COleDispatchDriver
{
public:
	IBtMessage() {}		// Calls COleDispatchDriver default constructor
	IBtMessage(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtMessage(const IBtMessage& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetNumber();
	long GetType();
	long GetSeverity();
	CString GetMessage();
	LPDISPATCH GetVerification();
	CString GetTypeText();
};
/////////////////////////////////////////////////////////////////////////////
// IBtVerification wrapper class

class IBtVerification : public COleDispatchDriver
{
public:
	IBtVerification() {}		// Calls COleDispatchDriver default constructor
	IBtVerification(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtVerification(const IBtVerification& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString GetObject();
	CString GetFields();
	CString GetProblem();
	CString GetResult();
	CString GetAutoFix();
};
/////////////////////////////////////////////////////////////////////////////
// DBtVerification wrapper class

class DBtVerification : public COleDispatchDriver
{
public:
	DBtVerification() {}		// Calls COleDispatchDriver default constructor
	DBtVerification(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtVerification(const DBtVerification& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetObject();
	void SetObject(LPCTSTR);
	CString GetFields();
	void SetFields(LPCTSTR);
	CString GetProblem();
	void SetProblem(LPCTSTR);
	CString GetResult();
	void SetResult(LPCTSTR);
	CString GetAutoFix();
	void SetAutoFix(LPCTSTR);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// DBtMessage wrapper class

class DBtMessage : public COleDispatchDriver
{
public:
	DBtMessage() {}		// Calls COleDispatchDriver default constructor
	DBtMessage(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtMessage(const DBtMessage& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetNumNumber();
	void SetNumNumber(long);
	long GetType();
	void SetType(long);
	long GetSeverity();
	void SetSeverity(long);
	CString GetMessage();
	void SetMessage(LPCTSTR);
	LPDISPATCH GetVerification();
	void SetVerification(LPDISPATCH);
	CString GetTypeText();
	void SetTypeText(LPCTSTR);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// DBtMessages wrapper class

class DBtMessages : public COleDispatchDriver
{
public:
	DBtMessages() {}		// Calls COleDispatchDriver default constructor
	DBtMessages(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtMessages(const DBtMessages& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetCount();
	void SetCount(long);

// Operations
public:
	LPDISPATCH Item(VARIANT* Index);
	LPDISPATCH GetMessage(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// IBtApplication wrapper class

class IBtApplication : public COleDispatchDriver
{
public:
	IBtApplication() {}		// Calls COleDispatchDriver default constructor
	IBtApplication(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtApplication(const IBtApplication& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetActiveFormat();
	long GetBuildNumber();
	LPDISPATCH GetFormats();
	BOOL GetIsPrinting();
	BOOL GetIsProcessingCommandLines();
	CString GetVersion();
	LPDISPATCH GetWindow();
	void Quit(long SaveChanges);
	void Save(BOOL DoPrompt);
	void SetSAPIDocDefinitionFile(LPCTSTR lpszNewValue);
	CString GetSAPIDocDefinitionFile();
	void CommandLine(LPCTSTR CommandLine);
	LPDISPATCH GetLicenseServer();
	CString XMLScript(LPCTSTR XMLScript, long SourceType, LPDISPATCH* Messages);
	CString GetEdition();
	void SetVisibleWindows(long nNewValue);
	long GetVisibleWindows();
	void SetTopMost(BOOL bNewValue);
	BOOL GetTopMost();
	// method 'SetParentWindow' not emitted because of invalid return type or parameter type
	// method 'GetParentWindow' not emitted because of invalid return type or parameter type
};
/////////////////////////////////////////////////////////////////////////////
// IBtFormat wrapper class

class IBtFormat : public COleDispatchDriver
{
public:
	IBtFormat() {}		// Calls COleDispatchDriver default constructor
	IBtFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtFormat(const IBtFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void SetAutoPrintAgain(BOOL bNewValue);
	BOOL GetAutoPrintAgain();
	CString GetBaseName();
	CString GetDirectory();
	CString GetFileName();
	void SetIdenticalCopiesOfLabel(long nNewValue);
	long GetIdenticalCopiesOfLabel();
	LPDISPATCH GetInputDataFileSetup();
	void SetNumberSerializedLabels(long nNewValue);
	long GetNumberSerializedLabels();
	CString GetPrinter();
	void SetPrintToFile(BOOL bNewValue);
	BOOL GetPrintToFile();
	CString GetTitle();
	void SetUseInputDataFile(BOOL bNewValue);
	BOOL GetUseInputDataFile();
	void Activate();
	void Close(long SaveChanges);
	long PrintOut(BOOL ShowStatusWindow, BOOL ShowPrintDialog);
	void SetNamedSubStringValue(LPCTSTR SubStringName, LPCTSTR Value);
	CString GetNamedSubStringValue(LPCTSTR SubStringName);
	void Save();
	void SaveAs(LPCTSTR FileName, BOOL OverwriteIfExists);
	void SetSaveAsVersion(long nNewValue);
	long GetSaveAsVersion();
	LPDISPATCH GetNamedSubStrings();
	void ExportToClipboard(long Color, long Resolution);
	void ExportToFile(LPCTSTR FileName, LPCTSTR FileType, long Color, long Resolution, long SaveOption);
	LPDISPATCH GetDatabases();
	void SetEnablePrompting(BOOL bNewValue);
	BOOL GetEnablePrompting();
	void SetRecordRange(LPCTSTR lpszNewValue);
	CString GetRecordRange();
	void SetSelectRecordsAtPrint(BOOL bNewValue);
	BOOL GetSelectRecordsAtPrint();
	void SetUseDatabase(BOOL bNewValue);
	BOOL GetUseDatabase();
	void SetPrinter(LPCTSTR lpszNewValue);
	void SetPrinterFile(LPCTSTR lpszNewValue);
	CString GetPrinterFile();
	LPDISPATCH GetPageSetup();
	LPDISPATCH GetPrintSetup();
	LPDISPATCH GetPrinterCodeTemplate();
	CString GetUsedNames(long FieldType, LPCTSTR delimiter);
	void SetAvailableTemplateFieldNames(LPCTSTR FieldnameSampleDataList, LPCTSTR delimiter);
	CString ExportPromptToXML(LPCTSTR XSLFileName);
	void SetPrompt(LPCTSTR PromptName, LPCTSTR PromptData);
	void SetJobName(LPCTSTR lpszNewValue);
	CString GetJobName();
	void SetPrintToFileLicense(LPCTSTR lpszNewValue);
	CString GetPrintToFileLicense();
	long Print(LPCTSTR PrintJobName, BOOL WaitForSpoolJobToComplete, long TimeoutMs, LPDISPATCH* Messages);
	void SetPaperTray(LPCTSTR lpszNewValue);
	CString GetPaperTray();
	void SetComment(LPCTSTR lpszNewValue);
	CString GetComment();
	long ExportPrintPreviewToImage(LPCTSTR Directory, LPCTSTR FileNameTemplate, LPCTSTR FileType, long Colors, long dpi, long backgroundColor, long SaveOpts, BOOL IncludeMargins, BOOL IncludeBorder, LPDISPATCH* Messages);
	void SetEncryption(long nNewValue);
	long GetEncryption();
	BOOL GetSupportsSetSerializedCopies();
	BOOL GetSupportsSetIdenticalCopies();
};
/////////////////////////////////////////////////////////////////////////////
// IBtInputDataFileSetup wrapper class

class IBtInputDataFileSetup : public COleDispatchDriver
{
public:
	IBtInputDataFileSetup() {}		// Calls COleDispatchDriver default constructor
	IBtInputDataFileSetup(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtInputDataFileSetup(const IBtInputDataFileSetup& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void SetAutoViewSelectedRecordsBeforePrint(BOOL bNewValue);
	BOOL GetAutoViewSelectedRecordsBeforePrint();
	LPDISPATCH GetTextFile();
	void SetType(long nNewValue);
	long GetType();
	LPDISPATCH GetOdbc();
	LPDISPATCH GetOledb();
};
/////////////////////////////////////////////////////////////////////////////
// IBtTextFile wrapper class

class IBtTextFile : public COleDispatchDriver
{
public:
	IBtTextFile() {}		// Calls COleDispatchDriver default constructor
	IBtTextFile(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtTextFile(const IBtTextFile& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void SetFileName(LPCTSTR lpszNewValue);
	CString GetFileName();
	void SetDelimitation(long nNewValue);
	long GetDelimitation();
	void SetFieldDelimiter(LPCTSTR lpszNewValue);
	CString GetFieldDelimiter();
	void SetNumberOfFields(long nNewValue);
	long GetNumberOfFields();
	void SetUseFieldNamesFromFirstRecord(BOOL bNewValue);
	BOOL GetUseFieldNamesFromFirstRecord();
};
/////////////////////////////////////////////////////////////////////////////
// DBtTextFile wrapper class

class DBtTextFile : public COleDispatchDriver
{
public:
	DBtTextFile() {}		// Calls COleDispatchDriver default constructor
	DBtTextFile(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtTextFile(const DBtTextFile& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetFileName();
	void SetFileName(LPCTSTR);
	long GetDelimitation();
	void SetDelimitation(long);
	CString GetFieldDelimiter();
	void SetFieldDelimiter(LPCTSTR);
	long GetNumberOfFields();
	void SetNumberOfFields(long);
	BOOL GetUseFieldNamesFromFirstRecord();
	void SetUseFieldNamesFromFirstRecord(BOOL);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// IBtODBC wrapper class

class IBtODBC : public COleDispatchDriver
{
public:
	IBtODBC() {}		// Calls COleDispatchDriver default constructor
	IBtODBC(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtODBC(const IBtODBC& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void SetWhere(LPCTSTR lpszNewValue);
	CString GetWhere();
	void SetPassword(LPCTSTR lpszNewValue);
	CString GetPassword();
	void SetUserId(LPCTSTR lpszNewValue);
	CString GetUserId();
	void SetSQLStatement(LPCTSTR lpszNewValue);
	CString GetSQLStatement();
};
/////////////////////////////////////////////////////////////////////////////
// DBtODBC wrapper class

class DBtODBC : public COleDispatchDriver
{
public:
	DBtODBC() {}		// Calls COleDispatchDriver default constructor
	DBtODBC(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtODBC(const DBtODBC& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetWhere();
	void SetWhere(LPCTSTR);
	CString GetPassword();
	void SetPassword(LPCTSTR);
	CString GetUserId();
	void SetUserId(LPCTSTR);
	CString GetSQLStatement();
	void SetSQLStatement(LPCTSTR);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// IBtOLEDB wrapper class

class IBtOLEDB : public COleDispatchDriver
{
public:
	IBtOLEDB() {}		// Calls COleDispatchDriver default constructor
	IBtOLEDB(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtOLEDB(const IBtOLEDB& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void SetPassword(LPCTSTR lpszNewValue);
	CString GetPassword();
	void SetUserId(LPCTSTR lpszNewValue);
	CString GetUserId();
	void SetSQLStatement(LPCTSTR lpszNewValue);
	CString GetSQLStatement();
};
/////////////////////////////////////////////////////////////////////////////
// DBtOLEDB wrapper class

class DBtOLEDB : public COleDispatchDriver
{
public:
	DBtOLEDB() {}		// Calls COleDispatchDriver default constructor
	DBtOLEDB(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtOLEDB(const DBtOLEDB& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetPassword();
	void SetPassword(LPCTSTR);
	CString GetUserId();
	void SetUserId(LPCTSTR);
	CString GetSQLStatement();
	void SetSQLStatement(LPCTSTR);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// DBtInputDataFileSetup wrapper class

class DBtInputDataFileSetup : public COleDispatchDriver
{
public:
	DBtInputDataFileSetup() {}		// Calls COleDispatchDriver default constructor
	DBtInputDataFileSetup(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtInputDataFileSetup(const DBtInputDataFileSetup& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	BOOL GetAutoViewSelectedRecordsBeforePrint();
	void SetAutoViewSelectedRecordsBeforePrint(BOOL);
	LPDISPATCH GetTextFile();
	void SetTextFile(LPDISPATCH);
	long GetType();
	void SetType(long);
	LPDISPATCH GetOdbc();
	void SetOdbc(LPDISPATCH);
	LPDISPATCH GetOledb();
	void SetOledb(LPDISPATCH);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// IBtNamedSubStrings wrapper class

class IBtNamedSubStrings : public COleDispatchDriver
{
public:
	IBtNamedSubStrings() {}		// Calls COleDispatchDriver default constructor
	IBtNamedSubStrings(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtNamedSubStrings(const IBtNamedSubStrings& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetCount();
	LPDISPATCH Item(VARIANT* Index);
	CString GetAll(LPCTSTR nameValueDelimiter, LPCTSTR recordDelimter);
	void SetAll(LPCTSTR setStr, LPCTSTR delimter);
	LPDISPATCH GetSubString(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// IBtSubString wrapper class

class IBtSubString : public COleDispatchDriver
{
public:
	IBtSubString() {}		// Calls COleDispatchDriver default constructor
	IBtSubString(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtSubString(const IBtSubString& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString GetName();
	CString GetValue();
	void SetValue(BSTR* newValue);
	long GetType();
};
/////////////////////////////////////////////////////////////////////////////
// DBtSubString wrapper class

class DBtSubString : public COleDispatchDriver
{
public:
	DBtSubString() {}		// Calls COleDispatchDriver default constructor
	DBtSubString(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtSubString(const DBtSubString& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetName();
	void SetName(LPCTSTR);
	CString GetValue();
	void SetValue(LPCTSTR);
	long GetType();
	void SetType(long);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// DBtNamedSubStrings wrapper class

class DBtNamedSubStrings : public COleDispatchDriver
{
public:
	DBtNamedSubStrings() {}		// Calls COleDispatchDriver default constructor
	DBtNamedSubStrings(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtNamedSubStrings(const DBtNamedSubStrings& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetCount();
	void SetCount(long);

// Operations
public:
	LPDISPATCH Item(VARIANT* Index);
	CString GetAll(LPCTSTR nameValueDelimiter, LPCTSTR recordDelimiter);
	void SetAll(LPCTSTR subStringsAndValues, LPCTSTR delimiter);
	LPDISPATCH GetSubString(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// IBtDatabases wrapper class

class IBtDatabases : public COleDispatchDriver
{
public:
	IBtDatabases() {}		// Calls COleDispatchDriver default constructor
	IBtDatabases(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtDatabases(const IBtDatabases& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetCount();
	LPDISPATCH GetQueryPrompts();
	LPDISPATCH Item(VARIANT* Index);
	LPDISPATCH GetDatabase(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// IBtQueryPrompts wrapper class

class IBtQueryPrompts : public COleDispatchDriver
{
public:
	IBtQueryPrompts() {}		// Calls COleDispatchDriver default constructor
	IBtQueryPrompts(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtQueryPrompts(const IBtQueryPrompts& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetCount();
	LPDISPATCH Item(VARIANT* Index);
	LPDISPATCH GetQueryPrompt(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// IBtQueryPrompt wrapper class

class IBtQueryPrompt : public COleDispatchDriver
{
public:
	IBtQueryPrompt() {}		// Calls COleDispatchDriver default constructor
	IBtQueryPrompt(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtQueryPrompt(const IBtQueryPrompt& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void SetDefaultReply(LPCTSTR lpszNewValue);
	CString GetDefaultReply();
	CString GetName();
	void SetUserPrompt(LPCTSTR lpszNewValue);
	CString GetUserPrompt();
	void SetValue(LPCTSTR lpszNewValue);
	CString GetValue();
};
/////////////////////////////////////////////////////////////////////////////
// DBtQueryPrompt wrapper class

class DBtQueryPrompt : public COleDispatchDriver
{
public:
	DBtQueryPrompt() {}		// Calls COleDispatchDriver default constructor
	DBtQueryPrompt(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtQueryPrompt(const DBtQueryPrompt& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetDefaultReply();
	void SetDefaultReply(LPCTSTR);
	CString GetName();
	void SetName(LPCTSTR);
	CString GetUserPrompt();
	void SetUserPrompt(LPCTSTR);
	CString GetValue();
	void SetValue(LPCTSTR);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// DBtQueryPrompts wrapper class

class DBtQueryPrompts : public COleDispatchDriver
{
public:
	DBtQueryPrompts() {}		// Calls COleDispatchDriver default constructor
	DBtQueryPrompts(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtQueryPrompts(const DBtQueryPrompts& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetCount();
	void SetCount(long);

// Operations
public:
	LPDISPATCH Item(VARIANT* Index);
	LPDISPATCH GetQueryPrompt(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// IBtDatabase wrapper class

class IBtDatabase : public COleDispatchDriver
{
public:
	IBtDatabase() {}		// Calls COleDispatchDriver default constructor
	IBtDatabase(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtDatabase(const IBtDatabase& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void SetName(LPCTSTR lpszNewValue);
	CString GetName();
	LPDISPATCH GetOdbc();
	LPDISPATCH GetTextFile();
	long GetType();
	LPDISPATCH GetSAPIDoc();
	LPDISPATCH GetOledb();
};
/////////////////////////////////////////////////////////////////////////////
// IBtSAPIDoc wrapper class

class IBtSAPIDoc : public COleDispatchDriver
{
public:
	IBtSAPIDoc() {}		// Calls COleDispatchDriver default constructor
	IBtSAPIDoc(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtSAPIDoc(const IBtSAPIDoc& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void SetIDocFileName(LPCTSTR lpszNewValue);
	CString GetIDocFileName();
};
/////////////////////////////////////////////////////////////////////////////
// DBtSAPIDoc wrapper class

class DBtSAPIDoc : public COleDispatchDriver
{
public:
	DBtSAPIDoc() {}		// Calls COleDispatchDriver default constructor
	DBtSAPIDoc(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtSAPIDoc(const DBtSAPIDoc& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetIDocFileName();
	void SetIDocFileName(LPCTSTR);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// DBtDatabase wrapper class

class DBtDatabase : public COleDispatchDriver
{
public:
	DBtDatabase() {}		// Calls COleDispatchDriver default constructor
	DBtDatabase(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtDatabase(const DBtDatabase& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetName();
	void SetName(LPCTSTR);
	LPDISPATCH GetOdbc();
	void SetOdbc(LPDISPATCH);
	LPDISPATCH GetTextFile();
	void SetTextFile(LPDISPATCH);
	long GetType();
	void SetType(long);
	LPDISPATCH GetSAPIDoc();
	void SetSAPIDoc(LPDISPATCH);
	LPDISPATCH GetOledb();
	void SetOledb(LPDISPATCH);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// DBtDatabases wrapper class

class DBtDatabases : public COleDispatchDriver
{
public:
	DBtDatabases() {}		// Calls COleDispatchDriver default constructor
	DBtDatabases(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtDatabases(const DBtDatabases& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetCount();
	void SetCount(long);
	LPDISPATCH GetQueryPrompts();
	void SetQueryPrompts(LPDISPATCH);

// Operations
public:
	LPDISPATCH Item(VARIANT* Index);
	LPDISPATCH GetDatabase(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// IBtPageSetup wrapper class

class IBtPageSetup : public COleDispatchDriver
{
public:
	IBtPageSetup() {}		// Calls COleDispatchDriver default constructor
	IBtPageSetup(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtPageSetup(const IBtPageSetup& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString GetStockCategory();
	CString GetStockType();
	void SetPaperName(LPCTSTR lpszNewValue);
	CString GetPaperName();
	void SetPaperID(long nNewValue);
	long GetPaperID();
	void SetOrientation(long nNewValue);
	long GetOrientation();
	void SetLabelColumns(long nNewValue);
	long GetLabelColumns();
	void SetLabelRows(long nNewValue);
	long GetLabelRows();
	void SetLabelSizeManual(BOOL bNewValue);
	BOOL GetLabelSizeManual();
	void SetLabelShape(long nNewValue);
	long GetLabelShape();
	void SetLabelCorners(long nNewValue);
	long GetLabelCorners();
	void SetLabelHole(BOOL bNewValue);
	BOOL GetLabelHole();
	void SetPaperWidth(long units, float newValue);
	float GetPaperWidth(long units);
	void SetPaperHeight(long units, float newValue);
	float GetPaperHeight(long units);
	void SetMarginLeft(long units, float newValue);
	float GetMarginLeft(long units);
	void SetMarginRight(long units, float newValue);
	float GetMarginRight(long units);
	void SetMarginTop(long units, float newValue);
	float GetMarginTop(long units);
	void SetMarginBottom(long units, float newValue);
	float GetMarginBottom(long units);
	void SetLabelWidth(long units, float newValue);
	float GetLabelWidth(long units);
	void SetLabelHeight(long units, float newValue);
	float GetLabelHeight(long units);
	void SetLabelHoleDiameter(long units, float newValue);
	float GetLabelHoleDiameter(long units);
	BOOL GetValid();
	void SetStock(LPCTSTR StockCategory, LPCTSTR StockType);
	void SetLabelGapManual(BOOL bNewValue);
	BOOL GetLabelGapManual();
	void SetLabelGapHorizontal(long units, float newValue);
	float GetLabelGapHorizontal(long units);
	void SetLabelGapVertical(long units, float newValue);
	float GetLabelGapVertical(long units);
	void SetMirror(BOOL bNewValue);
	BOOL GetMirror();
	void SetInverse(BOOL bNewValue);
	BOOL GetInverse();
	CString GetTray();
	void SetTray(LPCTSTR lpszNewValue);
	LPDISPATCH GetMediaHandling();
};
/////////////////////////////////////////////////////////////////////////////
// IBtMediaHandling wrapper class

class IBtMediaHandling : public COleDispatchDriver
{
public:
	IBtMediaHandling() {}		// Calls COleDispatchDriver default constructor
	IBtMediaHandling(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtMediaHandling(const IBtMediaHandling& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void SetAction(long nNewValue);
	long GetAction();
	void SetOccurrence(long nNewValue);
	long GetOccurrence();
	void SetDataItemType(long nNewValue);
	long GetDataItemType();
	void SetDataItemName(LPCTSTR lpszNewValue);
	CString GetDataItemName();
	void SetNumberOfLabels(long nNewValue);
	long GetNumberOfLabels();
	void SetIgnoreCase(BOOL bNewValue);
	BOOL GetIgnoreCase();
};
/////////////////////////////////////////////////////////////////////////////
// DBtMediaHandling wrapper class

class DBtMediaHandling : public COleDispatchDriver
{
public:
	DBtMediaHandling() {}		// Calls COleDispatchDriver default constructor
	DBtMediaHandling(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtMediaHandling(const DBtMediaHandling& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetAction();
	void SetAction(long);
	long GetOccurrence();
	void SetOccurrence(long);
	long GetDataItemType();
	void SetDataItemType(long);
	CString GetDataItemName();
	void SetDataItemName(LPCTSTR);
	long GetNumberOfLabels();
	void SetNumberOfLabels(long);
	BOOL GetIgnoreCase();
	void SetIgnoreCase(BOOL);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// DBtPageSetup wrapper class

class DBtPageSetup : public COleDispatchDriver
{
public:
	DBtPageSetup() {}		// Calls COleDispatchDriver default constructor
	DBtPageSetup(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtPageSetup(const DBtPageSetup& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetStockCategory();
	void SetStockCategory(LPCTSTR);
	CString GetStockType();
	void SetStockType(LPCTSTR);
	CString GetPaperName();
	void SetPaperName(LPCTSTR);
	long GetPaperID();
	void SetPaperID(long);
	long GetOrientation();
	void SetOrientation(long);
	long GetLabelColumns();
	void SetLabelColumns(long);
	long GetLabelRows();
	void SetLabelRows(long);
	BOOL GetLabelSizeManual();
	void SetLabelSizeManual(BOOL);
	long GetLabelShape();
	void SetLabelShape(long);
	long GetLabelCorners();
	void SetLabelCorners(long);
	BOOL GetLabelHole();
	void SetLabelHole(BOOL);
	BOOL GetValid();
	void SetValid(BOOL);
	BOOL GetLabelGapManual();
	void SetLabelGapManual(BOOL);
	BOOL GetMirror();
	void SetMirror(BOOL);
	BOOL GetInverse();
	void SetInverse(BOOL);
	CString GetTray();
	void SetTray(LPCTSTR);
	LPDISPATCH GetMediaHandling();
	void SetMediaHandling(LPDISPATCH);

// Operations
public:
	float GetPaperWidth(long units);
	// method 'SetPaperWidth' not emitted because of invalid return type or parameter type
	float GetPaperHeight(long units);
	// method 'SetPaperHeight' not emitted because of invalid return type or parameter type
	float GetMarginLeft(long units);
	// method 'SetMarginLeft' not emitted because of invalid return type or parameter type
	float GetMarginRight(long units);
	// method 'SetMarginRight' not emitted because of invalid return type or parameter type
	float GetMarginTop(long units);
	// method 'SetMarginTop' not emitted because of invalid return type or parameter type
	float GetMarginBottom(long units);
	// method 'SetMarginBottom' not emitted because of invalid return type or parameter type
	float GetLabelWidth(long units);
	// method 'SetLabelWidth' not emitted because of invalid return type or parameter type
	float GetLabelHeight(long units);
	// method 'SetLabelHeight' not emitted because of invalid return type or parameter type
	float GetLabelHoleDiameter(long units);
	// method 'SetLabelHoleDiameter' not emitted because of invalid return type or parameter type
	// method 'SetStock' not emitted because of invalid return type or parameter type
	float GetLabelGapHorizontal(long units);
	// method 'SetLabelGapHorizontal' not emitted because of invalid return type or parameter type
	float GetLabelGapVertical(long units);
	// method 'SetLabelGapVertical' not emitted because of invalid return type or parameter type
};
/////////////////////////////////////////////////////////////////////////////
// IBtPrintSetup wrapper class

class IBtPrintSetup : public COleDispatchDriver
{
public:
	IBtPrintSetup() {}		// Calls COleDispatchDriver default constructor
	IBtPrintSetup(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtPrintSetup(const IBtPrintSetup& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetPerformance();
	LPDISPATCH GetCache();
	LPDISPATCH GetPrintMethod();
	void SetAutoPrintAgain(BOOL bNewValue);
	BOOL GetAutoPrintAgain();
	void SetIdenticalCopiesOfLabel(long nNewValue);
	long GetIdenticalCopiesOfLabel();
	void SetNumberSerializedLabels(long nNewValue);
	long GetNumberSerializedLabels();
	void SetRefreshDatabases(BOOL bNewValue);
	BOOL GetRefreshDatabases();
	void SetLogPrintJob(BOOL bNewValue);
	BOOL GetLogPrintJob();
	CString GetPrinter();
	void SetPrintToFile(BOOL bNewValue);
	BOOL GetPrintToFile();
	void SetUseDatabase(BOOL bNewValue);
	BOOL GetUseDatabase();
	void SetPrinter(LPCTSTR lpszNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// IBtPerformance wrapper class

class IBtPerformance : public COleDispatchDriver
{
public:
	IBtPerformance() {}		// Calls COleDispatchDriver default constructor
	IBtPerformance(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtPerformance(const IBtPerformance& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void SetAllowStaticGraphics(BOOL bNewValue);
	BOOL GetAllowStaticGraphics();
	void SetAllowStaticObjects(BOOL bNewValue);
	BOOL GetAllowStaticObjects();
	void SetAllowSerialization(BOOL bNewValue);
	BOOL GetAllowSerialization();
	void SetAllowVariableDataOptimization(BOOL bNewValue);
	BOOL GetAllowVariableDataOptimization();
	void SetAllowGraphicsCaching(BOOL bNewValue);
	BOOL GetAllowGraphicsCaching();
	void SetAllowFormatCaching(BOOL bNewValue);
	BOOL GetAllowFormatCaching();
	void SetWarnWhenUsingTrueTypeFonts(BOOL bNewValue);
	BOOL GetWarnWhenUsingTrueTypeFonts();
};
/////////////////////////////////////////////////////////////////////////////
// DBtPerformance wrapper class

class DBtPerformance : public COleDispatchDriver
{
public:
	DBtPerformance() {}		// Calls COleDispatchDriver default constructor
	DBtPerformance(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtPerformance(const DBtPerformance& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	BOOL GetAllowStaticGraphics();
	void SetAllowStaticGraphics(BOOL);
	BOOL GetAllowStaticObjects();
	void SetAllowStaticObjects(BOOL);
	BOOL GetAllowSerialization();
	void SetAllowSerialization(BOOL);
	BOOL GetAllowVariableDataOptimization();
	void SetAllowVariableDataOptimization(BOOL);
	BOOL GetAllowGraphicsCaching();
	void SetAllowGraphicsCaching(BOOL);
	BOOL GetAllowFormatCaching();
	void SetAllowFormatCaching(BOOL);
	BOOL GetWarnWhenUsingTrueTypeFonts();
	void SetWarnWhenUsingTrueTypeFonts(BOOL);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// IBtCache wrapper class

class IBtCache : public COleDispatchDriver
{
public:
	IBtCache() {}		// Calls COleDispatchDriver default constructor
	IBtCache(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtCache(const IBtCache& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void SetFlushInterval(long nNewValue);
	long GetFlushInterval();
	void Flush();
};
/////////////////////////////////////////////////////////////////////////////
// DBtCache wrapper class

class DBtCache : public COleDispatchDriver
{
public:
	DBtCache() {}		// Calls COleDispatchDriver default constructor
	DBtCache(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtCache(const DBtCache& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetFlushInterval();
	void SetFlushInterval(long);

// Operations
public:
	void Flush();
};
/////////////////////////////////////////////////////////////////////////////
// IBtPrintMethod wrapper class

class IBtPrintMethod : public COleDispatchDriver
{
public:
	IBtPrintMethod() {}		// Calls COleDispatchDriver default constructor
	IBtPrintMethod(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtPrintMethod(const IBtPrintMethod& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void SetTrueTypeText(long nNewValue);
	long GetTrueTypeText();
	void SetUnsupported1DBarCodes(long nNewValue);
	long GetUnsupported1DBarCodes();
	void SetUnsupported2DBarCodes(long nNewValue);
	long GetUnsupported2DBarCodes();
	void SetLines(long nNewValue);
	long GetLines();
	void SetBoxes(long nNewValue);
	long GetBoxes();
	void SetEllipses(long nNewValue);
	long GetEllipses();
};
/////////////////////////////////////////////////////////////////////////////
// DBtPrintMethod wrapper class

class DBtPrintMethod : public COleDispatchDriver
{
public:
	DBtPrintMethod() {}		// Calls COleDispatchDriver default constructor
	DBtPrintMethod(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtPrintMethod(const DBtPrintMethod& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetTrueTypeText();
	void SetTrueTypeText(long);
	long GetUnsupported1DBarCodes();
	void SetUnsupported1DBarCodes(long);
	long GetUnsupported2DBarCodes();
	void SetUnsupported2DBarCodes(long);
	long GetLines();
	void SetLines(long);
	long GetBoxes();
	void SetBoxes(long);
	long GetEllipses();
	void SetEllipses(long);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// DBtPrintSetup wrapper class

class DBtPrintSetup : public COleDispatchDriver
{
public:
	DBtPrintSetup() {}		// Calls COleDispatchDriver default constructor
	DBtPrintSetup(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtPrintSetup(const DBtPrintSetup& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetPerformance();
	void SetPerformance(LPDISPATCH);
	LPDISPATCH GetCache();
	void SetCache(LPDISPATCH);
	LPDISPATCH GetPrintMethod();
	void SetPrintMethod(LPDISPATCH);
	BOOL GetAutoPrintAgain();
	void SetAutoPrintAgain(BOOL);
	long GetIdenticalCopiesOfLabel();
	void SetIdenticalCopiesOfLabel(long);
	long GetNumberSerializedLabels();
	void SetNumberSerializedLabels(long);
	CString GetPrinter();
	void SetPrinter(LPCTSTR);
	BOOL GetPrintToFile();
	void SetPrintToFile(BOOL);
	BOOL GetUseDatabase();
	void SetUseDatabase(BOOL);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// IBtPrinterCodeTemplate wrapper class

class IBtPrinterCodeTemplate : public COleDispatchDriver
{
public:
	IBtPrinterCodeTemplate() {}		// Calls COleDispatchDriver default constructor
	IBtPrinterCodeTemplate(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtPrinterCodeTemplate(const IBtPrinterCodeTemplate& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetPrintMethod();
	LPDISPATCH GetPerformance();
	LPDISPATCH GetJob();
	BOOL Export(LPCTSTR PrintSystemName, long ExportType, LPCTSTR FormatOrCombinedExportPath, LPCTSTR DataExportPath, LPDISPATCH* Messages);
	void SetExportAsVersion(long nNewValue);
	long GetExportAsVersion();
};
/////////////////////////////////////////////////////////////////////////////
// IBtJob wrapper class

class IBtJob : public COleDispatchDriver
{
public:
	IBtJob() {}		// Calls COleDispatchDriver default constructor
	IBtJob(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtJob(const IBtJob& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void SetFormatIdType(long nNewValue);
	long GetFormatIdType();
	void SetFormatId(LPCTSTR lpszNewValue);
	CString GetFormatId();
	void SetPrinterCopiesType(long nNewValue);
	long GetPrinterCopiesType();
	void SetPrinterCopies(LPCTSTR lpszNewValue);
	CString GetPrinterCopies();
	void SetPrinterSerializedCopiesType(long nNewValue);
	long GetPrinterSerializedCopiesType();
	void SetPrinterSerializedCopies(LPCTSTR lpszNewValue);
	CString GetPrinterSerializedCopies();
	void SetPrinterBatchCopiesType(long nNewValue);
	long GetPrinterBatchCopiesType();
	void SetPrinterBatchCopies(LPCTSTR lpszNewValue);
	CString GetPrinterBatchCopies();
};
/////////////////////////////////////////////////////////////////////////////
// DBtJob wrapper class

class DBtJob : public COleDispatchDriver
{
public:
	DBtJob() {}		// Calls COleDispatchDriver default constructor
	DBtJob(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtJob(const DBtJob& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetFormatIdValueType();
	void SetFormatIdValueType(long);
	CString GetFormatIdValue();
	void SetFormatIdValue(LPCTSTR);
	long GetPrinterCopiesValueType();
	void SetPrinterCopiesValueType(long);
	CString GetPrinterCopiesValue();
	void SetPrinterCopiesValue(LPCTSTR);
	long GetPrinterSerializedCopiesValueType();
	void SetPrinterSerializedCopiesValueType(long);
	CString GetPrinterSerializedCopiesValue();
	void SetPrinterSerializedCopiesValue(LPCTSTR);
	long GetPrinterBatchCopiesValueType();
	void SetPrinterBatchCopiesValueType(long);
	CString GetPrinterBatchCopiesValue();
	void SetPrinterBatchCopiesValue(LPCTSTR);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// DBtPrinterCodeTemplate wrapper class

class DBtPrinterCodeTemplate : public COleDispatchDriver
{
public:
	DBtPrinterCodeTemplate() {}		// Calls COleDispatchDriver default constructor
	DBtPrinterCodeTemplate(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtPrinterCodeTemplate(const DBtPrinterCodeTemplate& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetPrintMethod();
	void SetPrintMethod(LPDISPATCH);
	LPDISPATCH GetPerformance();
	void SetPerformance(LPDISPATCH);
	LPDISPATCH GetJob();
	void SetJob(LPDISPATCH);
	long GetExportAsVersion();
	void SetExportAsVersion(long);

// Operations
public:
	BOOL Export(LPCTSTR PrintSystemName, long ExportType, LPCTSTR FormatOrCombinedExportPath, LPCTSTR DataExportPath, LPDISPATCH* Messages);
};
/////////////////////////////////////////////////////////////////////////////
// DBtFormat wrapper class

class DBtFormat : public COleDispatchDriver
{
public:
	DBtFormat() {}		// Calls COleDispatchDriver default constructor
	DBtFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtFormat(const DBtFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	BOOL GetAutoPrintAgain();
	void SetAutoPrintAgain(BOOL);
	CString GetBaseName();
	void SetBaseName(LPCTSTR);
	CString GetDirectory();
	void SetDirectory(LPCTSTR);
	CString GetFileName();
	void SetFileName(LPCTSTR);
	long GetIdenticalCopiesOfLabel();
	void SetIdenticalCopiesOfLabel(long);
	LPDISPATCH GetInputDataFileSetup();
	void SetInputDataFileSetup(LPDISPATCH);
	long GetNumberSerializedLabels();
	void SetNumberSerializedLabels(long);
	CString GetPrinter();
	void SetPrinter(LPCTSTR);
	BOOL GetPrintToFile();
	void SetPrintToFile(BOOL);
	CString GetTitle();
	void SetTitle(LPCTSTR);
	BOOL GetUseInputDataFile();
	void SetUseInputDataFile(BOOL);
	long GetSaveAsVersion();
	void SetSaveAsVersion(long);
	LPDISPATCH GetNamedSubStrings();
	void SetNamedSubStrings(LPDISPATCH);
	LPDISPATCH GetDatabases();
	void SetDatabases(LPDISPATCH);
	BOOL GetEnablePrompting();
	void SetEnablePrompting(BOOL);
	CString GetRecordRange();
	void SetRecordRange(LPCTSTR);
	BOOL GetSelectRecordsAtPrint();
	void SetSelectRecordsAtPrint(BOOL);
	BOOL GetUseDatabase();
	void SetUseDatabase(BOOL);
	CString GetPrinterFile();
	void SetPrinterFile(LPCTSTR);
	LPDISPATCH GetPageSetup();
	void SetPageSetup(LPDISPATCH);
	LPDISPATCH GetPrintSetup();
	void SetPrintSetup(LPDISPATCH);
	LPDISPATCH GetPrinterCodeTemplate();
	void SetPrinterCodeTemplate(LPDISPATCH);
	CString GetUsedNames();
	void SetUsedNames(LPCTSTR);
	CString GetUsedNamesSampleData();
	void SetUsedNamesSampleData(LPCTSTR);
	CString GetJobName();
	void SetJobName(LPCTSTR);
	CString GetPrintToFileLicense();
	void SetPrintToFileLicense(LPCTSTR);
	CString GetPaperTray();
	void SetPaperTray(LPCTSTR);
	CString GetComment();
	void SetComment(LPCTSTR);
	long GetEncryption();
	void SetEncryption(long);
	BOOL GetSupportsSetSerializedCopies();
	void SetSupportsSetSerializedCopies(BOOL);
	BOOL GetSupportsSetIdenticalCopies();
	void SetSupportsSetIdenticalCopies(BOOL);

// Operations
public:
	void Activate();
	void Close(long SaveChanges);
	void PrintOut(BOOL ShowStatusWindow, BOOL ShowPrintDialog);
	void Save();
	void SaveAs(LPCTSTR FileName, BOOL OverwriteIfExists);
	void ExportToClipboard(long Colors, long Resolution);
	void ExportToFile(LPCTSTR FileName, LPCTSTR FileType, long Colors, long Resolution, long SaveOption);
	void SetAvailableTemplateFieldNames(LPCTSTR FieldnameSampleDataList, LPCTSTR delimiter);
	CString ExportPromptToXML(LPCTSTR XSLFileName);
	void SetPrompt(LPCTSTR PromptName, LPCTSTR PromptData);
	long Print(LPCTSTR PrintJobName, BOOL WaitForSpoolJobToComplete, long TimeoutMs, LPDISPATCH* Messages);
	long ExportPrintPreviewToImage(LPCTSTR Directory, LPCTSTR FileNameTemplate, LPCTSTR FileType, long Colors, long dpi, long backgroundColor, long SaveOpts, BOOL IncludeMargins, BOOL IncludeBorders, LPDISPATCH* Messages);
};
/////////////////////////////////////////////////////////////////////////////
// IFormatEvents wrapper class

class IFormatEvents : public COleDispatchDriver
{
public:
	IFormatEvents() {}		// Calls COleDispatchDriver default constructor
	IFormatEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IFormatEvents(const IFormatEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void OnNewPrintJob(LPCTSTR PrinterName, long JobID);
};
/////////////////////////////////////////////////////////////////////////////
// IBtFormats wrapper class

class IBtFormats : public COleDispatchDriver
{
public:
	IBtFormats() {}		// Calls COleDispatchDriver default constructor
	IBtFormats(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtFormats(const IBtFormats& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetCount();
	LPDISPATCH Add();
	LPDISPATCH Item(VARIANT* Index);
	LPDISPATCH Open(LPCTSTR FileName, BOOL CloseOutFirstFormat, LPCTSTR UsePrinter);
	LPDISPATCH GetFormat(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// DBtFormats wrapper class

class DBtFormats : public COleDispatchDriver
{
public:
	DBtFormats() {}		// Calls COleDispatchDriver default constructor
	DBtFormats(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtFormats(const DBtFormats& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetCount();
	void SetCount(long);

// Operations
public:
	LPDISPATCH Add();
	LPDISPATCH Open(LPCTSTR FileName, BOOL CloseOutFirstFormat, LPCTSTR UsePrinter);
	LPDISPATCH Item(VARIANT* Index);
	LPDISPATCH GetFormat(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// IBtWindow wrapper class

class IBtWindow : public COleDispatchDriver
{
public:
	IBtWindow() {}		// Calls COleDispatchDriver default constructor
	IBtWindow(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtWindow(const IBtWindow& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void SetLeft(long nNewValue);
	long GetLeft();
	void SetTop(long nNewValue);
	long GetTop();
	void SetWidth(long nNewValue);
	long GetWidth();
	void SetHeight(long nNewValue);
	long GetHeight();
	void SetWindowState(long nNewValue);
	long GetWindowState();
};
/////////////////////////////////////////////////////////////////////////////
// DBtWindow wrapper class

class DBtWindow : public COleDispatchDriver
{
public:
	DBtWindow() {}		// Calls COleDispatchDriver default constructor
	DBtWindow(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtWindow(const DBtWindow& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetLeft();
	void SetLeft(long);
	long GetTop();
	void SetTop(long);
	long GetWidth();
	void SetWidth(long);
	long GetHeight();
	void SetHeight(long);
	long GetWindowState();
	void SetWindowState(long);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// IBtLicenseServer wrapper class

class IBtLicenseServer : public COleDispatchDriver
{
public:
	IBtLicenseServer() {}		// Calls COleDispatchDriver default constructor
	IBtLicenseServer(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBtLicenseServer(const IBtLicenseServer& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void SetAddress(LPCTSTR lpszNewValue);
	CString GetAddress();
	void SetPort(long nNewValue);
	long GetPort();
	void SetTimeout(long nNewValue);
	long GetTimeout();
	BOOL GetIsConnected();
	BOOL Connect();
	void SetAlternateAddress(LPCTSTR lpszNewValue);
	CString GetAlternateAddress();
	void SetAlternatePort(long nNewValue);
	long GetAlternatePort();
	void SetAlternateTimeout(long nNewValue);
	long GetAlternateTimeout();
	long GetMaxAllowedPrinters();
	long GetMaxAllowedUsers();
	void SetRetries(long nNewValue);
	long GetRetries();
	void SetAlternateRetries(long nNewValue);
	long GetAlternateRetries();
};
/////////////////////////////////////////////////////////////////////////////
// DBtLicenseServer wrapper class

class DBtLicenseServer : public COleDispatchDriver
{
public:
	DBtLicenseServer() {}		// Calls COleDispatchDriver default constructor
	DBtLicenseServer(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DBtLicenseServer(const DBtLicenseServer& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetAddress();
	void SetAddress(LPCTSTR);
	CString GetAlternateAddress();
	void SetAlternateAddress(LPCTSTR);
	long GetPort();
	void SetPort(long);
	long GetAlternatePort();
	void SetAlternatePort(long);
	long GetTimeout();
	void SetTimeout(long);
	long GetAlternateTimeout();
	void SetAlternateTimeout(long);
	BOOL GetIsConnected();
	void SetIsConnected(BOOL);
	long GetMaxAllowedPrinters();
	void SetMaxAllowedPrinters(long);
	long GetMaxAllowedUsers();
	void SetMaxAllowedUsers(long);
	long GetRetries();
	void SetRetries(long);
	long GetAlternateRetries();
	void SetAlternateRetries(long);

// Operations
public:
	BOOL Connect();
};
/////////////////////////////////////////////////////////////////////////////
// IApplicationEvents wrapper class

class IApplicationEvents : public COleDispatchDriver
{
public:
	IApplicationEvents() {}		// Calls COleDispatchDriver default constructor
	IApplicationEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IApplicationEvents(const IApplicationEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void OnCommandLineCompleted(LPCTSTR ResponseString, LPDISPATCH* Messages, LPCTSTR CommandLine);
	void OnNewPrintJob(LPCTSTR PrinterName, long JobID);
};
/////////////////////////////////////////////////////////////////////////////
// IScreenDs wrapper class

class IScreenDs : public COleDispatchDriver
{
public:
	IScreenDs() {}		// Calls COleDispatchDriver default constructor
	IScreenDs(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IScreenDs(const IScreenDs& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetName();
	void SetName(LPCTSTR);
	CString GetValue();
	void SetValue(LPCTSTR);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// ITemplateFieldDs wrapper class

class ITemplateFieldDs : public COleDispatchDriver
{
public:
	ITemplateFieldDs() {}		// Calls COleDispatchDriver default constructor
	ITemplateFieldDs(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ITemplateFieldDs(const ITemplateFieldDs& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetName();
	void SetName(LPCTSTR);
	CString GetValue();
	void SetValue(LPCTSTR);
	CString GetFieldName();
	void SetFieldName(LPCTSTR);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// ILabelObjectDs wrapper class

class ILabelObjectDs : public COleDispatchDriver
{
public:
	ILabelObjectDs() {}		// Calls COleDispatchDriver default constructor
	ILabelObjectDs(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ILabelObjectDs(const ILabelObjectDs& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetObjectName();
	void SetObjectName(LPCTSTR);
	CString GetValue();
	void SetValue(LPCTSTR);

// Operations
public:
};
