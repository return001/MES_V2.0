// CombineImages.cpp: implementation of the CCombineImages class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CombineImages.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//构造函数
CCombineImages::CCombineImages()
{
	m_bAddText = FALSE;	
	m_strTextInfo = "";
	m_pWnd = NULL;
	m_nCombineFlag = 0;

}

//析构函数
CCombineImages::~CCombineImages()
{
	//外界窗口句柄删除
	if(m_pWnd !=NULL)
	{
		m_pWnd = NULL;
		delete m_pWnd;
	}
	
}

///////设置基本信息////////////////////////////////////
void CCombineImages::SetpWnd(CWnd* pWnd)
{
	m_pWnd = pWnd;	
}

void CCombineImages::SetAddTextFlag(BOOL bAddText)
{
	m_bAddText = bAddText;
}

void CCombineImages::SetTextInfo(CString strTextInfo)
{
	m_strTextInfo = strTextInfo;
}
///////////////////////////////////////////////////


////////////////////////合成图片的入口函数//////////////////////////////
/*
参数:strSourceNames 多张要合成的bmp图片名数组(指针,可动态定义数组大小)
	 nNum			数组长度
	 strDestName	合成文件的全路径
	 WOrH			按纵向还是按横向合成，0-纵向，1-横向
功能: 合成图片的入口函数，可以对多张宽度相同，高度不同的图片进行合成
返回:BOOL型,TRUE 为成功，FALSE为失败
*/
BOOL CCombineImages::CombineImagesEntry(CString * strSourceNames, 
										int nNum, 
										LPCTSTR strDestName,
										int WOrH)
{
	m_nCombineFlag = 1;				//开始合成标志

	BOOL bSuccess = FALSE;

	m_WOrH = WOrH;

	/////////用来保存每张图片的基本信息/////////////////////
	long * ImageSizePer;
	ImageSizePer = new long[nNum];		//每张图片总长数组

	long * ImageWidthPer;
	ImageWidthPer = new long[nNum];		//图片宽度数组
	long * ImageHeightPer;
	ImageHeightPer = new long[nNum];	//图片高度数组

	LPBYTE * lpImagePer;				//临时图片数据流数组
	lpImagePer = new LPBYTE[nNum];
	///////////////////////////////////////////////////////
		
	long lAllSize = 0;		//所有图片的长度，以便合成图用
	long lAllHeight= 0;		//合成图的高度
	long lAllWidth= 0;		//合成图的宽度

	CString strFileName = "";	//临时文件名
	int i;

	////////////////得到基本内容，以便合成图片/////////////////////
	for(i = 0;i<nNum;i++)
	{
		strFileName = strSourceNames[i];
		BITMAPINFO bmpInfo = m_PicProcess.GetBmpInfo(strFileName);	//get bmp info

		if(bmpInfo.bmiHeader.biWidth <=0)
		{
			m_nCombineFlag = 0;		//无效，合成结束
			AfxMessageBox("图片不存在或图片无效！");
			return FALSE;
		}
		
		ImageWidthPer[i] = bmpInfo.bmiHeader.biWidth;		//宽
		ImageHeightPer[i] = bmpInfo.bmiHeader.biHeight;		//高
		//把每张图片大小放到数组里
		ImageSizePer[i] = ImageWidthPer[i] * ImageHeightPer[i] * bmpInfo.bmiHeader.biBitCount;
		
		//lAllHeight = lAllHeight + ImageHeightPer[i];	//合成图总高度的累计
		//lAllWidth = lAllWidth + ImageWidthPer[i];		//合成图总宽度的累计
		
		lAllSize = lAllSize + ImageSizePer[i];			//合成图大小的累计

		//得到当前图片的数据流
		LPBYTE lpImage;
		lpImage=new byte[ImageSizePer[i]];
		m_PicProcess.GetBmpBuffer(strFileName,lpImage);

		//把流给指定图片数据流数组
		long lSizeTemp = ImageSizePer[i];
		lpImagePer[i] = new byte[lSizeTemp];
		memcpy(lpImagePer[i],lpImage,ImageSizePer[i]);

		//delete pointers
		if(lpImage != NULL)
		{
			lpImage = NULL;
			delete lpImage;
		}
	}
	
	//真正合成图片，并得到合成图片数据流,每次合成2张，循环合成////////////////////////
	//合成 需要lpImagePer[i], ImageSizeRowPer[i],ImageHeightPer[i], lAllSize 
	bSuccess = FALSE;
	long lAllSizeTemp = 0;	//临时合成图大小
	long lWidth,lHeight;	//合成图宽，高
	LPBYTE lpImageLast=NULL;		//最后合成图的数据流变量

	int nFor = nNum - 1;	//循环 nNum -1次,nNum为图片张数,如果3张，则循环2次
	for(i=0;i<nFor;i++)
	{
		lAllSizeTemp = (ImageWidthPer[i] * ImageHeightPer[i] * 3) + 
				(ImageWidthPer[i+1] * ImageHeightPer[i+1] * 3);
		
		if(lpImageLast != NULL)
		{
			//若已有内容，先删除
			lpImageLast = NULL;
			delete lpImageLast;
		}

		//分配空间
		lpImageLast=new byte[lAllSizeTemp];
		if(m_WOrH == 0)		//纵向
		{
			lWidth = ImageWidthPer[i];
			lHeight = ImageHeightPer[i] + ImageHeightPer[i+1];
		}
		else		//横向
		{
			lWidth = ImageWidthPer[i] + ImageWidthPer[i+1];
			lHeight = ImageHeightPer[i];
		
		}

		//进行两张图片的合成
		bSuccess = CombineTwoImages(
					lpImagePer[i],lpImagePer[i+1],
					lpImageLast,lAllSizeTemp,
					ImageWidthPer[i],ImageWidthPer[i+1],
					ImageHeightPer[i],ImageHeightPer[i+1]
				);
		
		//把本次合成的数据流信息给i+1,比如:前边第1和2张图合成后，
		//把合成图的信息给i+1,等下次循环时,作为下次的第1张图信息
		if((i+1) < nFor)
		{
			lpImagePer[i+1] = NULL;
			delete lpImagePer[i+1];
			lpImagePer[i+1] = new byte[lAllSizeTemp];
			memcpy(lpImagePer[i+1],lpImageLast,lAllSizeTemp);
			ImageWidthPer[i+1] = lWidth;
			ImageHeightPer[i+1] = lHeight;
		}
	}

	if(lpImageLast == NULL)
	{
		m_nCombineFlag = 0;		//无效，合成结束
		return FALSE;			//若没得到流，则返回
	}

	//是否叠加文字到图片,叠加时字要多些，否则出黑框
	CString sInfo = m_strTextInfo;
	if(m_bAddText)
		m_PicProcess.AddTextToBmp((LPCTSTR)strDestName,m_pWnd,lpImageLast,lWidth,lHeight,sInfo);
		
	//保存成bmp文件
	m_PicProcess.SaveImage(strDestName, lpImageLast, lWidth,lHeight);
	
	/////delete pointers
	if(lpImageLast == NULL)
	{
		lpImageLast = NULL;
		delete lpImageLast;
	}

	for(i = 0;i<nNum;i++)
	{
		if(lpImagePer[i] != NULL)
		{
			lpImagePer[i] = NULL;
			delete lpImagePer[i] ;
		}
	}

	if(ImageSizePer !=NULL)
		delete [] ImageSizePer;

	if(ImageWidthPer !=NULL)
		delete [] ImageWidthPer;

	if(ImageHeightPer !=NULL)
		delete [] ImageHeightPer;
	
	Sleep(50);
	m_nCombineFlag = 0;		//合成完毕 
	
	return bSuccess;

}
/////////////////////////////////////////////////////////////

