package com.jimi.mes_server.interceptor;


import com.jfinal.aop.Interceptor;
import com.jfinal.aop.Invocation;
import com.jfinal.core.Controller;
import com.jimi.mes_server.annotation.TestLog;
import com.jimi.mes_server.controller.UserController;
import com.jimi.mes_server.entity.vo.LUserAccountVO;
import com.jimi.mes_server.model.TestSystemSetting;
import com.jimi.mes_server.model.TestSystemSettingFunc;
import com.jimi.mes_server.model.TestSystemSettingLog;
import com.jimi.mes_server.model.TestSystemSettingOqc;
import com.jimi.mes_server.util.TokenBox;

import javax.servlet.http.HttpServletRequest;
import java.util.Date;

public class TestActionLogInterceptor implements Interceptor {

    @Override
    public void intercept(Invocation inv) {
        TestLog testLog = inv.getMethod().getAnnotation(TestLog.class);
        Controller controller = inv.getController();
        TestSystemSettingLog testSystemSettingLog = new TestSystemSettingLog();

        if (testLog != null) {
            HttpServletRequest request = controller.getRequest();
            LUserAccountVO user = TokenBox.get(controller.getPara(TokenBox.TOKEN_ID_KEY_NAME), UserController.SESSION_KEY_LOGIN_USER);
            testSystemSettingLog.setOperator(user.getName());
            String servletPath = request.getServletPath();

            int settingType = controller.getParaToInt("type");
            if (servletPath.equals("/test/create")) {
                testSystemSettingLog.setOperationType(0);
                testSystemSettingLog.setSoftVersion(controller.getPara("softVersion"));
                testSystemSettingLog.setOrderName(controller.getPara("orderName"));
            } else {
                TestSystemSetting coupleSetTing = null;
                TestSystemSettingFunc functionSetTing = null;
                TestSystemSettingOqc oqcSetTing = null;
                testSystemSettingLog.setOperationType(1);
                String softWare = controller.getPara("softWare");
                switch (settingType) {
                    case 0:
                        functionSetTing = TestSystemSettingFunc.dao.use("db1").findById(softWare);
                        if (functionSetTing != null) {
                            testSystemSettingLog.setSoftVersion(functionSetTing.getSoftVersion());
                            testSystemSettingLog.setOrderName(functionSetTing.getOrderName());
                        }
                    case 1:
                        functionSetTing = TestSystemSettingFunc.dao.use("db2").findById(softWare);
                        if (functionSetTing != null) {
                            testSystemSettingLog.setSoftVersion(functionSetTing.getSoftVersion());
                            testSystemSettingLog.setOrderName(functionSetTing.getOrderName());
                        }
                    case 2:
                        coupleSetTing = TestSystemSetting.dao.use("db2").findById(softWare);
                        if (coupleSetTing != null) {
                            testSystemSettingLog.setSoftVersion(coupleSetTing.getSoftVersion());
                            testSystemSettingLog.setOrderName(coupleSetTing.getOrderName());
                        }
                    case 3:
                        functionSetTing = TestSystemSettingFunc.dao.use("db3").findById(softWare);
                        if (functionSetTing != null) {
                            testSystemSettingLog.setSoftVersion(functionSetTing.getSoftVersion());
                            testSystemSettingLog.setOrderName(functionSetTing.getOrderName());
                        }
                    case 4:
                        coupleSetTing = TestSystemSetting.dao.use("db3").findById(softWare);
                        if (coupleSetTing != null) {
                            testSystemSettingLog.setSoftVersion(coupleSetTing.getSoftVersion());
                            testSystemSettingLog.setOrderName(coupleSetTing.getOrderName());
                        }
                    case 5:
                        oqcSetTing = TestSystemSettingOqc.dao.use("db2").findById(softWare);
                        if (oqcSetTing != null) {
                            testSystemSettingLog.setSoftVersion(oqcSetTing.getSoftVersion());
                            testSystemSettingLog.setOrderName(oqcSetTing.getOrderName());
                        }
                }
            }
            testSystemSettingLog.setSettingType(settingType);
            testSystemSettingLog.setStation(controller.getPara("station"));
            testSystemSettingLog.setModel(controller.getPara("machineName"));
            testSystemSettingLog.setTime(new Date());
            try {
                inv.invoke();
                testSystemSettingLog.setResult(true);
                testSystemSettingLog.save();
            } catch (Exception e) {
                testSystemSettingLog.setResult(false);
                testSystemSettingLog.save();
                throw e;
            }
        } else {
            inv.invoke();
        }
    }
}
