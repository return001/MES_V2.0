package com.jimi.mes_server.exception;

/**
 * 权限不足异常，result：401
 * <br>
 * <b>2018年6月2日</b>
 * @author 沫熊工作室 <a href="http://www.darhao.cc">www.darhao.cc</a>
 */
public class AccessException extends RuntimeException {

	/**
	 * serialVersionUID : TODO
	 */
	private static final long serialVersionUID = 3003607108310165286L;

	public AccessException(String message) {
		super(message);
	}
	
}
