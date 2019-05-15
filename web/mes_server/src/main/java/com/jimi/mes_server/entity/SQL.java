package com.jimi.mes_server.entity;

public class SQL {

	public final static String SELECT_USER_BY_NAME_PASSWORD = "SELECT * FROM LUserAccount where Name = ? and Password = ?";

	public final static String SELECT_WEBUSERTYPE = "SELECT * FROM WebUserType where TypeId <> 4";
	
	public final static String SELECT_TYPENAME_BY_TYPEID = "SELECT * FROM WebUserType where TypeId = ?";
	
}
