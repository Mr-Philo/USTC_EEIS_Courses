// SimpleDrawDlgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SimpleDrawDlg.h"
#include "SimpleDrawDlgDlg.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimpleDrawDlgDlg dialog

CSimpleDrawDlgDlg::CSimpleDrawDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSimpleDrawDlgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSimpleDrawDlgDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSimpleDrawDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimpleDrawDlgDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSimpleDrawDlgDlg, CDialog)
	//{{AFX_MSG_MAP(CSimpleDrawDlgDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_BASIC, OnButtonBasic)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, OnButtonColor)
	ON_BN_CLICKED(IDC_BUTTON_DATA, OnButtonData)
	ON_BN_CLICKED(IDC_BUTTON_MORE, OnButtonMore)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimpleDrawDlgDlg message handlers

BOOL CSimpleDrawDlgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSimpleDrawDlgDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSimpleDrawDlgDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSimpleDrawDlgDlg::OnButtonBasic() 
{
	//获取句柄
	CClientDC *pDC=new CClientDC(this);
	//设置坐标映射模式，具体信息请察看MSDN
	pDC->SetMapMode(MM_TEXT);	
	//绘制一个矩形区域
	pDC->Rectangle(12,12,402,302);
	pDC->Rectangle(11,11,401,301);
	//再绘制一个矩形区域，制造立体效果
	pDC->Rectangle(10,10,400,300);
	//删除句柄
	delete pDC;	
}

void CSimpleDrawDlgDlg::OnButtonMore() 
{
	//获取句柄
	CClientDC *pDC=new CClientDC(this);
	//设置坐标映射模式，具体信息请察看MSDN
	pDC->SetMapMode(MM_TEXT);	
	//绘制一条直线
	pDC->MoveTo(15,295);
	pDC->LineTo(395,295);
	//设置文本的颜色，0x00bbggrr 格式表示的RGB颜色
	pDC->SetTextColor(0x00ff0000);
	//输出文本
	pDC->TextOut(15,15,"Hello EEIS! This is the First!");
	//删除句柄
	delete pDC;	
}

void CSimpleDrawDlgDlg::OnButtonData() 
{
	float iDataToPlot[10];
	int i;
	for(i=0;i<10;i++) 
		iDataToPlot[i] = ( (i>5) ? (float)i-4 : (float)i ) * 10;

	//归一化数据
	float iMaxDataToPlot = 0.0;
	for(i=0;i<10;i++) 
		if(fabs(iDataToPlot[i]) > fabs(iMaxDataToPlot))
			iMaxDataToPlot = iDataToPlot[i];
	for(i=0;i<10;i++) 
		iDataToPlot[i] = iDataToPlot[i] / iMaxDataToPlot * 260;

	//获取句柄
	CClientDC *pDC=new CClientDC(this);
	//设置坐标映射模式，具体信息请察看MSDN
	pDC->SetMapMode(MM_TEXT);	
	for(i=0;i<10;i++)
	{
		pDC->MoveTo(15+i*10,295);
		pDC->LineTo(15+i*10,295-(int)iDataToPlot[i]);
	}
	//删除句柄
	delete pDC;	
}

void CSimpleDrawDlgDlg::OnButtonColor() 
{
	// TODO: Add your control notification handler code here
	
}

