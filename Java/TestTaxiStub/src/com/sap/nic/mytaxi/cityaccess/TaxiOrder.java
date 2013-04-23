package com.sap.nic.mytaxi.cityaccess;

/**
 * Taxi order information at city access level.
 */
public class TaxiOrder {
	/** Taxi order status: order request is submitted by user */
    static final int STAUS_SUBMITTED_BY_USER = 1;
	/** Taxi order status: order confirmed by taxi driver */
    static final int STAUS_CONFIRMED_BY_DRIVER = 2;
	/** Taxi order status: no taxi found after search */
    static final int STAUS_NO_TAXI_FOUND = 3;
	/** Taxi order status: order request is cancelled by user */
    static final int STAUS_CANCELLED_BY_USER = 4;
	/** Taxi order status: order request is cancelled by driver */
    static final int STAUS_CANCELLED_BY_DRIVER = 5;
	/** Taxi order status: order request is by driver but taxi not shown after long time */
    static final int STAUS_TAXI_NO_SHOW = 7;
	/** Taxi order status: order completed */
    static final int STAUS_COMPLETED = 8;

	/** Taxi order ID, must be available and unique. */
	String orderId;

	/** Taxi order status code */
	String orderStatusCode;

	/**
	 * Taxi order status description. Could be null.
	 * If order is cancelled, the description is the reason why order is cancelled.
	 */
	String orderStatusDescription;
}
