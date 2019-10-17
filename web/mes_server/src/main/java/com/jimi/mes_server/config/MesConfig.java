package com.jimi.mes_server.config;

import java.io.File;

import com.jfinal.config.Constants;
import com.jfinal.config.Handlers;
import com.jfinal.config.Interceptors;
import com.jfinal.config.JFinalConfig;
import com.jfinal.config.Plugins;
import com.jfinal.config.Routes;
import com.jfinal.kit.PropKit;
import com.jfinal.plugin.activerecord.ActiveRecordPlugin;
import com.jfinal.plugin.activerecord.dialect.SqlServerDialect;
import com.jfinal.plugin.activerecord.tx.Tx;
import com.jfinal.plugin.cron4j.Cron4jPlugin;
import com.jfinal.plugin.druid.DruidPlugin;
import com.jfinal.template.Engine;
import com.jimi.mes_server.controller.DeleteHistoryController;
import com.jimi.mes_server.controller.OrderController;
import com.jimi.mes_server.controller.ProductionController;
import com.jimi.mes_server.controller.RedTeaController;
import com.jimi.mes_server.controller.ReportController;
import com.jimi.mes_server.controller.ReworkController;
import com.jimi.mes_server.controller.SopController;
import com.jimi.mes_server.controller.TestController;
import com.jimi.mes_server.controller.UserController;
import com.jimi.mes_server.interceptor.AccessInterceptor;
import com.jimi.mes_server.interceptor.CORSInterceptor;
import com.jimi.mes_server.interceptor.ErrorLogInterceptor;
import com.jimi.mes_server.model.MappingKit;
import com.jimi.mes_server.model.NetMarkIMEI;
import com.jimi.mes_server.model.TestSystemSetting;
import com.jimi.mes_server.model.TestSystemSettingFunc;
import com.jimi.mes_server.model.TestSystemSettingOqc;
import com.jimi.mes_server.util.CommonUtil;
import com.jimi.mes_server.util.TokenBox;
import com.jimi.mes_server.websocket.handler.WebSocketHandler;
import com.jimi.mes_server.websocket.logger.PackageLogger;

import cc.darhao.pasta.Pasta;

/**
 * 全局配置 <br>
 * <b>2018年5月22日</b>
 * 
 * @author 沫熊工作室 <a href="http://www.darhao.cc">www.darhao.cc</a>
 */
public class MesConfig extends JFinalConfig {

	@Override
	public void configConstant(Constants me) {
		me.setDevMode(true);
		// 设置默认上传文件保存路径 getFile等使用
		me.setBaseUploadPath("upload/temp/");
		// 设置上传最大限制尺寸
		me.setMaxPostSize(Integer.MAX_VALUE);
	}


	@Override
	public void configEngine(Engine me) {
	}


	@Override
	public void configHandler(Handlers me) {
		/*me.add(new WebSocketHandler("^/sopserver"));*/
	}


	@Override
	public void configInterceptor(Interceptors me) {
		me.addGlobalActionInterceptor(new ErrorLogInterceptor());
		me.addGlobalActionInterceptor(new CORSInterceptor());
		/*me.addGlobalActionInterceptor(new AccessInterceptor());*/
		me.addGlobalServiceInterceptor(new Tx());
	}


