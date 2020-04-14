package com.jimi.mes_report.model.entity;

import java.util.*;

public class DailyProductionReportItem {
    private String orderNo;
    private String model;
    private String productNo;
    private String version;
    private int planProductionQuantity;
    private int productionQuantity;
    private int productionDays;
    private float completionRate;
    private float capacity;
    private float highestCapacityInHistory;
    private Date startDate;
    private Date planDate;
    private Map<Integer, List<HourStatisticsItem>> hourStatisticsItems;

    /**
     * 获取订单号
     */
    public String getOrderNo() {
        return orderNo;
    }

    /**
     * 设置订单号
     */
    public void setOrderNo(String orderNo) {
        this.orderNo = orderNo;
    }

    /**
     * 获取机型
     */
    public String getModel() {
        return model;
    }

    /**
     * 设置机型
     */
    public void setModel(String model) {
        this.model = model;
    }

    /**
     * 获取产品编号
     */
    public String getProductNo() {
        return productNo;
    }

    /**
     * 设置产品编号
     */
    public void setProductNo(String productNo) {
        this.productNo = productNo;
    }

    /**
     * 获取软件版本
     */
    public String getVersion() {
        return version;
    }

    /**
     * 设置软件版本
     */
    public void setVersion(String version) {
        this.version = version;
    }

    /**
     * 获取订单数量
     */
    public int getPlanProductionQuantity() {
        return planProductionQuantity;
    }

    /**
     * 设置订单数量
     */
    public void setPlanProductionQuantity(int planProductionQuantity) {
        this.planProductionQuantity = planProductionQuantity;
    }

    /**
     * 获取实际完成数量
     */
    public int getProductionQuantity() {
        return productionQuantity;
    }

    /**
     * 设置实际完成数量
     */
    public void setProductionQuantity(int productionQuantity) {
        this.productionQuantity = productionQuantity;
    }

    /**
     * 获取在制时长
     */
    public int getProductionDays() {
        return productionDays;
    }

    /**
     * 设置在制时长
     */
    public void setProductionDays(int productionDays) {
        this.productionDays = productionDays;
    }

    /**
     * 获取完成率（如12.05%，就是12.05，而不是0.1205）
     */
    public float getCompletionRate() {
        return completionRate;
    }

    /**
     * 设置完成率（如12.05%，就是12.05，而不是0.1205）
     */
    public void setCompletionRate(float completionRate) {
        this.completionRate = completionRate;
    }

    /**
     * 获取实际每小时产能
     */
    public float getCapacity() {
        return capacity;
    }

    /**
     * 设置实际每小时产能
     */
    public void setCapacity(float capacity) {
        this.capacity = capacity;
    }

    /**
     * 获取历史最高每小时产能
     */
    public float getHighestCapacityInHistory() {
        return highestCapacityInHistory;
    }

    /**
     * 设置历史最高每小时产能
     */
    public void setHighestCapacityInHistory(float highestCapacityInHistory) {
        this.highestCapacityInHistory = highestCapacityInHistory;
    }


    /**
     * 获取上线起始日期
     */
    public Date getStartDate() {
        return startDate;
    }

    /**
     * 设置上线起始日期
     */
    public void setStartDate(Date startDate) {
        this.startDate = startDate;
    }

    /**
     * 获取计划完成日期
     */
    public Date getPlanDate() {
        return planDate;
    }

    /**
     * 设置计划完成日期
     */
    public void setPlanDate(Date planDate) {
        this.planDate = planDate;
    }



    /**
     * 添加整点统计条目
     * @param type 工位类型，参考本类常量
     * @param hour 小时
     * @param output 完成量
     * @param consumingTime 实际用时（秒）
     */
    public void addHourStatisticsItem(int type, int hour, int output, int consumingTime) {
        List list = getHourStatisticsItems(type);
        list.add(new HourStatisticsItem(hour, output, consumingTime));
    }


    /**
     * 根据工位类型获取整点统计条目
     * @param type 工位类型，参考本类常量
     */
    public List getHourStatisticsItems(int type) {
        return hourStatisticsItems.get(type);
    }


    public DailyProductionReportItem() {
        hourStatisticsItems = new HashMap<Integer, List<HourStatisticsItem>>();
        for (int i = 0; i < 8; i++) {
            hourStatisticsItems.put(i, new LinkedList<HourStatisticsItem>());
        }
    }
}
