// BasicUse.h: interface for the CBasicUse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASICUSE_H__4593B362_44F0_4ECD_9982_FA88F21FD374__INCLUDED_)
#define AFX_BASICUSE_H__4593B362_44F0_4ECD_9982_FA88F21FD374__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBasicUse  
{
public:
	CBasicUse();
	virtual ~CBasicUse();
	CString GetApp_Path();		//取得应用程序所在目录
	CString GetCurDateTimeString();	//返回当前时间的串,以便SQL语句用
	CString GetDateTimeString(CTime tDatetime);	//返回时间的串,以便SQL语句用

	CString GetCurDateString();	//返回当前日期的串yyyy-mm-dd
	CString GetPicCurDateString();	//返回当前日期的串,以便创建日期文件夹时用yyyymmdd

	CString GetCurYearString();		//返回当前年年份串
	CString GetCurMonthString();	//返回当前年月份串
	CString GetCurDayString();		//返回当前年日子串

	//根据指定时间串取得日期串yyyymmdd
	CString GetDate_Connect(CString strDatetime);

	//根据指定时间串取得日期时间串yyyymmddhhmmss
	CString GetDateTime_Connect(CString strDatetime);
	CString GetCurDateTimeString_C(CTime tDatetime);	//当前时间的yyyymmddhhmmss

	BOOL CreatePath(CString strFilePath);		//创建目录
	BOOL PathExists(CString strFilePath);		//目录是否存在
	BOOL FileExists(CString strFileName);		//文件是否存在
		
	CString JPGToBMPString(CString strJPGFileName);		//jpg文件名串转换成.bmp串
	CString BMPToJPGString(CString strBMPFileName);		//bmp文件名串转换成.jpg串

	HINSTANCE LoadDll(CString strDllName);			//动态加载dll

	//数据库图象转换成bmp文件
	//放在CPictureProcess类里了，属于图片处理类的功能
	//BOOL DBStreamToBmpFile(VARIANT vDBStream,long lngStreamLength,CString strPicFile);

	//删除指定文件夹下所有文件
	BOOL DeleteFilesInFolder(CString strFolder);

};

#endif // !defined(AFX_BASICUSE_H__4593B362_44F0_4ECD_9982_FA88F21FD374__INCLUDED_)
