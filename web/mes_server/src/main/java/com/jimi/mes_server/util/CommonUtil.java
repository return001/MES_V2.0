package com.jimi.mes_server.util;

import java.io.File;
import java.io.FileFilter;
import java.io.FileInputStream;
import java.math.BigInteger;
import java.security.MessageDigest;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.jfinal.kit.PathKit;

public class CommonUtil {

	/**
	 * 拼接文件路径（web根目录/basePath/params1/params2...）
	 * @param basePath
	 * @param params
	 * @return
	 */
	public static String getFilePath(String basePath, String... params) {
		StringBuffer filePath = new StringBuffer();
		filePath.append(PathKit.getWebRootPath());
		filePath.append(File.separator);
		filePath.append(basePath);
		for (String param : params) {
			filePath.append(File.separator);
			filePath.append(param);
		}
		filePath.append(File.separator);
		return filePath.toString();
	}


	/**
	 * 根据路径和文件名(无文件后缀)查找文件，只返回一个文件
	 * @param filePath
	 * @param fileName
	 * @return
	 */
	public static File findFile(String filePath, final String fileName) {
		File dir = new File(filePath);
		File[] tempFile = dir.listFiles(new FileFilter() {

			@Override
			public boolean accept(File file) {
				String name = file.getName().substring(0, file.getName().lastIndexOf("."));
				if (name.equals(fileName)) {
					return true;
				}
				return false;
			}
		});
		if (tempFile == null || tempFile.length <= 0) {
			return null;
		}
		return tempFile[0];
	}


	public static boolean rename(String path, String oldName, String newName) {
		if (!oldName.equals(newName)) {
			File oldFile = new File(path + oldName);
			File newFile = new File(path + newName);
			if (newFile.exists()) {
				return false;
			}
			oldFile.renameTo(newFile);
		}
		return true;
	}


	/**
	 * 根据文件路径提取MD5值
	 * @param path 文件路径
	 * @date 2019年8月10日 下午3:13:04
	 */
	public static String getMD5(String path) {
		StringBuffer sb = new StringBuffer();
		try (FileInputStream fis = new FileInputStream(new File(path));) {
			MessageDigest md = MessageDigest.getInstance("MD5");
			byte[] buffer = new byte[1024];
			int length = -1;
			while ((length = fis.read(buffer, 0, 1024)) != -1) {
				md.update(buffer, 0, length);
			}
			BigInteger bigInt = new BigInteger(1, md.digest());
			sb.append(bigInt.toString(16).toUpperCase());
		} catch (Exception e) {
			e.printStackTrace();
		}
		return sb.toString();
	}


	/**@author HCJ
	 * 判断是否为整数
	 * @param input 输入字符串
	 * @date 2019年8月10日 下午3:44:25
	 */
	public static boolean isInteger(String input) {
		String regex = "^-?[1-9]\\d*$";
		return match(regex, input);
	}


	public static boolean isPositiveInteger(String input) {
		String regex = "^[1-9]\\d*|0$";
		return match(regex, input);
	}


	public static boolean isEmail(String input) {
		String regex = "[\\w!#$%&'*+/=?^_`{|}~-]+(?:\\.[\\w!#$%&'*+/=?^_`{|}~-]+)*@(?:[\\w](?:[\\w-]*[\\w])?\\.)+[\\w](?:[\\w-]*[\\w])?";
		return match(regex, input);
	}


	public static boolean isIPAddress(String input) {
		String regex = "([1-9]|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])(\\.(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])){3}";
		return match(regex, input);
	}


	/**@author HCJ
	 * 判断是否是windows系统下
	 * @date 2019年9月25日 上午10:17:37
	 */
	public static boolean isWindows() {
		return System.getProperty("os.name").toLowerCase().indexOf("windows") >= 0;
	}


	public static void main(String[] args) {
		getMD5("C:\\Users\\HCJ\\Desktop\\配置文件\\配置文件\\GT02F_20_61DM2_B25E_R0_V02_20151112_1042.rar");
	}


	private static boolean match(String regex, String input) {
		Pattern pattern = Pattern.compile(regex);
		Matcher matcher = pattern.matcher(input);
		return matcher.matches();
	}
}
