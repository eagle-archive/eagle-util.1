package com.sap.nic.mytaxi.cityaccess;

/**
 * Taxi order information at city access level.
 */
public class TaxiOrder {
	/**
	 * Taxi order status: order request is submitted by user
	 * 订单已经提交，正在下发
	 */
	public static final int STATUS_SUBMITTED_BY_USER = 1;
	/**
	 * Taxi order status: order confirmed by taxi driver
	 * 订单已被司机确认
	 */
    public static final int STATUS_CONFIRMED_BY_DRIVER = 2;
	/**
	 * Taxi order status: no taxi found after search
	 * 没有搜到车
	 */
    public static final int STATUS_NO_TAXI_FOUND = 3;
	/**
	 * Taxi order status: taxi found after search but no driver confirm
	 * 搜到了车，但是没有司机应答
	 */
    public static final int STATUS_NO_DRIVER_CONFIRM = 4;
    /**
     * Taxi order status: order request is cancelled by user
     * 订单被用户取消
     */
    public static final int STATUS_CANCELLED_BY_USER = 5;
	/**
	 * Taxi order status: order request is cancelled by driver
	 * 订单被司机取消
	 */
    public static final int STATUS_CANCELLED_BY_DRIVER = 6;
	/**
	 * Taxi order status: order request is by driver but taxi not shown after long time
	 * 司机应答确认了，但是很久没有出现
	 */
    public static final int STATUS_TAXI_NO_SHOW = 7;
	/**
	 * Taxi order status: order completed
	 * 订单完成
	 */
    public static final int STATUS_COMPLETED = 8;

	/**
	 * Taxi order ID, required, must be unique
	 * 接入提供商的招车订单号，必须有效而且不重复
	 */
    public String orderId;

	/** Taxi order status code */
    public String orderStatusCode;

	/**
	 * Taxi order status description. Could be null.
	 * If order is cancelled, the description is the reason why order is cancelled.
	 * 对订单代码orderStatusCode的补充描述信息。例如，订单被取消，这里填入取消理由
	 */
    public String orderStatusDescription;
}
