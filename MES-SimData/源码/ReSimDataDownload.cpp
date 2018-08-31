// ReSimDataDownload.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCP3SIMPORT.h"
#include "MFCP3SIMPORTDlg.h"
#include "ReSimDataDownload.h"
#include "afxdialogex.h"
#include "ADOManage.h"
#include "ThreadPool.h"
#include "Task.h"
#include "ADOManage.h"

// CReSimDataDownload 对话框
#define WM_MainRePortThreadControl WM_USER+1310

CString strReFolderpath, strReOKFolderpath, strReFolderFile, m_resimdatafolderPath;//放文件夹路径
HWND MainReFormHWND;//主控线程句柄
CString reComNo;
HANDLE reporthandler;
int simrestratflag = 0;

volatile BOOL m_RePortDownloadWrite1;
volatile BOOL m_RePortDownloadWrite2;
volatile BOOL m_RePortDownloadRead1;
volatile BOOL m_RePortDownloadRead2;
volatile BOOL m_RePortDownloadReadEnd2;

IMPLEMENT_DYNAMIC(CReSimDataDownload, CDialogEx)

CReSimDataDownload::CReSimDataDownload(CWnd* pParent /*=NULL*/)
   : CDialogEx(CReSimDataDownload::IDD, pParent)
   , simreconnectflag(0)
{

}

CReSimDataDownload::~CReSimDataDownload()
{
}

void CReSimDataDownload::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORTLIST1_COMBO, m_report1list1Combo);
	DDX_Control(pDX, IDC_RECURRENTINFORMATION_RICHEDIT, m_recurrentinformationRichedit);
}


BEGIN_MESSAGE_MAP(CReSimDataDownload, CDialogEx)
	ON_BN_CLICKED(IDC_REOPENSIMDATAFOLDERPATH_BUTTON, &CReSimDataDownload::OnBnClickedReopensimdatafolderpathButton)
	ON_BN_CLICKED(IDC_REPORT1CONNECT_BUTTON, &CReSimDataDownload::OnBnClickedReport1connectButton)
	ON_BN_CLICKED(IDC_RESTART_BUTTON, &CReSimDataDownload::OnBnClickedRestart1Button)
	ON_BN_CLICKED(IDCANCEL, &CReSimDataDownload::OnBnClickedCancel)
	ON_MESSAGE(WM_MainRePortThreadControl, &CReSimDataDownload::MainRePortThreadControl)
	ON_CBN_DROPDOWN(IDC_REPORTLIST1_COMBO, &CReSimDataDownload::OnCbnDropdownReportlist1Combo)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CReSimDataDownload 消息处理程序
