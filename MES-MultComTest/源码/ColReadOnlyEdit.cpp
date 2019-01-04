// ColReadOnlyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "WriteIMEI.h"
#include "ColReadOnlyEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColReadOnlyEdit

CColReadOnlyEdit::CColReadOnlyEdit()
{
}

CColReadOnlyEdit::~CColReadOnlyEdit()
{
}


BEGIN_MESSAGE_MAP(CColReadOnlyEdit, CEdit)
	//{{AFX_MSG_MAP(CColReadOnlyEdit)
	ON_WM_RBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColReadOnlyEdit message handlers

BOOL CColReadOnlyEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
		return TRUE;

	return CEdit::PreTranslateMessage(pMsg);
}

void CColReadOnlyEdit::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	return;

	CEdit::OnRButtonDown(nFlags, point);
}

void CColReadOnlyEdit::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
    // Delete the old brush

	HBRUSH mBrush= NULL;//»­Ë¢
	extern COLORREF gColor;
	extern CString gFinalResult;


	//set the final result box font
	CFont* font;
	font = new CFont;
	font->CreateFont(78,0,0,0,700,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE,
		"arial"); 
	// Cause the label to use the new font
	//pDC->SetFont(font);

    //gBrush.DeleteObject();
    mBrush=CreateSolidBrush(RGB(255,255,255));//RGB(212,208,200));
    CDC* pDC = GetDC();
    pDC->SetBkMode(OPAQUE);
    pDC->SetBkColor(RGB(255,255,255));
    pDC->SelectObject(&mBrush);
    CRect rc;
    GetClientRect(&rc);
    ScreenToClient(&rc);
    pDC->Rectangle(0, 0, rc.Width(), rc.Height());
    pDC->SetTextColor(gColor);
	
	//pDC->SetTextAlign(TA_LEFT);
	pDC->SelectObject(font);

    pDC->TextOut(1, 1, gFinalResult.GetBuffer(gFinalResult.GetLength()));

	// Do not call CEdit::OnPaint() for painting messages
}

void CColReadOnlyEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	return;

	CEdit::OnLButtonDown(nFlags, point);
}
