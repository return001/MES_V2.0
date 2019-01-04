#include "stdafx.h"
#include "ThumbnailBox.h"

IMPLEMENT_DYNAMIC(CThumbnailBox, CWnd)

BEGIN_MESSAGE_MAP(CThumbnailBox, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
END_MESSAGE_MAP()

CThumbnailBox::CThumbnailBox()
: m_pCanvasOffScreen(NULL)
, m_hCanvasOffScreenDC(NULL)
, m_iCurrentScrollPos(0)
, m_iLastScrollPos(0)
{

}

CThumbnailBox::~CThumbnailBox()
{		
	if(m_pCanvasOffScreen != NULL)
	{
		if(m_hCanvasOffScreenDC != NULL)
		{
			m_pCanvasOffScreen->ReleaseDC();
		}
		m_pCanvasOffScreen->Destroy();
		delete m_pCanvasOffScreen;
	}
	m_arrImage.RemoveAll();
}

BOOL CThumbnailBox::Create(int x, int y, int width, int height, CWnd* pParent)
{
	if(width < THUMBNAIL_BOX_WIDTH + 2 * THUMBNAIL_BOX_PADDING)
	{
		width = THUMBNAIL_BOX_WIDTH + 2 * THUMBNAIL_BOX_PADDING;
	}
	CRect rcWnd(x, y, x + width, y + height);
	BOOL bCreate = CWnd::Create(NULL, NULL, WS_VISIBLE|WS_CHILD|WS_VSCROLL, rcWnd, pParent, 999);
	if(!bCreate)
		return FALSE;
	if(m_pCanvasOffScreen == NULL)
	{
		m_pCanvasOffScreen = new CImage;
		m_pCanvasOffScreen->Create(width, height, 24);
		m_hCanvasOffScreenDC = m_pCanvasOffScreen->GetDC();
	}
	return TRUE;
}

BOOL CThumbnailBox::AddImage(LPCTSTR lpszImageName)
{
	CImage* pImage = new CImage;
	HRESULT hRes = pImage->Load(lpszImageName);
	//ASSERT(hRes == S_OK);
	if(hRes != S_OK)
	{
		return FALSE;
	}

	int iWidth = pImage->GetWidth();
	int iHeight  = pImage->GetHeight();
	float iRatio = 1.0f;
	if(iWidth > iHeight)
	{
		iRatio = (float)(THUMBNAIL_BOX_WIDTH - THUMBNAIL_BOX_PADDING * 2) / (float)iWidth;
	}
	else
	{
		iRatio = (float)(THUMBNAIL_BOX_HEIGHT - THUMBNAIL_BOX_PADDING * 2) / (float)iHeight;
	}
	int iThumbnailWidth = (int)(iWidth * iRatio);
	int iThumbnailHeight = (int)(iHeight * iRatio);
	int iThumnailX = (THUMBNAIL_BOX_WIDTH - iThumbnailWidth) / 2;
	int iThumnailY = (THUMBNAIL_BOX_HEIGHT - iThumbnailHeight) / 2;

	CImage* pStretchImage = new CImage;
	pStretchImage->Create(THUMBNAIL_BOX_WIDTH, THUMBNAIL_BOX_HEIGHT, 24, 0);
	HDC hDC = pImage->GetDC();
	HDC hStretchDC = pStretchImage->GetDC();
	::SetStretchBltMode(hStretchDC, COLORONCOLOR);
	
	// Clear thumbnail off screen image.
	HPEN hPen = ::CreatePen(PS_SOLID, 1, THUMBNAIL_BOX_BACKGROUND);
	HBRUSH hBrush = ::CreateSolidBrush(THUMBNAIL_BOX_BACKGROUND);
	HPEN hOldPen = (HPEN)::SelectObject(hStretchDC, hPen);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hStretchDC, hBrush);
	::Rectangle(hStretchDC, 0, 0, THUMBNAIL_BOX_WIDTH, THUMBNAIL_BOX_HEIGHT);
	::SelectObject(hStretchDC, hOldPen);
	::SelectObject(hStretchDC, hOldBrush);
	pImage->StretchBlt(hStretchDC, iThumnailX, iThumnailY, iThumbnailWidth, iThumbnailHeight, SRCCOPY);
	pImage->ReleaseDC();
	pStretchImage->ReleaseDC();
	delete pImage;

	m_arrImage.Add(pStretchImage);
	return TRUE;
}

CImage* CThumbnailBox::GetImageAt(int iIndex) const
{
	ASSERT(m_arrImage.GetSize());
	ASSERT(iIndex >= 0 && iIndex < m_arrImage.GetSize());
	if(iIndex < 0 || iIndex >= m_arrImage.GetSize())
		return NULL;
	return m_arrImage.GetAt(iIndex);
}

void CThumbnailBox::RemoveImageAt(int iIndex)
{
	ASSERT(m_arrImage.GetSize());
	ASSERT(iIndex >= 0 && iIndex < m_arrImage.GetSize());
	if(iIndex < 0 || iIndex >= m_arrImage.GetSize())
		return;
	m_arrImage.RemoveAt(iIndex);
}

