#if !defined(AFX_COLREADONLYEDIT_H__704AE313_0415_4E1B_B13C_1435C3913DF5__INCLUDED_)
#define AFX_COLREADONLYEDIT_H__704AE313_0415_4E1B_B13C_1435C3913DF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColReadOnlyEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColReadOnlyEdit window

class CColReadOnlyEdit : public CEdit
{
// Construction
public:
	CColReadOnlyEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColReadOnlyEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColReadOnlyEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColReadOnlyEdit)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLREADONLYEDIT_H__704AE313_0415_4E1B_B13C_1435C3913DF5__INCLUDED_)
