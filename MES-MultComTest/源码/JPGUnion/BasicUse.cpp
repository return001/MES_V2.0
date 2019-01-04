// BasicUse.cpp: implementation of the CBasicUse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BasicUse.h"

#include <direct.h>		//_mkdir
#include <stdlib.h>
#include <stdio.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBasicUse::CBasicUse()
{

}

CBasicUse::~CBasicUse()
{

}

/*
* 取得当前应用程序所在路径
* 参数:无
* 返回值 CString型 
*/
CString CBasicUse::GetApp_Path()
{
	CString strPath; 

    ::GetCurrentDirectory(MAX_PATH,strPath.GetBuffer(MAX_PATH)); 

    strPath.ReleaseBuffer(); 

	strPath.Format("%s",strPath);

    return strPath; 
}

/*
* 取得当前日期时间串,目的是为了SQL语句时使用;
* 参数:无
* 返回值 CString型 
*/
CString CBasicUse::GetCurDateTimeString(){
	CString strCurDatetime = "";
	CTime tCurDatetime = CTime::GetCurrentTime();
	strCurDatetime = tCurDatetime.Format("%Y-%m-%d %H:%M:%S");
	return strCurDatetime;
}

/*
* 取得日期串,目的是为了SQL语句时使用;
* 参数:日期型变量
* 返回值 CString型 
*/
CString CBasicUse::GetDateTimeString(CTime tDatetime)
{
	CString strCurDatetime = "";
	CTime tCurDatetime = tDatetime;
	strCurDatetime = tCurDatetime.Format("%Y-%m-%d %H:%M:%S");
	return strCurDatetime;
}

//根据时间取得串
CString CBasicUse::GetCurDateTimeString_C(CTime tDatetime)
{
	CString strCurDatetime = "";
	CTime tCurDatetime = tDatetime;
	strCurDatetime = tCurDatetime.Format("%Y%m%d%H%M%S");
	return strCurDatetime;
}

/*
* 取得当前日期串yyyy-mm-dd
* 参数:无
* 返回值 CString型
*/
CString CBasicUse::GetCurDateString(){
	CString strCurDate = "";
	CTime tCurDatetime = CTime::GetCurrentTime();
	strCurDate = tCurDatetime.Format("%Y-%m-%d");
	return strCurDate;
}

//返回当前年份
CString CBasicUse::GetCurYearString(){

	CString strCur = "";
	CTime tCurDatetime = CTime::GetCurrentTime();
	int intCur = tCurDatetime.GetYear();
	strCur.Format("%d",intCur);		//整形转换成字符串型
	return strCur;

}

//返回当前年月份
CString CBasicUse::GetCurMonthString(){

	CString strCur = "";
	CTime tCurDatetime = CTime::GetCurrentTime();
	int intCur = tCurDatetime.GetMonth();
	strCur.Format("%d",intCur);		//整形转换成字符串型
	return strCur;

}

//返回当前年日子
CString CBasicUse::GetCurDayString(){

	CString strCur = "";
	CTime tCurDatetime = CTime::GetCurrentTime();
	int intCur = tCurDatetime.GetDay();
	strCur.Format("%d",intCur);		//整形转换成字符串型
	return strCur;

}

/*
* 取得当前日期串格式:yyyymmdd
* 参数:无
* 返回值 CString型
*/
CString CBasicUse::GetPicCurDateString(){
	CString strCurDate = "";
	CTime tCurDatetime = CTime::GetCurrentTime();
	strCurDate = tCurDatetime.Format("%Y%m%d");
	return strCurDate;
}

