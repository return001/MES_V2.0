// WriteIMEI.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "WriteIMEI.h"
#include "WriteIMEIDlg.h"

#include "Login3.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWriteIMEIApp

BEGIN_MESSAGE_MAP(CWriteIMEIApp, CWinApp)
	//{{AFX_MSG_MAP(CWriteIMEIApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWriteIMEIApp construction

CWriteIMEIApp::CWriteIMEIApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWriteIMEIApp object

CWriteIMEIApp theApp;
CString gFinalResult;
COLORREF gColor;

/////////////////////////////////////////////////////////////////////////////
// CWriteIMEIApp initialization

BOOL CWriteIMEIApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox("初始化SOCKET错误!");
		return FALSE;
	}
	AfxEnableControlContainer();

	if(!AfxOleInit())
	{
		AfxMessageBox("初始化OLE错误!");
		return FALSE;
	}


	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CLogin3 dig3;
	if (dig3.DoModal() != IDOK)
	{
		exit(0);
		return FALSE;
	}
	
	CWriteIMEIDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
