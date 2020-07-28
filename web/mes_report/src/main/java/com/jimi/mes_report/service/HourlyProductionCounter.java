package com.jimi.mes_report.service;

import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Record;
import com.jimi.mes_report.constant.DailyProductionSql;
import com.jimi.mes_report.constant.StationDailyProductionSql;

import java.util.List;

import static com.jimi.mes_report.model.entity.WorkstationType.*;

public class HourlyProductionCounter {

    public static List<Record> count(int type,Object[] column){
        List<Record> record = null;
        switch (type){
            case SMT :
                record = Db.use("db1").find(DailyProductionSql.SELECT_SMT_PRODUCTION,column);
                break;
            case ASSEMBLE:
                record = Db.use("db1").find(DailyProductionSql.SELECT_ASSEMBLE_PRODUCTION,column);
                break;
            case AGEING:
                record = Db.use("db1").find(DailyProductionSql.SELECT_AGEING_PRODUCTION,column);
                break;
            case COUPLING:
                record = Db.use("db1").find(DailyProductionSql.SELECT_COUPLING_PRODUCTION,column);
                break;
            case BODY_STICKING:
                record = Db.use("db1").find(DailyProductionSql.SELECT_BODY_STICKING_PRODUCTION,column);
                break;
            case COLOR_BOX_STICKING:
                record = Db.use("db1").find(DailyProductionSql.SELECT_COLOR_BOX_PRODUCTION,column);
                break;
            case IMEI_COMPARISON:
                record = Db.use("db1").find(DailyProductionSql.SELECT_IMEI_PRODUCTION,column);
                break;
            case CARTON:
                record = Db.use("db1").find(DailyProductionSql.SELECT_CARTON_PRODUCTION,column);
                break;
        }
        return record;
    }
    
    public static List<Record> countStation(int type,Object[] column){
        List<Record> record = null;
        switch (type){
            case SMT :
                record = Db.use("db1").find(StationDailyProductionSql.SMT_COMPUTER_DAILAY_PRODUCTION_SQL,column);
                break;
            case ASSEMBLE:
                record = Db.use("db1").find(StationDailyProductionSql.ASSEMBLE_COMPUTER_DAILAY_PRODUCTION_SQL,column);
                break;
            case AGEING:
                record = Db.use("db1").find(StationDailyProductionSql.AGEING_COMPUTER_DAILAY_PRODUCTION_SQL,column);
                break;
            case COUPLING:
                record = Db.use("db1").find(StationDailyProductionSql.COUPLING_COMPUTER_DAILAY_PRODUCTION_SQL,column);
                break;
            case CARTON:
                record = Db.use("db1").find(StationDailyProductionSql.CARTON_COMPUTER_DAILAY_PRODUCTION_SQL,column);
                break;
            default:
        }
        return record;
    }

}
