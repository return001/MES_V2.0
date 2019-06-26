// HistogramDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HistogramDlg.h"
#include "afxdialogex.h"


// CHistogramDlg 对话框

/*全局变量和函数*/
CHistogramDlg *HistogramDlgHandle;


IMPLEMENT_DYNAMIC(CHistogramDlg, CDialogEx)

CHistogramDlg::CHistogramDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHistogramDlg::IDD, pParent)
{
	m_RectName = NULL;
	m_RectValue = NULL;
	m_RectValue_draw = NULL;
}

CHistogramDlg::~CHistogramDlg()
{
}

void CHistogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHistogramDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CHistogramDlg::OnBnClickedOk)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CHistogramDlg 消息处理程序
void CHistogramDlg::DrawPrev(int DrawTypeNum,CString *RectName, int *RectValue)
{
	if (m_RectName != NULL)
	{
		delete[] m_RectName;
		m_RectName = NULL;
	}

	if (m_RectValue != NULL)
	{
		delete [] m_RectValue;
		m_RectValue = NULL;
	}

	if (m_RectValue_draw != NULL)
	{
		delete[] m_RectValue_draw;
		m_RectValue_draw = NULL;
	}

	m_NumRect = DrawTypeNum;
	m_RectName = new CString[m_NumRect];
	m_RectValue = new int[m_NumRect];
	m_RectValue_draw = new int[m_NumRect];

	for (int i = 0; i<m_NumRect; i++){
		m_RectName[i] = RectName[i];
		m_RectValue[i] = RectValue[i];
		m_RectValue_draw[i] = RectValue[i];
	}

	m_MaxValue = m_RectValue[0];//选出最大的数值，假设是第一个最大
	for (int i = 0; i < m_NumRect; i++){
		if (m_MaxValue < m_RectValue[i]){
			m_MaxValue = m_RectValue[i];
		}
	}

}

void CHistogramDlg::ClearPoint()
{
	if (m_RectName != NULL)
	{
		delete[] m_RectName;
		m_RectName = NULL;  
	}

	if (m_RectValue != NULL)
	{
		delete[] m_RectValue;
		m_RectValue = NULL;
	}

	if (m_RectValue_draw != NULL)
	{
		delete[] m_RectValue_draw;
		m_RectValue_draw = NULL;
	}
	InvalidateRect(NULL);
	UpdateWindow();
}

void CHistogramDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	//CDialogEx::OnOK();

}


void CHistogramDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	if (m_RectValue != NULL)
	{
		CRect rc; //创建一个矩形，
		GetClientRect(rc); //获取对话框客户区的矩形；
		dc.FillSolidRect(rc, RGB(255, 255, 255));//设置为白色背景
		rc.DeflateRect(20, 20); //缩小矩形大小,不让柱形图和对话框的边重合；
		int nSegWidth = rc.Width() / (m_NumRect*1.5); //计算每个条形的宽度，m_NumRect是你要画的柱形图的柱形数；

		/////////////////////////////////////////////////////////////////接下来是作除法处理，求出单位高度，为保证单位高度大于1，将m_MaxValue缩小，并记录缩小倍数

		int count = 0;
		while (rc.Height() < (int)m_MaxValue){
			m_MaxValue /= 2;
			count++;
		}
		int nSegHeight = (int)(rc.Height() / m_MaxValue);//计算柱形的单位高度

		/////////////////////////////////////////////////////////////////////其他数据也作缩小处理

		for (int i = 0; i < m_NumRect; i++){
			for (int j = 0; j < count; j++){
				m_RectValue[i] /= 2;
			}
		}

		////////////////////////////////////////////////////////////////

		COLORREF crSeg = RGB(255, 0, 0); //设置颜色
		CBrush brush1(HS_FDIAGONAL, crSeg); //设置画刷1
		CBrush brush2(HS_BDIAGONAL, crSeg); //设置画刷2
		CPen pen(PS_INSIDEFRAME, 80, crSeg); //设置画笔
		CBrush* oldBrush = dc.SelectObject(&brush1);
		CPen* oldPen = dc.SelectObject(&pen);
		CRect rcSeg(rc); //创建一个矩形，后来的画柱形就是画它，改变他的高度度，宽度
		rcSeg.right = rcSeg.left + nSegWidth;

		CRect rcStr;
		for (int i = 0; i < m_NumRect; i++){
			if (i % 2)
				dc.SelectObject(&brush1);
			else
				dc.SelectObject(&brush2);
			rcSeg.top = rcSeg.bottom - (int)(m_RectValue[i] * nSegHeight) - 2;//计算每个条形的高度；
			dc.Rectangle(rcSeg);
			CString str;
			str.Format("%dDB", m_RectValue_draw[i]);//在条形图上输出文字
			dc.DrawText(str, rcSeg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			rcStr = rcSeg;
			rcStr.top = rcStr.bottom + 2;
			rcStr.bottom += 20;
			dc.DrawText(m_RectName[i]+"号", rcStr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			rcSeg.OffsetRect(nSegWidth + 10, 0); //将矩形右移；
		}
		dc.SelectObject(oldBrush);
		dc.SelectObject(oldPen);
	}
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


BOOL CHistogramDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	HistogramDlgHandle = this;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
