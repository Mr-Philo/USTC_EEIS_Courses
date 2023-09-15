// SimpleDrawDlgDlg.h : header file
//

#if !defined(AFX_SIMPLEDRAWDLGDLG_H__E279EF0F_A782_4E9D_BEBD_81B48AAC7193__INCLUDED_)
#define AFX_SIMPLEDRAWDLGDLG_H__E279EF0F_A782_4E9D_BEBD_81B48AAC7193__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSimpleDrawDlgDlg dialog

class CSimpleDrawDlgDlg : public CDialog
{
// Construction
public:
	CSimpleDrawDlgDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSimpleDrawDlgDlg)
	enum { IDD = IDD_SIMPLEDRAWDLG_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimpleDrawDlgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSimpleDrawDlgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonBasic();
	afx_msg void OnButtonColor();
	afx_msg void OnButtonData();
	afx_msg void OnButtonMore();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMPLEDRAWDLGDLG_H__E279EF0F_A782_4E9D_BEBD_81B48AAC7193__INCLUDED_)
