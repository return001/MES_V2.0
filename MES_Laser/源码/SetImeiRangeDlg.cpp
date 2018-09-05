// SetImeiRangeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCP1LD.h"
#include "SetImeiRangeDlg.h"
#include "afxdialogex.h"
#include "MFCP1LDDlg.h"

// CSetImeiRangeDlg 对话框

IMPLEMENT_DYNAMIC(CSetImeiRangeDlg, CDialogEx)

CSetImeiRangeDlg::CSetImeiRangeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetImeiRangeDlg::IDD, pParent)
	, m_imeirangestartEdit(1)
	, m_imeirangeendEdit(1)
{

}

CSetImeiRangeDlg::~CSetImeiRangeDlg()
{
}

void CSetImeiRangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IMEIRANGESTART_EDIT, m_imeirangestartEdit);
	DDX_Text(pDX, IDC_IMEIRANGEEND_EDIT, m_imeirangeendEdit);
}


BEGIN_MESSAGE_MAP(CSetImeiRangeDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSetImeiRangeDlg::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_IMEIRANGESTART_EDIT, &CSetImeiRangeDlg::OnEnKillfocusImeirangestartEdit)
	ON_EN_KILLFOCUS(IDC_IMEIRANGEEND_EDIT, &CSetImeiRangeDlg::OnEnKillfocusImeirangeendEdit)
END_MESSAGE_MAP()


// CSetImeiRangeDlg 消息处理程序
BOOL CSetImeiRangeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CMFCP1LDDlg *parent = (CMFCP1LDDlg *)GetParent();
	//先把东西从主窗口拿出来然后做个转换
	parent->m_imeistartshowonlyEdit.GetWindowText(m_imeirangestart);
	parent->m_imeiendshowonlyEdit.GetWindowText(m_imeirangeend);

	unsigned long long a = _atoi64(CStringA(m_imeirangestart));
	unsigned long long b = _atoi64(CStringA(m_imeirangeend));

	m_imeirangestartNo = 1;
	m_imeirangeendNo = b-a+1;

	//变成数字后再丢进去
	SetDlgItemInt(IDC_IMEIRANGESTART_EDIT,m_imeirangestartNo);
	SetDlgItemInt(IDC_IMEIRANGEEND_EDIT, m_imeirangeendNo);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

//当起始位编辑框被更改后做的操作
void CSetImeiRangeDlg::OnEnKillfocusImeirangestartEdit()
{
	// TODO:  在此添加控件通知处理程序代码
	int imeistart, imeiend;
	imeistart=GetDlgItemInt(IDC_IMEIRANGESTART_EDIT);
	imeiend = GetDlgItemInt(IDC_IMEIRANGEEND_EDIT);
	if (imeistart >= m_imeirangestartNo&&imeistart<=imeiend)
	{
		return;
	}
	else
	{
		MessageBox(L"起始号段设置错误！",L"提示信息",NULL);
		SetDlgItemInt(IDC_IMEIRANGESTART_EDIT,m_imeirangestartNo);
		return;
	}
}

//当结束位编辑框被更改后做的操作
void CSetImeiRangeDlg::OnEnKillfocusImeirangeendEdit()
{
	// TODO:  在此添加控件通知处理程序代码
	int imeistart, imeiend;
	imeistart = GetDlgItemInt(IDC_IMEIRANGESTART_EDIT);
	imeiend = GetDlgItemInt(IDC_IMEIRANGEEND_EDIT);
	if (imeiend <= m_imeirangeendNo&&imeistart <= imeiend)
	{
		return;
	}
	else
	{
		MessageBox(L"结束号段设置错误！", L"提示信息", NULL);
		SetDlgItemInt(IDC_IMEIRANGEEND_EDIT, m_imeirangeendNo);
		return;
	}
}

//点击确定按钮后要做的操作，起始就是把数据传回主窗口
void CSetImeiRangeDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	//再将IMEI号进行加减然后转换完后丢回去
	CMFCP1LDDlg *parent = (CMFCP1LDDlg *)GetParent();
	int imeistart, imeiend;
	imeistart = GetDlgItemInt(IDC_IMEIRANGESTART_EDIT);
	imeiend = GetDlgItemInt(IDC_IMEIRANGEEND_EDIT);

	unsigned long long a = _atoi64(CStringA(m_imeirangestart));
	unsigned long long b;

	a = a + imeistart - 1;//比如IMEI起始位8006，号段起始设置为1，那就是8006+1-1=8006;
	b = a + imeiend - 1;//比如号段设置结束为300，那就是起始位8006+300-1=8295;简单来说要镭雕1~300就是要镭300个号，即8295-8006+1=300是没错的。

	m_imeirangestart = _ui64toa(a, CT2A(m_imeirangestart), 10);
	m_imeirangeend= _ui64toa(b,CT2A(m_imeirangeend),10);

	parent->m_imeistartEdit.SetWindowText(m_imeirangestart);
	parent->m_imeiendEdit.SetWindowText(m_imeirangeend);

	CDialogEx::OnCancel();
}