void CThumbnailBox::RemoveAllImage()
{
	for (int i = 0; i < m_arrImage.GetSize(); i++)
	{
		CImage* pImage = m_arrImage.GetAt(i);
		pImage->Destroy();
		delete pImage;
	}
	m_arrImage.RemoveAll();
}

int CThumbnailBox::GetSize() const
{
	return m_arrImage.GetSize();
}


void CThumbnailBox::OnPaint()
{
	CPaintDC dc(this);
	CRect rcClient;
	GetClientRect(rcClient);
	// Clear canvas off screen image.
	HPEN hPen = ::CreatePen(PS_SOLID, 1, THUMBNAIL_CANVAS_BACKGROUND);
	HBRUSH hBrush = ::CreateSolidBrush(THUMBNAIL_CANVAS_BACKGROUND);
	HPEN hOldPen = (HPEN)::SelectObject(m_hCanvasOffScreenDC, hPen);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(m_hCanvasOffScreenDC, hBrush);
	::Rectangle(m_hCanvasOffScreenDC, 0, 0, rcClient.Width(), rcClient.Height());
	::SelectObject(m_hCanvasOffScreenDC, hOldPen);
	::SelectObject(m_hCanvasOffScreenDC, hOldBrush);

	// Draw thumbnail box.
	SCROLLINFO vsi;
	GetScrollInfo(SB_VERT, &vsi);

	int iColumn = rcClient.Width() / THUMBNAIL_BOX_WIDTH;
	int iThumbnailMargin = 0;//(rcClient.Width() % THUMBNAIL_BOX_WIDTH - THUMBNAIL_CANVAS_PADDING * 2) / (iColumn - 1);
	if(iThumbnailMargin <= 0)
		iThumbnailMargin = 1;
	for (int i = 0; i < GetSize(); i++)
	{
		int iThumbnailX = (i % iColumn) * (THUMBNAIL_BOX_WIDTH + iThumbnailMargin) + THUMBNAIL_CANVAS_PADDING ;
		int iThumbnailY = -vsi.nPos + (i / iColumn) * (THUMBNAIL_BOX_HEIGHT + THUMBNAIL_CANVAS_PADDING) + THUMBNAIL_CANVAS_PADDING;
		CImage* pThumbnail = GetImageAt(i);
		pThumbnail->BitBlt(m_hCanvasOffScreenDC, iThumbnailX, iThumbnailY, SRCCOPY);
	}
	m_pCanvasOffScreen->BitBlt((HDC)dc, 0, 0, SRCCOPY);
}

BOOL CThumbnailBox::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CThumbnailBox::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int iMin, iMax;
	GetScrollRange(SB_VERT, &iMin, &iMax);
	switch(nSBCode)
	{
	case SB_TOP:
		m_iCurrentScrollPos = iMin;
		break;
	case SB_BOTTOM:
		m_iCurrentScrollPos = iMax;
	case SB_LINEDOWN:
		if(m_iCurrentScrollPos < iMax)
		{
			m_iCurrentScrollPos += 10;
		}
		break;
	case SB_LINEUP:
		if(m_iCurrentScrollPos > iMin)
		{
			m_iCurrentScrollPos -= 10;
		}
		break;
	case SB_PAGEDOWN:
	{
		SCROLLINFO si;  
		GetScrollInfo(SB_VERT, &si, SIF_ALL);  
		if (m_iCurrentScrollPos < iMax)  
			m_iCurrentScrollPos = min(iMax, m_iCurrentScrollPos + (int)si.nPage);  
	}
		break;
	case SB_PAGEUP:
	{
		SCROLLINFO si;  
		GetScrollInfo(SB_VERT, &si, SIF_ALL);  
		if (m_iCurrentScrollPos > iMin)  
			m_iCurrentScrollPos = max(iMin, m_iCurrentScrollPos - (int)si.nPage);  
	}
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		m_iCurrentScrollPos = nPos;
		break;
	case SB_ENDSCROLL:
		break;
	}

	if (m_iCurrentScrollPos < 0) 
	{
		m_iCurrentScrollPos = 0;
	}
	if (m_iCurrentScrollPos > iMax) 
	{
		m_iCurrentScrollPos = iMax;
	}

	SetScrollPos(SB_VERT, m_iCurrentScrollPos);
	m_iLastScrollPos = m_iCurrentScrollPos;
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
	Invalidate(FALSE);
}

void CThumbnailBox::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	// Adjust scroll bar.
	int iColumn = cx / THUMBNAIL_BOX_WIDTH;
	int iRow = GetSize() / iColumn + 1;
	int iScrollMax = max(iRow * (THUMBNAIL_BOX_HEIGHT + THUMBNAIL_CANVAS_PADDING), cy - 1);
	SCROLLINFO si;  
	si.cbSize = sizeof(si);  
	si.fMask = SIF_RANGE | SIF_PAGE;  
	si.nMin = 0;  
	si.nMax = iScrollMax;  
	si.nPage = cy;
	SetScrollInfo(SB_VERT, &si, TRUE);  
}
