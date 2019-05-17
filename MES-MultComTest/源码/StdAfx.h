// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__5A72C70D_404D_4C83_A51A_B10919679674__INCLUDED_)
#define AFX_STDAFX_H__5A72C70D_404D_4C83_A51A_B10919679674__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
extern "C" CString DeviceCountM;
extern "C" int ReIniCount;
#include <afxsock.h>		// MFC socket extensions

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持

//导入ADO组件
//#import "D:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF")
//#import "C:\ado\msado15.dll" no_namespace rename("EOF","adoEOF")

extern CString UserNameDB;

//三合一新增功能
extern int g_ADCTFlag;//因为要实现三合一的缘故，所以这里做一个标志位来实现自动登陆和自动获取数据
extern int g_IsHideFlag;//是否隐藏窗口
extern int g_ToolFlag;//分工位发工具
extern int g_ExitFlag;//退出标志位
extern int g_WaitTimeoutFlag;//等待连接超时标志位
extern CString g_TesterIdStr;//测试人ID

extern CString g_BackDoorIP;//这里是一个后门
extern CString g_BackDoorDatabase;
extern CString g_BackDoorUser;
extern CString g_BackDoorPassword ;
extern CString g_BackDoorSocket;

#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF")
//#import "E:\工作资料\软件部门\winxp\ado\msado15.dll" no_namespace rename("EOF","adoEOF")
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__5A72C70D_404D_4C83_A51A_B10919679674__INCLUDED_)
