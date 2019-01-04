; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CColReadOnlyEdit
LastTemplate=CEdit
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "WriteIMEI.h"

ClassCount=3
Class1=CWriteIMEIApp
Class2=CWriteIMEIDlg

ResourceCount=3
Resource2=IDD_WRITEIMEI_DIALOG
Resource1=IDR_MAINFRAME
Class3=CColReadOnlyEdit
Resource3=IDD_WRITEIMEI_DIALOG (English (U.S.))

[CLS:CWriteIMEIApp]
Type=0
HeaderFile=WriteIMEI.h
ImplementationFile=WriteIMEI.cpp
Filter=N

[CLS:CWriteIMEIDlg]
Type=0
HeaderFile=WriteIMEIDlg.h
ImplementationFile=WriteIMEIDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_AUTO



[DLG:IDD_WRITEIMEI_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CWriteIMEIDlg

[DLG:IDD_WRITEIMEI_DIALOG (English (U.S.))]
Type=1
Class=CWriteIMEIDlg
ControlCount=14
Control1=IDC_IMEI,edit,1350631552
Control2=IDC_WRITE,button,1342242817
Control3=IDOK,button,1342242816
Control4=IDC_PORT,combobox,1344339971
Control5=IDC_BAUD,combobox,1344339971
Control6=IDC_TYPE,combobox,1344339971
Control7=IDC_RESULT,edit,1352730692
Control8=IDC_FINALRESULT,edit,1350567937
Control9=IDC_STATIC_TYPE,static,1342308353
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_AUTO,combobox,1344339971

[CLS:CColReadOnlyEdit]
Type=0
HeaderFile=ColReadOnlyEdit.h
ImplementationFile=ColReadOnlyEdit.cpp
BaseClass=CEdit
Filter=W
VirtualFilter=WC
LastObject=CColReadOnlyEdit

