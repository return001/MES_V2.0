// LibincStory.cpp : 实现文件
//

#include "stdafx.h"
#include "WriteIMEI.h"
#include "LibincStory.h"


// CLibincStory 对话框

IMPLEMENT_DYNAMIC(CLibincStory, CDialog)

CLibincStory::CLibincStory(CWnd* pParent /*=NULL*/)
	: CDialog(CLibincStory::IDD, pParent)
{
	m_hIcon_Story = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CLibincStory::~CLibincStory()
{
}

void CLibincStory::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Story_GIF, Story_Show);
}


BEGIN_MESSAGE_MAP(CLibincStory, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()//改变颜色
END_MESSAGE_MAP()


// CLibincStory 消息处理程序
BOOL CLibincStory::OnInitDialog()
{
	CDialog::OnInitDialog();
	::SetWindowPos( m_hWnd,HWND_TOPMOST,500,110,0,0,SWP_NOSIZE);

	if (Story_Show.Load(MAKEINTRESOURCE(IDR_GIFStory),_T("Gif")))
		Story_Show.Draw();
	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CLibincStory::OnPaint() 
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
		dc.DrawIcon(x, y, m_hIcon_Story);
	}
	else
	{
		CRect rect; 
		CPaintDC dc(this); 
		GetClientRect(rect); 
		dc.FillSolidRect(rect,RGB(200,217,100)); 
		CDialog::OnPaint();
	}
}

HBRUSH CLibincStory::OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor)//改变颜色
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC,pWnd,nCtlColor);
	switch(pWnd->GetDlgCtrlID())
	{
	case(IDC_EDIT2):
		pDC->SetTextColor(RGB(71,162,243));
		pDC->SetBkColor(RGB(215,227,145));
		break;
	}
	return hbr;

}