/************************************************************************
*                                                                       *
*   ThumbnailBox.h -- This module implements a thumbnail box            *
*                                                                       *
*   Copyright (c) CSDN PCRADIO. All rights reserved.                    *
*                                                                       *
************************************************************************/
#ifndef __THUMBNAILBOX_H_INCLUDED__
#define __THUMBNAILBOX_H_INCLUDED__
#include <afxtempl.h>

#define THUMBNAIL_BOX_WIDTH			856
#define THUMBNAIL_BOX_HEIGHT		480
//#define THUMBNAIL_BOX_HEIGHT		960
//#define THUMBNAIL_BOX_WIDTH		428
//#define THUMBNAIL_BOX_HEIGHT		240
#define THUMBNAIL_BOX_BACKGROUND	RGB(128, 128, 128)
#define THUMBNAIL_BOX_PADDING		3
#define THUMBNAIL_CANVAS_PADDING	10
#define THUMBNAIL_CANVAS_BACKGROUND	RGB(50, 50, 50)

/** 
 * Class CThumbnailBox
 */
class CThumbnailBox : public CWnd
{
	DECLARE_DYNAMIC(CThumbnailBox)
public:
	CThumbnailBox();
	virtual ~CThumbnailBox();

public:
	/** Create the thumbnail box. */
	BOOL   Create(int x, int y, int width, int height, CWnd* pParent);

	/** 
	 * Add image from file.
	 * 
	 * @param lpszImageName - Image file name.
	 */
	BOOL	AddImage(LPCTSTR lpszImageName);

	/** 
	 * Get image.
	 */
	CImage* GetImageAt(int iIndex) const;

	/** 
	 * Remove image.
	 */
	void	RemoveImageAt(int iIndex);

	/** 
	 * Remove all images.
	 */
	void	RemoveAllImage();

	/** 
	 * Get the number of images.
	 */
	int		GetSize() const;

protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

protected:
	/** Image container. */
	CArray<CImage*, CImage*> m_arrImage;

	/** Off screen image for canvas. */
	CImage* m_pCanvasOffScreen;

	/** Context device handle to off screen image of canvas. */
	HDC m_hCanvasOffScreenDC;

	/** Scroll position. */
	int m_iCurrentScrollPos;
	int m_iLastScrollPos;
};
#endif /* __THUMBNAILBOX_H_INCLUDED__ */
