// CombineImages.h: interface for the CCombineImages class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMBINEIMAGES_H__18C220B4_89A5_4F07_996B_A7AC40C3EAD2__INCLUDED_)
#define AFX_COMBINEIMAGES_H__18C220B4_89A5_4F07_996B_A7AC40C3EAD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PictureProcess.h"

class CCombineImages  
{
public:
	CCombineImages();
	virtual ~CCombineImages();

	//合成图片的入口函数,WOrH为按纵向还是按横向合成，0-纵向，1-横向
	BOOL CombineImagesEntry(CString * strSourceNames, int nNum, LPCTSTR strDestName,int WOrH);

	//对控制变量进行处理，见私有变量说明
	void SetpWnd(CWnd * pWnd);
	void SetAddTextFlag(BOOL bAddText);
	void SetTextInfo(CString strTextInfo);
private:

	//真正合成两张图片的流BufLast
	BOOL CombineTwoImages(LPBYTE Buf1,LPBYTE Buf2,
						  LPBYTE BufLast,long BufLastLen,
					      long ImageWidthPer1,long ImageWidthPer2,
						  long lHeight1,long lHeight2);
	
////成员变量////////////////////////////////////////////////
private:
	CPictureProcess m_PicProcess;		//图片处理基本类

	CWnd * m_pWnd;			//传递过来的窗口的句柄，以便叠加信息用
	BOOL m_bAddText;				//信息是否叠加到图片上
	CString m_strTextInfo;	//叠加的信息内容
	int m_WOrH;				//按纵向还是横向合成图片 0-纵向，1-横向
public:
	//合成状态:1:正在合成,不允许其他合成,0-合成完毕，可以进行下一次合成
	int m_nCombineFlag;
};

#endif // !defined(AFX_COMBINEIMAGES_H__18C220B4_89A5_4F07_996B_A7AC40C3EAD2__INCLUDED_)
