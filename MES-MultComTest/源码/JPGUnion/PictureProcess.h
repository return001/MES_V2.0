#pragma once

#include "gdiplusinit.h"
#include <gdiplus.h>
using namespace Gdiplus;
class CPictureProcess
{
public:
	CPictureProcess(void);

	virtual ~CPictureProcess(void);

	//文字叠加到位图
	void AddTextToBmp(CString sFile, CWnd* pWnd, BYTE *Buf, int Width, int Height, CString sInfo);
	
	//显示图片:文件
	void ShowPicture(CString strPicName,HWND hwndStill);

	//显示图片:流
	void ShowPicture(BYTE *bytePicName,HWND hwndStill);

	//显示图片-byte型
	void ShowPictureByByte(BYTE *Buf,HWND hwndStill,int Width,int Height);

	//bmp或jpg图片文件转换成数据流
	VARIANT ChangePicFileToBYTE(CString strPicName);
	//数据库blob图象转换成bmp文件
	BOOL DBStreamToBmpFile(VARIANT vDBStream,long lngStreamLength,CString strPicFile);

	//得到bmp图片的长度总数 宽*高*位数
	BITMAPINFO GetBmpInfo(LPCTSTR lpszBmpFile);

	//得到bmp图片的数据流
	void GetBmpBuffer(LPCTSTR lpszBmpFile,LPBYTE newbuffer);
	
	//得到bmp图片翻转后的数据流，以便北京亚视车牌识别用...
	//这里是调用动态库方式的，因此要传库名dllName
	void GetBmpVerBuffer(LPCTSTR lpszBmpFile,LPBYTE newbuffer, CString dllName);


	//把数据流保存到图片里
	BOOL SaveImage(LPCTSTR lpszFile, LPBYTE newbuffer, long lWidth,long lHeight);	//合成图保存

	//指定bmp数据流拉伸高度，lWidth和lHeight为原始流的宽高
	//目标流的宽和原始的一样，nHeightTimes为原始流高度的倍数
	void ZoomImageHeight(LPBYTE sourcebuffer, LPBYTE destbuffer,long lWidth,long lHeight,int nHeightTimes);

	//bmp拉伸高度后保存图片，以便查看原始图片 288->576	nHeightTimes为原始图片高度的倍数
	BOOL ZoomImageHeight(LPCTSTR lpszFile, int nHeightTimes);
	
	//jpg文件转换成bmp，并返回被转换的文件名，调用dll，得到同名的bmp名
	CString ChangeJPGToBMP(CString strJPGFileName,CString dllName);


	CString ChangeJPGToBMP2(CString strJPGFileName);
	int GetCodecClsid(const WCHAR* format, CLSID* pClsid);//编码类标识符

	//bmp文件转换成jpg，并返回被转换的文件名，调用dll，得到同名的jpg名
	CString ChangeBMPToJPG(CString strBMPFileName,CString dllName);
	CString ChangeBMPToJPG2(CString strBMPFileName);
public:
	//ULONG_PTR m_GdiplusToken;
	//GdiplusStartupInput m_GdiplusStartupInput;

};