//真正合成两张图片的流//////////////////////////////////////////
//Buf1,Buf2为2张图片流；BufLast为合成流，BufLastLen为合成流大小
//ImageWidthPer1,ImageWidthPer2为2张图片各自的宽度
//lHeight1,lHeight2为2张图片的各自高度
BOOL CCombineImages::CombineTwoImages(LPBYTE Buf1,LPBYTE Buf2,
									  LPBYTE BufLast,long BufLastLen,
									  long ImageWidthPer1,
									  long ImageWidthPer2,
									  long lHeight1,long lHeight2)
{
	
	int i,len;
	long lSizeAll1,lSizeAll2;
	if(m_WOrH == 0)		//纵向
	{
		len = ImageWidthPer1 * 3;		
		lSizeAll1 = len * lHeight1;		//第1张图的大小
		lSizeAll2 = len * lHeight2;		//第2张图的大小
	}
	else	//横向
	{
		
		
		
	}
	

	if(BufLastLen != lSizeAll1 + lSizeAll2)
		return FALSE;
	
	if(m_WOrH == 0)		//纵向
	{
		for(i=0;i<lHeight1;i++)
		{
			//第1张图放在第2张上边，而且是从上半部分开始
			//lSizeAll2的意思是把第1张图移动往前lSizeAll2（第2张图片的长度）
			memcpy(BufLast + i*len + lSizeAll2, Buf1+i*len,len);
		}
		for(i=0;i<lHeight2;i++)
		{
			//第2张图
			memcpy(BufLast + i*len, Buf2+i*len,len);
		}
	}
	else
	{
		
	}
	
	return TRUE;
}
//////////////end of combine 2 pictures///////////////

