// SimpleDrawDlg.h : main header file for the SIMPLEDRAWDLG application
//

#if !defined(AFX_SIMPLEDRAWDLG_H__642E81F6_DD01_4A24_8B89_D6A318BEC000__INCLUDED_)
#define AFX_SIMPLEDRAWDLG_H__642E81F6_DD01_4A24_8B89_D6A318BEC000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSimpleDrawDlgApp:
// See SimpleDrawDlg.cpp for the implementation of this class
//

class CSimpleDrawDlgApp : public CWinApp
{
public:
	CSimpleDrawDlgApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimpleDrawDlgApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSimpleDrawDlgApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMPLEDRAWDLG_H__642E81F6_DD01_4A24_8B89_D6A318BEC000__INCLUDED_)