//创建目录
BOOL CBasicUse::CreatePath(CString strFilePath){
	
	BOOL ret = false;

	/*
	//该方式只能建一级目录
	SECURITY_ATTRIBUTES attrib;
	attrib.bInheritHandle = FALSE;
	attrib.lpSecurityDescriptor = NULL;
	attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
	::CreateDirectory(strFilePath, &attrib);
	if (PathExists(strFilePath) == true){
		ret = true;
	}

	
	*/

	//该方式可以建立多级目录

	//若目录已存在，返回真
	if (PathExists(strFilePath) == TRUE){
		return true;
	 }

	CString pathname = strFilePath;

     if(pathname.Right(1) != "\\")

              pathname += "\\" ;

     int end = pathname.ReverseFind('\\');

     int pt = pathname.Find('\\');

     if (pathname[pt-1] == ':')

               pt = pathname.Find('\\', pt+1);

     CString path;

     while(pt != -1 && pt<=end)

     {

		path = pathname.Left(pt+1);

		_mkdir(path);

		pt = pathname.Find('\\', pt+1);

     }

	 if (PathExists(strFilePath) == TRUE){
		ret = true;
	 }

     return ret;
}

//目录是否存在
BOOL CBasicUse::PathExists(CString strFilePath){
	WIN32_FIND_DATA fd;
	BOOL ret = false;
    HANDLE hFind = FindFirstFile(strFilePath, &fd);
    if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
		//目录存在
		ret = true;
    
    }
    FindClose(hFind);
	return ret;

}

//文件是否存在
BOOL CBasicUse::FileExists(CString strFileName)
{
	CFileFind fFind;
	return fFind.FindFile(strFileName); 
}

//删除指定文件夹下所有文件
BOOL CBasicUse::DeleteFilesInFolder(CString strFolder)
{
	BOOL res = FALSE;

	CFileFind ff;

	CString strPath = strFolder;	//
	
	if (strPath == ""){

		return FALSE;
	}

	if(strPath.Right(1) != "\\"){
		strPath += "\\";
	}

	strPath += "*.*";

	try{
		res = ff.FindFile(strPath);

		CString strFile = "";

		while(res){

			res = ff.FindNextFile();

			if (!ff.IsDots() && !ff.IsDirectory()){

				strFile = ff.GetFilePath();

				DeleteFile(strFile);
			}
		}

		res = TRUE;

	}catch(CFileException e)
	{
		//CString error;

		//TCHAR szCause[255];
		//error.Format("读文件出错!\r\n错误信息：%s",e.GetErrorMessage(szCause, 255, 0));
	
		res = FALSE;
	}
	
	return res;
}

//jpg文件名串转换成.bmp串
CString CBasicUse::JPGToBMPString(CString strJPGFileName)
{
	CString retStr = "";

	retStr = strJPGFileName;

	retStr.MakeLower();

	int intLen = 0;

	intLen = retStr.Find(".jpg",0);

	if (intLen <= 0){

		return "";
	}

	retStr = retStr.Mid(0,intLen);

	retStr = retStr + ".bmp";

	retStr.Format("%s",retStr);

	return retStr;
}

//bmp文件名串转换成.jpg串
CString CBasicUse::BMPToJPGString(CString strBMPFileName)
{
	CString retStr = "";

	retStr = strBMPFileName;

	retStr.MakeLower();

	int intLen = 0;

	intLen = retStr.Find(".bmp",0);

	if (intLen <= 0){

		return "";
	}

	retStr = retStr.Mid(0,intLen);

	retStr = retStr + ".jpg";

	retStr.Format("%s",retStr);

	return retStr;
}

//根据指定时间串取得日期串yyyymmdd
CString CBasicUse::GetDate_Connect(CString strDatetime)
{

	CString strDate = "";

	COleDateTime tDatetime;		//用此类比CTime好用

	tDatetime.ParseDateTime(strDatetime,0);

	int intYear = tDatetime.GetYear();

	int	intMonth = tDatetime.GetMonth();

	int intDay = tDatetime.GetDay();

	CString str = "";

	str.Format("%d",intYear);

	strDate = strDate + str;

	str.Format("%d",intMonth);

	if (str.GetLength() <= 1){
		str = "0" + str;
	}

	strDate = strDate + str;

	str.Format("%d",intDay);

	if (str.GetLength() <= 1){
		str = "0" + str;
	}

	strDate = strDate + str;

	strDate.Format("%s",strDate);
	
	return strDate;
}

