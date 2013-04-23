package com.sap.nic.mytaxi.cityaccess;

/**
 * Taxi order information at city access level.
 */
public class TaxiOrder {
	/**
	 * Taxi order status: order request is submitted by user
	 * �����Ѿ��ύ�������·�
	 */
	public static final int STATUS_SUBMITTED_BY_USER = 1;
	/**
	 * Taxi order status: order confirmed by taxi driver
	 * �����ѱ�˾��ȷ��
	 */
    public static final int STATUS_CONFIRMED_BY_DRIVER = 2;
	/**
	 * Taxi order status: no taxi found after search
	 * û���ѵ���
	 */
    public static final int STATUS_NO_TAXI_FOUND = 3;
	/**
	 * Taxi order status: taxi found after search but no driver confirm
	 * �ѵ��˳�������û��˾��Ӧ��
	 */
    public static final int STATUS_NO_DRIVER_CONFIRM = 4;
    /**
     * Taxi order status: order request is cancelled by user
     * �������û�ȡ��
     */
    public static final int STATUS_CANCELLED_BY_USER = 5;
	/**
	 * Taxi order status: order request is cancelled by driver
	 * ������˾��ȡ��
	 */
    public static final int STATUS_CANCELLED_BY_DRIVER = 6;
	/**
	 * Taxi order status: order request is by driver but taxi not shown after long time
	 * ˾��Ӧ��ȷ���ˣ����Ǻܾ�û�г���
	 */
    public static final int STATUS_TAXI_NO_SHOW = 7;
	/**
	 * Taxi order status: order completed
	 * �������
	 */
    public static final int STATUS_COMPLETED = 8;

	/**
	 * Taxi order ID, required, must be unique
	 * �����ṩ�̵��г������ţ�������Ч���Ҳ��ظ�
	 */
    public String orderId;

	/** Taxi order status code */
    public String orderStatusCode;

	/**
	 * Taxi order status description. Could be null.
	 * If order is cancelled, the description is the reason why order is cancelled.
	 * �Զ�������orderStatusCode�Ĳ���������Ϣ�����磬������ȡ������������ȡ������
	 */
    public String orderStatusDescription;
}
