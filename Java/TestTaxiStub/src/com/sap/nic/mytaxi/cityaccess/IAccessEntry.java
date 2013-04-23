package com.sap.nic.mytaxi.cityaccess;

/**
 * This interface gives access to city level taxi calling provider/vendor (e.g., 南京通用,
 * 上海大众), who is supposed to implement this interface in Java.
 * 
 * 移动招车平台接入接口。该接口应由接入提供商实现。
 */
public interface IAccessEntry {
    /**
     * Returns the IAccessFeature interface.
     *
     * @return the IAccessFeature interface.
     */
	public IAccessFeature getAccessFeature();

    /**
     * Create a new taxi calling order. 用户招车请求。
     *
     * @param lat latitude, required
     * @param lng longitude, required
     * @param userId user ID, required, could be user registered mobile number or some generated ID
     * @param name name of the user, can be in different forms, e.g., 张女士, 小王
     * @param gender gender of the user, 0 - male, 1 - female, 2 - unknown
     * @param destLat destination latitude, optional, could be 0
     * @param destLng destination longitude, optional, could be 0
     * @param destDesc destination description, e.g., some name of the location or generated from map
     *        by manually pick a location. optional
     * @param orderCarType the user requested car type description, optional
     * @param otherDesc other user request description, e.g., female driver, optional
     * @param reqMobileNum mobile number of the requested user, required
     * @return TaxiOrder object.
     * @throws GeneralAccessException
     */
	public TaxiOrder createOrderReq(double lat, double lng, String userId, String name, int gender,
			double destLat, double destLng, String destDesc, String orderCarType, String otherDesc,
			String reqMobileNum) throws GeneralAccessException; 

    /**
     * Query order status.
     *
     * @param orderId order ID, required
     * @return TaxiOrder object.
     * @throws GeneralAccessException
     */
	public TaxiOrder queryOrderStatus(String orderId)
			throws GeneralAccessException; 

    /**
     * Create a new taxi calling order with enlarged search range.
     *
     * @param lat latitude, required
     * @param lng longitude, required
     * @param userId user ID, required, could be user registered mobile number or some generated ID
     * @param name name of the user, can be in different forms, e.g., 张女士, 小王
     * @param gender gender of the user, 0 - male, 1 - female, 2 - unknown
     * @param destLat destination latitude, optional, could be 0
     * @param destLng destination longitude, optional, could be 0
     * @param destDesc destination description, e.g., some name of the location or generated from map
     *        by manually pick a location. optional
     * @param orderCarType the user requested car type description, optional
     * @param otherDesc other user request description, e.g., female driver, optional
     * @param reqMobileNum mobile number of the requested user, required
     * @return TaxiOrder object.
     * @param enlargeTime suggested time of the enlarged range (1-3). The vendor can adjust it to fit its own
     *        system.
     *        建议的招车扩大范围倍数（一般在1-3倍）。接入提供商可以根据自己的系统做相应的映射来调整此参数
     * @return TaxiOrder object.
     * @throws GeneralAccessException
     */
	public TaxiOrder enlargeSearch(double lat, double lng, String userId, String name, int gender,
			double destLat, double destLng, String destDesc, String orderCarType, String otherDesc,
			String reqMobileNum, float enlargeTime) throws GeneralAccessException;

    /**
     * Get the nearby taxis of the specified location.
     *
     * @param lat latitude, required
     * @param lng longitude, required
     * @param radius Suggested radius in meters. If it is left to 0, the vender will decide the radius.
     *        NOTE: the radius is only the suggested parameter. It can be adjusted or ignored.
     *        建议的搜车半径。如果传入0，接入提供商采用自己默认的半径范围。
     *        注意：这个参数仅仅是建议的参数，接入提供商可以根据自身的系统进行调整。
     * @return array of TaxiInfo object. If some of the member of TaxiInfo is not available,
     *         leave them to null or 0. If not taxi is found, return null.
     * @throws IllegalArgumentException if (lat, lng) or radius is invalid
     */
	public TaxiInfo[] getTaxisNearBy(double lat, double lng, double radius)
			throws IllegalArgumentException;

    /**
     * Get the nearby taxis of the specified location.
     *
     * @param devId device ID
     * @param radius Suggested radius in meters. If it is left to 0, the vender will decide the radius.
     *        NOTE: the radius is only the suggested parameter. It can be adjusted or ignored.
     *        建议的搜车半径。如果传入0，接入提供商采用自己默认的半径范围。
     *        注意：这个参数仅仅是建议的参数，接入提供商可以根据自身的系统进行调整。
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
     * @param devId device ID
     * @return TaxiInfo object.
     * 		   If some of the member of TaxiInfo is not available, leave them to null or 0.
     * @throws IllegalArgumentException if devId or radius is invalid
     * @throws DeviceNotFoundException if devId is valid but not found
     */
	public TaxiInfo trackTaxi(String devId)
			throws IllegalArgumentException, DeviceNotFoundException;

    /**
     * Urge taxi driver go come fast。催促（提醒）司机快点过来。
     *
     * @param orderId order ID, required, order should be in confirmed status
     * @param devId device ID, required
     * @param title name of the user, can be in different forms, e.g., Miss Zhang, Xiao Wang
     *        名字，称呼，等等
     * @param gender gender of the user, 0 - male, 1 - female, 2 - unknown
     * @param text description text from the user input from mobile devices
     *        用户从移动终端输入的催车内容
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
     * @param orderId order ID, required, order should be in confirmed status
     * @param devId device ID, required
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
     * @param orderId order ID, required, order should be in confirmed status
     * @param devId device ID, required
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
     * @param orderIds order ID array, required
     * @return TaxiOrder object array in the same size of array orderIds.
     *         If some of the order IDs is invalid or query fails, put null in the corresponding
     *         return array.
     */
	public TaxiOrder[] batchQueryOrderStatus(String[] orderIds);
}
