package com.sap.nic.mytaxi.cityaccess;

/**
 * Taxi information.
 * If some of the member is not available, leave it to 0 or null;
 */
public class TaxiInfo {
	/** device ID */
	public String devId;
	/** Car plate number 车牌号码 */
	public String plateNum;

	/** Latitude */
	public double lat;
	/** Longitude */
	public double lng;

	/**
	 * Passenger state
	 * 0 - unknown 未知
	 * 1 - empty 空车
	 * 2 - with passenger(s) 重车
	 */
	public int passengerState;

	// others?
}
