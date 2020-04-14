package com.jimi.mes_report.util;

import java.util.ArrayList;
import java.util.List;

import com.jimi.mes_report.model.entity.*;

public class DateFormat {

    private String imeiDate;

    private String printDate;

    private String testDate;

    public DateFormat(String date){
        this.imeiDate = date.replace("-0","/").replace("-", "/");
        this.printDate = date.replace("-",".");
        this.testDate = date;
    }

    public String getImeiDate() {
        return imeiDate;
    }

    public void setImeiDate(String imeiDate) {
        this.imeiDate = imeiDate;
    }

    public String getPrintDate() {
        return printDate;
    }

    public void setPrintDate(String printDate) {
        this.printDate = printDate;
    }

    public String getTestDate() {
        return testDate;
    }

    public void setTestDate(String testDate) {
        this.testDate = testDate;
    }

    public String[] getTime(){
        String cpTimeStart = this.imeiDate + " 00:00:00";
        String cpTimeEnd = this.imeiDate + " 23:59:59";
        String printTimeStart = this.printDate + " 00:00:00:000";
        String printTimeEnd = this.printDate + " 23:59:59:999";
        String testTimeStart = this.testDate + " 00:00:00";
        String testTimeEnd = this.testDate + " 23:59:59";
        String[] time = new String[]{cpTimeStart,cpTimeEnd,testTimeStart,testTimeEnd,testTimeStart,testTimeEnd,testTimeStart,testTimeEnd,testTimeStart,testTimeEnd
                ,testTimeStart,testTimeEnd,printTimeStart,printTimeEnd,printTimeStart,printTimeEnd};
        return time;
    }

    public  String[] getTimeColumn(int type,String zhidan){
        List<String> column = new ArrayList<>();
        String startTime;
        String endTime;
        if (type == WorkstationType.COLOR_BOX_STICKING || type == WorkstationType.BODY_STICKING){
            for(int hour = 1;hour <= 24;hour++) {
                startTime = this.printDate + transHour(hour - 1) + ":00:00:000";
                endTime = this.printDate + transHour(hour - 1) + ":59:59:999";
                column.add(startTime);
                column.add(endTime);
                column.add(zhidan);
            }
        }else{
            String date;
            for(int hour = 1;hour <= 24;hour++) {
                if (type == WorkstationType.IMEI_COMPARISON ){
                    date = this.imeiDate;
                    column.add(zhidan);
                }else {
                    date = this.testDate;
                    column.add(zhidan);
                }
                startTime = date + transHour(hour - 1) + ":00:00:00";
                endTime = date + transHour(hour - 1) + ":59:59:99";
                column.add(startTime);
                column.add(endTime);
            }
        }
        String[] strs = column.toArray(new String[column.size()]);
        return strs;
    }

    private static String transHour(int hour){
        return hour < 10 ? " 0"+ hour : " "+hour;
    }


}
