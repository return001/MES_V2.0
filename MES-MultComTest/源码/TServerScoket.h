#pragma once

#include"SocketArrayList.h"
#include<WinSock2.h>

//全局变量或者全局函数
BOOL socket_Select(SOCKET hSocket, DWORD nTimeOut, BOOL bRead);
DWORD WINAPI ListenThreadFunc(LPVOID Lparam);
DWORD WINAPI ClientThreadProc(LPVOID Lparam);


class TServerScoket
{
public:
	TServerScoket();
	~TServerScoket();

private:
	BOOL equal(const CClientItem *p1, const CClientItem * p2);
	LRESULT OnTrayCallbackMsg(WPARAM wparam, LPARAM lparam);

/*变量以及函数*/
public:
	BOOL m_isServerOpen;
	SOCKET m_SockListen;
	UINT m_ServicePort;
	HANDLE m_hListenThread;
	CArray<CClientItem> m_ClientArray;
	BOOL m_RecvFlag, m_SimpleRecvFlag;//接收与否标志位,m_RecvFlag为图片接收标志位， m_SimpleRecvFlag为普通消息标志位
	CString m_PicName;//图片名字
	CString m_PicData;//图片数据
	CString m_SimpleData;//普通消息数据

	BOOL StartServer(UINT m_ServicePort);//开启服务器
	void StopServer();//关闭服务器
	void OpenRecv(CString PicName);//打开接收
	void CloseRecv();//关闭接收
	BOOL CreatePic();//生成图片

	void OpenSimpleRecv();//打开普通消息接收
	void CloseSimpleRecv();//关闭普通消息接收


	void RemoveClientFromArray(CClientItem in_item);
};
