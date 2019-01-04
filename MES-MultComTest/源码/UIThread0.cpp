// UIThread.cpp : implementation file
//

#include "stdafx.h"
#include "WriteIMEI.h"
#include "UIThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUIThread

IMPLEMENT_DYNCREATE(CUIThread, CWinThread)

CUIThread::CUIThread()
{
}

CUIThread::~CUIThread()
{
}

BOOL CUIThread::InitInstance()//界面线程
{	
	/*if((UI_Count==0)||(UI_Count>5))
		UI_Count=1;
	else if(UI_Count==5)
		UI_Count=0;
	else
		UI_Count++;
	if((UI_Count!=5)&&(UI_Count!=0))
	{
		CRect rs;
		rs.top=50+((UI_Count-1)/2)*350; 
		rs.bottom=390+((UI_Count-1)/2)*350; 
		rs.left=130+((UI_Count-1)%2)*550; 
		rs.right=650+((UI_Count-1)%2)*550;
		m_dlg.Create(IDD_CSR_test);
		m_dlg.MoveWindow(&rs);
		m_dlg.ShowWindow(SW_SHOW);
		m_pMainWnd=&m_dlg;
	}*/

	m_dlg.Create(IDD_CSR_test);
	m_dlg.ShowWindow(SW_SHOW);
	m_pMainWnd=&m_dlg;
	return TRUE;
}

int CUIThread::ExitInstance()//界面线程
{
	m_dlg.DestroyWindow();
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CUIThread, CWinThread)
	//{{AFX_MSG_MAP(CUIThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUIThread message handlers
