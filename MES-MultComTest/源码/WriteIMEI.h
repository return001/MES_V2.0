// WriteIMEI.h : main header file for the WRITEIMEI application
//

#if !defined(AFX_WRITEIMEI_H__C359537B_778A_48D5_A773_49001ED2F095__INCLUDED_)
#define AFX_WRITEIMEI_H__C359537B_778A_48D5_A773_49001ED2F095__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWriteIMEIApp:
// See WriteIMEI.cpp for the implementation of this class
//

class CWriteIMEIApp : public CWinApp
{
public:
	CWriteIMEIApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWriteIMEIApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWriteIMEIApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WRITEIMEI_H__C359537B_778A_48D5_A773_49001ED2F095__INCLUDED_)