	@Override
	public void configPlugin(Plugins me) {
		PropKit.use("properties.ini");
		DruidPlugin dp = null;
		DruidPlugin dp1 = null;
		DruidPlugin dp2 = null;
		DruidPlugin dp3 = null;
		String flagPath = null;
		if (CommonUtil.isWindows()) {
			flagPath = PropKit.get("windowsFlagPath");
		} else {
			flagPath = PropKit.get("linuxFlagPath");
		}
		// 判断是否是生产环境，配置数据连接池
		if (isProductionEnvironment(flagPath)) {
			dp = new DruidPlugin(PropKit.get("p_url"), PropKit.get("p_user"), PropKit.get("p_password"));
			dp1 = new DruidPlugin(PropKit.get("p_url1"), PropKit.get("p_user"), PropKit.get("p_password"));
			dp2 = new DruidPlugin(PropKit.get("p_url2"), PropKit.get("p_user"), PropKit.get("p_password"));
			dp3 = new DruidPlugin(PropKit.get("p_url3"), PropKit.get("p_user"), PropKit.get("p_password"));
			System.out.println("DateBase is in production envrionment");
		} else {
			dp = new DruidPlugin(PropKit.get("d_url"), PropKit.get("d_user"), PropKit.get("d_password"));
			dp1 = new DruidPlugin(PropKit.get("d_url1"), PropKit.get("d_user"), PropKit.get("d_password"));
			dp2 = new DruidPlugin(PropKit.get("d_url2"), PropKit.get("d_user"), PropKit.get("d_password"));
			dp3 = new DruidPlugin(PropKit.get("d_url3"), PropKit.get("d_user"), PropKit.get("d_password"));
			System.out.println("DateBase is in development envrionment");
		}
		me.add(dp);
		me.add(dp1);
		me.add(dp2);
		me.add(dp3);
		// 配置ORM
		ActiveRecordPlugin arp = new ActiveRecordPlugin(dp);
		ActiveRecordPlugin arp1 = new ActiveRecordPlugin("db1", dp1);
		ActiveRecordPlugin arp2 = new ActiveRecordPlugin("db2", dp2);
		ActiveRecordPlugin arp3 = new ActiveRecordPlugin("db3", dp3);
		arp.setDialect(new SqlServerDialect());
		arp1.setDialect(new SqlServerDialect());
		arp2.setDialect(new SqlServerDialect());
		arp3.setDialect(new SqlServerDialect());
		arp.setShowSql(true);
		arp1.setShowSql(true);
		arp2.setShowSql(true);
		arp3.setShowSql(true);
		MappingKit.mapping(arp);
		arp1.addMapping("TestSystemSetting", "SoftWare", TestSystemSetting.class);
		arp1.addMapping("TestSystemSettingFunc", "SoftWare", TestSystemSettingFunc.class);
		arp1.addMapping("NetMarkIMEI", "Id", NetMarkIMEI.class);
		arp2.addMapping("TestSystemSetting", "SoftWare", TestSystemSetting.class);
		arp2.addMapping("TestSystemSettingFunc", "SoftWare", TestSystemSettingFunc.class);
		arp2.addMapping("TestSystemSettingOqc", "SoftWare", TestSystemSettingOqc.class);
		arp3.addMapping("TestSystemSetting", "SoftWare", TestSystemSetting.class);
		arp3.addMapping("TestSystemSettingFunc", "SoftWare", TestSystemSettingFunc.class);
		me.add(arp);
		me.add(arp1);
		me.add(arp2);
		me.add(arp3);
		Cron4jPlugin cron = new Cron4jPlugin(PropKit.use("taskConfig.txt"), "cron4j");
		me.add(cron);
	}


	@Override
	public void configRoute(Routes me) {
		me.add("/report", ReportController.class);
		me.add("/order", OrderController.class);
		me.add("/user", UserController.class);
		me.add("/redTea", RedTeaController.class);
		me.add("/test", TestController.class);
		me.add("/history", DeleteHistoryController.class);
		me.add("/rework", ReworkController.class);
		me.add("/production", ProductionController.class);
		me.add("/sop", SopController.class);
	}


	@Override
	public void afterJFinalStart() {
		TokenBox.start(PropKit.use("properties.ini").getInt("sessionTimeout"));
		System.out.println("Mes Server is Running now...");
		/*Pasta.setLogCatcher(new PackageLogger());*/
	}


	@Override
	public void beforeJFinalStop() {
		TokenBox.stop();
	}


	public static boolean isProductionEnvironment() {
		File[] roots = File.listRoots();
		for (int i = 0; i < roots.length; i++) {
			if (new File(roots[i].toString() + "PRODUCTION_ENVIRONMENT_FLAG").exists()) {
				return true;
			}
		}
		return false;
	}


	public static boolean isProductionEnvironment(String path) {
		if (new File(path + "PRODUCTION_ENVIRONMENT_FLAG").exists()) {
			return true;
		}
		return false;
	}

}
