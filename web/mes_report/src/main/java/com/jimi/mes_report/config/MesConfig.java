package com.jimi.mes_report.config;

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
import com.jfinal.plugin.cron4j.Cron4jPlugin;
import com.jfinal.plugin.druid.DruidPlugin;
import com.jfinal.plugin.redis.RedisPlugin;
import com.jfinal.template.Engine;
import com.jfplugin.mail.MailPlugin;
import com.jimi.mes_report.model.MappingKit;
import com.jimi.mes_report.util.VisualSerializer;

/**全局配置
 * @author   HCJ
 * @date     2019年5月5日 下午2:32:43
 */
public class MesConfig extends JFinalConfig {


	@Override
	public void configConstant(Constants me) {
		me.setDevMode(true);
	}


	@Override
	public void configEngine(Engine me) {
	}


	@Override
	public void configHandler(Handlers me) {
	}


	@Override
	public void configInterceptor(Interceptors me) {
	}


	@Override
	public void configPlugin(Plugins me) {
		PropKit.use("properties.ini");
		DruidPlugin dp = null;
		DruidPlugin dp1 = null;
		RedisPlugin rp = null;
		// 判断是否是生产环境，配置数据连接池
		if (isProductionEnvironment()) {
			dp = new DruidPlugin(PropKit.get("p_url"), PropKit.get("p_user"), PropKit.get("p_password"));
			dp1 = new DruidPlugin(PropKit.get("p_url1"), PropKit.get("p_user"), PropKit.get("p_password"));
			rp = new RedisPlugin("MES_REPORT", PropKit.get("p_redisIp"), PropKit.get("p_redisPassword"));
			System.out.println("DateBase is in production envrionment");
		} else if (isTestEnvironment()) {
			dp = new DruidPlugin(PropKit.get("t_url"), PropKit.get("t_user"), PropKit.get("t_password"));
			dp1 = new DruidPlugin(PropKit.get("t_url1"), PropKit.get("t_user"), PropKit.get("t_password"));
			rp = new RedisPlugin("MES_REPORT", PropKit.get("t_redisIp"));
			System.out.println("DateBase is in test envrionment");
		} else {
			dp = new DruidPlugin(PropKit.get("d_url"), PropKit.get("d_user"), PropKit.get("d_password"));
			dp1 = new DruidPlugin(PropKit.get("d_url1"), PropKit.get("d_user"), PropKit.get("d_password"));
			rp = new RedisPlugin("MES_REPORT", PropKit.get("d_redisIp"), PropKit.get("d_redisPassword"));
			System.out.println("DateBase is in development envrionment");
		}
		rp.setSerializer(new VisualSerializer());
		me.add(dp);
		me.add(dp1);
		me.add(rp);
		// 配置ORM
		ActiveRecordPlugin arp = new ActiveRecordPlugin(dp);
		ActiveRecordPlugin arp1 = new ActiveRecordPlugin("db1", dp1);
		arp.setDialect(new SqlServerDialect());
		arp1.setDialect(new SqlServerDialect());
		arp.setShowSql(true);
		arp1.setShowSql(true);
		MappingKit.mapping(arp);
		me.add(arp);
		me.add(arp1);
		Cron4jPlugin cron = new Cron4jPlugin(PropKit.use("taskConfig.txt"), "cron4j");
		me.add(cron);
		me.add(new MailPlugin(PropKit.use("mail.properties").getProperties()));
	}


	@Override
	public void configRoute(Routes me) {
	}


	@Override
	public void afterJFinalStart() {
		System.out.println("Mes Server is Running now...");
	}


	@Override
	public void beforeJFinalStop() {
	}


	/**是否处于开发环境
	 * @date 2019年5月5日 下午2:33:29
	 */
	public static boolean isProductionEnvironment() {
		File[] roots = File.listRoots();
		for (int i = 0; i < roots.length; i++) {
			if (new File(roots[i].toString() + "PRODUCTION_ENVIRONMENT_FLAG").exists()) {
				return true;
			}
		}
		return false;
	}


	/**是否处于测试环境
	 * @date 2019年5月5日 下午2:33:29
	 */
	public static boolean isTestEnvironment() {
		File[] roots = File.listRoots();
		for (int i = 0; i < roots.length; i++) {
			if (new File(roots[i].toString() + "TEST_ENVIRONMENT_FLAG").exists()) {
				return true;
			}
		}
		return false;
	}

}
