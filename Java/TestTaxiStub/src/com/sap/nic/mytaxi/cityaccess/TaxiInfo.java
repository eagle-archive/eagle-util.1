package com.sap.nic.mytaxi.cityaccess;

/**
 * Taxi information.
 * If some of the member is not available, leave it to 0 or null;
 */
public class TaxiInfo {
	/** device ID */
	String devId;
	/** Car plate number */
	String plateNum;

	/** Latitude */
	double lat;
	/** Longitude */
	double lng;

	/**
	 * Passenger state
	 * 0 - unknown
	 * 1 - empty
	 * 2 - with passenger(s)
	 */
	int passengerState;

	// others?
}