//根据指定时间串取得日期时间串yyyymmddhhmmss
CString CBasicUse::GetDateTime_Connect(CString strDatetime)
{

	CString strDate = "";

	COleDateTime tDatetime;		//用此类比CTime好用

	tDatetime.ParseDateTime(strDatetime,0);

	int intYear = tDatetime.GetYear();

	int	intMonth = tDatetime.GetMonth();

	int intDay = tDatetime.GetDay();

	int intHour = tDatetime.GetHour();

	int intMinute = tDatetime.GetMinute();

	int intSecond = tDatetime.GetSecond();

	CString str = "";

	str.Format("%d",intYear);

	strDate = strDate + str;

	str.Format("%d",intMonth);

	if (str.GetLength() <= 1){
		str = "0" + str;
	}

	strDate = strDate + str;

	str.Format("%d",intDay);

	if (str.GetLength() <= 1){
		str = "0" + str;
	}

	strDate = strDate + str;

	//h,m,s
	str.Format("%d",intHour);

	if (str.GetLength() <= 1){
		str = "0" + str;
	}

	strDate = strDate + str;

	str.Format("%d",intMinute);

	if (str.GetLength() <= 1){
		str = "0" + str;
	}

	strDate = strDate + str;

	str.Format("%d",intSecond);

	if (str.GetLength() <= 1){
		str = "0" + str;
	}

	strDate = strDate + str;

	//
	strDate.Format("%s",strDate);
	
	return strDate;
}

/*动态加载dll
参数名：strDllName：动态库全称
*/
HINSTANCE CBasicUse::LoadDll(CString strDllName){

	HINSTANCE dllinstance; 
	
	dllinstance=::LoadLibrary(strDllName); 

	//如果无法打开，则退出系统
	if(dllinstance==NULL)   
	{
		return NULL;
	}
	
	//FreeLibrary(hDLL);	//卸载.dll文件

	return dllinstance;

}


//////////////全局函数/////////////////////////////////
//修正补0串，如"1" -> "01"///////////
CString GetMillFormat(int nMillNum,int nWidth)
{
	CString sReturn;
	sReturn.Format("%d",nMillNum);
	int nLen=strlen(sReturn);
	while (nLen<nWidth)
	{
		sReturn.Insert(0,"0");
		nLen=strlen(sReturn);
	}
	return sReturn ;
}

/////得到时间前缀：yyyymmddhhmmss_hm及 yyyy-mm-dd hh:mm:ss hm
CString GetFileNameRoot(char *sSource)
{
	CString sReturn ;
	COleDateTime t_timeA;//COleDateTime::GetCurrentTime();
	SYSTEMTIME  t_timeB;
	::GetSystemTime(&t_timeB);
	CString sT;
	sT=t_timeA.GetCurrentTime().Format(_T("%Y%m%d %H%M%S"))+"_"+GetMillFormat((int)t_timeB.wMilliseconds,3);
	sReturn.Format("%s",sT);
	
	CString sDBFlag;	//得到日期标准格式串
	sDBFlag=t_timeA.GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")) + ":" + GetMillFormat((int)t_timeB.wMilliseconds,3);
	wsprintf(sSource,"%s",sDBFlag);
	
	return sReturn ;		//得到日期连串,最后:的后边是毫秒
}

CString GetFileNameRoot()
{
	CString sReturn ;
	COleDateTime t_timeA;//COleDateTime::GetCurrentTime();
	SYSTEMTIME  t_timeB;
	::GetSystemTime(&t_timeB);
	CString sT;
	sT=t_timeA.GetCurrentTime().Format(_T("%Y%m%d %H%M%S"))+"_"+GetMillFormat((int)t_timeB.wMilliseconds,3);
	sReturn.Format("%s",sT);
	
	return sReturn ;
} 