BOOL CReSimDataDownload::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//初始化串口列表
	parentdlg->FindCommPort((CComboBox*)GetDlgItem(IDC_REPORTLIST1_COMBO), reComNo, 1);

	//将ini配置文件信息的东西读出来
	CString strpath;
	GetPrivateProfileString(_T("SimPathName"), _T("resimfolderpathname"), _T(""), strpath.GetBuffer(100), 100, _T(".\\SystemInfo.ini"));
	SetDlgItemText(IDC_RESIMDATAFOLDERPATH_EDIT, strpath);
	strpath.ReleaseBuffer();
	
	//初始化字体
	fontinit();
	MainReFormHWND = FindWindow(NULL, L"返工模式")->m_hWnd;

	//线程池初始化
	m_lpThreadPool = NULL;
	if (!m_lpThreadPool)
	{
		m_lpThreadPool = new CThreadPool();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

//初始化模块函数
//点击浏览文件夹按钮
void CReSimDataDownload::OnBnClickedReopensimdatafolderpathButton()
{
	// TODO:  在此添加控件通知处理程序代码
	//打开文件夹
	BROWSEINFO bi;
	char name[MAX_PATH];
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	bi.pszDisplayName = LPWSTR(name);
	bi.lpszTitle = L"选择文件夹目录";
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (idl == NULL)
		return;
	CString strDirectoryPath;
	SHGetPathFromIDList(idl, strDirectoryPath.GetBuffer(MAX_PATH));
	strDirectoryPath.ReleaseBuffer();
	if (strDirectoryPath.IsEmpty())
		return;
	if (strDirectoryPath.Right(1) != "\\")
		strDirectoryPath += "\\";

	//将选择的路径显示到SIM卡数据卡路径框中
	SetDlgItemText(IDC_RESIMDATAFOLDERPATH_EDIT, strDirectoryPath);
	//将路径信息保存到信息文件中
	WritePrivateProfileString(_T("SimPathName"), _T("resimfolderpathname"), strDirectoryPath, _T(".\\SystemInfo.ini"));
}

//点击连接按钮
void CReSimDataDownload::OnBnClickedReport1connectButton()
{
	// TODO:  在此添加控件通知处理程序代码
	if (simreconnectflag == 0)
	{
		//先获取当前选择的串口号
	parentdlg->GetCommPort((CComboBox*)GetDlgItem(IDC_REPORTLIST1_COMBO), reComNo);

	//然后再初始化串口号
	GetDlgItemText(IDC_RESIMDATAFOLDERPATH_EDIT, m_resimdatafolderPath);
	if (m_resimdatafolderPath == L"")
	{
		MessageBox(L"请选择SIM卡数据路径！", L"提示信息", NULL);
		return;
	}

	reporthandler = parentdlg->InitCom(reComNo);

	if (reporthandler == NULL)
	{
		return;
	}

	GetDlgItem(IDC_RESTART_BUTTON)->EnableWindow(TRUE);
	simreconnectflag = 1;
	SetDlgItemText(IDC_REPORT1CONNECT_BUTTON, L"断开");
	GetDlgItem(IDC_REOPENSIMDATAFOLDERPATH_BUTTON)->EnableWindow(FALSE);
	OnBnClickedRestart1Button();
	GetDlgItem(IDC_REPORTLIST1_COMBO)->EnableWindow(FALSE);
	}
	else if (simreconnectflag == 1)
	{
		if (simrestratflag == 1)
		{
			OnBnClickedRestart1Button();
		}
		if (!parentdlg->CloseCom(reporthandler))
		{
			//SetRicheditText(L"关闭串口失败", 1);
			MessageBox(L"关闭串口失败", L"提示信息", NULL);
			return;
		}

		//SetRicheditText(L"关闭串口成功", 0);
		GetDlgItem(IDC_RESTART_BUTTON)->EnableWindow(FALSE);
		simreconnectflag = 0;
		SetDlgItemText(IDC_REPORT1CONNECT_BUTTON, L"连接");
		GetDlgItem(IDC_REOPENSIMDATAFOLDERPATH_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_REPORTLIST1_COMBO)->EnableWindow(TRUE);
	}

	return;
}

//点击开始返工按钮
void CReSimDataDownload::OnBnClickedRestart1Button()
{
	// TODO:  在此添加控件通知处理程序代码
	if (simrestratflag == 0)
	{
		GetDlgItemText(IDC_RESIMDATAFOLDERPATH_EDIT, strReOKFolderpath);

		if (!PathIsDirectory(strReOKFolderpath))
		{
			MessageBox(L"您所选择的文件夹不存在！请重新选择！", L"提示信息", NULL);
			return;
		}
		simrestratflag = 1;
		SetDlgItemText(IDC_RESTART_BUTTON, L"停止返工");
		SetDlgItemText(IDC_REPORT1HINT_STATIC, L"就绪");
		//m_Port2DownloadControl = TRUE;
		s_bReExit = FALSE;
		OpenThreadPoolTask(PORT_REAUTO_THREAD);
	}
	else if (simrestratflag == 1)
	{
		s_bReExit = TRUE;
		//m_Port2DownloadControl = FALSE;
		simrestratflag = 0;
		SetDlgItemText(IDC_RESTART_BUTTON, L"开始返工");
		SetDlgItemText(IDC_REPORT1HINT_STATIC, L"停止");
		SetRePortEditEmpty();
		//StrFolder[1] = L"";
	}
}

//点击串口1下拉框
void CReSimDataDownload::OnCbnDropdownReportlist1Combo()
{
	// TODO:  在此添加控件通知处理程序代码
	parentdlg->FindCommPort((CComboBox*)GetDlgItem(IDC_REPORTLIST1_COMBO), reComNo, 1);
}

//清空IMEI和RID编辑框
void CReSimDataDownload::SetRePortEditEmpty()
{
	SetDlgItemText(IDC_REPORT1RID_EDIT, L"");
	SetDlgItemText(IDC_REPORT1IMEI_EDIT, L"");
}


//返工位线程
//开启线程池函数
void CReSimDataDownload::OpenThreadPoolTask(int Command)
{
	CTask* pTask = new CTask;
	pTask->SetCommand(Command);
	m_lpThreadPool->AddTask(pTask);
}

//线程池的消息循环
afx_msg LRESULT CReSimDataDownload::MainRePortThreadControl(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		//这里是单文件下载的
		//一开始先是发送test命令，test命令的写和读同时开
	case MainPort_RePort_Test:
		OpenThreadPoolTask(REPORT_WRITE1_THREAD);
		OpenThreadPoolTask(REPORT_READ1_THREAD);
		break;
		//然后开始发送RID和IMEI命令，写和读分别开
	case MainPort_RePort_Write2:
		OpenThreadPoolTask(REPORT_WRITE2_THREAD);
		break;
	case MainPort_RePort_Read2:
		OpenThreadPoolTask(REPORT_READ2_THREAD);
		break;
	default:
		break;
	}
	return 0;
}

