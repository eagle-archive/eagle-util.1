package com.sap.nic.mytaxi.cityaccess;

/**
 * This interface gives access to city level taxi calling provider (e.g., Nanjing Tong Yong,
 * Shanghai Da Zhong), who is supposed to implement this interface in Java.
 */
public interface IAccessEntry {
    /**
     * Returns the IAccessFeature interface.
     *
     * @return the IAccessFeature interface.
     */
	public IAccessFeature getAccessFeature();

    /**
     * Create a new taxi calling order.
     *
     * @param latitude, required
     * @param longitude, required
     * @param user ID, required, could be user registered mobile number or some generated ID
     * @param name of the user, can be in different forms, e.g., Miss Zhang, Xiao Wang
     * @param gender of the user, 0 - male, 1 - female, 2 - unknown
     * @param destination latitude, optional, could be 0
     * @param destination longitude, optional, could be 0
     * @param destination description, e.g., some name of the location or generated from map
     *        by manually pick a location. optional
     * @param the user requested car type description, optional
     * @param other user request description, e.g., female driver, optional
     * @param mobile number of the requested user, required
     * @return TaxiOrder object.
     * @throws GeneralAccessException
     */
	public TaxiOrder createOrderReq(double lat, double lng, String userId, String name, int gender,
			double destLat, double destLng, String destDesc, String orderCarType, String otherDesc,
			String reqMobileNum) throws GeneralAccessException; 

    /**
     * Query order status.
     *
     * @param order ID, required
     * @return TaxiOrder object.
     * @throws GeneralAccessException
     */
	public TaxiOrder queryOrderStatus(String orderId)
			throws GeneralAccessException; 

    /**
     * Create a new taxi calling order with enlarged search range.
     *
     * @param latitude, required
     * @param longitude, required
     * @param user ID, required, could be user registered mobile number or some generated ID
     * @param name of the user, can be in different forms, e.g., Miss Zhang, Xiao Wang
     * @param gender of the user, 0 - male, 1 - female, 2 - unknown
     * @param destination latitude, optional, could be 0
     * @param destination longitude, optional, could be 0
     * @param destination description, e.g., some name of the location or generated from map
     *        by manually pick a location. optional
     * @param the user requested car type description, optional
     * @param other user request description, e.g., female driver, optional
     * @param mobile number of the requested user, required
     * @param suggested time of the enlarged range (1-3). The vendor can adjust it to fit its own
     *        system.
     * @return TaxiOrder object.
     * @throws GeneralAccessException
     */
	public TaxiOrder enlargeSearch(double lat, double lng, String userId, String name, int gender,
			double destLat, double destLng, String destDesc, String orderCarType, String otherDesc,
			String reqMobileNum, float enlargeTime) throws GeneralAccessException;

    /**
     * Get the nearby taxis of the specified location.
     *
     * @param latitude, required
     * @param longitude, required
     * @param Suggested radius in meters. If it is left to 0, the vender will decide the radius.
     *        NOTE: the radius is only the suggested parameter. It can be adjusted or ignored.
     * @return array of TaxiInfo object. If some of the member of TaxiInfo is not available,
     *         leave them to null or 0. If not taxi is found, return null.
     * @throws IllegalArgumentException if (lat, lng) or radius is invalid
     */
	public TaxiInfo[] getTaxisNearBy(double lat, double lng, double radius)
			throws IllegalArgumentException;

    /**
     * Get the nearby taxis of the specified location.
     *
     * @param device ID
     * @param Suggested radius in meters. Leave it to 0, the vender will decide the radius.
     *        NOTE: the radius is only the suggested parameter. It can be adjusted or ignored.
     * @return array of TaxiInfo object.
     * 		   If some of the member of TaxiInfo is not available, leave them to null or 0.
     *         If no taxi is found, return null.
     * @throws IllegalArgumentException if devId or radius is invalid
     * @throws DeviceNotFoundException if devId is not found
     */
	public TaxiInfo[] trackTaxiWithNearBy(String devId, double radius) 
			throws IllegalArgumentException, DeviceNotFoundException;

    /**
     * Get the taxi information.
     *
     * @param device ID
     * @return TaxiInfo object.
     * 		   If some of the member of TaxiInfo is not available, leave them to null or 0.
     * @throws IllegalArgumentException if devId or radius is invalid
     * @throws DeviceNotFoundException if devId is valid but not found
     */
	public TaxiInfo trackTaxi(String devId)
			throws IllegalArgumentException, DeviceNotFoundException;

    /**
     * Urge taxi driver go come fast
     *
     * @param order ID, required, order should be in confirmed status
     * @param device ID, required
     * @param name of the user, can be in different forms, e.g., Miss Zhang, Xiao Wang
     * @param gender of the user, 0 - male, 1 - female, 2 - unknown
     * @param description text from the user input from mobile devices
     * @throws IllegalArgumentException if devId is invalid
     * @throws DeviceNotFoundException if devId is valid but not found
     * @throws GeneralAccessException for other internal error, e.g., if the order is 
     *         not in confirmed status 
     */
	public void urgeTaxiDriver(String orderId, String devId, String title, int gender, String text)
			throws IllegalArgumentException, DeviceNotFoundException, GeneralAccessException;

    /**
     * User cancels order
     *
     * @param order ID, required, order should be in confirmed status
     * @param device ID, required
     * @throws IllegalArgumentException if orderId or devId is invalid
     * @throws DeviceNotFoundException if devId is valid but not found
     * @throws GeneralAccessException for other internal error, e.g., if the order is 
     *         not in confirmed status 
     */
	public void cancelOrderByUser(String orderId, String devId)
			throws IllegalArgumentException, DeviceNotFoundException, GeneralAccessException;

    /**
     * User reports the driver does not appear after the a long time 
     *
     * @param order ID, required, order should be in confirmed status
     * @param device ID, required
     * @throws IllegalArgumentException if orderId or devId is invalid
     * @throws DeviceNotFoundException if devId is valid but not found
     * @throws GeneralAccessException for other internal error, e.g., if the order is 
     *         not in confirmed status 
     */
	public void reportDriverNoShown(String orderId, String devId)
			throws IllegalArgumentException, DeviceNotFoundException, GeneralAccessException;

    /**
     * Query order status in batches
     *
     * @param order IDs, required
     * @return TaxiOrder object array in the same size of array orderIds.
     *         If some of the order IDs is invalid or query fails, put null in the corresponding
     *         return array.
     */
	public TaxiOrder[] batchQueryOrderStatus(String[] orderIds);
}
