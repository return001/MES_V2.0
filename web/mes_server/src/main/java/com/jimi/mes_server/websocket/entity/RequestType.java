package com.jimi.mes_server.websocket.entity;

public class RequestType {

	// 我方请求类型
	public static final String SHOW = "show";
	public static final String CANCELSHOW = "cancelShow";
	public static final String CANCELPICTURE = "cancelPicture";
	public static final String CANCELNOTICE = "cancelNotice";

	// 对方请求类型
	public static final String LOGIN = "login";
	public static final String LOGOUT = "logout";
	public static final String CONFIRM = "confirm";
}
