
// ThumbnailBoxDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ThumbnailBoxDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CThumbnailBoxDlg 对话框




CThumbnailBoxDlg::CThumbnailBoxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThumbnailBoxDlg::IDD, pParent)
{
}

void CThumbnailBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CThumbnailBoxDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_WM_TIMER()   //定时器
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CThumbnailBoxDlg 消息处理程序

BOOL CThumbnailBoxDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CRect rcClient;
	GetClientRect(rcClient);
	CThumbnailBox* pThumbnailBox = new CThumbnailBox;
	/*for(int i = 1; i <= 1; i++)
	{
		TCHAR lpszImageName[64] = {0};
		//_stprintf(lpszImageName, TEXT("image\\%d (2).jpg"), i);//1 (2)
		//cstring str=_T("hello");
		//当是多字节的时候 它等效为 cstring str="hello";
		//当为unicode时候，它等效为cstring str=L"hello";
		if(pThumbnailBox->AddImage(_T(UnionPicPath))==FALSE)
		{
			_stprintf(lpszImageName, TEXT("image\\NULL.jpg"));
			pThumbnailBox->AddImage(lpszImageName);
		}
	}*/
	TCHAR lpszImageName[64] = {0};
	if(pThumbnailBox->AddImage(_T(PicPath0))==FALSE)
	{
		_stprintf(lpszImageName, TEXT("image\\NULL.jpg"));
		pThumbnailBox->AddImage(lpszImageName);
	}
	if(pThumbnailBox->AddImage(_T(PicPath1))==FALSE)
	{
		_stprintf(lpszImageName, TEXT("image\\NULL.jpg"));
		pThumbnailBox->AddImage(lpszImageName);
	}
	pThumbnailBox->Create(0, 0, rcClient.Width(), rcClient.Height(), this);

	MoveWindow(920,30,THUMBNAIL_BOX_WIDTH+20,THUMBNAIL_BOX_HEIGHT*2+20,TRUE);
	/*
	if(ShowPicTime>0)
		SetTimer(1, ShowPicTime, NULL);
	else
		SetTimer(1, 3000, NULL);
	*/
	CString Name;
	Name.Format("---------------------------------------------------------------------------照片%d------------------------------------------------------------------------------------",ShowHandleHun+1);
	SetWindowText(Name);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CThumbnailBoxDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CThumbnailBoxDlg::OnPaint()
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
HCURSOR CThumbnailBoxDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CThumbnailBoxDlg::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
		case 1:
			KillTimer(1);
			//exit(0);
			//EndDialog(0);//SendMessage(WM_CLOSE,   0,   0);//OnCancel();
			break;
	}
	CDialog::OnTimer(nIDEvent);
}