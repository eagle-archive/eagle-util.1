package com.sap.nic.mytaxi.cityaccess;

/**
 * Taxi information.
 * If some of the member is not available, leave it to 0 or null;
 */
public class TaxiInfo {
	/** device ID */
	public String devId;
	/** Car plate number ���ƺ��� */
	public String plateNum;

	/** Latitude */
	public double lat;
	/** Longitude */
	public double lng;

	/**
	 * Passenger state
	 * 0 - unknown δ֪
	 * 1 - empty �ճ�
	 * 2 - with passenger(s) �س�
	 */
	public int passengerState;

	// others?
}