//返工位主控线程
void CReSimDataDownload::ReDownloadMainThread(LPVOID lpParam)
{
	strReFolderpath = strOKFolderpath.Left(strOKFolderpath.GetLength() - 3) + L"\\";

	if (!PathIsDirectory(strReFolderpath))
	{
		::CreateDirectory(strReFolderpath, NULL);//创建目录,已有的话不影响
	}

	while (!s_bReExit)
	{
		//::PostMessage(MainReFormHWND, WM_MainRePortThreadControl, MainPort_RePort_Test, NULL);
		
		
		Sleep(8000);
	}
}

//返工位test指令的写线程
void CReSimDataDownload::ReDownloadWrite1PortThread(LPVOID lpParam)
{
	CReSimDataDownload* dlg;
	dlg = (CReSimDataDownload*)lpParam;
	PurgeComm(reporthandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;

	//放指令用变量
	CString strcommand = L"AT^GT_CM=TEST\r\n";

	//一直test，看看串口有没有连接上
	dlg->SetRePortEditEmpty();
	//dlg->GetDlgItem(IDC_OPENSIMDATAFILEPATH_BUTTON)->EnableWindow(TRUE);
	m_RePortDownloadWrite1 = TRUE;

	ClearCommError(reporthandler, &dwErrorFlags, &ComStat);

	Sleep(50);
	do
	{
		dlg->SetRicheditText(L"发:" + strcommand, 0);
		bWriteStat = WriteFile(reporthandler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);
		Sleep(1500);

		if (s_bReExit == TRUE)
		{
			dlg->ReDownloadRestPortThread();
			dlg->ReDownloadClosePortThread();
			return;
		}
	} while (m_RePortDownloadWrite1);

	return;
}

//返工位test指令的读线程
void CReSimDataDownload::ReDownloadWrite2PortThread(LPVOID lpParam)
{
	CReSimDataDownload* dlg;
	dlg = (CReSimDataDownload*)lpParam;
	//串口变量
	char str[100];
	memset(str, 0, sizeof(str));
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;

	m_RePortDownloadRead1 = TRUE;;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程

	do
	{
		Sleep(1520);
		bReadStat = ReadFile(reporthandler, str, 100, &readreal, 0);
		if (bReadStat)
		{
			strread = str;
			if (strread.Find(L"TEST_OK") >= 0)
			{
				m_RePortDownloadRead1 = FALSE;
				m_RePortDownloadWrite1 = FALSE;
			}
		}
		if (s_bReExit == TRUE)
		{
			dlg->ReDownloadRestPortThread();
			dlg->ReDownloadClosePortThread();
			return;
		}
	} while (m_RePortDownloadRead1);
	dlg->SetRicheditText(L"收:" + strread, 0);
	PurgeComm(reporthandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	::PostMessage(MainReFormHWND, WM_MainRePortThreadControl, MainPort_RePort_Write2, NULL);
	return;
}

//返工位读RID和IMEI指令的写线程
void CReSimDataDownload::ReDownloadRead1PortThread(LPVOID lpParam)
{
	CReSimDataDownload* dlg;
	dlg = (CReSimDataDownload*)lpParam;
	::PostMessage(MainReFormHWND, WM_MainRePortThreadControl, Main_Hint1_Connected, NULL);
	PurgeComm(reporthandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	//串口变量
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	DWORD dwTotalSize;//文件总大小

	//放指令用变量
	CString strcommand;
	CString strfilewritecommand;


	::PostMessage(MainReFormHWND, WM_MainRePortThreadControl, MainPort_RePort_Read2, NULL);

	Sleep(100);
	PurgeComm(reporthandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
	//连接上了就开始读它的RID和IMEI，如果连续发送五条命令都没反应，那就返回Test那里重新检测设备
	for (int i = 0; i < 2; i++)
	{
		int count = 0;
		if (i == 0)
		{
			strcommand = L"AT^GT_CM=ID,1\r\n";//读RID的一个命令
		}
		else if (i == 1)
		{
			strcommand = L"AT^GT_CM=IMEI\r\n";//读IMEI的一个命令
		}

		ClearCommError(reporthandler, &dwErrorFlags, &ComStat);

		//然后同时开启读线程
		m_RePortDownloadWrite2 = TRUE;

		PurgeComm(reporthandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);
		do
		{

			dlg->SetRicheditText(L"发:" + strcommand, 0);
			bWriteStat = WriteFile(reporthandler, CT2A(strcommand), strcommand.GetLength(), &dwBytesWrite, NULL);

			if (count == 7)
			{
				//::PostMessage(MainReFormHWND, WM_MainRePortThreadControl, Main_Hint1_Fail, NULL);
				dlg->ReDownloadRestPortThread();
				dlg->ReDownloadClosePortThread();
				return;
			}
			count++;
			Sleep(600);
			if (s_bReExit == TRUE)
			{
				dlg->ReDownloadRestPortThread();
				dlg->ReDownloadClosePortThread();
				return;
			}
		} while (m_RePortDownloadWrite2);
	}
	return;
}

//返工位读RID和IMEI指令的读线程
void CReSimDataDownload::ReDownloadRead2PortThread(LPVOID lpParam)
{
	CReSimDataDownload* dlg;
	dlg = (CReSimDataDownload*)lpParam;
	//串口变量
	char str[100];
	memset(str, 0, sizeof(str));
	DWORD readreal = 0;
	BOOL bReadStat;

	//其余变量
	CString strread;
	CString strcount, strcounttemp;//放RID和IMEI用
	CString strtemp;
	int findcount1, findcount2;//切割读出来的字符串用

	for (int i = 0; i < 2; i++)
	{
		m_RePortDownloadRead2 = TRUE;//全局变量，如果等于FALSE的时候，while就会跳出循环，然后退出这个线程
		m_RePortDownloadReadEnd2 = TRUE;
		int count = 0;
		if (i == 0)
		{
			strtemp = L"Chip RID:";//读RID的一个命令
		}
		else if (i == 1)
		{
			strtemp = L"IMEI:";//读IMEI的一个命令
		}
		do
		{
			Sleep(620);
			bReadStat = ReadFile(reporthandler, str, 100, &readreal, 0);
			if (bReadStat)
			{
				strread = str;
				if ((findcount1 = strread.Find(strtemp)) >= 0)
				{
					//读RID
					if (i == 0)
					{
						strcount = strread.Right(strread.GetLength() - findcount1 - 9);
						findcount2 = strcount.Find(L"\r\n");
						strcount = strcount.Left(findcount2);
						if (strcount != "")
						{
							dlg->SetDlgItemText(IDC_PORT1RID_EDIT, strcount);
						}
						else if (strcount == "")
						{
							continue;
						}
					}
					//读IMEI
					else if (i == 1)
					{
						strcount = strread.Right(strread.GetLength() - findcount1 - 5);
						findcount2 = strcount.Find(L"\r\n");
						strcount = strcount.Left(findcount2);
						strcounttemp = strcount;
						if (strcount != ""&&strcount != "^"&&strcounttemp.Trim(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789").GetLength() == 0)
						{
							dlg->SetDlgItemText(IDC_PORT1IMEI_EDIT, strcount);
							//::PostMessage(MainReFormHWND, WM_MainDataInsertControl, DataInsert_SinglePort_IsExit, NULL);
							Sleep(50);
						}
						else
						{
							continue;
						}
					}
					m_RePortDownloadWrite2 = FALSE;
					m_RePortDownloadRead2 = FALSE;
				}
			}
		} while (m_RePortDownloadRead2);
		if (m_RePortDownloadReadEnd2 == FALSE)
		{
			return;
		}
		if (s_bReExit == TRUE)
		{
			dlg->ReDownloadRestPortThread();
			dlg->ReDownloadClosePortThread();
			return;
		}
		dlg->SetRicheditText(L"收:" + strread, 0);
	}
	PurgeComm(reporthandler, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_TXABORT);

	return;
}

//返工位串口重置线程全局变量
void CReSimDataDownload::ReDownloadRestPortThread()
{
	m_RePortDownloadWrite1 = TRUE;
	m_RePortDownloadWrite2 = TRUE;
	m_RePortDownloadRead1 = TRUE;
	m_RePortDownloadRead2 = TRUE;
	m_RePortDownloadReadEnd2 = TRUE;

}

//返工位串口关闭线程全局变量
void CReSimDataDownload::ReDownloadClosePortThread()
{
	m_TreadRePortDownloadWrite1 = FALSE;
	m_TreadRePortDownloadWrite2 = FALSE;
	m_TreadRePortDownloadRead1 = FALSE;
	m_TreadRePortDownloadRead2 = FALSE;
}


//其它函数
void CReSimDataDownload::SetRicheditText(CString strMsg, int No)
{
	m_recurrentinformationRichedit.SetSel(-1, -1);
	CHARFORMAT   cf{ sizeof(cf) };//设置这个结构来改变你想要的文字的颜色等属性
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD | CFM_SIZE | CFM_FACE;//这个目前还没解析完成，但如果要输出不同颜色的字体一定要使用richedit
	cf.crTextColor = RGB(255, 0, 0);//设置为红色

	switch (No)
	{
	case 0:
		m_recurrentinformationRichedit.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
		m_recurrentinformationRichedit.LineScroll(m_recurrentinformationRichedit.GetLineCount() - 1 - 3);
		break;
	case 1:
		m_recurrentinformationRichedit.SetSelectionCharFormat(cf);
		m_recurrentinformationRichedit.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
		m_recurrentinformationRichedit.LineScroll(m_recurrentinformationRichedit.GetLineCount() - 1);
		break;
	default:
		break;
	}

	/*
	CHARFORMAT   cf{ sizeof(cf) };//设置这个结构来改变你想要的文字的颜色等属性
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD | CFM_SIZE | CFM_FACE;//这个目前还没解析完成，但如果要输出不同颜色的字体一定要使用richedit
	if (strMsg == "127.0.0.1>>测试信息")
	{
	cf.crTextColor = RGB(255, 0, 0);
	m_currenttcp1RichEdit.SetSelectionCharFormat(cf); //这个是richedit的专用方法，richedit常用来作为日志输出
	m_currenttcp1RichEdit.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("测试\r\n"));
	}
	cf.crTextColor = RGB(255, 200, 100);
	m_currenttcp1RichEdit.SetSelectionCharFormat(cf);*/
}

//获取系统时间
CString CReSimDataDownload::GetTime()
{
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d/%d/%d %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	return strTime;
}

//初始化字体
void CReSimDataDownload::fontinit()
{
	staticReHintfont.CreatePointFont(900, L"黑体");

	GetDlgItem(IDC_REPORT1HINT_STATIC)->SetFont(&staticReHintfont);
}

//改变字体颜色的消息
HBRUSH CReSimDataDownload::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	CString str1;
	if (pWnd->GetDlgCtrlID() == IDC_REPORT1HINT_STATIC)
	{
		GetDlgItemText(IDC_REPORT1HINT_STATIC, str1);
		if (str1 == "返工失败" || str1 == "无此机记录" || str1 == "数据路径错误" || str1 == "无需返工")
		{
			pDC->SetTextColor(RGB(255, 0, 0));//用RGB宏改变颜色 
			pDC->SelectObject(&staticReHintfont);
		}
		else if (str1 == "返工成功")
		{
			pDC->SetTextColor(RGB(0, 255, 0));//用RGB宏改变颜色 
			pDC->SelectObject(&staticReHintfont);
		}
		else
		{
			pDC->SetTextColor(RGB(0, 0, 0));//用RGB宏改变颜色 
			pDC->SelectObject(&staticReHintfont);
		}
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

//点击退出返工模式
void CReSimDataDownload::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	INT_PTR nRes;
	nRes = MessageBox(_T("确定要退出吗？"), _T("提示消息"), MB_OKCANCEL | MB_ICONQUESTION);
	// 判断消息对话框返回值。如果为IDCANCEL就return，否则继续向下执行   
	if (IDOK == nRes)
	{
	CDialogEx::OnCancel();
	}
}

