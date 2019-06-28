package com.jimi.mes_server.service;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.HashMap;
import java.util.Map;

import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.HttpClientBuilder;
import org.apache.http.util.EntityUtils;

import com.alibaba.fastjson.JSON;

public class RedTeaService {

	private static final String ACCESS_KEY = "d76d68e78dc34a9186510b4271b93f1a";

	private static final String SECRET_KEY = "586644ad23c14ac2868b606acef64ae4";

	private static final String URL = "http://redtea.easyiot.ai:8765/api/agent/cidByDeviceInfo";


	public String getCId(String sn, String deviceModel, String imei) throws ClientProtocolException, IOException {
		String resp = "";
		String body = makeJsonString(sn, deviceModel, imei);
		String stringToSign = MD5(body).concat(SECRET_KEY);
		String signature = SHA1(stringToSign);
		System.out.println("MD5:" + MD5(body));
		System.out.println("请求参数:" + body);
		System.out.println("签名:" + signature);
		resp = makeRequest(body, signature);
		return resp;
	}


	public static String makeRequest(String body, String signature) throws ClientProtocolException, IOException {
		HttpClient client = HttpClientBuilder.create().build();
		HttpPost post = new HttpPost(URL);
		post.setHeader("Content-Type", "application/json");
		post.setHeader("signature", signature);
		post.setEntity(new StringEntity(body));
		HttpResponse response = client.execute(post);
		String resp = EntityUtils.toString(response.getEntity());
		System.out.println("响应:" + resp);
		return resp;
	}


	public static String makeJsonString(String sn, String deviceModel, String imei) {
		long timestamp = System.currentTimeMillis();

		Map<String, Object> metadata = new HashMap<>();
		metadata.put("accessKey", ACCESS_KEY);
		metadata.put("signType", "SHA1");
		metadata.put("timestamp", timestamp);

		Map<String, Object> data = new HashMap<>();
		data.put("sn", sn);
		data.put("imei", imei);
		data.put("deviceModel", deviceModel);

		Map<String, Object> content = new HashMap<>();
		content.put("tranId", "" + timestamp);
		content.put("data", data);

		Map<String, Object> body = new HashMap<>();
		body.put("metadata", metadata);
		body.put("content", content);

		return JSON.toJSONString(body);
	}


	public static String MD5(String input) {
		MessageDigest md = null;

		try {
			md = MessageDigest.getInstance("MD5");
			md.update(input.getBytes("UTF-8"));
		} catch (NoSuchAlgorithmException e) {
		} catch (UnsupportedEncodingException e) {
		}

		byte byteData[] = md.digest();
		return bytesToHex(byteData).toLowerCase();
	}


	public static String SHA1(String input) {
		MessageDigest md = null;
		try {
			md = MessageDigest.getInstance("SHA1");
			md.update(input.getBytes("UTF-8"));
		} catch (NoSuchAlgorithmException e) {
		} catch (UnsupportedEncodingException e) {
		}

		byte byteData[] = md.digest();
		return bytesToHex(byteData).toLowerCase();
	}


	public static String bytesToHex(byte[] bytes) {
		char[] hexArray = "0123456789ABCDEF".toCharArray();
		char[] hexChars = new char[bytes.length * 2];
		for (int j = 0; j < bytes.length; j++) {
			int v = bytes[j] & 0xFF;
			hexChars[j * 2] = hexArray[v >>> 4];
			hexChars[j * 2 + 1] = hexArray[v & 0x0F];
		}
		return new String(hexChars);
	}

}
