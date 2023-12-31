
// MFCApplicationDrawTestDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MFCApplicationDrawTest.h"
#include "MFCApplicationDrawTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplicationDrawTestDlg 对话框



CMFCApplicationDrawTestDlg::CMFCApplicationDrawTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MFCAPPLICATIONDRAWTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplicationDrawTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplicationDrawTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_BASIC, &CMFCApplicationDrawTestDlg::OnBnClickedButtonBasic)
	ON_BN_CLICKED(IDC_BUTTON_MORE, &CMFCApplicationDrawTestDlg::OnBnClickedButtonMore)
	ON_BN_CLICKED(IDC_BUTTON_DATA, &CMFCApplicationDrawTestDlg::OnBnClickedButtonData)
END_MESSAGE_MAP()


// CMFCApplicationDrawTestDlg 消息处理程序

BOOL CMFCApplicationDrawTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplicationDrawTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplicationDrawTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplicationDrawTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CMFCApplicationDrawTestDlg::OnBnClickedButtonBasic()
{
	//获取句柄
	CClientDC *pDC = new CClientDC(this);
	//设置坐标映射模式，具体信息请察看MSDN
	pDC->SetMapMode(MM_TEXT);
	//绘制一个矩形区域
	pDC->Rectangle(12, 12, 402, 302);
	pDC->Rectangle(11, 11, 401, 301);
	//再绘制一个矩形区域，制造立体效果
	pDC->Rectangle(10, 10, 400, 300);
	//删除句柄
	delete pDC;
}


void CMFCApplicationDrawTestDlg::OnBnClickedButtonMore()
{
	//获取句柄
	CClientDC *pDC = new CClientDC(this);
	//设置坐标映射模式，具体信息请察看MSDN
	pDC->SetMapMode(MM_TEXT);
	//绘制一条直线
	pDC->MoveTo(15, 295);
	pDC->LineTo(395, 295);
	//设置文本的颜色，0x00bbggrr 格式表示的RGB颜色
	pDC->SetTextColor(0x00ff0000);
	//输出文本
	pDC->TextOut(15, 15, L"Hello EEIS! This is the First!");
	//删除句柄
	delete pDC;
}


void CMFCApplicationDrawTestDlg::OnBnClickedButtonData()
{
	float iDataToPlot[10];
	int i;
	for (i = 0; i<10; i++)
		iDataToPlot[i] = ((i>5) ? (float)i - 4 : (float)i) * 10;

	//归一化数据
	float iMaxDataToPlot = 0.0;
	for (i = 0; i<10; i++)
		if (fabs(iDataToPlot[i]) > fabs(iMaxDataToPlot))
			iMaxDataToPlot = iDataToPlot[i];
	for (i = 0; i<10; i++)
		iDataToPlot[i] = iDataToPlot[i] / iMaxDataToPlot * 260;
	

	//获取句柄
	CClientDC *pDC = new CClientDC(this);
	CPen pen;
	pen.CreatePen(0, 1, RGB(255, 0, 255));
	CPen* pOldPen = (CPen*)pDC->SelectObject(&pen);
	//设置坐标映射模式，具体信息请察看MSDN
	pDC->SetMapMode(MM_TEXT);
	for (i = 0; i<10; i++)
	{
		if(i > 5) 	pDC->SelectObject(pOldPen);
		pDC->MoveTo(15 + i * 10, 295);
		pDC->LineTo(15 + i * 10, 295 - (int)iDataToPlot[i]);
	}
	//删除句柄
	delete pDC;
}
